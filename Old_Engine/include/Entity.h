#ifndef ENTITY_H
#define ENTITY_H

#include <memory>
#include <unordered_map>
#include <typeindex>
#include <any>

namespace Klein {

    class Scene;

    // Component storage per entity
    struct EntityData {
        uint32_t id;
        std::unordered_map<std::type_index, std::any> components;
        
        template<typename T>
        bool HasComponent() const {
            return components.find(std::type_index(typeid(T))) != components.end();
        }
        
        template<typename T>
        T& GetComponent() {
            return std::any_cast<T&>(components[std::type_index(typeid(T))]);
        }
        
        template<typename T>
        const T& GetComponent() const {
            return std::any_cast<const T&>(components.at(std::type_index(typeid(T))));
        }
        
        template<typename T, typename... Args>
        T& AddComponent(Args&&... args) {
            components[std::type_index(typeid(T))] = T(std::forward<Args>(args)...);
            return GetComponent<T>();
        }
        
        template<typename T>
        void RemoveComponent() {
            components.erase(std::type_index(typeid(T)));
        }
    };

    // Entity is a lightweight wrapper around EntityData
    class Entity {
    public:
        Entity() = default;
        Entity(std::shared_ptr<EntityData> data, Scene* scene)
            : m_data(data), m_scene(scene) {}

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args) {
            return m_data->AddComponent<T>(std::forward<Args>(args)...);
        }

        template<typename T>
        T& GetComponent() {
            return m_data->GetComponent<T>();
        }

        template<typename T>
        const T& GetComponent() const {
            return m_data->GetComponent<T>();
        }

        template<typename T>
        bool HasComponent() const {
            return m_data->HasComponent<T>();
        }

        template<typename T>
        void RemoveComponent() {
            m_data->RemoveComponent<T>();
        }

        uint32_t GetID() const { return m_data->id; }
        bool IsValid() const { return m_data != nullptr; }
        
        operator bool() const { return IsValid(); }
        operator uint32_t() const { return GetID(); }

        bool operator==(const Entity& other) const {
            return m_data == other.m_data && m_scene == other.m_scene;
        }

        bool operator!=(const Entity& other) const {
            return !(*this == other);
        }

    private:
        std::shared_ptr<EntityData> m_data;
        Scene* m_scene = nullptr;

        friend class Scene;
    };

} // namespace Klein

#endif // ENTITY_H