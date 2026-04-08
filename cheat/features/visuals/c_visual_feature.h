#pragma once
#include "../feature_base.h"
#include "../../tools/entity_manager.h"

namespace Features {
    using UT = UnityResolve::UnityType;

    class CVisualFeature : public IFeature {
    public:
        void Initialize() override;
        void Render()    override;

    private:

        struct FrameCtx {
            UT::Camera* camera = nullptr;
            int         screen_w = 0;
            int         screen_h = 0;
        };

        struct EntityBox {
            ImRect rect;
            bool   valid = false;
        };

        void DrawESP();

        void      DrawSnaplines(Entity entity, const FrameCtx& ctx);
        void      DrawBox(void* entity, const FrameCtx& ctx, const EntityBox& eb);
        void      DrawBone(void* entity, const FrameCtx& ctx);


        float     DrawName(void* entity, const FrameCtx& ctx, const EntityBox& eb, float offset_y);
        float     DrawHealthBar(void* entity, const FrameCtx& ctx, const EntityBox& eb, float offset_y);
        float     DrawDistance(void* entity, const FrameCtx& ctx, const EntityBox& eb, float offset_y);


        EntityBox CalculateEntityBoundingBox(void* entity, const FrameCtx& ctx);


        bool      Filter(const std::string& entity_name) const;

        float     CalcLabelOffsetX(const EntityBox& eb) const;

        static ImU32 ToImColor(const float rgba[4]);
    };

    inline CVisualFeature* g_VisualFeature = nullptr;

} // namespace Features