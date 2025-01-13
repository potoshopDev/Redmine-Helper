#include <locale>
#include <print>
#include "app_helper.h"

int main()
{
    std::setlocale(LC_ALL,"Russian");
	std::println("Запуск каждые 5 мин....");

	helper::Run();
}