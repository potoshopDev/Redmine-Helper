#include "uiwin.h"


//
//#include <vector>
//#include <string>
//
//// Структура данных для управления окнами
//struct WindowData {
//	std::string title;                               // Заголовок окна
//	std::function<void()> contentRenderer;           // Функция для отрисовки содержимого окна
//};
//
//// Основная структура для управления окнами
//std::vector<WindowData> windows;
//
//// Добавление нового окна в динамический список
//void AddWindow(const std::string& title, const std::function<void()>& renderer) {
//	windows.push_back({ title, renderer });
//}
//
//// Удаление окна по заголовку
//void RemoveWindow(const std::string& title) {
//	windows.erase(std::remove_if(windows.begin(), windows.end(),
//		[&title](const WindowData& window) { return window.title == title; }),
//		windows.end());
//}
//
//// Основной цикл отрисовки ImGui
//void RenderWindows() {
//	for (const auto& window : windows) {
//		ImGui::Begin(window.title.c_str());          // Создаём окно
//		window.contentRenderer();                   // Отображаем содержимое окна (вызов функции из структуры)
//		ImGui::End();                               // Завершаем отрисовку окна
//	}
//}
//
//// Пример запуска программы
//void RunExample() {
//	// Добавляем первое окно
//	AddWindow("Окно 1", []() {
//		static float f = 0.0f;
//		ImGui::SliderFloat("Ползунок", &f, 0.0f, 1.0f);
//		ImGui::Text("Тестовое окно 1");
//		});
//
//	// Добавляем второе окно
//	AddWindow("Окно 2", []() {
//		ImGui::Text("Привет из второго тестового окна!");
//		if (ImGui::Button("Добавить новое окно")) {
//			AddWindow("Новое окно", []() {
//				ImGui::Text("Содержимое нового окна");
//				});
//		}
//		});
//
//	// Добавляем третье окно
//	AddWindow("Окно 3", []() {
//		ImGui::Text("Это окно №3");
//		if (ImGui::Button("Удалить это окно")) {
//			RemoveWindow("Окно 3");
//		}
//		});
//}
//
//int main() {
//	// Контекст ImGui (создание контекста, настройка окна и т.д.)
//	IMGUI_CHECKVERSION();
//	ImGui::CreateContext();
//	ImGui::StyleColorsDark();
//
//	// Запускаем тестовые окна
//	RunExample();
//
//	// Основной цикл рендера (в реальной программе будет игровой/основной цикл)
//	while (true) {
//		ImGui::NewFrame();
//
//		// Отрисовываем все окна
//		RenderWindows();
//
//		ImGui::Render();
//		// Здесь должен быть код рендера вашего бэкенда OpenGL/DirectX/Vulkan/etc.
//	}
//
//	ImGui::DestroyContext();
//	return 0;
//}
//
//
//#include "imgui.h"
//#include <vector>
//#include <string>
//#include <algorithm>
//#include <functional>
//
//class WindowManager {
//public:
//	struct WindowData {
//		std::string title;                               // Заголовок окна
//		std::function<void()> contentRenderer;           // Функция для отрисовки содержимого окна
//	};
//
//	// Добавление нового окна
//	void AddWindow(const std::string& title, const std::function<void()>& renderer) {
//		windows.emplace_back(WindowData{ title, renderer });
//	}
//
//	// Удаление окна по заголовку
//	void RemoveWindow(const std::string& title) {
//		windows.erase(std::remove_if(windows.begin(), windows.end(),
//			[&title](const WindowData& window) { return window.title == title; }),
//			windows.end());
//	}
//
//	// Отрисовка всех окон
//	void RenderWindows() {
//		for (const auto& window : windows) {
//			ImGui::Begin(window.title.c_str());          // Создаём окно
//			window.contentRenderer();                   // Отображаем содержимое окна
//			ImGui::End();                               // Завершаем отрисовку окна
//		}
//	}
//
//private:
//	std::vector<WindowData> windows;                     // Хранилище всех окон
//};
//
//// Основной пример использования
//void RunExample(WindowManager& manager) {
//	// Добавляем первое окно
//	manager.AddWindow("Окно 1", []() {
//		static float f = 0.0f;
//		ImGui::SliderFloat("Ползунок", &f, 0.0f, 1.0f);
//		ImGui::Text("Тестовое окно 1");
//		});
//
//	// Добавляем второе окно
//	manager.AddWindow("Окно 2", [&manager]() {
//		ImGui::Text("Привет из второго тестового окна!");
//		if (ImGui::Button("Добавить новое окно")) {
//			manager.AddWindow("Новое окно", []() {
//				ImGui::Text("Содержимое нового окна");
//				});
//		}
//		});
//
//	// Добавляем третье окно
//	manager.AddWindow("Окно 3", [&manager]() {
//		ImGui::Text("Это окно №3");
//		if (ImGui::Button("Удалить это окно")) {
//			manager.RemoveWindow("Окно 3");
//		}
//		});
//}
//
//int main() {
//	// Контекст ImGui (создание контекста, настройка окна и т.д.)
//	IMGUI_CHECKVERSION();
//	ImGui::CreateContext();
//	ImGui::StyleColorsDark();
//
//	WindowManager manager;         // Создаём менеджер окон
//	RunExample(manager);           // Заполняем менеджер окнами
//
//	// Основной цикл рендера
//	while (true) {
//		ImGui::NewFrame();
//
//		// Отрисовываем все окна через менеджер
//		manager.RenderWindows();
//
//		ImGui::Render();
//		// Здесь должен быть код рендера вашего бэкенда OpenGL/DirectX/Vulkan/etc.
//	}
//
//	ImGui::DestroyContext();
//	return 0;
//}
//
