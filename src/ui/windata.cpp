#include "windata.h"
#include <format>

namespace helper
{
autoUpdater<bool> madeAutoBool(WindowData& wd, const std::string key)
{
    return {wd, key};
}

autoUpdater<float> madeAutoFloat(WindowData& wd, const std::string key)
{
    return {wd, key};
}

autoUpdater<voidFunc> madeAutoFunc(WindowData& wd, const std::string key)
{
    return {wd, key};
}

autoUpdater<std::string> madeAutoString(WindowData& wd, const std::string key)
{
    return {wd, key};
}

void draw(WindowFront* window)
{
    if (window) window->Run();
}

void draw(DemoWindowFront& window)
{
    // auto showDemoWin{madeAutoBool(winData, titleDemoWindow)};
    // if (showDemoWin.data) window.contentRenderer(showDemoWin.data);
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

void addBool(WindowData& wd, const std::string& key, const bool value) noexcept
{
    wd._self->_add(key, value);
}
void addFloat(WindowData& wd, const std::string& key, const float value) noexcept
{
    wd._self->_add(key, value);
}
void addString(WindowData& wd, const std::string& key, const std::string& value) noexcept
{
    wd._self->_add(key, value);
}
void addFunction(WindowData& wd, const std::string& key, voidFunc value) noexcept
{
    wd._self->_add(key, value);
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
WindowFront::WindowFront(WindowData& wd) : _wd(wd)
{
    RegObjName();
}
WindowFront::WindowFront(WindowData& wd, const std::string_view title) : _wd(wd), _title(title)
{
    RegObjName();
}
void WindowFront::RegObjName() noexcept
{
    addString(_wd, getObjName(_title, ""), _title.data());
    addBool(_wd, getObjName(_title, _title), true);
}

std::string getObjName(std::string_view title, std::string_view nameObj) noexcept
{
    return std::format("{}_{}", title, nameObj);
}
}  // namespace helper
