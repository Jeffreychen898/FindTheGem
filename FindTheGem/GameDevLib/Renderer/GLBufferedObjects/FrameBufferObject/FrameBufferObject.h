#pragma once

#include <iostream>

#include <GL/glew.h>

namespace Rendering
{
	namespace Objects
	{
		struct FrameBufferData
		{
			unsigned int width;
			unsigned int height;
		};

		class FrameBufferObject
		{
		private:
			unsigned int m_frameBuffer;
			unsigned int m_frameBufferTexture;
		public:
			FrameBufferObject();
			~FrameBufferObject();

			void CreateFrameBuffer(FrameBufferData frameBufferData);

			void BindFrameBuffer() const { glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer); };
			void BindTexture() const { glBindTexture(GL_TEXTURE_2D, m_frameBufferTexture); };

			unsigned int GetTexture() const { return m_frameBufferTexture; };
		};
	}
}