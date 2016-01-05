#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "map.h"
#include "view.h"

using namespace sf;
const float SCALE = 30.f;
const float DEG = 57.29577f;

b2Vec2 Gravity(0.f, 9.8f);
b2World World(Gravity);

RenderWindow window(VideoMode(800, 600, 32), "Test PENGU");
void setObj(float x, float y) {
			b2PolygonShape gr;
			gr.SetAsBox(16 / SCALE, 16 / SCALE);
			b2BodyDef bdef;
			bdef.position.Set(x / SCALE, y / SCALE);
			b2Body *body = World.CreateBody(&bdef);
			body->CreateFixture(&gr, 1);
			body->SetUserData("ground");
			//b2Draw DrawSegment;

}

void setBody(float x, float y, long type) {
	if (type == 'disk') {
		b2CircleShape disk;
		disk.m_radius = 16 / SCALE;
		b2BodyDef bdef;
		bdef.type = b2_dynamicBody;
		bdef.position.Set(x / SCALE, y / SCALE);
		b2FixtureDef fdef;
		fdef.density = 1;
		fdef.restitution = 0.8;
		fdef.shape = &disk;
		b2Body *b_disk = World.CreateBody(&bdef);
		b_disk->CreateFixture(&fdef);
		b_disk->SetUserData("disk");
	}

	if (type == 'box') {
		b2PolygonShape shape;
		shape.SetAsBox(16 / SCALE, 16 / SCALE);
		b2BodyDef bdef;
		bdef.type = b2_dynamicBody;
		bdef.position.Set(x / SCALE, y / SCALE);
		b2Body *body = World.CreateBody(&bdef);
		body->CreateFixture(&shape, 2);
		body->SetUserData("box");
	}
}

class mob {
public: 
	float ox, oy;
	Texture texture;
	Sprite sprite;
	b2Vec2 position;
	float angle;
	b2Body *mpeople;
	bool nav=true;
	mob(int x, int y) {
		texture.loadFromFile("images/tilemap.png");
		sprite.setTexture(texture);
		sprite.setTextureRect(IntRect(0,32,32,64));
		sprite.setOrigin(16, 32);
		sprite.setPosition(x,y);

		b2PolygonShape shape;
		shape.SetAsBox(16 / SCALE, 32 / SCALE);
		b2BodyDef bdef;
		bdef.type = b2_dynamicBody;
		bdef.position.Set(x / SCALE, y / SCALE);
		b2FixtureDef fdef;
		fdef.density = 1;
		fdef.filter.groupIndex = -2;
		fdef.shape = &shape;
		mpeople = World.CreateBody(&bdef);
		mpeople->CreateFixture(&fdef);
		mpeople->SetUserData("Player");
	}
	void Update() {
		b2Vec2 pos = mpeople->GetPosition();
		angle	=	 mpeople->GetAngle();
		ox = pos.x*SCALE;
		oy = pos.y*SCALE;
		sprite.setPosition(pos.x*SCALE, pos.y*SCALE);
		sprite.setRotation(angle*DEG);
		window.draw(sprite);
	}
	void move(int v) {
		switch (v) {
		case 1: mpeople->ApplyLinearImpulse(b2Vec2(0, 0.5), mpeople->GetWorldCenter(),1); break;
		case 2: mpeople->ApplyLinearImpulse(b2Vec2(0, -0.5),mpeople->GetWorldCenter(),1); break;
		case 3: mpeople->ApplyLinearImpulse(b2Vec2(0.5, 0), mpeople->GetWorldCenter(),1); break;
		case 4: mpeople->ApplyLinearImpulse(b2Vec2(-0.5, 0),mpeople->GetWorldCenter(),1); break;
		}
	
	}
	void patrul(int start, int end) {
		b2Vec2 pos = mpeople->GetPosition();
		mpeople->SetTransform(b2Vec2(pos.x,pos.y),0);
		if (nav) {
			mpeople->ApplyLinearImpulse(b2Vec2(0.2, 0), mpeople->GetWorldCenter(), 1);
			if ((pos.x*SCALE) >= end) { nav = false; mpeople->SetLinearVelocity(b2Vec2(0, 0)); }
		}
		else {
			mpeople->ApplyLinearImpulse(b2Vec2(-0.2, 0), mpeople->GetWorldCenter(), 1);
			if ((pos.x*SCALE) <= start) { nav = true; mpeople->SetLinearVelocity(b2Vec2(0,0)); }
		}
	}
};

