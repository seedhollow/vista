#include "pch.h"
#include "menu/menu.h"
#include "cheat/features/feature_manager.h"
#include "cheat/features/bypass/c_bypass_feature.h"
#include "cheat/features/visuals/c_visual_feature.h"
#include "cheat/features/basetickmanager/c_game_tick_feature.h"
#include "cheat/features/autokill/c_auto_kill_feature.h"
#include "cheat/features/uncensored/c_uncensored_feature.h"
#include "cheat/features/chessboard/c_chessboard_feature.h"
#include "cheat/features/movespeed/c_movespeed_feature.h"
#include "cheat/features/attack/c_player_attack_feature.h"
#include "cheat/features/godmode/c_godmode_feature.h"
#include "cheat/features/stamina/c_infinite_stamina_feature.h"
#include "cheat/features/nocooldown/c_nocooldown_feature.h"
#include "cheat/features/ultimate/c_infinite_ultimate_feature.h"
#include "cheat/features/world/c_world_ambience_feature.h"
#include "cheat/features/noclip/c_no_clip_feature.h"
#include "cheat/features/visuals/c_chams_feature.h"
#include "cheat/features/dumbenemies/c_dumb_enemies_feature.h"
#include "cheat/features/entityfinder/c_entity_finder_feature.h"
#include "cheat/features/spoofer/c_spoofer_feature.h"
#include "cheat/features/jump/c_player_jump_feature.h"
#include "cheat/features/fov/c_camera_fov_feature.h"
#include "cheat/features/gravity/c_player_gravity_feature.h"

#include <shlobj.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static ID3D11RenderTargetView* g_pd3dRenderTarget = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static WNDPROC oWndProc = nullptr;

static HWND g_hWindow = nullptr;
static bool g_bInitialized = false;

static void CleanupDeviceD3D11();
static void CleanupRenderTarget();
static void RenderImGui_DX11(IDXGISwapChain* pSwapChain);
static void CreateRenderTarget(IDXGISwapChain* pSwapChain);
static bool CreateDeviceD3D11(HWND hWnd);

static std::add_pointer_t<HRESULT WINAPI(IDXGISwapChain*, UINT, UINT)> oPresent;
static std::add_pointer_t<HRESULT WINAPI(IDXGISwapChain*, UINT, UINT, const DXGI_PRESENT_PARAMETERS*)> oPresent1;
static std::add_pointer_t<HRESULT WINAPI(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT)> oResizeBuffers;
static std::add_pointer_t<HRESULT WINAPI(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT, const UINT*, IUnknown* const*)> oResizeBuffers1;
static std::add_pointer_t<HRESULT WINAPI(IDXGIFactory*, IUnknown*, DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**)> oCreateSwapChain;
static std::add_pointer_t<HRESULT WINAPI(IDXGIFactory*, IUnknown*, HWND, const DXGI_SWAP_CHAIN_DESC1*, const DXGI_SWAP_CHAIN_FULLSCREEN_DESC*, IDXGIOutput*, IDXGISwapChain1**)> oCreateSwapChainForHwnd;
static std::add_pointer_t<HRESULT WINAPI(IDXGIFactory*, IUnknown*, IUnknown*, const DXGI_SWAP_CHAIN_DESC1*, IDXGIOutput*, IDXGISwapChain1**)> oCreateSwapChainForCoreWindow;
static std::add_pointer_t<HRESULT WINAPI(IDXGIFactory*, IUnknown*, const DXGI_SWAP_CHAIN_DESC1*, IDXGIOutput*, IDXGISwapChain1**)> oCreateSwapChainForComposition;

static HRESULT WINAPI hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
	RenderImGui_DX11(pSwapChain);
	return oPresent(pSwapChain, SyncInterval, Flags);
}

static HRESULT WINAPI hkPresent1(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT PresentFlags, const DXGI_PRESENT_PARAMETERS* pPresentParameters) {
	RenderImGui_DX11(pSwapChain);
	return oPresent1(pSwapChain, SyncInterval, PresentFlags, pPresentParameters);
}

