#include <vector>

#include "../GameDevLib/Renderer/Rendering.h"

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

    /* layers and camera */
    std::vector<std::pair<unsigned int, Rendering::GameCamera*>> layers;
    layers.reserve(4);

    Rendering::GameCamera background_camera;
    Rendering::GameCamera foreground_camera;
    Rendering::GameCamera user_interface_camera;

    background_camera.Create2DCamera(0, (float)window_width, 0, (float)window_height);
    foreground_camera.Create2DCamera(0, (float)window_width, 0, (float)window_height);
    user_interface_camera.Create2DCamera(0, (float)window_width, 0, (float)window_height);

    unsigned int background_layer, foreground_layer, post_processing_layer, user_interface_layer;
    renderer.CreateLayer(background_layer, &background_camera);
    renderer.CreateLayer(foreground_layer, &foreground_camera);
    renderer.CreateLayer(post_processing_layer, &foreground_camera);
    renderer.CreateLayer(user_interface_layer, &user_interface_camera);

    layers.push_back({ background_layer, &background_camera });
    layers.push_back({ foreground_layer, &foreground_camera });
    layers.push_back({ post_processing_layer, &foreground_camera });
    layers.push_back({ user_interface_layer, &user_interface_camera });

    /* game states */
    game_state = 0;
    states.reserve(1);
    states.push_back(new GameStates::Game());

    /* game loop */
    while (!window.WindowShouldClose())
    {
        window.StartOfFrame();
        std::cout << "[FPS]: " << window.GetFrameRate() << "\n";

        states.at(game_state)->Update(layers);
        states.at(game_state)->Render(renderer, layers);

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