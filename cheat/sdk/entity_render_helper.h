#pragma once

namespace SDK {
	using UR = UnityResolve;

	class EntityRenderHelper {
	public:

		static UR::Class* GetClass( );

		static void Initialize( );
		// Function pointers
		inline static UR::Method* Tick= nullptr;
		inline static UR::Method* SetManualDitherAlphaValue = nullptr;
		inline static UR::Method* AddMaterialToAllRenderer = nullptr;
	private:
		inline static bool initialized = false;
		inline static UR::Class* pClass = nullptr;
	};
} // namespace SDK