static HRESULT WINAPI hkResizeBuffers(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags) {
	CleanupRenderTarget();
	return oResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

static HRESULT WINAPI hkResizeBuffers1(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags, const UINT* pCreationNodeMask, IUnknown* const* ppPresentQueue) {
	CleanupRenderTarget();
	return oResizeBuffers1(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags, pCreationNodeMask, ppPresentQueue);
}

static HRESULT WINAPI hkCreateSwapChain(IDXGIFactory* pFactory, IUnknown* pDevice, DXGI_SWAP_CHAIN_DESC* pDesc, IDXGISwapChain** ppSwapChain) {
	CleanupRenderTarget();
	return oCreateSwapChain(pFactory, pDevice, pDesc, ppSwapChain);
}

static HRESULT WINAPI hkCreateSwapChainForHwnd(IDXGIFactory* pFactory, IUnknown* pDevice, HWND hWnd, const DXGI_SWAP_CHAIN_DESC1* pDesc, const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* pFullscreenDesc, IDXGIOutput* pRestrictToOutput, IDXGISwapChain1** ppSwapChain) {
	CleanupRenderTarget();
	return oCreateSwapChainForHwnd(pFactory, pDevice, hWnd, pDesc, pFullscreenDesc, pRestrictToOutput, ppSwapChain);
}

static HRESULT WINAPI hkCreateSwapChainForCoreWindow(IDXGIFactory* pFactory, IUnknown* pDevice, IUnknown* pWindow, const DXGI_SWAP_CHAIN_DESC1* pDesc, IDXGIOutput* pRestrictToOutput, IDXGISwapChain1** ppSwapChain) {
	CleanupRenderTarget();
	return oCreateSwapChainForCoreWindow(pFactory, pDevice, pWindow, pDesc, pRestrictToOutput, ppSwapChain);
}

static HRESULT WINAPI hkCreateSwapChainForComposition(IDXGIFactory* pFactory, IUnknown* pDevice, const DXGI_SWAP_CHAIN_DESC1* pDesc, IDXGIOutput* pRestrictToOutput, IDXGISwapChain1** ppSwapChain) {
	CleanupRenderTarget();
	return oCreateSwapChainForComposition(pFactory, pDevice, pDesc, pRestrictToOutput, ppSwapChain);
}

static bool CreateDeviceD3D11(HWND hWnd) {
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;

	const D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0,
	};

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_NULL,
		nullptr,
		0,
		featureLevels,
		2,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&g_pSwapChain,
		&g_pd3dDevice,
		nullptr,
		nullptr
	);

	if (hr != S_OK) {
		return false;
	}

	return true;
}

static void CreateRenderTarget(IDXGISwapChain* pSwapChain) {
	ID3D11Texture2D* pBackBuffer = nullptr;
	pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	if (pBackBuffer) {
		g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pd3dRenderTarget);
		pBackBuffer->Release();
	}
}

static void CleanupRenderTarget() {
	if (g_pd3dRenderTarget) {
		g_pd3dRenderTarget->Release();
		g_pd3dRenderTarget = nullptr;
	}
}

static void CleanupDeviceD3D11() {
	CleanupRenderTarget();

	if (g_pSwapChain) {
		g_pSwapChain->Release();
		g_pSwapChain = nullptr;
	}
	if (g_pd3dDevice) {
		g_pd3dDevice->Release();
		g_pd3dDevice = nullptr;
	}
	if (g_pd3dDeviceContext) {
		g_pd3dDeviceContext->Release();
		g_pd3dDeviceContext = nullptr;
	}
}

static LRESULT WINAPI hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (ImGui::GetCurrentContext()) {
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
	}

	return CallWindowProcW(oWndProc, hWnd, uMsg, wParam, lParam);
}

