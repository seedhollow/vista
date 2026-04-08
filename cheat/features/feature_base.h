#pragma once

namespace Features {
    class IFeature {
    public:
        virtual ~IFeature() = default;
        virtual void Initialize() {}
        virtual void Update() {}
        virtual void Render() {}
        virtual void Shutdown() {}
    };
}