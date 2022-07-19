#include <iostream>
#include <thread>
#include <future>

#include <SDL.h>

#include "Complex.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

void render_julia(int** pixels, int iterations, int step, double a, double b, bool* terminate_render);

Complex screen_to_view(double x, double y, double xmin, double xmax, double ymin, double ymax);

int main(int argc, char** argv)
{
	int result = SDL_Init(SDL_INIT_VIDEO);

	if (result != 0)
	{
		std::cout << "Failed to initialize SDL with video: " << SDL_GetError() << "\n";

		return 1;
	}

	SDL_Window* window;
	SDL_Renderer* renderer;

	result = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, NULL, &window, &renderer);

	if (result != 0)
	{
		std::cout << "Failed to create window and renderer: " << SDL_GetError() << "\n";

		return 1;
	}

	int** pixels = (int**)calloc(SCREEN_HEIGHT, sizeof(int*));

	if (!pixels)
	{
		std::cout << "Failed to initialize pixel array.\n";

		return 1;
	}

	for (int i = 0; i < SCREEN_HEIGHT; i++)
	{
		pixels[i] = (int*)calloc(SCREEN_WIDTH, sizeof(int));

		if (!pixels[i])
		{
			std::cout << "Failed to initialize pixel array.\n";

			return 1;
		}
	}

	std::thread render_thread;

	bool terminate_render = false;
	bool running = true;

	SDL_Event event;

	double a = 0;
	double b = 0;

	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{

			case SDL_QUIT:
				running = false;

				break;

			case SDL_KEYDOWN:
			{
				double delta = 0.01;

				if (event.key.keysym.mod & KMOD_SHIFT)
				{
					delta *= 10;
				}

				switch (event.key.keysym.sym)
				{

				case SDLK_RIGHT:
					a += delta;

					break;

				case SDLK_LEFT:
					a -= delta;

					break;

				case SDLK_UP:
					b += delta;

					break;

				case SDLK_DOWN:
					b -= delta;

					break;

				}

				if (render_thread.joinable())
				{
					terminate_render = true;

					render_thread.join();

					terminate_render = false;
				}

				render_thread = std::thread(render_julia, std::ref(pixels), 50, 1, a, b, &terminate_render);

				break;
			}

			}
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		for (int i = 0; i < SCREEN_HEIGHT; i++)
		{
			for (int j = 0; j < SCREEN_WIDTH; j++)
			{
				int c = 255 * (1 - (float)pixels[i][j] / 50);

				SDL_SetRenderDrawColor(renderer, c, c, c, 255);
				SDL_RenderDrawPoint(renderer, j, i);
			}
		}

		SDL_RenderPresent(renderer);
	}

	free(pixels);

	SDL_Quit();

	return 0;
}

void render_julia(int** pixels, int iterations, int step, double a, double b, bool* terminate_render)
{
	for (double py = 0; py < SCREEN_HEIGHT; py += step)
	{
		for (double px = 0; px < SCREEN_WIDTH; px += step)
		{
			if (*terminate_render)
			{
				return;
			}

			Complex x = screen_to_view(px, py, -2, 2, -2, 2);

			int i = 0;

			for (i = 0; i < iterations; i++)
			{
				x.pow(2);
				x.add(a, b);

				if (x.sqrmagnitude() > 4)
				{
					break;
				}
			}

			for (int j = py; j < py + step; j++)
			{
				for (int k = px; k < px + step; k++)
				{
					pixels[j][k] = i;
				}
			}
		}
	}
}

Complex screen_to_view(double x, double y, double xmin, double xmax, double ymin, double ymax)
{
	return { xmin + (x / SCREEN_WIDTH) * (xmax - xmin), ymin + (y / SCREEN_HEIGHT) * (ymax - ymin) };
}