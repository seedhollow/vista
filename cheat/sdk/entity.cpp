#include "pch.h"
#include "entity.h"

namespace SDK {
	UR::Class* Entity::GetClass() {
		if (!pClass) {
			pClass = UR::Get("Gameplay.Beyond.dll")->Get("Entity");
		}
		return pClass;
	}

	void Entity::Initialize( ) {
		if (initialized)
			return;

        if (!GetClass()) {
			LOG("[-] Entity class not found!\n");
            return;
        }

		f_distance2MainCharSq	= GetClass()->Get<UR::Field>("distance2MainCharSq");
		f_distance2CameraSq		= GetClass()->Get<UR::Field>("distance2CameraSq");

		IsValid				= GetClass()->Get<UR::Method>("IsValid");
		get_name			= GetClass()->Get<UR::Method>("get_name");
        set_name			= GetClass()->Get<UR::Method>("set_name", {"System.String"});
		get_isHide			= GetClass()->Get<UR::Method>("get_isHide");
        set_isHide			= GetClass()->Get<UR::Method>("set_isHide", {"System.Boolean"});
		get_objectType		= GetClass()->Get<UR::Method>("get_objectType");
        set_objectType		= GetClass()->Get<UR::Method>("set_objectType");
		get_position		= GetClass()->Get<UR::Method>("get_position");
		get_rotation		= GetClass()->Get<UR::Method>("get_rotation");
		get_alive			= GetClass()->Get<UR::Method>("get_alive");
		get_abilityCom		= GetClass()->Get<UR::Method>("get_abilityCom");
		get_markDie			= GetClass()->Get<UR::Method>("get_markDie");
		get_character		= GetClass()->Get<UR::Method>("get_character");
		get_charCtrl		= GetClass()->Get<UR::Method>("get_charCtrl");
        get_baseController  = GetClass()->Get<UR::Method>("get_baseController");
		get_skMorphCom		= GetClass()->Get<UR::Method>("get_skMorphCom");
		get_rvoComponent	= GetClass()->Get<UR::Method>("get_rvoComponent");
		get_movementComponent = GetClass()->Get<UR::Method>("get_movementComponent");
		get_rootCom			= GetClass()->Get<UR::Method>("get_rootCom");
		get_animatorCom		= GetClass()->Get<UR::Method>("get_animatorCom");
		get_characterAnimCom = GetClass()->Get<UR::Method>("get_characterAnimCom");


        LOG("[+] Entity initialized.\n");
		LOG("[*] Entity::IsValid: %p\n", IsValid->function);
		LOG("[*] Entity::get_name: %p\n", get_name->function);
		LOG("[*] Entity::set_name: %p\n", set_name->function);
		LOG("[*] Entity::get_isHide: %p\n", get_isHide->function);
		LOG("[*] Entity::set_isHide: %p\n", set_isHide->function);
		LOG("[*] Entity::get_objectType: %p\n", get_objectType->function);
		LOG("[*] Entity::set_objectType: %p\n", set_objectType->function);
		LOG("[*] Entity::get_position: %p\n", get_position->function);
		LOG("[*] Entity::get_rotation: %p\n", get_rotation->function);
		LOG("[*] Entity::get_alive: %p\n", get_alive->function);
		LOG("[*] Entity::get_abilityCom: %p\n", get_abilityCom->function);
		LOG("[*] Entity::get_markDie: %p\n", get_markDie->function);
		LOG("[*] Entity::get_character: %p\n", get_character->function);
		LOG("[*] Entity::get_charCtrl: %p\n", get_charCtrl->function);
		LOG("[*] Entity::get_baseController: %p\n", get_baseController->function);
		LOG("[*] Entity::get_skMorphCom: %p\n", get_skMorphCom->function);
		LOG("[*] Entity::get_rvoComponent: %p\n", get_rvoComponent->function);
		LOG("[*] Entity::get_movementComponent: %p\n", get_movementComponent->function);
		LOG("[*] Entity::get_rootCom: %p\n", get_rootCom->function);
		LOG("[*] Entity::get_animatorCom: %p\n", get_animatorCom->function);
		LOG("[*] Entity::get_characterAnimCom: %p\n", get_characterAnimCom->function);

		initialized = true;
	}


	
} // namespace SDK
