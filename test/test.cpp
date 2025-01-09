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

	const auto redmine_url{ helper::getRedmineURL(helper::API_PATH) };
	ASSERT_TRUE(redmine_url);

	cpr::Response response = cpr::Get(cpr::Url{ redmine_url->c_str() }, cpr::VerifySsl{ false });
	ASSERT_TRUE(response.status_code == httpCodes::HTTP_OK);

	const auto redmineResponse{ parse<IssuesList>(response.text) };
	ASSERT_TRUE(redmineResponse->issues.size() > 0);
}


TEST(REDMINE, GETISSUES)
{
	const auto issue_id{ "215488" };
	const auto response{ helper::GetIssue(issue_id) };

	ASSERT_TRUE(response && response->status_code == httpCodes::HTTP_OK);
}

TEST(REDMINE, PARSEISSUE)
{
	const auto issue_id{ "215488" };
	const auto response{ helper::GetIssue(issue_id) };

	ASSERT_TRUE(response && response->status_code == httpCodes::HTTP_OK);

	const auto sourceTaskJson = nlohmann::json::parse(response->text);
	auto taskDetails = sourceTaskJson[helper::issue];
	const auto targetProjectIdentifier{ "999999" };

	nlohmann::json newTaskJson = {
		{helper::issue, {
			{helper::project_id, targetProjectIdentifier},
			{helper::subject, taskDetails[helper::subject]},
			{helper::description, taskDetails[helper::description]},
			{helper::tracker_id, taskDetails[helper::tracker][helper::id]},
			{helper::status_id, taskDetails[helper::status][helper::id]},
			{helper::priority_id, taskDetails[helper::priority][helper::id]}
		}}
	};

	ASSERT_TRUE(newTaskJson[helper::issue][helper::project_id] == targetProjectIdentifier);
}

TEST(REDMINE, COPYISSUE)
{
	const auto issue_id{ "219532" };
	const auto targetProjectIdentifier{ helper::projectKdev };

	const auto createResponse{ helper::CopyIssue(issue_id, targetProjectIdentifier) };
	ASSERT_TRUE(createResponse->status_code == httpCodes::HTTP_POSTOK);

	const auto createdTaskJson = nlohmann::json::parse(createResponse->text);
	const auto newTaskId = createdTaskJson[helper::issue][helper::id];

	const auto linkResponse{ helper::AddLinkIssue(issue_id, newTaskId.dump().c_str()) };
	ASSERT_TRUE(linkResponse);

	const auto response{ helper::GetIssue(issue_id) };
	ASSERT_TRUE(response);

	const auto sourceTaskJson = nlohmann::json::parse(response->text);
	auto taskDetails = sourceTaskJson[helper::issue];

	const auto copyAttachmentResult{ helper::CopyAttachment(taskDetails, newTaskId.dump().c_str()) };
	ASSERT_TRUE(copyAttachmentResult);
}

TEST(REDMINE, UPDATEISSUESUP)
{
	const auto issue_id{ "219532" };
	const auto issueData{ helper::GetSupIssueData()};

	const auto updateResult{ helper::UpdateIssue(issue_id, issueData) };
	ASSERT_TRUE(updateResult);
}

TEST(REDMINE, UPDATEISSUEDEV)
{
	const auto issue_id{ "219771" };
	const auto issueData{ helper::GetDevIssueData()};

	const auto updateResult{ helper::UpdateIssue(issue_id, issueData) };
	ASSERT_TRUE(updateResult);
}
