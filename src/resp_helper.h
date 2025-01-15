#pragma once

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <print>
#include <format>
#include "json_helper.h"
#include <string_view>

namespace msg_hlp
{
	constexpr const char* run_every_five_minutes{ "Запуск каждые 5 мин...." };
	constexpr const char* couldnt_find_new_issues{ "Не удалось найти новые задачи!" };
	constexpr const char* couldnt_copy_issues{ "Не удалось скопировать задачу: {}!" };
	constexpr const char* couldnt_find_key{ "Нет ключа в файле!" };
	constexpr const char* err_upload_file{ "Ошибка загрузки файла на сервер: {}" };
	constexpr const char* err_download_file { "Ошибка загрузка файла {}: {}" };
	constexpr const char* err_associate_file { "Ошибка ассоциации файла с задачей: {}" };
}

namespace
{
	bool is_status_code_ok(const cpr::Response& response);
	bool is_status_code_nook(const cpr::Response& response);
	cpr::Response put_response(const std::string_view url, const std::string_view IssueId, const std::string_view key, const nlohmann::json& issueData);
	const std::optional<std::string> print_couldnt_copy_issue(const std::string_view issue_id);
	cpr::Response get_response_nk(const std::string_view taskUrl);
	cpr::Response post_response(const std::string_view url, const std::string_view key, const std::optional<nlohmann::json>& newTaskJson);
	const std::string get_issueId_to_json(const nlohmann::json& createdTaskJson);
	nlohmann::json get_relation_data(const std::string_view issue_id);
	cpr::Response get_response_k(const std::string_view url, const std::string_view ApiKey);
	bool print_error_download_file(const nlohmann::json& attachment, const cpr::Response& fileResponse);
	bool print_err_upload_file(const cpr::Response& uploadResponse);
	bool print_err_associate_file(const cpr::Response& associateFileResponse);
	bool __copy_attachment(const nlohmann::json& taskDetails, const std::string_view key, const std::string_view newTaskId);
}

namespace helper
{
	std::optional<nlohmann::json> GetCopyIssueData(const std::string_view IssueId, const std::string_view targetProjectIdentifier);
	bool is_key_bad(const std::optional<std::string>& key);
	std::optional<cpr::Response> AddLinkIssue(const std::string_view FirstIssueId, const std::string_view SecondIssueId);
	bool print_err_associate_file(const cpr::Response& associateFileResponse);
	std::string put_url_update_issue(const std::string_view issue_id);

	constexpr const char* fmt_put_response{ "{}/{}.json" };
	constexpr const char* fmt_get_issues_url{ "{}/{}.json?key={}&include=attachments" };
	constexpr const char* fmt_relation_url{ "{}/{}/relations.json" };
	constexpr const char* fmt_put_update_issue{ "{}/issues/{}.json" };
	constexpr const char* fmt_post_uploads { "{}/uploads.json" };

	constexpr const char* REDMINE_URL{ "https://qa.fogsoft.ru" };
	constexpr const char* REDMINE_URL_ISSUE = "https://qa.fogsoft.ru/issues";
	constexpr const char* ISSUE_URL_F{ "https://qa.fogsoft.ru/issues.json" };
	constexpr const char* API_PATH = "API\\API_KEY.txt";
	constexpr const char* YANDEX_URL = "https://yandex.ru";

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
	constexpr const char* status_open{ "1" };
	constexpr const char* status_inprogress{ "2" };
	constexpr const char* fixed_version_id{ "fixed_version_id" };
	constexpr const char* sup{ "5081" };

