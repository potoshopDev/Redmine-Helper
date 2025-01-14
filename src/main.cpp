#include <locale>
#include <print>
#include "app_helper.h"

int main()
{
    std::setlocale(LC_ALL,"Russian");
	std::println(msg_hlp::run_every_five_minutes);

	helper::Run();
}