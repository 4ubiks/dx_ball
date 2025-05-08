#include "simpleOSlibc.h"
#include "constants.h"
#include "x86.h"
#include "screen.h"
#include "keyboard.h"
#include "vm.h"
#include "dxball.h"

// templates
void printBoard();

// game logic

void main(){
    clearScreen();
    disableCursor();

    while (true){
        printBoard();
    }
    return;
}

void printBoard(){
    for (int i=1; i<22; i++){
        printString(COLOR_ONE, i, 5, (char *)"|. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . |");
    }
    printString(COLOR_ONE, 22, 5, (char *)"|__________________________________________________________________|");
}
