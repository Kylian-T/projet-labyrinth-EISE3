#include "labyrinthAPI.h"
#include "clientAPI.h"
#include <stdlib.h>
#include <stdio.h>
#include "fonction_labyrinth.h"


void jouerCoupManuel(int *coteInsert, t_move *moveJoueur, t_return_code *moveFin) {

	printf("De quel côté souhaitez vous insérer la tuile ? gauche = 0, droite = 1, haut = 2 ou bas = 3.\n");
	scanf("%d", coteInsert);
	moveJoueur->insert = *coteInsert;
	printf("Saisir le numéro de la ligne ou de la colonne.\n");
	scanf("%d", &moveJoueur->number);
	printf("Saisir la rotation de la tile à insérer. (0 à 3 dans le sens des aiguilles d'une montre)\n");
	scanf("%d", &moveJoueur->rotation);
	printf("Saisir les coordonnées où vous souhaitez vous déplacer. (x y)\n");
	scanf("%d %d", &moveJoueur->x, &moveJoueur->y);
	*moveFin = sendMove(moveJoueur);
	return;
}

int coupFinal(t_return_code moveFin) {

	if (moveFin == WINNING_MOVE) {

			printf("Partie gagnée !\n");
			return 1;

	} else if (moveFin == LOOSING_MOVE) {

			printf("Partie perdue !\n");
			return -1;

	} return 0;

}

void majLab(t_tile *lab, int *labInit, int taille_x, int taille_y, t_move move, t_tile tuileExtra) {

	t_tile tuileRota = tuileExtra;
	switch(move.rotation) {

		case 1 :
			tuileRota.N = tuileExtra.W;
			tuileRota.E = tuileExtra.N;
			tuileRota.S = tuileExtra.E;
			tuileRota.W = tuileExtra.S;
		case 2 :
			tuileRota.N = tuileExtra.S;
			tuileRota.E = tuileExtra.W;
			tuileRota.S = tuileExtra.N;
			tuileRota.W = tuileExtra.E;
		case 3 :
			tuileRota.N = tuileExtra.E;
			tuileRota.E = tuileExtra.S;
			tuileRota.S = tuileExtra.W;
			tuileRota.W = tuileExtra.N;

	}

	int decalage;

	switch(move.insert) {

		case 0 :
			decalage = taille_x * (move.number);
			for (int i = (taille_x-1); i > 0; i--) {
				*(lab+(decalage+i)) = *(lab+(decalage+i-1));
			}
			*(lab+decalage) = tuileRota;
		case 1 :
			decalage = taille_x * (move.number);
			for (int i = 0; i < (taille_x-1); i++) {
				*(lab+(decalage+i)) = *(lab+(decalage+i+1));
			}
			*(lab+(decalage+taille_x-1)) = tuileRota;
		case 2 :
			decalage = move.number;
			for (int i = (taille_y-1); i > 0; i--) {
				*(lab+(i*taille_x + decalage)) = *(lab+((i-1)*taille_x + decalage));
			}
			*(lab+decalage) = tuileRota;
		case 3 :
			decalage = move.number;
			for (int i = 0; i < (taille_y-1); i++) {
				*(lab+(i*taille_x + decalage)) = *(lab+((i+1)*taille_x + decalage));
			}
			*(lab+(taille_x*(taille_y-1) + decalage)) = tuileRota;
	}

	int k = 0;

	for (int i = 0; i < taille_x; i++) {

		for (int j = 0; j < taille_y; j++) {

			*(labInit+k) = (lab+(taille_x*i + j))->N;
			*(labInit+k+1) = (lab+(taille_x*i + j))->E;
			*(labInit+k+2) = (lab+(taille_x*i + j))->S;
			*(labInit+k+3) = (lab+(taille_x*i + j))->W;
			*(labInit+k+4) = (lab+(taille_x*i + j))->item;
			k += 5;
		}

	}

	return;

}

// Copie un labyrinthe dans un autre
void copierLab(t_tile *lab, t_tile *labTemp, int taille_x, int taille_y) {

	for (int i = 0; i < taille_y; i++) {

		for (int j = 0; j < taille_x; j++) {

			*(labTemp+(i*taille_x + j)) = *(lab+(i*taille_x + j));

		}

	}
	return;

}

