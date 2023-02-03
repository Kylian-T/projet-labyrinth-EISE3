#include "labyrinthAPI.h"
#include "clientAPI.h"
#include <stdlib.h>
#include <stdio.h>
#include "fonction_labyrinth.h"


t_jeu jeu;


t_tile tileExtra;


int main() {
	
	int coteInsert;
	connectToServer("172.105.76.204", 1234, "KylianTardeDebug2");
	waitForLabyrinth("TRAINING DONTMOVE timeout=1000 margin=1 display=debug", jeu.nom, &(jeu.taille_x), &(jeu.taille_y));
	int *labInit = malloc(jeu.taille_x*jeu.taille_y*sizeof(t_tile));
	int tourAdverse = getLabyrinth(labInit, &tileExtra.N, &tileExtra.E, &tileExtra.S, &tileExtra.W, &tileExtra.item);

	t_tile *lab = malloc(jeu.taille_x*jeu.taille_y*sizeof(t_tile));

	int j = 0;

	for (int i = 0; i < jeu.taille_x*jeu.taille_y*5; i = i+5) {

		(lab+j)->N = labInit[i];
		(lab+j)->E = labInit[i+1];
		(lab+j)->S = labInit[i+2];
		(lab+j)->W = labInit[i+3];
		(lab+j)->item = labInit[i+4];
		j++;

	}

	t_move moveJoueur;
	t_move moveAdverse;
	t_return_code moveFin;

	if (tourAdverse) {
		jeu.posJoueur_x = (jeu.taille_x) - 1;
		jeu.posJoueur_y = (jeu.taille_y) - 1;
		jeu.posOrdi_x = 0;
		jeu.posOrdi_y = 0;
		printLabyrinth();
		moveFin = getMove(&moveAdverse);
		majLab(lab, labInit, jeu.taille_x, jeu.taille_y, moveAdverse, tileExtra);
		jeu.posOrdi_x = moveAdverse.x;
		jeu.posOrdi_y = moveAdverse.y;
	} else {
		jeu.posOrdi_x= (jeu.taille_x) - 1;
		jeu.posOrdi_y = (jeu.taille_y) - 1;
		jeu.posJoueur_x = 0;
		jeu.posJoueur_y = 0;
	}

	int partieEnCours = 0;

	printLabyrinth();

	while (partieEnCours == 0) {


		jouerCoupAuto(lab, jeu, &coteInsert, &moveJoueur, &moveFin, tileExtra);
		majLab(lab, labInit, jeu.taille_x, jeu.taille_y, moveJoueur, tileExtra);
		jeu.posJoueur_x = moveJoueur.x;
		jeu.posJoueur_y = moveJoueur.y;
		printLabyrinth();
		partieEnCours = coupFinal(moveFin);
		if (partieEnCours != 0) {

			break;

		}
		moveFin = getMove(&moveAdverse);
		majLab(lab, labInit, jeu.taille_x, jeu.taille_y, moveAdverse, tileExtra);
		jeu.posOrdi_x = moveAdverse.x;
		jeu.posOrdi_y = moveAdverse.y;
		printLabyrinth();
		partieEnCours = coupFinal(moveFin);

	}

	closeConnection();
	return 0;

}