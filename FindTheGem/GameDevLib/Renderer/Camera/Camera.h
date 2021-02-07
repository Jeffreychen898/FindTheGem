#pragma once

#include "../../Math/Math.h"

namespace Rendering
{
	class GameCamera : public GameLibMath::Matrix4
	{
	public:
		void Create2DCamera(float left, float right, float top, float bottom);
	};
}