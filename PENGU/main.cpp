#include <sstream>     //use in DrawText(...){...}
#include <stdlib.h>
#pragma hdrstop	       //��������� ��� ����� ���� ����� ��� ���� ������ � �� ���������� � ��������������, � ����� �������� �����������
//#include "MainObject.h"
#include "Mob.h"
#include "Cam.h"
#include "map.h"   
#include "main.h"
using namespace sf;
bool isWindowFocused = true;
int DefWinSizeX = 1024;		 //set standard width of virtual screen
int DefWinSizeY = 768;		//set standard height of virtual screen
const float SCALE = 30.f;
const float DEG = 57.29577f;   
b2Vec2 Gravity(0.f, 9.8f);
b2World World(Gravity);
RenderWindow window(VideoMode(DefWinSizeX, DefWinSizeY, 32), "PENGU"); 
Cam Camera;
void Mountain(int center, int count, int LineTarget, int MountainTopDelete) {
		if (count - MountainTopDelete <= 0)   return;
		Mountain(center, count - 1, LineTarget - 1, MountainTopDelete);
		int start = center - count + rand() % 2;
		int end = center + count - rand() % 2;
		if (start < 1) start = 1;
		if (end > WIDTH_MAP - 1) end = WIDTH_MAP - 1;
			for (int i = start; i <= end; i++) {
			TileMap[LineTarget][i] = '3';			
		}		
	}

void CreateRandWorld() {
	srand(static_cast<unsigned int>(time(NULL)));
	for (int i = 0; i < 100; i++)
		Mountain(rand() % 146, rand() % 25, HEIGHT_MAP - 1, rand() % 25);
	
	// ������� ����
	for (int j = 0; j < WIDTH_MAP; j++) {
		TileMap[0][j] = '3';
		TileMap[HEIGHT_MAP - 1][j] = '3';
		if (j < (HEIGHT_MAP - 1)) {
			TileMap[j][0] = '3';
			TileMap[j][WIDTH_MAP - 1] = '3';
		}
	}
	// ������� ���� �����

	//������� ��� �������� ��������������� �����
	for (int i = 0; i < HEIGHT_MAP; i++)							  
		for (int j = 0; j < WIDTH_MAP; j++) {
			if (TileMap[i][j] == '3') setObj((j * 32.f), (i * 32.f), "EdgeWorld");
		}
}

