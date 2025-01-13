#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <stdexcept>
#include <iostream>

#include "canvas.h"
#include "image_compiler.h"
#include "settings.h"

inline float calculateSSD(const sf::Image& image1, const sf::Image& image2) 
{
    // Ensure the images are the same size
    if (image1.getSize() != image2.getSize()) 
    {
        throw std::invalid_argument("Images must have the same resolution and aspect ratio.");
    }

    sf::Vector2u size = image1.getSize();
    float sumSquaredDifferences = 0.0f;

    for (unsigned y = 0; y < size.y; ++y) 
    {
        for (unsigned x = 0; x < size.x; ++x) 
        {
            sf::Color pixel1 = image1.getPixel(x, y);
            sf::Color pixel2 = image2.getPixel(x, y);

            float rDiff = static_cast<float>(pixel2.r - pixel1.r);
            float gDiff = static_cast<float>(pixel2.g - pixel1.g);
            float bDiff = static_cast<float>(pixel2.b - pixel1.b);
            float aDiff = static_cast<float>(pixel2.a - pixel1.a);

            sumSquaredDifferences += rDiff * rDiff + gDiff * gDiff + bDiff * bDiff + aDiff * aDiff;
        }
    }

    return sumSquaredDifferences;
}


inline sf::Image loadImage(const std::string& path) 
{
    sf::Image image;
    if (!image.loadFromFile(path)) 
    {
        throw std::runtime_error("Failed to load image from path: " + path);
    }
    return image;
}


class Evolver : MutationSettings, EvolutionSettings
{
    sf::Image reference_image;
    std::vector<Canvas> population;

   
    int gen = 0;
    
public:
    std::vector<Canvas> best_canvases_history;

public:
    Evolver()
    {
        // loading the reference image
        std::string reference_path = "images/Van-Gogh-Starry-Night-700.jpg";
        reference_image = loadImage(reference_path);

        // initializing the population
        population.resize(population_size);

        for (Canvas& canvas : population)
        {
            sf::Vector2u size = reference_image.getSize();
            canvas.res_x_ = size.x;
            canvas.res_y_ = size.y;
        }
    }


    void evolve(int generations)
    {
        best_canvases_history.resize(generations);

        for (int i = 0; i < generations; ++i)
        {
            evaluate_canvases();
            std::cout << i << "\n";
        }
    }

private:
    void evaluate_canvases()
    {
        // preparing space for the scores
        std::vector<float> scores;
        scores.resize(population_size);

        // converting all information into images
        std::vector<sf::Image> images = ImageCompiler::compile_all(population);

        // calculating how well each canvas performed
        for (int i =0; i < population_size; ++i)
        {
            scores[i] = calculateSSD(reference_image, images[i]);
        }

        // getting the best performing canvas and their score
        auto minIt = std::min_element(scores.begin(), scores.end());
        int bestIndex = std::distance(scores.begin(), minIt);
        float bestScore = *minIt;

        Canvas& best_canvas = population[bestIndex];

        best_canvases_history[gen] = best_canvas;
        gen++;

        // Output the best-performing canvas and score (optional)
        std::cout << "Best canvas Score: " << bestScore << std::endl;

        // for each canvas we mutate it based off of the best one
        int i = 0;
        for (Canvas& canvas : population)
        {
            canvas.set_canvas(best_canvas);
            canvas.mutate_canvas();
        }
    }
};