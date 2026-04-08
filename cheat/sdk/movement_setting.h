#pragma once
namespace SDK {
	using UR = UnityResolve;

	class MovementSetting {
	public:
		static UR::Class* GetClass();
		static void Initialize();
		// Function pointers
		inline static UR::Method* get_maxJumpHeight = nullptr;
		inline static UR::Method* Modify_maxJumpHeight = nullptr;

		inline static UR::Method* get_gravity = nullptr;
		inline static UR::Method* Modify_gravity = nullptr;

	private:
		inline static bool initialized = false;
		inline static UR::Class* pClass = nullptr;
	};
} // namespace SDK
