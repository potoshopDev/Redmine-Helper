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

class WindowData final
{
    using voidFunc = std::function<void()>;

    class iWindowData
    {

    public:
        virtual void setBool(const std::string& key, const bool value) noexcept = 0;
        virtual void setFloat(const std::string& key, const float value) noexcept = 0;
        virtual void setString(const std::string& key, const std::string& value) noexcept = 0;
        virtual void setFunction(const std::string& key, std::function<void()> value) noexcept = 0;

        virtual bool getBool(const std::string& key) noexcept = 0;
        virtual float getFloat(const std::string& key) noexcept = 0;
        virtual std::string getString(const std::string& key) noexcept = 0;
        virtual voidFunc getFunction(const std::string& key) noexcept = 0;

        using selfObject = std::unique_ptr<iWindowData>;
        virtual selfObject copy_() const = 0;

        virtual ~iWindowData() = default;
    };

    class ObjectData final : public iWindowData
    {
        using DataVariant = std::variant<bool, float, voidFunc, std::string>;
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

        virtual selfObject copy_() const override { return std::make_unique<ObjectData>(*this); }

        virtual void setBool(const std::string& key, const bool value) noexcept override { _set(key, value); }
        virtual void setFloat(const std::string& key, const float value) noexcept override { _set(key, value); }
        virtual void setString(const std::string& key, const std::string& value) noexcept override { _set(key, value); }
        virtual void setFunction(const std::string& key, std::function<void()> value) noexcept override { _set(key, value); }

        virtual bool getBool(const std::string& key) noexcept override { return _get<bool>(key); }
        virtual float getFloat(const std::string& key) noexcept override { return _get<float>(key); }
        virtual std::string getString(const std::string& key) noexcept override { return _get<std::string>(key); }
        virtual voidFunc getFunction(const std::string& key) noexcept override { return _get<voidFunc>(key); }
    };

	using selfObject = std::unique_ptr<iWindowData>;
    selfObject _self{};


public:
    void setBool(const std::string& key, const bool value) noexcept { _self->setBool(key, value); }
    void setFloat(const std::string& key, const float value) noexcept { _self->setFloat(key, value); }
    void setString(const std::string& key, const std::string& value) noexcept { _self->setString(key, value); }
    void setFunction(const std::string& key, std::function<void()> value) noexcept { _self->setFunction(key, value); }

    bool getBool(const std::string& key) noexcept { _self->getBool(key); }
    float getFloat(const std::string& key) noexcept { _self->getFloat(key); }
    std::string getString(const std::string& key) noexcept { _self->getString(key); }
    voidFunc getFunction(const std::string& key) noexcept { _self->getFunction(key); }

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
