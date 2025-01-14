#pragma once
#include "canvas.h"
#include "thread_pool.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <stdexcept>

struct ImageCompiler
{
    static void compile_all(std::vector<Canvas>& canvases, std::vector<sf::Image>& hollow_images_container)
    {
        const size_t size = canvases.size();

        if (size == 0) return;

        // Create an SFML RenderTexture once, assuming all canvases have the same resolution
        sf::RenderTexture render_texture;
        if (!render_texture.create(canvases[0].res_x_, canvases[0].res_y_))
        {
            throw std::runtime_error("Failed to create render texture");
        }


        for (size_t i = 0; i < size; ++i)
        {
            // Clear the render texture for each canvas
            render_texture.clear(sf::Color::Black);

            // Draw the triangles of the current canvas
            const auto& triangles = canvases[i].get_triangles();
            render_texture.draw(triangles);

            // Finalize the drawing
            render_texture.display();

            // Store the resulting image into the container
            hollow_images_container[i] = render_texture.getTexture().copyToImage();
        }
    
    }
};
