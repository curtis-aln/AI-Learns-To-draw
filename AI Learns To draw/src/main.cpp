#include <iostream>
#include <SFML/Graphics.hpp>

#include "canvas.h"
#include "image_compiler.h"

int main()
{
	// mutaiton paramaters
	const float mutation_rate = 0.04f;
	const float mutation_range = 0.04f;
	const float creation_rate = 0.8f;
	const float destruction_rate = 0.75f;

	Canvas canvas;
	std::vector<sf::Image> images;

	const std::string save_file = "video.mp4";
	const size_t video_length_seconds = 1;
	const size_t frames_per_second = 1;
	const size_t total_frames = video_length_seconds * frames_per_second;

	images.resize(total_frames);

	for (int frame = 0; frame < total_frames; ++frame)
	{
		// mutating
		canvas.mutate_canvas(mutation_rate, mutation_range, creation_rate, destruction_rate);

		// saving the frame
		images[frame] = convert_to_SFML_image(ImageCompiler::compile(canvas));
		save_pixel_map_as_image(ImageCompiler::compile(canvas), "image.png");
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
Create a video of random triangles mutating
*/