#pragma once

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <print>
#include <format>

namespace helper
{
	constexpr const char* REDMINE_URL{ "https://qa.fogsoft.ru" };
	constexpr const char* REDMINE_URL_ISSUE = "https://qa.fogsoft.ru/issues";
	constexpr const char* ISSUE_URL_F{ "https://qa.fogsoft.ru/issues.json" };
	constexpr const char* API_PATH = "API\\API_KEY.txt";

	constexpr const char* issue{ "issue" };
	constexpr const char* project_id{ "project_id" };
	constexpr const char* project{ "project" };
	constexpr const char* subject{ "subject" };
	constexpr const char* description{ "description" };
	constexpr const char* tracker{ "tracker" };
	constexpr const char* tracker_id{ "tracker_id" };
	constexpr const char* status{ "status" };
	constexpr const char* status_id{ "status_id" };
	constexpr const char* priority{ "priority" };
	constexpr const char* priority_id{ "priority_id" };
	constexpr const char* id{ "id" };
	constexpr const char* targetIssueStatusOpen{ "1" };

	constexpr const char* attachments{ "attachments" };
	constexpr const char* filename{ "filename" };
	constexpr const char* uploads{ "uploads" };
	constexpr const char* token{ "token" };
	constexpr const char* upload{ "upload" };
	constexpr const char* X_Redmine_API_Key{ "X-Redmine-API-Key" };
	constexpr const char* Content_Type{ "Content-Type" };
	constexpr const char* application_json{ "application/json" };
	constexpr const char* content_url{ "content_url" };
	constexpr const char* application_octet{ "application/octet-stream" };
	constexpr const char* status_open{ "0" };

	constexpr const char* value{ "value" };
	constexpr const char* one{ "1" };
	constexpr const char* zero{ "0" };
	constexpr const char* empty{ "" };
	constexpr const char* dogovor{ "Р§Р°СЃС‹ РїРѕ РґРѕРіРѕРІРѕСЂСѓ СЃРѕРїСЂРѕРІРѕР¶РґРµРЅРёСЏ" };
	constexpr const char* assigned_to_id{ "assigned_to_id" };
	constexpr const char* custom_fields{ "custom_fields" };
	constexpr const char* category_id{ "category_id" };
	constexpr const char* category{ "category" };
	constexpr const char* User{ "364" };
	constexpr const char* selling{ "4578" };

