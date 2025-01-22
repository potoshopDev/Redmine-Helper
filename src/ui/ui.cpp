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

#include "ui/uiwin.h"
#include "ui/win.h"

namespace
{
using Microsoft::WRL::ComPtr;
using Devices = std::tuple<ID3D11Device*, ID3D11DeviceContext*, IDXGISwapChain*>;

std::optional<Devices> CreateDeviceD3D(HWND hWnd);
std::optional<ID3D11RenderTargetView*> CreateRenderTarget(ID3D11Device* dev, IDXGISwapChain* sc);
void SDLClear(SDL_Window*);
[[nodiscard]] auto InitializeSDLWindow() -> std::unique_ptr<SDL_Window, decltype(&SDLClear)>;
[[nodiscard]] HWND GetHWND(SDL_Window* window);
void SHutdown();

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
        virtual inline ID3D11DeviceContext* GetDeviceContext() const noexcept override { return g_pd3dDeviceContext.Get(); }
        virtual inline IDXGISwapChain* GetSwapChain() const noexcept override { return g_pSwapChain.Get(); }
        virtual inline ID3D11RenderTargetView* GetRenderTarget() const noexcept override { return g_mainRenderTargetView.Get(); }
        virtual inline bool is_Ready() const noexcept
        {
            return (g_pd3dDevice && g_pd3dDeviceContext && g_pSwapChain && g_mainRenderTargetView);
        }
        virtual inline bool ResizeBuffer() noexcept override;

        // Конструктор копирования
        oGraphicsDevice(const oGraphicsDevice& other)
        {
            g_pd3dDevice = other.g_pd3dDevice;
            g_pd3dDeviceContext = other.g_pd3dDeviceContext;
            g_pSwapChain = other.g_pSwapChain;

            // Создать новый render target для копии, если нужно
            if (other.g_mainRenderTargetView)
            {
                auto ren{CreateRenderTarget(g_pd3dDevice.Get(), g_pSwapChain.Get())};
                g_mainRenderTargetView.Attach(*ren);
            }
        }

        // Конструктор перемещения
        oGraphicsDevice(oGraphicsDevice&& other) noexcept
        {
            g_pd3dDevice = std::move(other.g_pd3dDevice);
            g_pd3dDeviceContext = std::move(other.g_pd3dDeviceContext);
            g_pSwapChain = std::move(other.g_pSwapChain);
            g_mainRenderTargetView = std::move(other.g_mainRenderTargetView);
            // Перемещаемые ресурсы теперь в `other` находятся в корректном состоянии.
            other.g_pd3dDevice = nullptr;
            other.g_pd3dDeviceContext = nullptr;
            other.g_pSwapChain = nullptr;
            other.g_mainRenderTargetView = nullptr;
        }

    public:
        oGraphicsDevice(HWND hwnd);

    private:
        ComPtr<ID3D11Device> g_pd3dDevice{};
        ComPtr<ID3D11DeviceContext> g_pd3dDeviceContext{};
        ComPtr<IDXGISwapChain> g_pSwapChain{};
        ComPtr<ID3D11RenderTargetView> g_mainRenderTargetView{};
    };

    std::unique_ptr<iGraphicsDevice> _self;

public:
    GraphicsDevice(HWND hwnd) : _self(std::make_unique<oGraphicsDevice>(hwnd)) {}

    // Конструктор копирования
    GraphicsDevice(const GraphicsDevice& other)
        : _self(std::make_unique<oGraphicsDevice>(*static_cast<oGraphicsDevice*>(other._self.get())))
    {
    }

    // Конструктор перемещения
    GraphicsDevice(GraphicsDevice&& other) noexcept : _self(std::move(other._self)) {}

    [[nodiscard]] inline ID3D11Device* GetDevice() const noexcept { return _self->GetDevice(); }
    [[nodiscard]] inline ID3D11DeviceContext* GetDeviceContext() const noexcept { return _self->GetDeviceContext(); }
    [[nodiscard]] inline IDXGISwapChain* GetSwapChain() const noexcept { return _self->GetSwapChain(); }
    [[nodiscard]] inline ID3D11RenderTargetView* GetRenderTarget() const noexcept { return _self->GetRenderTarget(); }
    inline bool ResizeBuffer() noexcept { return _self->ResizeBuffer(); }
    inline bool is_Ready() const noexcept { return _self->is_Ready(); }
};

inline bool GraphicsDevice::oGraphicsDevice::ResizeBuffer() noexcept
{
    g_mainRenderTargetView.Reset();
    g_pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
    if (auto ren{CreateRenderTarget(g_pd3dDevice.Get(), g_pSwapChain.Get())})
    {
        g_mainRenderTargetView.Attach(*ren);
        return true;
    }

    return false;
}

