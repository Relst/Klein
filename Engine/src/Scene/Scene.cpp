#include "Scene.h"
#include "Logger.h"
#include <algorithm>

namespace Klein {

    Scene::Scene(const std::string& name)
        : m_name(name)
    {
        KleinLogger::Logger::EngineLog("Scene created: %s", name.c_str());
    }

    Scene::~Scene() {
        m_entities.clear();
        KleinLogger::Logger::EngineLog("Scene destroyed: %s", m_name.c_str());
    }

    Entity Scene::CreateEntity(const std::string& name) {
        auto entityData = std::make_shared<EntityData>();
        entityData->id = m_nextEntityID++;

        // Every entity gets a Tag and Transform by default
        entityData->AddComponent<TagComponent>(name);
        entityData->AddComponent<TransformComponent>();

        m_entities.push_back(entityData);

        Entity entity(entityData, this);
        KleinLogger::Logger::EngineLog("Entity created: %s (ID: %u)", name.c_str(), entity.GetID());
        return entity;
    }

    void Scene::DestroyEntity(Entity entity) {
        if (!entity.IsValid()) return;

        auto it = std::find_if(m_entities.begin(), m_entities.end(),
            [&entity](const std::shared_ptr<EntityData>& data) {
                return data->id == entity.GetID();
            });

        if (it != m_entities.end()) {
            KleinLogger::Logger::EngineLog("Entity destroyed (ID: %u)", entity.GetID());
            m_entities.erase(it);
        }
    }

    std::vector<Entity> Scene::GetEntitiesWithComponent(std::type_index componentType) {
        std::vector<Entity> result;
        for (auto& entityData : m_entities) {
            if (entityData->components.find(componentType) != entityData->components.end()) {
                result.emplace_back(entityData, this);
            }
        }
        return result;
    }

    std::vector<Entity> Scene::GetAllEntities() {
        std::vector<Entity> result;
        for (auto& entityData : m_entities) {
            result.emplace_back(entityData, this);
        }
        return result;
    }

    Entity Scene::GetPrimaryCamera() {
        auto cameras = GetEntitiesWithComponent<CameraComponent>();
        for (auto& entity : cameras) {
            if (entity.GetComponent<CameraComponent>().primary) {
                return entity;
            }
        }
        return Entity(); // Invalid entity if no primary camera found
    }

    std::vector<Entity> Scene::GetLights() {
        return GetEntitiesWithComponent<LightComponent>();
    }

    void Scene::OnUpdate(float deltaTime) {
        // Update all script components
        auto scriptEntities = GetEntitiesWithComponent<ScriptComponent>();
        for (auto& entity : scriptEntities) {
            auto& script = entity.GetComponent<ScriptComponent>();
            if (script.onUpdateFunc && script.instance) {
                script.onUpdateFunc(script.instance, deltaTime);
            }
        }

        // Update physics (rigidbodies)
        auto rigidBodies = GetEntitiesWithComponent<RigidbodyComponent>();
        for (auto& entity : rigidBodies) {
            auto& rb = entity.GetComponent<RigidbodyComponent>();
            auto& transform = entity.GetComponent<TransformComponent>();

            if (rb.type == RigidbodyComponent::BodyType::Dynamic) {
                // Apply gravity
                if (rb.useGravity) {
                    rb.velocity.y -= 9.81f * deltaTime;
                }

                // Apply velocity
                transform.position += rb.velocity * deltaTime;

                // Apply drag
                rb.velocity *= (1.0f - rb.drag * deltaTime);
            }
        }
    }

    void Scene::OnRender() {
        // This will be called by the renderer
        // The renderer will query entities with MeshRendererComponent
        // and render them accordingly
    }

    bool Scene::Save(const std::string& filepath) {
        // TODO: Implement scene serialization
        KleinLogger::Logger::EngineWarn("Scene serialization not yet implemented");
        return false;
    }

    std::shared_ptr<Scene> Scene::Load(const std::string& filepath) {
        // TODO: Implement scene deserialization
        KleinLogger::Logger::EngineWarn("Scene deserialization not yet implemented");
        return std::make_shared<Scene>("Loaded Scene");
    }



} // namespace Klein