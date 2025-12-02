#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include "Entity.h"
#include "Components.h" // TagComponent, TransformComponent, CameraComponent, etc.

namespace Klein {

    class Scene {
    public:
        Scene(const std::string& name = "Untitled Scene");
        ~Scene();

        // Entity management
        Entity CreateEntity(const std::string& name = "Entity");
        void DestroyEntity(Entity entity);

        // Queries
        std::vector<Entity> GetAllEntities();
        template<typename T>
        std::vector<Entity> GetEntitiesWithComponent() {
                    return GetEntitiesWithComponent(typeid(T));
                }
        std::vector<Entity> GetEntitiesWithComponent(std::type_index componentType);

        Entity GetPrimaryCamera();
        std::vector<Entity> GetLights();

        // Scene lifecycle
        void OnUpdate(float deltaTime);
        void OnRender();

        // Serialization
        bool Save(const std::string& filepath);
        static std::shared_ptr<Scene> Load(const std::string& filepath);

        const std::string& GetName() const { return m_name; }

    private:
        std::string m_name;
        std::vector<std::shared_ptr<EntityData>> m_entities;
        unsigned int m_nextEntityID = 1;
    };

} // namespace Klein

#endif // SCENE_H
