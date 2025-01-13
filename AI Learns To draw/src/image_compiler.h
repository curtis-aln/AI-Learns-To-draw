#pragma once
#include "canvas.h"
#include <SFML/Graphics.hpp>


struct PixelMap
{
	static const size_t resolution_x = 800;
	static const size_t resolution_y = 800;

	std::vector<std::vector<Color>> map;
	
	PixelMap()
	{
		map.resize(resolution_x, std::vector<Color>(resolution_y));
	}
};

static struct ImageCompiler
{
	static PixelMap compile(Canvas& canvas)
	{
		// Create an SFML RenderTexture
		sf::RenderTexture render_texture;
		if (!render_texture.create(PixelMap::resolution_x, PixelMap::resolution_y))
		{
			throw std::runtime_error("Failed to create render texture");
		}

		// Clear the render texture with a background color
		render_texture.clear(sf::Color::Black);

		// Draw each triangle onto the render texture
		for (const Triangle& triangle : canvas.get_triangles())
		{
			draw_triangle(render_texture, triangle);
		}

		// Display the texture to finalize rendering
		render_texture.display();

		// Extract the pixel data from the render texture
		sf::Image image = render_texture.getTexture().copyToImage();

		// Convert SFML image data into PixelMap
		PixelMap pixel_map;
		for (size_t x = 0; x < PixelMap::resolution_x; ++x)
		{
			for (size_t y = 0; y < PixelMap::resolution_y; ++y)
			{
				sf::Color sfml_color = image.getPixel(x, y);
				pixel_map.map[x][y] = Color{
					sfml_color.r,
					sfml_color.g,
					sfml_color.b,
					sfml_color.a
				};
			}
		}

		return pixel_map;
	}

private:
	static void draw_triangle(sf::RenderTexture& render_texture, const Triangle& triangle)
	{
		sf::VertexArray sf_triangle(sf::Triangles, 3);

		for (size_t i = 0; i < 3; ++i)
		{
			sf_triangle[i].position = sf::Vector2f(
				triangle.vertices[i].position.x * PixelMap::resolution_x,
				triangle.vertices[i].position.y * PixelMap::resolution_y
			);
			sf_triangle[i].color = sf::Color(
				triangle.vertices[i].color.r,
				triangle.vertices[i].color.g,
				triangle.vertices[i].color.b,
				triangle.vertices[i].color.a
			);
		}

		render_texture.draw(sf_triangle);
	}
};

inline sf::Image convert_to_SFML_image(const PixelMap& pixel_map)
{
	sf::Image image;
	image.create(PixelMap::resolution_x, PixelMap::resolution_y);

	// Populate the SFML image with PixelMap data
	for (size_t x = 0; x < PixelMap::resolution_x; ++x)
	{
		for (size_t y = 0; y < PixelMap::resolution_y; ++y)
		{
			const Color& pixel = pixel_map.map[x][y];
			image.setPixel(x, y, sf::Color(pixel.r, pixel.g, pixel.b, pixel.a));
		}
	}

	return image;
}

inline void save_pixel_map_as_image(const PixelMap& pixel_map, const std::string& filename)
{
	// Create an SFML Image
	sf::Image image = convert_to_SFML_image(pixel_map);

	// Save the image to a file
	if (!image.saveToFile(filename))
	{
		throw std::runtime_error("Failed to save image to " + filename);
	}
}