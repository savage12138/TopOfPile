/*	               
Author: Yida Wang
Last date modified: September 27th, 2019

This program loads a pile and finds the color of the topmost piece. 
*/

#include <stdio.h>
#include <stdlib.h>

#define DEBUG 0 
//             U3
//             U2
//             U1
//    L3 L2 L1  i R1 R2 R3 
//             D1
//             D2
//             D3
// i is the current location
int main(int argc, char *argv[]) {
   int	             PileInts[1024];
   int	             NumInts, TopColor=0;
   int  Load_Mem(char *, int *);
   // This allows you to access the pixels (individual bytes)
   // as byte array accesses (e.g., Pile[25] gives pixel 25):
   char *Pile = (char *)PileInts;

   if (argc != 2) {
     printf("usage: ./P1-1 valuefile\n");
     exit(1);
   }
   NumInts = Load_Mem(argv[1], PileInts);
   if (NumInts != 1024) {
      printf("valuefiles must contain 1024 entries\n");
      exit(1);
   }

   if (DEBUG){
     printf("Pile[0] is Pixel 0: 0x%02x\n", Pile[0]);
     printf("Pile[107] is Pixel 107: 0x%02x\n", Pile[107]);
   }

    // initialize variables
    int color[7];         // 7 colors total
    int count = 0;        // counting number of colors
    for (int i = 0; i < 4096; i++)                  // check all the pixels to maximize accuracy
    {
       if (count == 6)    // found 6 colors already
        {
            for (int j = 1; j < 8; j++)
                if (color[j] == 0) TopColor = j;    // top color is the last color have not been selected
            break;
        }
        if (Pile[i] != 0 && color[(int)Pile[i]] == 0) 
          // convert pointer back to int for color.
          // if pixel i is not black and pixel i is not been selected yet
        {
            // check up
            if (Pile[i - 64] != Pile[i] && (Pile[i - 64] != 0 && i > 127 && Pile[i - 128] == Pile[i]) && (Pile[i - 128 - 64] == Pile[i - 128] || Pile[i] == Pile[i + 64]))
              // if U1 does not have the same color as i 
              // and (U1 is not black and i is below first two row and U2 has the same color as i)
              // and (U3 has the same color as U2 or i has the same color as D1)
            { 
              // current location is below U1
                color[(int)Pile[i]] = -1;           // mark current color as -1        
                count++;                            // go to next color
                continue;                           
            }
            // check down
            if (i + 64 < 4096 && (Pile[i + 64] != Pile[i]) && (Pile[i + 64] != 0 && i + 128 < 4096 && Pile[i + 128] == Pile[i]) && (Pile[i + 128 + 64] == Pile[i + 128] || Pile[i] == Pile[i - 64]))
              // if D1 is within the range
              // and D1 does not have the same color as i
              // and (D1 is not black and D2 is within the range and D2 has the same color as i)
              // and (D3 has the same color as D2 or i has the same color as U1)
            {   
                color[(int)Pile[i]] = -1;           // mark current color as -1
                count++;                            // go to next color
                continue;
            }
            // check left
            if (i % 64 != 0 && (Pile[i - 1] != 0 && (i - 1) % 64 != 0 && Pile[i - 2] == Pile[i]) && (Pile[i - 1] != Pile[i]) && (Pile[i - 2] == Pile[i - 3] || Pile[i + 1] == Pile[i]))
              // if i is not on the far left
              // and (L1 is not black and L1 is not on the far left and L2 has the same color as i)
              // and L1 does not have the same color as i
              // and (L2 has the same color as L3 or i has the same color as R1)
            {     
                color[(int)Pile[i]] = -1;           // mark current color as -1
                count++;                            // go to next color
                continue;
            }
            // check right
            if ((i + 1) % 64 != 0 && (Pile[i + 1] != Pile[i]) && (Pile[i + 1] != 0 && (i + 2) % 64 != 0 && Pile[i + 2] == Pile[i])&&(Pile[i + 2] == Pile[i + 3] || Pile[i] == Pile[i - 1])) 
              // if i is not on the far right 
              // and R1 does not have the same color as i
              // and (R1 is not black and R1 is not on the far right and R2 has the same color as i)
              // and (R2 has the same color as R3 or i has the same color as L1)
            {    
                color[(int)Pile[i]] = -1;           // mark current color as -1
                count++;                            // go to next color
                continue;
            }
        }
    }
    printf("The topmost part color is: %d\n", TopColor);
    exit(0);
}

/* This routine loads in up to 1024 newline delimited integers from
a named file in the local directory. The values are placed in the
passed integer array. The number of input integers is returned. */

int Load_Mem(char *InputFileName, int IntArray[]) {
   int	N, Addr, Value, NumVals;
   FILE	*FP;

   FP = fopen(InputFileName, "r");
   if (FP == NULL) {
      printf("%s could not be opened; check the filename\n", InputFileName);
      return 0;
   } else {
      for (N=0; N < 1024; N++) {
         NumVals = fscanf(FP, "%d: %d", &Addr, &Value);
         if (NumVals == 2)
            IntArray[N] = Value;
         else
            break;
      }
      fclose(FP);
      return N;
   }
}
