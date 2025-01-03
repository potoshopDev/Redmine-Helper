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
			{"X-Redmine-API-Key", key->c_str()},
			{"Content-Type", "application/json"} },
			cpr::Body{ newTaskJson.dump() });

	return createResponse;
}

cpr::Response DownloadAttachment(const char* ApiKey, const nlohmann::json& attachment_json)
{
	const auto fileResponse = cpr::Get(
		cpr::Url{ attachment_json["content_url"].get<std::string>() },
		cpr::Header{ {"X-Redmine-API-Key", ApiKey} },
		cpr::VerifySsl{ false }
	);

	return fileResponse;
}
cpr::Response UploadAttachment(const char* ApiKey, const cpr::Response& fileResponse)
{
	// ��������� ���� ��� ��������� ������ � ����� ������
	auto uploadResponse = cpr::Post(
		cpr::Url{ REDMINE_URL + std::string("/uploads.json") },
		cpr::Header{
			{"X-Redmine-API-Key", ApiKey},
			{"Content-Type", "application/octet-stream"}
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
			{"issue", {
				{"uploads", {{
					{"token", uploadFileJson["upload"]["token"]},
					{"filename", attachment["filename"]},
					{"description", attachment["description"]}
				}}}
			}}
		};

		const auto upload_url{ std::format("{}/issues/{}.json", REDMINE_URL, newTaskId) };

		auto associateFileResponse = cpr::Put(
			cpr::Url{ upload_url },
			cpr::Header{
				{"X-Redmine-API-Key", ApiKey},
				{"Content-Type", "application/json"}
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


	for (const auto& attachment : taskDetails["attachments"])
	{
		const auto fileResponse{ DownloadAttachment(key->c_str(), attachment) };
		if (fileResponse.status_code != httpCodes::HTTP_OK) {
			std::cerr << "������ �������� ����� " << attachment["filename"].get<std::string>()
				<< ": " << fileResponse.status_code << std::endl;
			continue;
		}

		const auto uploadResponse{ UploadAttachment(key->c_str(), fileResponse) };
		if (uploadResponse.status_code != httpCodes::HTTP_POSTOK) {
			std::println("������ �������� ����� �� ������: {}", uploadResponse.status_code);
			continue;
		}

		const auto associateFileResponse{ AssociateAttachment(key->c_str(), uploadResponse, attachment, newTaskId) };
		if (associateFileResponse.status_code != httpCodes::HTTP_OK) {
			std::println("������ ���������� ����� � �������: {}", associateFileResponse.status_code);
			return false;
		}
	}

	return true;
}

