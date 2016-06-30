#pragma once 
#include <SFML/Graphics.hpp>
#include <cmath> //for func abc()	in 	smoothMove
class Cam
{
	int zoomCnt = 0, maxZoom = 10, minZoom = -20;  //zoomCnt - текущая позиция приблежения/отдаления, максимальная(maxZoom) и минимальная(minZoom) позиция приближения
	float zoomRate = 0, zoomfactor = 1;
	sf::Vector2u minWindowSize;	  		
	sf::Vector2u winSize; //winSize - Размеры окна с прошлого раза, use in autoResize
	sf::Vector2i inertForce = {2,2};
public:

	float debug_txt_Width, debug_txt_Height;
	Cam();
	~Cam();

	void KeyboardControl(sf::RenderWindow &window);
	void reset(sf::FloatRect rectangle);

	void setCamCenterOn(float coordX, float coordY);
	void setZoomRate(float Width, float Height, int wheelDelta);
	int getZoomCnt();
	void setZoomCnt(int setArg);
	sf::Vector2f getViewCenter();
	sf::Vector2f getViewSize();
	void renderAll(sf::RenderWindow &window);
	void setZoomDelta(int delta);
	void checkOrSetMinSize(sf::RenderWindow &window);
	void autoResize(sf::RenderWindow &window);
	void smoothMoveTo(float targetX, float targetY);
	float calcDistance(sf::Vector2f from, sf::Vector2f to);
};

