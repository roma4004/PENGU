#pragma once 
#include <SFML/Graphics.hpp>

class Cam
{
	int zoomCnt = 0, maxZoom = 10, minZoom = -20;  //zoomCnt - ������� ������� �����������/���������, ������������(maxZoom) � �����������(minZoom) ������� �����������
	float zoomRate = 0, zoomfactor = 1;

public:
	float debug_txt_Width, debug_txt_Height;
	Cam();
	~Cam();

	void viewMove(sf::RenderWindow &window);
	void reset(sf::FloatRect rectangle);

	void setCamCenterOn(float coordX, float coordY);
	void setZoomRate(float Width, float Height, int wheelDelta);
	int getZoomCnt();
	void setZoomCnt(int setArg);
	float getCenterX();
	float getCenterY();
	float getSizeX();
	float getSizeY();
	void renderAll(sf::RenderWindow &window);
	void setZoomDelta(int delta);
};

