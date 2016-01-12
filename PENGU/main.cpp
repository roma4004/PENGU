#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h> 
//#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#pragma hdrstop	       //��������� ��� ����� ���� ����� ��� ���� ������ � �� ���������� � ��������������, � ����� �������� �����������
#include "Mob.h"
#include "map.h"
#include "view.h"
#include "main.h"
using namespace sf;	 
void CreateRandWorld() {
	int OfMountains[30][2], start; // ����� 30 ��� �� 2 ��������� 0-�������� �� X, 1- ������ ����

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
	srand(time(0));
	OfMountains[0][0] = - 10;                  // �������������� ���� � ����� ����� �����,
	OfMountains[0][1] = (rand() % 12) + 10;   //  � ������ ������ ��� ���� ����.
	OfMountains[1][0] = WIDTH_MAP - 10;      // �������������� ���� � ������ ����� �����, 
	OfMountains[1][1] = (rand() % 12) + 10; //  � ������ ������ ��� ���� ����.
	for (int i = 2; i < 30; i++) {                // ������ ��������� ��������� ����� ��� 30 ���
		OfMountains[i][0] = rand() % WIDTH_MAP;  // ���������� �� �
		OfMountains[i][1] = (rand() % 12) + 10; // �������� ������ ��������� ���� ������� ������ ��������
	}															                 // ������������ ����																  
	for (int count = 0; count < 30; count++)                                    // ���������� �� ������ ����
		for (int h = 0; h <= OfMountains[count][1]-4; h++)                     // ������ ���� ��������� ������� � ����� ������ ������� � ���������� (������-4) ����� ��������� ������ ����� ���� � �������� ��������� (count ����������� ����)
			for (int k = 0; k <= (OfMountains[count][1] * 2 - (h * 2)); k++) {// ����� ���������� ������ ������� ������ ����������� ������ � ����������.
				start = OfMountains[count][0] + h + k;                       // ������� ������ ���������� ������� ��� ������ ������� ������� ����� �� 1 ���� ������ ��� ���� ���������
				if (start >= WIDTH_MAP) start = WIDTH_MAP - 1;           // ��������� ����� �� ���� ������	  					
				if (start <= 0        ) start = 1;	                    // ��������� ����� �� ���� ����� 												 
				TileMap[(HEIGHT_MAP - 5) - h][start] = '3'; // ���������� �������� 3-�����
			} 	
	for (int i = 0; i < HEIGHT_MAP; i++)							   //������� ��� �������� ��������������� �����
		for (int j = 0; j < WIDTH_MAP; j++) {
			if (TileMap[i][j] == '3') setObj( (j * 32.f), (i * 32.f) );
		}
}
void setObj(float x, float y) {
			b2PolygonShape gr;
			gr.SetAsBox(16.f / SCALE, 16.f / SCALE);
			b2BodyDef bdef;
			bdef.position.Set(x / SCALE, y / SCALE);
			b2Body *body = World.CreateBody(&bdef);
			body->CreateFixture(&gr, 1.f);
			body->SetUserData("ground");
			//b2Draw DrawSegment;  
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
		//if (e.type == Event::Resized) {
		//	view.reset(FloatRect(0.f, 0.f, window.getSize().x, window.getSize().y));
		//}
		switch (e.type) { 
			case Event::Closed         : window.close();	break;     	  //  ��������� ���� ���� ����� ������� � ���� ����  			 
			case Event::GainedFocus    : isControl = true;	break;       // ��������� ������ �������� ����������
			case Event::LostFocus      : isControl = false; break;      // ������ ������ ��������� ����������  			
			case Event::MouseWheelMoved:  
				//if ((e.mouseWheel.delta == -2) && (zoomCnt > -10)) { view.reset(FloatRect(0.f, 0.f, e.size.width*0.8f, e.size.height*0.8f)); zoomCnt--; }			if(W>H) 1024/768    =1, 333333333333333		1024/768    =1.3333...	
				//if ((e.mouseWheel.delta == -1) && (zoomCnt > -10)) { view.reset(FloatRect(0.f, 0.f, e.size.width*0.9f, e.size.height*0.9f)); zoomCnt -= 2; }		if(H>W)  768/1024   =0.75			1024/�	    =0.75		
				//if ((e.mouseWheel.delta == 1) && (zoomCnt < 10))  { view.reset(FloatRect(0.f, 0.f, e.size.width*1.1f, e.size.height*1.1f)); zoomCnt++; }			if(W>H)	 W/H						 �=	Wn*ratio
				//if ((e.mouseWheel.delta == 2) && (zoomCnt < 10))  { view.reset(FloatRect(0.f, 0.f, e.size.width*1.2f, e.size.height*1.2f)); zoomCnt += 2; }		if(H>W)	 H/W
				
					 // 800>600		ratio = 800/600 = 1.333333333333333
					// 1024>768		ratio = 1024/768 = 1.333333333333333
				//     if (W > H) { ratio = W / H; }			  //���� ����������� ������ �������� ��	W=W+100; �� ���������������� ������ ������  H=(W+100)*(W/H);
				//else if (H > W) { ratio = H / W; }			  //���� ����������� ������	�������� �� H=H+100; �� ���������������� ������ ������  W=(H+100)*(H/W);
			     	// 768>1024		ratio = 768/1024 = 0.75
					// 600>800		ratio = 600/800  = 0.75	
																 
				//if ((e.mouseWheel.delta == -1) && (zoomCnt > -1)) { view.reset(FloatRect(0.f, 0.f,  1024.f, (( 1024.f)*(H / W) ) ) ); zoomCnt--; }
				//if ((e.mouseWheel.delta ==  1) && (zoomCnt <  1)) { view.reset(FloatRect(0.f, 0.f,  800.f, ((  800.f)*(H / W) ) ) ); zoomCnt++; }	
																								  //
				//e.mouseWheel.delta   - �� ������� ����������
				//e.mouseWheel.x ; 	   - ��������� �� � ������� � ������ ��������
				//e.mouseWheel.x 	   - ��������� �� � ������� � ������ ��������

				float W = view.getSize().x;  // �������� ������ ������   
				float H = view.getSize().y; //  �� � � �� �             
				drawtxt = W; 
				drawtxt2 = H;
				// �� ������� ���� ����� ����� �������� ����� ��������� ���������� � ���������� ��� ���������� ���������. �������� ������ ��� �������. 
				if ((zoomCnt > -19)&&(e.mouseWheel.delta == -2)) {/*view.zoom(1.40f);*/view.reset(FloatRect(0.f, 0.f, W + 100.f, (W + 100.f)*(H / W) )); zoomCnt-=2;}	//��������� � ��������� �����		//���� ��������� ��� � �����������: ��� ��������� �� ���� ����� � �������� �����������, ����� ���� ��� �������������
				if ((zoomCnt > -20)&&(e.mouseWheel.delta == -1)) {/*view.zoom(1.20f);*/view.reset(FloatRect(0.f, 0.f, W +  50.f, (W +  50.f)*(H / W) )); zoomCnt--; }	//��������� � ������� �����			//���� ��������� ��� � �����������: ��� ��������� �� ���� ����� � �������� �����������, ����� ���� ��� �������������
				if ((zoomCnt <  10)&&(e.mouseWheel.delta ==  1)) {/*view.zoom(0.80f);*/view.reset(FloatRect(0.f, 0.f, W -  50.f, (W -  50.f)*(H / W) )); zoomCnt++; }	//����������� � ��������� �����		//���� ��������� ��� � �����������: ��� ��������� �� ���� ����� � �������� �����������, ����� ���� ��� �������������
				if ((zoomCnt <   9)&&(e.mouseWheel.delta ==  2)) {/*view.zoom(0.60f);*/view.reset(FloatRect(0.f, 0.f, W - 100.f, (W - 100.f)*(H / W) )); zoomCnt+=2;}	//����������� � ������� �����		//���� ��������� ��� � �����������: ��� ��������� �� ���� ����� � �������� �����������, ����� ���� ��� �������������

		}  
	}		
	if (Mouse::isButtonPressed(Mouse::Button::Middle)) { view.reset(FloatRect(0.f, 0.f, window.getSize().x, window.getSize().y)); zoomCnt = 0; }
}
int main(){

	window.setFramerateLimit(60);                   // ����������� ���� ������� ��� �� ���������� ����� ���� ������ ������� ���� ���
	window.setVerticalSyncEnabled(true);		   //  ��� �� ������ ����������� � ��������	
	view.reset(FloatRect(0.f, 0.f, DefWinSizeX, DefWinSizeY));// ������������� ��������� ������ ������ 800�600px   

	menu(window);  	

	Texture TextureMap;
	TextureMap.loadFromFile("images/tilemap.png");
	Sprite sMap(TextureMap), sPlayer(TextureMap), sBox(TextureMap), sDisk(TextureMap);

	sMap.setTextureRect (IntRect(32, 0, 32, 32)); 
	sBox.setTextureRect (IntRect(0,  0, 32, 32));
	sDisk.setTextureRect(IntRect(64, 0, 32, 32));
	sMap.setOrigin (16.f, 16.f);
	sBox.setOrigin (16.f, 16.f);
	sDisk.setOrigin(16.f, 16.f);
	
	CreateRandWorld();			 //������� ���	

	Mob mob1(800.f, 50.f);	   //������� ������� ���� ������������
	Mob mob2(900.f, 50.f);	   //������� ������� ���� �� ������������

	//mob1.InvetoryAdd(43, 34);
	Clock clock; //������� ���������� �������, �.�. �������� �� �������(� �� �������������/�������� ����������).
		float winSizeX = 0;
		float winSizeY = 0;

	while (window.isOpen()) {
		/* ���������� �� �������  void resizeAndZoom(int zoomScrollDelta = 0 ){

	     } 	 	
	    */
		if ( (window.getSize().x != winSizeX) || (window.getSize().y != winSizeY) ) {
			winSizeX = window.getSize().x;
			winSizeY = window.getSize().y;
			
			float zoomOffSet = 0;
			float zoomSetX = 0;
			float zoomSetY = 0;
			float rate = 0;
			//zoomScrollDelta==
			//switch (zoomScrollDelta) {
			//case -2:  100.f*zoomCnt;	break;
			//case -1:   50.f*zoomCnt;	break;
			//case  1:  -50.f*zoomCnt;	break;
			//case  2: -100.f*zoomCnt;	break;
			//zoomOffSet = 50.f*zoomCnt;                                                  //zoomOffSet = 50.f*zoomCnt;				     // zoomOffSet = 50.f*zoomCnt;
			//         													                 
			if (           winSizeX > winSizeY) {			                            // if (         winSizeX > winSizeY) {			 // if (         winSizeX > winSizeY) { 
				rate     =  winSizeY / winSizeX;                                        //  	rate     = winSizeY / winSizeX;			 // 	rate     =  winSizeY / winSizeX;
				zoomSetX =  winSizeX + zoomOffSet;                                      //    zoomSetX = winSizeX + zoomOffSet;			 // 	zoomSetX =  winSizeX + zoomOffSet;
				zoomSetY = zoomSetX * rate + (zoomOffSet * (winSizeY / winSizeX));      //    zoomSetY = zoomSetX * rate;				 // 	zoomSetY = (zoomSetX + zoomOffSet) * rate;
			}                                                                           // }											 // }
			else    /*if (winSizeX <= winSizeY)*/{                                      // else    /*if (winSizeX <= winSizeY)*/{		 // else    /*if (winSizeX <= winSizeY)*/{
				rate     =  winSizeX / winSizeY;	  		                            //   rate     = winSizeX / winSizeY;			 // 	rate     =  winSizeX / winSizeY;	  		
				zoomSetY =  winSizeY + zoomOffSet;         	                            //   zoomSetY = winSizeY + zoomOffSet;			 // 	zoomSetY =  winSizeY + zoomOffSet;
				zoomSetX =  zoomSetY * rate + (zoomOffSet * (winSizeY / winSizeX));     //   zoomSetX = zoomSetY * rate;				 // 	zoomSetX = (zoomSetY + zoomOffSet) * rate;
			}												                            // }											 // }
			view.reset(FloatRect(0.f, 0.f, zoomSetX, zoomSetY));
			drawtxt = zoomSetX;
			drawtxt2 = zoomSetY;
			}


		
		
		

		//view.reset(FloatRect(0.f, 0.f, window.getSize().x, window.getSize().y));

		eventsOn();	   	

		float time = clock.getElapsedTime().asMicroseconds(); //������ ���������� ������� � �������������
	    clock.restart(); //������������� �����
		time = time / 800; //�������� ����	

		World.Step( (1.f / 60.f) , 8, 3);
		window.clear(Color::White);

		mob1.move();  
		mob2.move();
	//	if ((Keyboard::isKeyPressed(Keyboard::Right)) && (isControl)) { mob1.move(3); }
	//	if ((Keyboard::isKeyPressed(Keyboard::Left) ) && (isControl)) { mob1.move(4); }
	//	if ((Keyboard::isKeyPressed(Keyboard::Up)   ) && (isControl)) { mob1.move(2); }
	//	if ((Keyboard::isKeyPressed(Keyboard::Down) ) && (isControl)) { mob1.move(1); }	  
		
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
				sDisk.setPosition( (pos.x*SCALE), (pos.y*SCALE) );
				sDisk.setRotation( (angle*DEG) );
				window.draw(sDisk);
			}
			if (it->GetUserData() == "ground") {
				b2Vec2 pos = it->GetPosition();
				sMap.setPosition( (pos.x*SCALE), (pos.y*SCALE) );
				window.draw(sMap);
			}
		}	
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
void DrawText(int fontSize, float posX, float posY, String setText, float value1,
	Color colorOfText, String FontFamily) {	                    // DrawText(18, 20, 100, L"��������� �����");
	Font font; String FontExt = ".ttf";
	font.loadFromFile("Fonts/" + FontFamily + FontExt);
	Text text("", font, fontSize);
	text.setColor(colorOfText);
	text.setStyle(sf::Text::Bold);
	std::ostringstream value;
	value << value1;
	text.setString(setText+value.str());
	text.setPosition(posX, posY);
	window.draw(text);
}