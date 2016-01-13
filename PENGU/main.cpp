#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h> 
//#include <iostream>
#include <sstream>     //use in DrawText(...){...}
//#include <stdio.h>
#include <stdlib.h>
#pragma hdrstop	       //��������� ��� ����� ���� ����� ��� ���� ������ � �� ���������� � ��������������, � ����� �������� �����������
#include "Mob.h"
#include "map.h"
#include "view.h"
#include "main.h"
using namespace sf;	
void CreateRandWorld() {
	srand(time(0));
	int OfMountains[30][2], start,StartString; // ����� 30 ��� �� 2 ��������� 0-�������� �� X, 1- ������ ����

	for (int i = HEIGHT_MAP - 1; i > HEIGHT_MAP - 5; i--) {// �������� ����� ���������� 4 ���� ������ ������������ ������ �� ������ ����� �������
		for (int j = 0; j < WIDTH_MAP; j++)
			TileMap[i][j] = '3';
	}
	// ������� �����
	for (int j = 0; j < WIDTH_MAP; j++) {        // ������� ������, ����� � ������
		TileMap[0][j] = '3';                    // ������� ������ 
		if (j < (HEIGHT_MAP - 1)) {            // ��� ��� ����� ������������� ���� ��������� ���� j �� �������� �� ������ ����� ����� ������
			TileMap[j][0] = '3';              // ������� �����
			TileMap[j][WIDTH_MAP - 1] = '3'; // ������� ������
		}
	}
	OfMountains[0][0] = -10;                  // �������������� ���� � ����� ����� �����,
	OfMountains[0][1] = (rand() % 12) + 10;   //  � ������ ������ ��� ���� ����.
	OfMountains[1][0] = WIDTH_MAP - 10;      // �������������� ���� � ������ ����� �����, 
	OfMountains[1][1] = (rand() % 12) + 10; //  � ������ ������ ��� ���� ����.
	for (int i = 2; i < 30; i++) {                // ������ ��������� ��������� ����� ��� 30 ���
		OfMountains[i][0] = rand() % WIDTH_MAP;  // ���������� �� �
		OfMountains[i][1] = (rand() % 12) + 10; // �������� ������ ��������� ���� ������� ������ ��������
	}															                 // ������������ ����																  
	for (int count = 0; count < 30; count++)                                    // ���������� �� ������ ����
		for (int h = 0; h <= OfMountains[count][1] - 4; h++)  {                   // ������ ���� ��������� ������� � ����� ������ ������� � ���������� (������-4) ����� ��������� ������ ����� ���� � �������� ��������� (count ����������� ����)
			StartString = h + rand() % 2 ;									// ���������� ��� �������� ������ ��������� ����� �� ���� 100% ������ 
			for (int k = 0; k <= (OfMountains[count][1] * 2 - (h * 2)); k++) {// ����� ���������� ������ ������� ������ ����������� ������ � ����������.
				start = OfMountains[count][0] + StartString + k;                       // ������� ������ ���������� ������� ��� ������ ������� ������� ����� �� 1 ���� ������ ��� ���� ���������
				if (start >= WIDTH_MAP-1) start = WIDTH_MAP - 2;           // ��������� ����� �� ���� ������	  					
				if (start <= 0) start = 1;	                    // ��������� ����� �� ���� ����� 												 
				TileMap[(HEIGHT_MAP - 4) - h][start] = '2'; // ���������� �������� 2-�����
			}
}
	for (int i = 0; i < HEIGHT_MAP; i++)													//����� �������� � ������
		for (int j = 0; j < WIDTH_MAP; j++) {
			if ((TileMap[i][j] == '2') && (TileMap[i-1][j] == ' ')) { TileMap[i][j] = '1'; } // ���� � ����� ������� ���� "2" � ��� ��� ����� ������ ��� ��������
		}

	for (int i = 0; i < HEIGHT_MAP; i++)							   //������� ��� �������� ��������������� �����
		for (int j = 0; j < WIDTH_MAP; j++) {
			if (TileMap[i][j] == '3') setObj( (j * 32.f), (i * 32.f), "EdgeWorld");
			if (TileMap[i][j] == '2') setObj((j * 32.f), (i * 32.f), "Ground");
			if (TileMap[i][j] == '1') setObj((j * 32.f), (i * 32.f), "TopGround");
		}
}
void setObj(float x, float y, String type) {
	if (type == "EdgeWorld") {
		b2PolygonShape gr;
		gr.SetAsBox(16.f / SCALE, 16.f / SCALE);
		b2BodyDef bdef;
		bdef.position.Set(x / SCALE, y / SCALE);
		b2Body *body = World.CreateBody(&bdef);
		body->CreateFixture(&gr, 1.f);
		body->SetUserData("EdgeWorld");
	}
	if (type == "Ground") {
		b2PolygonShape gr;
		gr.SetAsBox(16.f / SCALE, 16.f / SCALE);
		b2BodyDef bdef;
		bdef.position.Set(x / SCALE, y / SCALE);
		b2Body *body = World.CreateBody(&bdef);
		body->CreateFixture(&gr, 1.f);
		body->SetUserData("Ground");
	}
	if (type == "TopGround") {
		b2PolygonShape gr;
		gr.SetAsBox(16.f / SCALE, 16.f / SCALE);
		b2BodyDef bdef;
		bdef.position.Set(x / SCALE, y / SCALE);
		b2Body *body = World.CreateBody(&bdef);
		body->CreateFixture(&gr, 1.f);
		body->SetUserData("TopGround");
	}
}	 
void setBody(float x, float y, String type) {
	if (type == "disk") {
		b2CircleShape disk;
		disk.m_radius = (16.f / SCALE);
		b2BodyDef bdef;
		bdef.type = b2_dynamicBody;
		bdef.position.Set( (x / SCALE) , (y / SCALE) );
		b2FixtureDef fdef;
		fdef.density = 1.f;
		fdef.restitution = 0.8f;
		fdef.shape = &disk;
		b2Body *b_disk = World.CreateBody(&bdef);
		b_disk->CreateFixture(&fdef);
		b_disk->SetUserData("disk");
	}  
	if (type == "box") {
		b2PolygonShape shape;
		shape.SetAsBox( (16.f / SCALE), (16.f / SCALE) );
		b2BodyDef bdef;
		bdef.type = b2_dynamicBody;
		bdef.position.Set( (x / SCALE), (y / SCALE) );
		b2Body *body = World.CreateBody(&bdef);
		body->CreateFixture(&shape, 2.f);
		body->SetUserData("box");
	}
}	
void menu(RenderWindow &window) {	//����� ������� �������� ������� � ��������� �����
	Texture menuTexture1, menuTexture2;//, menuTexture3, aboutTexture, menuBackground;
	menuTexture1.loadFromFile("images/111.png");
	menuTexture2.loadFromFile("images/333.png");
	Sprite menu1(menuTexture1), menu2(menuTexture2);

	menu1.setPosition(100.f, 30.f);		              // ��������� �������,
	menu2.setPosition(100.f, 90.f);		             // ��� ������� ����
	while (window.isOpen()){

		eventsOn();
		window.clear(Color(129,181,221));  // �������� ���� ��� ����	 
		menu1.setColor(Color::White);
		menu2.setColor(Color::White);
		if (IntRect(100, 30, 300, 50).contains(Mouse::getPosition(window))) {	  // ������ ������� ������� ������ ���� 1 �� ����� ��� ���������
			menu1.setColor(Color::Blue);									     //  � ���� ���� ������ ����� ���� �� ���������� ����� ����, �� ���� ������ ����
			if (Mouse::isButtonPressed(Mouse::Left)) break;
		} 
		if (IntRect(100, 90, 300, 50).contains(Mouse::getPosition(window))) {	   // ������ ������� ������� ������ ���� 2 �� ����� ��� ���������
			menu2.setColor(Color::Blue);										  //  � ���� ���� ������ ����� ���� �� �������� ���� � �����, �� ���� ����� �� ����
			if (Mouse::isButtonPressed(Mouse::Left)) { window.close(); break; }
		} 
	window.draw(menu1);	    // ���������  
	window.draw(menu2);	   //  ������� ����
	window.display();	  //   � �� �����������
	}
}
int drawtxt = 0;    // ���������� ��� ����� 
int drawtxt2 = 0;  //  ���������� ��� �����
int zoomCnt = 0;  // ������� �����������/���������
void eventsOn(){ 
	Event e;
	while (window.pollEvent(e)) { 
		switch (e.type) { 
			case Event::Closed         : window.close();	break;     	  //  ��������� ���� ���� ����� ������� � ���� ����  			 
			case Event::GainedFocus    : isControl = true;	break;       // ��������� ������ �������� ����������
			case Event::LostFocus      : isControl = false; break;      // ������ ������ ��������� ����������  			
			case Event::MouseWheelMoved: setZoomRate(view.getSize().x, view.getSize().y, e.mouseWheel.delta); break; //e.mouseWheel.delta - �� ������� ���������� // e.mouseWheel.x - ��������� ������� �� � ������� � ������ �������� //e.mouseWheel.y - ��������� �� �
		}  
	}		
	if (Mouse::isButtonPressed(Mouse::Button::Middle)) { view.reset(FloatRect(0.f, 0.f, window.getSize().x, window.getSize().y)); zoomCnt = 0; }
}
void setZoomRate(float W, float H, int wheelDelta) {
	float zoomRate = 50.f * wheelDelta;               //��� ������� ������ �� ���-�� ��������, ��������� �������� ����� � �������� -2..2
	if ((zoomCnt+wheelDelta >= -20) && (zoomCnt+wheelDelta <= 10) ) {
		view.reset(FloatRect(0.f, 0.f, W - zoomRate, (W - zoomRate)*(H / W) ));
	 zoomCnt = zoomCnt + wheelDelta;
	}
	drawtxt = W; 
	drawtxt2 = H;	
}  
float winSizeX=0,winSizeY=0;