	constexpr const char* value{ "value" };
	constexpr const char* one{ "1" };
	constexpr const char* zero{ "0" };
	constexpr const char* empty{ "" };
	constexpr const char* dogovor{ "Р§Р°СЃС‹ РїРѕ РґРѕРіРѕРІРѕСЂСѓ СЃРѕРїСЂРѕРІРѕР¶РґРµРЅРёСЏ" };
	constexpr const char* no_use{ "РќРµ РІРЅРѕСЃРёС‚СЊ" };
	constexpr const char* selling_issue{ "РџР»Р°С‚РЅР°СЏ Р·Р°РґР°С‡Р°" };
	constexpr const char* assigned_to_id{ "assigned_to_id" };
	constexpr const char* custom_fields{ "custom_fields" };
	constexpr const char* category_id{ "category_id" };
	constexpr const char* category{ "category" };
	constexpr const char* User{ "364" };
	constexpr const char* User2{ "102" };
	constexpr const char* User3{ "172" };
	constexpr const char* selling{ "4578" };
	constexpr const char* projectKdev{ "600" };
	constexpr const char* projectKsup{ "446" };

	constexpr const int is_billable{ 8 };
	constexpr const int is_urgent{ 11 };
	constexpr const int billable_state{ 13 };
	constexpr const int price{ 19 };
	constexpr const int talk_client{ 35 };
	constexpr const int release_notes{ 51 };
	constexpr const int finance{ 52 };


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

	std::optional<std::string> getIssueUrl(const std::string& APIPath, const std::string_view issue_id)
	{
		const auto key{ helper::loadApiKey(APIPath) };
		if (helper::is_key_bad(key)) return std::nullopt;

		return std::format(helper::fmt_get_issues_url, REDMINE_URL_ISSUE, issue_id, key->c_str());
	}

	std::optional<cpr::Response> GetIssue(const std::string_view issue_id)
	{
		const auto taskUrl{ getIssueUrl(API_PATH, issue_id) };
		if (!taskUrl) return std::nullopt;

		return ::get_response_nk(*taskUrl);
	}

	std::optional<cpr::Response> CopyIssue(const std::string_view issue_id, const std::string_view project_id)
	{
		const auto newTaskJson{ GetCopyIssueData(issue_id, project_id) };
		if (!newTaskJson) return std::nullopt;

		const auto key{ loadApiKey(API_PATH) };
		if (helper::is_key_bad(key)) return std::nullopt;

		return post_response(helper::ISSUE_URL_F, *key, newTaskJson);
	}

	cpr::Response DownloadAttachment(const std::string_view ApiKey, const nlohmann::json& attachment_json)
	{
		const auto url{ attachment_json[content_url].get<std::string>() };
		return ::get_response_k(url, ApiKey);
	}

	cpr::Response UploadAttachment(const std::string_view ApiKey, const cpr::Response& fileResponse)
	{
		const auto url{ std::format(helper::fmt_post_uploads, REDMINE_URL) };
		return ::post_response(url, ApiKey, fileResponse.text);
	}

	cpr::Response AssociateAttachment(const std::string_view ApiKey,
		const cpr::Response& uploadResponse, const nlohmann::json& attachment, const std::string_view newTaskId)
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

