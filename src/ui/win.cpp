#include "win.h"
#include "app_helper.h"
#include "resp_helper.h"
#include "ui/win.h"
#include "ui/windata.h"
#include "ui/save.h"
#include <cstring>
#include <imgui.h>
#include <string>

namespace helper
{

void ShowImGuiWindow(bool& bShowFlag);

SimpleWindow::SimpleWindow(WindowData& wd, const std::string_view title) : WindowFront(wd, title) {}

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
MainWindow::MainWindow(WindowData& wd, const std::string_view title) : WindowFront(wd, title) {}

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
SettingsWindow::SettingsWindow(WindowData& wd, const std::string_view title) : WindowFront(wd, title) {}
void SettingsWindow::DefaultSettings() noexcept
{
    const auto ShowWinowName{helper::getObjName(_title, _title)};
    const auto SaveButtonName{helper::getObjName(helper::saveButtonName, helper::saveButtonName)};
    const auto inputIssueCountName{helper::getObjName(_title, helper::issueCountInput)};
    const auto RedmineIDWindowName{helper::getObjName(helper::titleRedmineIdWindow, helper::titleRedmineIdWindow)};
    const auto TelegramSettingsWindowName{helper::getObjName(helper::titleTelegramSettingsWindow, helper::titleTelegramSettingsWindow)};
    const auto updateTimerName{helper::getObjName(helper::updateTimer, helper::updateTimer)};

    helper::addBool(_wd, ShowWinowName, false);
    helper::addBool(_wd, SaveButtonName, false);
    helper::addFloat(_wd, inputIssueCountName, 25.0f);
    helper::addFloat(_wd, updateTimerName, 5.0f);
    helper::addBool(_wd, RedmineIDWindowName, false);
    helper::addBool(_wd, TelegramSettingsWindowName, false);
}

void helper::SettingsWindow ::Run() noexcept
{
    const auto ShowWinowName{helper::getObjName(_title, _title)};
    const auto SaveButtonName{helper::getObjName(helper::saveButtonName, helper::saveButtonName)};
    const auto inputIssueCountName{helper::getObjName(_title, helper::issueCountInput)};
    const auto RedmineIDWindowName{helper::getObjName(helper::titleRedmineIdWindow, helper::titleRedmineIdWindow)};
    const auto TelegramSettingsWindowName{helper::getObjName(helper::titleTelegramSettingsWindow, helper::titleTelegramSettingsWindow)};
    const auto updateTimerName{helper::getObjName(helper::updateTimer, helper::updateTimer)};

    auto bShowWindow{helper::madeAutoBool(_wd, ShowWinowName)};
    auto bSaveButton{helper::madeAutoBool(_wd, SaveButtonName)};
    auto fIssueCount{helper::madeAutoFloat(_wd, inputIssueCountName)};
    auto fUpdateTimer{helper::madeAutoFloat(_wd, updateTimerName)};
    auto bShowWindowRedmineId{helper::madeAutoBool(_wd, RedmineIDWindowName)};
    auto bShowWindowTelegram{helper::madeAutoBool(_wd, TelegramSettingsWindowName)};

    if (bShowWindow.data)
    {
        ImGui::Begin(_title.data(), &bShowWindow.data);
        if (ImGui::Button(helper::saveButtonName)) bSaveButton.data = true;

        ImGui::Separator();
        if (ImGui::Button(helper::RedmineIdButtonName)) bShowWindowRedmineId.data = true;

        ImGui::SameLine();
        if (ImGui::Button(helper::TelegramSettingsButtonName)) bShowWindowTelegram.data = true;
        ImGui::Separator();

        ImGui::SetNextItemWidth(100);
        if (ImGui::InputFloat(helper::issueCountInput, &fIssueCount.data, 1.0f, 10.0f, "%.0f"))
            fIssueCount.data <= .0f ? fIssueCount.data = 1.f : false;

        ImGui::SetNextItemWidth(100);
        if (ImGui::InputFloat(helper::updateTimer, &fUpdateTimer.data, 1.0f, 5.0f, "%.0f"))
            fUpdateTimer.data <= .0f ? fUpdateTimer.data = 1.f : false;

#ifndef __ADMIN_MODE
        if (ImGui::Button("Выгрузить save.data")) helper::SaveToFile(_wd, "tmp.data", false);
        ImGui::Separator();
#endif

        ImGui::End();
    }
}
//////////////////////////////////////////////////
/// IssueWindow
//////////////////////////////////////////////////
IssueWindow::IssueWindow(WindowData& wd, const std::string_view title) : WindowFront(wd, title) {}

void IssueWindow::DefaultSettings() noexcept
{
    const auto ShowWinowName{helper::getObjName(_title, _title)};
    const auto checkIssueButtonName{helper::getObjName(_title, helper::checkIssueButtonName)};

    helper::addBool(_wd, ShowWinowName, false);
    helper::addBool(_wd, checkIssueButtonName, false);

    issueHadnler.Run(test_filters);
}

void IssueWindow::Run() noexcept
{
    const auto ShowWinowName{helper::getObjName(_title, _title)};
    const auto checkIssueButtonName{helper::getObjName(_title, helper::checkIssueButtonName)};

    auto bShowWindow{helper::madeAutoBool(_wd, ShowWinowName)};
    auto bCheckButton{helper::madeAutoBool(_wd, checkIssueButtonName)};

    if (issueHadnler.is_ready()) issueHadnler.swap(issues);

    if (bShowWindow.data)
    {
        const char* items[] = {"Опция 1", "Опция 2"};
        static std::vector<int> selected_items(issues.size(), 0);
        if (issues.size() != selected_items.size()) selected_items.resize(issues.size());

        ImGui::Begin(_title.data(), &bShowWindow.data);
        {

            if (ImGui::Button(helper::checkIssueButtonName))
            {
                bCheckButton.data = true;

                const helper::issue_filters test_filters2{
                    .issue = {{helper::project_id, helper::projectKsup}, {helper::status_id, helper::status_open},
                        {helper::project_id, "632"}, {helper::project_id, helper::projectKdev}},
                    .relations = {},
                    .is_any_relations = false,
                };
                issueHadnler.Restart(test_filters2);
            }

            if (issueHadnler.is_running())
            {
                ImGui::SameLine();  // Размещаем следующую кнопку в одной линии
                ImGui::Text("Обновление задач ........");
            }

            auto index{0};

            ImGui::Separator();  // Добавляем разделитель между строками

            for (const auto& issue : issues)  // Цикл для создания 10 строк
            {
                if (issue.priority == "Нормальный")
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.3f, 0.2f, 1.0f));
                    ImGui::Text("Hello, world! ");
                }

                ImGui::Text("ID: %s", issue.id.c_str());             // Выводим ID
                ImGui::Text("Описание: %s", issue.subject.c_str());  // Выводим описание

                const auto labelCombo{"Задача:" + issue.id + "--" + items[selected_items[index]]};
                ImGui::Combo(labelCombo.c_str(), &selected_items[index], items, IM_ARRAYSIZE(items));  // Выпадающий список

                const auto labelButton1{"КнопкаA" + std::to_string(index)};
                if (ImGui::Button(labelButton1.c_str()))  // Первая кнопка
                {
                    // Обработчик для кнопки 1
                }
                ImGui::SameLine();  // Размещаем следующую кнопку в одной линии

                const auto labelButton2{"КнопкаB" + std::to_string(index)};
                if (ImGui::Button(labelButton2.c_str()))  // Вторая кнопка
                {
                    // Обработчик для кнопки 2
                }
                if (issue.priority == "Нормальный")
                {
                    ImGui::PopStyleColor();
                }
                ImGui::Separator();  // Добавляем разделитель между строками
                ImGui::Separator();  // Добавляем разделитель между строками
                ++index;
            }
        }
        ImGui::End();
    }
}
//////////////////////////////////////////////////
/// RedmineIDWindow
//////////////////////////////////////////////////
RedmineIDWindow::RedmineIDWindow(WindowData& wd, const std::string_view title) : WindowFront(wd) {}
void RedmineIDWindow::DefaultSettings() noexcept
{
    const auto RedmineIDWindowName{helper::getObjName(helper::titleRedmineIdWindow, helper::titleRedmineIdWindow)};
    helper::addBool(_wd, RedmineIDWindowName, false);
}
void RedmineIDWindow::Run() noexcept
{
    const auto RedmineIDWindowName{helper::getObjName(helper::titleRedmineIdWindow, helper::titleRedmineIdWindow)};
    auto bShowWindowRedmineId{helper::madeAutoBool(_wd, RedmineIDWindowName)};

    if (bShowWindowRedmineId.data)
    {
        ImVec2 windowSize(600, 200);  // Задайте размеры окна
        ImGui::SetNextWindowSize(windowSize);
        // ImGui::SetNextWindowFocus();

        ImGui::Begin(helper::titleRedmineIdWindow, nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        {
            ImGui::Text("Введите свой Redmine Api key");
            ImGui::InputText(": Redmine key", buf, helper::bufSize);

            if (ImGui::Button("Close"))
            {
                if (buf[0] != '\0')
                {
                    const auto RedmineKeyName{helper::getObjName(helper::RedmineIdKey, helper::RedmineIdKey)};
                    helper::setString(_wd, RedmineKeyName, buf);
                    std::memset(buf, '\0', sizeof(buf));
                }
                bShowWindowRedmineId.data = false;
            }
            ImGui::Separator();
            ImGui::Text("Внимание! Поле очищается для безопасности.\nНе нужно вводить Апи ключ каждый раз!");
        }
        ImGui::End();
    }
}
//////////////////////////////////////////////////
/// TelegramSettingWindow
//////////////////////////////////////////////////
TelegramSettingWindow::TelegramSettingWindow(WindowData& wd, const std::string_view title) : WindowFront(wd) {}
void TelegramSettingWindow::DefaultSettings() noexcept
{
    const auto TelegramSettingsWindowName{helper::getObjName(helper::titleTelegramSettingsWindow, helper::titleTelegramSettingsWindow)};
    helper::addBool(_wd, TelegramSettingsWindowName, false);
}
void TelegramSettingWindow::Run() noexcept
{

    const auto TelegramSettingsWindowName{helper::getObjName(helper::titleTelegramSettingsWindow, helper::titleTelegramSettingsWindow)};
    auto bShowWindowTelegramSettings{helper::madeAutoBool(_wd, TelegramSettingsWindowName)};

    if (bShowWindowTelegramSettings.data)
    {
        ImVec2 windowSize(600, 200);  // Задайте размеры окна

        ImGui::SetNextWindowSize(windowSize);
        ImGui::SetNextWindowFocus();

        (ImGui::Begin(helper::titleTelegramSettingsWindow, nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse));
        {
            ImGui::InputText(": Telegram ID чата", buf, helper::bufSize);
            ImGui::InputText(": Telegram ID пользователя", bufU, helper::bufSize);

            if (ImGui::Button("Close"))
            {
                if (buf[0] != '\0' && bufU[0] != '\0')
                {
                    const auto TelegramKeyName{helper::getObjName(helper::TIdKey, helper::TIdKey)};
                    const auto TelegramUserName{helper::getObjName(helper::TUserKey, helper::TUserKey)};

                    helper::setString(_wd, TelegramKeyName, buf);
                    helper::setString(_wd, TelegramUserName, bufU);
                    std::memset(buf, '\0', sizeof(buf));
                    std::memset(bufU, '\0', sizeof(bufU));
                }
                bShowWindowTelegramSettings.data = false;
            }
        }

        ImGui::Separator();
        ImGui::Text("Внимание! Поля очищается для безопасности.\nНе нужно заполнять поля каждый раз!");
        ImGui::End();
    }
}
}  // namespace helper
// namespace helper
