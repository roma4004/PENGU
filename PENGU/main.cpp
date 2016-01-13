#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h> 
//#include <iostream>
#include <sstream>     //use in DrawText(...){...}
//#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma hdrstop	       //указывает что файлы выше общие для всех файлов и не нужндаются в перекомпиляции, в итоге ускоряет комипиляцию
#include "Mob.h"
#include "map.h"
#include "view.h"
#include "main.h"
using namespace sf;	
void CreateRandWorld() {
	srand(time(0));
	int OfMountains[30][2], start,StartString; // всего 30 гор по 2 параметра 0-координа по X, 1- высота горы

	for (int i = HEIGHT_MAP - 1; i > HEIGHT_MAP - 5; i--) {// заливаем снизу текстурами 4 ряда блоков максимальной ширины от нижней точки полоски
		for (int j = 0; j < WIDTH_MAP; j++)
			TileMap[i][j] = '3';
	}
	// обводка карты
	for (int j = 0; j < WIDTH_MAP; j++) {        // обводим сверху, слева и справа
		TileMap[0][j] = '3';                    // полоска сверху 
		if (j < (HEIGHT_MAP - 1)) {            // так как карта прямоугольная надо проверять чтоб j не выходила за высоту иначе будет ошибка
			TileMap[j][0] = '3';              // полоска слева
			TileMap[j][WIDTH_MAP - 1] = '3'; // полоска справа
		}
	}
	OfMountains[0][0] = -10;                  // принудительная гора с левой части карты,
	OfMountains[0][1] = (rand() % 12) + 10;   //  и рандом высоты для этой горы.
	OfMountains[1][0] = WIDTH_MAP - 10;      // принудительная гора с правой части карты, 
	OfMountains[1][1] = (rand() % 12) + 10; //  и рандом высоты для этой горы.
	for (int i = 2; i < 30; i++) {                // задаем случайные параметры горам для 30 гор
		OfMountains[i][0] = rand() % WIDTH_MAP;  // координата по х
		OfMountains[i][1] = (rand() % 12) + 10; // поправка высоты исключает горы высотой меньше поправки
	}															                 // отрисовываем гору																  
	for (int count = 0; count < 30; count++)                                    // проходимся по каждой горе
		for (int h = 0; h <= OfMountains[count][1] - 4; h++)  {                   // рисуем гору полосками начиная с самой нижней полоски и заканчивая (высота-4) чтобы исключить острый конец горы и добавить плавности (count переключает горы)
			StartString = h + rand() % 2 ;									// переменная для смещения старта отрисовки чтобы не было 100% ёлочки 
			for (int k = 0; k <= (OfMountains[count][1] * 2 - (h * 2)); k++) {// самое интересное каждая полоска должна закончиться раньше в прогрессии.
				start = OfMountains[count][0] + StartString + k;                       // считаем начало прорисовки полоски для каждой полоски смещаем старт на 1 блок вправа что была ступенька
				if (start >= WIDTH_MAP-1) start = WIDTH_MAP - 2;           // проверяем выход за край справа	  					
				if (start <= 0) start = 1;	                    // проверяем выход за край слева 												 
				TileMap[(HEIGHT_MAP - 4) - h][start] = '2'; // собственно заливаем 2-йками
			}
}
	for (int i = 0; i < HEIGHT_MAP; i++)													//делаю верхушки с травой
		for (int j = 0; j < WIDTH_MAP; j++) {
			if ((TileMap[i][j] == '2') && (TileMap[i-1][j] == ' ')) { TileMap[i][j] = '1'; } // если я нашёл элемент горы "2" и над ним пусто значит это верхушка
		}

	for (int i = 0; i < HEIGHT_MAP; i++)							   //создаем мир согласно сгенерированной карте
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
			if (Mouse::isButtonPressed(Mouse::Left)) break;
		} 
		if (IntRect(100, 90, 300, 50).contains(Mouse::getPosition(window))) {	   // меняем заливку тектуры пункта меню 2 на синий при наведении
			menu2.setColor(Color::Blue);										  //  и если была нажата левая мыши то закрытие окна и цыкла, то есть выход из игры
			if (Mouse::isButtonPressed(Mouse::Left)) { window.close(); break; }
		} 
	window.draw(menu1);	    // отрисовка  
	window.draw(menu2);	   //  пунктов меню
	window.display();	  //   и их отображение
	}
}
int drawtxt = 0;    // переменные что потом 
int drawtxt2 = 0;  //  выводяться как текст
int zoomCnt = 0;  // позиция приблежения/отдаления
void eventsOn(){ 
	Event e;
	while (window.pollEvent(e)) { 
		switch (e.type) { 
			case Event::Closed         : window.close();	break;     	  //  закрываем окно если нажат крестик в углу окна  			 
			case Event::GainedFocus    : isControl = true;	break;       // получение фокуса включаем управление
			case Event::LostFocus      : isControl = false; break;      // потеря фокуса отключаем управление  			
			case Event::MouseWheelMoved: setZoomRate(view.getSize().x, view.getSize().y, e.mouseWheel.delta); break; //e.mouseWheel.delta - на сколько сместилось // e.mouseWheel.x - положение курсора по х курсора в момент смещения //e.mouseWheel.y - положение по у
		}  
	}		
	if (Mouse::isButtonPressed(Mouse::Button::Middle)) { view.reset(FloatRect(0.f, 0.f, window.getSize().x, window.getSize().y)); zoomCnt = 0; }
}
int maxZoomTop = 10;
int maxZoomBottom = -20;
void setZoomRate(float W, float H, int wheelDelta) {
	float zoomRate = 50.f * wheelDelta;               //шаг смещени множим на кол-во смещений, прилетают значения целые в диапазон -2..2
	if ((zoomCnt + wheelDelta >= maxZoomBottom) && (zoomCnt + wheelDelta <= maxZoomTop)) {
		view.reset(FloatRect(0.f,0.f, W - zoomRate, (W - zoomRate)*(H / W)));
		zoomCnt = zoomCnt + wheelDelta;
	}
	drawtxt = W;
	drawtxt2 = H;
}
float winSizeX = 0, winSizeY = 0;	 

