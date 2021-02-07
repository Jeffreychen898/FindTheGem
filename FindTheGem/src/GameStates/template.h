#pragma once

#include <iostream>
#include <vector>

#include "../../GameDevLib/Renderer/Rendering.h"

class GameStateTemplate
{
public:
	virtual void Render(Rendering::Renderer& renderer, std::vector<std::pair<unsigned int, Rendering::GameCamera*>>& layers) {}
	virtual void Update(std::vector<std::pair<unsigned int, Rendering::GameCamera*>>& layers) {}
	virtual void KeyPressed() {}
	virtual void KeyReleased() {}
};