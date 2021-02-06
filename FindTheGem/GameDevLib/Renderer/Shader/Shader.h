#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#include <GL/glew.h>

namespace Rendering {
	class Shader
	{
	private:
		unsigned int m_program;
	public:
		Shader();

		void CreateFromFile(const char* vertexPath, const char* fragmentPath, unsigned int textureSlotCount = 1);
		void CreateFromString(const char* vertexSource, const char* fragmentSource);
		void Bind() const { glUseProgram(m_program); };

		unsigned int GetShaderProgram() const { return m_program; };
	private:
		unsigned int CreateShader(const char* vertexShader, const char* fragmentShader);
		unsigned int CompileShader(unsigned int type, const char* src);

		std::string ReadFile(const char* path);
	};
}