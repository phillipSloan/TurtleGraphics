#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <time.h>
/*from COMSM1201 ADTs lectures*/
#include "../ADTs/General/general.h"
#include "../ADTs/General/bool.h"

#define INITSIZE 15
#define STRLN 10
#define SCALEFACTOR 1.5
/*from COMSM1201 Parsing 0's & 1's code*/
#define strsame(A,B) (strcmp(A, B) == 0)

typedef struct turtle{
   /*Array for the file*/
   char **wds;
   /*Current word*/
   int cw;
   /*Capacity of array*/
   int cap;
} turtle;

/*testing*/
void test(void);
/*Replicates fscanf (badly) but on a string instead of file*/
void testReadString(turtle* trtl, char* string, int n);
/*Parsing functions*/
bool prog(turtle* trtl);
bool instructLst(turtle* trtl);
bool instruction(turtle* trtl);
bool fd(turtle* trtl);
bool lt(turtle* trtl);
bool rt(turtle* trtl);
bool _do(turtle* trtl);
bool set(turtle* trtl);
bool polish(turtle* trtl);
bool varnum(turtle* trtl);
/*checks if a string contains a valid variable "A" -> "Z"*/
bool var(turtle* trtl);
/*checks if string contains a valid number*/
bool isNum(turtle* trtl);
/*checks for valid operand eg '+', '-', '*' or '/' */
bool op(turtle* trtl);
/*helper functions*/

bool readFile(turtle* trtl, char* fileName);
bool trtlInit(turtle** trtl);
bool trtlFree(turtle* trtl);
void trtlResize(turtle* trtl);
