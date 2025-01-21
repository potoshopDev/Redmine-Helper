#pragma once

#include <functional>
#include <string>
#include <imgui.h>

namespace helper
{

struct WindowData
{
    std::string title;                      // Заголовок окна
    std::function<void()> contentRenderer;  // Функция для отрисовки содержимого окна
};

struct DemoWindowData
{
    bool showDemoWindow{true};
    std::function<void(bool&)> contentRenderer;  // Функция для отрисовки содержимого окна
};

void draw(const WindowData& window)
{
    ImGui::Begin(window.title.c_str());  // Создаём окно
    window.contentRenderer();            // Отображаем содержимое окна
    ImGui::End();                        // Завершаем отрисовку окна
}

void draw(DemoWindowData& window)
{
    if (window.showDemoWindow) window.contentRenderer(window.showDemoWindow);  // Отображаем содержимое окна
}
}  // namespace helper