void setObj(float x, float y, void *type) {   	
	b2PolygonShape gr;						  
	gr.SetAsBox(16.f / SCALE, 16.f / SCALE);  
	b2BodyDef bdef;							  
	//bdef.position.Set(round(x / SCALE), round(y / SCALE));
	bdef.position.Set(x / SCALE, y / SCALE);
	b2Body *body = World.CreateBody(&bdef);	  
	body->CreateFixture(&gr, 1.f);			  
	body->SetUserData(type); 
}	 
void setBody(float x, float y, void *type) { //function now not used
	b2BodyDef bdef;								   
	bdef.type = b2_dynamicBody;					   
	bdef.position.Set(x / SCALE, y / SCALE);
	b2Body *body = World.CreateBody(&bdef);  
	body->SetUserData(type);

	if (type == "disk") {
		b2CircleShape shape;
		shape.m_radius = 16.f / SCALE;
		b2FixtureDef fdef;							   
		fdef.density = 1.f;
		fdef.restitution = 0.8f;
		fdef.shape = &shape;
		body->CreateFixture(&fdef);		
	}

	if (type == "box") {
		b2PolygonShape shape;
		shape.SetAsBox( 16.f / SCALE, 16.f / SCALE ); 		
		body->CreateFixture(&shape, 2.f);
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
			if (Mouse::isButtonPressed(Mouse::Left)) break;	} 
		if (IntRect(100, 90, 300, 50).contains(Mouse::getPosition(window))) {	   // ������ ������� ������� ������ ���� 2 �� ����� ��� ���������
			menu2.setColor(Color::Blue);										  //  � ���� ���� ������ ����� ���� �� �������� ���� � �����, �� ���� ����� �� ����
			if (Mouse::isButtonPressed(Mouse::Left)) { window.close(); break; }
		} 
	window.draw(menu1);	    // ���������  
	window.draw(menu2);	   //  ������� ����
	window.display();	  //   � �� �����������
	}
}	
void eventsOn(){ 
	Event e;
	while (window.pollEvent(e)) { 
		switch (e.type) { 
			case Event::Closed         : window.close();	                      break;   //  ��������� ���� ���� ����� ������� � ���� ����  			 
			case Event::GainedFocus    : isWindowFocused = true;                  break;  // ��������� ������ �������� ����������
			case Event::LostFocus      : isWindowFocused = false;                 break; // ������ ������ ��������� ���������� 
			case Event::MouseEntered   : isWindowFocused = true;                  break;
			case Event::MouseWheelMoved: Camera.setZoomDelta(e.mouseWheel.delta); break; //e.mouseWheel.delta - �� ������� ���������� // e.mouseWheel.x - ��������� ������� �� � ������� � ������ �������� //e.mouseWheel.y - ��������� �� �
		}  
	}		
	if (Mouse::isButtonPressed(Mouse::Button::Middle)) { 
		Camera.reset(
			FloatRect(0.f,0.f,
				static_cast<float>(window.getSize().x),
				static_cast<float>(window.getSize().y)
			)
		);
		Camera.setZoomCnt(0);
	}	
}	 
void autoResize() { 
	//ned func set one side of screen
	if (window.getSize().x <= Camera.minWindowSize.x) window.setSize(Vector2u(Camera.minWindowSize.x, window.getSize().y     ));
	if (window.getSize().y <= Camera.minWindowSize.y) window.setSize(Vector2u(    window.getSize().x, Camera.minWindowSize.y));

	if ((window.getSize().x != winSizeX) 
	 || (window.getSize().y != winSizeY)) {
		winSizeX = static_cast<float>( window.getSize().x);										  
		winSizeY = static_cast<float>( window.getSize().y);

		//view.reset(FloatRect(0.f, 0.f, winSizeX, winSizeY));
		//view.setViewport(sf::FloatRect(0.0f, 0.0f, 1.5f, 1.5f));
		//view.getViewport().
		int zoomDelta = Camera.getZoomCnt(); Camera.setZoomCnt(0);
		Camera.setZoomRate(window.getSize().x, window.getSize().y, zoomDelta);
		//setZoomRate(winSizeX, winSizeY, zoomDelta);
		//{start debug section 
		drawtxt = winSizeX;
		drawtxt2 = winSizeY;
		//}end debug section 
	}
}
void drawSprite(Sprite targetSprite, b2Vec2 pos, float angle)
{
	//while (angle <= 0 ) { angle += 360.f; }		  //the normalized angle if below 0
	//while (angle >=360) { angle -= 360.f; }		  //the normalized angle if above 360
	targetSprite.setPosition(pos.x*SCALE, pos.y*SCALE);
	targetSprite.setRotation(angle*DEG);
	window.draw(targetSprite);
}

