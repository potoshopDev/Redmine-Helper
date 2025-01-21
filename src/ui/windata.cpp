#include "windata.h"

namespace helper
{

void draw(const WindowFront& window, helper::WindowData& winData)
{
    ImGui::Begin(window.title.c_str());  // ������ ����
    window.contentRenderer();            // ���������� ���������� ����
    ImGui::End();                        // ��������� ��������� ����
}

void draw(DemoWindowFront& window, helper::WindowData& winData)
{
    if (window.showDemoWindow) window.contentRenderer(window.showDemoWindow);  // ���������� ���������� ����
}
}  // namespace helper
