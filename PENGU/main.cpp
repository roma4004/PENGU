#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h> 
//#include <iostream>
#include <sstream>     //use in DrawText(...){...}
//#include <stdio.h>
#include <stdlib.h>
#pragma hdrstop	       //указывает что файлы выше общие для всех файлов и не нужндаются в перекомпиляции, в итоге ускоряет комипиляцию
#include "Mob.h"
#include "map.h"
#include "view.h"
#include "main.h"
using namespace sf;	 
void CreateRandWorld() {
	int OfMountains[30][2], start; // всего 30 гор по 2 параметра 0-координа по X, 1- высота горы

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
	srand(time(0));
	OfMountains[0][0] = - 10;                  // принудительная гора с левой части карты,
	OfMountains[0][1] = (rand() % 12) + 10;   //  и рандом высоты для этой горы.
	OfMountains[1][0] = WIDTH_MAP - 10;      // принудительная гора с правой части карты, 
	OfMountains[1][1] = (rand() % 12) + 10; //  и рандом высоты для этой горы.
	for (int i = 2; i < 30; i++) {                // задаем случайные параметры горам для 30 гор
		OfMountains[i][0] = rand() % WIDTH_MAP;  // координата по х
		OfMountains[i][1] = (rand() % 12) + 10; // поправка высоты исключает горы высотой меньше поправки
	}															                 // отрисовываем гору																  
	for (int count = 0; count < 30; count++)                                    // проходимся по каждой горе
		for (int h = 0; h <= OfMountains[count][1]-4; h++)                     // рисуем гору полосками начиная с самой нижней полоски и заканчивая (высота-4) чтобы исключить острый конец горы и добавить плавности (count переключает горы)
			for (int k = 0; k <= (OfMountains[count][1] * 2 - (h * 2)); k++) {// самое интересное каждая полоска должна закончиться раньше в прогрессии.
				start = OfMountains[count][0] + h + k;                       // считаем начало прорисовки полоски для каждой полоски смещаем старт на 1 блок вправа что была ступенька
				if (start >= WIDTH_MAP) start = WIDTH_MAP - 1;           // проверяем выход за край справа	  					
				if (start <= 0        ) start = 1;	                    // проверяем выход за край слева 												 
				TileMap[(HEIGHT_MAP - 5) - h][start] = '3'; // собственно заливаем 3-йками
			} 	
	for (int i = 0; i < HEIGHT_MAP; i++)							   //создаем мир согласно сгенерированной карте
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
				//if (W>H) 1024/768    =1, 333333333333333		1024/768    =1.3333...	
				//if (H>W)  768/1024   =0.75			1024/х	    =0.75		
				//if (W>H)	 W/H						 х=	Wn*ratio
				//if (H>W)	 H/W
				
				// 	     800>600	ratio = 800/600 = 1.333333333333333
				//	     1024>768	ratio = 1024/768 = 1.333333333333333
				//     if (W > H) { ratio = W / H; }			  //если увеличиваем ширину допустим на	W=W+100; то пропорциональный размер высоты  H=(W+100)*(W/H);
				//else if (H > W) { ratio = H / W; }			  //если увеличиваем высоту	допустим на H=H+100; то пропорциональный размер высоты  W=(H+100)*(H/W);
			    //  	 768>1024	ratio = 768/1024 = 0.75
				//	     600>800	ratio = 600/800  = 0.75	
																 
				//if ((zoomCnt > -19)&&(e.mouseWheel.delta == -2)) {/*view.zoom(1.40f);*/view.reset(FloatRect(0.f, 0.f, W + 100.f, (W + 100.f)*(H / W) )); zoomCnt-=2;}	//Отдаление с одинарным шагом		
				//if ((zoomCnt > -20)&&(e.mouseWheel.delta == -1)) {/*view.zoom(1.20f);*/view.reset(FloatRect(0.f, 0.f, W +  50.f, (W +  50.f)*(H / W) )); zoomCnt--; }	//Отдаление с двойным шагом			
				//if ((zoomCnt <  10)&&(e.mouseWheel.delta ==  1)) {/*view.zoom(0.80f);*/view.reset(FloatRect(0.f, 0.f, W -  50.f, (W -  50.f)*(H / W) )); zoomCnt++; }	//Приближение с одинарным шагом		
				//if ((zoomCnt <   9)&&(e.mouseWheel.delta ==  2)) {/*view.zoom(0.60f);*/view.reset(FloatRect(0.f, 0.f, W - 100.f, (W - 100.f)*(H / W) )); zoomCnt+=2;}	//Приближение с двойным шагом	
		}  
	}		
	if (Mouse::isButtonPressed(Mouse::Button::Middle)) { view.reset(FloatRect(0.f, 0.f, window.getSize().x, window.getSize().y)); zoomCnt = 0; }
}
void setZoomRate(float W, float H, int wheelDelta) {
	float zoomRate = 50.f * wheelDelta;               //шаг смещени множим на кол-во смещений, прилетают значения целые в диапазон -2..2
	if ((zoomCnt+wheelDelta >= -20) && (zoomCnt+wheelDelta <= 10) ) {
		view.reset(FloatRect(0.f, 0.f, W - zoomRate, (W - zoomRate)*(H / W) ));
	 zoomCnt = zoomCnt + wheelDelta;
	}
	drawtxt = W; 
	drawtxt2 = H;	
//	view.reset(FloatRect(0.f, 0.f, W + 100.f, (W + 100.f)*(H / W)));
}
int main(){

	window.setFramerateLimit(60);                   // обязательно надо сделать что бы настройках можно было задать желаемы макс фпс
	window.setVerticalSyncEnabled(true);		   //  так же должно управляться с настроек	
	view.reset(FloatRect(0.f, 0.f, DefWinSizeX, DefWinSizeY));// устанавливаем начальный размер камеры 800х600px   

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
	
	CreateRandWorld();			 //создаем мир	

	Mob mob1(800.f, 50.f);	   //создаем первого моба управляемого
	Mob mob2(900.f, 50.f);	   //создаем второго моба не управляемого

	//mob1.InvetoryAdd(43, 34);
	Clock clock; //создаем переменную времени, т.о. привязка ко времени(а не загруженности/мощности процессора).
		float winSizeX = 0;
		float winSizeY = 0;

	while (window.isOpen()) {
		/*autoResize(); переделать на функцию void autoResize(){

	     } 	 	
	    */	
		winSizeX = window.getSize().x;
		winSizeY = window.getSize().y;		
		if ( (winSizeX != winSizeX) || (winSizeY != winSizeY) ) {				
			float zoomSetX = 0;
			float zoomSetY = 0;
			float rate = 0;                   		 
			if (       winSizeX > winSizeY) {	     
				 rate = winSizeY / winSizeX;		 
			 zoomSetX = winSizeX;	                 
			 zoomSetY = zoomSetX * rate;			 
			}										 
			else {    
				 rate = winSizeX / winSizeY;
			zoomSetX = zoomSetY * rate;
			zoomSetY = winSizeY;		 
			}							 
		view.reset(FloatRect(0.f, 0.f, zoomSetX, zoomSetY));
		drawtxt = zoomSetX;
		drawtxt2 = zoomSetY;
		}
		//view.reset(FloatRect(0.f, 0.f, window.getSize().x, window.getSize().y));           (W + 100.f)*(H / W) 

		eventsOn();	   	

		float time = clock.getElapsedTime().asMicroseconds(); //запись прошедшего времени в микросекундах
	    clock.restart(); //перезагружает время
		time = time / 800; //скорость игры	

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