	constexpr const int is_billable{ 8 };
	constexpr const int is_urgent{ 11 };
	constexpr const int billable_state{ 13 };
	constexpr const int price{ 19 };
	constexpr const int talk_client{ 35 };


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
			return REDMINE_URL_ISSUE + std::string(".json?key=") + apiKey->c_str();
		}
		return std::nullopt;
	}

	std::optional<std::string> getIssueUrl(const std::string& APIPath, const char* issue_id)
	{
		if (const auto apiKey{ loadApiKey(APIPath) })
		{
			return REDMINE_URL_ISSUE + std::string("/") + issue_id + ".json?key=" + apiKey->c_str() + "&include=attachments";
		}
		return std::nullopt;
	}

	std::optional<cpr::Response> GetIssue(const char* issue_id)
	{
		if (const auto taskUrl = getIssueUrl(API_PATH, issue_id))
		{
			auto response = cpr::Get(
				cpr::Url{ taskUrl->c_str() },
				cpr::VerifySsl{ false }
			);
			return response;
		}

		return std::nullopt;
	}

	std::optional<cpr::Response> CopyIssue(const nlohmann::json& taskDetails, const char* targetProjectIdentifier)
	{
		nlohmann::json newTaskJson = {
			{issue, {
				{project_id, targetProjectIdentifier},
				{subject, taskDetails[subject]},
				{description, taskDetails[description]},
				{tracker_id, taskDetails[tracker][id]},
				{status_id, targetIssueStatusOpen},
				{priority_id, taskDetails[priority][id]}
			}}
		};

		const auto key{ loadApiKey(API_PATH) };
		if (!key) return std::nullopt;

		auto createResponse = cpr::Post(
			cpr::Url{ ISSUE_URL_F },
			cpr::VerifySsl{ false },
			cpr::Header{
				{X_Redmine_API_Key, key->c_str()},
				{Content_Type, application_json} },
				cpr::Body{ newTaskJson.dump() });

		return createResponse;
	}

	cpr::Response DownloadAttachment(const char* ApiKey, const nlohmann::json& attachment_json)
	{
		const auto fileResponse = cpr::Get(
			cpr::Url{ attachment_json[content_url].get<std::string>() },
			cpr::Header{ {X_Redmine_API_Key, ApiKey} },
			cpr::VerifySsl{ false }
		);

		return fileResponse;
	}
	cpr::Response UploadAttachment(const char* ApiKey, const cpr::Response& fileResponse)
	{
		auto uploadResponse = cpr::Post(
			cpr::Url{ REDMINE_URL + std::string("/uploads.json") },
			cpr::Header{
				{X_Redmine_API_Key, ApiKey},
				{Content_Type, application_octet}
			},
			cpr::Body{ fileResponse.text },
			cpr::VerifySsl{ false }
		);

		return uploadResponse;
	}
	cpr::Response AssociateAttachment(const char* ApiKey, const cpr::Response& uploadResponse, const nlohmann::json& attachment, const char* newTaskId)
	{
		const auto uploadFileJson = nlohmann::json::parse(uploadResponse.text);
		nlohmann::json updateAttachments = {
			{issue, {
				{uploads, {{
					{token, uploadFileJson[upload][token]},
					{filename, attachment[filename]},
					{description, attachment[description]}
				}}}
			}}
		};

		const auto upload_url{ std::format("{}/issues/{}.json", REDMINE_URL, newTaskId) };

		auto associateFileResponse = cpr::Put(
			cpr::Url{ upload_url },
			cpr::Header{
				{X_Redmine_API_Key, ApiKey},
				{Content_Type, application_json}
			},
			cpr::Body{ updateAttachments.dump() },
			cpr::VerifySsl{ false }
		);


		return associateFileResponse;
	}


	bool CopyAttachment(const nlohmann::json& taskDetails, const char* newTaskId)
	{
		const auto key{ loadApiKey(API_PATH) };
		if (!key) return false;


		for (const auto& attachment : taskDetails[attachments])
		{
			const auto fileResponse{ DownloadAttachment(key->c_str(), attachment) };
			if (fileResponse.status_code != httpCodes::HTTP_OK) {
				std::println("Ошибка загрузка файла {}: {}", attachment[filename].get<std::string>(), fileResponse.status_code);
				return false;
			}

			const auto uploadResponse{ UploadAttachment(key->c_str(), fileResponse) };
			if (uploadResponse.status_code != httpCodes::HTTP_POSTOK) {
				std::println("Ошибка загрузки файла на сервер: {}", uploadResponse.status_code);
				return false;
			}

			const auto associateFileResponse{ AssociateAttachment(key->c_str(), uploadResponse, attachment, newTaskId) };
			if (associateFileResponse.status_code != httpCodes::HTTP_OK && associateFileResponse.status_code != httpCodes::HTTP_NO_CONTENT) {
				std::println("Ошибка ассоциации файла с задачей: {}", associateFileResponse.status_code);
				return false;
			}
		}

		return true;
	}

	std::optional<cpr::Response> AddLinkIssue(const char* FirstIssueId, const char* SecondIssueId)
	{
		const auto key{ loadApiKey(API_PATH) };
		if (!key) return std::nullopt;

		nlohmann::json linkTask = {
			{"relation", {
				{"issue_to_id", FirstIssueId},
				{"relation_type", "copied_from"},
			}}
		};

		auto link_response = cpr::Post(
			cpr::Url{ std::format("{}/{}/relations.json",REDMINE_URL_ISSUE, SecondIssueId) },
			cpr::VerifySsl{ false },
			cpr::Header{ {Content_Type, application_json},
						{X_Redmine_API_Key, key->c_str()} },
			cpr::Body{ linkTask.dump() }
		);

		if (link_response.status_code == httpCodes::HTTP_POSTOK)
		{
			return link_response;
		}

		return std::nullopt;
	}

	bool UpdateIssue(const char* IssueId, const nlohmann::json& issueData)
	{
		const auto key{ loadApiKey(API_PATH) };
		if (!key) return false;

		auto response = cpr::Put(
			cpr::Url{ std::format("{}/{}.json", REDMINE_URL_ISSUE, IssueId) },
			cpr::Header{ {Content_Type, application_json},
						 {X_Redmine_API_Key, key->c_str()} },
			cpr::VerifySsl{ false },
			cpr::Body{ issueData.dump() });

		if (response.status_code == httpCodes::HTTP_OK || response.status_code == httpCodes::HTTP_NO_CONTENT)
			return true;

		return false;
	}

	nlohmann::json GetSupIssueData()
	{


		nlohmann::json customFields = nlohmann::json::array({
			{{id, is_billable}, {value, one}},
			{{id, is_urgent}, {value, zero}},
			{{id, billable_state}, {value, dogovor}},
			{{id, price}, {value, zero}},
			{{id, talk_client}, {value, empty}}
			});


		const nlohmann::json request_body = {
				{issue, {
					{assigned_to_id, User},
					{custom_fields, customFields},
					{status_id, status_open},
					{category_id, selling}
				}}
		};

		return request_body;
	}


	std::optional<nlohmann::json> GetSupIssueData(const char* IssueId)
	{
		const auto response{ GetIssue(IssueId) };
		if (!response)
			return std::nullopt;

		const auto sourceTaskJson = nlohmann::json::parse(response->text);
		auto taskDetails = sourceTaskJson[issue];



		nlohmann::json newTaskJson = {
			{issue, {
				{id, taskDetails[id]},
				{assigned_to_id, User},
				{status_id, status_open},
				{custom_fields, taskDetails[custom_fields]},
				{category_id, taskDetails[category][id]}
		}}
		};

		return newTaskJson;
	}
}
