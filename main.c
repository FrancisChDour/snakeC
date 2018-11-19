//
//  main.c
//  SNAKE
//
//  Created by François Dourlens-Monchy - Léopold JEAN on 03/01/2017.
//  Copyright © 2017 François Dourlens-Monchy. All rights reserved.
//  Groupe 12

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "conioMac.h"
#include "snake.h"
#include "ia.h"

/* ---------- MAIN -------- */

int main(){
    serpent serpent;
    srand(time(NULL));
    // Hauteur, Largeur tableau
    int height = 26, width = 51;
    // Création d'un tableau à double entrée
    char tableau[height][width];
    // Initialisation du tableau
    clearArray(width, height, tableau);
    // On clear la console
    clearAll();
    // On affiche le menu
    int continuer = 1;
    while (continuer==1) {
        initStruct(&serpent);
        switch (menu(width, height, tableau)) {
            case 97: //touche a, lance la partie A
                serpent.difficulty = 0;
                serpent.gameMode = 1;
                beforePlaying(width, height, tableau, &serpent);
                snake1(width, height, tableau, &serpent);
                registerScore(width, height, tableau, &serpent);
                break;
            case 113: // touche q, lance le mode classique
                serpent.difficulty = 1;
                serpent.gameMode = 2;
                beforePlaying(width, height, tableau, &serpent);
                snake1(width, height, tableau, &serpent);
                registerScore(width, height, tableau, &serpent);
                break;
            case 98: // touche b, lance la partie B
                serpent.difficulty = 2;
                serpent.gameMode = 3;
                beforePlaying(width, height, tableau, &serpent);
                snake1(width, height, tableau, &serpent);
                registerScore(width, height, tableau, &serpent);
                break;
            case 100: // touche c, lance la partie C
                serpent.difficulty = 2;
                serpent.gameMode = 4;
                beforePlaying(width, height, tableau, &serpent);
                snakeIA(width, height, tableau, &serpent);
                registerScore(width, height, tableau, &serpent);
                break;
            case 99: // touche d, lance la partie c
                serpent.difficulty = 3;
                serpent.gameMode=5;
                beforePlaying(width, height, tableau, &serpent);
                snake1(width, height, tableau, &serpent);
                registerScore(width, height, tableau, &serpent);
                break;
            case 108: // touche l, quitte la partie
                continuer = 0;
                break;
            default:
                continuer = 1;
                break;
        }
    }
    goodbye(width, height, tableau);
    return 0;
}
