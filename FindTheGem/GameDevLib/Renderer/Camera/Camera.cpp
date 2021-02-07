#include "Camera.h"

namespace Rendering
{
	void GameCamera::Create2DCamera(float left, float right, float top, float bottom)
	{
		Identity();
		float r_minus_l = right - left;
		float r_plus_l = right + left;
		float t_minus_b = top - bottom;
		float t_plus_b = top + bottom;
		float projection_matrix[16] = {
			2.f / r_minus_l, 0.f, 0.f, 0.f,
			0.f, 2.f / (t_minus_b), 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			-(r_plus_l / r_minus_l), -(t_plus_b / t_minus_b), 0.f, 1.f
		};
		MultiplyRawMatrix(&projection_matrix[0]);
	}
}