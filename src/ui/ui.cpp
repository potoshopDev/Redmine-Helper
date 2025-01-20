#include "ui.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <stdio.h>
#include <utility>
#include <wrl/client.h>
#include <memory>
#include <optional>
#include <tuple>

namespace
{
	using Microsoft::WRL::ComPtr;
	using Devices = std::tuple<ID3D11Device*, ID3D11DeviceContext*, IDXGISwapChain*>;

	std::optional<Devices> CreateDeviceD3D(HWND hWnd);
	std::optional<ID3D11RenderTargetView*> CreateRenderTarget(ID3D11Device* dev, IDXGISwapChain* sc);
	void SDLClear(SDL_Window*);
	[[nodiscard]] auto InitializeSDLWindow() -> std::unique_ptr<SDL_Window, decltype(&SDLClear)>;
	[[nodiscard]] HWND GetHWND(SDL_Window* window);

	class GraphicsDevice final
	{
		class iGraphicsDevice
		{
		public:
			virtual inline ID3D11Device* GetDevice() const noexcept = 0;
			virtual inline ID3D11DeviceContext* GetDeviceContext() const noexcept = 0;
			virtual inline IDXGISwapChain* GetSwapChain() const noexcept = 0;
			virtual inline ID3D11RenderTargetView* GetRenderTarget() const noexcept = 0;
			virtual inline bool ResizeBuffer() noexcept = 0;
			virtual inline bool is_Ready() const noexcept = 0;

			virtual ~iGraphicsDevice() = default;
		};

		class oGraphicsDevice : public iGraphicsDevice
		{
		public:
			virtual inline ID3D11Device* GetDevice() const noexcept override { return g_pd3dDevice.Get(); }
			virtual inline ID3D11DeviceContext* GetDeviceContext()const noexcept override { return g_pd3dDeviceContext.Get(); }
			virtual inline IDXGISwapChain* GetSwapChain() const noexcept override { return g_pSwapChain.Get(); }
			virtual inline ID3D11RenderTargetView* GetRenderTarget() const noexcept override { return g_mainRenderTargetView.Get(); }
			virtual inline bool is_Ready() const noexcept { return (g_pd3dDevice && g_pd3dDeviceContext && g_pSwapChain && g_mainRenderTargetView); }
			virtual inline bool ResizeBuffer() noexcept override;

		public:

			oGraphicsDevice(HWND hwnd)
			{
				if (auto tmpDev{ CreateDeviceD3D(hwnd) })
				{
					auto& [Device, Context, SwapChain] = *tmpDev;
					g_pd3dDevice.Attach(Device);
					g_pd3dDeviceContext.Attach(Context);
					g_pSwapChain.Attach(SwapChain);

					if (auto tmpRen{ CreateRenderTarget(g_pd3dDevice.Get(), g_pSwapChain.Get()) })
					{
						g_mainRenderTargetView.Attach(*tmpRen);
					}
				}
			}

		private:
			ComPtr<ID3D11Device> g_pd3dDevice{};
			ComPtr<ID3D11DeviceContext> g_pd3dDeviceContext{};
			ComPtr<IDXGISwapChain> g_pSwapChain{};
			ComPtr<ID3D11RenderTargetView> g_mainRenderTargetView{};
		};

		const std::unique_ptr<iGraphicsDevice> _self;

	public:

		GraphicsDevice(HWND hwnd) : _self(std::make_unique<oGraphicsDevice>(hwnd)) {}

		[[nodiscard]] inline ID3D11Device* GetDevice() const noexcept { return _self->GetDevice(); }
		[[nodiscard]] inline ID3D11DeviceContext* GetDeviceContext()const noexcept { return _self->GetDeviceContext(); }
		[[nodiscard]] inline IDXGISwapChain* GetSwapChain() const noexcept { return _self->GetSwapChain(); }
		[[nodiscard]] inline ID3D11RenderTargetView* GetRenderTarget() const noexcept { return _self->GetRenderTarget(); }
		inline bool ResizeBuffer() noexcept { return _self->ResizeBuffer(); }
		inline bool is_Ready() const noexcept { return _self->is_Ready(); }
	};

	inline bool GraphicsDevice::oGraphicsDevice::ResizeBuffer() noexcept
	{
		g_mainRenderTargetView.Reset();
		g_pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		if (auto ren{ CreateRenderTarget(g_pd3dDevice.Get(), g_pSwapChain.Get()) })
		{
			g_mainRenderTargetView.Attach(*ren);
			return true;
		}

		return false;
	}
}

