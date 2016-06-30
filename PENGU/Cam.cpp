#include "Cam.h"
sf::View view;
Cam::Cam() {
	minWindowSize.x=640, 
	minWindowSize.y=480;
}  		 
Cam::~Cam(){
}
//////////////////////////////////////////////////////////////////////////////////////////
/// argument must be in vector format
////////////////////////////////////////////////////////////////////////////////////////// 
void Cam::setCamCenterOn(float coordX, float coordY) {
	view.setCenter(coordX, coordY);
}
void Cam::reset(sf::FloatRect rectangle) {
	view.reset(rectangle);
}	  
//b2Vec2 getB2Vec2FromVector2i(Vector2i vec2i) {
//	b2Vec2 vec2;
//	vec2.x = vec2i.x;
//	vec2.y = vec2i.y;
//	return vec2;
//}
void Cam::KeyboardControl(sf::RenderWindow &window) {
	//Event onFocusEvent;	//тут еще не доделан фокус, нужно как в том управлении игроком сделать
	//while (window.pollEvent(onFocusEvent)) {
	//if (onFocusEvent.type == Event::GainedFocus){		
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) view.move( 8.f,  0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) view.move(-8.f,  0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) view.move( 0.f, -8.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) view.move( 0.f,  8.f);
	//	}
	//}
}
void Cam::setZoomRate(float Width, float Height, int wheelDelta) {//шаг смещения умножаем на кол-во смещений, аргуметом присылают целые значения (wheelDelta)  в диапазоне -2..2

	if ((zoomCnt + wheelDelta >= minZoom) 	 // minZoom = -20;   
	&&  (zoomCnt + wheelDelta <= maxZoom)) { // maxZoom = +10;	   			
		zoomRate = 50.f * wheelDelta;
		float viewOldLeft = view.getCenter().x - (view.getSize().x / 2);
		float viewOldTop = view.getCenter().y - (view.getSize().y / 2);

		float viewWidth = Width - zoomRate;
		float viewHeight = viewWidth*(Height / Width);

		float viewTopOffset =  (Width  - viewWidth) / 2;
		float viewLeftOffset = (Height - viewHeight) / 2;

		float viewLeft = viewOldLeft + viewTopOffset;
		float viewTop  = viewOldTop  + viewLeftOffset;

		reset(sf::FloatRect(viewLeft, viewTop, viewWidth, viewHeight));
		zoomCnt += wheelDelta;	  
	}						  
	//if ((wheelDelta / 100) != 0) zoomfactor += (wheelDelta / 100);
	//	view.zoom(zoomfactor); 	   

	///{start debug section 
	debug_txt_Width = Width;
	debug_txt_Height = Height;
	///}end debug section 
}
int   Cam::getZoomCnt() {return zoomCnt;};  
void  Cam::setZoomCnt(int setArg) {zoomCnt = setArg;};
sf::Vector2f Cam::getViewCenter() {return view.getCenter();};
sf::Vector2f Cam::getViewSize() {return view.getSize();};
void  Cam::setZoomDelta(int delta) {
	setZoomRate(view.getSize().x,
				view.getSize().y,
				delta);
}
void Cam::renderAll(sf::RenderWindow &window) {
	window.setView(view);
	window.display();
}	
void Cam::checkOrSetMinSize(sf::RenderWindow &window) {	//may rename this func to MinSizeConstraints?
	sf::Vector2u sideSize = window.getSize();
	if (window.getSize().x <= minWindowSize.x) { 
		sideSize.x = minWindowSize.x; 
		window.setSize(sideSize);	
	}
	if (window.getSize().y <= minWindowSize.y) { 
		sideSize.y = minWindowSize.y; 
		window.setSize(sideSize);	
	}  	
}
void Cam::autoResize(sf::RenderWindow &window) {
	checkOrSetMinSize(window);

	if ((window.getSize().x != winSize.x)
	||  (window.getSize().y != winSize.y)) {
		winSize = window.getSize();				

		//view.reset(FloatRect(0.f, 0.f, winSizeX, winSizeY));
		//view.setViewport(sf::FloatRect(0.0f, 0.0f, 1.5f, 1.5f));
		//view.getViewport().
		int zoomDelta = getZoomCnt(); setZoomCnt(0);
		setZoomRate(window.getSize().x, window.getSize().y, zoomDelta);
		//setZoomRate(winSizeX, winSizeY, zoomDelta);
		///{start debug section 
		debug_txt_Width = winSize.x;
		debug_txt_Width = winSize.y;
		///}end debug section 
	}
}
float Cam::calcDistance(sf::Vector2f from, sf::Vector2f to) {
	return sqrt( pow( (to.x - from.x), 2)+ 
		         pow( (to.y - from.y), 2)
			   );
}
void Cam::smoothMoveTo(float targetX, float targetY){
	 sf::Vector2f setView; 	 
	 sf::Vector2f targetCoord; 	
	 setView = view.getCenter();
	 targetCoord.x = targetX;
	 targetCoord.y = targetY;
	 //надо будет сделать подгонку скорости, что бы обе координаты одновременно приходили к финишу центрирования
	// дистанцию делить на (60кадров*5секунд) и это будет равняться поправке которую 
   //  надо будет давать камере на каждом кадре,
  //   что бы обеспечить центрирование на любом расстоянии за 5 сек

	//разгон
	//inertForce.x *= 1.1f;
	//inertForce.y *= 1.1f;

	 //нормальная скорость при приближении
	//float dist;
	//dist = abs(calkDistance(setView, targetCoord));
	//if (dist <= 10) {
	//	inertForce.x = 3.5; 
	//	inertForce.y = 3.5;
	// };

	//if (finalCoord.x > currentViewCoord.x){inertForce.x = finalCoord.x - currentViewCoord.x / (60*5);}
	//if (finalCoord.x < currentViewCoord.x){inertForce.x = finalCoord.x + currentViewCoord.x / (60*5);}
	//if (finalCoord.y > currentViewCoord.y){inertForce.y = finalCoord.y - currentViewCoord.y / (60*5);}
	//if (finalCoord.y < currentViewCoord.y){inertForce.y = finalCoord.y + currentViewCoord.y / (60*5);}

	
	
					//200				  	 130		  10
	if ( targetX > (setView.x + inertForce.x) ) { setView.x += inertForce.x; }
	if ( targetY > (setView.y + inertForce.y) ) { setView.y += inertForce.y; }
	if ( targetX < (setView.x - inertForce.x) ) { setView.x -= inertForce.x; }
	if ( targetY < (setView.y - inertForce.y) ) { setView.y -= inertForce.y; }
	
	view.setCenter(setView.x, setView.y);
}
//плавное центрирование                                                                 надо отловить случай когда там целое число , допустим обрезать
///if (mob1.ox >= view.getCenter().x + innertCntX) { setCamCenter(view.getCenter().x + innertCntX, view.getCenter().y); }
///if (mob1.oy >= view.getCenter().y + innertCntY) { setCamCenter(view.getCenter().x             , view.getCenter().y + innertCntY); }
///if (mob1.ox <= view.getCenter().x - innertCntX) { setCamCenter(view.getCenter().x - innertCntX, view.getCenter().y); }
///if (mob1.oy <= view.getCenter().y - innertCntY) { setCamCenter(view.getCenter().x             , view.getCenter().y - innertCntY); }
///innertIncreaseX = innertCntX *2.5;
///innertCntX = 2 * 2.5;//floor(innertIncreaseX);
///innertIncreaseY = innertCntY *2.5;
///innertCntY = 2 * 2.5;//floor(innertIncreaseY);	 