static void InitializeImGui(HWND hwnd) {
	if (g_bInitialized) return;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	// Load fonts
	ImFontConfig cfg;
	cfg.FontDataOwnedByAtlas = false;
	cfg.FontBuilderFlags;

	static const ImWchar ranges[] =
	{
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0x0370, 0x03FF, // Greek and Coptic
		0x0400, 0x04FF, // Cyrillic
		0x3131, 0x3163, // Korean alphabets
		0xAC00, 0xD7A3, // Korean characters
		0x4E00, 0x9FAF, // CJK Ideograms (Chinese)
		0x2000, 0x206F, // General Punctuation
		0x3000, 0x30FF, // CJK Symbols and Punctuation, Hiragana, Katakana
		0x31F0, 0x31FF, // Katakana Phonetic Extensions
		0xFF00, 0xFFEF, // Half-width characters
		0xFFFD, 0xFFFD, // Invalid
		0,
	};

	{
		var->font.inter_medium[0] = io.Fonts->AddFontFromMemoryTTF(inter_medium, sizeof(inter_medium), 14.f, &cfg, ranges);
		var->font.inter_bold[0] = io.Fonts->AddFontFromMemoryTTF(inter_bold, sizeof(inter_bold), 20.f, &cfg, ranges);
		var->font.inter_semibold[0] = io.Fonts->AddFontFromMemoryTTF(inter_semibold, sizeof(inter_semibold), 14.f, &cfg, ranges);

		var->font.icon[0] = io.Fonts->AddFontFromMemoryTTF(icon, sizeof(icon), 20.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
		var->font.icon[1] = io.Fonts->AddFontFromMemoryTTF(icon, sizeof(icon), 12.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
		var->font.icon[2] = io.Fonts->AddFontFromMemoryTTF(icon, sizeof(icon), 15.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
		var->font.icon[3] = io.Fonts->AddFontFromMemoryTTF(icon_menu_endfield, sizeof(icon_menu_endfield), 20.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
		var->font.icon[4] = io.Fonts->AddFontFromMemoryTTF(icon_menu_endfield, sizeof(icon_menu_endfield), 12.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
		var->font.icon[5] = io.Fonts->AddFontFromMemoryTTF(icon_menu_endfield, sizeof(icon_menu_endfield), 15.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());


	}

	ImGui_ImplWin32_Init(hwnd);

	if (!oWndProc) {
		oWndProc = (WNDPROC)SetWindowLongPtrW(hwnd, GWLP_WNDPROC, (LONG_PTR)hkWndProc);
	}
	g_bInitialized = true;
}

static void RenderImGui_DX11(IDXGISwapChain* pSwapChain) {
	if (ImGui::GetCurrentContext() == nullptr) {
		if (SUCCEEDED(pSwapChain->GetDevice(IID_PPV_ARGS(&g_pd3dDevice)))) {
			g_pd3dDevice->GetImmediateContext(&g_pd3dDeviceContext);

			DXGI_SWAP_CHAIN_DESC desc;
			pSwapChain->GetDesc(&desc);
			g_hWindow = desc.OutputWindow;
			InitializeImGui(g_hWindow);
			ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
		}
		else {
			return;
		}
	}

	if (!g_pd3dRenderTarget) {
		CreateRenderTarget(pSwapChain);
	}

	if (ImGui::GetCurrentContext() && g_pd3dRenderTarget) {


		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// show_menu
		if (GetAsyncKeyState(VK_INSERT) & 1) {
			var->gui.show_menu = !var->gui.show_menu;
		}

		if (var->gui.show_menu) {
			Menu::Render();
		}
		
		Features::FeatureManager::Render();

		ImGui::Render();
		g_pd3dDeviceContext->OMSetRenderTargets(1, &g_pd3dRenderTarget, nullptr);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}

namespace DX11 {
	void Hook(HWND hwnd) {

		if (!CreateDeviceD3D11(hwnd)) {
			return;
		}

		if (g_pd3dDevice) {
			IDXGIDevice* pDXGIDevice = nullptr;
			g_pd3dDevice->QueryInterface(IID_PPV_ARGS(&pDXGIDevice));

			IDXGIAdapter* pDXGIAdapter = nullptr;
			pDXGIDevice->GetAdapter(&pDXGIAdapter);

			IDXGIFactory* pIDXGIFactory = nullptr;
			pDXGIAdapter->GetParent(IID_PPV_ARGS(&pIDXGIFactory));

			if (!pIDXGIFactory) {
				return;
			}

			void** pVTable = *reinterpret_cast<void***>(g_pSwapChain);
			void** pFactoryVTable = *reinterpret_cast<void***>(pIDXGIFactory);

			void* fnCreateSwapChain = pFactoryVTable[10];
			void* fnCreateSwapChainForHwndChain = pFactoryVTable[15];
			void* fnCreateSwapChainForCWindowChain = pFactoryVTable[16];
			void* fnCreateSwapChainForCompChain = pFactoryVTable[24];

			void* fnPresent = pVTable[8];
			void* fnPresent1 = pVTable[22];

			void* fnResizeBuffers = pVTable[13];
			void* fnResizeBuffers1 = pVTable[39];

			pIDXGIFactory->Release();
			pDXGIAdapter->Release();
			pDXGIDevice->Release();
			CleanupDeviceD3D11();

			MH_CreateHook(fnCreateSwapChain, &hkCreateSwapChain, reinterpret_cast<void**>(&oCreateSwapChain));
			MH_CreateHook(fnCreateSwapChainForHwndChain, &hkCreateSwapChainForHwnd, reinterpret_cast<void**>(&oCreateSwapChainForHwnd));
			MH_CreateHook(fnCreateSwapChainForCWindowChain, &hkCreateSwapChainForCoreWindow, reinterpret_cast<void**>(&oCreateSwapChainForCoreWindow));
			MH_CreateHook(fnCreateSwapChainForCompChain, &hkCreateSwapChainForComposition, reinterpret_cast<void**>(&oCreateSwapChainForComposition));

			MH_CreateHook(fnPresent, &hkPresent, reinterpret_cast<void**>(&oPresent));
			MH_CreateHook(fnPresent1, &hkPresent1, reinterpret_cast<void**>(&oPresent1));

			MH_CreateHook(fnResizeBuffers, &hkResizeBuffers, reinterpret_cast<void**>(&oResizeBuffers));
			MH_CreateHook(fnResizeBuffers1, &hkResizeBuffers1, reinterpret_cast<void**>(&oResizeBuffers1));

			// Enable hooks
			MH_EnableHook(fnCreateSwapChain);
			MH_EnableHook(fnCreateSwapChainForHwndChain);
			MH_EnableHook(fnCreateSwapChainForCWindowChain);
			MH_EnableHook(fnCreateSwapChainForCompChain);

			MH_EnableHook(fnPresent);
			MH_EnableHook(fnPresent1);

			MH_EnableHook(fnResizeBuffers);
			MH_EnableHook(fnResizeBuffers1);

		}
	}

	void Unhook() {
		if (ImGui::GetCurrentContext()) {
			if (ImGui::GetIO().BackendRendererUserData)
				ImGui_ImplDX11_Shutdown();

			if (ImGui::GetIO().BackendPlatformUserData)
				ImGui_ImplWin32_Shutdown();

			ImGui::DestroyContext();
		}

		CleanupDeviceD3D11();

		MH_DisableHook(MH_ALL_HOOKS);
		MH_Uninitialize();
	}
}

static DWORD WINAPI InitializeThread(LPVOID lpParam) {
	if (MH_Initialize() != MH_OK) {
		LOG("[-] Failed to initialize MinHook");
		return 1;
	}

	LOG("[*] Waiting for GameAssembly.dll...");

	HMODULE hGameAssembly = nullptr;
	while (!hGameAssembly) {
		hGameAssembly = GetModuleHandleA("GameAssembly.dll");
		if (!hGameAssembly) {
			Sleep(1000);
		}
	}

	LOG("[+] GameAssembly.dll loaded at 0x%p\n", hGameAssembly);

	Sleep(10000);

	UnityResolve::Init(hGameAssembly, UnityResolve::Mode::Il2Cpp);
	
	

	// uncomment this to dump the game
	// it will be stored in the documents

	/*char documentsPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_MYDOCUMENTS, NULL, 0, documentsPath))) {
		std::string dumpPath = std::string(documentsPath) + "\\";
		UnityResolve::DumpToFile(dumpPath);
		LOG("[+] Dumped to: %s\n", dumpPath.c_str());
	}*/

	
	SDK::InitializeSDK();


	Features::g_BypassFeature				= new Features::CBypassFeature();
	Features::g_VisualFeature				= new Features::CVisualFeature();
	Features::g_CGameTickFeature			= new Features::CGameTickFeature();
	Features::g_AutoKillFeature				= new Features::CAutoKillFeature();
	Features::g_CUncensoredFeature			= new Features::CUncensoredFeature();
	Features::g_CChessboardFeature			= new Features::CChessboardFeature();
	Features::g_CMovespeedFeature			= new Features::CMovespeedFeature();
	Features::g_CPlayerAttackFeature		= new Features::CPlayerAttackFeature();
	Features::g_CGodModeFeature				= new Features::CGodModeFeature();
	Features::g_CInfiniteStaminaFeature		= new Features::CInfiniteStaminaFeature();
	Features::g_CNocooldownFeature			= new Features::CNocooldownFeature();
	Features::g_CInfiniteUltimateFeature	= new Features::CInfiniteUltimateFeature();
	Features::g_CWorldAmbienceFeature		= new Features::CWorldAmbienceFeature();
	Features::g_CNoClipFeature				= new Features::CNoClipFeature();
	Features::g_ChamsFeature				= new Features::CChamsFeature();
	Features::g_CDumbEnemiesFeature			= new Features::CDumbEnemiesFeature();
	Features::g_CEntityFinderFeature		= new Features::CEntityFinderFeature();
	Features::g_CSpooferFeature				= new Features::CSpooferFeature();
	Features::g_CPlayerJumpFeature			= new Features::CPlayerJumpFeature();
	Features::g_CCameraFovFeature			= new Features::CCameraFovFeature();
	Features::g_CPlayerGravityFeature		= new Features::CPlayerGravityFeature();

	Features::FeatureManager::RegisterFeature(Features::g_BypassFeature);
	Features::FeatureManager::RegisterFeature(Features::g_VisualFeature);
	Features::FeatureManager::RegisterFeature(Features::g_CGameTickFeature);
	Features::FeatureManager::RegisterFeature(Features::g_AutoKillFeature);
	Features::FeatureManager::RegisterFeature(Features::g_CUncensoredFeature);
	Features::FeatureManager::RegisterFeature(Features::g_CChessboardFeature);
	Features::FeatureManager::RegisterFeature(Features::g_CMovespeedFeature);
	Features::FeatureManager::RegisterFeature(Features::g_CPlayerAttackFeature);
	Features::FeatureManager::RegisterFeature(Features::g_CGodModeFeature);
	Features::FeatureManager::RegisterFeature(Features::g_CInfiniteStaminaFeature);
	Features::FeatureManager::RegisterFeature(Features::g_CNocooldownFeature);
	Features::FeatureManager::RegisterFeature(Features::g_CInfiniteUltimateFeature);
	Features::FeatureManager::RegisterFeature(Features::g_CWorldAmbienceFeature);
	Features::FeatureManager::RegisterFeature(Features::g_CNoClipFeature);
	Features::FeatureManager::RegisterFeature(Features::g_ChamsFeature);
	Features::FeatureManager::RegisterFeature(Features::g_CDumbEnemiesFeature);
	Features::FeatureManager::RegisterFeature(Features::g_CEntityFinderFeature);
	Features::FeatureManager::RegisterFeature(Features::g_CSpooferFeature);
	Features::FeatureManager::RegisterFeature(Features::g_CPlayerJumpFeature);
	Features::FeatureManager::RegisterFeature(Features::g_CCameraFovFeature);
	Features::FeatureManager::RegisterFeature(Features::g_CPlayerGravityFeature);

	Features::FeatureManager::Initialize();

	auto game_version = UnityResolve::UnityType::Application::GetVersion();
	if (game_version) {
		game_info->game_version = game_version->ToString();
		LOG("[+] Game version: %s\n", game_info->game_version.c_str());
	}


	// Initialize features

	LOG("[*] Waiting for target window...\n");

	// Wait for a valid window
	HWND hwnd = nullptr;
	while (!hwnd) {
		hwnd = GetForegroundWindow();
		if (hwnd) {
			char windowTitle[256];
			GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle));
			LOG("[+] Found window: %s (0x%p)\n", windowTitle, hwnd);
		}
		Sleep(100);
	}

	DX11::Hook(hwnd);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);

		// Create console with random title
		Console::Alloc();
		CreateThread(nullptr, 0, InitializeThread, nullptr, 0, nullptr);
		break;

	case DLL_PROCESS_DETACH:
		LOG("[*] DLL detaching...");
		Sleep(1000); 

		Console::Free();
		break;
	}
	return TRUE;
}