void menu(RenderWindow & window) {
	Texture menuTexture1, menuTexture2, menuTexture3, aboutTexture, menuBackground;
	menuTexture1.loadFromFile("images/111.png");
	menuTexture2.loadFromFile("images/333.png");
	Sprite menu1(menuTexture1), menu2(menuTexture2);
	bool isMenu = 1;
	int menuNum = 0;
	menu1.setPosition(100, 30);
	menu2.setPosition(100, 90);
while (isMenu)
	{
		menu1.setColor(Color::White);
		menu2.setColor(Color::White);
		menuNum = 0;
		window.clear(Color(129, 181, 221));

		if (IntRect(100, 30, 300, 50).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Blue); menuNum = 1; }
		if (IntRect(100, 90, 300, 50).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Blue); menuNum = 2; }
		
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) isMenu = false;
			if (menuNum == 2) { window.close(); isMenu = false; }
		}

		window.draw(menu1);
		window.draw(menu2);

		window.display();
	}
}

int main()
{	
	menu(window);

	view.reset(FloatRect(0, 0, 800, 600));
	window.setFramerateLimit(60);
	Texture TextureMap;
	TextureMap.loadFromFile("images/tilemap.png");
	Sprite sMap(TextureMap),sPlayer(TextureMap),sBox(TextureMap),sDisk(TextureMap);

	sMap.setTextureRect(IntRect(32, 0, 32, 32));
	sMap.setOrigin(16, 16);

	sBox.setTextureRect(IntRect(0,0,32,32));
	sDisk.setTextureRect(IntRect(64, 0, 32, 32));

	sBox.setOrigin(16, 16);
	sDisk.setOrigin(16, 16);

	for (int i = 0; i < HEIGHT_MAP; i++)
		for (int j = 0; j < WIDTH_MAP; j++)
		{
			if ((TileMap[i][j] == '3')) setObj(j * 32.f, i  * 32.f);
		}
	
	mob mob1(800, 50);
	mob mob2(900, 50);

	while (window.isOpen()) {
		Event e;
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed)
				window.close();
		}

		World.Step(1 / 60.f, 8, 3);
		window.clear(Color::White);

		if (Keyboard::isKeyPressed(Keyboard::Right)){ mob1.move(3); }
		if (Keyboard::isKeyPressed(Keyboard::Left)) { mob1.move(4); }
		if (Keyboard::isKeyPressed(Keyboard::Up))	{ mob1.move(2); }
		if (Keyboard::isKeyPressed(Keyboard::Down)) { mob1.move(1); }
		
		for (b2Body* it = World.GetBodyList(); it != 0; it = it->GetNext()) {

			if (it->GetUserData() == "box") {
				b2Vec2 pos = it->GetPosition();
				float angle = it->GetAngle();
				sBox.setPosition(pos.x*SCALE, pos.y*SCALE);
				sBox.setRotation(angle*DEG);
				window.draw(sBox);
			}
			if (it->GetUserData() == "disk") {
				b2Vec2 pos = it->GetPosition();
				float angle = it->GetAngle();
				sDisk.setPosition(pos.x*SCALE, pos.y*SCALE);
				sDisk.setRotation(angle*DEG);
				window.draw(sDisk);
			}
			if (it->GetUserData() == "ground") {
				b2Vec2 pos = it->GetPosition();
				sMap.setPosition(pos.x*SCALE, pos.y*SCALE);
				window.draw(sMap);
			}
		}

		getplayercoordinateforview(mob1.ox, mob1.oy);
		
		mob1.Update();
		
		mob2.patrul(600,1800);
		mob2.Update();
				
		//viewMove();
		window.setView(view);
		window.display();
		}
	return 0;
}
