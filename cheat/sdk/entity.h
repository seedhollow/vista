#pragma once

namespace SDK {
    using UR = UnityResolve;

    class Entity {
    public:
		static UR::Class* GetClass();
        static void Initialize( );

		inline static UR::Method* IsValid = nullptr;
        inline static UR::Method* get_name = nullptr;
        inline static UR::Method* set_name = nullptr;
        inline static UR::Method* get_isHide = nullptr;
        inline static UR::Method* set_isHide = nullptr;
        inline static UR::Method* get_objectType = nullptr;
        inline static UR::Method* set_objectType = nullptr;
        inline static UR::Method* get_position = nullptr;
		inline static UR::Method* get_rotation = nullptr;
        inline static UR::Method* get_alive = nullptr;
        inline static UR::Method* get_abilityCom = nullptr;
        inline static UR::Method* get_markDie = nullptr;
        inline static UR::Method* get_character = nullptr;
		inline static UR::Method* get_charCtrl = nullptr;
        inline static UR::Method* get_baseController = nullptr;
		inline static UR::Method* get_skMorphCom = nullptr;
		inline static UR::Method* get_rvoComponent = nullptr;
		inline static UR::Method* get_movementComponent = nullptr;
		inline static UR::Method* get_rootCom = nullptr;
		inline static UR::Method* get_animatorCom = nullptr;
		inline static UR::Method* get_characterAnimCom = nullptr;
		inline static UR::Method* get_modelCom = nullptr;


		inline static UR::Field* f_distance2MainCharSq = nullptr;
		inline static UR::Field* f_distance2CameraSq = nullptr;
    private:
		inline static UR::Class* pClass = nullptr;
        inline static bool initialized = false;
    };
} // namespace SDK
