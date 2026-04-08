#pragma once
#include "thirdparty/minhook/MinHook.h"
#include "console.hpp"
#include <map>

#define WIN32_LEAN_AND_MEAN

#define CALL_ORIGIN(function, ...) HookManager::call(function, __func__, __VA_ARGS__)

class HookManager {
public:
	template <typename Fn>
	static void install(Fn func, Fn handler) {
		enable(func, handler);
		holderMap[reinterpret_cast<void*>(handler)] = reinterpret_cast<void*>(func);
	}

	template <typename Fn>
	static void install(void* target, Fn handler) {
		if (!target) {
			LOG("[-] Target function pointer is null. Cannot install hook for handler: %s\n", __func__);
			return;
		}
		// Explicitly cast the void* target to the handler's type
		Fn func = reinterpret_cast<Fn>(target);
		install(func, handler);
	}

	template <typename Fn>
	static Fn getOrigin(Fn handler, const char* callerName = nullptr) noexcept {
		if (holderMap.count(reinterpret_cast<void*>(handler)) == 0) {
			LOG("[!] Origin not found for handler: %s. Maybe racing bug.\n", callerName ? callerName : "unknown");
			return nullptr;
		}
		return reinterpret_cast<Fn>(holderMap[reinterpret_cast<void*>(handler)]);
	}

	template <typename Fn>
	static void detach(Fn handler) noexcept {
		disable(handler);
		holderMap.erase(reinterpret_cast<void*>(handler));
	}

	template <typename RType, typename... Params>
	static RType call(RType(*handler)(Params...), const char* callerName = nullptr, Params... params) {
		auto origin = getOrigin(handler, callerName);
		if (origin != nullptr)
			return origin(params...);
		return RType();
	}

	static void detachAll() noexcept {
		for (const auto& [key, value] : holderMap) {
			disable(key);
		}
		holderMap.clear();
	}
private:
	inline static std::map<void*, void*> holderMap{ };
	template <typename Fn>
	static void disable(Fn handler) {
		Fn origin = getOrigin(handler);
		if (origin != nullptr) {
			MH_DisableHook(reinterpret_cast<LPVOID>(origin));
			MH_RemoveHook(reinterpret_cast<LPVOID>(origin));
		}
	}

	template <typename Fn>
	static void enable(Fn& func, Fn handler) {
		void* pTarget = reinterpret_cast<void*>(func);
		void* pDetour = reinterpret_cast<void*>(handler);
		void* pOriginal = nullptr;

		MH_CreateHook(pTarget, pDetour, &pOriginal);
		MH_EnableHook(pTarget);

		func = reinterpret_cast<Fn>(pOriginal);
	}
};