void autoResize() { //надо дописать ограничение минимальный размер окна 640х480  например таким способом, и должен быть способ задавать без вектора	window.setSize(sf::Vector2u(640, 480));	   
	if (window.getSize().x <= 320) window.setSize(Vector2u(320, window.getSize().y));    // maxZoomTop = 5;
	if (window.getSize().y <= 240) window.setSize(Vector2u(window.getSize().x, 240)); //maxZoomBottom = 5;

	if ((window.getSize().x != winSizeX) || (window.getSize().y != winSizeY)) {
		winSizeX = window.getSize().x;							//получаем рамеры окна, для вычисления соотношения сторон			  
		winSizeY = window.getSize().y;

		view.reset(FloatRect(0.f, 0.f, winSizeX, winSizeY));

		int zoomDelta = zoomCnt;
		zoomCnt = 0;
		setZoomRate(winSizeX, winSizeY, zoomDelta);

		drawtxt = winSizeX;
		drawtxt2 = winSizeY;
	}
}
int main(){		   
	//window.setFramerateLimit(60);                   // обязательно надо сделать что бы настройках можно было задать желаемы макс фпс
	//window.setVerticalSyncEnabled(true);		   //  так же должно управляться с настроек	
	//view.reset(FloatRect(0.f, 0.f, DefWinSizeX, DefWinSizeY));// устанавливаем начальный размер камеры  
	//virtual void sf::Window::onResize()			возможно получится избежать использования ивентов еще и здесь 
   // virtual void sf::RenderWindow::onResize()		возможно получится избежать использования ивентов еще и здесь 

	//menu(window);  	
	/////////////////////Кусок текстур потом функцию надо сделать ////////////////////////
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
	CreateRandWorld();			 //создаем мир	

	Mob mob1(800.f, 50.f);	   //создаем первого моба управляемого
	Mob mob2(900.f, 50.f);	   //создаем второго моба не управляемого

	//mob1.InvetoryAdd(43, 34);
	//Clock clock; //создаем переменную времени, т.о. привязка ко времени(а не загруженности/мощности процессора).
		float winSizeX = 0;
		float winSizeY = 0;

		float innertCntX = 2;
		float innertCntY = 2;

		float innertIncreaseX = 2;
		float innertIncreaseY = 2;

		while (window.isOpen()) { 	
 
		eventsOn();	   	
		autoResize();  
		
		//float time = clock.getElapsedTime().asMicroseconds(); //запись прошедшего времени в микросекундах
	    //clock.restart(); //перезагружает время
		//time = time / 800; //скорость игры	

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

		setCamCenter(mob1.ox, mob1.oy);
		
		mob1.update();

		mob2.patrul(600,1800);
		mob2.update();
				
	    viewMove();
			
		//DrawText(18, mob1.ox - 30, mob1.oy - 120, "zoomSetX ", drawtxt);
		//DrawText(18, mob1.ox - 30, mob1.oy - 100, "zoomSetY ", drawtxt2);
		//DrawText(18, mob1.ox - 30, mob1.oy - 80,  "zoomCnt  ", zoomCnt);

		DrawText(18, mob1.ox - 30, mob1.oy - 120, "zoomSetX ", drawtxt);
		DrawText(18, mob1.ox - 30, mob1.oy - 100, "zoomSetY ", drawtxt2);
		DrawText(18, mob1.ox - 30, mob1.oy - 80, "zoomCnt  ", zoomCnt);
		DrawText(18, mob1.ox - 30, mob1.oy - 140, "mob1.ox  ", mob1.ox);
		DrawText(18, mob1.ox - 30, mob1.oy - 160, "mob1.oy  ", mob1.oy);

		window.setView(view);
		window.display();	   
		}  
	return 0;
}  
void DrawText(int fontSize, float posX, float posY, String setText, float addFloatToText,
	Color colorOfText, String FontFamily) {	                    // DrawText(18, 20, 100, L"Выводимый текст");
	Font font; String FontExt = ".ttf";
	font.loadFromFile("Fonts/" + FontFamily + FontExt);  // по полученому пути загружаем шрифт
	Text text("", font, fontSize);                      // по полученому пути загружаем шрифт
	text.setColor(colorOfText);                        // укаание цвет текста
	text.setStyle(sf::Text::Bold);                    // укаание жирности (или курсива) текста, по умолчанию обычный стиль
	std::ostringstream value;                        // создаем поток                                                  (нужно доработать, возможно работу с потоком можно убрать и конвертировать иным спосбом)
	value << addFloatToText;                        //  полученное значение в поток
	text.setString(setText+value.str());		   //   потоковое значение конвертируем в строку
	text.setPosition(posX, posY);				  // указание позиции текста на экране
	window.draw(text);							 // отрисовываем текст
}