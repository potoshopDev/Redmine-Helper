#include <gtest/gtest.h>
#include <json_helper.h>
#include "resp_helper.h"


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

TEST(REDMINE, COPYISSUE)
{
	const auto issue_id{ "215488" };
	const auto response{ GetIssue(issue_id) };

	ASSERT_TRUE(response && response->status_code == httpCodes::HTTP_OK);

	auto sourceTaskJson = nlohmann::json::parse(response->text);
	auto taskDetails = sourceTaskJson[issue];

	const auto targetProjectIdentifier{ "508" };

	const auto createResponse{ CopyIssue(taskDetails, targetProjectIdentifier) };
	ASSERT_TRUE(createResponse->status_code == httpCodes::HTTP_POSTOK);

	const auto createdTaskJson = nlohmann::json::parse(createResponse->text);
	const auto newTaskId = createdTaskJson[issue][id];

	const auto copyAttachmentResult{ CopyAttachment(taskDetails, newTaskId.dump().c_str()) };
	ASSERT_TRUE(copyAttachmentResult);
}

