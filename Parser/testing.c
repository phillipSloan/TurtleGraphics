#include "turtle.h"

void test(void)
{
   int j;
   turtle* trtl;
   char string[250];
   srand(time(NULL));

   assert(trtlInit(&trtl));
   assert(trtl);
   assert(trtl->cap == 15);
   trtlResize(trtl);
   assert(trtl->cap == 22);

   /*WHITE BOX TESTING*/
   /*testing the op function - first four are valid, rest aren't*/
   strcpy(string, "+ - * / ++ 0 . F 44 { } ; FD LT RT DO :=");
   testReadString(trtl, string, 17);
   /*testing valid operands*/
   for (j = 0; j < 4; j++){
      trtl->cw = j;
      assert(op(trtl));
   }
   /*testing invalid operands*/
   for (j = 4; j < 17; j++){
      trtl->cw = j;
      assert(!op(trtl));
   }
   /*testing var for "A to Z" also testing file system*/
   /*to ensure it resizes if required*/
   assert(readFile(trtl, "Testing/White/AtoZ.test"));
   for (j = 0; j < 26; j++){
      trtl->cw = j;
      assert(var(trtl));
      assert(varnum(trtl));
   }
   /*testing isNum for a range of valid numbers*/
   assert(readFile(trtl, "Testing/White/numbers.test"));
   for (j = 0; j < 26; j++){
      trtl->cw = j;
      assert(isNum(trtl));
      assert(varnum(trtl));
   }
   /*clearing array*/
   assert(trtlFree(trtl));
   assert(trtlInit(&trtl));
   trtlResize(trtl);
   trtlResize(trtl);

   /*testing varnum*/
   strcpy(trtl->wds[0], "A");
   assert(varnum(trtl));
   strcpy(trtl->wds[0], "50");
   assert(varnum(trtl));
   strcpy(trtl->wds[0], " ");
   assert(!varnum(trtl));
   strcpy(trtl->wds[0], "!");
   assert(!varnum(trtl));

   /*testing FD/RT/LT  instruction */
   strcpy(trtl->wds[0], "FD");
   strcpy(trtl->wds[1], " ");
   trtl->cw = 0;
   assert(!instruction(trtl));

   strcpy(trtl->wds[0], "FD");
   strcpy(trtl->wds[1], ".");
   trtl->cw = 0;
   assert(!instruction(trtl));

   strcpy(string, "FD 50");
   testReadString(trtl, string, 2);
   trtl->cw = 0;
   assert(instruction(trtl));

   strcpy(string, "FD A");
   testReadString(trtl, string, 2);
   trtl->cw = 0;
   assert(instruction(trtl));

   strcpy(string, "FD -4.5");
   testReadString(trtl, string, 2);
   trtl->cw = 0;
   assert(instruction(trtl));

   trtl->cw = 0;
   strcpy(string, "FD 7-5");
   testReadString(trtl, string, 2);
   trtl->cw = 0;
   assert(!instruction(trtl));

   trtl->cw = 0;
   strcpy(string, "FD AA");
   testReadString(trtl, string, 2);
   trtl->cw = 0;
   assert(!instruction(trtl));

   trtl->cw = 0;
   strcpy(trtl->wds[0], "RT");
   strcpy(trtl->wds[1], " ");
   trtl->cw = 0;
   assert(!instruction(trtl));

   trtl->cw = 0;
   strcpy(string, "RT 50");
   testReadString(trtl, string, 2);
   trtl->cw = 0;
   assert(instruction(trtl));

   trtl->cw = 0;
   strcpy(string, "RT A");
   testReadString(trtl, string, 1);
   trtl->cw = 0;
   assert(instruction(trtl));

   trtl->cw = 0;
   strcpy(string, "RT -4.5");
   testReadString(trtl, string, 2);
   trtl->cw = 0;
   assert(instruction(trtl));

   trtl->cw = 0;
   strcpy(string, "RT a");
   testReadString(trtl, string, 2);
   trtl->cw = 0;
   assert(!instruction(trtl));

   trtl->cw = 0;
   strcpy(trtl->wds[0], "LT");
   strcpy(trtl->wds[1], " ");
   trtl->cw = 0;
   assert(!instruction(trtl));

   trtl->cw = 0;
   strcpy(string, "LT 50");
   testReadString(trtl, string, 2);
   trtl->cw = 0;
   assert(instruction(trtl));
   assert(!instructLst(trtl));

   trtl->cw = 0;
   strcpy(string, "LT Z");
   testReadString(trtl, string, 1);
   trtl->cw = 0;
   assert(instruction(trtl));
   assert(!instructLst(trtl));

   trtl->cw = 0;
   strcpy(string, "LT -7.1");
   testReadString(trtl, string, 2);
   trtl->cw = 0;
   assert(instruction(trtl));
   assert(!instructLst(trtl));

   trtl->cw = 0;
   strcpy(string, "LT ZZ");
   testReadString(trtl, string, 2);
   trtl->cw = 0;
   assert(!instruction(trtl));

   trtl->cw = 0;
   strcpy(string, "FD 30 LT 45 }");
   testReadString(trtl, string, 5);
   assert(instructLst(trtl));
   assert(!prog(trtl));

   trtl->cw = 0;
   strcpy(string, "FD 45 LT A RT -4.5 }");
   testReadString(trtl, string, 7);
   assert(instructLst(trtl));
   assert(!prog(trtl));

   trtl->cw = 0;
   strcpy(string, "FD 45 LT RT 56 }");
   testReadString(trtl, string, 6);
   assert(!instructLst(trtl));

   trtl->cw = 0;
   strcpy(string, "FD 4-5 LT 45 RT 56 }");
   testReadString(trtl, string, 6);
   assert(!instructLst(trtl));

   trtlFree(trtl);
   trtlInit(&trtl);

   trtl->cw = 0;
   strcpy(string, "{ FD 30 LT 4.5 RT -45 }");
   testReadString(trtl, string, 8);
   assert(prog(trtl));

   trtl->cw = 0;
   strcpy(string, "{ FD 30 LT 4.5 RT -45 }");
   testReadString(trtl, string, 8);
   assert(prog(trtl));

   trtlFree(trtl);
   trtlInit(&trtl);

   /*testing polish*/

   strcpy(trtl->wds[0], ";");
   assert(polish(trtl));

   trtl->cw = 0;
   strcpy(trtl->wds[0], "+");
   strcpy(trtl->wds[1], ";");
   assert(polish(trtl));

   trtl->cw = 0;
   strcpy(string, "A Z ;");
   testReadString(trtl, string, 3);
   assert(polish(trtl));

   trtl->cw = 0;
   strcpy(string, "5 5 + ;");
   testReadString(trtl, string, 4);
   assert(polish(trtl));

   trtl->cw = 0;
   strcpy(string, "$ + + ;");
   testReadString(trtl, string, 4);
   assert(!polish(trtl));

   trtl->cw = 0;
   strcpy(string, "SET A := 5 ;");
   testReadString(trtl, string, 5);
   assert(set(trtl));

   trtl->cw = 0;
   strcpy(string, "SET Z := A ;");
   testReadString(trtl, string, 5);
   assert(set(trtl));

   /*Despite being infix, is valid grammar so passes*/
   trtl->cw = 0;
   strcpy(string, "SET Z := A + A ;");
   testReadString(trtl, string, 7);
   assert(set(trtl));

   trtl->cw = 0;
   strcpy(string, "4 4 + A D / - + ;");
   testReadString(trtl, string, 9);
   assert(polish(trtl));

   /*Testing set*/
   trtl->cw = 0;
   strcpy(string, "SET A := A 5 + ;");
   testReadString(trtl, string, 7);
   assert(instruction(trtl));

   trtl->cw = 0;
   strcpy(string, "SET Z := J % + ;");
   testReadString(trtl, string, 7);
   assert(!instruction(trtl));

   trtl->cw = 0;
   strcpy(string, "WORD Q := E 5 + ;");
   testReadString(trtl, string, 7);
   assert(!instruction(trtl));

   trtl->cw = 0;
   strcpy(string, "SET A := B 5 + ;");
   testReadString(trtl, string, 7);
   assert(instruction(trtl));

   trtl->cw = 0;
   strcpy(string, "SET A =: B 5 + ;");
   testReadString(trtl, string, 7);
   assert(!instruction(trtl));

   trtl->cw = 0;
   strcpy(string, "SET % := B 5 + ;");
   testReadString(trtl, string, 7);
   assert(!instruction(trtl));

   /* testing do*/
   trtl->cw = 0;
   strcpy(string, "DO A FROM 5 TO 10 { FD 45 LT 30 }");
   testReadString(trtl, string, 12);
   assert(instruction(trtl));
   assert(instructLst(trtl));

   trtl->cw = 0;
   strcpy(string, "DO A FROM B TO C { FD D LT E }");
   testReadString(trtl, string, 12);
   assert(instruction(trtl));
   assert(instructLst(trtl));

   trtl->cw = 0;
   strcpy(string, "DO A FROM -5 TO -10.04 { FD 45 LT 30 }");
   testReadString(trtl, string, 12);
   assert(instruction(trtl));
   assert(instructLst(trtl));

   trtl->cw = 0;
   strcpy(string, "A FROM 5 TO 10 { FD 45 LT 30 }");
   testReadString(trtl, string, 11);
   assert(!instruction(trtl));

   trtl->cw = 0;
   strcpy(string, "DO FROM 5 TO 10 { FD 45 LT 30 }");
   testReadString(trtl, string, 11);
   assert(!instruction(trtl));

   trtl->cw = 0;
   strcpy(string, "DO A 5 TO 10 { FD 45 LT 30 }");
   testReadString(trtl, string, 11);
   assert(!instruction(trtl));

   trtl->cw = 0;
   strcpy(string, "DO A FROM TO 10 { FD 45 LT 30 }");
   testReadString(trtl, string, 11);
   assert(!instruction(trtl));

   trtl->cw = 0;
   strcpy(string, "DO A FROM 5 10 { FD 45 LT 30 }");
   testReadString(trtl, string, 11);
   assert(!instruction(trtl));

   trtl->cw = 0;
   strcpy(string, "DO A FROM 5 TO { FD 45 LT 30 }");
   testReadString(trtl, string, 11);
   assert(!instruction(trtl));

   trtl->cw = 0;
   strcpy(string, "DO A FROM 5 TO 10 FD 45 LT 30 }");
   testReadString(trtl, string, 11);
   assert(!instruction(trtl));

   trtl->cw = 0;
   strcpy(string, "DO A FROM 5 TO 10 { 45 LT 30 }");
   testReadString(trtl, string, 11);
   assert(!instruction(trtl));
   assert(!instructLst(trtl));

   trtl->cw = 0;
   strcpy(string, "DO A FROM 5 TO 10 { FD 45 LT 30");
   testReadString(trtl, string, 11);
   assert(!instructLst(trtl));

   trtl->cw = 0;
   strcpy(string, "DO a FROM 5 TO 10 { FD 45 LT 30 }");
   testReadString(trtl, string, 12);
   assert(!instructLst(trtl));

   trtl->cw = 0;
   strcpy(string, "DO A FROM 5 TO TEN { FD 45 LT 30 }");
   testReadString(trtl, string, 12);
   assert(!instructLst(trtl));

   trtl->cw = 0;
   strcpy(string, "DO A FROM B TO C { FD D LT E } }");
   testReadString(trtl, string, 13);
   assert(instructLst(trtl));

   trtl->cw = 0;
   trtlResize(trtl);
   strcpy(string, "DO A FROM 1 TO 5 { SET Z := 45 5 + ; LT Z } }");
   testReadString(trtl, string, 18);
   assert(instructLst(trtl));

   trtlFree(trtl);
   trtlInit(&trtl);

   strcpy(string, "FD 45 }");
   testReadString(trtl, string, 3);
   assert(instructLst(trtl));
   assert(!prog(trtl));

   trtl->cw = 0;
   strcpy(string, "SET A := 45 ; FD A }");
   testReadString(trtl, string, 8);
   assert(instruction(trtl));
   assert(!prog(trtl));

   trtl->cw = 0;
   strcpy(string, "DO A FROM 1 TO 8 { FD 30 LT 45 } }");
   testReadString(trtl, string, 13);
   assert(instructLst(trtl));
   assert(!prog(trtl));

   trtl->cw = 0;
   trtlResize(trtl);
   strcpy(string, "DO A FROM 1 TO 5 { SET Z := 45 5 + ; LT Z } }");
   testReadString(trtl, string, 18);
   assert(instructLst(trtl));
   assert(!prog(trtl));

   trtl->cw = 0;
   trtlResize(trtl);
   strcpy(string, "DO A FROM 1 TO 50 { FD A RT 30 DO B FROM \
      1 TO 8 { SET C := A 5 / ; FD C RT 45 } } }");
   testReadString(trtl, string, 32);
   assert(instructLst(trtl));
   assert(!prog(trtl));

   trtl->cw = 0;
   trtlResize(trtl);
   strcpy(string, "{ DO A FROM 1 TO 50 { FD A RT 30 DO B FROM \
      1 TO 8 { SET C := A 5 / ; FD C RT 45 } } }");
   testReadString(trtl, string, 33);
   assert(!instructLst(trtl));
   assert(prog(trtl));

   trtlFree(trtl);
   trtlInit(&trtl);

   trtl->cw = 0;
   strcpy(string, "{");
   testReadString(trtl, string, 1);
   assert(!prog(trtl));

   trtl->cw = 0;
   strcpy(string, "}");
   testReadString(trtl, string, 1);
   assert(!prog(trtl));

   strcpy(string, "{ }");
   testReadString(trtl, string, 2);
   assert(prog(trtl));

   trtl->cw = 0;
   strcpy(string, "{ } RT 45");
   testReadString(trtl, string, 4);
   assert(!prog(trtl));

   trtlFree(trtl);
 /*Moved onto black box testing with files at this point*/
}

void testReadString(turtle* trtl, char* str, int n)
{
   char *tok;
   int i = 0;

   tok = strtok(str, " ");
   strcpy(trtl->wds[i++], tok);
   while (tok != NULL && i < n){
      tok = strtok(NULL," ");
      strcpy(trtl->wds[i++], tok);
   }
}
