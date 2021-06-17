#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
/*from COMSM1201 ADTs lectures*/
#include "../../ADTs/General/general.h"
#include "../../ADTs/General/bool.h"
/*changed datatype from int to double for accuracy* */
#include "../../ADTs/Stack/Linked/specific.h"
#include "../../ADTs/Stack/stack.h"

/*size of array, length of strings and number array resizes by*/
#define ASCIIBEFOREA 'A'
#define INITSIZE 15
#define STRLN 10
#define SCALEFACTOR 1.5
#define PI 3.14159265358979323846264338327950288
#define DEG 270
/*initial coords for X and Y on screen */
#define XSTARTINGPOINT 400
#define YSTARTINGPOINT 300
/*converting degree input into radians*/
#define TORADIANS PI/180
#define WAIT 100
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
   /*stores set() and _do() variable values*/
   double varArr[26];
   /*flag array preventing use of an unused variable*/
   bool varUsed[26];
} turtle;

/*testing*/
void test(void);
/*Replicates fscanf (badly) but on a string instead of file*/
void testReadString(turtle* trtl, char* string, int n);
/*allows mimicing of input from a do and polish function input*/
void testPushToStack(stack* s, char* str, int n);
            /*Parser Functions*/
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

         /*Interpreter functions*/
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
