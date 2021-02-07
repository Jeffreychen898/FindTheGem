#pragma once

#include "../template.h"

namespace GameStates
{
	class Game : public GameStateTemplate
	{
	public:
	private:
		void Render(Rendering::Renderer& renderer, std::vector<std::pair<unsigned int, Rendering::GameCamera*>>& layers) override;
		void Update(std::vector<std::pair<unsigned int, Rendering::GameCamera*>>& layers) override;
		void KeyPressed() override;
		void KeyReleased() override;
	};
}