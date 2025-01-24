#include "windata.h"

constexpr const char* titleDemoWindow{"Демо окно"};
constexpr const char* titleSimpleWindow{"Простое окно"};

namespace helper
{
autoUpdater<bool> madeAutoBool(WindowData& wd, const std::string_view key)
{
    return {wd, key};
}

autoUpdater<float> madeAutoFloat(WindowData& wd, const std::string_view key)
{
    return {wd, key};
}

autoUpdater<voidFunc> madeAutoFunc(WindowData& wd, const std::string_view key)
{
    return {wd, key};
}

autoUpdater<std::string> madeAutoString(WindowData& wd, const std::string_view key)
{
    return {wd, key};
}

void draw(const WindowFront& window, helper::WindowData& winData)
{
    static bool s{true};
    if (s)
    {
        helper::setBool(winData, titleDemoWindow, true);
        s = false;
    }
    ImGui::Begin(window.title.c_str());
    window.contentRenderer();
    ImGui::End();
}

void draw(DemoWindowFront& window, helper::WindowData& winData)
{
    auto showDemoWin{madeAutoBool(winData, titleDemoWindow)};
    if (showDemoWin.data) window.contentRenderer(showDemoWin.data);
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
