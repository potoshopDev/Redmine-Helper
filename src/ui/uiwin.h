#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <functional>
#include <imgui.h>


struct Vector3D
{
    int x, y, z;
};

struct WindowData
{
    std::string title;                      // Заголовок окна
    std::function<void()> contentRenderer;  // Функция для отрисовки содержимого окна
};

void draw(const WindowData& window)
{
        ImGui::Begin(window.title.c_str());  // Создаём окно
        window.contentRenderer();            // Отображаем содержимое окна
        ImGui::End();                        // Завершаем отрисовку окна
}


class WindowsDraw
{
    struct IWindowsDraw
    {
        virtual ~IWindowsDraw() = default;
        virtual std::unique_ptr<IWindowsDraw> copy_() const = 0;
        virtual void draw_(const WindowData& window) const = 0;
    };

    template <typename T>
    struct DrawableObject final : IWindowsDraw
    {
        T data_;
        DrawableObject(T x) : data_(std::move(x)) {}
        std::unique_ptr<IWindowsDraw> copy_() const override { return std::make_unique<DrawableObject>(*this); }
        void draw_(const WindowData& window) const override;
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
    friend void draw(const WindowsDraw& x, const WindowData& win) { x.self_->draw_(win); }
};


using WindowsStack = std::vector<WindowsDraw>;


void draw(const WindowsStack& windows, const WindowData& data)
{
    for (const auto& window : windows)
        draw(window, data);
}

template <typename T>
inline void WindowsDraw::DrawableObject<T>::draw_(const WindowData& window) const
{
    ::draw(window);
}



