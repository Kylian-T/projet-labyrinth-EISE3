#ifndef __FONCTION_LABYRINTH_
#define __FONCTION_LABYRINTH__

typedef struct {
	int N, E, S, W, item;
} t_tile;

typedef struct {

	char nom[50];
	int taille_x;
	int taille_y;
	int posJoueur_x;
	int posJoueur_y;
	int posOrdi_x;
	int posOrdi_y;

} t_jeu;

void jouerCoupManuel(int *coteInsert, t_move *moveJoueur, t_return_code *moveFin);

int coupFinal(t_return_code moveFin);

void majLab(t_tile *lab, int *labInit, int taille_x, int taille_y, t_move move, t_tile tuileExtra);

void jouerCoupAuto(t_tile *lab, t_jeu jeu, int *coteInsert, t_move *moveJoueur, t_return_code *moveFin, t_tile tuileExtra);

int plusCourtChemin(t_tile *lab, int x, int y, int x2, int y2, int taille_x, int taille_y, int *xsuiv, int *ysuiv);

int DFS(t_tile *lab, int *visite, int *chemin, int x, int y, int x2, int y2, int taille_x, int taille_y);

void copierLab(t_tile *lab, t_tile *labTemp, int taille_x, int taille_y);

#endif