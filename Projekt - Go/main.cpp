#include <iostream>
#include "classes.h"

int main() {
 
    std::shared_ptr<GameEngine> engine = std::make_shared<GameEngine>();
    std::shared_ptr<mainMenu> mainMenuInst = std::make_shared<mainMenu>(engine);
    std::shared_ptr<helpScene> helpSceneInst = std::make_shared<helpScene>(engine);
    std::shared_ptr<gameScene> gameSceneInst = std::make_shared<gameScene>(engine);
    sceneCoordinator scenes(engine, mainMenuInst, helpSceneInst, gameSceneInst);
    //mainMenu m;
    //m.show();
    return 0;
}