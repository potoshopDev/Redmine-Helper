#include "windata.h"

constexpr const char* titleDemoWindow{"Демо окно"};
constexpr const char* titleSimpleWindow{"Простое окно"};

namespace helper
{

    #include <string_view>
template <typename T>
class autoUpdater final
{
public:
    T data;
    autoUpdater(WindowData& wd, const std::string_view key) : _winData(wd), _key(key) { data = _winData._get<T>(_key.data()); }
    ~autoUpdater() { _winData._set<T>(_key.data(), data); }

private:
    WindowData& _winData{};
    const std::string_view _key{};
};
void draw(const WindowFront& window, helper::WindowData& winData)
{
    static bool s{true};
    if (s)
    {
        helper::setBool(winData, titleDemoWindow, true);
        s = false;
    }
    ImGui::Begin(window.title.c_str());  // Создаём окно
    window.contentRenderer();            // Отображаем содержимое окна
    ImGui::End();                        // Завершаем отрисовку окна
}

void draw(DemoWindowFront& window, helper::WindowData& winData)
{
    autoUpdater<bool> showDemoWin{winData, titleDemoWindow};
    if (showDemoWin.data) window.contentRenderer(showDemoWin.data);  // Отображаем содержимое окна
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
