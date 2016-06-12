#include "Mob.h"
#include <SFML/Graphics.hpp>				//����� ��������� ��� ������� � ������� ����, � �� ���� ��� ����������� ��������� ��� �����������
#include <Box2D/Box2D.h> 					//����� ��������� ��� ������� � ������� ����, � �� ���� ��� ����������� ��������� ��� �����������

////////////////////////////////////////////////////////////
/// \constuct objects like settlers into set coordinate...	 
/// todo: ����� ������� ���������� ������� ��� �������� ������� � �������� ���������
////////////////////////////////////////////////////////////

Mob::Mob(float SpawnPosX, float SpawnPosY, float SCALE,
	b2World &World, IntRect TextureRectangl) :isSelect(false), invCntElem(0) {
	mobTexture.loadFromFile("images/tilemap.png");		  //load texture tile map
	mobSprite.setTexture(mobTexture);
	mobSprite.setTextureRect(TextureRectangl);
	mobSprite.setOrigin(16.f, 32.f);	                  //set center of the mobBody
	mobSprite.setPosition(SpawnPosX, SpawnPosY);

	PolygonShape.SetAsBox(16.f / SCALE, 32.f / SCALE);
	
	BodyDef.type = b2_dynamicBody;
	BodyDef.position.Set(SpawnPosX / SCALE, SpawnPosY / SCALE);

	FixtureDef.density = 1;
	FixtureDef.filter.groupIndex = -2;
	FixtureDef.shape = &PolygonShape;
	mpeople = World.CreateBody(&BodyDef);	//add the created object in the world
	mpeople->CreateFixture(&FixtureDef);
	mpeople->SetUserData("Player");
}
Mob::~Mob() { //����� ������� ����������	 	//delete[] ptr_inventoryMas;
}
//////////////////////////////////////////////////////////////////////////////////////////
/// \������ �� ����� � ���������
///  ���� id ��� ����, �� ��������� ���-��, � ��������� ���������� ������.
///  ���� �������� ����� ��� ����� ��� ���, �� ����� ������ c ��������� ���� � ���-��.
//////////////////////////////////////////////////////////////////////////////////////////
void Mob::InvetoryAdd(int idElem, int cntElem) {
	for (int i = 0; i < invCntElem; i++) {
		if ((inventoryMas[i][0] == idElem) && (inventoryMas[i][1] < invLimitEachItem)) {
			inventoryMas[i][1] += cntElem;				//todo: ������ ���� ����� ���������� ����� ������ �����		
			return;
		}
	}
	if (invCntElem < invMaxItem) {
		inventoryMas[invCntElem][0] = idElem;
		inventoryMas[invCntElem][1] = cntElem;
		invCntElem++;
	}
	//else {
	//	return; //show message "not enaugt free space in the inventory"	����� ����� ������� ����� � ���� ��������� ������ ����������� �� ����� �������� ������.
	//}  
}
void Mob::InvetoryDropAllElem() {
	// for (invCntElem; invCntElem > 0 ; invCntElem--;) {
	for (int i = 0; i < invCntElem; i++) {
		//drop code, obj in inventory are create in world in the coordinate of this mob
		//�������� �������� ���������� �������� �� ��������� � ���� � ������������ ������
		inventoryMas[i][0] = 0;
		inventoryMas[i][1] = 0;
		invCntElem--;
	}
}
//void Mob::InvetoryCompact() {}
//void Mob::InvetoryDropElem(int idElem, int cntElem) {}
//void Mob::InvetoryUsedElem(int idElem, int cntElem) {}