void autoResize() { //���� �������� ����������� ����������� ������ ���� 640�480  �������� ����� ��������, � ������ ���� ������ �������� ��� �������	window.setSize(sf::Vector2u(640, 480));	   
	if ((window.getSize().x <= 680) || (window.getSize().y <= 480)) window.setSize(Vector2u(640, 480)); // �������
	if ((window.getSize().x != winSizeX) || (window.getSize().y != winSizeY)) {
		winSizeX = window.getSize().x;
		winSizeY = window.getSize().y;
		float zoomSetX = 0, zoomSetY = 0;
		if (winSizeX > winSizeY) {
			zoomSetX = winSizeX;
			zoomSetY = zoomSetX * winSizeY / winSizeX;
		}
		else {
			zoomSetY = winSizeY;
			zoomSetX = zoomSetY * winSizeX / winSizeY;
		}
		view.reset(FloatRect(0.f, 0.f, zoomSetX, zoomSetY));
		int zoomDelta = zoomCnt;
		zoomCnt = 0;
		setZoomRate(zoomSetX, zoomSetY, zoomDelta);

		drawtxt = zoomSetX;
		drawtxt2 = zoomSetY;
	}
}
int main(){		   
	window.setFramerateLimit(30);                   // ����������� ���� ������� ��� �� ���������� ����� ���� ������ ������� ���� ���
	window.setVerticalSyncEnabled(false);		   //  ��� �� ������ ����������� � ��������	
	view.reset(FloatRect(0.f, 0.f, DefWinSizeX, DefWinSizeY));// ������������� ��������� ������ ������  
	//virtual void sf::Window::onResize()			�������� ��������� �������� ������������� ������� ��� � ����� 
   // virtual void sf::RenderWindow::onResize()		�������� ��������� �������� ������������� ������� ��� � ����� 

	//menu(window);  	
	/////////////////////����� ������� ����� ������� ���� ������� ////////////////////////
	Texture TextureMap;
	TextureMap.loadFromFile("images/tilemap.png");
	Sprite sEdgeWorld(TextureMap), sPlayer(TextureMap), sBox(TextureMap), sDisk(TextureMap),sGround(TextureMap),sTopGround(TextureMap);

	sEdgeWorld.setTextureRect (IntRect(32, 0, 32, 32));
	sBox.setTextureRect (IntRect(0,  0, 32, 32));
	sDisk.setTextureRect(IntRect(64, 0, 32, 32));
	sGround.setTextureRect(IntRect(32, 64, 32, 32));
	sTopGround.setTextureRect(IntRect(32, 32, 32, 32));
	sGround.setOrigin(16.f, 16.f);
	sTopGround.setOrigin(16.f, 16.f);
	sEdgeWorld.setOrigin (16.f, 16.f);
	sBox.setOrigin (16.f, 16.f);
	sDisk.setOrigin(16.f, 16.f);
	//////////////////////////////////////////////
	CreateRandWorld();			 //������� ���	

	Mob mob1(800.f, 50.f);	   //������� ������� ���� ������������
	Mob mob2(900.f, 50.f);	   //������� ������� ���� �� ������������

	//mob1.InvetoryAdd(43, 34);
	//Clock clock; //������� ���������� �������, �.�. �������� �� �������(� �� �������������/�������� ����������).
		float winSizeX = 0;
		float winSizeY = 0;

		while (window.isOpen()) { 	
 
		eventsOn();	   	
		autoResize();  
		
		//float time = clock.getElapsedTime().asMicroseconds(); //������ ���������� ������� � �������������
	    //clock.restart(); //������������� �����
		//time = time / 800; //�������� ����	

		World.Step( (1.f / 60.f) , 8, 3);
		window.clear(Color(181,228,240,1));

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
				sDisk.setPosition((pos.x*SCALE), (pos.y*SCALE));
				sDisk.setRotation((angle*DEG));
				window.draw(sDisk);
			}
			if (it->GetUserData() == "EdgeWorld") {						
				b2Vec2 pos = it->GetPosition();							
				sEdgeWorld.setPosition((pos.x*SCALE), (pos.y*SCALE));	
				window.draw(sEdgeWorld);								
			}															
			if (it->GetUserData() == "Ground") {
				b2Vec2 pos = it->GetPosition();
				sGround.setPosition((pos.x*SCALE), (pos.y*SCALE));
				window.draw(sGround);
			}
			if (it->GetUserData() == "TopGround") {
				b2Vec2 pos = it->GetPosition();
				sTopGround.setPosition((pos.x*SCALE), (pos.y*SCALE));
				window.draw(sTopGround);
			}
		}

		mob1.move();  
		mob2.move();
	//	if ((Keyboard::isKeyPressed(Keyboard::Right)) && (isControl)) { mob1.move(3); }
	//	if ((Keyboard::isKeyPressed(Keyboard::Left) ) && (isControl)) { mob1.move(4); }
	//	if ((Keyboard::isKeyPressed(Keyboard::Up)   ) && (isControl)) { mob1.move(2); }
	//	if ((Keyboard::isKeyPressed(Keyboard::Down) ) && (isControl)) { mob1.move(1); }	  
		
		
		setCamCenter(mob1.ox, mob1.oy);
		
		mob1.update();

		mob2.patrul(600,1800);
		mob2.update();
				
	    viewMove();
			
		DrawText(18, mob1.ox - 30, mob1.oy - 120, "zoomSetX ", drawtxt);
		DrawText(18, mob1.ox - 30, mob1.oy - 100, "zoomSetY ", drawtxt2);
		DrawText(18, mob1.ox - 30, mob1.oy - 80, "zoomCnt  ", zoomCnt);
		window.setView(view);
		window.display();	   
		}  
	return 0;
}  
void DrawText(int fontSize, float posX, float posY, String setText, float addFloatToText,
	Color colorOfText, String FontFamily) {	                    // DrawText(18, 20, 100, L"��������� �����");
	Font font; String FontExt = ".ttf";
	font.loadFromFile("Fonts/" + FontFamily + FontExt);  // �� ���������� ���� ��������� �����
	Text text("", font, fontSize);                      // �� ���������� ���� ��������� �����
	text.setColor(colorOfText);                        // ������� ���� ������
	text.setStyle(sf::Text::Bold);                    // ������� �������� (��� �������) ������, �� ��������� ������� �����
	std::ostringstream value;                        // ������� �����                                                  (����� ����������, �������� ������ � ������� ����� ������ � �������������� ���� �������)
	value << addFloatToText;                        //  ���������� �������� � �����
	text.setString(setText+value.str());		   //   ��������� �������� ������������ � ������
	text.setPosition(posX, posY);				  // �������� ������� ������ �� ������
	window.draw(text);							 // ������������ �����
}