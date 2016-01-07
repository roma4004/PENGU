
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h> 
#pragma once
using namespace sf;
RenderWindow window(VideoMode(800, 600, 32), "PENGU");
const float SCALE = 30.f;
const float DEG = 57.29577f;

b2Vec2 Gravity(0.f, 9.8f);
b2World World(Gravity);

class Mob{
	private:   	
		const float SCALE = 30.f;
		const float DEG = 57.29577f;
	public:
		float ox, oy;
		Texture texture;
		Sprite sprite;
		b2Vec2 position;
		float angle;
		b2Body *mpeople;
		bool nav = true;
		Mob(float x, float y);	
	    ~Mob();
		void Update();
		void move(int v);
		void patrul(int start, int end);
};