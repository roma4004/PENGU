#pragma once

void CreateRandWorld();

void setObj(float x, float y, void *type);

void setBody(float x, float y, void *type);

void menu(sf::RenderWindow &window);

void eventsOn();

void drawSprite(sf::Sprite targetSprite, b2Vec2 pos, float angle);

float drawtxt = 0, drawtxt2 = 0, drawtxt3 = 0,
winSizeX = 0, winSizeY = 0;  // drawtxt - ���������� ����������; winSize - ������� ���� � �������� ����, use in autoResize

unsigned char optimaFPS = 60; 