int main(){		   
	//window.setFramerateLimit(optimaFPS);                   // ����������� ���� ������� ��� �� ���������� ����� ���� ������ ������� ���� ���
	window.setVerticalSyncEnabled(true);		   //  ��� �� ������ ����������� � ��������	
	//view.reset(FloatRect(0.f, 0.f, DefWinSizeX, DefWinSizeY));// ������������� ��������� ������ ������  
	//virtual void sf::Window::onResize()			�������� ��������� �������� ������������� ������� ��� � ����� 
    // virtual void sf::RenderWindow::onResize()		�������� ��������� �������� ������������� ������� ��� � ����� 
	Font font; font.loadFromFile("Fonts/times.ttf");  // �� ���������� ���� ��������� �����
	Text text("", font, 18);                      // �� ���������� ���� ��������� �����
	text.setColor(Color::Black);                        // ������� ���� ������
	text.setStyle(sf::Text::Bold);                    // ������� �������� (��� �������) ������, �� ��������� ������� �����
	
	//menu(window);  	
	/////////////////////����� ������� ����� ������� ���� ������� ////////////////////////
	Texture TextureMap;
	TextureMap.loadFromFile("images/tilemap.png");
	Sprite sEdgeWorld(TextureMap), sPlayer(TextureMap), sBox(TextureMap), sDisk(TextureMap),sGround(TextureMap),sTopGround(TextureMap);	

	      sBox.setTextureRect (IntRect( 0,  0, 32, 32));
	     sDisk.setTextureRect (IntRect(64,  0, 32, 32));
	   sGround.setTextureRect (IntRect(32, 64, 32, 32));
	sTopGround.setTextureRect (IntRect(32, 32, 32, 32));
	sEdgeWorld.setTextureRect (IntRect(32,  0, 32, 32));

	      sBox.setOrigin (16.f, 16.f);
	     sDisk.setOrigin (16.f, 16.f);
	   sGround.setOrigin (16.f, 16.f);  
	sEdgeWorld.setOrigin (16.f, 16.f);
	sTopGround.setOrigin (16.f, 16.f);

	//////////////////////////////////////////////
	CreateRandWorld();			 //������� ���

	Mob mob1(800.f, 50.f, SCALE, World, IntRect(0, 32, 32, 64)); mob1.isControl = 1; //������� ������� ���� ������������
	Mob mob2(900.f, 50.f, SCALE, World, IntRect(0, 32, 32, 64)); mob2.isControl = 0; //������� ������� ���� �� ������������

	//mob1.InvetoryAdd(43, 34);
	//Clock clock; //������� ���������� �������, �.�. �������� �� �������(� �� �������������/�������� ����������).

	float innertCntX = 2;
	float innertCntY = 2;

	float innertIncreaseX = 2;
	float innertIncreaseY = 2;

	std::ostringstream textRenderBuff;

	while (window.isOpen()) { 	
 
	eventsOn();	   	
	autoResize();  
	
	//float time = clock.getElapsedTime().asMicroseconds(); //������ ���������� ������� � �������������
	//clock.restart(); //������������� �����
	//time = time / 800; //�������� ����	

	World.Step( (1.f / optimaFPS), 8, 3);
	window.clear(Color(181,228,240,1));

	for (b2Body* it = World.GetBodyList(); it != 0; it = it->GetNext()) {//���� ������� ����� ������������ ������ �� ������� �� ���� it->GetUserData() == "box"
		if (it->GetUserData() == "box"      ) { drawSprite(sBox      , it->GetPosition(), it->GetAngle() ); }
		if (it->GetUserData() == "disk"     ) { drawSprite(sDisk     , it->GetPosition(), it->GetAngle() ); }
		if (it->GetUserData() == "EdgeWorld") { drawSprite(sEdgeWorld, it->GetPosition(), it->GetAngle() ); }
		if (it->GetUserData() == "Ground"   ) { drawSprite(sGround   , it->GetPosition(), it->GetAngle() ); }
		if (it->GetUserData() == "TopGround") { drawSprite(sTopGround, it->GetPosition(), it->GetAngle() ); }
	}	  
	mob1.move();
	mob2.move();
	//	if ((Keyboard::isKeyPressed(Keyboard::Right)) && (isControl)) { mob1.move(3); }
	//	if ((Keyboard::isKeyPressed(Keyboard::Left) ) && (isControl)) { mob1.move(4); }
	//	if ((Keyboard::isKeyPressed(Keyboard::Up)   ) && (isControl)) { mob1.move(2); }
	//	if ((Keyboard::isKeyPressed(Keyboard::Down) ) && (isControl)) { mob1.move(1); }	  	  

	//�������������� ������
	//if (mob1.ox > view.getCenter().x > -100) {}
	//if (mob1.oy > view.getCenter().y > -100) {}                                                      float
	//if (mob1.ox < view.getCenter().x > -100) {}													   innertCnt, 
	//if (mob1.oy < view.getCenter().y > -100) {}                                                      param = 3.14159265; fractpart = modf(param, &intpart);	printf("%f = %f + %f \n", param, intpart, fractpart);

		
	//if (mob1.ox >= view.getCenter().x) {innertCntX = mob1.ox - view.getCenter().x;}
	//if (mob1.oy >= view.getCenter().y) {innertCntY = mob1.oy - view.getCenter().y;}
	//if (mob1.ox <= view.getCenter().x) {innertCntX = view.getCenter().x - mob1.ox;}
	//if (mob1.oy <= view.getCenter().y) {innertCntY = view.getCenter().y - mob1.oy;}
	//������� �������������                                                                                                          ���� �������� ������ ����� ��� ����� ����� , �������� ��������
	///if (mob1.ox >= view.getCenter().x + innertCntX) { setCamCenter(view.getCenter().x + innertCntX, view.getCenter().y); }
	///if (mob1.oy >= view.getCenter().y + innertCntY) { setCamCenter(view.getCenter().x             , view.getCenter().y + innertCntY); }
	///if (mob1.ox <= view.getCenter().x - innertCntX) { setCamCenter(view.getCenter().x - innertCntX, view.getCenter().y); }
	///if (mob1.oy <= view.getCenter().y - innertCntY) { setCamCenter(view.getCenter().x             , view.getCenter().y - innertCntY); }
	///innertIncreaseX = innertCntX *2.5;
	///innertCntX = 2 * 2.5;//floor(innertIncreaseX);
	///innertIncreaseY = innertCntY *2.5;
	///innertCntY = 2 * 2.5;//floor(innertIncreaseY);	 

	//innertCnt += 0.9f;
	//����� �������
	//if (mob1.ox == view.getCenter().x) innertCntX = 2;
	//if (mob1.oy == view.getCenter().y) innertCntY = 2;

	      if (mob1.isSelected()){Camera.setCamCenterOn(mob1.mobPos.x, mob1.mobPos.y);
	}else if (mob2.isSelected()) Camera.setCamCenterOn(mob2.mobPos.x, mob2.mobPos.y);

	mob1.update(window, SCALE, DEG);
	if (Mouse::isButtonPressed(Mouse::Button::Left)) {
	Vector2i mouseCoord = Mouse::getPosition(window);
	mouseCoord.x += Camera.getCenterX() - (Camera.getSizeX() / 2);
	mouseCoord.y += Camera.getCenterY() - (Camera.getSizeY() / 2);
	if (IntRect(mob1.mobPos.x - 8, mob1.mobPos.y - 16,
				mob1.mobPos.x + 8, mob1.mobPos.y + 16).contains(mouseCoord)
		) mob1.select();
	if (IntRect(mob2.mobPos.x - 8, mob2.mobPos.y - 16,
				mob2.mobPos.x + 8, mob2.mobPos.y + 16).contains(mouseCoord)
		) mob2.select();		//�������� �������� �������� �� ���
	}
	mob2.patrul(600, 1800, SCALE);
	mob2.update(window, SCALE, DEG);
	
	Camera.KeyboardControl(window);
	//{start debug section 
	textRenderBuff.str("");								    // ������ �����
	textRenderBuff << "zoomSetX " << Camera.debug_txt_Width  << "\n" 	   // � ���������� ������� ���������� ����������
		           << "zoomSetY " << Camera.debug_txt_Height << "\n"
		           << "zoomCnt "  << Camera.getZoomCnt() << "\n"
				   << "mob1.mobPos.x " << mob1.mobPos.x << "\n"
				   << "mob1.mobPos.y " << mob1.mobPos.y << "\n"			
				   << "drawtxt3 " << drawtxt3 << "\n";
	text.setString(textRenderBuff.str());		    // ��������� �������� ������������ � ������
	text.setPosition(mob1.mobPos.x - 30, mob1.mobPos.y - 160); // �������� ������� ������ �� ������
	window.draw(text);					          // ������������ �����
	//}end debug section 
	Camera.renderAll(window);
	}  
	return 0;
} 
