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
		float viewOldTop  = view.getCenter().y - (view.getSize().y / 2);

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

	//{start debug section 
	debug_txt_Width = Width;
	debug_txt_Height = Height;
	//}end debug section 
}
int   Cam::getZoomCnt() {return zoomCnt;           };  
void  Cam::setZoomCnt(int setArg){zoomCnt = setArg;};
float Cam::getCenterX() {return view.getCenter().x;};
float Cam::getCenterY() {return view.getCenter().y;};
float Cam::getSizeX()   {return view.getSize().x;  };
float Cam::getSizeY()   {return view.getSize().y;  };
void  Cam::setZoomDelta(int delta) {
	setZoomRate(view.getSize().x,
				view.getSize().y,
				delta);
}
void Cam::renderAll(sf::RenderWindow &window) {
	window.setView(view);
	window.display();
}