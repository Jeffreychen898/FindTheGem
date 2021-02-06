#include "FrameBufferObject.h"

namespace Rendering
{
	namespace Objects
	{
		FrameBufferObject::FrameBufferObject()
			: m_frameBuffer(0), m_frameBufferTexture(0)
		{
		}

		void FrameBufferObject::CreateFrameBuffer(FrameBufferData frameBufferData)
		{
			glGenFramebuffers(1, &m_frameBuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

			glGenTextures(1, &m_frameBufferTexture);
			glBindTexture(GL_TEXTURE_2D, m_frameBufferTexture);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frameBufferData.width, frameBufferData.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_frameBufferTexture, 0);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "[ERR] framebuffer fail to be created!" << std::endl;
		}

		FrameBufferObject::~FrameBufferObject()
		{
			//
		}
	}
}