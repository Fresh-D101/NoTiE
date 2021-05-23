#pragma once
#include <string>

#include "imgui.h"

struct Console
{
	struct Entry
	{
		int BufferStart;
		int BufferEnd;
		Color color;
	};
	
	Console();
	~Console();

	void Clear();
	void AddLog(const char* fmt, const Color& color, ...);
	void Draw(const char* title, bool* p_open = nullptr);
	
	ImGuiTextBuffer Buffer;
	ImGuiTextFilter Filter;
	ImVector<Entry> Entries;
	bool AutoScroll;
};
