#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

#include "ComplexPlane.h"

int main()
{
	int pixelWidth =  VideoMode::getDesktopMode().width;
	int pixelHeight = VideoMode::getDesktopMode().height;
	VideoMode vm(pixelWidth, pixelHeight);
	RenderWindow window(vm, "Mandelbrot Set", Style::Default);

	ComplexPlane c(pixelWidth, pixelHeight);
	Vector2i center(0, 0);

	Font font;
        if (!font.loadFromFile("fonts/Ubuntu-Regular.ttf"))
        {
                return 1;
        }

	Text text;
	text.setFont(font);
	text.setCharacterSize(24);

	while (window.isOpen())
	{

		///Input

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}

			if (event.type == Event::MouseButtonPressed)
            {       
                if (Mouse::isButtonPressed(Mouse::Button::Right))
				{
					cout << "Right Button Clicked..." << endl;	
					c.zoomOut();
					c.setCenter(Mouse::getPosition(window));
				}

				if (Mouse::isButtonPressed(Mouse::Button::Left))
                {
					cout << "Left Button Clicked..." << endl;	
                    c.zoomIn();
					c.setCenter(Mouse::getPosition(window));
                } 
             }

			if (event.type == Event::MouseMoved)
			{
				c.setMouseLocation(Mouse::getPosition(window));
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
                {       
                        window.close();
                }

		if (Keyboard::isKeyPressed(Keyboard::Right))
                {
                       	c.zoomOut();
                }

		if (Keyboard::isKeyPressed(Keyboard::Left))
                {
                        c.zoomIn();
                }
	
		///Update

		c.updateRender();
		c.loadText(text);

		///Draw	

		window.clear();

		window.draw(c);
		window.draw(text);

		window.display();

	}

	return 0;
}
