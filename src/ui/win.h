#pragma once

#include "windata.h"

namespace helper
{

constexpr const char* titleDemoWindow{"Демо окно"};
constexpr const char* titleSimpleWindow{"Простое окно"};

DemoWindowData demoWindow{
    .contentRenderer = [](bool& showDemoWindow)
    {
        ImGui::ShowDemoWindow(&showDemoWindow);
    }};

const WindowData simpleWindow{.title = helper::titleSimpleWindow,
    .contentRenderer = []()
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Text("This is some useful text.");  // Display some text (you can use a format strings too)
        // ImGui::Checkbox("Demo Window", &show_demo_window);  // Edit bools storing our window open/close state
        // ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);  // Edit 1 float using a slider from 0.0f to 1.0f
        // ImGui::ColorEdit3("clear color", (float*)&clearColor);  // Edit 3 floats representing a color

        if (ImGui::Button("Button"))  // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }};
}  // namespace helper
