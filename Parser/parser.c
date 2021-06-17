#include "turtle.h"

int main(int argc, char* argv[])
{
   turtle *trtl;
   char* fileName = argv[1];
   int validCmdLine = 2;

   if(argc != validCmdLine){
      on_error("Invalid Command Line Arguments");
   }

  /* test();*/
   trtlInit(&trtl);
   readFile(trtl, fileName);

   if(prog(trtl)){
      trtlFree(trtl);
   }
   else {
      fprintf(stderr,"Failed to parse.\n");
      trtlFree(trtl);
      exit(EXIT_FAILURE);
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

bool prog(turtle* trtl)
{
   if(!strsame(trtl->wds[trtl->cw], "{")){
      return false;
   }
   trtl->cw++;
   if(!instructLst(trtl)){
      return false;
   }
   trtl->cw++;
   /*if there are commands after the final }, return false*/
   if(*trtl->wds[trtl->cw] != '\0'){
      return false;
   }
   return true;
}

bool instructLst(turtle* trtl)
{
   if(strsame(trtl->wds[trtl->cw], "}")){
      return true;
   }
   if(!instruction(trtl)){
      return false;
   }
   trtl->cw++;
   if(!instructLst(trtl)){
      return false;
   }
   return true;
}

bool instruction(turtle* trtl)
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
      return _do(trtl);
   }
   else if(strsame(currentWord, "SET")){
      return set(trtl);
   }
   else{
      return false;
   }
}

bool fd(turtle* trtl)
{
   trtl->cw++;
   if(varnum(trtl)){
      return true;
   }
   return false;
}

bool lt(turtle* trtl)
{
   trtl->cw++;
   if(varnum(trtl)){
      return true;
   }
   return false;
}
bool rt(turtle* trtl)
{
   trtl->cw++;
   if(varnum(trtl)){
      return true;
   }
   return false;
}

bool _do(turtle* trtl)
{
   trtl->cw++;
   if(var(trtl)){
      trtl->cw++;
      if (strsame(trtl->wds[trtl->cw], "FROM")){
         trtl->cw++;
         if(varnum(trtl)){
            trtl->cw++;
            if (strsame(trtl->wds[trtl->cw], "TO")){
               trtl->cw++;
               if(varnum(trtl)){
                  trtl->cw++;
                  if (strsame(trtl->wds[trtl->cw], "{")){
                     trtl->cw++;
                     if(instructLst(trtl)){
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

bool varnum(turtle* trtl)
{
   /*if it's a number or single uppercase char, return true*/
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
   int len, i = 0, fullStops = 0, maxPossible = 1;
   char currentChar;

   len = strlen(trtl->wds[trtl->cw]);
   if(trtl->wds[trtl->cw][i] == '.' && (len == 1)){
      return false;
   }
   if(trtl->wds[trtl->cw][i] == '-'){
      i++;  /*if negative, skip minus sign for the check*/
   }
   while(i < len){
      currentChar = trtl->wds[trtl->cw][i];
      /*check the digits of the current word*/
      if((!isdigit(currentChar)) && !(currentChar == '.')){
         /*if one isn't a numerical digit, return false*/
         return false;
      }
      /*if there's more than one decimal point it's not valid*/
      if (currentChar == '.'){
         fullStops++;
      }
      i++;
   }
   if (fullStops > maxPossible){
      return false;
   }
   return true;
}

bool set(turtle* trtl)
{
   trtl->cw++;
   if(var(trtl)){
      trtl->cw++;

      if (strsame(trtl->wds[trtl->cw], ":=")){
         trtl->cw++;

         if(polish(trtl)){
            return true;
         }
      }
   }
   return false;
}

bool polish(turtle* trtl)
{
   if (strsame(trtl->wds[trtl->cw], ";")){
      return true;
   }
   if(op(trtl)){
      trtl->cw++;
      return polish(trtl);
   }
   if(varnum(trtl)){
      trtl->cw++;
      return polish(trtl);
   }
   return false;
}

bool op(turtle* trtl)
{
   char currentWord = *trtl->wds[trtl->cw];
   int singleChar = 1;

   if((int)strlen(trtl->wds[trtl->cw]) > singleChar){
      return false;
   }

   switch(currentWord)
   {
      case '+':
         return true;
      case '-':
         return true;
      case '*':
         return true;
      case '/':
         return true;
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
   trtl = NULL;
   return true;
}
