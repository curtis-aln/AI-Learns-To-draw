#pragma once
#include "canvas.h"
#include "thread_pool.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <stdexcept>

struct ImageCompiler
{
    static sf::Image compile_one(Canvas& canvas)
    {
        sf::RenderTexture render_texture;
        if (!render_texture.create(canvas.resolution_x_, canvas.resolution_y_))
        {
            throw std::runtime_error("Failed to create render texture");
        }

        // Clear the render texture for each canvas
        render_texture.clear(sf::Color::Black);

        // Draw the triangles of the current canvas
        const auto& triangles = canvas.get_triangles();
        render_texture.draw(triangles);

        // Finalize the drawing
        render_texture.display();

        // Store the resulting image into the container
        return render_texture.getTexture().copyToImage();
    }

    static void compile_all2(std::vector<Canvas>& canvases, std::vector<sf::Image>& hollow_images_container)
    {
        const size_t size = canvases.size();

        if (size == 0) return;

        // Create an SFML RenderTexture once, assuming all canvases have the same resolution
        sf::RenderTexture render_texture;
        if (!render_texture.create(canvases[0].resolution_x_, canvases[0].resolution_y_))
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

    static void compile_all(std::vector<Canvas>& canvases, std::vector<sf::Image>& hollow_images_container, tp::ThreadPool& thread_pool)
    {
        const size_t size = canvases.size();

        if (size == 0) return;

        // Multithreading
        const size_t threads = thread_pool.m_thread_count;
        const size_t canvases_per_thread = size / threads;
        const size_t last_thread_canvases = size - (threads - 1) * canvases_per_thread;


        for (uint32_t t = 0; t < threads; ++t)
        {
            thread_pool.addTask([&hollow_images_container, &canvases, t, canvases_per_thread, last_thread_canvases, threads]
                {
                    const size_t start = t * canvases_per_thread;
                    const size_t end = (t == threads - 1) ? start + last_thread_canvases : start + canvases_per_thread;

                    sf::RenderTexture texture;
                    if (!texture.create(canvases[0].resolution_x_, canvases[0].resolution_y_))
                    {
                        throw std::runtime_error("Failed to create RenderTexture");
                    }

                    for (int i = start; i < end; ++i)
                    {
                        // Clear the render texture for each canvas
                        texture.clear(sf::Color::Black);

                        // Draw the triangles of the current canvas
                        const auto& triangles = canvases[i].get_triangles();
                        texture.draw(triangles);

                        // Store the resulting image into the container
                        hollow_images_container[i] = texture.getTexture().copyToImage();
                    }
                });
        }

        thread_pool.waitForCompletion();
    }

};
