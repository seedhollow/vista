#pragma once
namespace SDK {
	using UR = UnityResolve;
	class UIText {
	public:
		static UR::Class* GetClass();
		static void Initialize();
		// Function pointers
		inline static UR::Method* get_text = nullptr;
		inline static UR::Method* set_text = nullptr;

	private:
		inline static bool initialized = false;
		inline static UR::Class* pClass = nullptr;
	};
} // namespace SDK