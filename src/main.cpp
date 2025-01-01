#include <print>
#include <nlohmann/json.hpp>
#include <locale>

int main()
{
	std::setlocale(LC_ALL, "ru-RU.UTF-8");
	const auto json{ nlohmann::json::parse("{}") };
	std::println("Hello world");
}