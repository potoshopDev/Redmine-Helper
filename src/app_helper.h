#pragma once

#include <thread>
#include <chrono>
#include <vector>
#include <functional>
#include <resp_helper.h>
#include <string_view>



#include <atomic>
#include <iostream>


namespace helper
{
	constexpr const int NOT_SUITABLE{ -1 };

	using issues_array = std::vector<std::string>;
	using filters = std::vector<std::pair<const char*, const char*>>;

	void sendMessageToTelegram(const std::string_view message);

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

	constexpr const std::chrono::minutes sleep_five_minutes{ 5 };
}

namespace
{
	std::string __get_url_find(const helper::issue_filters& filters);
	helper::issues_array print_err_response_find(cpr::Response& response, const std::string_view url);
	std::optional<cpr::Response> get_issue_relation(std::string_view issueId, std::string_view key);

	helper::issues_array find_issues(const helper::issue_filters& filters)
	{
		const auto key{ helper::loadApiKey(helper::API_PATH) };
		if (helper::is_key_bad(key)) return {};

		const auto url{ __get_url_find(filters) };
		auto response{ helper::get_response_k(url, *key) };

		if (helper::is_status_code_nook(response))
			return print_err_response_find(response, url);

		const auto issues = nlohmann::json::parse(response.text);
		helper::issues_array return_issues{};

		for (const auto& issue : issues[helper::issues])
		{
			auto issueId{ std::to_string(issue[helper::id].get<int>()) };
			const auto response{ get_issue_relation(issueId, *key) };
			if (!response) continue;

			const auto relationsData = nlohmann::json::parse(response->text);

			if (filters.is_any_relations)
				if (relationsData[helper::relations].is_array() && !relationsData[helper::relations].empty()) continue;

			return_issues.push_back(issueId.c_str());
		}

		return return_issues;
	}

	std::optional<cpr::Response> get_issue_relation(std::string_view issueId, std::string_view key)
	{
		const auto relationsURL{ std::format(helper::fmt_relation_issue_url, helper::REDMINE_URL, issueId) };
		const auto response = helper::get_response_k(relationsURL, key);

		if (helper::is_status_code_nook(response))
		{
			std::println("�� ������� �������� ������ ��������� ����� � {}, ������ ���: {}",
				issueId, response.status_code);
			return std::nullopt;
		}

		return response;
	}

	helper::issues_array print_err_response_find(cpr::Response& response, const std::string_view url)
	{
		std::println(msg_hlp::err_response_find, response.status_code, url);
		return {};
	}

	std::string __get_url_find(const helper::issue_filters& filters)
	{
		auto url{ std::format("{}.json", helper::REDMINE_URL_ISSUE) };
		bool flag_lit{ true };

		for (const auto& filter : filters.issue)
		{
			url = std::format("{}{}{}={}", url, flag_lit ? "?" : "&", filter.first, filter.second);
			flag_lit = false;
		}
		return url;
	}

	std::string formatting_msg_copy_issue(const std::string_view current_issue, const std::string_view new_issue)
	{
		const auto key{ helper::loadApiKey(helper::API_PATH) };
		if (helper::is_key_bad(key)) return {};

		const auto response{ helper::GetIssue(new_issue.data()) };
		if (!response) return std::format("������ ��� ��������� �������� � ������ {}", new_issue);

		const auto data{ nlohmann::json::parse(response->text) };
		const auto subject{ data[helper::issue][helper::subject] };

		return std::format("Новая задача: {}/issues/{} : {}", helper::REDMINE_URL, new_issue, subject.dump().c_str());
	}

	void send_msg_copy_issue(const std::string_view current_issue, const std::string_view new_issue)
	{
		std::println("���������� ������ {} � {}", current_issue, new_issue);

		const auto msg{ formatting_msg_copy_issue(current_issue, new_issue) };
		helper::sendMessageToTelegram(msg);
	}

	void push_to_working_issue(const helper::issues_array& issues_arr)
	{
		for (const auto& current_issue : issues_arr)
		{
			const auto issueDataSup{ helper::GetSupIssueData() };
			helper::UpdateIssue(current_issue, issueDataSup);

			const auto newTaskId{ helper::copy(current_issue) };
			if (!newTaskId) continue;

			const auto issueDataDev{ helper::GetDevIssueData() };
			helper::UpdateIssue(*newTaskId, issueDataDev);

			send_msg_copy_issue(current_issue, *newTaskId);
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
		if (!helper::check_response(helper::YANDEX_URL)) return;
		if (!helper::check_response(helper::REDMINE_URL)) return;

		while (true)
		{
			const auto issues_arr{ find_issues(csp_filters) };
			if (issues_arr.empty())
				std::println(msg_hlp::couldnt_find_new_issues);
			else
				push_to_working_issue(issues_arr);

			std::this_thread::sleep_for(helper::sleep_five_minutes);
		}
	}



	//---------------example----------------
	std::atomic<bool> isRunning(true);

	// �������, ����������� � ������
	void workerFunction(int threadId)
	{
		while (isRunning)
		{
			std::cout << "������ ������ " << threadId << "...\n";
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		std::cout << "����� " << threadId << " ��������.\n";
	}
}
