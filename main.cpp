/*
 * main.cpp
 *
 *  Created on: 28 de Mai de 2014
 *      Author: Vânia
 */

#include <string>
#include <iostream>

using namespace std;

const int altura = 20;
const int largura = 16;

char Tetris[altura][largura];

void init(){
	for(int i=0;i<altura;i++)
		for(int j=0;j<largura;j++)
			Tetris[i][j]=' ';
}

void printGame(){
	for(int i=0;i<altura;i++)
		for(int j=0;j<largura;j++){
			if (j==largura-1)
				cout << "\n";
			cout << Tetris[i][j];
		}

}

void printMenu(){
	cout << "********************";
	cout << "     1.Jogar";
	cout << "     2.Sair";



}

int main(){
	init();
	printGame();


	return 0;
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
