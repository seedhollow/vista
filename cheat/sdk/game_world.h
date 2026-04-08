#pragma once
namespace SDK {
	using UR = UnityResolve;

	class GameWorld {
	public:
		static UR::Class* GetClass();
		static void Initialize();

		inline static UR::Field* f_battle = nullptr;
	private:
		inline static bool initialized = false;
		inline static UR::Class* pClass = nullptr;
	};
} // namespace SDK
