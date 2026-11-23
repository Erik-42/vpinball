// IA Assistant Plugin - AI Client implementations
// Support for multiple AI providers (local, freellmapi, fcc, openai)

#include "ia_assistant.h"
#include "common.h"
#include <fstream>
#include <sstream>
#include <curl/curl.h>
#include <ctime>

namespace iaassistant {

// Callback pour libcurl
static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Client de base pour API HTTP
class HttpAIClient : public IClientAI {
protected:
    Config config_;
    CURL* curl_;

    std::string http_post(const std::string& url, const std::string& data) {
        std::string read_buffer;
        struct curl_slist* headers = nullptr;

        curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl_, CURLOPT_POST, 1L);
        curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &read_buffer);
        curl_easy_setopt(curl_, CURLOPT_TIMEOUT, config_.timeout_seconds);

        // Headers JSON
        headers = curl_slist_append(headers, "Content-Type: application/json");
        if (!config_.api_key.empty()) {
            std::string auth_header = "Authorization: Bearer " + config_.api_key;
            headers = curl_slist_append(headers, auth_header.c_str());
        }
        curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers);

        CURLcode res = curl_easy_perform(curl_);
        curl_slist_free_all(headers);

        if (res != CURLE_OK) {
            return "";
        }

        return read_buffer;
    }

public:
    HttpAIClient(const Config& config) : config_(config) {
        curl_ = curl_easy_init();
        curl_global_init(CURL_GLOBAL_DEFAULT);
    }

    ~HttpAIClient() override {
        if (curl_) {
            curl_easy_cleanup(curl_);
        }
        curl_global_cleanup();
    }

    bool is_configured() const override {
        return !config_.api_base_url.empty() && !config_.api_key.empty();
    }
};

// Client FreeLlmApi
class FreeLlmApiClient : public HttpAIClient {
public:
    FreeLlmApiClient(const Config& config) : HttpAIClient(config) {
        if (config_.api_base_url.empty()) {
            config_.api_base_url = "https://api.freellmapi.com/v1";
        }
    }

    std::string provider_name() const override {
        return "freellmapi";
    }

    AIResult call(const std::string& prompt) override {
        AIResult result;

        if (!is_configured()) {
            result.success = false;
            result.error_message = ERR_NOT_CONFIGURED;
            return result;
        }

        nlohmann::json payload = {
            {"model", "llama3.1:latest"},
            {"messages", {{{"role", "user"}, {"content", prompt}}}},
            {"max_tokens", config_.max_tokens},
            {"temperature", config_.temperature}
        };

        std::string response = http_post(config_.api_base_url + "/chat/completions",
                                         payload.dump());

        if (response.empty()) {
            result.success = false;
            result.error_message = ERR_CONNECTION_FAILED;
            return result;
        }

        try {
            auto json_resp = nlohmann::json::parse(response);
            if (json_resp.contains("choices") && !json_resp["choices"].empty()) {
                result.success = true;
                result.response = json_resp["choices"][0]["message"]["content"];
                result.metadata = json_resp;
            } else {
                result.success = false;
                result.error_message = "Invalid response format";
            }
        } catch (const std::exception& e) {
            result.success = false;
            result.error_message = std::string("Parse error: ") + e.what();
        }

        return result;
    }

    AIResult analyze_table(const TableAnalysis& analysis) override {
        std::ostringstream prompt;
        prompt << "Analyze this Visual Pinball X table and suggest improvements:\n\n";
        prompt << "Filename: " << analysis.filename << "\n";
        prompt << "Elements found: " << analysis.elements.size() << "\n";

        for (const auto& elem : analysis.elements) {
            prompt << "- " << static_cast<int>(elem.type) << " : " << elem.name
                   << " at (" << elem.x << ", " << elem.y << ")\n";
        }

        return call(prompt.str());
    }

