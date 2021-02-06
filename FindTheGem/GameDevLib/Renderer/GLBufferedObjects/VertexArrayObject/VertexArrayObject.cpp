#include "VertexArrayObject.h"

namespace Rendering
{
	namespace Objects
	{
		VertexArrayObject::VertexArrayObject()
			: m_vertexArray(0)
		{
			//
		}

		void VertexArrayObject::CreateVertexArray()
		{
			glGenVertexArrays(1, &m_vertexArray);
			Bind();
		}

		void VertexArrayObject::CreateVertexBufferObject(unsigned int location, unsigned int count)
		{
			Bind();
			if (m_vertexBufferObjects.find(location) == m_vertexBufferObjects.end())
			{
				unsigned int vertex_buffer_object;
				glGenBuffers(1, &vertex_buffer_object);
				glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);

				glEnableVertexAttribArray(location);
				glVertexAttribPointer(location, count, GL_FLOAT, GL_FALSE, sizeof(float) * count, 0);

				m_vertexBufferObjects.insert({location, vertex_buffer_object });
			}
			else {
				std::cout << "[ERR] location " << location << " is already occupied in this VAO!\n";
			}
		}

		void VertexArrayObject::VertexBufferObjectData(unsigned int location, unsigned int size, float data[], GLenum usage)
		{
			Bind();
			if (m_vertexBufferObjects.find(location) == m_vertexBufferObjects.end())
			{
				std::cout << "[ERR] location " << location << " has not been created in this VAO!\n";
			}
			else {
				glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjects.at(location));
				glBufferData(GL_ARRAY_BUFFER, size, data, usage);
			}
		}

		VertexArrayObject::~VertexArrayObject()
		{
			//
		}
	}
}