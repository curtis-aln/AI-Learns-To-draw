#pragma once

#include "random.h"

#include <array>
#include <vector>
#include <SFML/Graphics.hpp>

struct PixelMap
{
	static const size_t resolution_x = 800;
	static const size_t resolution_y = 800;
};


struct Triangle
{
	sf::VertexArray vertex_array;
	
	Triangle()
	{
		vertex_array.setPrimitiveType(sf::Triangles);
		vertex_array.resize(3);
	}
};

class Canvas
{
	std::vector<Triangle> triangles_; // todo use array


public:
	Canvas()
	{
		// creating many triangles
		for (int i = 0; i < 15; ++i)
		{
			mutate_canvas(0, 0, 1, 0);
		}
	}


	std::vector <Triangle>& get_triangles()
	{
		return triangles_;
	}



	void mutate_canvas(const float mutation_rate, const float mutation_range, const float creation_rate, const float destruction_rate)
	{
		// For each triangle, there is a probability it will be mutated
		for (Triangle& triangle : triangles_)
		{
			if (Random::rand01_float() < mutation_rate)
			{
				mutate_triangle(triangle, mutation_range);
			}
		}

		// probabilities of creating or destroying triangles
		if (Random::rand01_float() < creation_rate)
		{
			triangles_.push_back(random_triangle());
		}

		if (Random::rand01_float() < destruction_rate)
		{
			remove_triangle(Random::rand_range(size_t(0), triangles_.size() - 1));
		}
	}


private:
	void mutate_triangle(Triangle& triangle, const float mutation_range)
	{
		// possible mutations: offset a vertex, modify the color of the triangle
		const int vertex_idx = Random::rand_range(0, 3);
		const int pos_or_col = Random::rand01_int(); // 0 == position, 1 == color

		sf::Vertex& vertex = triangle.vertex_array[vertex_idx];
		
		if (pos_or_col == 0)
		{
			vertex.position += Random::rand_vector(-40 * mutation_range, 40 * mutation_range);
		}

		else if (pos_or_col)
		{
			const float dt = 40;
			vertex.color.r += Random::rand_range(-dt, dt);
			vertex.color.g += Random::rand_range(-dt, dt);
			vertex.color.b += Random::rand_range(-dt, dt);
			vertex.color.a += Random::rand_range(-dt, dt);
		}
	}

	Triangle random_triangle()
	{
		// assume canvas space is (0, 0, 1, 1) (x, y, w, h)
		Triangle triangle;
		for (int i = 0; i < 3; ++i)
		{

			sf::FloatRect rect = { 0.f, 0.f, float(PixelMap::resolution_x), float(PixelMap::resolution_y) };
			triangle.vertex_array[i].position = Random::rand_pos_in_rect(rect);
			triangle.vertex_array[i].color = Random::rand_color();
		}
		return triangle;
	}


	void remove_triangle(const size_t triangle_index)
	{
		// swapping the triangle with the one at the end then popping the vector
		const size_t end = triangles_.size() - 1;
		Triangle temp = triangles_[triangle_index];
		triangles_[triangle_index] = triangles_[end];
		triangles_[end] = temp;
	}
};