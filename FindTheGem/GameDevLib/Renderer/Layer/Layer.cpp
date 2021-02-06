#include "Layer.h"

namespace Rendering
{
	namespace Objects
	{
		Layer::Layer()
			: m_lastUse(0), m_cameraLocation(nullptr)
		{
			//
		}

		void Layer::CreateLayer(unsigned int width, unsigned int height, GameLibMath::Matrix4* cameraPtr)
		{
			m_cameraLocation = cameraPtr;
			m_frameBuffer.CreateFrameBuffer({ width, height });
		}

		void Layer::Clear(unsigned int& frameCount)
		{
			if (frameCount > m_lastUse)
			{
				m_frameBuffer.BindFrameBuffer();
				glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
				glClearColor(0, 0, 0, 0);
				glClear(GL_COLOR_BUFFER_BIT);
			}
		}

		Layer::~Layer()
		{
			//
		}
	}
}