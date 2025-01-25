#include "win.h"
#include <imgui.h>

helper::SimpleWindow::SimpleWindow(WindowData& wd, const std::string_view title) : WindowFront(wd, title)
{
    RegObjName();
}

void helper::SimpleWindow::RegObjName() noexcept
{
    helper::addBool(_wd, helper::getObjName(_title, bButtonPress), true);
    helper::addFloat(_wd, helper::getObjName(_title, fSlide), 0.f);
    helper::addFloat(_wd, helper::getObjName(_title, fCounter), 0.f);
}

void helper::SimpleWindow::Run() noexcept
{
    auto bShow{helper::madeAutoBool(_wd, helper::getObjName(_title, _title))};

    if (bShow.data)
    {
        auto title{helper::madeAutoString(_wd, helper::getObjName(_title, ""))};
        auto f{helper::madeAutoFloat(_wd, helper::getObjName(_title, fSlide))};
        auto counter{helper::madeAutoFloat(_wd, helper::getObjName(_title, fCounter))};

        ImGui::Begin(title.data.c_str(), &bShow.data);
        ImGui::Text("This is some useful text.");  // Display some text (you can use a format strings too)
        // ImGui::Checkbox("Demo Window", &show_demo_window);  // Edit bools storing our window open/close state
        // ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f.data, 0.0f, 1.0f);  // Edit 1 float using a slider from 0.0f to 1.0f
        // ImGui::ColorEdit3("clear color", (float*)&clearColor);  // Edit 3 floats representing a color

        if (ImGui::Button("Button"))  // Buttons return true when clicked (most widgets return true when edited/activated)
            counter.data++;
        ImGui::SameLine();
        ImGui::Text("counter = %f", counter.data);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }
}
