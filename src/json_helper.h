#pragma once

#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include <optional>
#include <print>

namespace httpCodes
{
constexpr int HTTP_OK = 200;
constexpr int HTTP_NOT_FOUND = 404;
}  // namespace httpCodes


template <typename T>
static std::optional<T> parse(const std::string& jsonStr)
{
    try
    {
        const auto json = nlohmann::json::parse(jsonStr);
        return json.get<T>();
    }
    catch (nlohmann::json::out_of_range& e)
    {
        std::println("Wrong JSON field: {}", e.what());
    }
    catch (nlohmann::json::parse_error& e)
    {
        std::println("Parse error: {}", e.what());
    }
    catch (...)
    {
        std::cout << "Something wrong with JSON." << std::endl;
        std::println("Unknow Error", e.what());
    }

    return std::nullopt;
}
