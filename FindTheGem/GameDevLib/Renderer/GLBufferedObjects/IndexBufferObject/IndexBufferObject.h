#pragma once

#include <GL/glew.h>

namespace Rendering
{
	namespace Objects
	{
		class IndexBufferObject
		{
		private:
			unsigned int m_indexBuffer;
		public:
			IndexBufferObject();
			~IndexBufferObject();

			void CreateIndexBufferObject();
			void IndexBufferData(unsigned int size, unsigned int data[], GLenum usage);

			void Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer); };
		};
	}
}