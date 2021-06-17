/*NOT A PRODUCTION VERSION - NO SDL TO ALLOW/SHOW TESTING*/
#include "turtle.h"

int main(int argc, char* argv[])
{
   turtle *trtl;
   stack* s;
   char* fileName = argv[1];
   int validCmdLine = 2;

   if(argc != validCmdLine){
      on_error("Invalid Command Line Arguments");
   }
   test();
   trtlInit(&trtl);
   s = stack_init();
   readFile(trtl, fileName);
   if(prog(trtl, s)){
      trtlFree(trtl);
      stack_free(s);
      return 0;
   }
   else {
      printf("Failed to parse.\n");
      trtlFree(trtl);
      stack_free(s);
      return 1;
   }
   return 0;
}

bool readFile(turtle* trtl, char* fileName)
{
   int i = 0;
   FILE *fp;
   fp = nfopen(fileName, "r");

   while(fscanf(fp, "%s", trtl->wds[i++]) == true){
      if (i >= trtl->cap){
         trtlResize(trtl);
      }
   }
   fclose(fp);
   return true;
}

bool prog(turtle* trtl, stack* s)
{
   if(!strsame(trtl->wds[trtl->cw], "{")){
      return false;
   }
   trtl->cw++;
   if(!instructLst(trtl, s)){
      return false;
   }
   trtl->cw++;
   /*if there are commands after the final }, return false*/
   if(*trtl->wds[trtl->cw] != '\0'){
      return false;
   }
   return true;
}

bool instructLst(turtle* trtl, stack* s)
{
   if(strsame(trtl->wds[trtl->cw], "}")){
      return true;
   }
   if(!instruction(trtl, s)){
      return false;
   }
   trtl->cw++;
   if(!instructLst(trtl, s)){
      return false;
   }
   return true;
}

bool instruction(turtle* trtl, stack* s)
{
   char* currentWord = trtl->wds[trtl->cw];
   if(strsame(currentWord, "FD")){
      return fd(trtl);
   }
   else if(strsame(currentWord, "LT")){
      return lt(trtl);
   }
   else if(strsame(currentWord, "RT")){
      return rt(trtl);
   }
   else if(strsame(currentWord, "DO")){
      return _do(trtl, s);
   }
   else if(strsame(currentWord, "SET")){
      return set(trtl, s);
   }
   else{
      return false;
   }
}

bool fd(turtle* trtl)
{
   int varIndex;
   double distance;

   trtl->cw++;
   if(isNum(trtl)){
      distance = atof(trtl->wds[trtl->cw]);
      return moveForward(trtl, distance);
   }
   if(var(trtl)){
      varIndex = *trtl->wds[trtl->cw] - ASCIIBEFOREA;
      if(trtl->varUsed[varIndex]){
         distance = trtl->varArr[varIndex];
         return moveForward(trtl, distance);
      }
   }
   return false;
}

bool moveForward(turtle* trtl, int distance)
{
   double radians;
/* These lines not required as not using SDL
   int xPrev, yPrev;

   xPrev = trtl->x;
   yPrev = trtl->y;
*/
   radians = ((double)trtl->angle) * TORADIANS;
   trtl->x += (distance * cos(radians));
   trtl->y += (distance * sin(radians));

   return true;
}

bool lt(turtle* trtl)
{
   trtl->cw++;
   if(varnum(trtl)){
      convertAngle(trtl);
      return true;
   }
   return false;
}

bool rt(turtle* trtl)
{
   trtl->cw++;
   if(varnum(trtl)){
      convertAngle(trtl);
      return true;
   }
   return false;
}

bool convertAngle(turtle* trtl)
{
   double angle = 0;

   if (isNum(trtl)){
      angle = atof(trtl->wds[trtl->cw]);
   }
   else if (var(trtl)){
      angle = parseVariable(trtl);
   }
   else {
      return false;
   }
   return storeAngle(trtl, angle);
}

bool storeAngle(turtle* trtl, double angle){
   int threesixtydegrees = 360;
   int zerodegrees = 0;
   trtl->cw--;
   if (strsame(trtl->wds[trtl->cw], "RT")){
      trtl->angle += angle;
      if (trtl->angle > threesixtydegrees){
         trtl->angle -= threesixtydegrees;
      }
   }  /*LT turns the turtle counterclockwise*/
   else if (strsame(trtl->wds[trtl->cw], "LT")){
      trtl->angle -= angle;
      if (trtl->angle < zerodegrees){
         trtl->angle += threesixtydegrees;
      }
   }
   else {
      return false;
   }
   trtl->cw++;
   return true;
}

