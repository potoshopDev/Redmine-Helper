#include <print>
#include <nlohmann/json.hpp>

int main()
{
	const auto json{ nlohmann::json::parse("{}") };
	std::println("Hello world");
}