// Algorithme récursif de recherche en profondeur
// Renvoie 1 s'il existe un chemin entre les deux points, 0 sinon
int DFS(t_tile *lab, int *visite, int *chemin, int x, int y, int x2, int y2, int taille_x, int taille_y) {
    if (x == x2 && y == y2) {
        *(chemin+(x + y*taille_x)) = 1;
        return 1;
    }
    *(visite+(x + y*taille_x)) = 1;

    if (!(lab+(x + y*taille_x))->N && !*(visite+(x + (y - 1)*taille_x))) {
        if (DFS(lab, visite, chemin, x, y - 1, x2, y2, taille_x, taille_y)) {
            *(chemin+(x + y*taille_x)) = 1;
            return 1;
        }
    }
    if (!(lab+(x + y*taille_x))->S && !*(visite+(x + (y + 1)*taille_x))){
        if (DFS(lab, visite, chemin, x, y + 1, x2, y2, taille_x, taille_y)) {
            *(chemin+(x + y*taille_x)) = 1;
            return 1;
        }
    }
    if (!(lab+(x + y*taille_x))->W && !*(visite+(x - 1 + y*taille_x))) {
        if (DFS(lab, visite, chemin, x - 1, y, x2, y2, taille_x, taille_y)) {
            *(chemin+(x + y*taille_x)) = 1;
            return 1;
        }
    }
    if (!(lab+(x + y*taille_x))->E && !*(visite+(x + 1 + y*taille_x))) {
        if (DFS(lab, visite, chemin, x + 1, y, x2, y2, taille_x, taille_y)) {
            *(chemin+(x + y*taille_x)) = 1;
            return 1;
        }
    }
    return 0;
}

// Cherche le plus court chemin entre deux points données dans le labyrinthe
// Renvoie la taille du plus court chemin
int plusCourtChemin(t_tile *lab, int x, int y, int x2, int y2, int taille_x, int taille_y, int *xsuiv, int *ysuiv) {

	*xsuiv = x;
    *ysuiv = y;
    int tailleChemin = 0;

	// Variables pour stocker les cases visitées
	int *visite = malloc(taille_x*taille_y*sizeof(int));

	// Variables pour stocker le chemin parcouru
	int *chemin = malloc(taille_x*taille_y*sizeof(int));

	// Initialiser les variables
	for (int i = 0; i < taille_x; i++) {
	    for (int j = 0; j < taille_y; j++) {
            *(visite+(i + j*taille_x)) = 0;
            *(chemin+(i + j*taille_x)) = -1;
        }
    }

    if (DFS(lab, visite, chemin, x, y, x2, y2, taille_x, taille_y)) {

    	if (y > 0 && *(chemin+(x + (y-1)*taille_x)) && !(lab+(x + y*taille_x))->N && !(lab+(x + (y-1)*taille_x))->S) {

    		*xsuiv = x;
    		*ysuiv = y-1;

    	} 
    	if (y < taille_y && *(chemin+(x + (y+1)*taille_x)) && !(lab+(x + y*taille_x))->S && !(lab+(x + (y+1)*taille_x))->N) {

    		*xsuiv = x;
    		*ysuiv = y+1;
    		
    	} 
    	if (x > 0 && *(chemin+(x - 1 + y*taille_x)) && !(lab+(x + y*taille_x))->W && !(lab+(x - 1 + y*taille_x))->E) {

    		*xsuiv = x - 1;
    		*ysuiv = y;
    		
    	} 
    	if (x < taille_x && *(chemin+(x + 1 + y*taille_x)) && !(lab+(x + y*taille_x))->E && !(lab+(x + 1 + y*taille_x))->W) {

    		*xsuiv = x + 1;
    		*ysuiv = y;
    		
    	} 

    	for (int i = 0; i < taille_x; i++) {
	    	for (int j = 0; j < taille_y; j++) {
            	if (*(chemin+(i + j*taille_x)) == 1) {
            		tailleChemin++;
            	}
        	}
    	}

    	free(visite);
    	free(chemin);
    	return tailleChemin;

    }
	
	free(visite);
    free(chemin);
    return 99999;

}


