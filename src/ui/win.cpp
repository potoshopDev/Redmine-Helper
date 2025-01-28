#include "win.h"
#include "ui/win.h"
#include <imgui.h>

namespace helper
{

void ShowImGuiWindow(bool& bShowFlag);

SimpleWindow::SimpleWindow(WindowData& wd, const std::string_view title) : WindowFront(wd, title)
{
    DefaultSettings();
}

void helper::SimpleWindow::DefaultSettings() noexcept
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

//////////////////////////////////////////////////
/// MainWindow
//////////////////////////////////////////////////
MainWindow::MainWindow(WindowData& wd, const std::string_view title) : WindowFront(wd, title)
{
    DefaultSettings();
}

void MainWindow::DefaultSettings() noexcept
{
    helper::setBool(_wd, helper::getObjName(_title, _title), true);
    helper::addString(_wd, helper::getObjName(_title, titleSettingsWindow), titleSettingsWindow);
    helper::addString(_wd, helper::getObjName(_title, titleIssueWindow), titleIssueWindow);
}

void MainWindow::Run() noexcept
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    const auto nameSettingsWindow{helper::getObjName(helper::titleSettingsWindow, helper::titleSettingsWindow)};
    const auto nameIssueWindow{helper::getObjName(helper::titleIssueWindow, helper::titleIssueWindow)};
    const auto nameButtonSettings{helper::getObjName(_title, titleSettingsWindow)};
    const auto nameButtonIssue{helper::getObjName(_title, titleIssueWindow)};

    auto buttonSettings{helper::madeAutoString(_wd, nameButtonSettings)};
    auto buttonIssue{helper::madeAutoString(_wd, nameButtonIssue)};
    auto showButtonSet{helper::madeAutoBool(_wd, nameSettingsWindow)};
    auto showButtonIssue{helper::madeAutoBool(_wd, nameIssueWindow)};

    ImGui::Begin(_title.data(), NULL,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav |
            ImGuiWindowFlags_NoBringToFrontOnFocus);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    if (ImGui::Button(buttonSettings.data.c_str())) ShowImGuiWindow(showButtonSet.data);
    ImGui::SameLine();  // Размещаем следующую кнопку в одной линии
    if (ImGui::Button(buttonIssue.data.c_str())) ShowImGuiWindow(showButtonIssue.data);

    ImGui::End();
}

//////////////////////////////////////////////////
/// SettingsWindow
//////////////////////////////////////////////////
SettingsWindow::SettingsWindow(WindowData& wd, const std::string_view title) : WindowFront(wd, title)
{
    DefaultSettings();
}
void SettingsWindow::DefaultSettings() noexcept
{
    const auto ShowWinowName{helper::getObjName(_title, _title)};
    const auto SaveButtonName{helper::getObjName(helper::saveButtonName, helper::saveButtonName)};

    helper::addBool(_wd, ShowWinowName, false);
    helper::addBool(_wd, SaveButtonName, false);
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
//////////////////////////////////////////////////
/// IssueWindow
//////////////////////////////////////////////////
IssueWindow::IssueWindow(WindowData& wd, const std::string_view title) : WindowFront(wd, title)
{
    DefaultSettings();
}

void IssueWindow::DefaultSettings() noexcept
{
    const auto ShowWinowName{helper::getObjName(_title, _title)};
    const auto checkIssueButtonName{helper::getObjName(_title, helper::checkIssueButtonName)};

    helper::addBool(_wd, ShowWinowName, false);
    helper::addBool(_wd, checkIssueButtonName, false);
}

void IssueWindow::Run() noexcept
{
    const auto ShowWinowName{helper::getObjName(_title, _title)};
    const auto checkIssueButtonName{helper::getObjName(_title, helper::checkIssueButtonName)};

    auto bShowWindow{helper::madeAutoBool(_wd, ShowWinowName)};
    auto bCheckButton{helper::madeAutoBool(_wd, checkIssueButtonName)};

    if (bShowWindow.data)
    {
        ImGui::Begin(_title.data(), &bShowWindow.data);
        if (ImGui::Button(helper::checkIssueButtonName)) bCheckButton.data = true;

        for (int i = 0; i < 10; ++i)  // Цикл для создания 10 строк
        {
            ImGui::Text("ID: %d", i);                                  // Выводим ID
            ImGui::Text("Описание: Краткое описание элемента %d", i);  // Выводим описание

            const char* items[] = {"Опция 1", "Опция 2"};
            static int selected_item = 0;  // Хранит выбранный элемент

            ImGui::Combo("Выпадающий список", &selected_item, items, IM_ARRAYSIZE(items));  // Выпадающий список

            if (ImGui::Button("Кнопка 1"))  // Первая кнопка
            {
                // Обработчик для кнопки 1
            }
            ImGui::SameLine();  // Размещаем следующую кнопку в одной линии

            if (ImGui::Button("Кнопка 2"))  // Вторая кнопка
            {
                // Обработчик для кнопки 2
            }
            ImGui::Separator();  // Добавляем разделитель между строками
        }

        ImGui::End();
    }
}
}  // namespace helper
