#include "Vector4.h"

namespace GameLibMath
{
	Vector4::Vector4(float x, float y, float z, float w)
	{
		SetData({ x, y, z, w });
	}

	void Vector4::SetData(GameLibMath::Vector4Data data)
	{
		m_vector[0] = data.x;
		m_vector[1] = data.y;
		m_vector[2] = data.z;
		m_vector[3] = data.w;
	}

	void Vector4::Multiply(Matrix4 matrix)
	{
		//copy the vector
		float vector_copy[4];
		for (unsigned int i = 0; i < 4; i++)
			vector_copy[i] = m_vector[i];

		//multiply the vector by a matrix
		for (unsigned int i = 0; i < 4; i++)
		{
			float cell_value = 0;
			for (unsigned int c = 0; c < 4; c++)
				cell_value += vector_copy[c] * *(matrix.GetRawMatrixPtr() + c * 4 + i);

			m_vector[i] = cell_value;
		}
	}

	void Vector4::Print()
	{
		for (unsigned int i = 0; i < 4; i++)
			std::cout << m_vector[i] << " ";

		std::cout << std::endl;
	}
}