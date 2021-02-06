#include "Window.h"

namespace Rendering
{
	Window::Window(unsigned int width, unsigned int height, const char* title)
		: m_deltaTime(0), m_frameRate(0), m_renderer(nullptr)
	{
		m_width = width;
		m_height = height;
        glfwInit();
        m_window = glfwCreateWindow(width, height, title, NULL, NULL);
        glfwMakeContextCurrent(m_window);
		m_previousTime = std::chrono::high_resolution_clock::now();
	}

	void Window::StartOfFrame()
	{
		std::chrono::steady_clock::time_point current_time = std::chrono::high_resolution_clock::now();
		std::chrono::steady_clock::duration time_passed = current_time - m_previousTime;
		std::chrono::nanoseconds nanoseconds_passed = std::chrono::duration_cast<std::chrono::nanoseconds>(time_passed);
		m_deltaTime = nanoseconds_passed.count() / 1000000000.f;
		m_frameRate = 1.f / m_deltaTime;
		m_previousTime = current_time;

		m_renderer->StartOfFrame();
	}

	void Window::EndOfFrame()
	{
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	Window::~Window()
	{
		//
	}
}