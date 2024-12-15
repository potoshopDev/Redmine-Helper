#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

TEST(Json, InitilizedTest)
{
	const auto json{ nlohmann::json::parse("{}") };
	ASSERT_TRUE(json.empty());
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    const auto status = RUN_ALL_TESTS();
    return status;
}
