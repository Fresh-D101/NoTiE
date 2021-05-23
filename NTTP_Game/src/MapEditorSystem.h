#pragma once
#include "Core/System.h"
#include <ImGui/imgui.h>
#include "ImGui/imgui-filebrowser.h"

class IsoEngine;

class MapEditorSystem : public System
{
public:
	MapEditorSystem(IsoEngine& isoEngine);
	
	void Update() override;
	void OnMousePressed(MouseButton button) override;
	void OnKeyPressed(Key key) override;

private:
	static void ShowMainMenuBar();
	static void ShowEditorWindow(bool* p_open);
	void ShowIsoInfo(bool* p_open);
	void LoadTileNames(const std::string& tilePath);
	void ManageFileDialogs();
	
	::IsoEngine& IsoEngine;
	static bool showEditor;
	static bool showLog;
	static bool showDemo;
	static bool showInfo;
	static int selectedTileId;
	static std::vector<std::string> TileNames;
	static ImGui::FileBrowser saveDialog;
	static ImGui::FileBrowser loadDialog;
};
