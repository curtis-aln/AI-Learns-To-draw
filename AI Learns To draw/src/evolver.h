#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <stdexcept>
#include <iostream>
#include <chrono>

#include "canvas.h"
#include "image_compiler.h"
#include "settings.h"
#include "thread_pool.h"


inline sf::Image loadImage(const std::string& path) 
{
    sf::Image image;
    if (!image.loadFromFile(path)) 
    {
        throw std::runtime_error("Failed to load image from path: " + path);
    }
    return image;
}


template <int PopulationSize, int Generations>
class Evolver : MutationSettings, EvolutionSettings
{
    std::vector<Canvas> population;

    // runtime stats
    int current_generation = 0;
    float best_score = 0.f;

    std::array<float, PopulationSize> scores;
    std::vector<sf::Image> images;
    Canvas* best_canvas = nullptr;
    int bestIndex = 0;
    
public:
    sf::Image reference_image;
    static const int threads = 4;
    tp::ThreadPool thread_pool{ threads };
    
    std::vector<Canvas> best_canvases_history;

public:
    Evolver()
    {
        // loading the reference image
        std::string reference_path = "images/Van-Gogh-Starry-Night-700.jpg";
        reference_image = loadImage(reference_path);

        // initializing the population
        population.resize(PopulationSize);
        images.resize(PopulationSize);

        for (Canvas& canvas : population)
        {
            sf::Vector2u size = reference_image.getSize();
            canvas.resolution_x_ = size.x;
            canvas.resolution_y_ = size.y;
        }
    }


    void evolve(bool show_debug = false)
    {
        best_canvases_history.resize(Generations);

        // Time tracking variables
        auto last_stats_time = std::chrono::steady_clock::now();
        const auto stats_interval = std::chrono::seconds(1);

        float total_compile_time = 0.f;
        float total_score_time = 0.f;
        float total_next_gen_time = 0.f;

        // Running generations
        for (int i = 0; i < Generations; ++i)
        {
            // Compile images
            auto start_compile = std::chrono::steady_clock::now();
            compile_imags();
            auto end_compile = std::chrono::steady_clock::now();
            total_compile_time += std::chrono::duration<float, std::milli>(end_compile - start_compile).count();

            // Calculate scores
            auto start_score = std::chrono::steady_clock::now();
            calculate_scores();
            auto end_score = std::chrono::steady_clock::now();
            total_score_time += std::chrono::duration<float, std::milli>(end_score - start_score).count();

            // Determine best canvas
            get_best_canvas();

            // Create next generation
            auto start_next_gen = std::chrono::steady_clock::now();
            create_next_gen();
            auto end_next_gen = std::chrono::steady_clock::now();
            total_next_gen_time += std::chrono::duration<float, std::milli>(end_next_gen - start_next_gen).count();

            // Check if it's time to display stats
            auto now = std::chrono::steady_clock::now();
            if (now - last_stats_time >= stats_interval && show_debug)
            {
                display_stats();
                last_stats_time = now;
            }

            if (i % 500 == 0 && current_generation != 0)
            {
                sf::Image image = ImageCompiler::compile_one(best_canvases_history[current_generation-1]);
                
                if (!image.saveToFile("image.png"))
                {
                    throw std::runtime_error("Failed to save image to file: ");
                }
                std::cout << "Saved\n";
            }
        }

        // Display timing results at the end
        std::cout << "Evolve Timing Analysis:\n";
        std::cout << "Total time spent on compile_images: " << total_compile_time << " ms\n";
        std::cout << "Total time spent on calculate_scores: " << total_score_time << " ms\n";
        std::cout << "Total time spent on create_next_gen: " << total_next_gen_time << " ms\n";
        std::cout << "Average time per generation: "
            << (total_compile_time + total_score_time + total_next_gen_time) / Generations
            << " ms\n";
    }


private:
    void compile_imags()
    {
        ImageCompiler::compile_all(population, images, thread_pool);
    }

    void calculate_scores()
    {
        for (int i = 0; i < population_size; ++i)
        {
            const sf::Image& image = images[i];

            // Ensure the images are the same size
            if (reference_image.getSize() != image.getSize())
            {
                throw std::invalid_argument("Images must have the same resolution and aspect ratio.");
            }

            const sf::Uint8* pixels1 = reference_image.getPixelsPtr();
            const sf::Uint8* pixels2 = image.getPixelsPtr();
            unsigned int pixelCount = reference_image.getSize().x * reference_image.getSize().y * 4; // RGBA has 4 components

            float sumSquaredDifferences = 0.0f;

            for (unsigned int i = 0; i < pixelCount; i++)
            {
                float diff = static_cast<float>(pixels2[i] - pixels1[i]);
                sumSquaredDifferences += diff * diff;
            }

            scores[i] = sumSquaredDifferences;
        }
    }

    void get_best_canvas()
    {
        auto minIt = std::min_element(scores.begin(), scores.end());
        bestIndex = std::distance(scores.begin(), minIt);
        best_score = *minIt;

        best_canvas = &population[bestIndex];

        best_canvases_history[current_generation] = *best_canvas;
    }

    void create_next_gen()
    {
        current_generation++;

        int i = 0;
        for (Canvas& canvas : population)
        {
            canvas.set_canvas(*best_canvas);

            if (i++ != bestIndex)
            {
                canvas.mutate_canvas();
            }
        }
    }


    void display_stats()
    {
        const auto percentage = static_cast<int>((float(current_generation) / float(Generations)) * 100);
        std::cout << "Generation " << current_generation << "/" << Generations << "(" << percentage << "%)\n";
        std::cout << "best score: " << best_score << "\n";


        std::cout << "\n";
    }
};


// Population size 160
// 836 - 1 thread
// 800 - 16 threads
// 650 - 8 threads
// 600 - 4 threads
// 520 - no display

// Population size 1000
// 16 threads: 3276
// 12 threads:
// 8 threads:
// 4 threads: 2760