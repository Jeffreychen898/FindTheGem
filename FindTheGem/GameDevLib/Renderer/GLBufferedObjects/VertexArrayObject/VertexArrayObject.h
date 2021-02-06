#pragma once

#include <unordered_map>
#include <iostream>

#include <GL/glew.h>

namespace Rendering
{
	namespace Objects
	{
		class VertexArrayObject
		{
		private:
			unsigned int m_vertexArray;
			std::unordered_map<unsigned int, unsigned int> m_vertexBufferObjects;//location, buffer object id
		public:
			VertexArrayObject();
			~VertexArrayObject();

			void CreateVertexArray();

			void CreateVertexBufferObject(unsigned int location, unsigned int count);
			void VertexBufferObjectData(unsigned int location, unsigned int size, float data[], GLenum usage);

			void Bind() const { glBindVertexArray(m_vertexArray); };
		};
	}
}