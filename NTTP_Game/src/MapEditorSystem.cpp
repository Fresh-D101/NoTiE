#include <fstream>
#include "MapEditorSystem.h"

#include <iostream>

#include "Core/Log.h"
#include "Core/Utils.h"
#include "Graphics/RenderSystem.h"
#include "Iso/IsoEngine.h"
#include "ImGui/imgui-NoTiE.h"

bool MapEditorSystem::showEditor = true;
bool MapEditorSystem::showLog = true;
bool MapEditorSystem::showDemo = false;
bool MapEditorSystem::showInfo = true;
int MapEditorSystem::selectedTileId = 0;
std::vector<std::string> MapEditorSystem::TileNames;
ImGui::FileBrowser MapEditorSystem::saveDialog = ImGui::FileBrowser(ImGuiFileBrowserFlags_EnterNewFilename | ImGuiFileBrowserFlags_CreateNewDir);
ImGui::FileBrowser MapEditorSystem::loadDialog;

MapEditorSystem::MapEditorSystem(::IsoEngine& isoEngine) : IsoEngine(isoEngine)
{
	AddRequiredComponent<NoEntitiesAllowedTag>();

	LoadTileNames("/graphics");

	saveDialog.SetTitle("Save Map");
	saveDialog.SetTypeFilters({ ".ntm" });
	loadDialog.SetTitle("Load Map");
	loadDialog.SetTypeFilters({ ".ntm" });
}

void MapEditorSystem::Update()
{	
	ShowMainMenuBar();
	if (showEditor) ShowEditorWindow(&showEditor);
	if (showLog) notie::Log::DrawConsole(&showLog);
	if (showDemo) ImGui::ShowDemoWindow(&showDemo);
	if (showInfo) ShowIsoInfo(&showInfo);

	ManageFileDialogs();
}

void MapEditorSystem::ShowMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Load Map.."))
			{
				loadDialog.Open();
			}
			
			if (ImGui::MenuItem("Save Map.."))
			{
				saveDialog.Open();
			}
	
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Map Editor")) showEditor = true;
			if (ImGui::MenuItem("Log Console")) showLog = true;
			if (ImGui::MenuItem("Iso Info")) showInfo = true;
			if (ImGui::MenuItem("DearImGui Demo")) showDemo = true;
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void MapEditorSystem::ShowEditorWindow(bool* p_open)
{
	if (!ImGui::Begin("Map Editor", p_open))
	{
		ImGui::End();
		return;
	}
	
	ImGui::TextWrapped("Select a Tile!");

	ImGui::Columns(4);
	ImGui::Separator();
	for (int i = 0; i < TileNames.size(); i++)
	{
		ImGui::PushID(i);
		int frame_padding = -1 + i;     // -1 = uses default padding
		if (ImGui::ImageButton(Sprite(TileNames.at(i)), Vector2Float(64, 32)))
		{
			selectedTileId = i + 1;
		}
		ImGui::PopID();
		ImGui::NextColumn();
	}

	ImGui::End();
}

void MapEditorSystem::ShowIsoInfo(bool* p_open)
{
	if (!ImGui::Begin("Isometric Engine Info", p_open))
	{
		ImGui::End();
		return;
	}

	auto mousePos = Input::GetMousePositionInWindow(*RenderSystem::GetActiveWindow());
	auto Tile = IsoEngine.GetTilePicker()->GetSelectedTile();
	
	ImGui::TextWrapped("Mouse Position: (%d, %d)", mousePos.x, mousePos.y);
	ImGui::TextWrapped("Seleced Tile: (%d, %d)", Tile.x, Tile.y);

	ImGui::End();
}

void MapEditorSystem::LoadTileNames(const std::string& tilePath)
{
	std::filesystem::directory_iterator iterator(Utils::GetWorkingDirectory() + tilePath);
	std::filesystem::directory_iterator end;

	int id = 1;
	
	while (iterator != end)
	{
		if(!iterator->is_directory())
		{
			std::string fileName = iterator->path().stem().string();

			auto tile = std::make_shared<TileInfo>();
			tile->Id = id;
			tile->Name = fileName + " Tile";
			tile->Texture = fileName;
			tile->Sprite.SetTexture(tile->Texture);

			IsoEngine.GetMap()->AddTileInfo(tile);

			TileNames.push_back(fileName);
		}
		
		std::error_code ec;
		iterator.increment(ec);
		if (ec)
		{
			std::cerr << "Error While Accessing : " << iterator->path().string() << " :: " << ec.message() << std::endl;
		}

		id++;
	}
}

void MapEditorSystem::OnMousePressed(MouseButton button)
{
	auto picker = IsoEngine.GetTilePicker();
	if (button == MouseButton::Left && picker->IsInMapBounds())
	{
		auto selected = picker->GetSelectedTile();
		IsoEngine.GetMap()->SetTileAt(selected, selectedTileId);
		NTE_TRACE("Changed Tile {0}, {1}", selected.x, selected.y);
	}
}

void MapEditorSystem::ManageFileDialogs()
{
	saveDialog.Display();

	if(saveDialog.HasSelected())
	{
		//Save Map
		auto path = saveDialog.GetSelected().string();

		if(path.find(".ntm") == std::string::npos)
		{
			path.append(".ntm");
		}

		std::ofstream fileStream(path, std::ios::out | std::ios::binary);
		IsoEngine.GetMap()->Serialize(fileStream);
		fileStream.close();
		NTE_INFO("Map successfully saved to: {0}", path);
		saveDialog.ClearSelected();
	}

	loadDialog.Display();

	if(loadDialog.HasSelected())
	{
		auto path = loadDialog.GetSelected().string();

		std::ifstream fileStream(path, std::ios::in | std::ios::binary);
		IsoEngine.GetMap()->Load(fileStream);
		if(!fileStream)
		{
			NTE_ERROR("Loading Map failed!");
		}
		else
		{
			NTE_INFO("Loaded Map successfully");
		}
		
		fileStream.close();
		loadDialog.ClearSelected();
	}
}

void MapEditorSystem::OnKeyPressed(Key key)
{
	if (key == Key::E)
	{
		NTE_CORE_ERROR("Renderer could not be initialized!");

	}

	if (key == Key::D)
	{
		NTE_DEBUG("Key was Pressed: 'D'");
	}

	if (key == Key::M)
	{
		NTE_INFO("Loaded Map successfully");
	}
}