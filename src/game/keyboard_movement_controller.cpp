#include "keyboard_movement_controller.hpp"

namespace bor
{
    void KeyboardMovementController::MoveInPlaneXZ(GLFWwindow* window, float dt, BoRGameObject& gameObject)
    {
        glm::vec3 rotate{0.0f};
        if(glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotate.y += 1.0f;
        if(glfwGetKey(window, keys.lookfleft) == GLFW_PRESS) rotate.y -= 1.0f;
        if(glfwGetKey(window, keys.lookUp) == GLFW_PRESS) rotate.x += 1.0f;
        if(glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotate.x -= 1.0f;

        if(glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
        {
            gameObject.transform.rotation += lookSpeed * dt * glm::normalize(rotate);
        }

        gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f); // clamp around 85~ degrees
        gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::tau<float>());

        float yaw = gameObject.transform.rotation.y;
        const glm::vec3 forwardDirection{sin(yaw), 0.0f, cos(yaw)};
        const glm::vec3 rightDirection{forwardDirection.z, 0.0f, -forwardDirection.x};
        const glm::vec3 upDirection{0.0f, -1.0f, 0.0f};

        glm::vec3 moveDirection{0.0f};
        if(glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDirection += forwardDirection;
        if(glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDirection -= forwardDirection;
        if(glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDirection += rightDirection;
        if(glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDirection -= rightDirection;
        if(glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDirection += upDirection;
        if(glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDirection -= upDirection;

        if(glm::dot(moveDirection, moveDirection) > std::numeric_limits<float>::epsilon())
        {
            gameObject.transform.translation += moveSpeed * dt * glm::normalize(moveDirection);
        }
    }

}