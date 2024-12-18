module;

#include <memory>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

export module engine.gameObject;
import engineModel;

namespace gears {

   export struct TransformComponent {
      glm::vec3 position{};
      glm::vec3 scale{1.f, 1.f, 1.f};
      glm::quat rotation;

      glm::mat4 mat4();
      glm::mat3 normalMatrix();
   };

   export class EngineGameObject {
   public:
      using id_t = unsigned int;

      static EngineGameObject createGameObject() {
         static id_t currentId = 0;
         return EngineGameObject{currentId++};
      }

      EngineGameObject(const EngineGameObject&) = delete;
      EngineGameObject& operator=(const EngineGameObject&) = delete;
      EngineGameObject(EngineGameObject&&) = default;
      EngineGameObject& operator=(EngineGameObject&&) = default;

      id_t getId() { return _id; };

      std::shared_ptr<EngineModel> model{};
      glm::vec3 color{};

      TransformComponent transform{};

   private:
      EngineGameObject(id_t objId) : _id{objId} {}
      id_t _id;
   };

   // ========================================== implementation ==========================================

   glm::mat4 TransformComponent::mat4() {
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
              0.f,
          },
          {
              scale.y * (c3 * s1 * s2 - c1 * s3),
              scale.y * (c2 * c3),
              scale.y * (c1 * c3 * s2 + s1 * s3),
              0.f,
          },
          {
              scale.z * (c2 * s1),
              scale.z * (-s2),
              scale.z * (c1 * c2),
              0.f,
          },
          {position.x, position.y, position.z, 1.f}};
   }

   glm::mat3 TransformComponent::normalMatrix() {
      const float c3 = glm::cos(rotation.z);
      const float s3 = glm::sin(rotation.z);
      const float c2 = glm::cos(rotation.x);
      const float s2 = glm::sin(rotation.x);
      const float c1 = glm::cos(rotation.y);
      const float s1 = glm::sin(rotation.y);
      const glm::vec3 invScale = 1.0f / scale;
      return glm::mat3{
          {
              invScale.x * (c1 * c3 + s1 * s2 * s3),
              invScale.x * (c2 * s3),
              invScale.x * (c1 * s2 * s3 - c3 * s1),
          },
          {
              invScale.y * (c3 * s1 * s2 - c1 * s3),
              invScale.y * (c2 * c3),
              invScale.y * (c1 * c3 * s2 + s1 * s3),
          },
          {
              invScale.z * (c2 * s1),
              invScale.z * (-s2),
              invScale.z * (c1 * c2),
          }};
   }
} // namespace gears