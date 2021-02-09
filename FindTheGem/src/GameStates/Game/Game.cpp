#include "Game.h"

namespace GameStates
{
	Game::Game()
	{
		m_backgroundImage.LoadTexture("res/background.png", false);
		m_tileImage.LoadTexture("res/tiles.png", false);

		m_tileImage.UseSection(0, 0, 16, 16);
	}

	void Game::Render(Rendering::Renderer& renderer, std::vector<std::pair<unsigned int, Rendering::GameCamera*>>& layers)
	{
		/* drawing the background */
		renderer.UseLayer(layers.at(0).first);
		renderer.SetColor(255);
		renderer.DrawImage(m_backgroundImage, 0, 0, 1024, 768);

		/* draw the foreground */
		renderer.UseLayer(layers.at(1).first);
		//renderer.SetColor(255, 0, 0);
		//renderer.DrawImage(m_tileImage, 100, 100, 500, 500);
		for (unsigned int i = 0; i < 5; ++i)
		{
			for (unsigned int j = 0; j < 10; ++j)
				renderer.DrawImage(m_tileImage, j * 50, 500 + i * 50, 50, 50);
		}

		/*renderer.UseLayer(layers.at(3).first);
		renderer.SetColor(0, 255, 0);
		renderer.DrawRect(200, 200, 624, 368);*/

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