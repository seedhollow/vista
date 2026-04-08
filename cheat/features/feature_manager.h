#pragma once
#include "feature_base.h"
#include <vector>

namespace Features {
    class FeatureManager {
    private:
        static inline std::vector<IFeature*> features;

    public:
        static void RegisterFeature(IFeature* feature) {
            features.push_back(feature);
        }

        static void Initialize() {
            for (auto* feature : features) {
                feature->Initialize();
            }
        }

        static void Update() {
            for (auto* feature : features) {
                feature->Update();
            }
        }

        static void Render() {
            for (auto* feature : features) {
                feature->Render();
            }
        }

        static void Shutdown() {
            for (auto* feature : features) {
                feature->Shutdown();
            }
        }
    };
}