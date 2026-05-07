#ifndef BOR_GAME_OBJECT_HPP
#define BOR_GAME_OBJECT_HPP

#include <memory>
#include <unordered_map>

#include <glm/gtc/matrix_transform.hpp>

#include "vk/bor_model.hpp"

namespace bor
{
    struct TransformComponent
    {
        glm::vec3 translation{};
        glm::vec3 scale{1.0f, 1.0f, 1.0f};
        glm::vec3 rotation;


        // model matrix = translate * Ry * Rx * Rz * scale
        // rotation convetion used tait-bryan with axis order Y(1), X(2), Z(3)
        // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
        
        glm::mat4 mat4();
        glm::mat3 normalMatrix();
    };

    struct PointLightComponent
    {
        float lightIntensity = 1.0f;
    };

    class BoRGameObject
    {
    public:
        using id_t = unsigned int;
        using Map = std::unordered_map<id_t, BoRGameObject>;

        static BoRGameObject createGameObject()
        {
            static id_t currentId = 0;
            return BoRGameObject{currentId++};
        }
        
        static BoRGameObject makePointLight(float intensity = 10.0f, float radius = 0.1f, glm::vec3 color = glm::vec3(1.0f));

        BoRGameObject(const BoRGameObject&) = delete;
        BoRGameObject &operator=(const BoRGameObject&) = delete;
        BoRGameObject(BoRGameObject&&) = default;
        BoRGameObject& operator=(BoRGameObject&&) = default;

        id_t getId() { return id; }

        glm::vec3 color{};
        TransformComponent transform{};
        
        //optional components;
        std::shared_ptr<BoRModel> model{};
        std::unique_ptr<PointLightComponent> pointLight = nullptr;

    private:
        BoRGameObject(id_t objId) : id{objId} {}

        id_t id;
    };
}

#endif BOR_GAME_OBJECT_HPP