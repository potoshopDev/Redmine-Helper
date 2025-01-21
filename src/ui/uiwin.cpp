#include "uiwin.h"


//
//#include <vector>
//#include <string>
//
//// ��������� ������ ��� ���������� ������
//struct WindowData {
//	std::string title;                               // ��������� ����
//	std::function<void()> contentRenderer;           // ������� ��� ��������� ����������� ����
//};
//
//// �������� ��������� ��� ���������� ������
//std::vector<WindowData> windows;
//
//// ���������� ������ ���� � ������������ ������
//void AddWindow(const std::string& title, const std::function<void()>& renderer) {
//	windows.push_back({ title, renderer });
//}
//
//// �������� ���� �� ���������
//void RemoveWindow(const std::string& title) {
//	windows.erase(std::remove_if(windows.begin(), windows.end(),
//		[&title](const WindowData& window) { return window.title == title; }),
//		windows.end());
//}
//
//// �������� ���� ��������� ImGui
//void RenderWindows() {
//	for (const auto& window : windows) {
//		ImGui::Begin(window.title.c_str());          // ������ ����
//		window.contentRenderer();                   // ���������� ���������� ���� (����� ������� �� ���������)
//		ImGui::End();                               // ��������� ��������� ����
//	}
//}
//
//// ������ ������� ���������
//void RunExample() {
//	// ��������� ������ ����
//	AddWindow("���� 1", []() {
//		static float f = 0.0f;
//		ImGui::SliderFloat("��������", &f, 0.0f, 1.0f);
//		ImGui::Text("�������� ���� 1");
//		});
//
//	// ��������� ������ ����
//	AddWindow("���� 2", []() {
//		ImGui::Text("������ �� ������� ��������� ����!");
//		if (ImGui::Button("�������� ����� ����")) {
//			AddWindow("����� ����", []() {
//				ImGui::Text("���������� ������ ����");
//				});
//		}
//		});
//
//	// ��������� ������ ����
//	AddWindow("���� 3", []() {
//		ImGui::Text("��� ���� �3");
//		if (ImGui::Button("������� ��� ����")) {
//			RemoveWindow("���� 3");
//		}
//		});
//}
//
//int main() {
//	// �������� ImGui (�������� ���������, ��������� ���� � �.�.)
//	IMGUI_CHECKVERSION();
//	ImGui::CreateContext();
//	ImGui::StyleColorsDark();
//
//	// ��������� �������� ����
//	RunExample();
//
//	// �������� ���� ������� (� �������� ��������� ����� �������/�������� ����)
//	while (true) {
//		ImGui::NewFrame();
//
//		// ������������ ��� ����
//		RenderWindows();
//
//		ImGui::Render();
//		// ����� ������ ���� ��� ������� ������ ������� OpenGL/DirectX/Vulkan/etc.
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
//		std::string title;                               // ��������� ����
//		std::function<void()> contentRenderer;           // ������� ��� ��������� ����������� ����
//	};
//
//	// ���������� ������ ����
//	void AddWindow(const std::string& title, const std::function<void()>& renderer) {
//		windows.emplace_back(WindowData{ title, renderer });
//	}
//
//	// �������� ���� �� ���������
//	void RemoveWindow(const std::string& title) {
//		windows.erase(std::remove_if(windows.begin(), windows.end(),
//			[&title](const WindowData& window) { return window.title == title; }),
//			windows.end());
//	}
//
//	// ��������� ���� ����
//	void RenderWindows() {
//		for (const auto& window : windows) {
//			ImGui::Begin(window.title.c_str());          // ������ ����
//			window.contentRenderer();                   // ���������� ���������� ����
//			ImGui::End();                               // ��������� ��������� ����
//		}
//	}
//
//private:
//	std::vector<WindowData> windows;                     // ��������� ���� ����
//};
//
//// �������� ������ �������������
//void RunExample(WindowManager& manager) {
//	// ��������� ������ ����
//	manager.AddWindow("���� 1", []() {
//		static float f = 0.0f;
//		ImGui::SliderFloat("��������", &f, 0.0f, 1.0f);
//		ImGui::Text("�������� ���� 1");
//		});
//
//	// ��������� ������ ����
//	manager.AddWindow("���� 2", [&manager]() {
//		ImGui::Text("������ �� ������� ��������� ����!");
//		if (ImGui::Button("�������� ����� ����")) {
//			manager.AddWindow("����� ����", []() {
//				ImGui::Text("���������� ������ ����");
//				});
//		}
//		});
//
//	// ��������� ������ ����
//	manager.AddWindow("���� 3", [&manager]() {
//		ImGui::Text("��� ���� �3");
//		if (ImGui::Button("������� ��� ����")) {
//			manager.RemoveWindow("���� 3");
//		}
//		});
//}
//
//int main() {
//	// �������� ImGui (�������� ���������, ��������� ���� � �.�.)
//	IMGUI_CHECKVERSION();
//	ImGui::CreateContext();
//	ImGui::StyleColorsDark();
//
//	WindowManager manager;         // ������ �������� ����
//	RunExample(manager);           // ��������� �������� ������
//
//	// �������� ���� �������
//	while (true) {
//		ImGui::NewFrame();
//
//		// ������������ ��� ���� ����� ��������
//		manager.RenderWindows();
//
//		ImGui::Render();
//		// ����� ������ ���� ��� ������� ������ ������� OpenGL/DirectX/Vulkan/etc.
//	}
//
//	ImGui::DestroyContext();
//	return 0;
//}
//
