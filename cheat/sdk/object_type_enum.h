#pragma once
#include <cstdint>

enum class ObjectType_enum : uint32_t {
	All = 0xFFFFFFFF,
	Invalid = 1,
    Character = 8,
    Enemy = 16,
    Interactive = 32,
    Projectile = 64,
    FactoryRegion = 128,
    Npc = 256,
    AbilityEntity = 512,
    CinematicEntity = 1024,
    RemoteFactoryEntity = 2048,
    Creature = 4096,
    GodEntity = 8192,
    EnemyPart = 16384,
    SocialBuilding = 32768,
    EnemyAll = 16400
};