//////////////////////////////////////////////////////////////////////////////////////////
/// \������ �� ����� � ���������
///  ���� id ������, �� ���������� ���-��
//////////////////////////////////////////////////////////////////////////////////////////
int Mob::InvetoryGetCntElem(int idElem) {
	int result; result = 0;
	for (int i = 0; i < invCntElem; i++) {
		if (inventoryMas[i][0] == idElem) {
			result += inventoryMas[i][1];
		}								  //����� �� �� ����������� ���� ��� ����� ��������� �����	 		 	   
	}return result; //����� ������ ������� ����� ����� ���������, ����� �������� ��� ����������� �� ������, 
					//���� ���� � ������ 155 ��. �� ������ ����� ������� �� �� ��� � ������� ��� �������, �� ������� ����� ��� ���� ������.
}
void Mob::update(RenderWindow &window, float SCALE, float DEG) {
	mobPos.x = mpeople->GetPosition().x*SCALE;
	mobPos.y = mpeople->GetPosition().y*SCALE;
	mobSprite.setPosition(mobPos.x, mobPos.y);
	mobSprite.setRotation(mpeople->GetAngle()*DEG);					
	window.draw(mobSprite);
	//mobView.setCenter(mobPos.x, mobPos.y);
  //if (Mouse::isButtonPressed(Mouse::Left))	

}			//b2Vec2 vec2;   
			//Vector2i vec2i = Mouse::getPosition(window);
			//vec2.x = vec2i.x;
			//vec2.y = vec2i.y;
			////GetWorldPoint(vec2);
			//	SCALE
			//b2AABB worldAABB;
			//worldAABB.lowerBound.Set(Mouse::getPosition(window).x / SCALE - 100.0f, Mouse::getPosition(window).y / SCALE - 100.0f);
			//worldAABB.upperBound.Set(Mouse::getPosition(window).x / SCALE + 100.0f, Mouse::getPosition(window).y / SCALE + 100.0f);
			
			//4.7 In Shape Point Test
			//	You can test a point for overlap with a shape.You provide a transform for the shape and a world point.
			//	b2Transfrom transform;
			//  transform.SetIdentity();
			//  b2Vec2 point(5.0f, 2.0f);
			//  bool hit = shape->TestPoint(transform, point);
			//Edge and chain shapes always return false, even if the chain is a loop.

			//if (mobSprite.getGlobalBounds().contains(Mouse::getPosition(window))) {	

void Mob::move() { if (!this->isControl) return; 	//������� ������� ������������� �� ���������
	if (Keyboard::isKeyPressed(Keyboard::Right) ) {	mpeople->ApplyLinearImpulse(b2Vec2( 0.5f,  0.f ), mpeople->GetWorldCenter(), 1); } //���������� ����������� ������ �� ����������� ���������� ������
	if (Keyboard::isKeyPressed(Keyboard::Left)  ) { mpeople->ApplyLinearImpulse(b2Vec2(-0.5f,  0.f ), mpeople->GetWorldCenter(), 1); }
	if (Keyboard::isKeyPressed(Keyboard::Up)    ) {	mpeople->ApplyLinearImpulse(b2Vec2( 0.f , -0.5f), mpeople->GetWorldCenter(), 1); }
	if (Keyboard::isKeyPressed(Keyboard::Down)  ) {	mpeople->ApplyLinearImpulse(b2Vec2( 0.f ,  0.5f), mpeople->GetWorldCenter(), 1); }
}
void Mob::patrul(int start, int end, float SCALE) {
	mpeople->SetTransform(mpeople->GetPosition(), 0.f);
	float offset = 0.2f;
	if (mpeople->GetPosition().x*SCALE >= end) offset = -0.2f;
	if (mpeople->GetPosition().x*SCALE <= start) offset = 0.2f;
	mpeople->ApplyLinearImpulse(b2Vec2(offset, 0.f), mpeople->GetWorldCenter(), 1);
}
void Mob::select() {
	//if (Mouse::isButtonPressed(Mouse::Left)) { 
	
	isControl = isSelect = isSelect ? false : true;
}									 //�������� ��������� ���� � ��� �� ������ ������������� �� ��� ����� �� �������
bool Mob::isSelected() {
	return isSelect;
}
