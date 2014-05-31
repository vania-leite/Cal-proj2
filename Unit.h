/*
 * Unit.h
 *
 *  Created on: May 29, 2014
 *      Author: pim
 */
#ifndef UNIT_H_
#define UNIT_H_
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <string>
using namespace std;
class Unit {
public:
	string L;
	ALLEGRO_BITMAP *bouncer = NULL;
	int SIZE;
	int x,y;
	bool fixed=false;
	ALLEGRO_FONT *font;
	Unit();
	Unit(string L,ALLEGRO_FONT *font);
	virtual ~Unit();
	int getX(){
		return x;
	}
	int getY(){
		return y;
	}
	int getTeX(){
		return x/SIZE ;
	}
	int getTeY(){
		return 20 - (y/SIZE) -1;
	}
};

#endif /* UNIT_H_ */
