#pragma once

namespace SDK {
	using UR = UnityResolve;

	class ComplexAnimatorComponent {
	public:

		static UR::Class* GetClass();

		static void Initialize();
		// Function pointers
		inline static UR::Method* get_animator = nullptr;
	private:
		inline static bool initialized = false;
		inline static UR::Class* pClass = nullptr;
	};
} // namespace SDK
