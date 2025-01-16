#include <iostream>
#include <SFML/Graphics.hpp>

#include "canvas.h"
#include "image_compiler.h"
#include "renderer.h"
#include "settings.h"
#include "evolver.h"


#include <vector>
#include <string>
#include <iomanip>
#include <cmath>  // For std::round



int main()
{
	Random::set_seed(0);

	Evolver<EvolutionSettings::population_size, EvolutionSettings::generations> evolver;
	
	const sf::Vector2u size = evolver.reference_image.getSize();
	sf::RenderWindow window(sf::VideoMode(size.x, size.y), "window"); //todo noframe

	std::cout << "Evolving canvases. . .\n";
	evolver.evolve(true);

	std::cout << "Compiling. . .\n";
	std::vector<Canvas> canvases = evolver.best_canvases_history;

	std::vector<sf::Image> images;
	images.resize(canvases.size());
	ImageCompiler::compile_all(canvases, images, evolver.thread_pool);

	std::cout << "Saving\n";
	if (!images[images.size()-1].saveToFile("image.png"))
	{
		throw std::runtime_error("Failed to save image to file: ");
	}
	std::cout << "Saved\n";


	std::cout << "Rendering. . .\n";
	while (true)
	{
		bool quit = SFML_Renderer::Render(images, VideoSettings::frames_per_second, window);
		if (quit)
		{
			break;
		}
	}
}

/* Plan and Todo
idea:
All training information will be on text and a video will be generated of the training process

canvas parameters:
- Consists of 3 point Triangles at any XY position for each vertex. 
  each triangle has a color (potential: each vertex has a color)
- A canvas can have as many triangles as it wants


------ TODO ------
[Optimization]
- learn if there are any alternatives for multithreading the image compiler renderTextures

[Functionality]

[Improvements]
- triangles spawn mostly transparent and spawn within a smaller rect instead of taking up the whole screen

[Bug Fixes]
*/