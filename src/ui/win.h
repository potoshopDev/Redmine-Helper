#pragma once

#include "windata.h"
#include "app_helper.h"
#include "resp_helper.h"
#include <vector>

namespace helper
{

constexpr const char* titleDemoWindow{"Демо окно"};
constexpr const char* titleSimpleWindow{"Простое окно"};
constexpr const char* titleMainWindow{"Главное окно"};
constexpr const char* titleSettingsWindow{"Настройки"};
constexpr const char* titleIssueWindow{"Задачи"};
constexpr const char* saveButtonName{"Сохранить"};
constexpr const char* checkIssueButtonName{"Проверить задачи"};

class SimpleWindow final : public helper::WindowFront
{
protected:
    const std::string_view bButtonPress{"ButtonOK"};
    const std::string_view fSlide{"slide"};
    const std::string_view fCounter{"counter"};

public:
    SimpleWindow(WindowData& wd, const std::string_view title);
    virtual void DefaultSettings() noexcept override;
    virtual void Run() noexcept override;
};

class MainWindow final : public helper::WindowFront
{

public:
    MainWindow(WindowData& wd, const std::string_view title);
    virtual void DefaultSettings() noexcept override;
    virtual void Run() noexcept override;
};

class SettingsWindow final : public helper::WindowFront
{

public:
    SettingsWindow(WindowData& wd, const std::string_view title);

    void DefaultSettings() noexcept override;
    virtual void Run() noexcept override;
};

class IssueWindow final : public helper::WindowFront
{
protected:
    const std::string_view bButtonPress{"ButtonOK"};
    const std::string_view fSlide{"slide"};
    const std::string_view fCounter{"counter"};
    helper::issues_vec issues{};

    const helper::issue_filters test_filters{
        .issue = {{helper::project_id, helper::projectKsup}, {helper::status_id, helper::status_inprogress}},
        .relations = {},
        .is_any_relations = true,
    };

public:
    IssueWindow(WindowData& wd, const std::string_view title);
    virtual void DefaultSettings() noexcept override;
    virtual void Run() noexcept override;
};

}  // namespace helper
