#pragma once
namespace SDK {
	using UR = UnityResolve;

	class GameUtil {
	public:
		static UR::Class* GetClass();
		static void Initialize();
		// Function pointers
		inline static UR::Method* get_mainCharacter = nullptr;
		inline static UR::Method* get_mainCamera = nullptr;
		inline static UR::Method* get_playerTrans = nullptr;
		inline static UR::Method* get_playerPos = nullptr;
	private:
		inline static bool initialized = false;
		inline static UR::Class* pClass = nullptr;
	};
} // namespace SDK
