#pragma once

#include <functional>
#include <string>
#include <imgui.h>
#include <unordered_map>
#include <variant>
#include <memory>
#include <string_view>
#include <concepts>
#include <imgui.h>

namespace helper
{

using voidFunc = std::function<void()>;
using DataVariant = std::variant<bool, float, voidFunc, std::string>;

template <typename T>
concept AllowedType = std::same_as<T, bool> || std::same_as<T, float> || std::same_as<T, voidFunc> || std::same_as<T, std::string>;

class WindowData;
void setBool(WindowData& wd, const std::string& key, const bool value) noexcept;
void setFloat(WindowData& wd, const std::string& key, const float value) noexcept;
void setString(WindowData& wd, const std::string& key, const std::string& value) noexcept;
void setFunction(WindowData& wd, const std::string& key, voidFunc value) noexcept;

void addBool(WindowData& wd, const std::string& key, const bool value) noexcept;
void addFloat(WindowData& wd, const std::string& key, const float value) noexcept;
void addString(WindowData& wd, const std::string& key, const std::string& value) noexcept;
void addFunction(WindowData& wd, const std::string& key, voidFunc value) noexcept;

bool getBool(const WindowData& wd, const std::string& key) noexcept;
float getFloat(const WindowData& wd, const std::string& key) noexcept;
std::string getString(const WindowData& wd, const std::string& key) noexcept;
voidFunc getFunction(const WindowData& wd, const std::string& key) noexcept;
std::string getObjName(std::string_view title, std::string_view nameObj) noexcept;

template <AllowedType T>
class autoUpdater;

class WindowData final
{
    class ObjectData final
    {
    public:
        std::unordered_map<std::string, DataVariant> data;

        template <AllowedType T>
        void _set(const std::string& key, const T& value)
        {
            data[key] = value;
        }

        template <AllowedType T>
        void _add(const std::string& key, const T& value)
        {
            data.emplace(key, value);
        }

        template <AllowedType T>
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

    friend void addBool(WindowData& wd, const std::string& key, const bool value) noexcept;
    friend void addFloat(WindowData& wd, const std::string& key, const float value) noexcept;
    friend void addString(WindowData& wd, const std::string& key, const std::string& value) noexcept;
    friend void addFunction(WindowData& wd, const std::string& key, voidFunc value) noexcept;

    friend bool getBool(const WindowData& wd, const std::string& key) noexcept;
    friend float getFloat(const WindowData& wd, const std::string& key) noexcept;
    friend std::string getString(const WindowData& wd, const std::string& key) noexcept;
    friend voidFunc getFunction(const WindowData& wd, const std::string& key) noexcept;
    friend void SaveToFile(const helper::WindowData& data, const std::string& filename, bool encryption);

    template <AllowedType T>
    friend class autoUpdater;

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

template <AllowedType T>
class autoUpdater final
{
public:
    T data;
    autoUpdater(WindowData& wd, const std::string key) : _winData(wd), _key(key) { data = _winData._self->_get<T>(_key); }
    void Save() noexcept { _winData._self->_set<T>(_key, data); };
    ~autoUpdater() { Save(); }

private:
    WindowData& _winData;
    const std::string _key{};
};

autoUpdater<bool> madeAutoBool(WindowData& wd, const std::string key);
autoUpdater<float> madeAutoFloat(WindowData& wd, const std::string key);
autoUpdater<voidFunc> madeAutoFunc(WindowData& wd, const std::string key);
autoUpdater<std::string> madeAutoString(WindowData& wd, const std::string key);

std::string getObjName(std::string_view title, std::string_view nameObj) noexcept;

class WindowFront
{
protected:
    WindowData& _wd;

public:
    WindowFront(WindowData& wd);
    WindowFront(WindowData& wd, const std::string_view title);

    virtual void Run() noexcept {};
    virtual void DefaultSettings() noexcept;
    virtual ~WindowFront() = default;
    const std::string_view _title{"new window"};
};

struct DemoWindowFront
{
    bool showDemoWindow{true};
    std::function<void(bool&)> contentRenderer;  // Функция для отрисовки содержимого окна
};

void draw(WindowFront* window);
void DefaultSettings(WindowFront* window);
void draw(DemoWindowFront& window);

}  // namespace helper
