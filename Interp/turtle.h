#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include "../SDL/neillsdl2.h"
/*from COMSM1201 ADTs lectures*/
#include "../ADTs/General/general.h"
#include "../ADTs/General/bool.h"
/*changed datatype from int to double for accuracy* */
#include "../ADTs/Stack/Linked/specific.h"
#include "../ADTs/Stack/stack.h"

#define ASCIIBEFOREA 'A'
#define PI 3.14159265358979323846264338327950288
/*size of array, length of strings and number array resizes by*/
#define INITSIZE 15
#define STRLN 10
#define SCALEFACTOR 1.5
#define DEG 270
/*SDL DEFINES*/
#define WHITE 255
#define WAIT 1500
/*initial coords for X and Y WWIDTH and WHEIGHT set in
neilsdl2*/
#define XSTARTINGPOINT WWIDTH/2
#define YSTARTINGPOINT WHEIGHT/2
/*converting degree input into radians*/
#define TORADIANS PI/180
/*from COMSM1201 Parsing 0's & 1's code*/
#define strsame(A,B) (strcmp(A, B) == 0)

typedef enum operators {none, plus, minus, multiply,\
                        divide} operators;

typedef struct turtle{
   /*Array for the file*/
   char **wds;
   /*Current word*/
   int cw;
   /*Capacity of array*/
   int cap;
   /*coordinates and angles for calculating lines in SDL*/
   double x;
   double y;
   double angle;
   /*stores set() variables and minimum do() loop values*/
   double varArr[26];
   /*flag array stopping assignment of a used variable*/
   bool varUsed[26];
   SDL_Simplewin sw;
} turtle;

/*Parsing functions functions*/
bool prog(turtle* trtl, stack* s);
bool instructLst(turtle* trtl, stack* s);
bool instruction(turtle* trtl, stack* s);
bool fd(turtle* trtl);
bool lt(turtle* trtl);
bool rt(turtle* trtl);
bool _do(turtle* trtl, stack* s);
bool set(turtle* trtl, stack* s);
bool polish(turtle* trtl, stack* s, int varIndex);
bool varnum(turtle* trtl);
/*checks if a string contains a valid variable "A" -> "Z"*/
bool var(turtle* trtl);
/*checks if string contains a valid number*/
bool isNum(turtle* trtl);
/*checks for valid operand eg '+', '-', '*' or '/' */
int op(turtle* trtl);
/*Interpretting functions*/
bool convertAngle(turtle* trtl);
bool storeAngle(turtle* trtl, double angle);
bool moveForward(turtle* trtl, int distance);
double parseVariable(turtle* trtl);
int setVariable(turtle* trtl);
bool calculateRPN(turtle* trtl, stack* s);
bool storeVariable(turtle* trtl, stack* s, int varIndex);
int parseVarnum(turtle* trtl);
bool incLoop(turtle* trtl, stack* s);
bool decLoop(turtle* trtl, stack* s);
/*helper functions*/
bool readFile(turtle* trtl, char* fileName);
bool trtlInit(turtle** trtl);
bool trtlFree(turtle* trtl);
void trtlResize(turtle* trtl);
bool resetStack(stack* s);
bool chkDivByZero(double operand);
