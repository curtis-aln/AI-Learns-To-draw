#include <iostream>
#include <SFML/Graphics.hpp>

#include "canvas.h"
#include "image_compiler.h"


#include <vector>
#include <string>
#include <iomanip>
#include <cmath>  // For std::round

struct SFML_Renderer
{
	static void Render(std::vector<sf::Image>& images, const size_t frames_per_second)
	{
		sf::RenderWindow window(sf::VideoMode(PixelMap::resolution_x, PixelMap::resolution_y), "window");

		window.setFramerateLimit(frames_per_second);

		for (int i = 0; i < images.size(); ++i)
		{
			sf::Texture texture;
			texture.loadFromImage(images[i]);
			sf::Sprite sprite(texture);

			// Draw the frame
			window.clear();
			window.draw(sprite);
			window.display();
		}
	}
};


int main()
{
	Random::set_seed(0);

	// mutaiton paramaters
	const float mutation_rate = 0.08f;
	const float mutation_range = 0.08f;
	const float creation_rate = 0.08f;
	const float destruction_rate = 0.07f;

	Canvas canvas;
	std::vector<Canvas> canvases;

	const std::string save_file = "video.mp4";
	const size_t video_length_seconds = 10;
	const size_t frames_per_second = 60;
	const size_t total_frames = video_length_seconds * frames_per_second;

	canvases.resize(total_frames);

	for (int frame = 0; frame < total_frames; ++frame)
	{
		// mutating
		canvas.mutate_canvas(mutation_rate, mutation_range, creation_rate, destruction_rate);

		// saving the frame
		canvases[frame] = canvas;
	}

	std::cout << "Compiling. . .\n";

	std::vector<sf::Image> images = ImageCompiler::compile(canvases);

	std::cout << "Rendering. . .\n";
	SFML_Renderer::Render(images, frames_per_second);
}

/* Plan and Todo
idea:
All training information will be on text and a video will be generated of the training process

canvas parameters:
- Consists of 3 point Triangles at any XY position for each vertex. 
  each triangle has a color (potential: each vertex has a color)
- A canvas can have as many triangles as it wants


TODO:
[DONE] Create a canvas of random triangles
Create a video of random triangles mutating
Load Images and convert to suitable format
create population and apply natural selection


0.4s/10%
*/