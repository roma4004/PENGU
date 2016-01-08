#pragma once
using namespace sf;
RenderWindow window(VideoMode(800, 600, 32), "PENGU");
const float SCALE = 30.f;
const float DEG = 57.29577f;

b2Vec2 Gravity(0.f, 9.8f);
b2World World(Gravity);	

/// todo: сделать инвентарь в виде списка, в который потом заносить айди предмета и кол-во
		// int invMaxItem=10;		   
		// int invLimitEachItem=100;  	 
		// int *ptr_inventoryMas = new int[invMaxItem][invLimitEachItem]; // ¬ыделение пам€ти дл€ массива
		//	ptr_inventoryMas[0] = CntOfElem;
		//
		// for (int i=0; i<invMaxItem; i++) { 	// «аполнение массива и вывод значений его элементов
		// 
		//ptr_inventoryMas[i] = i;
		// cout << "Value of " << i << " element is " << p_darr[i] << endl;
		//}
		//delete[] p_darr; // очистка пам€ти

	////////////////////////////////////////////////////////////
	/// \objects like settlers...
	////////////////////////////////////////////////////////////
class Mob{ 
	private:
		bool isSelect;			     //mark which indicate mouse selected or not mob object
		int invCntElem = 0;			 //total counter of element in the inventory
		int invMaxItem = 10;         //max element of the inventory
		int invLimitEachItem = 100;  //max limit of each element in the inventory
		//int **ptr_inventoryMas = (int **)new int[invMaxItem][2];   		  
		//така€ запись позвол€ет объ€вл€ть массив через переменную длинну, только дл€ самого левого значение, остальные должны быть константными mas[perem][5]
		
		int inventoryMas[10][2] = { { },{ } }; //[0][0]=id of element; [0][1]=count of element	

		unsigned __int8 health;		//0..255	<stil not used> 
		unsigned __int8 armore;		//0..255	<stil not used>
		
		Texture mobTexture;
		Sprite mobSprite;
		float mobAngle;
		b2Vec2 position;		
		b2Body *mpeople;
		bool nav = true;
	public:
		float ox, oy; 
		Mob(float x, float y);	
	    ~Mob();	
		void InvetoryAdd(int idElem, int cntElem);
		void InvetoryDropAllElem();
		void update();
		void move(int v);
		void patrul(int start, int end);
		void select();
		bool isSelected();
};	
////////////////////////////////////////////////////////////
/// \constuct objects like settlers into set coordinate...	 
/// todo: нужно сделать перегрузку функции дл€ создание объекта с заданной текстурой
////////////////////////////////////////////////////////////	
Mob::Mob(float SpawnPosX, float SpawnPosY):isSelect(false), invCntElem(0) {
	mobTexture.loadFromFile("images/tilemap.png");		  //load texture tile map
	mobSprite.setTexture(mobTexture);
	mobSprite.setTextureRect(IntRect(0, 32, 32, 64));
	mobSprite.setOrigin(16.f, 32.f);	                  //set center of the mobBody
	mobSprite.setPosition(SpawnPosX, SpawnPosY);

	b2PolygonShape shape;
	shape.SetAsBox( (16.f / SCALE), (32.f / SCALE) );
	b2BodyDef bdef;
	bdef.type = b2_dynamicBody;
	bdef.position.Set( (SpawnPosX / SCALE), (SpawnPosY / SCALE) );
	b2FixtureDef fdef;
	fdef.density = 1;
	fdef.filter.groupIndex = -2;
	fdef.shape = &shape;
	  //add the created object in the world
	mpeople = World.CreateBody(&bdef);
	mpeople->CreateFixture(&fdef);
	mpeople->SetUserData("Player");
}
 Mob::~Mob(){ //нужно сделать деструктор	 	//delete[] ptr_inventoryMas;
 }
 //////////////////////////////////////////////////////////////////////////////////////////
 /// \проход по всему в инвентаре
 ///  если id уже есть, то добавл€ем кол-во, с проверкой превышени€ лимита.
 ///  если превышен лимит или таких еще нет, то нова€ запись c указанным айди и кол-во.
 //////////////////////////////////////////////////////////////////////////////////////////
void Mob::InvetoryAdd(int idElem, int cntElem) {					
	for (int i = 0; i < invCntElem; i++) {
		if((inventoryMas[i][0] == idElem)&&(inventoryMas[i][1] < invLimitEachItem)) {							
			inventoryMas[i][1] += cntElem;				//todo: учесть если после добавлени€ будет больше сотни		
			return;												
		}
	}
	inventoryMas[invCntElem][0] = idElem;
	inventoryMas[invCntElem][1] = cntElem;
	invCntElem++;  
}
void Mob::InvetoryDropAllElem() {
	for (int i = 0; i < invCntElem; i++) {
		//drop code, obj in inventory are create in world in the coordinate of this mob
		//дописать создание вычитаемых объектов из инвентар€ в мире с координамати игрока
		inventoryMas[i][0] = 0;
		inventoryMas[i][1] = 0;
		invCntElem--;
	}
}
//void Mob::InvetoryCompact() {}
//void Mob::InvetoryDropElem(int idElem, int cntElem) {}
//void Mob::InvetoryUsedElem(int idElem, int cntElem) {}
void Mob::update() {
	b2Vec2 pos = mpeople->GetPosition();
	mobAngle = mpeople->GetAngle();
	ox = pos.x*SCALE;
	oy = pos.y*SCALE;
	mobSprite.setPosition(pos.x*SCALE, pos.y*SCALE);
	mobSprite.setRotation(mobAngle*DEG);
	window.draw(mobSprite);
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
void Mob::select() {
	isSelect = isSelect ? false : true;
}
bool Mob::isSelected() {
	return isSelect;
}  