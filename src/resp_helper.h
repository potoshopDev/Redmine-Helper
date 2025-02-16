#pragma once

#include <cpr/cpr.h>
#include <optional>
#include <string>
#include <string_view>
#include <nlohmann/json.hpp>
#include <print>

namespace msg_hlp
{
constexpr const char* run_every_five_minutes{"Запуск каждые 5 мин...."};
constexpr const char* couldnt_find_new_issues{"Не удалось найти новые задачи!"};
constexpr const char* couldnt_copy_issues{"Не удалось скопировать задачу: {}!"};
constexpr const char* couldnt_find_key{"Нет ключа в файле!"};
constexpr const char* err_upload_file{"Ошибка загрузки файла на сервер: {}"};
constexpr const char* err_download_file{"Ошибка загрузка файла {}: {}"};
constexpr const char* err_associate_file{"Ошибка ассоциации файла с задачей: {}"};
constexpr const char* err_response_find{"Запрос на получения списка задач закончился не удачей! \n Статус код: {}\n URL: {}"};
}  // namespace msg_hlp

namespace helper
{
std::optional<nlohmann::json> GetCopyIssueData(const std::string_view IssueId, const std::string_view targetProjectIdentifier);
bool is_key_bad(const std::optional<std::string>& key);
std::optional<cpr::Response> AddLinkIssue(const std::string_view FirstIssueId, const std::string_view SecondIssueId);
bool print_err_associate_file(const cpr::Response& associateFileResponse);
std::string put_url_update_issue(const std::string_view issue_id);
bool is_status_code_nook(const cpr::Response& response);
cpr::Response get_response_k(const std::string_view url, const std::string_view ApiKey);
void sendMessageToTelegram(const std::string_view message);

constexpr const char* fmt_put_response{"{}/{}.json"};
constexpr const char* fmt_get_issues_url{"{}/{}.json?key={}&include=attachments"};
constexpr const char* fmt_relation_url{"{}/{}/relations.json"};
constexpr const char* fmt_relation_issue_url{"{}/issues/{}/relations.json"};
constexpr const char* fmt_put_update_issue{"{}/issues/{}.json"};
constexpr const char* fmt_post_uploads{"{}/uploads.json"};

constexpr const char* REDMINE_URL{"https://qa.fogsoft.ru"};
constexpr const char* REDMINE_URL_ISSUE = "https://qa.fogsoft.ru/issues";
constexpr const char* ISSUE_URL_F{"https://qa.fogsoft.ru/issues.json"};
constexpr const char* API_PATH = "API\\API_KEY.txt";
constexpr const char* YANDEX_URL = "https://yandex.ru";

constexpr const char* issue{"issue"};
constexpr const char* issues{"issues"};
constexpr const char* relations{"relations"};
constexpr const char* project_id{"project_id"};
constexpr const char* project{"project"};
constexpr const char* name{"name"};
constexpr const char* subject{"subject"};
constexpr const char* description{"description"};
constexpr const char* fixed_version{"fixed_version"};
constexpr const char* tracker{"tracker"};
constexpr const char* tracker_id{"tracker_id"};
constexpr const char* assigned_to{"assigned_to"};
constexpr const char* status{"status"};
constexpr const char* status_id{"status_id"};
constexpr const char* priority{"priority"};
constexpr const char* priority_id{"priority_id"};
constexpr const char* id{"id"};
constexpr const char* author{"author"};
constexpr const char* targetIssueStatusOpen{"1"};

constexpr const char* attachments{"attachments"};
constexpr const char* filename{"filename"};
constexpr const char* uploads{"uploads"};
constexpr const char* token{"token"};
constexpr const char* upload{"upload"};
constexpr const char* X_Redmine_API_Key{"X-Redmine-API-Key"};
constexpr const char* Content_Type{"Content-Type"};
constexpr const char* application_json{"application/json"};
constexpr const char* content_url{"content_url"};
constexpr const char* application_octet{"application/octet-stream"};
constexpr const char* status_open{"1"};
constexpr const char* status_inprogress{"2"};
constexpr const char* fixed_version_id{"fixed_version_id"};
// constexpr const char* sup{"5081"};
constexpr const char* sup{"7242"};

constexpr const char* value{"value"};
constexpr const char* one{"1"};
constexpr const char* zero{"0"};
constexpr const char* empty{""};
constexpr const char* dogovor{"Р§Р°СЃС‹ РїРѕ РґРѕРіРѕРІРѕСЂСѓ СЃРѕРїСЂРѕРІРѕР¶РґРµРЅРёСЏ"};
constexpr const char* no_use{"РќРµ РІРЅРѕСЃРёС‚СЊ"};
constexpr const char* selling_issue{"РџР»Р°С‚РЅР°СЏ Р·Р°РґР°С‡Р°"};
constexpr const char* assigned_to_id{"assigned_to_id"};
constexpr const char* custom_fields{"custom_fields"};
constexpr const char* category_id{"category_id"};
constexpr const char* category{"category"};
constexpr const char* User{"364"};
constexpr const char* User2{"102"};
constexpr const char* User3{"172"};
constexpr const char* User4{"297"};
constexpr const char* selling{"4578"};
constexpr const char* projectKdev{"600"};
constexpr const char* projectKsup{"446"};

constexpr const int is_billable{8};
constexpr const int is_urgent{11};
constexpr const int billable_state{13};
constexpr const int price{19};
constexpr const int talk_client{35};
constexpr const int release_notes{51};
constexpr const int finance{52};

std::optional<std::string> loadApiKey(const std::string& filePath);

std::optional<std::string> getRedmineURL(const std::string& filePath);

std::optional<std::string> getIssueUrl(const std::string& APIPath, const std::string_view issue_id);

std::optional<cpr::Response> GetIssue(const std::string_view issue_id);

std::optional<cpr::Response> CopyIssue(const std::string_view issue_id, const std::string_view project_id);

cpr::Response DownloadAttachment(const std::string_view ApiKey, const nlohmann::json& attachment_json);

cpr::Response UploadAttachment(const std::string_view ApiKey, const cpr::Response& fileResponse);

cpr::Response AssociateAttachment(
    const std::string_view ApiKey, const cpr::Response& uploadResponse, const nlohmann::json& attachment, const std::string_view newTaskId);

bool CopyAttachment(const nlohmann::json& taskDetails, const std::string_view newTaskId);

bool UpdateIssue(const std::string_view IssueId, const nlohmann::json& issueData);

nlohmann::json GetSupIssueData();

nlohmann::json GetDevIssueData();

std::optional<nlohmann::json> GetSupIssueData(const char* IssueId);

std::optional<std::string> copy(const std::string_view issue_id);

bool check_response(std::string_view URL);

constexpr const char* TELEGRAM_KEY_PATH{"TEL\\key.txt"};
using str_botToken = std::string;
using str_chatId = std::string;
using opt_tel_key = std::optional<std::pair<str_botToken, str_chatId>>;

opt_tel_key loadTelegramKey(const std::string& filePath);

}  // namespace helper

