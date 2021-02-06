#pragma once

#include <iostream>

namespace GameLibMath
{
	class Matrix4
	{
	private:
		float m_matrix[4][4];
	public:
		Matrix4();
		~Matrix4();

		void Identity();
		void Transpose();
		void Inverse();

		void Multiply(Matrix4& matrix);
		void MultiplyRawMatrix(float* firstIndex);

		float* GetRawMatrixPtr() { return &m_matrix[0][0]; };

		void Print();
	};
}