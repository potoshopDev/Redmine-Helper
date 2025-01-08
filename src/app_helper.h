#pragma once

#include <thread>
#include <chrono>
#include <vector>
#include <functional>



#include <atomic>
#include <iostream>

namespace helper
{

	using issues_array = std::vector<const std::string>;
	using issues_data = std::vector<std::pair<std::string, std::string>>;
}

namespace
{
	helper::issues_array find_issues(const helper::issues_data& data)
	{
		return helper::issues_array();
	}
}

namespace helper
{
	issues_array delay_find_issues(const std::chrono::seconds duration, const issues_data& data)
	{
		std::this_thread::sleep_for(duration);
		return ::find_issues(data);
	}




	//---------------example----------------
	std::atomic<bool> isRunning(true); 

	// Функция, выполняемая в потоке
	void workerFunction(int threadId)
	{
		while (isRunning)
		{
			std::cout << "Работа потока " << threadId << "...\n";
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		std::cout << "Поток " << threadId << " завершён.\n";
	}
}
