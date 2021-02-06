#pragma once

#include <GL/glew.h>

namespace Rendering
{
	namespace Objects
	{
		class UniformBufferObject
		{
		private:
			unsigned int m_uniformBufferObject;
		public:
			UniformBufferObject();

			void CreateUniformObject(unsigned int totalBytes, unsigned int bindingPoint);
			void UniformObjectData(unsigned int stride, unsigned int size, float* data);
			void Bind();
		};
	}
}