#include "windata.h"

namespace helper
{

void draw(const WindowFront& window, helper::WindowData& winData)
{
    ImGui::Begin(window.title.c_str());  // Создаём окно
    window.contentRenderer();            // Отображаем содержимое окна
    ImGui::End();                        // Завершаем отрисовку окна
}

void draw(DemoWindowFront& window, helper::WindowData& winData)
{
    if (window.showDemoWindow) window.contentRenderer(window.showDemoWindow);  // Отображаем содержимое окна
}
}  // namespace helper
