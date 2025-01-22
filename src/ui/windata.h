#pragma once

#include <functional>
#include <string>
#include <imgui.h>
#include <iostream>
#include <unordered_map>
#include <variant>
#include <memory>

namespace helper
{

using voidFunc = std::function<void()>;
using DataVariant = std::variant<bool, float, voidFunc, std::string>;

class WindowData;
void setBool(WindowData& wd, const std::string& key, const bool value) noexcept;
void setFloat(WindowData& wd, const std::string& key, const float value) noexcept;
void setString(WindowData& wd, const std::string& key, const std::string& value) noexcept;
void setFunction(WindowData& wd, const std::string& key, voidFunc value) noexcept;

bool getBool(const WindowData& wd, const std::string& key) noexcept;
float getFloat(const WindowData& wd, const std::string& key) noexcept;
std::string getString(const WindowData& wd, const std::string& key) noexcept;
voidFunc getFunction(const WindowData& wd, const std::string& key) noexcept;

class WindowData final
{
    class ObjectData final
    {
        std::unordered_map<std::string, DataVariant> data;

    public:
        template <typename T>
        void _set(const std::string& key, const T& value)
        {
            data[key] = value;
        }

        template <typename T>
        T _get(const std::string& key)
        {
            return std::get<T>(data[key]);
        }

        std::unique_ptr<ObjectData> copy_() const { return std::make_unique<ObjectData>(*this); }
    };

    using selfObject = std::unique_ptr<ObjectData>;
    selfObject _self{};

public:
    friend void setBool(WindowData& wd, const std::string& key, const bool value) noexcept;
    friend void setFloat(WindowData& wd, const std::string& key, const float value) noexcept;
    friend void setString(WindowData& wd, const std::string& key, const std::string& value) noexcept;
    friend void setFunction(WindowData& wd, const std::string& key, voidFunc value) noexcept;

    friend bool getBool(const WindowData& wd, const std::string& key) noexcept;
    friend float getFloat(const WindowData& wd, const std::string& key) noexcept;
    friend std::string getString(const WindowData& wd, const std::string& key) noexcept;
    friend voidFunc getFunction(const WindowData& wd, const std::string& key) noexcept;

public:
    WindowData() : _self(std::make_unique<ObjectData>()) {}
    WindowData(const WindowData& x) : _self(x._self->copy_()) {}
    WindowData(WindowData&& x) noexcept = default;
    WindowData& operator=(WindowData x) noexcept
    {
        _self = std::move(x._self);
        return *this;
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
