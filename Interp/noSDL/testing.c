#include "turtle.h"

void test(void)
{
   int j;
   turtle* trtl;
   char string[250];
   stack* s;
   double sum;

   s = stack_init();
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
   /*+ should return 1*/
   trtl->cw = 0;
   assert(op(trtl) == 1);
   /* - should return 2*/
   trtl->cw++;
   assert(op(trtl) == 2);
   /* * should return 3*/
   trtl->cw++;
   assert(op(trtl) == 3);
   /* / should return 4*/
   trtl->cw++;
   assert(op(trtl) == 4);
   /*everything else = 0 */
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

   /*testing FD/RT/LT  instruction*/
   strcpy(trtl->wds[0], "FD");
   strcpy(trtl->wds[1], " ");
   trtl->cw = 0;
   assert(!instruction(trtl, s));
   assert(!convertAngle(trtl));

   strcpy(trtl->wds[0], "FD");
   strcpy(trtl->wds[1], ".");
   trtl->cw = 0;
   assert(!instruction(trtl, s));

   /*When intialised trtl->y is set to 300 (middle of screen)*/
   assert(((int)trtl->y) == 300);
   strcpy(string, "FD 50");
   testReadString(trtl, string, 2);
   trtl->cw = 0;
   assert(instruction(trtl, s));
   /*no angle, turtle should move up 50 pixels*/
   assert(((int)trtl->y) == 250);

   /*fails assert as variable A is not currently set as used*/
   strcpy(string, "FD A");
   testReadString(trtl, string, 2);
   trtl->cw = 0;
   assert(!instruction(trtl, s));

   /*Once set, the assert passes*/
   trtl->varUsed[0] = true;
   trtl->varArr[0] = 25;
   strcpy(string, "FD A");
   testReadString(trtl, string, 2);
   trtl->cw = 0;
   assert(instruction(trtl, s));
   /*A is set to 25, so should move 25 north*/
   assert(((int)trtl->y) == 225);

   strcpy(string, "FD -4.5");
   testReadString(trtl, string, 2);
   trtl->cw = 0;
   assert(instruction(trtl, s));

   trtl->cw = 0;
   strcpy(string, "FD 7-5");
   testReadString(trtl, string, 2);
   trtl->cw = 0;
   assert(!instruction(trtl, s));

   trtl->cw = 0;
   strcpy(string, "FD AA");
   testReadString(trtl, string, 2);
   trtl->cw = 0;
   assert(!instruction(trtl, s));

   trtl->cw = 0;
   strcpy(trtl->wds[0], "RT");
   strcpy(trtl->wds[1], " ");
   trtl->cw = 0;
   assert(!instruction(trtl, s));

   trtl->cw = 0;
   trtl->angle = 0;
   strcpy(string, "RT 50");
   testReadString(trtl, string, 2);
   trtl->cw = 0;
   assert(instruction(trtl, s));
   assert(((int)trtl->angle) == 50);

   trtl->cw = 0;
   /*Setting A to 25 degrees*/
   trtl->varArr[0] = 25;
   strcpy(string, "RT A");
   testReadString(trtl, string, 2);
   trtl->cw = 0;
   assert(instruction(trtl, s));
   assert(((int)trtl->angle) == 75);

   trtl->cw = 0;
   strcpy(string, "RT -4.5");
   testReadString(trtl, string, 2);
   trtl->cw = 0;
   assert(instruction(trtl, s));
   assert((trtl->angle > 70.499) && (trtl->angle < 70.501));

   trtl->cw = 0;
   strcpy(string, "RT a");
   testReadString(trtl, string, 2);
   trtl->cw = 0;
   assert(!instruction(trtl, s));

   trtl->cw = 0;
   strcpy(trtl->wds[0], "LT");
   strcpy(trtl->wds[1], " ");
   trtl->cw = 0;
   assert(!instruction(trtl, s));
   assert(!convertAngle(trtl));

   trtl->cw = 0;
   trtl->angle = 0;
   strcpy(string, "LT 50");
   testReadString(trtl, string, 2);
   trtl->cw = 0;
   assert(instruction(trtl, s));
   assert(!instructLst(trtl, s));
   /* 360 - 50 = 310 */
   assert(((int)trtl->angle) == 310);

   /*Fails instructLst as no } at the end*/
   trtl->cw = 0;
   trtl->varUsed[25] = true;
   trtl->varArr[25] = 10;
   strcpy(string, "LT Z");
   testReadString(trtl, string, 2);
   trtl->cw = 0;
   assert(instruction(trtl, s));
   assert(!instructLst(trtl, s));
   assert(((int)trtl->angle) == 300);


   trtl->cw = 0;
   strcpy(string, "LT -7.1");
   testReadString(trtl, string, 2);
   trtl->cw = 0;
   assert(instruction(trtl, s));
   assert(!instructLst(trtl, s));
   assert((trtl->angle > 307.09) && (trtl->angle < 307.101));

   trtl->cw = 0;
   strcpy(string, "LT ZZ");
   testReadString(trtl, string, 2);
   trtl->cw = 0;
   assert(!instruction(trtl, s));
   assert(!instructLst(trtl, s));

   /*Fails prog as there is no leading { */
   trtl->cw = 0;
   strcpy(string, "FD 30 LT 45 }");
   testReadString(trtl, string, 5);
   assert(instructLst(trtl, s));
   assert(!prog(trtl, s));

   trtl->cw = 0;
   strcpy(string, "FD 45 LT A RT -4.5 }");
   testReadString(trtl, string, 7);
   assert(instructLst(trtl, s));
   assert(!prog(trtl, s));

   trtl->cw = 0;
   strcpy(string, "FD 45 LT RT 56 }");
   testReadString(trtl, string, 6);
   assert(!instructLst(trtl, s));

   trtl->cw = 0;
   strcpy(string, "FD 4-5 LT 45 RT 56 }");
   testReadString(trtl, string, 6);
   assert(!instructLst(trtl, s));

   trtlFree(trtl);
   trtlInit(&trtl);

   trtl->cw = 0;
   strcpy(string, "{ FD 30 LT 4.5 RT -45 }");
   testReadString(trtl, string, 8);
   assert(prog(trtl, s));

   trtl->cw = 0;
   strcpy(string, "{ FD 30 LT 4.5 RT -45 }");
   testReadString(trtl, string, 8);
   assert(prog(trtl, s));

   trtlFree(trtl);
   trtlInit(&trtl);

   strcpy(trtl->wds[0], ";");
   assert(!polish(trtl, s, 0));

   trtl->cw = 0;
   strcpy(trtl->wds[0], "+");
   strcpy(trtl->wds[1], ";");
   assert(!polish(trtl, s, 0));

   /*Testing basic polish*/
   trtl->varUsed[0] = true;
   trtl->varUsed[25] = true;
   trtl->cw = 0;
   strcpy(string, "A Z + ;");
   testReadString(trtl, string, 4);
   assert(polish(trtl, s, 0));
   /*changing sign from prev equation*/
   trtl->cw = 0;
   strcpy(trtl->wds[2], "-");
   assert(polish(trtl, s, 0));

   /*fails as variables are set to zero due to calloc*/
   trtl->cw = 0;
   strcpy(trtl->wds[2], "/");
   assert(!polish(trtl, s, 0));

   /*passes once they are set*/
   trtl->cw = 0;
   trtl->varArr[0] = 10;
   trtl->varArr[25] = 2;
   assert(polish(trtl, s, 0));

   trtl->cw = 0;
   strcpy(trtl->wds[2], "*");
   assert(polish(trtl, s, 0));

   /*fails with invalid operator*/
   trtl->cw = 0;
   strcpy(trtl->wds[2], "%");
   assert(!polish(trtl, s, 0));
   assert(resetStack(s));

   /*testing simple polish with numbers*/
   trtl->cw = 0;
   strcpy(string, "10 6 + ;");
   testReadString(trtl, string, 4);
   assert(polish(trtl, s, 0));
   /*changing sign of previous expression*/
   trtl->cw = 0;
   strcpy(trtl->wds[2], "-");
   assert(polish(trtl, s, 0));

   trtl->cw = 0;
   strcpy(trtl->wds[2], "/");
   assert(polish(trtl, s, 0));

   trtl->cw = 0;
   strcpy(trtl->wds[2], "*");
   assert(polish(trtl, s, 0));

   /*fails with invalid operator*/
   trtl->cw = 0;
   strcpy(trtl->wds[2], "%");
   assert(!polish(trtl, s, 0));
   assert(resetStack(s));

   trtl->cw = 0;
   strcpy(string, "5 5 + ;");
   testReadString(trtl, string, 4);
   assert(polish(trtl, s, 0));

   /*more complex polish*/
   for(j=0; j<26;j++){
      trtl->varUsed[j] = true;
      trtl->varArr[j] = rand()%250;
   }

   trtl->cw = 0;
   strcpy(string, "J 2 / -4 5.1 6.5 + * - ;");
   testReadString(trtl, string, 10);
   assert(polish(trtl, s, 0));

   trtlResize(trtl);
   trtl->cw = 0;
   strcpy(string, "R D / F + T * Z / A * D - ;");
   testReadString(trtl, string, 14);
   assert(polish(trtl, s, 0));

   trtl->cw = 0;
   strcpy(string, "J 2 / -4 5.1 6.5 + * - 5 * 6 245 / + ;");
   testReadString(trtl, string, 16);
   assert(polish(trtl, s, 0));

   trtl->cw = 0;
   strcpy(string, "J 2 / -4 5.1 6.5 + * - 5 * 6 245 50 + ;");
   testReadString(trtl, string, 16);
   assert(!polish(trtl, s, 0));

   trtlFree(trtl);
   trtlInit(&trtl);
   for(j=0; j<26;j++){
      trtl->varUsed[j] = true;
      trtl->varArr[j] = rand()%250;
   }

   /*Testing Polish Notation and other incorrect RPN*/
   trtl->cw = 0;
   strcpy(string, "+ A A ;");
   testReadString(trtl, string, 4);
   assert(!polish(trtl, s, 0));
   resetStack(s);

   /*too many operators*/
   trtl->cw = 0;
   strcpy(string, "A A + + ;");
   testReadString(trtl, string, 5);
   assert(!polish(trtl, s, 0));
   resetStack(s);

   /*too many operands*/
   trtl->cw = 0;
   strcpy(string, "1 5 10 * ;");
   testReadString(trtl, string, 5);
   assert(!polish(trtl, s, 0));
   resetStack(s);

   /*NPN*/
   trtl->cw = 0;
   strcpy(string, "+ 4 5 ;");
   testReadString(trtl, string, 4);
   assert(!polish(trtl, s, 0));
   resetStack(s);

   trtl->cw = 0;
   strcpy(string, "* Q 5 ;");
   testReadString(trtl, string, 4);
   assert(!polish(trtl, s, 0));
   resetStack(s);

   trtl->cw = 0;
   strcpy(string, "/ 4 L ;");
   testReadString(trtl, string, 4);
   assert(!polish(trtl, s, 0));
   resetStack(s);

   trtl->cw = 0;
   strcpy(string, "- A Z ;");
   testReadString(trtl, string, 4);
   assert(!polish(trtl, s, 0));
   resetStack(s);

   /*missing semi colon as a terminator*/
   trtl->cw = 0;
   strcpy(string, "J 2 / -4 5.1 6.5 + * }");
   testReadString(trtl, string, 9);
   assert(!polish(trtl, s, 0));
   resetStack(s);

   /*basic infix fails*/
   trtl->cw = 0;
   strcpy(string, "5 + 5 ;");
   testReadString(trtl, string, 4);
   assert(!polish(trtl, s, 0));
   resetStack(s);

   trtl->cw = 0;
   strcpy(string, "A - 13 ;");
   testReadString(trtl, string, 4);
   assert(!polish(trtl, s, 0));
   resetStack(s);

   trtl->cw = 0;
   strcpy(string, "99 * V ;");
   testReadString(trtl, string, 4);
   assert(!polish(trtl, s, 0));
   resetStack(s);

   trtl->cw = 0;
   strcpy(string, "R + U ;");
   testReadString(trtl, string, 4);
   assert(!polish(trtl, s, 0));
   resetStack(s);

   /*changing sign of prev expression*/
   trtl->cw = 0;
   strcpy(trtl->wds[1], "-");
   assert(!polish(trtl, s, 0));
   resetStack(s);

   trtl->cw = 0;
   strcpy(trtl->wds[1], "*");
   assert(!polish(trtl, s, 0));
   resetStack(s);

   trtl->cw = 0;
   strcpy(trtl->wds[1], "%");
   assert(!polish(trtl, s, 0));
   resetStack(s);

   trtl->cw = 0;
   strcpy(string, "R + U * 5 ;");
   testReadString(trtl, string, 6);
   assert(!polish(trtl, s, 0));
   resetStack(s);

   trtl->cw = 0;
   strcpy(string, "$ + + ;");
   testReadString(trtl, string, 4);
   assert(!polish(trtl, s, 0));

   trtlFree(trtl);
   trtlInit(&trtl);
   for(j=0; j<26;j++){
      trtl->varUsed[j] = true;
      trtl->varArr[j] = rand()%250;
   }

   strcpy(string, "SET D := 4 4 + ;");
   testReadString(trtl, string, 7);
   assert(set(trtl, s));

   trtl->cw = 0;
   strcpy(string, "SET D := 4 4 + + ;");
   testReadString(trtl, string, 7);
   assert(!set(trtl, s));
   resetStack(s);

   trtl->cw = 0;
   strcpy(string, "SET A := 5 5 + ;");
   testReadString(trtl, string, 7);
   assert(instruction(trtl, s));

   trtl->cw = 0;
   strcpy(string, "SET Z := J % + ;");
   testReadString(trtl, string, 7);
   assert(!instruction(trtl, s));
   resetStack(s);

   trtl->cw = 0;
   strcpy(string, "WORD Q := E 5 + ;");
   testReadString(trtl, string, 7);
   assert(!instruction(trtl, s));
   resetStack(s);

   trtl->cw = 0;
   strcpy(string, "SET A := B 5 + ;");
   testReadString(trtl, string, 7);
   assert(instruction(trtl, s));
   resetStack(s);

   trtl->cw = 0;
   strcpy(string, "SET A =: B 5 + ;");
   testReadString(trtl, string, 7);
   assert(!instruction(trtl, s));
   resetStack(s);

   trtl->cw = 0;
   strcpy(string, "SET % := B 5 + ;");
   testReadString(trtl, string, 7);
   assert(!instruction(trtl, s));
   resetStack(s);

   trtl->cw = 0;
   strcpy(string, "DO A FROM 5 TO 10 { FD 45 LT 30 }");
   testReadString(trtl, string, 12);
   assert(instruction(trtl, s));
   assert(instructLst(trtl, s));

   trtl->cw = 0;
   strcpy(string, "DO A FROM B TO C { FD D LT E }");
   testReadString(trtl, string, 12);
   assert(instruction(trtl, s));
   assert(instructLst(trtl, s));

   trtl->cw = 0;
   strcpy(string, "DO A FROM -5 TO -10.04 { FD 45 LT 30 }");
   testReadString(trtl, string, 12);
   assert(instruction(trtl, s));
   assert(instructLst(trtl, s));

   trtl->cw = 0;
   strcpy(string, "A FROM 5 TO 10 { FD 45 LT 30 }");
   testReadString(trtl, string, 11);
   assert(!instruction(trtl, s));
   resetStack(s);

   trtl->cw = 0;
   strcpy(string, "DO FROM 5 TO 10 { FD 45 LT 30 }");
   testReadString(trtl, string, 11);
   assert(!instruction(trtl, s));
   resetStack(s);

   trtl->cw = 0;
   strcpy(string, "DO A 5 TO 10 { FD 45 LT 30 }");
   testReadString(trtl, string, 11);
   assert(!instruction(trtl, s));
   resetStack(s);

   trtl->cw = 0;
   strcpy(string, "DO A FROM TO 10 { FD 45 LT 30 }");
   testReadString(trtl, string, 11);
   assert(!instruction(trtl, s));
   resetStack(s);

   trtl->cw = 0;
   strcpy(string, "DO A FROM 5 10 { FD 45 LT 30 }");
   testReadString(trtl, string, 11);
   assert(!instruction(trtl, s));

   trtl->cw = 0;
   strcpy(string, "DO A FROM 5 TO { FD 45 LT 30 }");
   testReadString(trtl, string, 11);
   assert(!instruction(trtl, s));
   resetStack(s);

   trtl->cw = 0;
   strcpy(string, "DO A FROM 5 TO 10 FD 45 LT 30 }");
   testReadString(trtl, string, 11);
   assert(!instruction(trtl, s));
   resetStack(s);

   trtl->cw = 0;
   strcpy(string, "DO A FROM 5 TO 10 { 45 LT 30 }");
   testReadString(trtl, string, 11);
   assert(instruction(trtl, s));

   trtl->cw = 0;
   strcpy(string, "DO A FROM 5 TO 10 { FD 45 LT 30");
   testReadString(trtl, string, 11);
   assert(!instructLst(trtl, s));
   resetStack(s);

   trtl->cw = 0;
   trtlResize(trtl);
   strcpy(string, "DO A FROM 1 TO 5 { SET Z := 45 5 + ; LT Z } }");
   testReadString(trtl, string, 18);
   assert(instructLst(trtl, s));
   assert(!prog(trtl, s));

   trtlFree(trtl);
   trtlInit(&trtl);

   strcpy(string, "FD 45 }");
   testReadString(trtl, string, 3);
   assert(instructLst(trtl, s));
   assert(!prog(trtl, s));

   trtl->cw = 0;
   strcpy(string, "DO A FROM 1 TO 8 { FD 30 LT 45 } }");
   testReadString(trtl, string, 13);
   assert(instructLst(trtl, s));
   assert(!prog(trtl, s));

   trtl->cw = 0;
   trtlResize(trtl);
   trtlResize(trtl);
   strcpy(string, "DO A FROM 1 TO 50 { FD A RT 30 DO B FROM \
                   1 TO 8 { SET C := A 5 / ; FD C RT 45 } } }");
   testReadString(trtl, string, 32);
   assert(instructLst(trtl, s));
   assert(!prog(trtl, s));

   trtl->cw = 0;
   trtlResize(trtl);
   strcpy(string, "{ DO A FROM 1 TO 50 { FD A RT 30 DO B FROM \
      1 TO 8 { SET C := A 5 / ; FD C RT 45 } } }");
   testReadString(trtl, string, 33);
   assert(!instructLst(trtl, s));
   assert(prog(trtl, s));

   trtlFree(trtl);
   trtlInit(&trtl);

   strcpy(string, "{");
   testReadString(trtl, string, 1);
   assert(!prog(trtl, s));

   trtl->cw = 0;
   strcpy(string, "}");
   testReadString(trtl, string, 1);
   assert(!prog(trtl, s));

   trtl->cw = 0;
   strcpy(string, "{ }");
   testReadString(trtl, string, 2);
   assert(prog(trtl, s));

   trtl->cw = 0;
   strcpy(string, "{ } RT 45");
   testReadString(trtl, string, 4);
   assert(!prog(trtl, s));

   trtlFree(trtl);
   trtlInit(&trtl);
   trtlResize(trtl);

   strcpy(trtl->wds[0], "LT");
   strcpy(trtl->wds[1], "A");
   trtl->varArr[0] = 25;
   trtl->varUsed[0] = true;
   trtl->cw = 1;
   assert(convertAngle(trtl));
   assert((int)(parseVariable(trtl)) == 25);
   trtl->varArr[0] = -25;
   assert((int)(parseVariable(trtl)) == -25);
   trtl->varArr[0] = 25.125;
   assert(((parseVariable(trtl)) > 25.124) &&
           (parseVariable(trtl) < 25.126));


   strcpy(trtl->wds[1], "a");
   assert(!convertAngle(trtl));
   strcpy(trtl->wds[1], "%");
   assert(!convertAngle(trtl));
   strcpy(trtl->wds[1], "1");
   assert(convertAngle(trtl));
   strcpy(trtl->wds[1], "45");
   assert(convertAngle(trtl));
   strcpy(trtl->wds[1], "-45");
   assert(convertAngle(trtl));
   strcpy(trtl->wds[1], "0.5");
   assert(convertAngle(trtl));
   strcpy(trtl->wds[1], "4.5");
   assert(convertAngle(trtl));

   /*testing storeAngle*/
   trtl->angle = 90;
   strcpy(trtl->wds[0], "LT");
   strcpy(trtl->wds[1], "45");
   trtl->cw = 1;
   storeAngle(trtl, 45);
   assert((trtl->angle > 44.999999) & (trtl->angle < 45.000001));

   strcpy(trtl->wds[0], "RT");
   storeAngle(trtl, 45);
   assert((trtl->angle > 89.999999) & (trtl->angle < 90.000001));

   /*should only work on LT and RT*/
   strcpy(trtl->wds[0], "FD");
   assert(!storeAngle(trtl, 45));

   /*ensuring it loops around after 360 330 + 45 = 15 degrees*/
   strcpy(trtl->wds[0], "RT");
   trtl->cw = 1;
   trtl->angle = 330;
   assert(storeAngle(trtl, 45));
   assert((trtl->angle > 14.999999) & (trtl->angle < 15.000001));

   /*ensuring it loops around after 0 -  15 - 45 = 330 */
   strcpy(trtl->wds[0], "LT");
   trtl->cw = 1;
   trtl->angle = 15;
   assert(storeAngle(trtl, 45));
   assert((trtl->angle > 329.999999) & (trtl->angle < 330.000001));

   /*Checking incLoop() and decLoop() from inside _do()*/
   /*pushing "varIndex", "from" and then "to" onto stack*/
   trtl->cw = 0;
   strcpy(string, "0 1 10");
   testPushToStack(s, string, 3);
   strcpy(string, "FD 50 LT 45 }");
   testReadString(trtl, string, 5);
   assert(incLoop(trtl, s));
   resetStack(s);

   trtl->cw = 0;
   strcpy(string, "0 15 1");
   testPushToStack(s, string, 3);
   strcpy(string, "FD 50 LT 45 }");
   testReadString(trtl, string, 5);
   assert(decLoop(trtl, s));
   resetStack(s);

   trtl->cw = 0;
   strcpy(string, "0 1 10");
   testPushToStack(s, string, 3);
   strcpy(string, "SET A := A + 1 ; }");
   testReadString(trtl, string, 8);
   assert(incLoop(trtl, s));
   resetStack(s);

   trtl->cw = 0;
   strcpy(string, "0 20 1");
   testPushToStack(s, string, 3);
   strcpy(string, "SET A := A + 1 ; }");
   testReadString(trtl, string, 8);
   assert(decLoop(trtl, s));
   resetStack(s);

   /*not enough on the stack*/
   trtl->cw = 0;
   strcpy(string, "0 20");
   testPushToStack(s, string, 2);
   strcpy(string, "SET A := A + 1 ; }");
   testReadString(trtl, string, 8);
   assert(!decLoop(trtl, s));
   resetStack(s);

   trtlFree(trtl);
   trtlInit(&trtl);

   /*testing setVariable()*/
   /*setting VAR G to unused*/
   trtl->varUsed[6] = false;
   strcpy(trtl->wds[0], "G");
   assert(setVariable(trtl) == 6);
   /* "G" should now be true */
   assert(trtl->varUsed[6]);

   trtl->cw = 0;
   /*setting VAR Z to used to check for undefined behavior
   when setting to true twice*/
   trtl->varUsed[25] = true;
   strcpy(trtl->wds[0], "Z");
   assert(setVariable(trtl) == 25);
   /* "G" should now be true */
   assert(trtl->varUsed[6]);

   /* testing calculateRPN */
   /* 4 / 2 = 2 */
   trtl->cw = 0;
   strcpy(string, "4 2");
   testPushToStack(s, string, 2);
   strcpy(trtl->wds[0], "/");
   assert(calculateRPN(trtl, s));
   stack_pop(s, &sum);
   assert((sum > 1.9999999999999) & (sum < 2.0000000001));
   assert(s->size == 0);

   /* Checking that / 0 = Fail */
   trtl->cw = 0;
   strcpy(string, "4 0");
   testPushToStack(s, string, 2);
   strcpy(trtl->wds[0], "/");
   assert(!calculateRPN(trtl, s));
   assert(s->size == 0);

   /* 16 - 4 = 12*/
   trtl->cw = 0;
   strcpy(string, "16 4");
   testPushToStack(s, string, 2);
   strcpy(trtl->wds[0], "-");
   assert(calculateRPN(trtl, s));
   stack_pop(s, &sum);
   assert((sum > 11.9999999999999) & (sum < 12.0000000001));
   assert(s->size == 0);

   /* 14 + 2 = 16 */
   trtl->cw = 0;
   strcpy(string, "14 2");
   testPushToStack(s, string, 2);
   strcpy(trtl->wds[0], "+");
   assert(calculateRPN(trtl, s));
   stack_pop(s, &sum);
   assert((sum > 15.9999999999999) & (sum < 16.0000000001));
   assert(s->size == 0);

   /* 25 * 2 = 50 */
   trtl->cw = 0;
   strcpy(string, "25 2");
   testPushToStack(s, string, 2);
   strcpy(trtl->wds[0], "*");
   assert(calculateRPN(trtl, s));
   stack_pop(s, &sum);
   assert((sum > 49.9999999999999) & (sum < 50.0000000001));
   assert(s->size == 0);

   /*testing parseVarnum */
   trtl->cw = 0;
   strcpy(trtl->wds[0], "45");
   assert(parseVarnum(trtl) == 45);
   strcpy(trtl->wds[0], "-45");
   assert(parseVarnum(trtl) == -45);
   trtl->varUsed[1] = true;
   trtl->varArr[1] = 30;
   strcpy(trtl->wds[0], "B");
   assert(parseVarnum(trtl) == 30);
   trtl->varArr[1] = -30;
   assert(parseVarnum(trtl) == -30);

   assert(chkDivByZero(1));
   assert(!chkDivByZero(0));
   assert(chkDivByZero(-1));

   stack_free(s);
   trtlFree(trtl);
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

void testPushToStack(stack* s, char* str, int n)
{
   char *tok;
   int tmp, i = 1;

   tok = strtok(str, " ");

   tmp = atoi(tok);
   stack_push(s, tmp);

   while (tok != NULL && i < n){
      tok = strtok(NULL," ");
      tmp = atoi(tok);
      stack_push(s, tmp);
      i++;
   }
}
