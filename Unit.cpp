/*
 * Unit.cpp
 *
 *  Created on: May 29, 2014
 *      Author: pim
 */

#include "Unit.h"
#include <iostream>
using namespace std;
Unit::Unit(){

}
Unit::Unit(char Le, ALLEGRO_FONT *font) {
	this->font=font;

	cout<<"Le : "<<Le<<"\n";

	L =" ";
	L[0]=Le;
	cout<<"L : "<<L<<"\n";
	SIZE = 32;
	x=96;
	y=0;
	 bouncer = al_create_bitmap(SIZE,SIZE);
	al_set_target_bitmap(bouncer);

	al_clear_to_color(al_map_rgb(30, 232, 210));
	al_draw_text(font, al_map_rgb(255,255,255), 16, 0,ALLEGRO_ALIGN_CENTRE, L.c_str());

}

Unit::~Unit() {
	// TODO Auto-generated destructor stub
}

