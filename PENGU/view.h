#pragma once
using namespace sf;
//нужно сделать класс камера и это все и подобное туда.
View view;
void setCamCenter(float coordX, float coordY) {
	view.setCenter(coordX, coordY);
	}
void setCamCenter(Vector2f coord) {
	view.setCenter(coord.x, coord.y);
}
//void setCamCenter(Vector2u coord) {
//	view.setCenter(coord.x, coord.y);
//}
void setCamCenter(b2Vec2 coord) {
	view.setCenter(coord.x, coord.y);
}
//b2Vec2 getB2Vec2FromVector2i(Vector2i vec2i) {
//	b2Vec2 vec2;
//	vec2.x = vec2i.x;
//	vec2.y = vec2i.y;
//	return vec2;
//}
void viewMove(RenderWindow &window) {
	//Event onFocusEvent;	//тут еще не доделан фокус, нужно как в том управлении игроком сделать
	//while (window.pollEvent(onFocusEvent)) {
	//if (onFocusEvent.type == Event::GainedFocus){		
		if (Keyboard::isKeyPressed(Keyboard::D)) view.move( 8.f,  0.f);
		if (Keyboard::isKeyPressed(Keyboard::A)) view.move(-8.f,  0.f);
		if (Keyboard::isKeyPressed(Keyboard::W)) view.move( 0.f, -8.f);
		if (Keyboard::isKeyPressed(Keyboard::S)) view.move( 0.f,  8.f);	 
	//	}
	//}
}  		 