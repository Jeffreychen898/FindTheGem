#pragma once

#include <iostream>
#include <vector>

#include <GL/glew.h>

#include "../Shader/Shader.h"
#include "../GLBufferedObjects/VertexArrayObject/VertexArrayObject.h"
#include "../GLBufferedObjects/IndexBufferObject/IndexBufferObject.h"
#include "../GLBufferedObjects/FrameBufferObject/FrameBufferObject.h"
#include "../GLBufferedObjects/UniformBufferObject/UniformBufferObject.h"
#include "../Layer/Layer.h"
#include "../../Math/Math.h"
#include "../Texture/Texture.h"
#include "../Font/Font.h"
#include "../Camera/Camera.h"

namespace Rendering
{
	const unsigned int LayerRenderingOverlap = 0;
	const unsigned int LayerRenderingMultiply = 1;
	const unsigned int LayerRenderingAdditive = 2;

	struct Layers
	{
		Rendering::Shader shader;
		std::vector<unsigned int> layers;
	};

	struct RGBA
	{
		float red;
		float green;
		float blue;
		float alpha = 255.f;
	};

	struct Positions
	{
		float x;
		float y;
		float z = 0.f;
	};

	struct TexCoords
	{
		float u;
		float v;
	};

	struct PolygonVertices
	{
		Positions position;
		RGBA color;
		TexCoords texCoord;
	};

	class Renderer
	{
	private:
		/* max vertex per batch constants */
		const unsigned int m_MAX_VERTEX_COUNT = 10000;
		const unsigned int m_MAX_INDEX_COUNT = 10000;

		/* attribute per vertex */
		float* m_positions = new float[3 * 10000];
		unsigned int m_positionCounter;

		float* m_texCoords = new float[2 * 10000];
		unsigned int m_texCoordCounter;

		float* m_colors = new float[4 * 10000];
		unsigned int m_colorCounter;

		unsigned int* m_indices = new unsigned int[10000];
		unsigned int m_indicesCounter;

		/* shaders */

		Rendering::Shader m_defaultShader;
		Rendering::Shader m_glowShader;
		Rendering::Shader m_textShader;

		unsigned int m_currentShader;

		/* attribute values */
		Rendering::RGBA m_selectedColors;

		/* window information */
		unsigned int m_frameCounter;
		unsigned int m_windowWidth, m_windowHeight;

		/* GL buffered objects */
		Rendering::Objects::VertexArrayObject m_vao;
		Rendering::Objects::IndexBufferObject m_ibo;
		Rendering::Objects::UniformBufferObject m_ubo;

		std::vector<Rendering::Objects::Layer> m_layers;
		unsigned int m_currentLayer;
		GameLibMath::Matrix4* m_currentUsedCameraMatrix;

		/* textures */
		unsigned int m_currentTexture;

		Texture m_whiteTexture;

		/* font loading */

		FT_Library m_freetypeLibrary;
		bool m_freetypeLibTerminated;

		/* drawing polygon */
		std::vector<PolygonVertices> m_polygonVertices;
		Texture* m_polygonTexture;
	public:
		Renderer();
		~Renderer();

		void Init(unsigned int width, unsigned int height);
		void StartOfFrame();

		Rendering::Font LoadFont(const char* path);
		void FinishedFontLoading() { FT_Done_FreeType(m_freetypeLibrary); m_freetypeLibTerminated = true; };

		void SetColor(float red, float green, float blue, float alpha = 255);
		void SetColor(float brightness, float alpha = 255) { SetColor(brightness, brightness, brightness, alpha); };

		/* Below are the rendering methods*/
		void DrawText(Rendering::Font& font, std::string text, float x, float y, float size);
		void DrawRect(float x, float y, float width, float height);
		void DrawImage(Rendering::Texture& texture, float x, float y, float width, float height);
		void EmitGlow(float x, float y, float size);

		void BeginPolygon() { BeginPolygon(m_whiteTexture); };
		void BeginPolygon(Rendering::Texture& texture);
		void PolygonVertex(Positions position) { PolygonVertex(position, m_selectedColors, { 0.f, 0.f }); };
		void PolygonVertex(Positions position, RGBA color) { PolygonVertex(position, color, { 0.f, 0.f }); };
		void PolygonVertex(Positions position, RGBA color, TexCoords texCoord);
		void EndPolygon();
		/* Above are the rendering methods */

		void CreateLayer(unsigned int& layer, GameCamera* camera);
		void UseLayer(unsigned int& layer);
		void RenderLayers(std::vector<Layers>& layers);

		Layers SingleLayer(unsigned int layer) const { return { m_defaultShader, {layer} }; };
		Layers SingleLayer(Rendering::Shader shader, unsigned int layer) const { return { shader, {layer} }; };
	private:
		void MakeDrawCall();

		void RenderRectangle(float x, float y, float width, float height);
		void RenderLayerRectangle(float x, float y, float width, float height);

		void OpenGLInitialization();
	};
}