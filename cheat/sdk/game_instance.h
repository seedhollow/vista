#pragma once
namespace SDK {
	using UR = UnityResolve;

	class GameInstance {
	public:
		static UR::Class* GetClass();
		static void Initialize();
		// Function pointers
		inline static UR::Method* get_instance = nullptr;
		
		inline static UR::Field* f_m_world = nullptr;
		inline static UR::Field* f_cameraManager = nullptr;
		inline static UR::Field* f_m_player = nullptr;
	private:
		inline static bool initialized = false;
		inline static UR::Class* pClass = nullptr;
	};
} // namespace SDK
