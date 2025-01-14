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
	std::cout << "Evolving canvases. . .\n";
	Evolver<EvolutionSettings::population_size, EvolutionSettings::generations> evolver;
	evolver.evolve(true);

	std::cout << "Compiling. . .\n";
	std::vector<Canvas> canvases = evolver.best_canvases_history;

	std::vector<sf::Image> images;
	images.resize(canvases.size());
	ImageCompiler::compile_all(canvases, images);

	std::cout << "Saving\n";
	if (!images[images.size()-1].saveToFile("image.png"))
	{
		throw std::runtime_error("Failed to save image to file: ");
	}
	std::cout << "Saved\n";


	std::cout << "Rendering. . .\n";
	while (true)
	{
		bool quit = SFML_Renderer::Render(images, VideoSettings::frames_per_second);
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


TODO:
[DONE] Create a canvas of random triangles
[DONE] Create a video of random triangles mutating
[DONE] Load Images and convert to suitable format
[DONE] create population and apply natural selection
End image saves
More intuitive training info
improved genetic algorithm 
fix mutation settings

13s
9s



Total time spent on compile_images: 9941.88 ms
Total time spent on calculate_scores: 2687.7 ms
Total time spent on get_best_canvas: 0.2171 ms
Total time spent on create_next_gen: 3.4673 ms
Average time per generation: 631.663 ms
Average time per generation: 687.08 ms
*/