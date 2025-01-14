#pragma once

#include <thread>
#include <chrono>
#include <vector>
#include <functional>
#include <resp_helper.h>
#include <tgbot/tgbot.h>
#include <string_view>



#include <atomic>
#include <iostream>


namespace helper
{
	constexpr const int NOT_SUITABLE{ -1 };
	constexpr const char* TELEGRAM_KEY_PATH{ "TEL\\key.txt" };

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

	helper::issues_array find_issues(const helper::issue_filters& filters)
	{
		const auto key{ helper::loadApiKey(helper::API_PATH) };
		if (helper::is_key_bad(key)) return {};

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
		{
			std::println("Запрос на получения списка задач закончился не удачей! \n Статус код: {}\n URL: {}",
				response.status_code, url);
			return {};
		}

		const auto issues = nlohmann::json::parse(response.text);
		helper::issues_array return_issues{};

		for (const auto& issue : issues["issues"])
		{
			auto issueId{ std::to_string(issue[helper::id].get<int>()) };

			std::string relationsUrl = std::format("{}/issues/{}/relations.json", helper::REDMINE_URL, issueId);
			response = cpr::Get(
				cpr::Url{ relationsUrl },
				cpr::VerifySsl{ false },
				cpr::Header{ {helper::Content_Type, helper::application_json},
							 {helper::X_Redmine_API_Key, key->c_str()} }

			);

			if (response.status_code != 200)
			{
				std::println("Не удалось получить список связанных задач у {}, статус код: {}",
					issueId, response.status_code);
				continue;
			}

			const auto relationsData = nlohmann::json::parse(response.text);
			if (filters.is_any_relations)
				if (relationsData["relations"].is_array() && !relationsData["relations"].empty()) continue;

			return_issues.push_back(issueId.c_str());
		}
		return return_issues;
	}

	std::string formatting_msg_copy_issue(const std::string_view current_issue,const std::string_view new_issue)
	{
		const auto key{ helper::loadApiKey(helper::API_PATH) };
		if (helper::is_key_bad(key)) return {};

		const auto response{ helper::GetIssue(new_issue.data()) };
		if (!response) return std::format("Ошибка при получении названия у задачи {}", new_issue);

		const auto data{ nlohmann::json::parse(response->text) };
		const auto subject{ data[helper::issue][helper::subject] };

		return std::format("РќРѕРІР°СЏ Р·Р°РґР°С‡Р°: {}/issues/{} : {}", helper::REDMINE_URL, new_issue, subject.dump().c_str());
	}

	void send_msg_copy_issue(const std::string_view current_issue,const std::string_view new_issue)
	{
		std::println("Скопировал задачу {} в {}", current_issue, new_issue);

		const auto msg{ formatting_msg_copy_issue(current_issue, new_issue) };
		helper::sendMessageToTelegram(msg);
	}

	void push_to_working_issue(const helper::issues_array& issues_arr)
	{
		for (const auto& current_issue : issues_arr)
		{
			const auto issueDataSup{ helper::GetSupIssueData() };
			helper::UpdateIssue(current_issue, issueDataSup);

			const auto newTaskId{ helper::copy(current_issue.c_str()) };
			if (!newTaskId) continue;

			const auto issueDataDev{ helper::GetDevIssueData() };
			helper::UpdateIssue(*newTaskId, issueDataDev);

			send_msg_copy_issue(current_issue, newTaskId->c_str());
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
			const auto issues_arr{ ::find_issues(csp_filters) };
			if (issues_arr.empty())
				std::println(msg_hlp::couldnt_find_new_issues);
			else
				::push_to_working_issue(issues_arr);

			std::this_thread::sleep_for(helper::sleep_five_minutes);
		}
	}

	using str_botToken = std::string;
	using str_chatId = std::string;
	using opt_tel_key = std::optional<std::pair <str_botToken, str_chatId>>;

	opt_tel_key loadTelegramKey(const std::string& filePath)
	{
		std::ifstream file(filePath);
		if (!file.is_open())
		{
			std::println("Не удалось найти файл {}", filePath);
			return std::nullopt;
		}

		std::string botToken{};
		std::string chatIp{};

		if (std::getline(file, botToken) && std::getline(file, chatIp))
		{
			file.close();
			return std::pair{ botToken, chatIp };
		}
		else
		{
			std::println("Не удалось прочитать Апи ключ из файла: {}", filePath);
			file.close();
			return std::nullopt;
		}

		file.close();
		return std::nullopt;
	}

	void sendMessageToTelegram(const std::string_view message)
	{
		const auto key{ helper::loadTelegramKey(helper::TELEGRAM_KEY_PATH) };
		if (!key) return;
		const auto [botToken, chatId] = *key;

		TgBot::Bot bot(botToken);

		try
		{
			bot.getApi().sendMessage(chatId, message.data());
		}
		catch (const TgBot::TgException& e)
		{
			std::cerr << "Ошибка отправки сообщения: " << e.what() << std::endl;
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
