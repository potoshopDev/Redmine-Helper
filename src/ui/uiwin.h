#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include <imgui.h>
#include "windata.h"

namespace helper
{

class WindowsDraw
{
    struct IWindowsDraw
    {
        virtual ~IWindowsDraw() = default;
        virtual std::unique_ptr<IWindowsDraw> copy_() const = 0;
        virtual void draw_(helper::WindowData& winData) = 0;
    };

    template <typename T>
    struct DrawableObject final : IWindowsDraw
    {
        T data_;
        DrawableObject(T x) : data_(std::move(x)) {}
        std::unique_ptr<IWindowsDraw> copy_() const override { return std::make_unique<DrawableObject>(*this); }
        void draw_(helper::WindowData& winData) override;
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
    friend void draw(const WindowsDraw& wd, helper::WindowData& winData) { wd.self_->draw_(winData); }
};

using WindowsStack = std::vector<WindowsDraw>;
struct WindowsApp
{
    WindowsStack windowsStack{};
    helper::WindowData windowsData{};

    template <typename T>
    void emplace_back(T& obj)
    {
        windowsStack.emplace_back(obj);
    }
};

void draw(WindowsApp& app);

template <typename T>
inline void WindowsDraw::DrawableObject<T>::draw_(helper::WindowData& winData)
{
    draw(data_, winData);
}

}  // namespace helper