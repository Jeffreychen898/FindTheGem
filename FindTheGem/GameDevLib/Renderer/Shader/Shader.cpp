#include "Shader.h"

namespace Rendering
{
	Shader::Shader()
		: m_program(0)
	{
	}

	void Shader::CreateFromFile(const char* vertexPath, const char* fragmentPath, unsigned int textureSlotCount)
	{
		std::string vertex_code = ReadFile(vertexPath);
		std::string fragment_code = ReadFile(fragmentPath);
		CreateFromString(vertex_code.c_str(), fragment_code.c_str());
		if (textureSlotCount > 1 && textureSlotCount < 33)
		{
			int texture_count[32];
			for (unsigned int i = 0; i < textureSlotCount; ++i)
				texture_count[i] = i;

			unsigned int uniform_location = glGetUniformLocation(m_program, "u_sampler");
			glUniform1iv(uniform_location, textureSlotCount, texture_count);
		}
	}

	void Shader::CreateFromString(const char* vertexSource, const char* fragmentSource)
	{
		m_program = CreateShader(vertexSource, fragmentSource);
		Bind();
	}

	std::string Shader::ReadFile(const char* path)
	{
		std::ifstream file(path);
		std::string content = "";
		if (file.is_open())
		{
			std::string line;
			while (std::getline(file, line))
			{
				content += line + "\n";
			}
		}
		else {
			std::cout << "[ERR] Fail to open " << path << "!\n";
		}

		return content;
	}

	unsigned int Shader::CreateShader(const char* vertexShader, const char* fragmentShader)
	{
		unsigned int program = glCreateProgram();
		unsigned int vertex = CompileShader(GL_VERTEX_SHADER, vertexShader);
		unsigned int fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

		glAttachShader(program, vertex);
		glAttachShader(program, fragment);
		glLinkProgram(program);
		glValidateProgram(program);

		return program;
	}

	unsigned int Shader::CompileShader(unsigned int type, const char* src)
	{
		unsigned int shader = glCreateShader(type);
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);

		int result;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)malloc(length * sizeof(char));
			glGetShaderInfoLog(shader, length, &length, message);
			std::cout << "[ERR] Failed To Compile " << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " Shader!" << std::endl;
			std::cout << message << std::endl;
			glDeleteShader(shader);
			return 0;
		}

		return shader;
	}
}