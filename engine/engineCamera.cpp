#include <glm/gtc/matrix_inverse.hpp>

#include "engineCamera.hpp"

namespace gears {

   void EngineCamera::setOrthographicProjection(float left, float right, float top, float bottom, float near, float far) {
      projectionMatrix = glm::mat4{1.f};
      projectionMatrix[0][0] = 2.f / (right - left);
      projectionMatrix[1][1] = 2.f / (bottom - top);
      projectionMatrix[2][2] = 1.f / (far - near);
      projectionMatrix[3][0] = -(right + left) / (right - left);
      projectionMatrix[3][1] = -(bottom + top) / (bottom - top);
      projectionMatrix[3][2] = -near / (far - near);
   }

   void EngineCamera::setPerspectiveProjection(float fovy, float aspect, float near, float far) {
      assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.f);
      const float tanHalfFovy = tan(fovy / 2.f);
      projectionMatrix = glm::mat4{0.f};
      projectionMatrix[0][0] = 1.f / (aspect * tanHalfFovy);
      projectionMatrix[1][1] = 1.f / (tanHalfFovy);
      projectionMatrix[2][2] = far / (far - near);
      projectionMatrix[2][3] = 1.f;
      projectionMatrix[3][2] = -(far * near) / (far - near);
   }

   void EngineCamera::setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up) {
      const glm::vec3 w{glm::normalize(direction)};
      const glm::vec3 u{glm::normalize(glm::cross(w, up))};
      const glm::vec3 v{glm::cross(w, u)};

      viewMatrix = glm::mat4{1.f};
      viewMatrix[0][0] = u.x;
      viewMatrix[1][0] = u.y;
      viewMatrix[2][0] = u.z;
      viewMatrix[0][1] = v.x;
      viewMatrix[1][1] = v.y;
      viewMatrix[2][1] = v.z;
      viewMatrix[0][2] = w.x;
      viewMatrix[1][2] = w.y;
      viewMatrix[2][2] = w.z;
      viewMatrix[3][0] = -glm::dot(u, position);
      viewMatrix[3][1] = -glm::dot(v, position);
      viewMatrix[3][2] = -glm::dot(w, position);
   }

   void EngineCamera::setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up) {
      setViewDirection(position, target - position, up);
   }

   void EngineCamera::setViewYXZ(glm::vec3 position, glm::quat rotation) {
      auto t = glm::translate(glm::mat4(1.f), position);
      auto r = glm::mat4_cast(rotation);
      auto camera = t * r;
      viewMatrix = glm::affineInverse(camera);
   }
} // namespace gears
