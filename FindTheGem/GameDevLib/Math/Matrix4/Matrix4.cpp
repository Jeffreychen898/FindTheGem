#include "Matrix4.h"

namespace GameLibMath
{
	Matrix4::Matrix4()
	{
		Identity();
	}

	void Matrix4::Identity()
	{
		for (unsigned int i = 0; i < 16; ++i)
			m_matrix[(unsigned int)(i / 4)][i % 4] = (i % 5 == 0) ? 1.f : 0.f;
	}

	void Matrix4::Transpose()
	{
		float matrix_copy[4][4];
		for (unsigned int i = 0; i < 16; ++i)
			matrix_copy[(unsigned int)(i / 4)][i % 4] = m_matrix[(unsigned int)(i / 4)][i % 4];

		for (unsigned int i = 0; i < 16; ++i)
			m_matrix[(unsigned int)(i / 4)][i % 4] = matrix_copy[i % 4][(unsigned int)(i / 4)];
	}

	void Matrix4::Inverse()
	{
		float adjugate[4][4];
		for (unsigned int r = 0; r < 4; ++r)
		{
			for (unsigned int c = 0; c < 4; ++c)
			{
				float cofactor_matrix[3][3];
				for (unsigned int i = 0; i < 3; ++i)
				{
					for (unsigned int j = 0; j < 3; ++j)
						cofactor_matrix[i][j] = m_matrix[(i >= r) ? i + 1 : i][(j >= c) ? j + 1 : j];
				}

				float determinant = cofactor_matrix[0][0] * cofactor_matrix[1][1] * cofactor_matrix[2][2]
					+ cofactor_matrix[0][1] * cofactor_matrix[1][2] * cofactor_matrix[2][0]
					+ cofactor_matrix[0][2] * cofactor_matrix[1][0] * cofactor_matrix[2][1]
					- cofactor_matrix[0][0] * cofactor_matrix[1][2] * cofactor_matrix[2][1]
					- cofactor_matrix[0][1] * cofactor_matrix[1][0] * cofactor_matrix[2][2]
					- cofactor_matrix[1][1] * cofactor_matrix[2][0] * cofactor_matrix[0][2];

				adjugate[c][r] = ((c - r) % 2 == 0) ? determinant : -determinant;
			}
		}
		float determinant = 0;
		for (unsigned int i = 0; i < 4; ++i)
			determinant += m_matrix[0][i] * adjugate[i][0];

		if (determinant == 0)
		{
			std::cout << "[ERR] Matrix4 determinent is 0, inverse cannot be found!\n";
			return;
		}

		determinant = 1.f / determinant;

		for (unsigned int i = 0; i < 16; ++i)
			m_matrix[(unsigned int)(i / 4)][i % 4] = determinant * adjugate[(unsigned int)(i / 4)][i % 4];
	}

	void Matrix4::Multiply(Matrix4& matrix)
	{
		MultiplyRawMatrix(matrix.GetRawMatrixPtr());
	}

	void Matrix4::MultiplyRawMatrix(float* firstIndex)
	{
		float matrix_copy[4][4];
		for (unsigned int i = 0; i < 16; ++i)
			matrix_copy[(unsigned int)(i / 4)][i % 4] = m_matrix[(unsigned int)(i / 4)][i % 4];

		for (unsigned int r = 0; r < 4; ++r)
		{
			for (unsigned int c = 0; c < 4; ++c)
			{
				float cell_value = 0;
				for (unsigned int i = 0; i < 4; ++i)
				{
					cell_value += matrix_copy[r][i] * (*(firstIndex + i * 4 + c));
				}
				m_matrix[r][c] = cell_value;
			}
		}
	}

	void Matrix4::Print()
	{
		for (unsigned int i = 0; i < 4; ++i)
		{
			for (unsigned int j = 0; j < 4; ++j)
				std::cout << m_matrix[i][j] << " ";

			std::cout << "\n";
		}
	}

	Matrix4::~Matrix4()
	{
		//
	}
}