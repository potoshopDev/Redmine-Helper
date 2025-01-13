#pragma once

#include <thread>
#include <chrono>
#include <vector>
#include <functional>
#include <resp_helper.h>



#include <atomic>
#include <iostream>


namespace helper
{
	constexpr const int NOT_SUITABLE{ -1 };

	using issues_array = std::vector<std::string>;
	using filters = std::vector<std::pair<const char*, const char*>>;

	struct issue_filters
	{
		filters issue;
		filters relations;

		bool is_any_relations{ false };
	};

	const issue_filters csp_filters{
		.issue = { {helper::project_id, helper::projectKsup}, {helper::status_id, helper::status_open}},
		.relations = {},
		.is_any_relations = true,
	};
}

namespace 
{

	helper::issues_array find_issues(const helper::issue_filters& filters)
	{

		const auto key{ helper::loadApiKey(helper::API_PATH) };
		if (!key) return {};

		auto url{ std::format("{}.json", helper::REDMINE_URL_ISSUE) };
		bool flag_lit{ true };

		for (const auto& filter : filters.issue)
		{
			url = std::format("{}{}{}={}", url, flag_lit ? "?" : "&", filter.first, filter.second);
			flag_lit = false;
		}

		auto response = cpr::Get(
			cpr::Url{ url },
			cpr::VerifySsl{ false },
			cpr::Header{ {helper::Content_Type, helper::application_json},
						 {helper::X_Redmine_API_Key, key->c_str()} }
		);

		if (response.status_code != 200)
			return {};

		const auto issues = nlohmann::json::parse(response.text);
		helper::issues_array return_issues{};

		for (const auto& issue : issues["issues"])
		{
			auto issueId{ std::to_string(issue[helper::id].get<int>()) };
			// Проверяем связи с другими задачами
			std::string relationsUrl = std::format("{}/issues/{}/relations.json", helper::REDMINE_URL, issueId);
			response = cpr::Get(
				cpr::Url{ relationsUrl },
				cpr::VerifySsl{ false },
				cpr::Header{ {helper::Content_Type, helper::application_json},
							 {helper::X_Redmine_API_Key, key->c_str()} }

			);

			if (response.status_code != 200) return {};

			// Парсим связи
			const auto relationsData = nlohmann::json::parse(response.text);
			if (filters.is_any_relations)
				if (relationsData["relations"].is_array() && !relationsData["relations"].empty()) continue;

			return_issues.push_back(issueId.c_str());
		}
		return return_issues;
	}

	void push_to_working_issue(const helper::issues_array& issues_arr)
	{
		for (const auto& current_issue : issues_arr)
		{
			const auto issueDataSup{ helper::GetSupIssueData()};
			helper::UpdateIssue(current_issue.c_str(), issueDataSup);

			const auto newTaskId{ helper::copy(current_issue.c_str())};
			if (!newTaskId) continue;

			const auto issueDataDev{ helper::GetDevIssueData()};
			helper::UpdateIssue(newTaskId->c_str(), issueDataDev);

			std::println("Задача: {}, скопированна в {}", current_issue, newTaskId->c_str());
		}
	}
}

namespace helper
{
	//issues_array delay_find_issues(const std::chrono::seconds duration, const issue_filters& data)
	//{
	//	std::this_thread::sleep_for(duration);
	//	return ::find_issues(data);
	//}


	void Run()
	{
		while (true)
		{

			const auto issues_arr{ ::find_issues(csp_filters) };
			if (issues_arr.empty())
				std::println("Не удалось найти новые задачи!");
			else
				::push_to_working_issue(issues_arr);

			const std::chrono::minutes duration(5);
			std::this_thread::sleep_for(duration);
		}
	}


	//---------------example----------------
	std::atomic<bool> isRunning(true);

	// Функция, выполняемая в потоке
	void workerFunction(int threadId)
	{
		while (isRunning)
		{
			std::cout << "Работа потока " << threadId << "...\n";
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		std::cout << "Поток " << threadId << " завершён.\n";
	}
}
