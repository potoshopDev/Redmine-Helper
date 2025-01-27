#include "win.h"
#include <imgui.h>

namespace helper
{

void ShowImGuiWindow(bool& bShowFlag);

SimpleWindow::SimpleWindow(WindowData& wd, const std::string_view title) : WindowFront(wd, title)
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
void ShowImGuiWindow(bool& bShowFlag)
{
    bShowFlag = !bShowFlag;
}

MainWindow::MainWindow(WindowData& wd, const std::string_view title) : WindowFront(wd, title)
{
    RegObjName();
}

void MainWindow::RegObjName() noexcept
{
    helper::setBool(_wd, helper::getObjName(_title, _title), true);
    helper::addString(_wd, helper::getObjName(_title, titleSettingsWindow), titleSettingsWindow);
}

void MainWindow::Run() noexcept
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    const auto nameSettingsWindow{helper::getObjName(helper::titleSettingsWindow, helper::titleSettingsWindow)};
    const auto nameButtonSettings{helper::getObjName(_title, titleSettingsWindow)};

    auto buttonSettings{helper::madeAutoString(_wd, nameButtonSettings)};
    auto showButtonSet{helper::madeAutoBool(_wd, nameSettingsWindow)};

    ImGui::Begin(_title.data(), NULL,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav |
            ImGuiWindowFlags_NoBringToFrontOnFocus);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    if (ImGui::Button(buttonSettings.data.c_str())) ShowImGuiWindow(showButtonSet.data);

    ImGui::End();
}

SettingsWindow::SettingsWindow(WindowData& wd, const std::string_view title) : WindowFront(wd, title)
{
    RegObjName();
}
void SettingsWindow::RegObjName() noexcept
{
    const auto bShowWinowName{helper::getObjName(_title, _title)};
    const auto bSaveButtonName{helper::getObjName(helper::saveButtonName, helper::saveButtonName)};

    helper::addBool(_wd, bShowWinowName, false);
    helper::addBool(_wd, bSaveButtonName, false);
}
void helper::SettingsWindow ::Run() noexcept
{
    const auto ShowWinowName{helper::getObjName(_title, _title)};
    const auto SaveButtonName{helper::getObjName(helper::saveButtonName, helper::saveButtonName)};

    auto bShowWindow{helper::madeAutoBool(_wd, ShowWinowName)};
    auto bSaveButton{helper::madeAutoBool(_wd, SaveButtonName)};

    if (bShowWindow.data)
    {
        ImGui::Begin(_title.data(), &bShowWindow.data);
        if (ImGui::Button(helper::saveButtonName)) bSaveButton.data = true;

        ImGui::End();
    }
}

}  // namespace helper
