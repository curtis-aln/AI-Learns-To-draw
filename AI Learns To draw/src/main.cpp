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
	Evolver evolver;
	
	evolver.evolve(EvolutionSettings::generations);

	std::cout << "Compiling. . .\n";

	std::vector<Canvas> canvases = evolver.best_canvases_history;
	std::vector<sf::Image> images = ImageCompiler::compile_all(canvases);

	std::cout << "Rendering. . .\n";
	SFML_Renderer::Render(images, VideoSettings::frames_per_second);
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

0.4s/10%
*/