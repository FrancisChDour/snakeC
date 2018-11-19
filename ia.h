//
//  ia.h
//  snake
//
//  Created by François Dourlens-Monchy - Léopold JEAN on 05/01/2017.
//  Copyright © 2017 François Dourlens-Monchy. All rights reserved.
//

#ifndef ia_h
#define ia_h
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "conioMac.h"
#include "snake.h"
#include "ia.h"

int letMeDo(int width, int heigth, char tableau[heigth][width],serpent* serpent){

    int toReturn=0;
    usleep(50000);

    if (serpent->posMembreX[0] == serpent->fruitX && serpent->posMembreY[0] == serpent->fruitY)
        serpent->hungryIA = 0;
    else if (serpent->posMembreX[0] == 48 && serpent->posMembreY[0] == 24)
        serpent->tiredIA = 0;
    else if (serpent->posMembreX[0] == 1 && serpent->posMembreY[0] == 1){
        serpent->hungryIA = 1;
        serpent->tiredIA = 1;
    }
    else if (serpent->posMembreX[0] == serpent->fruitX && serpent->posMembreY[0] < serpent->fruitY && serpent->tiredIA){
        serpent->hungryIA = 1;
    }
    else if (serpent->posMembreX[0] == serpent->fruitX && serpent->posMembreY[0] > serpent->fruitY && serpent->tiredIA){
        serpent->hungryIA = 1;
    }

    if (serpent->hungryIA && serpent->tiredIA) {
        if(serpent->posMembreX[0] != serpent->fruitX)
            toReturn = 100;
        else{
            if(serpent->posMembreY[0] != serpent->fruitY && serpent->posMembreY[0] < serpent->fruitY)
                toReturn = 115;
            else if(serpent->posMembreY[0] != serpent->fruitY && serpent->posMembreY[0] > serpent->fruitY)
                toReturn = 122;
        }
    }
    else if(serpent->hungryIA == 0 && serpent->tiredIA){
        if (serpent->tiredIA) {
            if(serpent->posMembreX[0] != 48)
                toReturn = 100;
            else{
                toReturn = 115;
            }
        }
    }
    else if(serpent->hungryIA == 0 && serpent->tiredIA == 0){
        if(serpent->posMembreX[0] != 1)
            toReturn = 113;
        else{
            if(serpent->posMembreY[0] != 1)
                toReturn = 122;
        }
    }

    return toReturn;
}

void deplacementIA(int width, int heigth, char tableau[heigth][width],serpent* serpent){

        switch (letMeDo(width, heigth, tableau, serpent)) {
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
                serpent->orientation = 1;
                break;
        }

}

void snakeIA(int width, int heigth, char tableau[heigth][width],serpent* serpent){
    serpent->fruitX = rand_a_b(2, width-4);
    serpent->fruitY = rand_a_b(2, heigth-3);
    int time = 0;
    while (serpent->alive) {
        clearArray(width, heigth, tableau);
        deplacementIA(width, heigth, tableau, serpent);
        majArray(width, heigth, tableau, serpent);
        clearAll();
        scoreBoard(serpent);
        output(width, heigth, tableau);
        time++;
    }
}
#endif /* ia_h */
