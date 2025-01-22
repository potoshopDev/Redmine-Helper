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

void setBool(WindowData& wd, const std::string& key, const bool value) noexcept
{
    wd._self->_set(key, value);
}
void setFloat(WindowData& wd, const std::string& key, const float value) noexcept
{
    wd._self->_set(key, value);
}
void setString(WindowData& wd, const std::string& key, const std::string& value) noexcept
{
    wd._self->_set(key, value);
}
void setFunction(WindowData& wd, const std::string& key, voidFunc value) noexcept
{
    wd._self->_set(key, value);
}

bool getBool(const WindowData& wd, const std::string& key) noexcept
{
    return wd._self->_get<bool>(key);
}
float getFloat(const WindowData& wd, const std::string& key) noexcept
{
    return wd._self->_get<float>(key);
}
std::string getString(const WindowData& wd, const std::string& key) noexcept
{
    return wd._self->_get<std::string>(key);
}
voidFunc getFunction(const WindowData& wd, const std::string& key) noexcept
{
    return wd._self->_get<voidFunc>(key);
}
}  // namespace helper
