#pragma once

#include <chrono>

#include "../Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Rendering
{
	class Window
	{
	private:
		GLFWwindow* m_window;

		unsigned int m_width, m_height;

		std::chrono::high_resolution_clock::time_point m_previousTime;
		float m_deltaTime, m_frameRate;

		Rendering::Renderer* m_renderer;
	public:
		Window(unsigned int width, unsigned int height, const char* title);
		~Window();

		bool WindowShouldClose() const { return glfwWindowShouldClose(m_window); };
		void StartOfFrame();
		void EndOfFrame();
		void Terminate() { glfwTerminate(); };

		void AccessRenderer(Rendering::Renderer* renderer) { m_renderer = renderer; };

		float GetDeltaTime() const { return m_deltaTime; };
		float GetFrameRate() const { return m_frameRate; };
	};
}