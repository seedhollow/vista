#include "pch.h"
#include "object_translator_extensions.h"

namespace SDK {
    UR::Class* ObjectTranslatorExtensions::GetClass() {
        if (!pClass) {
            pClass = UR::Get("Assembly-CSharp.dll")->Get("ObjectTranslatorExtensions");
        }
		return pClass;
	}

    void ObjectTranslatorExtensions::Initialize( ) {
        if (initialized)
            return;

        if (!GetClass()) {
            LOG("[-] ObjectTranslatorExtensions class not found!\n");
            return;
		}

		UpdateBeyondGEnumsAntiCheatBehaviourEnum = UR::Get("Assembly-CSharp.dll")->Get("ObjectTranslatorExtensions")->Get<UR::Method>("UpdateBeyondGEnumsAntiCheatBehaviourEnum", { "ObjectTranslator", "System.IntPtr", "System.Int32", "AntiCheatBehaviourEnum" });
		PushBeyondGEnumsAntiCheatBehaviourEnum   = UR::Get("Assembly-CSharp.dll")->Get("ObjectTranslatorExtensions")->Get<UR::Method>("PushBeyondGEnumsAntiCheatBehaviourEnum", { "ObjectTranslator", "System.IntPtr", "AntiCheatBehaviourEnum" });
		Get                                      = UR::Get("Assembly-CSharp.dll")->Get("ObjectTranslatorExtensions")->Get<UR::Method>("Get", { "ObjectTranslator", "System.IntPtr", "System.Int32", "AntiCheatBehaviourEnum&" });
		UpdateBeyondNetworkBanRoleType           = UR::Get("Assembly-CSharp.dll")->Get("ObjectTranslatorExtensions")->Get<UR::Method>("UpdateBeyondNetworkBanRoleType", { "ObjectTranslator", "System.IntPtr", "System.Int32", "Beyond.Network.BanRoleType" });
		PushBeyondNetworkBanRoleType             = UR::Get("Assembly-CSharp.dll")->Get("ObjectTranslatorExtensions")->Get<UR::Method>("PushBeyondNetworkBanRoleType", { "ObjectTranslator", "System.IntPtr", "Beyond.Network.BanRoleType" });

        initialized = true;

        LOG("[+] ObjectTranslatorExtensions initialized.\n");
		LOG("[*] ObjectTranslatorExtensions::UpdateBeyondGEnumsAntiCheatBehaviourEnum: %p\n", UpdateBeyondGEnumsAntiCheatBehaviourEnum);
		LOG("[*] ObjectTranslatorExtensions::PushBeyondGEnumsAntiCheatBehaviourEnum: %p\n", PushBeyondGEnumsAntiCheatBehaviourEnum);
		LOG("[*] ObjectTranslatorExtensions::Get: %p\n", Get);
    }
} // namespace SDK
