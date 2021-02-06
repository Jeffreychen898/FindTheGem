#include "UniformBufferObject.h"

namespace Rendering
{
	namespace Objects
	{
		UniformBufferObject::UniformBufferObject()
			: m_uniformBufferObject(0)
		{
		}

		void UniformBufferObject::CreateUniformObject(unsigned int totalBytes, unsigned int bindingPoint)
		{
			glGenBuffers(1, &m_uniformBufferObject);
			Bind();
			glBufferData(GL_UNIFORM_BUFFER, totalBytes, 0, GL_DYNAMIC_DRAW);
			glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_uniformBufferObject);
		}

		void UniformBufferObject::Bind()
		{
			glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferObject);
		}

		void UniformBufferObject::UniformObjectData(unsigned int stride, unsigned int size, float* data)
		{
			glBufferSubData(GL_UNIFORM_BUFFER, stride, size, data);
		}
	}
}