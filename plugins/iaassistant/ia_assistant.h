// IA Assistant Plugin - Header
// AI-powered table assistant for Visual Pinball X

#pragma once

#include <windows.h>
#include <string>

// Niveaux de difficulté
enum difficulty_level {
    DIFFICULTY_BEGINNER = 0,
    DIFFICULTY_INTERMEDIATE = 1,
    DIFFICULTY_EXPERT = 2
};

// Fonctions exportées du plugin
#ifdef __cplusplus
extern "C" {
#endif

// Informations du plugin
__declspec(dllexport) int VPinballPluginInfo();
__declspec(dllexport) int GetPluginVersion();
__declspec(dllexport) const char* GetPluginName();
__declspec(dllexport) const char* GetPluginDescription();

// Cycle de vie
__declspec(dllexport) void InitializePlugin(HWND hwnd_parent);
__declspec(dllexport) void ProcessPlugin();
__declspec(dllexport) void ShutdownPlugin();

// Interface utilisateur
__declspec(dllexport) void ShowOptionsDialog(HWND hwnd_parent);

// Fonctions IA
__declspec(dllexport) void AnalyzeCurrentTable();
__declspec(dllexport) void GenerateTable(const char* theme, difficulty_level difficulty);

// Fonctions utilitaires
std::string GetVPinballTablesPath();
std::string GetCurrentTableName();
void ApplyGeneratedElements(const nlohmann::json& elements);

// Helpers
INT_PTR CALLBACK OptionsDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
std::string GetEditText(HWND hwnd, int ctrl_id);
void SetEditText(HWND hwnd, int ctrl_id, const std::string& text);

#ifdef __cplusplus
}
#endif