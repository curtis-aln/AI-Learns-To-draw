#include <iostream>
#include <SFML/Graphics.hpp>

#include "canvas.h"
#include "image_compiler.h"
#include "renderer.h"
#include "settings.h"


#include <vector>
#include <string>
#include <iomanip>
#include <cmath>  // For std::round


int main()
{
	Canvas canvas;
	std::vector<Canvas> canvases;

	canvases.resize(VideoSettings::total_frames);

	for (int frame = 0; frame < VideoSettings::total_frames; ++frame)
	{
		// mutating
		canvas.mutate_canvas(
			MutationSettings::mutation_rate,
			MutationSettings::mutation_range, 
			MutationSettings::creation_rate, 
			MutationSettings::destruction_rate);

		// saving the frame
		canvases[frame] = canvas;
	}

	std::cout << "Compiling. . .\n";

	std::vector<sf::Image> images = ImageCompiler::compile(canvases);

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
Create a video of random triangles mutating
Load Images and convert to suitable format
create population and apply natural selection


0.4s/10%
*/