double parseVariable(turtle* trtl)
{
   double variable = 0;
   int varIndex = 0;
   char varChar = '\0';

   varChar = *trtl->wds[trtl->cw];
   varIndex = varChar - ASCIIBEFOREA;
   if (trtl->varUsed[varIndex]){
      variable = trtl->varArr[varIndex];
   }
   else{
      on_error("Attempting to use an uninitialized variable");
   }
   return variable;
}

bool _do(turtle* trtl, stack* s)
{
   int from, to, varIndex;
   trtl->cw++;
   if(var(trtl)){
      varIndex = setVariable(trtl);
      stack_push(s, varIndex);
      trtl->cw++;
      if (strsame(trtl->wds[trtl->cw], "FROM")){
         trtl->cw++;
         if(varnum(trtl)){
            from = parseVarnum(trtl);
            stack_push(s, from);
            trtl->cw++;
            if (strsame(trtl->wds[trtl->cw], "TO")){
               trtl->cw++;
               if(varnum(trtl)){
                  to = parseVarnum(trtl);
                  stack_push(s, to);
                  trtl->cw++;
                  if (strsame(trtl->wds[trtl->cw], "{")){
                     trtl->cw++;
                     if (from < to){
                        return incLoop(trtl, s);
                     }
                     else if(from > to){
                        return decLoop(trtl, s);
                     }
                     else{
                        resetStack(s);
                        return true;
                     }
                  }
               }
            }
         }
      }
   }
   return false;
}

bool incLoop(turtle* trtl, stack* s)
{
   double sFrom, sTo, sVarIndex;
   int from, to, varIndex, loopBack, pushedFromPrevFunc = 3;
   loopBack = trtl->cw;

   if (s->size != pushedFromPrevFunc){
      return false;
   }

   stack_pop(s, &sTo);
   stack_pop(s, &sFrom);
   stack_pop(s, &sVarIndex);
   from = (int)sFrom;
   to = (int)sTo;
   varIndex = (int)sVarIndex;

   while (from <= to){
      trtl->varArr[varIndex] = from;
      instructLst(trtl, s);
      from++;
      if(from <= to){
         trtl->cw = loopBack;
      }
   }
   return true;
}

bool decLoop(turtle* trtl, stack* s)
{
   double sFrom, sTo, sVarIndex;
   int from, to, varIndex, loopBack, pushedFromPrevFunc = 3;
   loopBack = trtl->cw;

   if (s->size < pushedFromPrevFunc){
      return false;
   }

   stack_pop(s, &sTo);
   stack_pop(s, &sFrom);
   stack_pop(s, &sVarIndex);
   from = (int)sFrom;
   to = (int)sTo;
   varIndex = (int)sVarIndex;

   while (from >= to){
      trtl->varArr[varIndex] = from;
      instructLst(trtl, s);
      from--;
      if(from >= to){
         trtl->cw = loopBack;
      }
   }
   return true;
}

int parseVarnum(turtle* trtl)
{
   int number = 0;
   if(var(trtl)){
      number = (int)parseVariable(trtl);
   }
   if(isNum(trtl)){
      number = atof(trtl->wds[trtl->cw]);
   }
   return (int)number;
}

bool varnum(turtle* trtl)
{
   return (isNum(trtl) || var(trtl));
}

bool var(turtle* trtl)
{
   char currentWord = *trtl->wds[trtl->cw];
   int singleChar = 1;
   /*is it upper case and only one character*/
   if(isupper(currentWord) &&
     ((int)strlen(trtl->wds[trtl->cw]) == singleChar)){
      return true;
   }

   return false;
}

bool isNum(turtle* trtl)
{
   int len, j = 0, fullStops = 0, maxPossible = 1;
   char currentChar;

   len = strlen(trtl->wds[trtl->cw]);
   if(trtl->wds[trtl->cw][j] == '.' && (len == 1)){
      return false;
   }
   /*if negative, skip minus sign for the check*/
   if(trtl->wds[trtl->cw][j] == '-'){
      j++;
   }
   while(j < len){
      currentChar = trtl->wds[trtl->cw][j];
      /*check the digits of the current word*/
      if((!isdigit(currentChar)) && !(currentChar == '.')){
         /*if one isn't a numerical digit, return false*/
         return false;
      }
      /*if there's more than one decimal point it's not valid*/
      if (currentChar == '.'){
         fullStops++;
      }
      j++;
   }
   if (fullStops > maxPossible){
      return false;
   }
   return true;
}

