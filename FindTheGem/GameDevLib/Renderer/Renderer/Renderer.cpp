#include "Renderer.h"

namespace Rendering
{
	Renderer::Renderer()
		: m_positionCounter(0), m_indicesCounter(0), m_texCoordCounter(0), m_frameCounter(0), m_currentLayer(0), m_windowWidth(0),
		m_windowHeight(0), m_currentUsedCameraMatrix(nullptr), m_colorCounter(0), m_currentTexture(0), m_currentShader(0),
		m_polygonTexture(nullptr), m_freetypeLibTerminated(false)
	{
		m_selectedColors = { 255, 255, 255, 255 };
		FT_Init_FreeType(&m_freetypeLibrary);
	}

	void Renderer::OpenGLInitialization()
	{
		glewInit();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Renderer::Init(unsigned int width, unsigned int height)
	{
		OpenGLInitialization();

		m_windowWidth = width;
		m_windowHeight = height;
		/* shaders */
		m_glowShader.CreateFromFile("GameDevLib/resource/shaders/glow/glow.vert", "GameDevLib/resource/shaders/glow/glow.frag");
		m_textShader.CreateFromFile("GameDevLib/resource/shaders/text/text.vert", "GameDevLib/resource/shaders/text/text.frag");
		m_defaultShader.CreateFromFile("GameDevLib/resource/shaders/default/default.vert", "GameDevLib/resource/shaders/default/default.frag");
		m_currentShader = m_defaultShader.GetShaderProgram();

		/* ubo */
		m_ubo.CreateUniformObject(64, 0);

		/* vao */
		m_vao.CreateVertexArray();
		m_vao.CreateVertexBufferObject(0, 3);
		m_vao.CreateVertexBufferObject(1, 2);
		m_vao.CreateVertexBufferObject(2, 4);

		/* ibo */
		m_ibo.CreateIndexBufferObject();

		/* stbi flip the image vertically */
		stbi_set_flip_vertically_on_load(1);

		/* create a white texture */
		m_whiteTexture.LoadTexture();
		m_currentTexture = m_whiteTexture.GetTexture();
	}

	void Renderer::StartOfFrame()
	{
		++m_frameCounter;
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::SetColor(float red, float green, float blue, float alpha) {
		m_selectedColors.red = red / 255.f;
		m_selectedColors.green = green / 255.f;
		m_selectedColors.blue = blue / 255.f;
		m_selectedColors.alpha = alpha / 255.f;
	}

	Rendering::Font Renderer::LoadFont(const char* path)
	{
		Rendering::Font newFont(m_freetypeLibrary, path);

		return newFont;
	}

	void Renderer::DrawText(Rendering::Font& font, std::string text, float x, float y, float size)
	{
		/* check if the texture atlas is bounded */
		if (m_currentTexture != font.GetTextureAtlas())
		{
			MakeDrawCall();
			m_currentTexture = font.GetTextureAtlas();
			font.BindTextureAtlas();
		}

		/* make sure the shader is attached and/or there are enough room for more rendering */
		if (m_indicesCounter + 6 > m_MAX_INDEX_COUNT || m_currentShader != m_textShader.GetShaderProgram())
		{
			MakeDrawCall();
			if (m_currentShader != m_textShader.GetShaderProgram())
			{
				m_textShader.Bind();
				m_currentShader = m_textShader.GetShaderProgram();
			}
		}

		const float scale = size / (float)font.GetFontSize();
		float starting_x = x;
		float starting_y = y;

		/* loop through each character of the string and render the letters */
		for (const char c : text)
		{
			Rendering::Character character = font.GetCharacter(c);

			/* the position of the letter */
			float x = starting_x + character.left * scale;
			float y = starting_y - character.top * scale;
			float width = character.width * scale;
			float height = character.height * scale;
			float position[] = {
				 x,  y, 0.0f,//top left
				 x + width,  y, 0.0f,//top right
				 x + width,  y + height, 0.0f,//bottom right
				 x,  y + height, 0.0f//bottom left
			};

			starting_x += character.advance * scale;

			/* write out the indices */
			unsigned int starting_point = m_positionCounter / 3;
			unsigned int indices[] = {
				starting_point, starting_point + 1, starting_point + 2,
				starting_point, starting_point + 3, starting_point + 2
			};

			/* look up the character in the atlas use the data to form the texture coordinates */
			float texcoord_left = (character.stride + 4.f) / font.GetTextureAtlasWidth();
			float texcoord_top = 4.f / font.GetTextureAtlasHeight();
			float texcoord_width = character.width / font.GetTextureAtlasWidth();
			float texcoord_height = character.height / font.GetTextureAtlasHeight();

			float tex_coords[] = {
				texcoord_left, texcoord_top,
				texcoord_left + texcoord_width, texcoord_top,
				texcoord_left + texcoord_width, texcoord_top + texcoord_height,
				texcoord_left, texcoord_top + texcoord_height
			};

			float colors[4] = { m_selectedColors.red, m_selectedColors.green, m_selectedColors.blue, m_selectedColors.alpha };

			/* inserting the data in the attribute variables */
			for (unsigned int i = 0; i < 12; ++i)
				m_positions[m_positionCounter + i] = position[i];

			for (unsigned int i = 0; i < 6; ++i)
				m_indices[m_indicesCounter + i] = indices[i];

			for (unsigned int i = 0; i < 8; ++i)
				m_texCoords[m_texCoordCounter + i] = tex_coords[i];

			for (unsigned int i = 0; i < 16; ++i)
				m_colors[m_colorCounter + i] = colors[i % 4];

			m_positionCounter += 12;
			m_indicesCounter += 6;
			m_texCoordCounter += 8;
			m_colorCounter += 16;
		}
	}

	void Renderer::DrawImage(Rendering::Texture& texture, float x, float y, float width, float height)
	{
		if (m_currentTexture != texture.GetTexture())
		{
			MakeDrawCall();
			m_currentTexture = texture.GetTexture();
			texture.Bind();
		}

		std::pair<float, float> first_position = texture.GetFirstUVPosition();
		std::pair<float, float> second_position = texture.GetSecondUVPosition();

		float uv_top = first_position.second;
		float uv_bottom = second_position.second;
		float uv_left = first_position.first;
		float uv_right = second_position.first;

		float tex_coords[] = {
			uv_left, uv_top,
			uv_right, uv_top,
			uv_right, uv_bottom,
			uv_left, uv_bottom
		};

		for (unsigned int i = 0; i < 8; ++i)
			m_texCoords[m_texCoordCounter + i] = tex_coords[i];

		RenderRectangle(x, y, width, height);
	}

	void Renderer::DrawRect(float x, float y, float width, float height)
	{
		if (m_currentTexture != m_whiteTexture.GetTexture())
		{
			MakeDrawCall();
			m_currentTexture = m_whiteTexture.GetTexture();
			m_whiteTexture.Bind();
		}

		float tex_coords[] = {
			0.f, 1.f,
			1.f, 1.f,
			1.f, 0.f,
			0.f, 0.f
		};

		for (unsigned int i = 0; i < 8; ++i)
			m_texCoords[m_texCoordCounter + i] = tex_coords[i];

		RenderRectangle(x, y, width, height);
	}

	void Renderer::RenderRectangle(float x, float y, float width, float height)
	{
		if (m_indicesCounter + 6 > m_MAX_INDEX_COUNT || m_currentShader != m_defaultShader.GetShaderProgram())
		{
			MakeDrawCall();
			if (m_currentShader != m_defaultShader.GetShaderProgram())
			{
				m_defaultShader.Bind();
				m_currentShader = m_defaultShader.GetShaderProgram();
			}
		}

		/* data */
		float position[] = {
			 x,  y, 0.0f,//top left
			 x + width,  y, 0.0f,//top right
			 x + width,  y + height, 0.0f,//bottom right
			 x,  y + height, 0.0f//bottom left
		};
		unsigned int starting_point = m_positionCounter / 3;
		unsigned int indices[] = {
			starting_point, starting_point + 1, starting_point + 2,
			starting_point, starting_point + 3, starting_point + 2
		};

		float colors[4] = { m_selectedColors.red, m_selectedColors.green, m_selectedColors.blue, m_selectedColors.alpha };

		for (unsigned int i = 0; i < 12; ++i)
			m_positions[m_positionCounter + i] = position[i];

		for (unsigned int i = 0; i < 6; ++i)
			m_indices[m_indicesCounter + i] = indices[i];

		for (unsigned int i = 0; i < 16; ++i)
			m_colors[m_colorCounter + i] = colors[i % 4];

		m_positionCounter += 12;
		m_indicesCounter += 6;
		m_texCoordCounter += 8;
		m_colorCounter += 16;
	}

	void Renderer::BeginPolygon(Rendering::Texture& texture)
	{
		if (m_currentShader != m_defaultShader.GetShaderProgram())
		{
			MakeDrawCall();
			m_defaultShader.Bind();
			m_currentShader = m_defaultShader.GetShaderProgram();
			m_polygonTexture = &m_whiteTexture;
		}

		if (m_currentTexture != texture.GetTexture())
		{
			MakeDrawCall();
			texture.Bind();
			m_currentTexture = texture.GetTexture();
			m_polygonTexture = &texture;
		}
	}

	void Renderer::PolygonVertex(Positions position, RGBA color, TexCoords texCoord)
	{
		RGBA clipped_color = { color.red / 255.f, color.green / 255.f, color.blue / 255.f, color.alpha / 255.f };
		float texcoord_u = texCoord.u / (float)m_polygonTexture->GetTextureWidth();
		float texcoord_v = 1.f - texCoord.v / (float)m_polygonTexture->GetTextureHeight();
		TexCoords clipped_texcoord = { texcoord_u, texcoord_v };
		m_polygonVertices.push_back({ position, clipped_color, clipped_texcoord });
	}

	void Renderer::EndPolygon()
	{
		if (m_polygonVertices.size() > 2)
		{
			if (m_indicesCounter + m_polygonVertices.size() > m_MAX_INDEX_COUNT)
				MakeDrawCall();

			/* render the polygon */
			for (unsigned int i = 2; i < m_polygonVertices.size(); ++i)
			{
				unsigned int starting_point = m_positionCounter / 3;
				m_indices[m_indicesCounter] = starting_point;
				m_indices[m_indicesCounter + 1] = starting_point + i - 1;
				m_indices[m_indicesCounter + 2] = starting_point + i;

				m_indicesCounter += 3;
			}

			for (unsigned int i = 0; i < m_polygonVertices.size(); ++i)
			{
				Rendering::PolygonVertices current_vertex = m_polygonVertices[i];

				m_positions[m_positionCounter] = current_vertex.position.x;
				m_positions[m_positionCounter + 1] = current_vertex.position.y;
				m_positions[m_positionCounter + 2] = current_vertex.position.z;

				m_texCoords[m_texCoordCounter] = current_vertex.texCoord.u;
				m_texCoords[m_texCoordCounter + 1] = current_vertex.texCoord.v;

				m_colors[m_colorCounter] = current_vertex.color.red;
				m_colors[m_colorCounter + 1] = current_vertex.color.green;
				m_colors[m_colorCounter + 2] = current_vertex.color.blue;
				m_colors[m_colorCounter + 3] = current_vertex.color.alpha;

				m_positionCounter += 3;
				m_texCoordCounter += 2;
				m_colorCounter += 4;
			}
		}
		m_polygonVertices.clear();
	}

	void Renderer::EmitGlow(float x, float y, float size)
	{
		if (m_indicesCounter + 6 > m_MAX_INDEX_COUNT || m_currentShader != m_glowShader.GetShaderProgram())
		{
			MakeDrawCall();
			if (m_currentShader != m_glowShader.GetShaderProgram())
			{
				m_glowShader.Bind();
				m_currentShader = m_glowShader.GetShaderProgram();
			}
		}

		/* data */
		float position[] = {
			 -1.f,  1.f, size,//top left
			  1.f,  1.f, size,//top right
			  1.f, -1.f, size,//bottom right
			 -1.f, -1.f, size//bottom left
		};
		unsigned int starting_point = m_positionCounter / 3;
		unsigned int indices[] = {
			starting_point, starting_point + 1, starting_point + 2,
			starting_point, starting_point + 3, starting_point + 2
		};

		/* using the texture coordinate attributes to hold the positions of the glow */
		GameLibMath::Vector4 glow_position((float)x, (float)y, 0.f, 1.f);
		GameLibMath::Matrix4 projection_matrix = *(m_layers[m_currentLayer].GetCameraPtr());
		glow_position.Multiply(projection_matrix);
		glow_position.GetVector();
		GameLibMath::Vector4Data glow_position_data = glow_position.GetVector();
		++glow_position_data.x;
		++glow_position_data.y;

		glow_position_data.x *= m_windowWidth / 2;
		glow_position_data.y *= m_windowHeight / 2;

		float tex_coords[] = {
			glow_position_data.x, glow_position_data.y,
			glow_position_data.x, glow_position_data.y,
			glow_position_data.x, glow_position_data.y,
			glow_position_data.x, glow_position_data.y
		};

		float colors[4] = { m_selectedColors.red, m_selectedColors.green, m_selectedColors.blue, m_selectedColors.alpha };

		for (unsigned int i = 0; i < 12; ++i)
			m_positions[m_positionCounter + i] = position[i];

		for (unsigned int i = 0; i < 6; ++i)
			m_indices[m_indicesCounter + i] = indices[i];

		for (unsigned int i = 0; i < 8; ++i)
			m_texCoords[m_texCoordCounter + i] = tex_coords[i];

		for (unsigned int i = 0; i < 16; ++i)
			m_colors[m_colorCounter + i] = colors[i % 4];

		m_positionCounter += 12;
		m_indicesCounter += 6;
		m_texCoordCounter += 8;
		m_colorCounter += 16;
	}

	void Renderer::MakeDrawCall()
	{
		if (m_indicesCounter > 0)
		{
			m_vao.VertexBufferObjectData(0, sizeof(float) * 3 * m_MAX_VERTEX_COUNT, m_positions, GL_DYNAMIC_DRAW);
			m_vao.VertexBufferObjectData(1, sizeof(float) * 2 * m_MAX_VERTEX_COUNT, m_texCoords, GL_DYNAMIC_DRAW);
			m_vao.VertexBufferObjectData(2, sizeof(float) * 4 * m_MAX_VERTEX_COUNT, m_colors, GL_DYNAMIC_DRAW);
			m_ibo.IndexBufferData(m_MAX_INDEX_COUNT * sizeof(unsigned int), m_indices, GL_DYNAMIC_DRAW);

			glDrawElements(GL_TRIANGLES, m_indicesCounter, GL_UNSIGNED_INT, nullptr);

			m_positionCounter = 0;
			m_indicesCounter = 0;
			m_texCoordCounter = 0;
			m_colorCounter = 0;
		}
	}

	void Renderer::CreateLayer(unsigned int& layer, GameCamera* camera)
	{
		m_currentLayer = layer;
		layer = m_layers.size();
		Rendering::Objects::Layer new_layer;
		new_layer.CreateLayer(m_windowWidth, m_windowHeight, camera);
		m_layers.push_back(new_layer);

		/* use the layer */
		m_currentLayer = layer;
		new_layer.BindLayer();
		new_layer.Clear(m_frameCounter);

		/* bind the camera matrix */
		m_currentUsedCameraMatrix = new_layer.GetCameraPtr();
		m_ubo.UniformObjectData(0, 64, camera->GetRawMatrixPtr());
	}

	void Renderer::UseLayer(unsigned int& layer)
	{
		if (layer != m_currentLayer)
		{
			MakeDrawCall();

			m_currentLayer = layer;
			Rendering::Objects::Layer current_layer = m_layers.at(layer);
			current_layer.BindLayer();
			current_layer.Clear(m_frameCounter);

			/* bind the camera matrix */
			if (m_currentUsedCameraMatrix != current_layer.GetCameraPtr())
			{
				m_currentUsedCameraMatrix = current_layer.GetCameraPtr();
				m_ubo.UniformObjectData(0, 64, current_layer.GetCameraPtr()->GetRawMatrixPtr());
			}
		}
	}

	void Renderer::RenderLayers(std::vector<Layers>& layers)
	{
		/*make sure all content is rendered before layers are rendered*/
		MakeDrawCall();
		/*bind an identity matrix*/
		m_currentUsedCameraMatrix = nullptr;
		float identity_matrix[16] = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
		m_ubo.UniformObjectData(0, 64, &identity_matrix[0]);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		SetColor(255.f);
		for (Layers layer : layers)
		{
			if (m_currentShader != layer.shader.GetShaderProgram())
			{
				MakeDrawCall();
				layer.shader.Bind();
				m_currentShader = layer.shader.GetShaderProgram();
			}

			RenderLayerRectangle(-1, 1, 2, -2);

			for (unsigned int i = 0; i < layer.layers.size(); ++i)
				glBindTextureUnit(i, m_layers.at(layer.layers.at(i)).GetTexture());
			MakeDrawCall();
		}
		m_currentTexture = m_whiteTexture.GetTexture();
		m_whiteTexture.Bind();
	}

	void Renderer::RenderLayerRectangle(float x, float y, float width, float height)
	{
		if (m_indicesCounter + 6 > m_MAX_VERTEX_COUNT)
			MakeDrawCall();

		/* data */
		float position[] = {
			 x,  y, 0.0f,//top left
			 x + width,  y, 0.0f,//top right
			 x + width,  y + height, 0.0f,//bottom right
			 x,  y + height, 0.0f//bottom left
		};
		unsigned int starting_point = m_positionCounter / 3;
		unsigned int indices[] = {
			starting_point, starting_point + 1, starting_point + 2,
			starting_point, starting_point + 3, starting_point + 2
		};

		float tex_coords[] = {
			0.f, 1.f,
			1.f, 1.f,
			1.f, 0.f,
			0.f, 0.f
		};

		float colors[4] = { m_selectedColors.red, m_selectedColors.green, m_selectedColors.blue, m_selectedColors.alpha };

		for (unsigned int i = 0; i < 12; ++i)
			m_positions[m_positionCounter + i] = position[i];

		for (unsigned int i = 0; i < 6; ++i)
			m_indices[m_indicesCounter + i] = indices[i];

		for (unsigned int i = 0; i < 8; ++i)
			m_texCoords[m_texCoordCounter + i] = tex_coords[i];

		for (unsigned int i = 0; i < 16; ++i)
			m_colors[m_colorCounter + i] = colors[i % 4];

		m_positionCounter += 12;
		m_indicesCounter += 6;
		m_texCoordCounter += 8;
		m_colorCounter += 16;
	}

	Renderer::~Renderer()
	{
		delete[] m_positions;
		delete[] m_texCoords;
		delete[] m_colors;
		delete[] m_indices;
		if (!m_freetypeLibTerminated)
			FinishedFontLoading();
	}
}