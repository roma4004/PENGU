#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h> 
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
		Event e; while (window.pollEvent(e)) if (e.type == Event::Closed) window.close(); 	   // ��������� ���� ���� ����� �������
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
		window.draw(menu1);	    //���������  
		window.draw(menu2);	   // ������� ����
		window.display();	  //  � �� �����������
	}
}	
int main(){	
	window.setFramerateLimit(60);                   //����������� ���� ������� ��� �� ���������� ����� ���� ������ ������� ���� ���
	window.setVerticalSyncEnabled(true);		   // ��� �� ������ ����������� � ��������	
	
	menu(window);

	view.reset(FloatRect(0.f, 0.f, 800.f, 600.f));

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
	bool isControl = true;
	while (window.isOpen()) {


		float time = clock.getElapsedTime().asMicroseconds(); //������ ���������� ������� � �������������
	    clock.restart(); //������������� �����
		time = time / 800; //�������� ����


		Event e;
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed     ) window.close();	   // ��������� ���� ���� ����� �������
			if (e.type == Event::GainedFocus) isControl = true;	  // ��������� ������ �������� ����������
			if (e.type == Event::LostFocus  ) isControl = false; // ������ ������ ��������� ����������
		}		 
		World.Step( (1.f / 60.f) , 8, 3);
		window.clear(Color::White);

				if ( (Keyboard::isKeyPressed(Keyboard::Right)) && (isControl) ) { mob1.move(3); }
				if ( (Keyboard::isKeyPressed(Keyboard::Left) ) && (isControl) ) { mob1.move(4); }
				if ( (Keyboard::isKeyPressed(Keyboard::Up)   ) && (isControl) ) { mob1.move(2); }
				if ( (Keyboard::isKeyPressed(Keyboard::Down) ) && (isControl) ) { mob1.move(1); }
   														   
		
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
		DrawText(18, 20, 100, L"��������� �����");
		window.setView(view);
		window.display();	   

		/*sf::Event event;
		while (window.pollEvent(event))	{ // The window was resized
			if (event.type == sf::Event::Resized)
				doSomethingWithTheNewSize(event.size.width, event.size.height);
		}  */
		}
	return 0;
}  
void DrawText(int fontSize, float posX, float posY, String setText,
	Color colorOfText, String FontFamily) {	                    // DrawText(18, 20, 100, L"��������� �����");
	Font font; String FontExt = ".ttf";
	font.loadFromFile("Fonts/" + FontFamily + FontExt);
	Text text("", font, fontSize);
	text.setColor(colorOfText);
	text.setStyle(sf::Text::Bold);
	text.setString(setText);
	text.setPosition(posX, posY);
	window.draw(text);
}