#pragma once

#include "../Matrix4/Matrix4.h"

namespace GameLibMath
{
	struct Vector4Data
	{
		float x;
		float y;
		float z;
		float w;
	};

	class Vector4
	{
	private:
		float m_vector[4];
	public:
		Vector4(float x, float y, float z, float w);

		void SetData(unsigned int component, float value) { m_vector[component] = value; };
		void SetData(GameLibMath::Vector4Data data);

		void Multiply(Matrix4 matrix);

		GameLibMath::Vector4Data GetVector() const { return { m_vector[0], m_vector[1] , m_vector[2] , m_vector[3] }; };

		void Print();
	};
}