#include "Client/GameApp.h"

int main() {
    auto app = new GameApp();
    app->Run();
    delete app;
    return 0;
}
