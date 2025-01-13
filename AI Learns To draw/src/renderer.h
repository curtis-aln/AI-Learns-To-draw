#pragma once

struct SFML_Renderer
{
	static void Render(std::vector<sf::Image>& images, const size_t frames_per_second)
	{
		const sf::Vector2u size = images[0].getSize();
		sf::RenderWindow window(sf::VideoMode(size.x, size.y), "window"); //todo noframe

		window.setFramerateLimit(frames_per_second);

		for (int i = 0; i < images.size(); ++i)
		{
			sf::Texture texture;
			texture.loadFromImage(images[i]);
			sf::Sprite sprite(texture);

			// Draw the frame
			window.clear();
			window.draw(sprite);
			window.display();
		}
	}
};