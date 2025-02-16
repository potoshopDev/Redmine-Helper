#pragma once

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>
#include <imgui.h>
#include "windata.h"

namespace helper
{

class WindowsDraw;
void draw(const WindowsDraw& wd);
void DefaultSettings(const WindowsDraw& wd);

class WindowsDraw
{
    struct IWindowsDraw
    {
        virtual ~IWindowsDraw() = default;
        virtual std::unique_ptr<IWindowsDraw> copy_() const = 0;
        virtual inline void draw_() = 0;
        virtual inline void defaultSettings_() = 0;
    };

    template <typename T>
    struct DrawableObject final : IWindowsDraw
    {
        T data_;
        DrawableObject(T x) : data_(std::move(x)) {}
        std::unique_ptr<IWindowsDraw> copy_() const override { return std::make_unique<DrawableObject>(*this); }
        virtual inline void draw_() override;
        virtual inline void defaultSettings_() override;
    };

    std::unique_ptr<IWindowsDraw> self_;

public:
    template <typename T>
    WindowsDraw(T x) : self_(std::make_unique<DrawableObject<T>>(std::move(x)))
    {
    }

public:
    WindowsDraw(const WindowsDraw& x) : self_(x.self_->copy_()) {}
    WindowsDraw(WindowsDraw&& x) noexcept = default;
    WindowsDraw& operator=(WindowsDraw x) noexcept
    {
        self_ = std::move(x.self_);
        return *this;
    }

public:
    friend void draw(const WindowsDraw& wd);
    friend void DefaultSettings(const WindowsDraw& wd);
};

using WindowsStack = std::vector<WindowsDraw>;
struct WindowsApp
{
    WindowsStack windowsStack{};

    template <typename T>
    void emplace_back(T obj)
    {
        windowsStack.emplace_back(obj);
    }
};

void draw(WindowsApp& app);
void DefaultSettings(WindowsApp& app);

template <typename T>
inline void WindowsDraw::DrawableObject<T>::draw_()
{
    draw(&data_);
}

template <typename T>
inline void WindowsDraw::DrawableObject<T>::defaultSettings_()
{
    DefaultSettings(&data_);
}

}  // namespace helper
