#pragma once

#include "random.h"
#include "settings.h"

#include <array>
#include <vector>
#include <SFML/Graphics.hpp>


class Canvas : CanvasSettings, MutationSettings
{
	sf::VertexArray triangles_;


public:
	// image dimensions
	float resolution_x_;
	float resolution_y_;


	Canvas(const float resolution_x = 0, const float resolution_y = 0)
	{
		resolution_x_ = resolution_x;
		resolution_y_ = resolution_y;

		// initializing the triangle vertex array
		triangles_.setPrimitiveType(sf::Triangles);

		// creating many triangles
		for (int i = 0; i < init_triangles; ++i)
		{
			add_random_triangle();
		}
	}


	sf::VertexArray& get_triangles()
	{
		return triangles_;
	}

	void set_canvas(const Canvas& reference)
	{
		triangles_ = reference.triangles_;
	}


	void mutate_canvas()
	{
		// For each triangle, there is a probability it will be mutated
		for (int i = 0; i < triangles_.getVertexCount(); i+=3)
		{
			if (Random::rand01_float() < mutation_rate)
			{
				mutate_triangle(i);
			}
		}

		// probabilities of creating or destroying triangles
		if (Random::rand01_float() < creation_rate)
		{
			add_random_triangle();
		}

		if (Random::rand01_float() < destruction_rate)
		{
			// Generate a random index in terms of triangles
			int index = Random::rand_range(size_t(0), triangles_.getVertexCount() / 3 - 1);
			remove_triangle(index);
		}

	}


private:
	void mutate_triangle(int starting_index)
	{
		// possible mutations: offset a vertex, modify the color of the triangle
		const int vertex_idx = starting_index + Random::rand_range(0, 2);
		const int pos_or_col = Random::rand01_int(); // 0 == position, 1 == color

		sf::Vertex& vertex = triangles_[vertex_idx];
		
		if (pos_or_col == 0)
		{
			vertex.position += Random::rand_vector(-pos_range * mutation_range, pos_range * mutation_range);
		}

		else if (pos_or_col)
		{
			const float dt = col_range;
			vertex.color.r += Random::rand_range(-dt, dt);
			vertex.color.g += Random::rand_range(-dt, dt);
			vertex.color.b += Random::rand_range(-dt, dt);
			vertex.color.a += Random::rand_range(-dt, dt);
		}
	}


	void add_random_triangle()
	{
		// assume canvas space is (0, 0, 1, 1) (x, y, w, h)
		// triangles are generated mostly transparent and within a small area

		// generating the triangle bounds
		const float size = Random::rand_range(70.f, 500.f); // todo make float
		const sf::FloatRect shrunk_rect = { size, size, resolution_x_ - size * 2, resolution_y_ - size * 2 };
		const sf::Vector2f pos = Random::rand_pos_in_rect(shrunk_rect);
		const sf::FloatRect triangle_bounds = { pos.x, pos.y, size, size };

		// for each vertex
		for (int i = 0; i < 3; ++i)
		{
			sf::Vertex vertex;

			sf::FloatRect rect = { 0.f, 0.f, float(resolution_x_), float(resolution_y_) };
			vertex.position = Random::rand_pos_in_rect(triangle_bounds);
			vertex.color = Random::rand_color();
			vertex.color.a = Random::rand_range(5, 30); // initial low alpha color

			triangles_.append(vertex);
		}
	}


	void remove_triangle(const size_t triangle_index)
	{
		if (triangles_.getVertexCount() < 6) return;
		// Each triangle is represented by 3 vertices, so find the start index of the triangle
		const size_t start_index = triangle_index * 3;
		const size_t end = triangles_.getVertexCount() - 1;

		// Swap the vertices of the triangle to remove with the last three vertices
		for (size_t i = 0; i < 3; ++i)
		{
			// Swap each corresponding vertex
			sf::Vertex temp = triangles_[start_index + i];
			triangles_[start_index + i] = triangles_[end - 2 + i];
			triangles_[end - 2 + i] = temp;
		}

		// Remove the last 3 vertices (the swapped triangle)
		triangles_.resize(triangles_.getVertexCount() - 3);
	}
};