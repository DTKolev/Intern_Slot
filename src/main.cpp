#include "../Singleton/Singleton.hpp"
#include "../Game/Game.hpp"

int main() {

    single::Engine engine {"Window", 500, 700};

    engine.Init<MainMenu>();
    engine.Run();

    return 0;
}