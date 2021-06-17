#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include "../../SDL/neillsdl2.h"
/*from COMSM1201 ADTs lectures*/
#include "../../ADTs/General/general.h"
#include "../../ADTs/General/bool.h"
/*changed datatype from int to double for accuracy* */
#include "../../ADTs/Stack/Linked/specific.h"
#include "../../ADTs/Stack/stack.h"

#define ASCIIBEFOREA 'A'
#define PI 3.14159265358979323846264338327950288
/*size of array, length of strings and number array resizes by*/
#define INITSIZE 15
#define STRLN 10
#define SCALEFACTOR 1.5
#define DEG 270
/*SDL DEFINES*/
#define MAXRGB 255
#define MINRGB 0
#define WHITE 255
#define WAIT 1500
#define DELAY 100
/*initial coords for X and Y WWIDTH and WHEIGHT set in
neilsdl2*/
#define XSTARTINGPOINT WWIDTH/2
#define YSTARTINGPOINT WHEIGHT/2
/*converting degree input into radians*/
#define TORADIANS PI/180
/*from COMSM1201 Parsing 0's & 1's code*/
#define strsame(A,B) (strcmp(A, B) == 0)

typedef enum operators {none, plus, minus, multiply,\
                        divide, modulus} operators;

typedef struct turtle{
   /*Array for the file*/
   char **wds;
   /*Current word*/
   int cw;
   /*Total words*/
   int tw;
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
   /*turns on drawing or not*/
   bool pen;
   bool delay;
   /*RGB triplet for colours*/
   int red;
   int green;
   int blue;
} turtle;

/*testing*/
void test(void);
/*Replicates fscanf (badly) but on a string instead of file*/
void testReadString(turtle* trtl, char* string, int n);
/*allows mimicing of input from a do and polish function input*/
void testPushToStack(stack* s, char* str, int n);
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
/*extension functions*/
bool _if(turtle* trtl, stack* s);
bool ifGreaterThan(turtle* trtl, stack* s);
bool ifGreaterThanEqual(turtle* trtl, stack* s);
bool ifLessThan(turtle* trtl, stack* s);
bool ifLessThanEqual(turtle* trtl, stack* s);
bool ifEqual(turtle* trtl, stack* s);
int isSign(turtle* trtl);
bool comment(turtle* trtl);
bool pen(turtle* trtl);
bool colour(turtle* trtl);
bool customColour(turtle* trtl);
int colourInRange(int colour);
bool randomColour(turtle* trtl);
bool drawCircle(turtle* trtl);
bool showMovement(turtle* trtl);
bool trtlJump(turtle* trtl);
/*helper functions*/
bool readFile(turtle* trtl, char* fileName);
bool trtlInit(turtle** trtl);
bool trtlFree(turtle* trtl);
void trtlResize(turtle* trtl);
bool resetStack(stack* s);
bool chkDivByZero(double operand);
