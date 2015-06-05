#include <atomic>

#include "Application.hpp"

#include <GLFW/glfw3.h>

std::atomic<uint32_t> width(1280);
std::atomic<uint32_t> height(720);

static noob::app* app;

void window_close_callback(GLFWwindow* window)
{
}

void window_size_callback(GLFWwindow* window, int w, int h)
{
	width = std::abs(w);
	height = std::abs(h);
	app->window_resize(width, height);
}

void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
	width = std::abs(w);
	height = std::abs(h);
	app->window_resize(width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}

int main(int /*_argc*/, char** /*_argv*/)
{
	GLFWwindow* window;

	if (!glfwInit())
	{
		return -1;
	}
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "Repro", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	app = new noob::app();

	if (!app)
	{
		std::cerr << "Could not init user app";
	}

	glfwSetWindowCloseCallback(window, window_close_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);

	app->init();

	while (!glfwWindowShouldClose(window))
	{
		app->step();
		glfwPollEvents();
	}

	delete app;
	return 0;
}
