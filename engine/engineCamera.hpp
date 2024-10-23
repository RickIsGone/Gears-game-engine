#pragma once

#include "engineGameObject.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


namespace gears {

   class EngineCamera {
   public:
      void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);
      void setPerspectiveProjection(float fovy, float aspect, float near, float far);

      void setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{0.0f, -1.0f, 0.0f});
      void setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{0.0f, -1.0f, 0.0f});
      void setViewYXZ(glm::vec3 position, glm::quat rotation);

      const glm::mat4 &getProjection() const { return projectionMatrix; }
      const glm::mat4 &getView() const { return viewMatrix; }

      EngineGameObject& object() { return obj; }

   private:
      EngineGameObject obj = EngineGameObject::createGameObject();
      glm::mat4 projectionMatrix{1.0f};
      glm::mat4 viewMatrix{1.0f};
   };
} // namespace gears