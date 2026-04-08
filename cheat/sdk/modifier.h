#pragma once
namespace SDK {
	using UR = UnityResolve;

	class Modifier {
	public:
		static UR::Class* GetClass();
		static void Initialize();
		// Function pointers
		inline static UR::Method* NewDamage = nullptr;
		inline static UR::Method* Apply = nullptr;
	private:
		inline static bool initialized = false;
		inline static UR::Class* pClass = nullptr;
	};
} // namespace SDK
