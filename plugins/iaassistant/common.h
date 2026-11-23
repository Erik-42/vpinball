// IA Assistant Plugin - Common definitions
// AI-powered table assistant for Visual Pinball X

#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <nlohmann/json.hpp>

namespace iaassistant {

// Configuration du plugin
struct Config {
    std::string api_provider;     // "local", "freellmapi", "fcc", "openai", etc.
    std::string api_key;
    std::string api_base_url;
    int max_tokens = 2048;
    double temperature = 0.7;
    int timeout_seconds = 30;

    static Config load(const std::string& config_path);
};

// Éléments de tableau
struct TableElement {
    enum class Type {
        BUMPER,
        FLIPPER,
        TARGET,
        DRAIN,
        SILLOSCOPE,
        LIGHT,
        PRALL,
        TRIGGER,
        SILL,
        UNDEFINED
    };

    Type type;
    std::string name;
    float x, y, z;
    float rotation;
    std::map<std::string, std::string> properties;
};

// Table analisée
struct TableAnalysis {
    std::string filename;
    std::string version;
    std::vector<TableElement> elements;
    std::string physics_config;
    std::vector<std::string> scripts;
};

// Résultat d'opération IA
struct AIResult {
    bool success;
    std::string response;
    std::string error_message;
    nlohmann::json metadata;
};

// Options de génération
struct GenerationOptions {
    enum class Difficulty {
        BEGINNER,
        INTERMEDIATE,
        EXPERT
    };

    std::string manufacturer = "Custom";
    std::string theme = "Generic";
    Difficulty difficulty = Difficulty::INTERMEDIATE;
    bool include_scripts = true;
    bool include_music = false;
};

// Interface client IA
class IClientAI {
public:
    virtual ~IClientAI() = default;

    virtual AIResult call(const std::string& prompt) = 0;
    virtual AIResult analyze_table(const TableAnalysis& analysis) = 0;
    virtual AIResult generate_elements(const GenerationOptions& options) = 0;
    virtual AIResult suggest_improvements(const TableAnalysis& analysis) = 0;

    virtual bool is_configured() const = 0;
    virtual std::string provider_name() const = 0;
};

// Factory pour créer les clients IA
std::unique_ptr<IClientAI> create_client(const Config& config);

// Messages d'erreur standardisés
constexpr const char* ERR_NOT_CONFIGURED = "IA client not configured";
constexpr const char* ERR_CONNECTION_FAILED = "Failed to connect to AI service";
constexpr const char* ERR_INVALID_API_KEY = "Invalid API key";
constexpr const char* ERR_TIMEOUT = "Request timeout";

} // namespace iaassistant