#pragma once
namespace SDK {
	using UR = UnityResolve;
	class Skill {
	public:
		static UR::Class* GetClass();
		static void Initialize();
		// Function pointers
		inline static UR::Method* get_cooldown	= nullptr;
		inline static UR::Method* get_owner		= nullptr;
		inline static UR::Method* _ApplyCost	= nullptr;
		inline static UR::Method* CheckCd		= nullptr;
		inline static UR::Method* CheckTag		= nullptr;

	private:
		inline static bool initialized = false;
		inline static UR::Class* pClass = nullptr;
	};
} // namespace SDK