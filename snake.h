//
//  snake.h
//  snake
//
//  Created by François Dourlens-Monchy - Léopold JEAN on 05/01/2017.
//  Copyright © 2017 François Dourlens-Monchy. All rights reserved.
//

#ifndef snake_h
#define snake_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "conioMac.h"

typedef struct serpent  serpent;

struct serpent{
    int posMembreX[200];
    int posMembreY[200];
    int orientation;
    int taille;
    int alive;
    int life;
    int difficulty;
    unsigned int score;
    int time;
    int clock;
    int clockLauch;
    int timeRemaining;
    int gameMode;

    int fruitX;
    int fruitY;

    int hungryIA;
    int tiredIA;
};

// initialise les valeurs des differents parametres ( fct appellée au début de chaque partie )
void initStruct(serpent* serpent){
    // Structure du serpent
    serpent->orientation = 1; // 1  = Droite
    serpent->taille = 0;
    serpent->posMembreX[0] = 2;
    serpent->posMembreY[0] = 2;
    serpent->alive = 1;
    serpent->difficulty=1;
    serpent->score = 0;
    serpent->time = 0;
    serpent->hungryIA = 1;
    serpent->tiredIA = 1;
    serpent->clock = 0;
    serpent->clockLauch = 0;
    serpent->life = 3;
    serpent->timeRemaining = 10;
    serpent->gameMode = 0;

}

void clearArray(int width, int heigth, char (*tableau)[width]);
void output(int width, int heigth, char (*tableau)[width]);
void clearAll();
int menu(int width, int heigth, char tableau[heigth][width]);
int getKeyboard();
void deplacement(int width, int heigth, char tableau[heigth][width],serpent* serpent);
void snake1(int width, int heigth, char tableau[heigth][width],serpent* serpent);
int rand_a_b(int a, int b);
void majArray(int width, int heigth, char tableau[heigth][width],serpent* serpent);
void initStruct(serpent* serpent);
void scoreBoard(serpent* serpent);
void randomFruit(int width, int heigth, char tableau[heigth][width],serpent* serpent);
void registerScore(int width, int heigth, char tableau[heigth][width], serpent* serpent);
void gameOver(int width, int heigth, char tableau[heigth][width]);

// Initialisation du cadre ( attribuer des valeurs dans le tableau )
void clearArray(int width, int heigth, char tableau[heigth][width]){
    for (int y = 0; y < heigth; y++) {
        for (int x = 0; x < width; x++) {
            if (y == 0 || y == heigth-1) {
                if (x == width-1) {
                    tableau[y][x] = '\n';
                }
                else{
                    tableau[y][x] = 'X';
                }
            }
            else{
                if (x == width-1) {
                    tableau[y][x] = '\n';
                }
                else if(x == 0 || x == width-2){
                    tableau[y][x] = 'X';
                }
                else{
                    tableau[y][x] = ' ';
                }
            }
        }
    }
}

// Afficher le tableau une fois initialisé
void output(int width, int heigth, char tableau[heigth][width]){
    for (int y = 0; y < heigth; y++) {
        for (int x = 0; x < width; x++) {
            printf("%c", tableau[y][x]);
            printf(" ");
        }
    }
}

void clearAll(){
    system("clear");
}

// affiche le texte du menu a l'ecran
int menu(int width, int heigth, char tableau[heigth][width]){
    int choix = 0;
    while (choix != 100 && choix != 98 && choix !=113 && choix != 108 && choix != 97 && choix != 99){
        clearAll();
        clearArray(width, heigth, tableau);
        printText("_____________________", width, heigth, tableau, 1, 15);
        printText("*** Snake Game FL ***", width, heigth, tableau, 3, 15);
        printText("_____________________", width, heigth, tableau, 4, 15);
        printText("Time Trial  tapez A", width, heigth, tableau, 7, 16);
        printText("Jeu Classique tapez Q", width, heigth, tableau, 9, 15);
        printText("Mode hardcore tapez B", width, heigth, tableau, 11, 15);
        printText("Passe-Partout tapez C", width, heigth, tableau, 13, 15);
        printText("Laisser Faire tapez D", width, heigth, tableau, 15, 15);
        printText("Pour quitter  tapez L", width, heigth, tableau, 17, 15);
        printf("\n ");
        output(width, heigth, tableau);
        choix = mygetch();
    }
    
    return choix;
}

