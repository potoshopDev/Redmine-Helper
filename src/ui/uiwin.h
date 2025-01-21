#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include <imgui.h>

namespace helper
{

class WindowsDraw
{
    struct IWindowsDraw
    {
        virtual ~IWindowsDraw() = default;
        virtual std::unique_ptr<IWindowsDraw> copy_() const = 0;
        virtual void draw_() = 0;
    };

    template <typename T>
    struct DrawableObject final : IWindowsDraw
    {
        T data_;
        DrawableObject(T x) : data_(std::move(x)) {}
        std::unique_ptr<IWindowsDraw> copy_() const override { return std::make_unique<DrawableObject>(*this); }
        void draw_() override;
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
    friend void draw(const WindowsDraw& wd) { wd.self_->draw_(); }
};

using WindowsStack = std::vector<WindowsDraw>;
void draw(const WindowsStack& windows);

template <typename T>
inline void WindowsDraw::DrawableObject<T>::draw_() 
{
    draw(data_);
}

}  // namespace helper