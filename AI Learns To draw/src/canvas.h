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
	float res_x_;
	float res_y_;


	Canvas(float res_x = 0, float res_y = 0)
	{
		res_x_ = res_x;
		res_y_ = res_y;

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

	void set_canvas(Canvas& reference)
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
		int starting_index = triangles_.getVertexCount();

		for (int i = 0; i < 3; ++i)
		{
			sf::Vertex vertex;

			sf::FloatRect rect = { 0.f, 0.f, float(res_x_), float(res_y_) };
			vertex.position = Random::rand_pos_in_rect(rect);
			vertex.color = Random::rand_color();

			triangles_.append(vertex);
		}
	}


	void remove_triangle(const size_t triangle_index)
	{
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