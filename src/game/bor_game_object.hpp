#ifndef BOR_GAME_OBJECT_HPP
#define BOR_GAME_OBJECT_HPP

#include <memory>

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
        glm::mat4 mat4()
        {
            const float c3 = glm::cos(rotation.z);
            const float s3 = glm::sin(rotation.z);
            const float c2 = glm::cos(rotation.x);
            const float s2 = glm::sin(rotation.x);
            const float c1 = glm::cos(rotation.y);
            const float s1 = glm::sin(rotation.y);

            return glm::mat4{
                {
                    scale.x * (c1 * c3 + s1 * s2 * s3),
                    scale.x * (c2 * s3),
                    scale.x * (c1 * s2 * s3 - c3 * s1),
                    0.0f,
                },
                {
                    scale.y * (c3 * s1 * s2 - c1 * s3),
                    scale.y * (c2 * c3),
                    scale.y * (c1 * c3 * s2 + s1 * s3),
                    0.0f,
                },
                {
                    scale.z * (c2 * s1),
                    scale.z * (-s2),
                    scale.z * (c1 * c2),
                    0.0f,
                },
                {translation.x, translation.y, translation.z, 1.0f}};
            }
    };

    class BoRGameObject
    {
    public:
        using id_t = unsigned int;

        static BoRGameObject createGameObject()
        {
            static id_t currentId = 0;
            return BoRGameObject{currentId++};
        }

        BoRGameObject(const BoRGameObject&) = delete;
        BoRGameObject &operator=(const BoRGameObject&) = delete;
        BoRGameObject(BoRGameObject&&) = default;
        BoRGameObject& operator=(BoRGameObject&&) = default;

        id_t getId() { return id; }

        std::shared_ptr<BoRModel> model{};
        glm::vec3 color{};
        TransformComponent transform{};
    private:
        BoRGameObject(id_t objId) : id{objId} {}

        id_t id;
    };
}

#endif BOR_GAME_OBJECT_HPP