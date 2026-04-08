#include "pch.h"
#include "entity_render_helper.h"

namespace SDK {
	UR::Class* EntityRenderHelper::GetClass( ) {
        if (!pClass) {
			pClass = UR::Get("Gameplay.Beyond.dll")->Get("EntityRenderHelper");
        }
		return pClass;
    }

	void EntityRenderHelper::Initialize( ) {
		if (initialized)
			return;

		if (!GetClass()) {
			LOG("[-] EntityRenderHelper class not found!\n");
			return;
		}

		Tick						= GetClass()->Get<UR::Method>("Tick", {"System.Single"});
		SetManualDitherAlphaValue	= GetClass()->Get<UR::Method>("SetManualDitherAlphaValue", {"System.Single"});
		AddMaterialToAllRenderer	= GetClass()->Get<UR::Method>("AddMaterialToAllRenderer", { "UnityEngine.Material", "Beyond.Rendering.EntityVFXRendererMask" });
		
		LOG("[+] EntityRenderHelper initialized.\n");
        LOG("[*] EntityRenderHelper::Tick: %p\n", Tick ? Tick->function : nullptr);
		LOG("[*] EntityRenderHelper::SetManualDitherAlphaValue: %p\n", SetManualDitherAlphaValue ? SetManualDitherAlphaValue->function : nullptr);
		LOG("[*] EntityRenderHelper::AddMaterialToAllRenderer: %p\n", AddMaterialToAllRenderer ? AddMaterialToAllRenderer->function : nullptr);
	}
} // namespace SDK
