#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>  
#include "MainSource.h"	 
	
int main(){		
	CircleShape shape(100.f);
	shape.setFillColor(Color::Green);
	while (window.isOpen() ){
		Event event;
		while (window.pollEvent(event)){
			if (event.type == Event::Closed) window.close();
		}
		window.clear();
		window.draw(shape);	
		
		DrawText(18, 20, 100, L"Выводимый текст");
		
		window.display();
	}
	return 0;
}
									 	
void DrawText(int fontSize, float posX, float posY, String setText, 
			  Color colorOfText, String FontFamily){
	Font font; String FontExt = ".ttf";
	font.loadFromFile("Fonts/"+FontFamily+FontExt);
	Text text("", font, fontSize);
	text.setColor(colorOfText);
	text.setStyle(sf::Text::Bold);
	text.setString(setText);
	text.setPosition(posX, posY);
	window.draw(text);
};