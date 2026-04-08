#pragma once
namespace SDK {
    using UR = UnityResolve;

    class SkeletalMorphComponent {
    public:
        static void Initialize();

        static UR::Class* GetClass();

		inline static UR::Field* f_m_animator = nullptr;
		inline static UR::Field* f_m_core = nullptr;
    private:
        inline static bool initialized = false;
        inline static UR::Class* pClass = nullptr;
    };
} // namespace SDK