// Détermine le coup que doit jouer l'ordi
void jouerCoupAuto(t_tile *lab, t_jeu jeu, int *coteInsert, t_move *moveJoueur, t_return_code *moveFin, t_tile tuileExtra) {

	t_move moveTemp;
	t_move bestMove;
	int tailleMin = 99999;
	int tailleChemin;
	t_tile *labTemp = malloc(jeu.taille_x*jeu.taille_y*sizeof(t_tile));
	int *labInit = malloc(jeu.taille_x*jeu.taille_y*sizeof(t_tile));
	int *xsuiv;
	int *ysuiv;

	int x2 = -1;
	int y2;

	for (int i = 0; i < jeu.taille_x; i++) {

		for (int j = 0; j < jeu.taille_y; j++) {

			if ((lab+(i + j*jeu.taille_x))->item == moveJoueur->nextItem) {

				x2 = i;
				y2 = j;

			}

		}

	}

	if (x2==-1) {
		x2 = 0;
		y2 = 1;
		moveJoueur->insert = 0;
		moveJoueur->number = 1;
		moveJoueur->rotation = 0;
		copierLab(lab, labTemp, jeu.taille_x, jeu.taille_y);
		majLab(labTemp, labInit, jeu.taille_x, jeu.taille_y, *moveJoueur, tuileExtra);
		plusCourtChemin(labTemp, jeu.posJoueur_x, jeu.posJoueur_y, x2, y2, jeu.taille_x, jeu.taille_y, xsuiv, ysuiv);
		moveJoueur->x = *xsuiv;
		moveJoueur->y = *ysuiv;
		*moveFin = sendMove(moveJoueur);
		free(labTemp);
		free(labInit);
		return;
	}

	copierLab(lab, labTemp, jeu.taille_x, jeu.taille_y);

	for (int i = 0; i < 4; i++) {

		if (i < 2) {

			moveTemp.insert = i;
			for (int j = 1; j < jeu.taille_y; j+=2) {

				moveTemp.number = j;
				for (int k = 0; k < 4; k++) {

					moveTemp.rotation = k;
					copierLab(lab, labTemp, jeu.taille_x, jeu.taille_y);
					majLab(labTemp, labInit, jeu.taille_x, jeu.taille_y, moveTemp, tuileExtra);
					tailleChemin = plusCourtChemin(labTemp, jeu.posJoueur_x, jeu.posJoueur_y, x2, y2, jeu.taille_x, jeu.taille_y, xsuiv, ysuiv);
					if (tailleChemin < tailleMin) {

						tailleMin = tailleChemin;
						bestMove.insert = i;
						bestMove.number = j;
						bestMove.rotation = k;
						bestMove.x = *xsuiv;
						bestMove.y = *ysuiv;

					}
				}

			}

		} else {

			moveTemp.insert = i;
			for (int j = 1; j < jeu.taille_x; j+=2) {

				moveTemp.number = j;
				for (int k = 0; k < 4; k++) {

					moveTemp.rotation = k;
					copierLab(lab, labTemp, jeu.taille_x, jeu.taille_y);
					majLab(labTemp, labInit, jeu.taille_x, jeu.taille_y, moveTemp, tuileExtra);
					tailleChemin = plusCourtChemin(labTemp, jeu.posJoueur_x, jeu.posJoueur_y, x2, y2, jeu.taille_x, jeu.taille_y, xsuiv, ysuiv);
					if (tailleChemin < tailleMin) {

						tailleMin = tailleChemin;
						bestMove.insert = i;
						bestMove.number = j;
						bestMove.rotation = k;
						bestMove.x = *xsuiv;
						bestMove.y = *ysuiv;

					}

				}

			}

		}

	}

	free(labTemp);
	free(labInit);
	moveJoueur->insert = bestMove.insert;
	moveJoueur->number = bestMove.number;
	moveJoueur->rotation = bestMove.rotation;
	moveJoueur->x = bestMove.x;
	moveJoueur->y = bestMove.y;
	*moveFin = sendMove(moveJoueur);
	return;

}