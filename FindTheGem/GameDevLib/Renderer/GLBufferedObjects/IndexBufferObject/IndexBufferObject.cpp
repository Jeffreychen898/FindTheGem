#include "IndexBufferObject.h"

namespace Rendering
{
	namespace Objects
	{
		IndexBufferObject::IndexBufferObject()
			: m_indexBuffer(0)
		{
			//
		}

		void IndexBufferObject::CreateIndexBufferObject()
		{
			glGenBuffers(1, &m_indexBuffer);
			Bind();
		}

		void IndexBufferObject::IndexBufferData(unsigned int size, unsigned int data[], GLenum usage)
		{
			Bind();
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
		}

		IndexBufferObject::~IndexBufferObject()
		{
			//
		}
	}
}