		const auto upload_url{ std::format(helper::fmt_put_update_issue, REDMINE_URL, newTaskId) };
		return ::put_response(upload_url, newTaskId, ApiKey, updateAttachments);
	}

	bool CopyAttachment(const nlohmann::json& taskDetails, const std::string_view newTaskId)
	{
		const auto key{ loadApiKey(API_PATH) };
		if (helper::is_key_bad(key)) return false;

		return 	__copy_attachment(taskDetails, *key, newTaskId);
	}

	std::optional<cpr::Response> AddLinkIssue(const std::string_view FirstIssueId, const std::string_view SecondIssueId)
	{
		const auto key{ loadApiKey(API_PATH) };
		if (helper::is_key_bad(key)) return std::nullopt;

		nlohmann::json linkTask{ ::get_relation_data(FirstIssueId) };
		const auto url{ std::format(helper::fmt_relation_url,REDMINE_URL_ISSUE, SecondIssueId) };
		const auto link_response{ ::post_response(url, *key, linkTask) };

		if (::is_status_code_ok(link_response))
			return link_response;

		return std::nullopt;
	}

	bool UpdateIssue(const std::string_view IssueId, const nlohmann::json& issueData)
	{
		const auto key{ loadApiKey(API_PATH) };
		if (helper::is_key_bad(key)) return false;

		const auto url{ helper::put_url_update_issue(IssueId) };
		const auto response{ ::put_response(url, IssueId, *key, issueData) };

		if (::is_status_code_ok(response))
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
					{status_id, status_inprogress},
					{category_id, selling},
					{fixed_version_id, sup}
				}}
		};

		return request_body;
	}

	nlohmann::json GetDevIssueData()
	{
		nlohmann::json customFields = nlohmann::json::array({
			{{id, release_notes}, {value, no_use}},
			{{id, finance}, {value, selling_issue}}
			});

		const nlohmann::json request_body = {
				{issue, {
				//{assigned_to_id, User2},
				{assigned_to_id, User3},
				{custom_fields, customFields},
				{status_id, status_open},
			}}
		};

		return request_body;
	}

	std::optional<nlohmann::json> GetCopyIssueData(const std::string_view IssueId, const std::string_view targetProjectIdentifier)
	{
		const auto response{ helper::GetIssue(IssueId) };
		if (!response && !response->status_code == httpCodes::HTTP_OK) return std::nullopt;

		const auto sourceTaskJson = nlohmann::json::parse(response->text);
		const auto taskDetails = sourceTaskJson[helper::issue];

		nlohmann::json newTaskJson = {
			{issue, {
				{project_id, targetProjectIdentifier.data()},
				{subject, taskDetails[subject]},
				{description, taskDetails[description]},
				{tracker_id, taskDetails[tracker][id]},
				{status_id, targetIssueStatusOpen},
				{priority_id, taskDetails[priority][id]}
			}}
		};

		return newTaskJson;
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
				{status_id, taskDetails[status][id]},
				{custom_fields, taskDetails[custom_fields]},
				{category_id, taskDetails[category][id]}
		}}
		};

		return newTaskJson;
	}

	std::optional<std::string> copy(const std::string_view issue_id)
	{
		const auto targetProjectIdentifier{ helper::projectKdev };

		const auto createResponse{ helper::CopyIssue(issue_id, targetProjectIdentifier) };
		if (!createResponse) return print_couldnt_copy_issue(issue_id);

		const auto createdTaskJson{ nlohmann::json::parse(createResponse->text) };
		const auto newTaskId{ ::get_issueId_to_json(createdTaskJson) };

		const auto linkResponse{ helper::AddLinkIssue(issue_id, newTaskId) };
		if (!linkResponse) return newTaskId;

		const auto response{ helper::GetIssue(issue_id) };
		if (!response) return newTaskId;

		const auto sourceTaskJson = nlohmann::json::parse(response->text);
		auto taskDetails = sourceTaskJson[helper::issue];

		const auto copyAttachmentResult{ helper::CopyAttachment(taskDetails, newTaskId.c_str()) };
		if (!copyAttachmentResult) return newTaskId;

		return newTaskId;
	}

	bool check_response(std::string_view URL)
	{
		std::print("Проверяю запрос {}...", URL);
		cpr::Response response = cpr::Get(cpr::Url{ URL }, cpr::VerifySsl{ false });
		if (response.status_code == httpCodes::HTTP_OK || response.status_code == httpCodes::HTTP_NO_CONTENT)
		{
			std::println("OK");
		}
		else
		{
			std::println("Ошибка. status code: {};", response.status_code);
			return false;
		}

		return true;
	}

	bool is_key_bad(const std::optional<std::string>& key)
	{
		if (!key) return true;
		if (key->empty()) { std::println(msg_hlp::couldnt_find_key); return true; }

		return false;
	}

	std::string put_url_update_issue(const std::string_view issue_id)
	{
		return std::format(helper::fmt_put_response, helper::REDMINE_URL_ISSUE, issue_id);
	}
}

