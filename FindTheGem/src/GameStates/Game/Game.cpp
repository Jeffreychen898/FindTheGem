#include "Game.h"

namespace GameStates
{
	void Game::Render(Rendering::Renderer& renderer, std::vector<std::pair<unsigned int, Rendering::GameCamera*>>& layers)
	{
		//std::cout << "rendering\n";
	}
	
	void Game::Update(std::vector<std::pair<unsigned int, Rendering::GameCamera*>>& layers)
	{
		//std::cout << "updating\n";
	}

	void Game::KeyPressed()
	{
		//std::cout << "pressed\n";
	}

	void Game::KeyReleased()
	{
		//std::cout << "released\n";
	}
}