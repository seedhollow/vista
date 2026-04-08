#pragma once
namespace SDK {
    using UR = UnityResolve;

    class Animator {
    public:
        static UR::Class* GetClass();
        static void Initialize();

        // Function pointers
		inline static UR::Method* GetBoneTransform = nullptr;
		inline static UR::Method* GetBoneTransformInternal = nullptr;
    private:
        inline static UR::Class* pClass = nullptr;
        inline static bool initialized = false;
    };
} // namespace SDK
