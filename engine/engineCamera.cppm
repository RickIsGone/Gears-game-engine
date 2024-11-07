module;

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "engineGameObject.hpp"

export module engine.camera;

namespace gears {

   export class EngineCamera {
   public:
      void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);
      void setPerspectiveProjection(float fovy, float aspect, float near, float far);

      void setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{0.0f, -1.0f, 0.0f});
      void setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{0.0f, -1.0f, 0.0f});
      void setViewYXZ(glm::vec3 position, glm::quat rotation);

      const glm::mat4& getProjection() const { return _projectionMatrix; }
      const glm::mat4& getView() const { return _viewMatrix; }

      EngineGameObject& object() { return _object; }

   private:
      EngineGameObject _object = EngineGameObject::createGameObject();
      glm::mat4 _projectionMatrix{1.0f};
      glm::mat4 _viewMatrix{1.0f};
   };

   //  ========================================== implementation ==========================================

   void EngineCamera::setOrthographicProjection(float left, float right, float top, float bottom, float near, float far) {
      _projectionMatrix = glm::mat4{1.f};
      _projectionMatrix[0][0] = 2.f / (right - left);
      _projectionMatrix[1][1] = 2.f / (bottom - top);
      _projectionMatrix[2][2] = 1.f / (far - near);
      _projectionMatrix[3][0] = -(right + left) / (right - left);
      _projectionMatrix[3][1] = -(bottom + top) / (bottom - top);
      _projectionMatrix[3][2] = -near / (far - near);
   }

   void EngineCamera::setPerspectiveProjection(float fovy, float aspect, float near, float far) {
      assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.f);
      const float tanHalfFovy = tan(fovy / 2.f);
      _projectionMatrix = glm::mat4{0.f};
      _projectionMatrix[0][0] = 1.f / (aspect * tanHalfFovy);
      _projectionMatrix[1][1] = 1.f / (tanHalfFovy);
      _projectionMatrix[2][2] = far / (far - near);
      _projectionMatrix[2][3] = 1.f;
      _projectionMatrix[3][2] = -(far * near) / (far - near);
   }

   void EngineCamera::setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up) {
      const glm::vec3 w{glm::normalize(direction)};
      const glm::vec3 u{glm::normalize(glm::cross(w, up))};
      const glm::vec3 v{glm::cross(w, u)};

      _viewMatrix = glm::mat4{1.f};
      _viewMatrix[0][0] = u.x;
      _viewMatrix[1][0] = u.y;
      _viewMatrix[2][0] = u.z;
      _viewMatrix[0][1] = v.x;
      _viewMatrix[1][1] = v.y;
      _viewMatrix[2][1] = v.z;
      _viewMatrix[0][2] = w.x;
      _viewMatrix[1][2] = w.y;
      _viewMatrix[2][2] = w.z;
      _viewMatrix[3][0] = -glm::dot(u, position);
      _viewMatrix[3][1] = -glm::dot(v, position);
      _viewMatrix[3][2] = -glm::dot(w, position);
   }

   void EngineCamera::setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up) {
      setViewDirection(position, target - position, up);
   }

   void EngineCamera::setViewYXZ(glm::vec3 position, glm::quat rotation) {
      auto t = glm::translate(glm::mat4(1.f), position);
      auto r = glm::mat4_cast(rotation);
      auto camera = t * r;
      _viewMatrix = glm::affineInverse(camera);
   }
} // namespace gears
