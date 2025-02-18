#pragma once

#include <vector>
#include <string_view>
#include "resp_helper.h"
#include <atomic>

namespace helper
{

void Start();

constexpr const int NOT_SUITABLE{-1};
struct issueData
{
    std::string id{};
    std::string subject{};
    std::string description{};
    std::string project{};
    std::string status{};
    std::string tracker{};
    std::string priority{};
    std::string author{};
    std::string assigned_to{};
    std::string category{};
    std::string fixed_version{};
};

using issues_array = std::vector<std::string>;
using filters = std::vector<std::pair<const char*, const char*>>;
using issues_vec = std::vector<issueData>;

void sendMessageToTelegram(const std::string_view message);
template <typename T>
T print_err_response_find(cpr::Response& response, const std::string_view url);

struct issue_filters
{
    filters issue;
    filters relations;

    bool is_any_relations{false};
};

helper::issues_array find_issues(const helper::issue_filters& filters);

class IssueHandler
{
    std::atomic<bool> isRunning{false};  // Флаг состояния процесса Update
    std::atomic<bool> isStopped{true};   // Флаг остановки Run
    issues_vec _ret_val{};
    bool isReady{false};
    std::thread updateThread;  // Поток для выполнения Update
    helper::issue_filters cur_fil{};

    void Update(const helper::issue_filters& filters);

public:
    void Run(const helper::issue_filters& filters);      // Запускает Update каждые 5 минут
    void Stop();                                         // Останавливает выполнение Run (цикл)
    void Restart(const helper::issue_filters& filters);  // Перезапускает процесс
    void swap(helper::issues_vec& ivec);
    bool is_running() const;
    bool is_ready() const;
    void stopAndWait();  // Вспомогательная функция для остановки Run и ожидания завершения Update

    // Конструкторы и операторы
    IssueHandler();
    ~IssueHandler();

    IssueHandler(const IssueHandler& other);
    IssueHandler(IssueHandler&& other) noexcept;
    IssueHandler& operator=(const IssueHandler& other) = delete;
    IssueHandler& operator=(IssueHandler&& other) = delete;
};

template <typename T>
T print_err_response_find(cpr::Response& response, const std::string_view url)
{
    std::println(msg_hlp::err_response_find, response.status_code, url);
    return {};
}
}  // namespace helper
