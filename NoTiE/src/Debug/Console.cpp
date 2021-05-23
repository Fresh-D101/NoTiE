#include "Console.h"


#include "../Graphics/Color.h"
#include "../ImGui/imgui-NoTiE.h"

Console::Console()
{
	AutoScroll = true;
	Clear();
}

Console::~Console()
{
	
}

void Console::Clear()
{
	Buffer.clear();
    Entries.clear();
}

void Console::AddLog(const char* fmt, const Color& color, ...)
{
    int start = Buffer.empty() ? 0 : Buffer.size();
	va_list args;
	va_start(args, fmt);
	Buffer.appendfv(fmt, args);
	va_end(args);
    Entries.push_back({ start, Buffer.size(), color });
}

void Console::Draw(const char* title, bool* p_open)
{
	if (!ImGui::Begin(title, p_open))
	{
		ImGui::End();
		return;
	}

	// Options menu
	if (ImGui::BeginPopup("Options"))
	{
		ImGui::Checkbox("Auto-scroll", &AutoScroll);
		ImGui::EndPopup();
	}

    // Main window
    if (ImGui::Button("Options"))
        ImGui::OpenPopup("Options");
    ImGui::SameLine();
    bool clear = ImGui::Button("Clear");
    ImGui::SameLine();
    bool copy = ImGui::Button("Copy");
    ImGui::SameLine();
    Filter.Draw("Filter", -100.0f);

    ImGui::Separator();
    ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

    if (clear)
        Clear();
    if (copy)
        ImGui::LogToClipboard();

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 4));
    const char* buf = Buffer.begin();
    const char* buf_end = Buffer.end();
    if (Filter.IsActive())
    {
        for (int entry = 0; entry < Entries.Size; entry++)
        {
            const char* BufferStart = buf + Entries[entry].BufferStart;
            const char* BufferEnd = buf + Entries[entry].BufferEnd;
            if (Filter.PassFilter(BufferStart, BufferEnd))
            {
                ImGui::PushStyleColor(ImGuiCol_Text, Entries[entry].color);
                ImGui::TextUnformatted(BufferStart, BufferEnd);
                ImGui::PopStyleColor();
            }
        }
    }
    else
    {
        for (int entry = 0; entry < Entries.Size; entry++)
        {
            const char* BufferStart = buf + Entries[entry].BufferStart;
            const char* BufferEnd = buf + Entries[entry].BufferEnd;
			ImGui::PushStyleColor(ImGuiCol_Text, Entries[entry].color);
			ImGui::TextUnformatted(BufferStart, BufferEnd);
			ImGui::PopStyleColor();
        }
    }
    ImGui::PopStyleVar();

    if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

    ImGui::EndChild();
    ImGui::End();
}