int printText(const char* text, int width, int heigth, char tableau[heigth][width],int ord, int abs){
    int i;
    for (i=0; text[i] !='\0'; i++) {
        tableau[ord][abs + i] = text[i];
    }
    return 0;
}

void clearBuffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void gameOver(int width, int heigth, char tableau[heigth][width]){
    clearAll();
    clearArray(width, heigth, tableau);
    printText("_____________________", width, heigth, tableau, 1, 15);
    printText("  *** Game Over ***", width, heigth, tableau, 3, 15);
    printText("_____________________", width, heigth, tableau, 4, 15);
    printText("Press enter to continue", width, heigth, tableau, 7, 14);
    printf("\n ");
    output(width, heigth, tableau);
    while(mygetch()!=10);
}

void beforePlaying(int width, int heigth, char tableau[heigth][width], serpent*serpent){
    clearArray(width, heigth, tableau);
    clearAll();
    printText("_____________________", width, heigth, tableau, 1, 15);
    printText("** Are You Ready ? **", width, heigth, tableau, 3, 15);
    printText("_____________________", width, heigth, tableau, 4, 15);
    printText("Press enter to start!", width, heigth, tableau, 7, 15);
    
    FILE * fp;
    char name[100] = " ";
    int oldscore = 0;
    
    switch (serpent->gameMode) {
        case 1:
            fp = fopen("scores/trial.txt", "r+");
            fscanf(fp, "%s %d", name, &oldscore);
            fclose(fp);
            break;
        case 2:
            fp = fopen("scores/classique.txt", "r+");
            fscanf(fp, "%s %d", name, &oldscore);
            fclose(fp);
            break;
        case 3:
            fp = fopen("scores/hardcore.txt", "r+");
            fscanf(fp, "%s %d", name, &oldscore);
            fclose(fp);
            break;
        case 4:
            fp = fopen("scores/ia.txt", "r+");
            fscanf(fp, "%s %d", name, &oldscore);
            fclose(fp);
            break;
        case 5:
            fp = fopen("scores/partout.txt", "r+");
            fscanf(fp, "%s %d", name, &oldscore);
            fclose(fp);
            break;
            
        default:
            break;
    }
    
    printf(" Meilleur score : %s avec %d $\n ", name, oldscore);
    
    output(width, heigth, tableau);
    while(mygetch()!=10);
}

void goodbye(int width, int heigth, char tableau[heigth][width]){
    clearArray(width, heigth, tableau);
    clearAll();
    printText("_____________________", width, heigth, tableau, 1, 15);
    printText("*** Goodbye !!! ***", width, heigth, tableau, 3, 16);
    printText("_____________________", width, heigth, tableau, 4, 15);
    printf("\n ");
    output(width, heigth, tableau);
    sleep(1);
    clearAll();
    
}

// fct qui gere l'orientation ainsi que la vitesse du serpent
void deplacement(int width, int heigth, char tableau[heigth][width],serpent* serpent){

    if ((serpent->difficulty == 1 || serpent->difficulty == 2) && serpent->difficulty !=0) {
        if (serpent->taille <5) {
            usleep(70000);
        }
        else if (serpent->taille <11) {
            usleep(60000);
        }
        else if (serpent->taille <18) {
            usleep(50000);
        }
        else if (serpent->taille <26) {
            usleep(40000);
        }
        else if (serpent->taille <33) {
            usleep(30000);
        }
        else if (serpent->taille <41) {
            usleep(20000);
        }
        else if (serpent->taille <50) {
            usleep(10000);
        }
        else if (serpent->taille <60) {
            usleep(5000);
        }
        else{
            usleep(2500);
        }
    }
    else{
        usleep(70000);
    }

    if (serpent->difficulty == 0) {
        switch (mygetch()) {
            case 100:
                //RIGHT D
                if(serpent->orientation!=4)
                    serpent->orientation = 1;
                break;
            case 115:
                //UP Z
                if(serpent->orientation!=3)
                    serpent->orientation = 2;
                break;
            case 122:
                //DOWN S
                if(serpent->orientation!=2)
                    serpent->orientation = 3;
                break;
            case 113:
                //LEFT Q
                if(serpent->orientation!=1)
                    serpent->orientation = 4;
                break;
            default:
                //FAUT PAS EN METTRE
                deplacement(width, heigth, tableau, serpent);
                break;
        }
    }
    else{
        if (kbhit()) {
            switch (mygetch()) {
                case 100:
                    //RIGHT D
                    if(serpent->orientation!=4)
                        serpent->orientation = 1;
                    break;
                case 115:
                    //UP Z
                    if(serpent->orientation!=3)
                        serpent->orientation = 2;
                    break;
                case 122:
                    //DOWN S
                    if(serpent->orientation!=2)
                        serpent->orientation = 3;
                    break;
                case 113:
                    //LEFT Q
                    if(serpent->orientation!=1)
                        serpent->orientation = 4;
                    break;
                default:
                    //FAUT PAS EN METTRE
                    deplacement(width, heigth, tableau, serpent);
                    break;
            }
        }
    }
}

