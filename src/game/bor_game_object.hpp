#ifndef BOR_GAME_OBJECT_HPP
#define BOR_GAME_OBJECT_HPP

#include <memory>

#include "vk/bor_model.hpp"

namespace bor
{
    struct Transform2DComponent
    {
        glm::vec2 translation{};
        glm::vec2 scale{1.0f, 1.0f};
        float rotation;

        // glm construction is column based, so for the scale matrix it should be like
        //     
        //  X COL         Y COL
        //  __________ _________
        // |         ||         | 
        // | scale.x ||    0    |
        // |_________||_________|
        // |         ||         |
        // |    0    || scale.y |
        // |_________||_________|   
        // 
        // rotation matrix
        //   X COL    Y COL
        //  _______  _______
        // |       ||       | 
        // |  cos  || -sin  |
        // |_______||_______|
        // |       ||       |
        // |  sin  ||  cos  |
        // |_______||_______|   


        glm::mat2 mat2() {
            const float sin = glm::sin(rotation);
            const float cos = glm::cos(rotation);
            glm::mat2 rotationMat{{cos, sin}, {-sin, cos}};
            glm::mat2 scaleMat{{scale.x, 0.0f}, {0.0f, scale.y}};
            return rotationMat * scaleMat; 
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
        Transform2DComponent transform2D{};
    private:
        BoRGameObject(id_t objId) : id{objId} {}

        id_t id;
    };
}

#endif BOR_GAME_OBJECT_HPP