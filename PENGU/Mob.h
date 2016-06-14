#pragma once 
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h> 
using namespace sf;		 
/// todo: сделать инвентарь в виде списка, в который потом заносить айди предмета и кол-во
		// int invMaxItem=10;		   
		// int invLimitEachItem=100;  	 
		// int *ptr_inventoryMas = new int[invMaxItem][invLimitEachItem]; // ¬ыделение пам€ти дл€ массива
		//	ptr_inventoryMas[0] = CntOfElem;
		//
		// for (int i=0; i<invMaxItem; i++) { 	// «аполнение массива и вывод значений его элементов
		// 
		//ptr_inventoryMas[i] = i;
		// cout << "Value of " << i << " element is " << p_darr[i] << endl;
		//}
		//delete[] p_darr; // очистка пам€ти

	////////////////////////////////////////////////////////////
	/// \objects like settlers...
	////////////////////////////////////////////////////////////
class Mob{ 
	private:				
		bool nav = true; 
		float mobAngle;
		bool isSelect;			       //mark which indicate mouse selected or not mob object
		int invCntElem = 0;		      //total counter of element in the inventory
		int invMaxItem = 10;         //max element of the inventory
		int invLimitEachItem = 10;  //max limit of each element in the inventory
		//int **ptr_inventoryMas = (int **)new int[invMaxItem][2];   		  
		//така€ запись позвол€ет объ€вл€ть массив через переменную длинну, только дл€ самого левого значение, остальные должны быть константными mas[perem][5]
		
		int inventoryMas[10][2] = { { },{ } }; //[0][0]=id of element; [0][1]=count of element	

		unsigned __int8 health;		//0..255	<stil not used> 
		unsigned __int8 armore;		//0..255	<stil not used>
		
		Texture mobTexture;
		Sprite mobSprite;
		b2Body *mpeople;
		b2PolygonShape PolygonShape;
		b2BodyDef BodyDef;
		b2FixtureDef FixtureDef;  		  
	public:	
		//View mobView;				
		b2Vec2 mobPos;
		Mob(float x, float y, float SCALE, b2World &World, IntRect rectangl);  //x,y-spawn coord; scale-  ; ;
	    ~Mob();			  			
		void InvetoryAdd(int idElem, int cntElem);
		void InvetoryDropAllElem();
		int InvetoryGetCntElem(int idElem);
		void update(RenderWindow &window, float SCALE, float DEG, Vector2i mouseCoord);
		void move();
		void patrul(int start, int end, float SCALE);
		void select();
		bool isSelected();
		bool isControl = false;	
};