//fct qui gere une partie de jeu, appelle les fcts affichage du tableau/serpent/score
void snake1(int width, int heigth, char tableau[heigth][width],serpent* serpent){

    randomFruit(width, heigth, tableau, serpent);
    if (serpent->difficulty ==2 || serpent->difficulty ==3) {
        while (tableau[serpent->fruitY][serpent->fruitX] == 'X') {
            randomFruit(width, heigth, tableau, serpent);
        }
    }
    int time = 0;
    while (serpent->alive) {
        clearArray(width, heigth, tableau);
        deplacement(width, heigth, tableau, serpent);

        if (serpent->difficulty == 2 || serpent->difficulty == 3) {
              int obstacley[] = {5,6,7,8,9,15,16,17,18,19};

            for (int i=0; i<10; i++) {
                printText("XXXXXXXX", width, heigth, tableau, obstacley[i], 6);
                printText("XXXXXXXXX", width, heigth, tableau, obstacley[i], 21);
                printText("XXXXXXXX", width, heigth, tableau, obstacley[i], 36);
            }
        }

        majArray(width, heigth, tableau, serpent);
        clearAll();
        scoreBoard(serpent);
        output(width, heigth, tableau);
        time++;
    }
}

int rand_a_b(int a, int b){
    return rand()%(b-a) +a;
}

//donne des coordonnées aléatoires au fruit
void randomFruit(int width, int heigth, char tableau[heigth][width],serpent* serpent){
    serpent->fruitX = rand_a_b(2, width-4);
    serpent->fruitY = rand_a_b(2, heigth-3);
}

// fct qui gere les collisions, et les game over, le systeme de score
//  elle fait se deplacer le serpent et augmente sa taille, voir le teleporte et gere ses vies
void majArray(int width, int heigth, char tableau[heigth][width],serpent* serpent){
    tableau[serpent->fruitY][serpent->fruitX] = '$';
    if (serpent->posMembreX[0] == serpent->fruitX && serpent->posMembreY[0] == serpent->fruitY) {
        randomFruit(width, heigth, tableau, serpent);
        if (serpent->difficulty ==2 || serpent->difficulty ==3) {
            while (tableau[serpent->fruitY][serpent->fruitX] == 'X') {
                randomFruit(width, heigth, tableau, serpent);
            }
        }
        tableau[serpent->fruitY][serpent->fruitX] = '$';
        serpent->taille++;
        serpent->score += serpent->taille;
        if (serpent->difficulty == 0) {
            serpent->timeRemaining += 3;
        }
    }
    int i;
    for (i=serpent->taille; i> 0; i--) {
        serpent->posMembreX[i] = serpent->posMembreX[i-1];
        serpent->posMembreY[i] = serpent->posMembreY[i-1];
    }
    switch (serpent->orientation) {
        case 1:
            serpent->posMembreX[0]++;
            break;
        case 2:
            serpent->posMembreY[0]++;
            break;
        case 3:
            serpent->posMembreY[0]--;
            break;
        case 4:
            serpent->posMembreX[0]--;
            break;
        default:
            //
            break;
    }
    if ((tableau[serpent->posMembreY[0]][serpent->posMembreX[0]] == 'X') && serpent->difficulty != 3) {
        serpent->alive = 0;
        clearAll();
        gameOver(width, heigth, tableau);
    }
    else if(serpent->difficulty == 3){
        if (tableau[serpent->posMembreY[0]][serpent->posMembreX[0]] == 'X'){
            switch (serpent->orientation) {
                case 1:
                    //
                    serpent->posMembreX[0] = 1;
                    break;
                case 2:
                    serpent->posMembreY[0] = 1;
                    break;
                case 3:
                    serpent->posMembreY[0] = heigth-2;
                    break;
                case 4:
                    serpent->posMembreX[0] = width-3;
                    break;
                default:
                    break;
            }
            serpent->life--;
            if (serpent->life < 0) {
                gameOver(width, heigth, tableau);
                serpent->alive =0;
            }
        }
    }
    for (i=serpent->taille; i>0; i--) {
        if (serpent->posMembreX[0] == serpent->posMembreX[i] && serpent->posMembreY[0] == serpent->posMembreY[i]){
            serpent->alive = 0;
            clearAll();
            gameOver(width, heigth, tableau);
        }
    }
    tableau[serpent->posMembreY[0]][serpent->posMembreX[0]] = 'O';
    for (i=serpent->taille; i> 0; i--) {
        tableau[serpent->posMembreY[i]][serpent->posMembreX[i]] = 'o';
    }
}

