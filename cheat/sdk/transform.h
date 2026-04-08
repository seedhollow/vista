#pragma once
namespace SDK {
    using UR = UnityResolve;

    class Transform {
    public:
        static UR::Class* GetClass();
        static void Initialize();

        // Function pointers
		inline static UR::Method* get_position = nullptr;
		inline static UR::Method* set_position = nullptr;
		inline static UR::Method* get_rotation = nullptr;
		inline static UR::Method* set_rotation = nullptr;

    private:
        inline static UR::Class* pClass = nullptr;
        inline static bool initialized = false;
    };
} // namespace SDK
