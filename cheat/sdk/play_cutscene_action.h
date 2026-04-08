#pragma once
namespace SDK {
	using UR = UnityResolve;
	class PlayCutsceneAction {
	public:
		static UR::Class* GetClass();
		static void Initialize();
		// Function pointers
		inline static UR::Method* Execute = nullptr;

	private:
		inline static bool initialized = false;
		inline static UR::Class* pClass = nullptr;
	};
} // namespace SDK