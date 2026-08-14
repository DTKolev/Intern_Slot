#include "../Singleton/Singleton.hpp"
#include "../Game/Game.hpp"
#include <iostream>

int main() {

    try {
        single::Engine engine {"Window", 500, 700};

        engine.Init<MainMenu>();
        engine.Run();
    }
    catch (single::CriticalError& err) {
        std::cout << "Critical error: " << err.GetMessage() << '\n';
        return 1;
    }

    return 0;
}