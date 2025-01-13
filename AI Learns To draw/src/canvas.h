#pragma once

#include "random.h"

#include <array>
#include <vector>

struct Vector2f
{
	float x;
	float y;
};

struct Color
{
	int r;
	int g;
	int b;
	int a;
};

struct Vertex
{
	Vector2f position;
	Color color;
};

struct Triangle
{
	std::array<Vertex, 3> vertices;
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
				mutate_triangle(triangle);
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
	void mutate_triangle(Triangle& triangle)
	{
		// possible mutations: offset a vertex, modify the color of the triangle
	}

	Triangle random_triangle()
	{
		// assume canvas space is (0, 0, 1, 1) (x, y, w, h)
		Triangle triangle;
		for (Vertex& vertex : triangle.vertices)
		{
			vertex.position = Vector2f{ Random::rand01_float(), Random::rand01_float() };
			vertex.color = Color{ Random::rand_range(0, 255), Random::rand_range(0, 255), Random::rand_range(0, 255), Random::rand_range(0, 255) };
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