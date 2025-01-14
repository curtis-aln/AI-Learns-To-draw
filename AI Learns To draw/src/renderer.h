#pragma once

struct SFML_Renderer
{
	static bool Render(std::vector<sf::Image>& images, const size_t frames_per_second)
	{
		const sf::Vector2u size = images[0].getSize();
		sf::RenderWindow window(sf::VideoMode(size.x, size.y), "window"); //todo noframe

		window.setFramerateLimit(frames_per_second);

		for (int i = 0; i < images.size(); ++i)
		{
			sf::Event event{};
			while (window.pollEvent(event))
			{

				if (event.type == sf::Event::Closed)
				{
					return true;
				}
					
			}

			sf::Texture texture;
			texture.loadFromImage(images[i]);
			sf::Sprite sprite(texture);

			// Draw the frame
			window.clear();
			window.draw(sprite);
			window.display();
		}
		return false;
	}
};