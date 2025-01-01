#pragma once

#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include <optional>
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


constexpr const char* REDMINE_URL = "https://qa.fogsoft.ru/issues";
constexpr const char* API_PATH = "API\\API_KEY.txt";

constexpr const char* issue{ "issue" };
constexpr const char* project_id{ "project_id" };
constexpr const char* project{ "project" };
constexpr const char* subject { "subject" };
constexpr const char* description { "description" };
constexpr const char* tracker{ "tracker" };
constexpr const char* tracker_id{ "tracker_id" };
constexpr const char* status{ "status" };
constexpr const char* status_id{ "status_id" };
constexpr const char* priority{ "priority" };
constexpr const char* priority_id{ "priority_id" };
constexpr const char* id { "id" };



std::optional<std::string> loadApiKey(const std::string& filePath)
{
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		std::println("Не удалось найти файл {}", filePath);
		return std::nullopt;
	}

	std::string apiKey;
	if (std::getline(file, apiKey))
	{
		file.close();
		return apiKey;
	}
	else
	{
		std::println("Не удалось прочитать Апи ключ из файла: {}", filePath);
		file.close();
		return std::nullopt;
	}
}

std::optional<std::string> getRedmineURL(const std::string& filePath)
{
	if (const auto apiKey{ loadApiKey(filePath) })
	{
		return REDMINE_URL + std::string(".json?key=") + apiKey->c_str();
	}
	return std::nullopt;
}

std::optional<std::string> getIssueUrl(const std::string& APIPath, const char* issue_id)
{
	if (const auto apiKey{ loadApiKey(APIPath) })
	{
		return REDMINE_URL + std::string("/") + issue_id + ".json?key=" + apiKey->c_str() + "&include=attachments";
	}
	return std::nullopt;
}