GraphicsDevice::oGraphicsDevice::oGraphicsDevice(HWND hwnd)
{
    if (auto tmpDev{CreateDeviceD3D(hwnd)})
    {
        auto& [Device, Context, SwapChain] = *tmpDev;
        g_pd3dDevice.Attach(Device);
        g_pd3dDeviceContext.Attach(Context);
        g_pSwapChain.Attach(SwapChain);

        if (auto tmpRen{CreateRenderTarget(g_pd3dDevice.Get(), g_pSwapChain.Get())})
        {
            g_mainRenderTargetView.Attach(*tmpRen);
        }
    }
}

void SetupImGui(SDL_Window& window, GraphicsDevice& device);
void EventProcessing(GraphicsDevice& device, SDL_Window& window, bool& done);
void StartFrame();
void Rendering(GraphicsDevice& device, ImVec4& clearColor);
}  // namespace

namespace ImGui
{
int RunUI()
{
    auto window{InitializeSDLWindow()};
    const auto hwnd{GetHWND(window.get())};
    auto DirectXDevice{GraphicsDevice(hwnd)};

    if (!DirectXDevice.is_Ready()) return std::to_underlying(RETURN_CODE::NO_INITILIZE_DIREXTX);

    SetupImGui(*window, DirectXDevice);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clearColor = ImVec4(0.35f, 0.35f, 0.35f, 1.f);

    // Main loop
    bool done = false;
    
    helper::WindowData windowData{};
    helper::setBool(windowData, helper::titleSimpleWindow, true);
    helper::setBool(windowData, helper::titleDemoWindow, false);


    helper::WindowsApp winApp{.windowsData = windowData};
    winApp.emplace_back(helper::demoWindow);
    winApp.emplace_back(helper::simpleWindow);

    while (!done)
    {
        EventProcessing(DirectXDevice, *window, done);
        StartFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about
        // Dear ImGui!). if (show_demo_window) 	ImGui::ShowDemoWindow(&show_demo_window);

        helper::draw(winApp);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);  // Pass a pointer to our bool variable (the window will have a closing
                                                                   // button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me")) show_another_window = false;
            ImGui::End();
        }

        {
            ImGui::Begin("New windows");
            ImGui::Text("СЌС‚РѕС‚ С‚РµРєСЃС‚ РЅР° СЂСѓСЃСЃРєРѕРј");
            ImGui::Text("Привет");
            ImGui::End();
        }

        Rendering(DirectXDevice, clearColor);
    }

    SHutdown();
    return std::to_underlying(RETURN_CODE::SUCCESED);
}
}  // namespace ImGui

namespace
{
void Rendering(GraphicsDevice& device, ImVec4& clearColor)
{
    ImGui::Render();
    const float clear_color_with_alpha[4] = {
        clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w};
    auto* DContext{device.GetDeviceContext()};
    auto* DRen{device.GetRenderTarget()};

    DContext->OMSetRenderTargets(1, &DRen, nullptr);
    DContext->ClearRenderTargetView(DRen, clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    device.GetSwapChain()->Present(1, 0);  // Present with vsync
}

void EventProcessing(GraphicsDevice& device, SDL_Window& window, bool& done)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT) done = true;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
            event.window.windowID == SDL_GetWindowID(&window))
            done = true;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED &&
            event.window.windowID == SDL_GetWindowID(&window))
        {
            // Release all outstanding references to the swap chain's buffers before resizing.
            device.ResizeBuffer();
        }
    }
}
void StartFrame()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

std::optional<Devices> CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
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
    const D3D_FEATURE_LEVEL featureLevelArray[2] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0,
    };

    ID3D11Device* Device{};
    ID3D11DeviceContext* Context{};
    IDXGISwapChain* SwapChain{};

    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd,
            &SwapChain, &Device, &featureLevel, &Context) != S_OK)
        return std::nullopt;

    return std::tuple(Device, Context, SwapChain);
}

std::optional<ID3D11RenderTargetView*> CreateRenderTarget(ID3D11Device* dev, IDXGISwapChain* sc)
{
    ComPtr<ID3D11Texture2D> pBackBuffer{};
    sc->GetBuffer(0, IID_PPV_ARGS(pBackBuffer.GetAddressOf()));

    ID3D11RenderTargetView* RenderTV{};
    if (SUCCEEDED(dev->CreateRenderTargetView(pBackBuffer.Get(), NULL, &RenderTV))) return RenderTV;

    return std::nullopt;
}

auto InitializeSDLWindow() -> std::unique_ptr<SDL_Window, decltype(&SDLClear)>
{
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    return std::unique_ptr<SDL_Window, decltype(&SDLClear)>(
        SDL_CreateWindow("Redmine Helper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags), &SDLClear);
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

void SHutdown()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void SetupImGui(SDL_Window& window, GraphicsDevice& device)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    // Setup
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForD3D(&window);
    ImGui_ImplDX11_Init(device.GetDevice(), device.GetDeviceContext());

    const auto font{io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\Arial.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesCyrillic())};
    IM_ASSERT(font != nullptr);
}
}  // namespace