bool set(turtle* trtl, stack* s)
{
   int varIndex;

   trtl->cw++;
   if(var(trtl)){
      varIndex = setVariable(trtl);
      trtl->cw++;
      if (strsame(trtl->wds[trtl->cw], ":=")){
         trtl->cw++;
         if(polish(trtl, s, varIndex)){
            return true;
         }
      }
   }
   return false;
}

int setVariable(turtle* trtl)
{
   char variable;
   int varIndex;

   variable = *trtl->wds[trtl->cw];
   varIndex = variable - ASCIIBEFOREA;

   if(!trtl->varUsed[varIndex]){
      trtl->varUsed[varIndex] = true;
   }
   return varIndex;
}

bool polish(turtle* trtl, stack* s, int varIndex)
{

   double operand;
   int validStackSize = 2;
   if (strsame(trtl->wds[trtl->cw], ";")){
      return storeVariable(trtl, s, varIndex);
   }
   if(op(trtl)){
      if(s->size < validStackSize){
         return false;
      }
      if(!calculateRPN(trtl, s)){
         return false;
      }
      trtl->cw++;
      return polish(trtl, s, varIndex);
   }
   if(var(trtl)){
      operand = parseVariable(trtl);
      stack_push(s, operand);
      trtl->cw++;
      return polish(trtl, s, varIndex);
   }
   if(isNum(trtl)){
      operand = atof(trtl->wds[trtl->cw]);
      stack_push(s, operand);
      trtl->cw++;
      return polish(trtl, s, varIndex);
   }
   return false;
}

bool calculateRPN(turtle* trtl, stack* s)
{
   double op1, op2, sum;
   int operator;

   operator = op(trtl);
   stack_pop(s, &op2);
   stack_pop(s, &op1);

   switch(operator)
   {
      case plus:
         sum = op1 + op2;
         break;
      case minus:
         sum = op1 - op2;
         break;
      case multiply:
         sum = op1 * op2;
         break;
      case divide:
         if (!chkDivByZero(op2)){
            return false;
         }
         sum = op1 / op2;
         break;
      default:
         return false;
   }
   stack_push(s, sum);
   return true;
}

bool chkDivByZero(double operand)
{
   double smallerThanZero = -0.999999999999999;
   double greaterThanZero =  0.0000000000001;
   if ((operand > smallerThanZero) &&
       (operand < greaterThanZero)){
      return false;
   }
   return true;
}

bool storeVariable(turtle* trtl, stack* s, int varIndex)
{
   int nothingOnStack = 0;
   /*if there is nothing on the stack then we can't pop*/
   if(s->size == nothingOnStack){
      return false;
   }
   stack_pop(s, &trtl->varArr[varIndex]);
   /*if there is something on the stack, RPN has not been used*/
   if (s->size != nothingOnStack){
      return false;
   }
   return true;
}

int op(turtle* trtl)
{
   char currentWord = *trtl->wds[trtl->cw];
   int singleChar = 1;
   /*if it's more than a single char it's not an operator*/
   if((int)strlen(trtl->wds[trtl->cw]) > singleChar){
      return false;
   }

   switch(currentWord)
   {
      case '+':
         return plus;
      case '-':
         return minus;
      case '*':
         return multiply;
      case '/':
         return divide;
      default:
         return false;
   }
}

bool trtlInit(turtle** trtl)
{
   turtle* tmp;
   tmp = (turtle*)ncalloc(1, sizeof(turtle));
   tmp->wds = (char**)n2dcalloc(INITSIZE, STRLN, sizeof(char*));
   tmp->cap= INITSIZE;
   tmp->x = XSTARTINGPOINT;
   tmp->y = YSTARTINGPOINT;
   tmp->angle = DEG;
   if (tmp){
      *trtl = tmp;
      return true;
   }
   trtl = NULL;
   return false;
}

void trtlResize(turtle* trtl)
{
   int oldCap = trtl->cap;
   trtl->cap = trtl->cap * SCALEFACTOR;
   trtl->wds = (char**)n2drecalloc((void**)trtl->wds, oldCap,
               trtl->cap, STRLN, STRLN, sizeof(char*));
}

bool trtlFree(turtle* trtl)
{
   if (!trtl){
      return false;
   }

   n2dfree((void**)trtl->wds, trtl->cap);
   free(trtl);
   return true;
}

bool resetStack(stack* s)
{
   double tmp;
   int empty = 0;

   while (s->size != empty){
      stack_pop(s, &tmp);
   }
   if (s->size == 0){
      return true;
   }
   return false;
}
