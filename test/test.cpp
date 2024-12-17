#include <gtest/gtest.h>
#include <json_helper.h>

TEST(Json, InitilizedTest)
{
	const auto json{ nlohmann::json::parse("{}") };
	ASSERT_TRUE(json.empty());
}

