#pragma once

#include <vector>
#include <string_view>

namespace helper
{
constexpr const int NOT_SUITABLE{-1};

using issues_array = std::vector<std::string>;
using filters = std::vector<std::pair<const char*, const char*>>;

void sendMessageToTelegram(const std::string_view message);

struct issue_filters
{
    filters issue;
    filters relations;

    bool is_any_relations{false};
};

void Run();

}  // namespace helper
