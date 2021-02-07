#include <vector>

#include "../GameDevLib/Renderer/Rendering.h"
#include "../GameDevLib/Math/Math.h"

#include "GameStates/gamestates.h"

unsigned int game_state;
std::vector<GameStateTemplate*> states;

void key_pressed(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
        states.at(game_state)->KeyPressed();
    else if (action == GLFW_RELEASE)
        states.at(game_state)->KeyReleased();
}

int main()
{
    unsigned int window_width = 1024;
    unsigned int window_height = 768;
    const char* title = "Find The Gem";

    /* setup renderer and window */
    Rendering::Renderer renderer;
    Rendering::Window window;

    window.Init(window_width, window_height, title);
    renderer.Init(window_width, window_height);

    window.AccessRenderer(&renderer);

    /* window event callback */
    window.SetKeyCallback(key_pressed);

    /* game states */
    game_state = 0;
    states.reserve(1);
    states.push_back(new GameStates::Game());

    /* game loop */
    while (!window.WindowShouldClose())
    {
        window.StartOfFrame();
        std::cout << "[FPS]: " << window.GetFrameRate() << "\n";

        states.at(game_state)->Update();
        states.at(game_state)->Render();

        /* end of the frame */
        window.EndOfFrame();
    }

    /* before program closes */
    for (GameStateTemplate* states : states)
        delete states;

    states.clear();

    window.Terminate();

    return 0;
}