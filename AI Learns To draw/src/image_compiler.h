#pragma once
#include "canvas.h"
#include <SFML/Graphics.hpp>


struct ImageCompiler
{
	static std::vector<sf::Image> compile(std::vector<Canvas>& canvases)
	{
		const size_t size = canvases.size();

		// Create an SFML RenderTexture
		sf::RenderTexture render_texture;
		if (!render_texture.create(PixelMap::resolution_x, PixelMap::resolution_y))
		{
			throw std::runtime_error("Failed to create render texture");
		}

		// Clear the render texture with a background color
		render_texture.clear(sf::Color::Black);

		std::vector<sf::Image> images;
		images.resize(size);
		
		for (int i = 0; i < size; ++i)
		{
			// Draw each triangle onto the render texture
			for (const Triangle& triangle : canvases[i].get_triangles())
			{
				render_texture.draw(triangle.vertex_array);
			}

			images[i] = render_texture.getTexture().copyToImage();

			if (i % int(size / 10) == 0)
			{
				std::cout << int((i * 100)/size) << "% compiled\n";
			}
		}

		return images;
	}

};


inline void save_canvas_as_image(Canvas& canvas, const std::string& filename)
{
	// Create an SFML Image
	//sf::Image image = ImageCompiler::compile(canvas);

	// Save the image to a file
	//if (!image.saveToFile(filename))
	//{
	//	throw std::runtime_error("Failed to save image to " + filename);
	//}
}