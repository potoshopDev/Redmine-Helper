#pragma once

#include <functional>
#include <string>
#include <imgui.h>

namespace helper
{

struct WindowData
{
    std::string title;                      // ��������� ����
    std::function<void()> contentRenderer;  // ������� ��� ��������� ����������� ����
};

struct DemoWindowData
{
    bool showDemoWindow{true};
    std::function<void(bool&)> contentRenderer;  // ������� ��� ��������� ����������� ����
};

void draw(const WindowData& window)
{
    ImGui::Begin(window.title.c_str());  // ������ ����
    window.contentRenderer();            // ���������� ���������� ����
    ImGui::End();                        // ��������� ��������� ����
}

void draw(DemoWindowData& window)
{
    if (window.showDemoWindow) window.contentRenderer(window.showDemoWindow);  // ���������� ���������� ����
}
}  // namespace helper
