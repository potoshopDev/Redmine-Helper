#pragma once

#include <nlohmann/json.hpp>
#include <print>
#include <vector>

namespace httpCodes
{
	constexpr int HTTP_OK = 200;
	constexpr int HTTP_POSTOK = 201;
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
		std::println("Unknow Error");
	}

	return std::nullopt;
}


struct Project {
	int id;
	std::string name;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Project, id, name)
};


struct Tracker {
	int id;
	std::string name;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Tracker, id, name)
};

struct Issue {
	int id;
	Project project;
	Tracker tracker;
	std::string subject;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Issue, id, project, tracker, subject)
};

struct IssuesList {
	std::vector<Issue> issues;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(IssuesList, issues)
};



