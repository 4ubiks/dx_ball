#include "simpleOSlibc.h"
#include "constants.h"
#include "x86.h"
#include "screen.h"
#include "keyboard.h"
#include "vm.h"
#include "dxball.h"

// templates
void printBoard();
void titleScreenOn();
void titleScreenOff();
void rules();

// general logic
void refreshRate(int ms);
int checkKey(char* key);

// game logic
void setPaddle(int y, int x);
void setBall(int y, int x);
int movePaddleLeft(int x);
int movePaddleRight(int x);
int moveBallX(int x, int x_offset);
int invertX(int x_offset);
int moveBallY(int y, int y_offset);
int invertY(int y_offset);

void main(){
    clearScreen();
    disableCursor();

    char *buffer = (char *)KEYBOARD_BUFFER;

    // "Welcome" screen and rule screen
    while (true){
        titleScreenOn();
        readKeyboard(buffer);
        unsigned char compareKey = (unsigned char)(*(char *)KEYBOARD_BUFFER);
        if (compareKey == 0x93){
            rules(); 
            readKeyboard(buffer);}
        else if (compareKey == 0x9c){
            break;
        }
    }

    // Variable Declarations
    int PADDLE_Y = 21;
    int PADDLE_X = 34;

    int BALL_Y = 20;
    int BALL_X = 38;

    int x_offset = 1;
    int y_offset = 1;

    printBoard();
    fillMemory((char *)KEYBOARD_BUFFER, (unsigned char)0x0, (KEYBOARD_BUFFER_SIZE * 2));

    setPaddle(PADDLE_Y, PADDLE_X);
    setBall(BALL_Y, BALL_X);

    // Gameplay loop
    while (true){
        char *bufferMem = (char *)KEYBOARD_BUFFER;

        int keyPressed = checkKey((char *)bufferMem);
        if (keyPressed){
            unsigned char key = (unsigned char)(*(char *)KEYBOARD_BUFFER);

            // presses 'a'
            if (key == 0x9e){
                PADDLE_X = movePaddleLeft(PADDLE_X);
                printBoard();
                setPaddle(PADDLE_Y, PADDLE_X);
            }
            else if (key == 0xa0){
                PADDLE_X = movePaddleRight(PADDLE_X);
                printBoard();
                setPaddle(PADDLE_Y, PADDLE_X);
            }
        }   
        refreshRate(5000);
        
        // i think this is theoretically good
        // just missing some logic for edge cases
        if (BALL_X >= 0x47 || BALL_X <= 0x05){
            x_offset = invertX(x_offset);
        }

        if (BALL_Y >=0x15 || BALL_Y <= 0x02){
            y_offset = invertY(y_offset);
        }

        BALL_X = moveBallX(BALL_X, x_offset);
        BALL_Y = moveBallY(BALL_Y, y_offset);

        printBoard();
        setPaddle(PADDLE_Y, PADDLE_X);
        setBall(BALL_Y, BALL_X);
    }  
}

// templates
void titleScreenOn(){
    // ascii font: 'Orge'
    printString(COLOR_TEAL, 2, 10, (char *)"|-----------------------------------------------------------|");
    printString(COLOR_TEAL, 3, 10, (char *)"|     _____  __    ___    _      __    __                   |");
    printString(COLOR_TEAL, 4, 10, (char *)"|    /   \\ \\/ /   / __\\  /_\\    / /   / /                  |");
    printString(COLOR_TEAL, 5, 10, (char *)"|   / /\\ /\\  /   /__\\// //_\\\\  / /   / /                    |");
    printString(COLOR_TEAL, 6, 10, (char *)"|  / /_// /  \\  / \\/  \\/  _  \\/ /___/ /___                  |");
    printString(COLOR_TEAL, 7, 10, (char *)"| /___,' /_/\\_\\ \\_____/\\_/ \\_/\\____/\\____/                  |");
    printString(COLOR_TEAL, 8, 10, (char *)"|-----------------------------------------------------------|");

    printString(COLOR_WHITE, 15, 10, (char *)"Press [ENTER] to start...");
}
void titleScreenOff(){
    printString(COLOR_WHITE, 15, 10, (char *)"                                                  ");
}

void rules(){
    clearScreen();
    printString(COLOR_RED, 5, 10, (char *)"Filler. Implement this later.");
}


void printBoard(){
    clearScreen();
    for (int i=1; i<22; i++){
        printString(COLOR_ONE, i, 5, (char *)"|. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . |");
    }
    printString(COLOR_ONE, 22, 5, (char *)"|__________________________________________________________________|");
}

// general logic
void refreshRate(int ms){
    for (int i=0; i<ms*1000; i++){
        true == true; 
    }
}

int checkKey(char* key){
    if (inputIOPort(KEYBOARD_STATUS_PORT) & 0x1){
        unsigned char scanCode;

        inputIOPort(KEYBOARD_DATA_PORT); // discard first read
        scanCode = inputIOPort(KEYBOARD_DATA_PORT);

        *key = ((unsigned char)scanCode);
        if (scanCode != 0){
            return 1;
        }
        return 0;
    }
    return 0;
}

// game logic
void setPaddle(int y, int x){
    printString(COLOR_WHITE, y, x, (char *)"=========");
}

void setBall(int y, int x){
    printString(COLOR_TEAL, y, x, (char *)"@");
}

int movePaddleLeft(int x){
    if (x>6){
        x-=2;
    }
    return x;
}

int movePaddleRight(int x){
    if (x<62){
        x+=2;
    }
    return x;
}

int moveBallX(int x, int x_offset){
    if (x >= 5){
        x+=(2*x_offset);
    }
    else if (x <= 71){
        x-=(2*x_offset);
    }
    return x;
}

int invertX(int x_offset){
    return -x_offset;
}

int moveBallY(int y, int y_offset){
    if (y >= 5){
        y-=(y_offset);
    }
    else if (y <= 21){
        y+=y_offset;
    }
    return y;
}

int invertY(int y_offset){
    return -y_offset;
}
