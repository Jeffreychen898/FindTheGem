#pragma once

#include <iostream>

#include "../../GameDevLib/Renderer/Rendering.h"

class GameStateTemplate
{
public:
	virtual void Render() {}
	virtual void Update() {}
	virtual void KeyPressed() {}
	virtual void KeyReleased() {}
};