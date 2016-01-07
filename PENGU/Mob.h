#pragma once
using namespace sf;
RenderWindow window(VideoMode(800, 600, 32), "PENGU");
const float SCALE = 30.f;
const float DEG = 57.29577f;

b2Vec2 Gravity(0.f, 9.8f);
b2World World(Gravity);

class Mob{
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
Mob::Mob(float x, float y) {
	texture.loadFromFile("images/tilemap.png");
	sprite.setTexture(texture);
	sprite.setTextureRect(IntRect(0, 32, 32, 64));
	sprite.setOrigin(16.f, 32.f);
	sprite.setPosition(x, y);

	b2PolygonShape shape;
	shape.SetAsBox( (16.f / SCALE), (32.f / SCALE) );
	b2BodyDef bdef;
	bdef.type = b2_dynamicBody;
	bdef.position.Set( (x / SCALE), (y / SCALE) );
	b2FixtureDef fdef;
	fdef.density = 1;
	fdef.filter.groupIndex = -2;
	fdef.shape = &shape;
	mpeople = World.CreateBody(&bdef);
	mpeople->CreateFixture(&fdef);
	mpeople->SetUserData("Player");
}
Mob::~Mob() {}		 //Mob::~Mob();{} нужно сделать деструктор

void Mob::Update() {
	b2Vec2 pos = mpeople->GetPosition();
	angle = mpeople->GetAngle();
	ox = pos.x*SCALE;
	oy = pos.y*SCALE;
	sprite.setPosition(pos.x*SCALE, pos.y*SCALE);
	sprite.setRotation(angle*DEG);
	window.draw(sprite);
}
void Mob::move(int v) {
	switch (v) {
	case 1: mpeople->ApplyLinearImpulse(b2Vec2( 0.f,  0.5f), mpeople->GetWorldCenter(), 1); break;
	case 2: mpeople->ApplyLinearImpulse(b2Vec2( 0.f, -0.5f), mpeople->GetWorldCenter(), 1); break;
	case 3: mpeople->ApplyLinearImpulse(b2Vec2( 0.5f, 0.f),  mpeople->GetWorldCenter(), 1); break;
	case 4: mpeople->ApplyLinearImpulse(b2Vec2(-0.5f, 0.f),  mpeople->GetWorldCenter(), 1); break;
	}
}
void Mob::patrul(int start, int end) {
	b2Vec2 pos = mpeople->GetPosition();
	mpeople->SetTransform(b2Vec2(pos.x, pos.y), 0.f);
	if (nav) {
		mpeople->ApplyLinearImpulse(b2Vec2(0.2f, 0.f), mpeople->GetWorldCenter(), 1);
		if ( (pos.x*SCALE) >= end) { nav = false; mpeople->SetLinearVelocity(b2Vec2(0.f, 0.f)); }
	}
	else {
		mpeople->ApplyLinearImpulse(b2Vec2(-0.2f, 0.f), mpeople->GetWorldCenter(), 1);
		if ( (pos.x*SCALE) <= start) { nav = true; mpeople->SetLinearVelocity(b2Vec2(0.f, 0.f)); }
	}
}