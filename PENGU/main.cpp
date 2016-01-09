#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h> 
#include "Mob.h"
#include "map.h"
#include "view.h"
#include "main.h"
using namespace sf;	 
//пилотная версия мира
void CreateWorld() {
	int OfMountains[30][2], start; // всего 30 гор по 2 параметра 0-координа по X, 1- высота горы
	// заливаем снизу текстурой;
	for (int i = HEIGHT_MAP - 1; i > HEIGHT_MAP - 5; i--) {// заливаем пол текстурой 4 полоски
		for (int j = 0; j < WIDTH_MAP; j++)
			TileMap[i][j] = '3';
	}
	// обводка карты
	for (int j = 0; j < WIDTH_MAP; j++) {//обводим сверху, слева и справа
		TileMap[0][j] = '3';// полоска сверху 
		if (j < (HEIGHT_MAP - 1)) {//так как карта прямоугольная надо проверять чтоб j не выходила за высоту иначе будет ошибка
			TileMap[j][0] = '3';// полоска слева
			TileMap[j][WIDTH_MAP - 1] = '3'; // полоска справа
		}
	}
	//рандомим параметры горы координата и высота	
	srand(time(0)); // нужная штуковина
	for (int i = 0; i < 30; i++) {// для 30 гор под завязку массива
		OfMountains[i][0] = rand() % WIDTH_MAP; // координата по х
		OfMountains[i][1] = (rand() % 12) + 10; //высота +5 исключает горы высотой меньше 10
	}
	//отрисовываем гору
	for (int count = 0; count < 30; count++) // проходимся по каждой горе
		for (int h = 0; h <= OfMountains[count][1]-4; h++) // рисуем гору полосками начиная с самой нижней полоски и заканчивая (высота-4) чтобы исключить острый конец горы и добавить плавности (count переключает горы)
			for (int k = 0; k <= (OfMountains[count][1] * 2 - (h * 2)); k++) {// самое интересное каждая полоска должна закончиться раньше в прогрессии.
				start = OfMountains[count][0] + h + k;// считаем начало прорисовки полоски для каждой полоски смещаем старт на 1 блок вправа что была ступенька

				if (start >= WIDTH_MAP)// проверяем выход за край справа
					start = WIDTH_MAP - 1;
				if (start <= 0)// проверяем выход за край слева
					start = 1;
				TileMap[(HEIGHT_MAP - 5) - h][start] = '3'; // собственно заливаем 3-йками
			}
}// конец

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
	Texture menuTexture1, menuTexture2, menuTexture3, aboutTexture, menuBackground;
	menuTexture1.loadFromFile("images/111.png");
	menuTexture2.loadFromFile("images/333.png");
	Sprite menu1(menuTexture1), menu2(menuTexture2);
	bool isMenu = 0;							  //set 1 to on or set 0 to switch off
	int menuNum = 0;
	menu1.setPosition(100.f, 30.f);
	menu2.setPosition(100.f, 90.f);
while (isMenu){
		menu1.setColor(Color::White);
		menu2.setColor(Color::White);
		menuNum = 0;
		window.clear(Color(129, 181, 221));
		
		if ( IntRect(100, 30, 300, 50).contains(Mouse::getPosition(window)) ) {
			menu1.setColor(Color::Blue);
			menuNum = 1; 
		}
		if ( IntRect(100, 90, 300, 50).contains(Mouse::getPosition(window)) ) { 
			menu2.setColor(Color::Blue);
			menuNum = 2;   
		}	   		
		if (Mouse::isButtonPressed(Mouse::Left)){
			isMenu = false;	
			if (menuNum == 2) window.close();
		}
		window.draw(menu1);
		window.draw(menu2);

		window.display();
	}
}	
int main(){	
	menu(window);

	view.reset(FloatRect(0.f, 0.f, 800.f, 600.f));
	window.setFramerateLimit(60);
	Texture TextureMap;
	TextureMap.loadFromFile("images/tilemap.png");
	Sprite sMap(TextureMap), sPlayer(TextureMap), sBox(TextureMap), sDisk(TextureMap);

	sMap.setTextureRect (IntRect(32, 0, 32, 32)); 
	sBox.setTextureRect (IntRect(0,  0, 32, 32));
	sDisk.setTextureRect(IntRect(64, 0, 32, 32));
	sMap.setOrigin (16.f, 16.f);
	sBox.setOrigin (16.f, 16.f);
	sDisk.setOrigin(16.f, 16.f);
	CreateWorld();
	for (int i = 0; i < HEIGHT_MAP; i++)
		for (int j = 0; j < WIDTH_MAP; j++){
			if (TileMap[i][j] == '3') setObj( (j * 32.f), (i  * 32.f) );
		}		
	Mob mob1(800.f, 50.f);
	Mob mob2(900.f, 50.f);
	mob1.InvetoryAdd(43, 34);
	while (window.isOpen()) {
		Event e;
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed)
				window.close();	
		}		 
		World.Step( (1.f / 60.f) , 8, 3);
		window.clear(Color::White);

		if (Keyboard::isKeyPressed(Keyboard::Right)){ mob1.move(3); }
		if (Keyboard::isKeyPressed(Keyboard::Left)) { mob1.move(4); }
		if (Keyboard::isKeyPressed(Keyboard::Up))	{ mob1.move(2); }
		if (Keyboard::isKeyPressed(Keyboard::Down)) { mob1.move(1); }
		
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
				
	    //viewMove();
		DrawText(18, 20, 100, L"Выводимый текст");
		window.setView(view);
		window.display();
		}
	return 0;
}  
void DrawText(int fontSize, float posX, float posY, String setText,
	Color colorOfText, String FontFamily) {	                    // DrawText(18, 20, 100, L"Выводимый текст");
	Font font; String FontExt = ".ttf";
	font.loadFromFile("Fonts/" + FontFamily + FontExt);
	Text text("", font, fontSize);
	text.setColor(colorOfText);
	text.setStyle(sf::Text::Bold);
	text.setString(setText);
	text.setPosition(posX, posY);
	window.draw(text);
}