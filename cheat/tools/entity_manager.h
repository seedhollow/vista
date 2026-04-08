#pragma once
#include <vector>

// singleton manager for entities in the game, such as player, enemies, interactives, etc.

struct Entity {
	void* ptr;
	float distance_to_local_player;
	float current_hp;
	float max_hp;
	UnityResolve::UnityType::Vector3 position;
	std::string name;
};

namespace Manager {
	class EntityManager {
	public:
		static EntityManager& GetInstance() {
			static EntityManager instance;
			return instance;
		}

		void AddEntity(Entity entity);
		void SetEntities(const std::vector<Entity>& entityPtrs);
		void RemoveEntity(Entity entity);
		void Clear();

		const std::vector<Entity>& GetEntities() const;
		Entity GetEntity(size_t index);
		Entity GetLocalPlayer() const;
		size_t GetEntityCount() const;

	private:
		EntityManager() = default;
		~EntityManager() = default;
		EntityManager(const EntityManager&) = delete;
		EntityManager& operator=(const EntityManager&) = delete;

		std::vector<Entity> m_entities;
	};
} // namespace Manager