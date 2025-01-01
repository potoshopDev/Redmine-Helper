#include <gtest/gtest.h>
#include <json_helper.h>
#include <print>


struct ChuckResponse
{
	std::string value;
	std::string url;
	std::string id;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(ChuckResponse, value, url, id);
};

constexpr const char* CHUCK_URL = "https://api.chucknorris.io/jokes/random";

TEST(Json, InitilizedTest)
{
	const auto json{ nlohmann::json::parse("{}") };
	ASSERT_TRUE(json.empty());
}

TEST(Json, STATUSCODE)
{
	cpr::SslOptions ssl_options;
	ssl_options.verify_peer = false;

	cpr::Response response = cpr::Get(cpr::Url{ CHUCK_URL }, cpr::VerifySsl{ false });
	std::println("status code: {};", response.status_code);
	ASSERT_TRUE(response.status_code == httpCodes::HTTP_OK);
}

TEST(Json, RESPONSENOTEMPTY)
{
	cpr::Response response = cpr::Get(cpr::Url{ CHUCK_URL }, cpr::VerifySsl{ false });
	ASSERT_TRUE(response.status_code == httpCodes::HTTP_OK);

	const auto chuckResponse{ parse<ChuckResponse>(response.text) };
	const auto value{ chuckResponse->value };
	std::println("Value: {}", value);
	ASSERT_TRUE(!value.empty());

}
TEST(REDMINE, RESPONSE)
{

	const auto redmine_url{ getRedmineURL(API_PATH) };
	ASSERT_TRUE(redmine_url);

	cpr::Response response = cpr::Get(cpr::Url{ redmine_url->c_str() }, cpr::VerifySsl{ false });
	ASSERT_TRUE(response.status_code == httpCodes::HTTP_OK);

	const auto redmineResponse{ parse<IssuesList>(response.text) };
	ASSERT_TRUE(redmineResponse->issues.size() > 0);
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

TEST(REDMINE, GETISSUES)
{
	const auto issue_id{ "215488" };
	const auto response{ GetIssue(issue_id) };

	ASSERT_TRUE(response && response->status_code == httpCodes::HTTP_OK);
}

TEST(REDMINE, PARSEISSUE)
{
	const auto issue_id{ "215488" };
	const auto response{ GetIssue(issue_id) };

	ASSERT_TRUE(response && response->status_code == httpCodes::HTTP_OK);

	const auto sourceTaskJson = nlohmann::json::parse(response->text);
	auto taskDetails = sourceTaskJson[issue];
	const auto targetProjectIdentifier{ "999999" };

	nlohmann::json newTaskJson = {
		{issue, {
			{project_id, targetProjectIdentifier},
			{subject, taskDetails[subject]},
			{description, taskDetails[description]},
			{tracker_id, taskDetails[tracker][id]},
			{status_id, taskDetails[status][id]},
			{priority_id, taskDetails[priority][id]}
		}}
	};

	ASSERT_TRUE(newTaskJson[issue][project_id] == targetProjectIdentifier);
}

#include <iostream>
#include <format>
#include <locale>

TEST(REDMINE, CREATEISSUE)
{
	std::setlocale(LC_ALL, "ru-RU.UTF-8");

	const auto issue_id{ "215488" };
	const auto response{ GetIssue(issue_id) };

	ASSERT_TRUE(response && response->status_code == httpCodes::HTTP_OK);

	auto sourceTaskJson = nlohmann::json::parse(response->text);
	auto taskDetails = sourceTaskJson[issue];
	const auto targetProjectIdentifier{ "508" };
	const auto targetIssueStatusOpen{ "1" };


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


	const auto newIssueURL{ getRedmineURL(API_PATH) };
	ASSERT_TRUE(newIssueURL);
	const auto url{ "https://qa.fogsoft.ru/issues.json" };
	const auto key{ loadApiKey(API_PATH) };

	auto createResponse = cpr::Post(
		cpr::Url{ url },
		cpr::VerifySsl{ false },
		cpr::Header{
			{"X-Redmine-API-Key", key->c_str()},
			{"Content-Type", "application/json"} },
			cpr::Body{ newTaskJson.dump() });

	ASSERT_TRUE(createResponse.status_code == httpCodes::HTTP_POSTOK);

	const auto createdTaskJson = nlohmann::json::parse(createResponse.text);
	const auto newTaskId = createdTaskJson["issue"]["id"];

	// Копируем вложения
	for (const auto& attachment : taskDetails["attachments"]) {
		// Скачиваем вложение
		auto fileResponse = cpr::Get(
			cpr::Url{ attachment["content_url"].get<std::string>() },
			cpr::Header{ {"X-Redmine-API-Key", key->c_str()} },
			cpr::VerifySsl{ false }
		);

		if (fileResponse.status_code != httpCodes::HTTP_OK) {
			std::cerr << "Ошибка загрузки файла " << attachment["filename"].get<std::string>()
				<< ": " << fileResponse.status_code << std::endl;
			continue;
		}

		// Загружаем файл как временный ресурс к новой задаче
		const auto serverUrl{ "https://qa.fogsoft.ru" };
		auto uploadResponse = cpr::Post(
			cpr::Url{ serverUrl + std::string("/uploads.json") },
			cpr::Header{
				{"X-Redmine-API-Key", key->c_str()},
				{"Content-Type", "application/octet-stream"}
			},
			cpr::Body{ fileResponse.text },
			cpr::VerifySsl{ false }
		);

		if (uploadResponse.status_code != 201) {
			std::cerr << "Ошибка загрузки файла на сервер: " << uploadResponse.status_code << std::endl;
			continue;
		}

		// Ассоциируем файл с новой задачей
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

		const auto newurl{ std::format("{}/issues/{}.json", serverUrl, newTaskId.dump()) };
		std::println("attach url:{}", newurl);

		auto associateFileResponse = cpr::Put(
			cpr::Url{ newurl },
			cpr::Header{
				{"X-Redmine-API-Key", key->c_str()},
				{"Content-Type", "application/json"}
			},
			cpr::Body{ updateAttachments.dump() },
			cpr::VerifySsl{ false }
		);

		if (associateFileResponse.status_code != 200) {
			std::cerr << "Ошибка ассоциации файла с задачей: " << associateFileResponse.status_code << std::endl;
		}
	}
}

