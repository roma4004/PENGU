#include <sstream>     //use in DrawText(...){...}
#include <stdlib.h>
#pragma hdrstop	       //указывает что файлы выше общие для всех файлов и не нужндаются в перекомпиляции, в итоге ускоряет комипиляцию
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
	if (count - MountainTopDelete <= 0) return;
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
	
	// коробка мира
	for (int j = 0; j < WIDTH_MAP; j++) {
		TileMap[0][j] = '3';
		TileMap[HEIGHT_MAP - 1][j] = '3';
		if (j < (HEIGHT_MAP - 1)) {
			TileMap[j][0] = '3';
			TileMap[j][WIDTH_MAP - 1] = '3';
		}
	}
	// коробка мира Конец

	//создаем мир согласно сгенерированной карте
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
void menu(RenderWindow &window) {	//стоит вынести создание текстур в отдельный класс
	Texture menuTexture1, menuTexture2;//, menuTexture3, aboutTexture, menuBackground;
	menuTexture1.loadFromFile("images/111.png");
	menuTexture2.loadFromFile("images/333.png");
	Sprite menu1(menuTexture1), menu2(menuTexture2);

	menu1.setPosition(100.f, 30.f);		              // установка позиции,
	menu2.setPosition(100.f, 90.f);		             // для пунктов меню
	while (window.isOpen()){ 
		eventsOn();
		window.clear(Color(129,181,221));  // отчистка окна под меню	 
		menu1.setColor(Color::White);
		menu2.setColor(Color::White);
		if (IntRect(100, 30, 300, 50).contains(Mouse::getPosition(window))) {	  // меняем заливку тектуры пункта меню 1 на синий при наведении
			menu1.setColor(Color::Blue);									     //  и если была нажата левая мыши то завершение цыкла меню, то есть запуск игры
			if (Mouse::isButtonPressed(Mouse::Left)) break;	} 
		if (IntRect(100, 90, 300, 50).contains(Mouse::getPosition(window))) {	   // меняем заливку тектуры пункта меню 2 на синий при наведении
			menu2.setColor(Color::Blue);										  //  и если была нажата левая мыши то закрытие окна и цыкла, то есть выход из игры
			if (Mouse::isButtonPressed(Mouse::Left)) { window.close(); break; }
		} 
	window.draw(menu1);	    // отрисовка  
	window.draw(menu2);	   //  пунктов меню
	window.display();	  //   и их отображение
	}
}	
void eventsOn(){ 
	Event e;
	while (window.pollEvent(e)) { 
		switch (e.type) { 
			case Event::Closed         : window.close();	                      break;   //  закрываем окно если нажат крестик в углу окна  			 
			case Event::GainedFocus    : isWindowFocused = true;                  break;  // получение фокуса включаем управление
			case Event::LostFocus      : isWindowFocused = false;                 break; // потеря фокуса отключаем управление 
			case Event::MouseEntered   : isWindowFocused = true;                  break;
			case Event::MouseWheelMoved: Camera.setZoomDelta(e.mouseWheel.delta); break; //e.mouseWheel.delta - на сколько сместилось // e.mouseWheel.x - положение курсора по х курсора в момент смещения //e.mouseWheel.y - положение по у
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
void drawSprite(Sprite targetSprite, b2Vec2 pos, float angle)
{
	//while (angle <= 0 ) { angle += 360.f; }		  //the normalized angle if below 0
	//while (angle >=360) { angle -= 360.f; }		  //the normalized angle if above 360
	targetSprite.setPosition(pos.x*SCALE, pos.y*SCALE);
	targetSprite.setRotation(angle*DEG);
	window.draw(targetSprite);
}
int main(){		   
	//window.setFramerateLimit(optimaFPS);                   // обязательно надо сделать что бы настройках можно было задать желаемы макс фпс
	window.setVerticalSyncEnabled(true);		   //  так же должно управляться с настроек	
	//view.reset(FloatRect(0.f, 0.f, DefWinSizeX, DefWinSizeY));// устанавливаем начальный размер камеры  
	//virtual void sf::Window::onResize()			возможно получится избежать использования ивентов еще и здесь 
    // virtual void sf::RenderWindow::onResize()		возможно получится избежать использования ивентов еще и здесь 
	Font font; font.loadFromFile("Fonts/times.ttf");  // по полученому пути загружаем шрифт
	Text text("", font, 18);                      // по полученому пути загружаем шрифт
	text.setColor(Color::Black);                        // укаание цвет текста
	text.setStyle(sf::Text::Bold);                    // укаание жирности (или курсива) текста, по умолчанию обычный стиль
	
	//menu(window);  	
	/////////////////////Кусок текстур потом функцию надо сделать ////////////////////////
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
	CreateRandWorld();			 //создаем мир

	Mob mob1(800.f, 50.f, SCALE, World, IntRect(0, 32, 32, 64)); mob1.isControl = 1; //создаем первого моба управляемого
	Mob mob2(900.f, 50.f, SCALE, World, IntRect(0, 32, 32, 64)); mob2.isControl = 0; //создаем второго моба не управляемого

	//mob1.InvetoryAdd(43, 34);
	//Clock clock; //создаем переменную времени, т.о. привязка ко времени(а не загруженности/мощности процессора).

	float innertCntX = 2;
	float innertCntY = 2;

	float innertIncreaseX = 2;
	float innertIncreaseY = 2;

	std::ostringstream textRenderBuff;

	while (window.isOpen()) { 	
 
	eventsOn();	   	
	Camera.autoResize(window);
	
	//float time = clock.getElapsedTime().asMicroseconds(); //запись прошедшего времени в микросекундах
	//clock.restart(); //перезагружает время
	//time = time / 800; //скорость игры	

	World.Step( (1.f / optimaFPS), 8, 3);
	window.clear(Color(181,228,240,1));

	for (b2Body* it = World.GetBodyList(); it != 0; it = it->GetNext()) {//надо сделать метод возвращающий спрайт по запросу из боди it->GetUserData() == "box"
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

	//иннерционность камеры
	//if (mob1.ox > view.getCenter().x > -100) {}
	//if (mob1.oy > view.getCenter().y > -100) {}                                                      float
	//if (mob1.ox < view.getCenter().x > -100) {}													   innertCnt, 
	//if (mob1.oy < view.getCenter().y > -100) {}                                                      param = 3.14159265; fractpart = modf(param, &intpart);	printf("%f = %f + %f \n", param, intpart, fractpart);

		
	//if (mob1.ox >= view.getCenter().x) {innertCntX = mob1.ox - view.getCenter().x;}
	//if (mob1.oy >= view.getCenter().y) {innertCntY = mob1.oy - view.getCenter().y;}
	//if (mob1.ox <= view.getCenter().x) {innertCntX = view.getCenter().x - mob1.ox;}
	//if (mob1.oy <= view.getCenter().y) {innertCntY = view.getCenter().y - mob1.oy;}
	//плавное центрирование                                                                                                          надо отловить случай когда там целое число , допустим обрезать
	///if (mob1.ox >= view.getCenter().x + innertCntX) { setCamCenter(view.getCenter().x + innertCntX, view.getCenter().y); }
	///if (mob1.oy >= view.getCenter().y + innertCntY) { setCamCenter(view.getCenter().x             , view.getCenter().y + innertCntY); }
	///if (mob1.ox <= view.getCenter().x - innertCntX) { setCamCenter(view.getCenter().x - innertCntX, view.getCenter().y); }
	///if (mob1.oy <= view.getCenter().y - innertCntY) { setCamCenter(view.getCenter().x             , view.getCenter().y - innertCntY); }
	///innertIncreaseX = innertCntX *2.5;
	///innertCntX = 2 * 2.5;//floor(innertIncreaseX);
	///innertIncreaseY = innertCntY *2.5;
	///innertCntY = 2 * 2.5;//floor(innertIncreaseY);	 

	//innertCnt += 0.9f;
	//сброс инерции
	//if (mob1.ox == view.getCenter().x) innertCntX = 2;
	//if (mob1.oy == view.getCenter().y) innertCntY = 2;

	//may need friend func between mob & Cam
	//      if (mob1.isSelected()){Camera.setCamCenterOn(mob1.mobPos.x, mob1.mobPos.y);
	//}else if (mob2.isSelected()) Camera.setCamCenterOn(mob2.mobPos.x, mob2.mobPos.y);
	if (mob1.isSelected()){
		Camera.smoothMoveTo(mob1.mobPos.x, mob1.mobPos.y);
	}
	else if (mob2.isSelected()) {
		Camera.smoothMoveTo(mob2.mobPos.x, mob2.mobPos.y);
	}
	
	Vector2i mouseCoord = Mouse::getPosition(window);
	Vector2f ViewCenter = Camera.getViewCenter();
	
	mouseCoord.x += ViewCenter.x - (ViewCenter.x / 2);
	mouseCoord.y += ViewCenter.y - (ViewCenter.y / 2);
	mob1.update(window, SCALE, DEG, mouseCoord);
	
	mob2.patrul(600, 1800, SCALE);
	mob2.update(window, SCALE, DEG, mouseCoord);
	
	Camera.KeyboardControl(window);
	///{start debug section 
	textRenderBuff.str("");								    // чистим поток
	textRenderBuff << "zoomSetX " << Camera.debug_txt_Width  << "\n" 	   // и поочередно заносим отладочную информацию
		           << "zoomSetY " << Camera.debug_txt_Height << "\n"
		           << "zoomCnt "  << Camera.getZoomCnt() << "\n"
				   << "mob1.mobPos.x " << mob1.mobPos.x << "\n"
				   << "mob1.mobPos.y " << mob1.mobPos.y << "\n"			
				   << "drawtxt3 " << drawtxt3 << "\n";
	text.setString(textRenderBuff.str());		    // потоковое значение конвертируем в строку
	text.setPosition(mob1.mobPos.x - 30, mob1.mobPos.y - 160); // указание позиции текста на экране
	window.draw(text);					          // отрисовываем текст
	///}end debug section 
	Camera.renderAll(window);
	}  
	return 0;
} 
