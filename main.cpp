/*
 * main.cpp
 *
 *  Created on: 28 de Mai de 2014
 *      Author: V�nia
 */

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "Unit.h"
#include <cstdlib>
#include <ctime>
#include <sstream>
const float FPS = 60;
const int SCREEN_W = 512;
const int SCREEN_H = 640;
const string ABC = "ABCDEFGHIJLKMNOPQRSTUVWXYZ";
const int BOUNCER_SIZE = 32;
int pontos = 0;
bool easy = true;
enum MYKEYS {
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

using namespace std;

const int altura = 20;
const int largura = 16;
vector<string> WORDS;
vector<Unit*> pecas;
bool key[4] = { false, false, false, false };

char Tetris[altura][largura];

void load() {
	ifstream file;
	file.open("dic2.txt");
	string tmp;
	while (file.good()) {
		tmp.clear();
		file >> tmp;
		WORDS.push_back(tmp);
	}

}
int cmpfunc(const void * a, const void * b) {
	return (*(char*) a - *(char*) b);
}

void init() {
	for (int i = 0; i < altura; i++)
		for (int j = 0; j < largura; j++)
			Tetris[i][j] = ' ';
}

void kmp_table(string W, int *T) {

	int pos = 2;
	int cnd = 0;

	T[0] = -1;
	T[1] = 0;

	while (pos < W.size()) {

		if (W[pos - 1] == W[cnd]) {
			cnd++;
			T[pos] = cnd;
			pos++;
		} else if (cnd > 0) {
			cnd = T[cnd];

		} else {
			T[pos] = 0;
			pos++;
		}

	}

}

int anag_search(string S, string W) {
	char* tmp = new char[W.length()];
	char* tmp2;
	int smE = 0, smA = 0;
	for (int j = 0; j < W.length(); j++) {

		tmp[j] = W[j];
	}
	qsort(tmp, W.length(), sizeof(char), cmpfunc);

	for (int j = 0; j < W.length(); j++) {
		smE = tmp[j] + smE * 37;

	}

	int r = S.length();
	int s = W.length();
	int l = r + s + 1;
	for (int m = 0; m < l; m++) {
		smA = 0;
		if (S[m] != ' ') {
			tmp2 = new char[W.length()];
			for (int i = 0; i < W.length(); i++) {
				tmp2[i] = S[m + i];
			}

			qsort(tmp2, W.length(), sizeof(char), cmpfunc);

			for (int i = 0; i < W.length(); i++) {
				if (tmp2[i] == ' ')
					break;

				smA = tmp2[i] + smA * 37;

			}
			if (smA == smE) {
				cout << "deleting word : " << W << endl;
				cout << "returning m with : " << m << endl;
				return m;
			}
		}
	}
	return -1;
}

/**
 * @Param S text to be searched
 * @Param W word to look for
 *
 * @return 	position in S where W is found
 */
int kmp_search(string S, string W) {

	int m = 0;
	int i = 0;
	int* T = new int[W.size()];
	kmp_table(W, T);

	while ((m + i) < S.size()) {
		if (W[i] == S[m + i]) {
			if (i == W.size() - 1)
				return m;

			i++;
		} else {
			if (T[i] > -1) {
				i = T[i];
				m = m + i - T[i];

			} else {
				i = 0;
				m++;
			}
		}

	}
	return -1;
}

void deleteUnit(int line, int collumn) {
	cout << "deleting :" << line << " , " << collumn << endl;
	int idel = -1;
	for (int i = 0; i < pecas.size(); i++) {
		if (pecas[i]->getTeX() == collumn && pecas[i]->getTeY() == line) {
			idel = i;
			break;
		}
	}
	if (idel != -1) {
		al_destroy_bitmap(pecas[idel]->bouncer);
		pecas.erase(pecas.begin() + idel);
	}
}

bool contem(vector<char> c, char ch) {
	for (int i = 0; i < c.size(); i++)
		if (ch == c[i])
			return true;
	return false;
}

void check(int line) {
	//passing the line to a string
	char* e = new char[largura];
	for (int i = 0; i < largura; i++) {
		e[i] = Tetris[line][i];
	}
	string str(e);

	cout << "Read the line : " << line << " with :" << str << endl;
	vector<char> ch;
	for (int i = 0; i < largura; i++) {
		if (!contem(ch, str[i]))
			if (str[i] != ' ')
				ch.push_back(str[i]);
	}

	string word;
	int ind;
	for (int i = 0; i < WORDS.size(); i++) {
		word = WORDS[i];
		if (word.size() > 2) {
			if (contem(ch, word[0])) {
				if (easy)
					ind = anag_search(str, word);
				else
					ind = kmp_search(str, word);
				if (ind != -1) {
					for (int j = 0; j < word.length(); j++) {
						pontos++;
						str[ind + j] = ' ';
						deleteUnit(line, ind + j);
					}
					for (int i = 0; i < largura; i++) {
						Tetris[line][i] = str[i];
					}
					return;
				}

			}
		}
	}

}

void checkVer(int column) {
	//passing the line to a string
	char* e = new char[altura];
	for (int i = 0; i < altura; i++) {
		e[i] = Tetris[i][column];
	}
	string str(e);

	cout << "Read the column : " << column << " with :" << str << endl;
	vector<char> ch;
	for (int i = 0; i < altura; i++) {
		if (!contem(ch, str[i]))
			if (str[i] != ' ')
				ch.push_back(str[i]);
	}

	string word;
	int ind;
	for (int i = 0; i < WORDS.size(); i++) {
		word = WORDS[i];
		if (word.size() > 2) {
			if (contem(ch, word[0])) {
				if (easy)
					ind = anag_search(str, word);
				else
					ind = kmp_search(str, word);
				if (ind != -1)
					for (int j = 0; j < word.length(); j++) {
						pontos++;
						str[ind + j] = ' ';
						deleteUnit(ind + j, column);
					}

			}
		}
	}

	for (int i = 0; i < largura; i++) {
		Tetris[i][column] = str[i];
	}

}

void printGame() {
	for (int i = 19; i >= 0; i--)
		for (int j = 0; j < largura; j++) {
			cout << Tetris[i][j];
			if (j == largura - 1)
				cout << "\n";
		}

}

void printPieces() {
	cout << "Current pieces in game: \n";
	for (int i = 0; i < pecas.size(); i++) {
		cout << pecas[i]->L[0] << endl;
	}
}

bool moveLeft(int i) {
	if (pecas[pecas.size() - 1]->getTeX() == 0)
		return false;
	if (key[KEY_LEFT] && pecas[i]->x >= 32
			&& Tetris[pecas[i]->getTeY()][pecas[i]->getTeX() - 1] == ' ') {
		return true;
	}
	return false;
}

bool moveRight(int i) {
	if (pecas[pecas.size() - 1]->getTeX() == 15)
		return false;
	if (key[KEY_RIGHT] && pecas[i]->x <= SCREEN_W - 32
			&& Tetris[pecas[i]->getTeY()][pecas[i]->getTeX() + 1] == ' ') {
		return true;
	}
	return false;
}

bool moveDown(int i, bool time) {
	if (pecas[i]->getTeY() == 0)
		return false;
	if ((key[KEY_DOWN] || time) && pecas[i]->y <= SCREEN_H - BOUNCER_SIZE - 32
			&& Tetris[pecas[i]->getTeY() - 1][pecas[i]->getTeX()] == ' ') {
		return true;
	}
	return false;
}

char RandomLetter() {

	int e = rand() % 26;
	return ABC[e];

}
bool end() {
	for (int i = 0; i < largura; i++) {
		if (Tetris[altura - 1][i] != ' ') {
			return true;
		}
	}
	return false;
}

int main() {
	srand(time(0));
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *botao1 = NULL;
	ALLEGRO_BITMAP *botao2 = NULL;
	bool doexit = false, mainMenu = true;
	bool redraw = true;
	bool addP = true;
	int cnt = 0, cnth = 30;
	int cntl = 0;

	init();
	al_init();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_keyboard();
	al_install_mouse();
	timer = al_create_timer(1.0 / FPS);
	display = al_create_display(SCREEN_W, SCREEN_H);
	ALLEGRO_FONT *font = al_load_ttf_font("pirulen.ttf", 32, 0);
	event_queue = al_create_event_queue();
	load();
	botao1 = al_create_bitmap(90, 50);
	botao2 = al_create_bitmap(90, 50);
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_set_target_bitmap(botao1);
	al_clear_to_color(al_map_rgb(10, 10, 10));
	string L = "Facil";
	al_draw_text(font, al_map_rgb(255, 255, 255), 45, 15, ALLEGRO_ALIGN_CENTRE,
			L.c_str());
	al_set_target_bitmap(botao2);
	al_clear_to_color(al_map_rgb(10, 10, 10));
	L = "Dificil";
	al_draw_text(font, al_map_rgb(255, 255, 255), 45, 15, ALLEGRO_ALIGN_CENTRE,
			L.c_str());
	al_set_target_bitmap(al_get_backbuffer(display));

	al_flip_display();

	al_start_timer(timer);

	ALLEGRO_EVENT ev;
	while (mainMenu) {
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_TIMER) {
			redraw = true;
		} else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			return 0;
		} else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			if (ev.mouse.x > 220 && ev.mouse.x < 270 && ev.mouse.y > 200
					&& ev.mouse.y < 250) {
				easy = true;
				mainMenu = false;
			}
			if (ev.mouse.x > 220 && ev.mouse.x < 270 && ev.mouse.y > 300
					&& ev.mouse.y < 350) {
				easy = false;
				mainMenu = false;
			}

		}

		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;

			al_clear_to_color(al_map_rgb(20, 40, 70));

			al_draw_bitmap(botao1, 220, 200, 0);
			al_draw_bitmap(botao2, 220, 300, 0);

			al_flip_display();
		}
	}

	while (!doexit) {

		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER) {
			cntl++;
			if (cntl > 65) {
				addP = true;
				Tetris[pecas[pecas.size() - 1]->getTeY()][pecas[pecas.size() - 1]->getTeX()] =
						pecas[pecas.size() - 1]->L[0];
				pecas[pecas.size() - 1]->fixed = true;

				check(pecas[pecas.size() - 1]->getTeY());
				checkVer(pecas[pecas.size() - 1]->getTeX());
				printGame();
			}
			if (addP) {
				pecas.push_back(new Unit(RandomLetter(), font));
				cntl = 0;
				addP = false;
			}
			cnth++;
			if (cnth > 10) {
				if (key[KEY_UP]) {
					printPieces();
				}

				if (moveDown(pecas.size() - 1, false)) {
					pecas[pecas.size() - 1]->y += 32.0;
					cnth = 0;
					cntl = 0;

				}

				if (moveLeft(pecas.size() - 1)) {
					pecas[pecas.size() - 1]->x -= 32.0;
					cnth = 0;
					cntl = 0;

				}

				if (moveRight(pecas.size() - 1)) {
					pecas[pecas.size() - 1]->x += 32.0;
					cnth = 0;
					cntl = 0;

				}
			}

			al_set_target_bitmap(al_get_backbuffer(display));

			if (cnt > 60) {
				for (int i = 0; i < pecas.size(); i++) {
					if (moveDown(i, true)) {
						//cout<<"moving piece in :"<<pecas[i]->getTeX()<<" , "<<pecas[i]->getTeY()<<" with letter :"<<pecas[i]->L[0]<<endl;
						if (pecas[i]->fixed)
							Tetris[pecas[i]->getTeY()][pecas[i]->getTeX()] =
									' ';
						pecas[i]->y += 32.0;
						if (pecas[i]->fixed)
							Tetris[pecas[i]->getTeY()][pecas[i]->getTeX()] =
									pecas[i]->L[0];
						cntl = 0;

					}

				}
				cnt = 0;
			}
			cnt++;
			if (end()) {
				doexit = true;
			} else
				redraw = true;

		} else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			doexit = true;
		} else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key[KEY_UP] = true;
				break;

			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = true;
				break;

			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = true;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = true;
				break;
			}
		} else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key[KEY_UP] = false;
				break;

			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = false;
				cnth = 10;
				break;

			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = false;
				cnth = 10;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = false;
				cnth = 10;
				break;

			case ALLEGRO_KEY_ESCAPE:
				doexit = true;
				break;
			}
		}

		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;

			al_clear_to_color(al_map_rgb(0, 0, 0));
			for (int i = 0; i < pecas.size(); i++) {
				al_draw_bitmap(pecas[i]->bouncer, pecas[i]->x, pecas[i]->y, 0);
			}
			string tr = "Game Over";
			stringstream ss;
			ss << "Pontos :";
			ss << pontos;
			al_set_target_bitmap(al_get_backbuffer(display));

			al_draw_text(font, al_map_rgb(255, 255, 255), 400, 20,
					ALLEGRO_ALIGN_CENTRE, ss.str().c_str());

			al_flip_display();
		}
	}
	string tr = "Game Over";
	stringstream ss;
	ss << "Pontos :";
	ss << pontos;
	al_set_target_bitmap(al_get_backbuffer(display));
	cout << "pontos: " << pontos << endl;
	al_clear_to_color(al_map_rgb(255, 0, 0));
	al_draw_text(font, al_map_rgb(255, 255, 255), 250, 230,
			ALLEGRO_ALIGN_CENTRE, tr.c_str());
	al_draw_text(font, al_map_rgb(255, 255, 255), 250, 280,
			ALLEGRO_ALIGN_CENTRE, ss.str().c_str());

	al_flip_display();
	al_rest(10.0);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}