namespace
{
	bool is_status_code_ok(const cpr::Response& response)
	{
		return response.status_code == httpCodes::HTTP_OK || response.status_code == httpCodes::HTTP_NO_CONTENT;
	}

	cpr::Response put_response(const std::string_view url, const std::string_view IssueId, const std::string_view key, const nlohmann::json& issueData)
	{
		return cpr::Put(
			cpr::Url{ url.data() },
			cpr::Header{ { helper::Content_Type, helper::application_json },
							{ helper::X_Redmine_API_Key, key.data()} },
			cpr::VerifySsl{ false },
			cpr::Body{ issueData.dump() });
	}

	const std::optional<std::string> print_couldnt_copy_issue(const std::string_view issue_id)
	{
		std::println(msg_hlp::couldnt_copy_issues, issue_id);
		return std::nullopt;
	}

	cpr::Response get_response_nk(const std::string_view taskUrl)
	{
		return cpr::Get(
			cpr::Url{ taskUrl.data() },
			cpr::VerifySsl{ false }
		);
	}

	cpr::Response post_response(const std::string_view url, const std::string_view key, const std::optional<nlohmann::json>& newTaskJson)
	{
		return cpr::Post(
			cpr::Url{ url.data() },
			cpr::VerifySsl{ false },
			cpr::Header{
				{ helper::X_Redmine_API_Key, key.data()},
				{ helper::Content_Type, helper::application_json } },
				cpr::Body{ newTaskJson->dump() });
	}

	const std::string get_issueId_to_json(const nlohmann::json& createdTaskJson)
	{
		return std::to_string(createdTaskJson[helper::issue][helper::id].get<int>());
	}

	nlohmann::json get_relation_data(const std::string_view issue_id)
	{
		return {
					{"relation", {
						{"issue_to_id", issue_id.data()},
						{"relation_type", "copied_from"},
					}}
		};
	}

	cpr::Response get_response_k(const std::string_view url, const std::string_view ApiKey)
	{
		return	cpr::Get(
						cpr::Url{url.data()},
						cpr::Header{ { helper::X_Redmine_API_Key, ApiKey.data() } },
						cpr::VerifySsl{ false }
		);
	}

	bool is_status_code_nook(const cpr::Response& response)
	{
		return !(::is_status_code_ok(response));
	}

	bool print_error_download_file(const nlohmann::json& attachment, const cpr::Response& fileResponse)
	{
		std::println(msg_hlp::err_download_file, attachment[helper::filename].get<std::string>(), fileResponse.status_code);
		return false;
	}

	bool print_err_upload_file(const cpr::Response& uploadResponse)
	{
		std::println(msg_hlp::err_upload_file, uploadResponse.status_code);
		return false;
	}

	bool print_err_associate_file(const cpr::Response& associateFileResponse)
	{
		std::println("Ошибка ассоциации файла с задачей: {}", associateFileResponse.status_code);
		return false;
	}

	bool __copy_attachment(const nlohmann::json& taskDetails, const std::string_view key, const std::string_view newTaskId)
	{
		for (const auto& attachment : taskDetails[helper::attachments])
		{
			const auto fileResponse{ helper::DownloadAttachment(key, attachment) };
			if (::is_status_code_nook(fileResponse))
				return print_error_download_file(attachment, fileResponse);

			const auto uploadResponse{ helper::UploadAttachment(key, fileResponse) };
			if (::is_status_code_nook(uploadResponse))
				return ::print_err_upload_file(uploadResponse);

			const auto associateFileResponse{ helper::AssociateAttachment(key, uploadResponse, attachment, newTaskId) };
			if (::is_status_code_nook(associateFileResponse))
				return ::print_err_associate_file(associateFileResponse);
		}

		return true;
	}
}


