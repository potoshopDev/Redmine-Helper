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
	ssl_options.verify_peer= false;

	cpr::Response response = cpr::Get(cpr::Url{CHUCK_URL},cpr::VerifySsl{false});
	std::println("status code: {};", response.status_code);
	ASSERT_TRUE(response.status_code == httpCodes::HTTP_OK);
}

TEST(Json, RESPONSENOTEMPTY)
{
	cpr::Response response = cpr::Get(cpr::Url{ CHUCK_URL }, cpr::VerifySsl{false});
	//ASSERT_TRUE(response.status_code == httpCodes::HTTP_OK);

	const auto chuckResponse{ parse<ChuckResponse>(response.text) };
	const auto value{ chuckResponse->value };
	std::println("Value: {}", value);
	ASSERT_TRUE(!value.empty());
	
}

