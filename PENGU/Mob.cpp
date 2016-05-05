#include "Mob.h"
#include <SFML/Graphics.hpp>				//����� ��������� ��� ������� � ������� ����, � �� ���� ��� ����������� ��������� ��� �����������
#include <Box2D/Box2D.h> 					//����� ��������� ��� ������� � ������� ����, � �� ���� ��� ����������� ��������� ��� �����������
using namespace sf;
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
	if (Mouse::isButtonPressed(Mouse::Button::Left)) {
		if (IntRect(0, 0, 300, 300).contains(Mouse::getPosition(window))) {	 
	//if (mobSprite.getGlobalBounds().contains(Mouse::getPosition(window))) {				  
			if (Mouse::isButtonPressed(Mouse::Left)) select(); 
		}
	}
}
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
