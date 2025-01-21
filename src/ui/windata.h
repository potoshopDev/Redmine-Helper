#pragma once

#include <functional>
#include <string>
#include <imgui.h>
#include <iostream>
#include <unordered_map>
#include <variant>

namespace helper
{

class WindowData final
{
    using voidFunc = std::function<void()>;
    using DataVariant = std::variant<bool, float, voidFunc, std::string>;
    std::unordered_map<std::string, DataVariant> data;

public:
    template <typename T>
    void set(const std::string& key, const T& value)
    {
        data[key] = value;
    }

    template <typename T>
    T get(const std::string& key)
    {
        return std::get<T>(data[key]);
    }
};

struct WindowFront
{
    std::string title;                      // Заголовок окна
    std::function<void()> contentRenderer;  // Функция для отрисовки содержимого окна
};

struct DemoWindowFront
{
    bool showDemoWindow{true};
    std::function<void(bool&)> contentRenderer;  // Функция для отрисовки содержимого окна
};

void draw(const WindowFront& window, helper::WindowData& winData);
void draw(DemoWindowFront& window, helper::WindowData& winData);

}  // namespace helper
