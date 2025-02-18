#include "app_helper.h"
#include "resp_helper.h"
#include <thread>
#include <chrono>

#include <thread>
#include <utility>

namespace helper
{

constexpr const std::chrono::minutes sleep_five_minutes{5};
std::string __get_url_find(const helper::issue_filters& filters);
std::optional<cpr::Response> get_issue_relation(std::string_view issueId, std::string_view key);

helper::issues_array find_issues(const helper::issue_filters& filters)
{
    const auto key{helper::loadApiKey(helper::API_PATH)};

    if (helper::is_key_bad(key)) return {};

    const auto url{__get_url_find(filters)};
    auto response{helper::get_response_k(url, *key)};

    if (helper::is_status_code_nook(response)) return print_err_response_find<helper::issues_array>(response, url);

    const auto issues = nlohmann::json::parse(response.text);
    helper::issues_array return_issues{};

    for (const auto& issue : issues[helper::issues])
    {
        auto issueId{std::to_string(issue[helper::id].get<int>())};
        const auto response{get_issue_relation(issueId, *key)};
        if (!response) continue;

        const auto relationsData = nlohmann::json::parse(response->text);

        if (filters.is_any_relations)
            if (relationsData[helper::relations].is_array() && !relationsData[helper::relations].empty()) continue;

        return_issues.push_back(issueId.c_str());
    }

    return return_issues;
}

helper::issues_vec filter_issues(const helper::issue_filters& filters)
{
    const auto key{helper::loadApiKey(helper::API_PATH)};
    if (helper::is_key_bad(key)) return {};

    const auto url{__get_url_find(filters)};
    auto response{helper::get_response_k(url, *key)};

    if (helper::is_status_code_nook(response)) return print_err_response_find<helper::issues_vec>(response, url);

    const auto issues = nlohmann::json::parse(response.text);
    helper::issues_vec return_issues{};

    for (const auto& issue : issues[helper::issues])
    {
        auto issueId{std::to_string(issue[helper::id].get<int>())};
        const auto response{get_issue_relation(issueId, *key)};
        if (!response) continue;

        const auto relationsData = nlohmann::json::parse(response->text);

        if (filters.is_any_relations)
            if (relationsData[helper::relations].is_array() && !relationsData[helper::relations].empty()) continue;

        helper::issueData tmpData{.id = issueId,
            .subject = issue.contains(helper::subject) ? issue[helper::subject] : "",
            .description = issue.contains(helper::description) ? issue[helper::description] : "",
            .project = issue.contains(helper::project) ? issue[helper::project][helper::name] : "",
            .status = issue.contains(helper::status) ? issue[helper::status][helper::name] : "",
            .tracker = issue.contains(helper::tracker) ? issue[helper::tracker][helper::name] : "",
            .priority = issue.contains(helper::priority) ? issue[helper::priority][helper::name] : "",
            .author = issue.contains(helper::author) ? issue[helper::author][helper::name] : "",
            .assigned_to = issue.contains(helper::assigned_to) ? issue[helper::assigned_to][helper::name] : "",
            .category = issue.contains(helper::category) ? issue[helper::category][helper::name] : "",
            .fixed_version = issue.contains(helper::fixed_version) ? issue[helper::fixed_version][helper::name] : ""};

        return_issues.emplace_back(tmpData);
    }

    return return_issues;
}

std::optional<cpr::Response> get_issue_relation(std::string_view issueId, std::string_view key)
{
    const auto relationsURL{std::format(helper::fmt_relation_issue_url, helper::REDMINE_URL, issueId)};
    const auto response = helper::get_response_k(relationsURL, key);

    if (helper::is_status_code_nook(response))
    {
        std::println("Не удалось получить список связанных задач у {}, статус код: {}", issueId, response.status_code);
        return std::nullopt;
    }

    return response;
}

std::string __get_url_find(const helper::issue_filters& filters)
{
    auto url{std::format("{}.json", helper::REDMINE_URL_ISSUE)};
    bool flag_lit{true};

    for (const auto& filter : filters.issue)
    {
        url = std::format("{}{}{}={}", url, flag_lit ? "?" : "&", filter.first, filter.second);
        flag_lit = false;
    }
    return url;
}

std::string formatting_msg_copy_issue(const std::string_view current_issue, const std::string_view new_issue)
{
    const auto key{helper::loadApiKey(helper::API_PATH)};
    if (helper::is_key_bad(key)) return {};

    const auto response{helper::GetIssue(new_issue.data())};
    if (!response) return std::format("Ошибка при получении названия у задачи {}", new_issue);

    const auto data{nlohmann::json::parse(response->text)};
    const auto subject{data[helper::issue][helper::subject]};

    return std::format("РќРѕРІР°СЏ Р·Р°РґР°С‡Р°: {}/issues/{} : {}", helper::REDMINE_URL, new_issue, subject.dump().c_str());
}

void send_msg_copy_issue(const std::string_view current_issue, const std::string_view new_issue)
{
    std::println("Скопировал задачу {} в {}", current_issue, new_issue);

    const auto msg{formatting_msg_copy_issue(current_issue, new_issue)};
    helper::sendMessageToTelegram(msg);
}

void push_to_working_issue(const helper::issues_array& issues_arr)
{
    for (const auto& current_issue : issues_arr)
    {
        const auto issueDataSup{helper::GetSupIssueData()};
        helper::UpdateIssue(current_issue, issueDataSup);

        const auto newTaskId{helper::copy(current_issue)};
        if (!newTaskId) continue;

        const auto issueDataDev{helper::GetDevIssueData()};
        helper::UpdateIssue(*newTaskId, issueDataDev);

        send_msg_copy_issue(current_issue, *newTaskId);
    }
}

void Start()
{

    if (!helper::check_response(helper::YANDEX_URL)) return;
    if (!helper::check_response(helper::REDMINE_URL)) return;

    const helper::issue_filters csp_filters{
        .issue = {{helper::project_id, helper::projectKsup}, {helper::status_id, helper::status_open}},
        .relations = {},
        .is_any_relations = true,
    };

    while (true)
    {
        const auto issues_arr{find_issues(csp_filters)};
        if (issues_arr.empty())
            std::println(msg_hlp::couldnt_find_new_issues);
        else
            push_to_working_issue(issues_arr);
        std::this_thread::sleep_for(helper::sleep_five_minutes);
    }
}
// Остановка процесса Run и ожидание Update (вспомогательная функция)
void IssueHandler::stopAndWait()
{
    Stop();  // Останавливаем поток Run
    while (isRunning.load())
    {
        std::this_thread::yield();  // Ждем завершения Update
    }
}

// Метод Update (переименованный из Run)
void IssueHandler::Update(const helper::issue_filters& filters)
{
    if (isRunning.exchange(true)) return;  // Если уже выполняется, выйти
    cur_fil = filters;

    std::thread(
        [this, filters]()
        {
            _ret_val.clear();
            _ret_val.shrink_to_fit();
            _ret_val = filter_issues(filters);  // Выполняем фильтрацию данных
                                                //
            isReady = true;
            isRunning = false;  // Сбрасываем флаг после завершения
        })
        .detach();
}

// Запуск цикла Run
void IssueHandler::Run(const helper::issue_filters& filters)
{
    if (!isStopped.exchange(false)) return;  // Если уже запущено, выходим
    cur_fil = filters;                       // Сохраняем фильтры

    updateThread = std::thread(
        [this, filters]()
        {
            while (!isStopped.load())
            {
                Update(filters);

                auto time{10.f};
                while (time > 0.f)
                {
                    if (isStopped.load()) break;

                    std::this_thread::sleep_for(std::chrono::milliseconds(340));
                    time -= 0.34f;
                }
            }
        });
}

// Остановка процесса Run
void IssueHandler::Stop()
{
    if (!isStopped.exchange(true))
    {  // Если процесс еще идет
        if (updateThread.joinable())
        {
            updateThread.join();  // Ожидаем завершения потока
        }
    }
}

// Перезапуск процесса Run
void IssueHandler::Restart(const helper::issue_filters& filters)
{
    Stop();
    Run(filters);
}

// Конструктор по умолчанию
IssueHandler::IssueHandler() = default;

// Деструктор
IssueHandler::~IssueHandler()
{
    Stop();
}

// Конструктор копирования
IssueHandler::IssueHandler(const IssueHandler& other) : isReady(other.isReady), cur_fil(other.cur_fil)
{
    if (!other.isStopped) Run(cur_fil);
}

// Конструктор перемещения
IssueHandler::IssueHandler(IssueHandler&& other) noexcept
    : isRunning(other.isRunning.load()), isStopped(other.isStopped.load()), _ret_val(std::move(other._ret_val)), isReady(other.isReady)
{
    // Останавливаем процесс обновления и поток оригинала
    other.stopAndWait();

    if (!isStopped)
    {  // Если Run был запущен, перезапускаем в новом объекте
        Run(cur_fil);
        other.isStopped = true;  // Указываем, что Run в оригинале остановлен
    }
}

bool IssueHandler::is_running() const
{
    return isRunning.load();
}

bool IssueHandler::is_ready() const
{
    return isReady;
}

void IssueHandler::swap(helper::issues_vec& ivec)
{
    if (isReady && isRunning.exchange(true)) return;

    std::swap(ivec, _ret_val);

    _ret_val.clear();
    _ret_val.shrink_to_fit();
    isReady = false;
    isRunning = false;
}
}  // namespace helper