    AIResult generate_elements(const GenerationOptions& options) override {
        std::ostringstream prompt;
        prompt << "Generate a complete Visual Pinball X table with these specs:\n";
        prompt << "- Manufacturer: " << options.manufacturer << "\n";
        prompt << "- Theme: " << options.theme << "\n";
        prompt << "- Difficulty: " << static_cast<int>(options.difficulty) << "\n";
        prompt << "\nReturn as JSON with elements array containing bumpers, flippers, targets, drains.\n";

        AIResult result = call(prompt.str());
        if (result.success) {
            try {
                result.metadata = nlohmann::json::parse(result.response);
            } catch (...) {
                // Ignore parse errors, keep raw response
            }
        }
        return result;
    }

    AIResult suggest_improvements(const TableAnalysis& analysis) override {
        std::ostringstream prompt;
        prompt << "Suggest specific improvements for this pinball table:\n";
        prompt << "Provide a JSON array of improvements, each with 'type', 'element', 'suggestion', and 'priority' (high/medium/low).\n";

        for (const auto& elem : analysis.elements) {
            prompt << "- Element " << elem.name << " : type=" << static_cast<int>(elem.type) << "\n";
        }

        return call(prompt.str());
    }
};

// Client FCC (Free Code Camp)
class FccApiClient : public HttpAIClient {
public:
    FccApiClient(const Config& config) : HttpAIClient(config) {
        if (config_.api_base_url.empty()) {
            config_.api_base_url = "https://api.fcc.dev";
        }
    }

    std::string provider_name() const override {
        return "fcc";
    }

    AIResult call(const std::string& prompt) override {
        AIResult result;

        if (!is_configured()) {
            result.success = false;
            result.error_message = ERR_NOT_CONFIGURED;
            return result;
        }

        nlohmann::json payload = {
            {"prompt", prompt},
            {"max_tokens", config_.max_tokens},
            {"temperature", config_.temperature}
        };

        std::string response = http_post(config_.api_base_url + "/generate",
                                         payload.dump());

        if (response.empty()) {
            result.success = false;
            result.error_message = ERR_CONNECTION_FAILED;
            return result;
        }

        result.success = true;
        result.response = response;
        return result;
    }

    AIResult analyze_table(const TableAnalysis& analysis) override {
        return call("Analyze pinball table: " + analysis.filename);
    }

    AIResult generate_elements(const GenerationOptions& options) override {
        return call("Generate pinball table: " + options.theme);
    }

    AIResult suggest_improvements(const TableAnalysis& analysis) override {
        return call("Suggest pinball improvements");
    }
};

// Client local (Ollama ou modèle local)
class LocalAIClient : public IClientAI {
private:
    Config config_;
    std::string local_model_ = "llama3.1:latest";

public:
    LocalAIClient(const Config& config) : config_(config) {
        if (!config_.api_base_url.empty()) {
            // Utiliser l'URL spécifiée (Ollama par défaut)
        } else {
            config_.api_base_url = "http://localhost:11434";
        }
    }

    std::string provider_name() const override {
        return "local";
    }

    bool is_configured() const override {
        return true; // Les mods locaux sont toujours disponibles si Ollama tourne
    }

    AIResult call(const std::string& prompt) override {
        AIResult result;

        // Implémentation simple sans libcurl pour local
        // Dans une version complète, utiliser curl ou système de fichiers
        result.success = false;
        result.error_message = "Local AI client not fully implemented - requires Ollama API integration";

        return result;
    }

    AIResult analyze_table(const TableAnalysis& analysis) override {
        return call("Analyze table: " + analysis.filename);
    }

    AIResult generate_elements(const GenerationOptions& options) override {
        return call("Generate pinball table: " + options.theme);
    }

    AIResult suggest_improvements(const TableAnalysis& analysis) override {
        return call("Suggest improvements");
    }
};

// Factory function
std::unique_ptr<IClientAI> create_client(const Config& config) {
    if (config.api_provider == "freellmapi" || config.api_provider == "ollama") {
        return std::make_unique<FreeLlmApiClient>(config);
    } else if (config.api_provider == "fcc") {
        return std::make_unique<FccApiClient>(config);
    } else if (config.api_provider == "local") {
        return std::make_unique<LocalAIClient>(config);
    } else {
        // Par défaut, essayer freellmapi
        return std::make_unique<FreeLlmApiClient>(config);
    }
}

} // namespace iaassistant