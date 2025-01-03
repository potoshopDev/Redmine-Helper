#pragma once

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <print>
#include <format>

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


std::optional<std::string> loadApiKey(const std::string& filePath)
{
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		std::println("�� ������� ����� ���� {}", filePath);
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
		std::println("�� ������� ��������� ��� ���� �� �����: {}", filePath);
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
			std::println("������ �������� ����� {}: {}", attachment[filename].get<std::string>(), fileResponse.status_code);
			return false;
		}

		const auto uploadResponse{ UploadAttachment(key->c_str(), fileResponse) };
		if (uploadResponse.status_code != httpCodes::HTTP_POSTOK) {
			std::println("������ �������� ����� �� ������: {}", uploadResponse.status_code);
			return false;
		}

		const auto associateFileResponse{ AssociateAttachment(key->c_str(), uploadResponse, attachment, newTaskId) };
		if (associateFileResponse.status_code != httpCodes::HTTP_OK && associateFileResponse.status_code != httpCodes::HTTP_NO_CONTENT) {
			std::println("������ ���������� ����� � �������: {}", associateFileResponse.status_code);
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
		cpr::Url{ std::format("{}/{}/relations.json",REDMINE_URL_ISSUE, SecondIssueId ) },
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
