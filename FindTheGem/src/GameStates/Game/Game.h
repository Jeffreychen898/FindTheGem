#pragma once

#include "../template.h"

namespace GameStates
{
	class Game : public GameStateTemplate
	{
	public:
	private:
		void Render() override;
		void Update() override;
		void KeyPressed() override;
		void KeyReleased() override;
	};
}