namespace ImGui
{
	int RunUI()
	{
		auto window{ InitializeSDLWindow() };
		const auto hwnd{ GetHWND(window.get()) };
		auto DirectXDevice{ GraphicsDevice(hwnd) };

		if (!DirectXDevice.is_Ready())
			return std::to_underlying(RETURN_CODE::NO_INITILIZE_DIREXTX);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		//Setup
		ImGui::StyleColorsDark();

		 //Setup Platform/Renderer backends
		ImGui_ImplSDL2_InitForD3D(window.get());
		ImGui_ImplDX11_Init(DirectXDevice.GetDevice(), DirectXDevice.GetDeviceContext());

		const auto font{ io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\Arial.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesCyrillic()) };
		IM_ASSERT(font != nullptr);

		//Our state
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.35f, 0.35f, 0.35f, 1.f);

		//Main loop
		bool done = false;
		while (!done)
		{
			//Poll and handle events (inputs, window resize, etc.)
			//You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
			//- When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
			//- When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
			//Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				ImGui_ImplSDL2_ProcessEvent(&event);
				if (event.type == SDL_QUIT)
					done = true;
				if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window.get()))
					done = true;
				if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED && event.window.windowID == SDL_GetWindowID(window.get()))
				{
					//Release all outstanding references to the swap chain's buffers before resizing.
					DirectXDevice.ResizeBuffer();
				}
			}

			//Start the Dear ImGui frame
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();

			//1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
			if (show_demo_window)
				ImGui::ShowDemoWindow(&show_demo_window);

			//2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
			{
				static float f = 0.0f;
				static int counter = 0;

				ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

				ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
				ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
				ImGui::Checkbox("Another Window", &show_another_window);

				ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
				ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

				if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
					counter++;
				ImGui::SameLine();
				ImGui::Text("counter = %d", counter);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
			}


			//3. Show another simple window.
			if (show_another_window)
			{
				ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
				ImGui::Text("Hello from another window!");
				if (ImGui::Button("Close Me"))
					show_another_window = false;
				ImGui::End();
			}
			{
				ImGui::Begin("New windows");
				ImGui::Text("этот текст на русском");
				ImGui::Text("������");
				ImGui::End();
			}

			//Rendering
			ImGui::Render();
			const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
			auto* DContext{ DirectXDevice.GetDeviceContext() };
			auto* DRen{ DirectXDevice.GetRenderTarget() };

			DContext->OMSetRenderTargets(1, &DRen, nullptr);
			DContext->ClearRenderTargetView(DRen, clear_color_with_alpha);
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			DirectXDevice.GetSwapChain()->Present(1, 0); // Present with vsync
			//g_pSwapChain->Present(0, 0); // Present without vsync
		}

		//Cleanup
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();

		return std::to_underlying(RETURN_CODE::SUCCESED);
	}
}

namespace
{
	std::optional<Devices> CreateDeviceD3D(HWND hWnd)
	{
		//Setup swap chain
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 2;
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		UINT createDeviceFlags = 0;
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
		D3D_FEATURE_LEVEL featureLevel;
		const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

		ID3D11Device* Device{};
		ID3D11DeviceContext* Context{};
		IDXGISwapChain* SwapChain{};

		if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &SwapChain, &Device, &featureLevel, &Context) != S_OK)
			return std::nullopt;

		return std::tuple(Device, Context, SwapChain);
	}

	std::optional<ID3D11RenderTargetView*> CreateRenderTarget(ID3D11Device* dev, IDXGISwapChain* sc)
	{
		ComPtr<ID3D11Texture2D> pBackBuffer{};
		sc->GetBuffer(0, IID_PPV_ARGS(pBackBuffer.GetAddressOf()));

		ID3D11RenderTargetView* RenderTV{};
		if (SUCCEEDED(dev->CreateRenderTargetView(pBackBuffer.Get(), NULL, &RenderTV)))
			return RenderTV;

		return std::nullopt;
	}

	auto InitializeSDLWindow() -> std::unique_ptr<SDL_Window, decltype(&SDLClear)>
	{
		SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
		return  std::unique_ptr<SDL_Window, decltype(&SDLClear)>
			(SDL_CreateWindow("Redmine Helper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags), &SDLClear);
	}

	HWND GetHWND(SDL_Window* window)
	{
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(window, &wmInfo);

		return static_cast<HWND>(wmInfo.info.win.window);
	}

	void SDLClear(SDL_Window* window)
	{
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
}
