#include "Application.hpp"
#include "logger.hpp"

namespace gears {

   Application::Application(int width, int height, const std::string& windowName)
       : _window{width, height, windowName},
         _device{_window},
         _renderer{_window, _device},
         _renderSystem{_device, _renderer.getSwapChainRenderPass()} {
      loadGameObjects();
   }

   void Application::loadGameObjects() {
      std::shared_ptr<EngineModel> engineModel = EngineModel::createModelFromFile(_device, "fish.obj");

      auto gameObject = EngineGameObject::createGameObject();
      gameObject.model = engineModel;
      gameObject.transform.position = {0.f, 0.f, 2.f};
      gameObject.transform.scale = {0.5f, 0.5f, 0.5f};

      _gameObjects.push_back(std::move(gameObject));

      logger->log("Loaded model \"fish.obj\"");
   }

} // namespace gears
