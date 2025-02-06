#pragma once

#include "windata.h"
#include "app_helper.h"
#include "resp_helper.h"

namespace helper
{

constexpr const char* titleDemoWindow{"Демо окно"};
constexpr const char* titleSimpleWindow{"Простое окно"};
constexpr const char* titleMainWindow{"Главное окно"};
constexpr const char* titleSettingsWindow{"Настройки"};
constexpr const char* titleIssueWindow{"Задачи"};
constexpr const char* titleRedmineIdWindow{"Redmine ID"};
constexpr const char* titleTelegramSettingsWindow{"Настройки телеграмма"};
constexpr const char* saveButtonName{"Сохранить"};
constexpr const char* RedmineIdButtonName{"Настроить Redmine ID"};
constexpr const char* RedmineIdKey{"Redmine Key"};
constexpr const char* TelegramIdKey{"Telegram ID Key"};
constexpr const char* TelegramUserKey{"Telegram USER Key"};
constexpr const char* TIdKey{"Telegram ID"};
constexpr const char* TUserKey{"Telegram USER"};
constexpr const char* TelegramSettingsButtonName{"Настроить Telegram"};
constexpr const char* checkIssueButtonName{"Запустить поиск задач"};

constexpr const char* issueCountInput{"Максимальное количество задач в запросе"};
constexpr const size_t bufSize{512};

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

const helper::issue_filters test_filters{
    .issue = {{helper::project_id, helper::projectKsup}, {helper::status_id, helper::status_inprogress}},
    .relations = {},
    .is_any_relations = true,
};
class IssueWindow final : public helper::WindowFront
{
protected:
    const std::string_view bButtonPress{"ButtonOK"};
    const std::string_view fSlide{"slide"};
    const std::string_view fCounter{"counter"};
    helper::issues_vec issues{};

public:
    IssueWindow(WindowData& wd, const std::string_view title);
    virtual void DefaultSettings() noexcept override;
    virtual void Run() noexcept override;
    helper::IssueHandler issueHadnler{};
};
class RedmineIDWindow final : public helper::WindowFront
{

public:
    RedmineIDWindow(WindowData& wd, const std::string_view title);
    virtual void DefaultSettings() noexcept override;
    virtual void Run() noexcept override;

private:
    char buf[bufSize] = "";
};
class TelegramSettingWindow final : public helper::WindowFront
{

public:
    TelegramSettingWindow(WindowData& wd, const std::string_view title);
    virtual void DefaultSettings() noexcept override;
    virtual void Run() noexcept override;

private:
    char buf[bufSize] = "";
    char bufU[bufSize] = "";
};

}  // namespace helper