void scoreBoard(serpent* serpent){
    if (serpent->clockLauch == 0) {
        serpent->clockLauch = time(NULL);
    }
    serpent->clock = time(NULL) - serpent->clockLauch;
    if (serpent->difficulty == 0) {
        if (serpent->clock >= serpent->timeRemaining) {
            serpent->alive = 0;
        }
    }

    if (serpent->difficulty == 3) {
        printf("Taille : %d || Coups joués : %d || Temps : %d s || Score : %d $ || Vies : %d\n ", serpent->taille, serpent->time, serpent->clock, serpent->score, serpent->life);
    }
    else if (serpent->difficulty == 0) {
        printf("Taille : %d || Coups joués : %d || Temps : %d / %d s || Score : %d $ || Vies : %d\n ", serpent->taille, serpent->time, serpent->clock, serpent->timeRemaining, serpent->score, serpent->life);
    }
    else{
        printf("Taille : %d || Coups joués : %d || Temps : %d s || Score : %d $\n ", serpent->taille, serpent->time, serpent->clock, serpent->score);
    }
    serpent->time++;
}

void registerScore(int width, int heigth, char tableau[heigth][width], serpent* serpent){
    
    int score = serpent->score;
    char name[15] = " ";
    int oldscore;
    
    FILE *fp;
    
    switch (serpent->gameMode) {
        case 1:
            fp = fopen("scores/trial.txt", "r+");
            fscanf(fp, "%s %d", name, &oldscore);
            fclose(fp);
            break;
        case 2:
            fp = fopen("scores/classique.txt", "r+");
            fscanf(fp, "%s %d", name, &oldscore);
            fclose(fp);
            break;
        case 3:
            fp = fopen("scores/hardcore.txt", "r+");
            fscanf(fp, "%s %d", name, &oldscore);
            fclose(fp);
            break;
        case 4:
            fp = fopen("scores/ia.txt", "r+");
            fscanf(fp, "%s %d", name, &oldscore);
            fclose(fp);
            break;
        case 5:
            fp = fopen("scores/partout.txt", "r+");
            fscanf(fp, "%s %d", name, &oldscore);
            fclose(fp);
            break;
            
        default:
            break;
    }
    
    
    if (oldscore < score) {
        clearArray(width, heigth, tableau);
        clearAll();
        printText("_____________________", width, heigth, tableau, 1, 15);
        printText("*** Reccord battu ***", width, heigth, tableau, 3, 15);
        printText("_____________________", width, heigth, tableau, 4, 15);
        printText("Entrez votre nom !!", width, heigth, tableau, 6, 14);
        printf("\n");
        output(width, heigth, tableau);
        printf("Votre nom : ");
        scanf("%s", name);
        
        switch (serpent->gameMode) {
            case 1:
                fp  = fopen ("scores/trial.txt", "w+");
                fprintf(fp, "%s %d", name, score);
                fclose(fp);
                break;
            case 2:
                fp  = fopen ("scores/classique.txt", "w+");
                fprintf(fp, "%s %d", name, score);
                fclose(fp);
                break;
            case 3:
                fp  = fopen ("scores/hardcore.txt", "w+");
                fprintf(fp, "%s %d", name, score);
                fclose(fp);
                break;
            case 4:
                fp  = fopen ("scores/ia.txt", "w+");
                fprintf(fp, "%s %d", name, score);
                fclose(fp);
                break;
            case 5:
                fp  = fopen ("scores/partout.txt", "w+");
                fprintf(fp, "%s %d", name, score);
                fclose(fp);
                break;
                
            default:
                break;
        }
    }
}

#endif /* snake_h */
