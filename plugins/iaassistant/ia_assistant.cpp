// IA Assistant Plugin - Main entry point for Visual Pinball X
// AI-powered table assistant

#include "ia_assistant.h"
#include "common.h"
#include <plugin.h>
#include <vpsupport.h>
#include <commctrl.h>
#include <string>
#include <memory>
#include <fstream>

// Version du plugin
#define PLUGIN_VERSION "1.0.0"
#define PLUGIN_NAME "IA Assistant"

struct IAAssistantPlugin {
    std::unique_ptr<iaassistant::Config> config;
    std::unique_ptr<iaassistant::IClientAI> client;
    HWND hwnd_dialog;
};

static IAAssistantPlugin* g_plugin = nullptr;

// Fonctions principales du plugin

extern "C" __declspec(dllexport) int VPinballPluginInfo()
{
    return 0;
}

extern "C" __declspec(dllexport) int GetPluginVersion()
{
    return 1;  // Version du plugin
}

extern "C" __declspec(dllexport) const char* GetPluginName()
{
    return PLUGIN_NAME;
}

extern "C" __declspec(dllexport) const char* GetPluginDescription()
{
    return "AI-powered assistant for pinball table creation and editing";
}

extern "C" __declspec(dllexport) void InitializePlugin(HWND hwnd_parent)
{
    g_plugin = new IAAssistantPlugin();

    // Charger la configuration
    std::string config_path = GetVPinballTablesPath();
    config_path += "/iaassistant/config.json";

    if (std::filesystem::exists(config_path)) {
        g_plugin->config = std::make_unique<iaassistant::Config>(
            iaassistant::Config::load(config_path)
        );
    } else {
        g_plugin->config = std::make_unique<iaassistant::Config>();
        g_plugin->config->api_provider = "freellmapi";
    }

    // Créer le client IA
    g_plugin->client = iaassistant::create_client(*g_plugin->config);
}

extern "C" __declspec(dllexport) void ProcessPlugin()
{
    // Point d'extension principal - appelé chaque frame
    // Peut être utilisé pour des opérations asynchrones
}

extern "C" __declspec(dllexport) void ShutdownPlugin()
{
    if (g_plugin) {
        delete g_plugin;
        g_plugin = nullptr;
    }
}

// Interface utilisateur - fenêtre de dialogue
extern "C" __declspec(dllexport) void ShowOptionsDialog(HWND hwnd_parent)
{
    if (!g_plugin) return;

    // Afficher le dialogue de configuration
    DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_OPTIONS), hwnd_parent, OptionsDialogProc);
}

extern "C" __declspec(dllexport) void AnalyzeCurrentTable()
{
    if (!g_plugin || !g_plugin->client) return;

    // Analyser la table active
    iaassistant::TableAnalysis analysis;
    analysis.filename = GetCurrentTableName();

    auto result = g_plugin->client->analyze_table(analysis);

    if (result.success) {
        MessageBox(NULL,
            ("Analysis result:\n" + result.response).c_str(),
            "Table Analysis",
            MB_OK | MB_ICONINFORMATION);
    }
}

extern "C" __declspec(dllexport) void GenerateTable(const char* theme, difficulty_level difficulty)
{
    if (!g_plugin || !g_plugin->client) return;

    iaassistant::GenerationOptions options;
    options.theme = theme ? theme : "Generic";

    switch (difficulty) {
        case DIFFICULTY_BEGINNER:
            options.difficulty = iaassistant::GenerationOptions::Difficulty::BEGINNER;
            break;
        case DIFFICULTY_EXPERT:
            options.difficulty = iaassistant::GenerationOptions::Difficulty::EXPERT;
            break;
        default:
            options.difficulty = iaassistant::GenerationOptions::Difficulty::INTERMEDIATE;
            break;
    }

    auto result = g_plugin->client->generate_elements(options);

    if (result.success) {
        // Appliquer les éléments générés à la table
        ApplyGeneratedElements(result.metadata);
    }
}

// Fonctions utilitaires

std::string GetVPinballTablesPath()
{
    // Retourne le chemin des dossiers tables
    char path[MAX_PATH];
    SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path);
    return std::string(path) + "/VPinball/Tables";
}

std::string GetCurrentTableName()
{
    // Retourne le nom de la table actuellement chargée
    return "current_table.vpx";
}

void ApplyGeneratedElements(const nlohmann::json& elements)
{
    // Applique les éléments IA générés à la table en cours
    // Cette fonction modifierait le .vpx en temps réel
}

// Dialogue d'options
INT_PTR CALLBACK OptionsDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_INITDIALOG:
            // Initialiser les contrôles avec la config
            SetEditText(hwnd, IDC_EDIT_API_PROVIDER, g_plugin->config->api_provider.c_str());
            return (INT_PTR)TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK) {
                // Sauvegarder la configuration
                iaassistant::Config new_config;
                new_config.api_provider = GetEditText(hwnd, IDC_EDIT_API_PROVIDER);
                new_config.api_key = GetEditText(hwnd, IDC_EDIT_API_KEY);

                // Sauvegarder dans fichier
                std::ofstream out("config.json");
                out << nlohmann::json(*new_config);
                out.close();

                EndDialog(hwnd, IDOK);
                return (INT_PTR)TRUE;
            }
            if (LOWORD(wParam) == IDCANCEL) {
                EndDialog(hwnd, IDCANCEL);
                return (INT_PTR)TRUE;
            }
            break;
    }
    return (INT_PTR)FALSE;
}

std::string GetEditText(HWND hwnd, int ctrl_id)
{
    char buffer[1024];
    GetDlgItemText(hwnd, ctrl_id, buffer, sizeof(buffer));
    return std::string(buffer);
}

void SetEditText(HWND hwnd, int ctrl_id, const std::string& text)
{
    SetDlgItemText(hwnd, ctrl_id, text.c_str());
}