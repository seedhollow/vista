#pragma once
namespace SDK {
	using UR = UnityResolve;

	class MovementComponent {
	public:
		static UR::Class* GetClass();
		static void Initialize();
		// Function pointers
		inline static UR::Method* get_config = nullptr;
		inline static UR::Method* get_setting = nullptr;
		inline static UR::Method* SetSpeedMultiplierFromEnvironment = nullptr;
		inline static UR::Method* SetSpeed = nullptr;
		inline static UR::Method* Tick = nullptr;
		inline static UR::Method* PreTick = nullptr;
		inline static UR::Method* set_moveMode = nullptr;
		inline static UR::Method* TeleportTo = nullptr;
		inline static UR::Method* IsPositionWalkable = nullptr;
	private:
		inline static bool initialized = false;
		inline static UR::Class* pClass = nullptr;
	};
} // namespace SDK
