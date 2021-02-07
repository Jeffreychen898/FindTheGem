#include "Game.h"

namespace GameStates
{
	Game::Game()
	{
	}

	void Game::Render(Rendering::Renderer& renderer, std::vector<std::pair<unsigned int, Rendering::GameCamera*>>& layers)
	{
		renderer.UseLayer(layers.at(0).first);
		renderer.SetColor(0, 0, 255);
		renderer.DrawRect(0, 0, 1024, 768);
		renderer.UseLayer(layers.at(1).first);
		renderer.SetColor(255, 0, 0);
		renderer.DrawRect(100, 100, 824, 568);
		renderer.UseLayer(layers.at(3).first);
		renderer.SetColor(0, 255, 0);
		renderer.DrawRect(200, 200, 624, 368);

		m_layersToRender.push_back(renderer.SingleLayer(layers.at(0).first));
		m_layersToRender.push_back(renderer.SingleLayer(layers.at(1).first));
		m_layersToRender.push_back(renderer.SingleLayer(layers.at(3).first));

		renderer.RenderLayers(m_layersToRender);
		m_layersToRender.clear();
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