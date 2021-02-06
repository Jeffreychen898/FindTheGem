#pragma once

#include <GL/glew.h>

#include "../GLBufferedObjects/FrameBufferObject/FrameBufferObject.h"
#include "../../Math/Matrix4/Matrix4.h"

namespace Rendering
{
	namespace Objects
	{
		class Layer
		{
		private:
			unsigned int m_lastUse;
			Rendering::Objects::FrameBufferObject m_frameBuffer;
			GameLibMath::Matrix4* m_cameraLocation;
		public:
			Layer();
			~Layer();

			void CreateLayer(unsigned int width, unsigned int height, GameLibMath::Matrix4* cameraPtr);
			void Clear(unsigned int& frameCount);
			void BindLayer() { m_frameBuffer.BindFrameBuffer(); };
			unsigned int GetTexture() const { return m_frameBuffer.GetTexture(); };
			void BindTexture() const { m_frameBuffer.BindTexture(); };

			GameLibMath::Matrix4* GetCameraPtr() { return m_cameraLocation; };
		};
	}
}