#pragma once

#include <vector>
#include <string_view>
#include "resp_helper.h"

namespace helper
{
constexpr const int NOT_SUITABLE{-1};
struct issueData
{
    std::string id{};
    std::string subject{};
    std::string description{};
    std::string project{};
    std::string status1{};
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
helper::issues_vec filter_issues(const helper::issue_filters& filters);
void Run();

template <typename T>
T print_err_response_find(cpr::Response& response, const std::string_view url)
{
    std::println(msg_hlp::err_response_find, response.status_code, url);
    return {};
}
}  // namespace helper
