#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS

#include <d3d11.h>
#include <dxgi1_2.h>
#include <D3DX11tex.h>
#include <Windows.h>
#include <memory>
#include <iostream>
#include <map>
#include <cmath>
#include <ctime>
#include <string>
#include <sstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_freetype.h"
#include "console.hpp"
#include "settings/functions.h"
#include "settings/variables.h"
#include "settings/colors.h"
#include "settings/elements.h"
#include "data/fonts.h"
#include "data/images.h"
#include "data/icons.h"
#include "thirdparty/minhook/MinHook.h"
#include "thirdparty/il2cpp/UnityResolve.hpp"
#include "thirdparty/json/json.hpp"
#include "cheat/sdk/sdk_exporter.h"
#include "cheat/tools/hook_manager.h"
#include "cheat/tools/config_manager.h"

#pragma comment(lib, "D3DX11.lib")