/* ===================================
   ASCII Tic Tac Toe
   =================================== */

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

/* The Tic-Tac-Toe 3x3 board */
char board[10];

/* 3^9 positions */
int tttPos[19683];

/* Set X's as 1 and O's as -1 */
enum { X = 1, O = -1 };

/* Declare states */
enum { Xwon = 1, Owon = -1, Cat = 2, None = 0 };

/* Human Player flag */
int hplayer = 0;
int diff = 0;

display_board() {
   char *brd = board;
   int i;
   /* Print out the playing board    */
   for (i = 2; i < 9; i += 3) {
      printf(" %c | %c | %c\n", brd[i-2], brd[i-1], brd[i]);
      if (i < 6)
         printf("------------\n");
   }
}

int analyze_board(char *board) {
   int i, out = 0, ex = 1;

   for (i = 0; i < 9; i++, ex *= 3) {
      switch(board[i]) {
      case '_':
         break;
      case 'X':
         out += ex;
         break;
      case 'O':
         out += ex * 2;
      }
   }

   return out;
}


int winner() {
   int i, notCat = 0;
   char *b = board;

   /* Check to see if X is the winner */
   if ((b[0] == 'X' && b[1] == 'X' && b[2] == 'X') ||
      (b[3] == 'X' && b[4] == 'X' && b[5] == 'X') ||
      (b[6] == 'X' && b[7] == 'X' && b[8] == 'X') || 
      (b[0] == 'X' && b[3] == 'X' && b[6] == 'X') || 
      (b[1] == 'X' && b[4] == 'X' && b[7] == 'X') || 
      (b[2] == 'X' && b[5] == 'X' && b[8] == 'X') || 
      (b[0] == 'X' && b[4] == 'X' && b[8] == 'X') || 
      (b[2] == 'X' && b[4] == 'X' && b[6] == 'X'))
      return Xwon;
   
   if ((b[0] == 'O' && b[1] == 'O' && b[2] == 'O') ||
      (b[3] == 'O' && b[4] == 'O' && b[5] == 'O') ||
      (b[6] == 'O' && b[7] == 'O' && b[8] == 'O') || 
      (b[0] == 'O' && b[3] == 'O' && b[6] == 'O') || 
      (b[1] == 'O' && b[4] == 'O' && b[7] == 'O') || 
      (b[2] == 'O' && b[5] == 'O' && b[8] == 'O') || 
      (b[0] == 'O' && b[4] == 'O' && b[8] == 'O') || 
      (b[2] == 'O' && b[4] == 'O' && b[6] == 'O'))
      return Owon;

   /* Check to see if it is a Cat game (Tie) */
   /* by searching the board for blanks      */
   for (i = 0; i < 9; i++) {
      if (b[i] == '_')
         notCat = 1;
   }
   /* No blanks on the board and no winner means Cat (Tie game) */
   if (!notCat)
      return Cat;

   /* If X/O didn't win and no Cat, then it's not over... */
   return None;

}

int minimax(int cplayer, int depth) {
   int ck, best;
   int index, i;

   index = analyze_board(board);
   if ((tttPos[index] % 10) >= depth)
      return (tttPos[index] - (tttPos[index] % 10));

   if (depth > diff)
      return 0;

   ck = winner();
   /* Check if won */
   if (ck != None) {
      switch(ck) {
      case Xwon:
      case Owon:
         return 1000 * ck;
      case Cat:
         return 0;
      }
   }

   best = -cplayer * 1000000;
   for (i=0; i<9; i++) {
      if (board[i] == '_') {
         board[i] = (cplayer == X) ? 'X' : 'O';
         ck = minimax(-cplayer, depth + 1);
      if (ck * cplayer > best * cplayer)
         best = ck;
      board[i] = '_';
      }
   }
  
   tttPos[index] = best + depth;
   return best;
}



computer_move()
{
   int ck, best, besti[9], i, bestcount = 0;
   int cplayer = -hplayer; /* set comp to be the other letter */

   best = -cplayer * 1000000;
   for (i = 0; i < 9; i++) {
      if (board[i] == '_') {
         board[i] = (cplayer == X) ? 'X' : 'O';
         ck = minimax(-cplayer, 1);
         if (ck == best) {
	        best = ck;
            besti[bestcount++] = i;
         }
         else if (ck * cplayer > best * cplayer) {
	        best = ck;
	        bestcount = 0;
            besti[bestcount++] = i;
         }
         board[i] = '_';
      }
   }
   i = 0;

   if(cplayer == X)
      board[besti[i]] = 'X';
   else
	  board[besti[i]] = 'O';
}

int main() {
   int status, choice, validchoice = 0;
   char XorO;

   /* Copy numbered sequence to board to show rules */
   strcpy(board, "012345678");
   /* Show how to play the game at the very start   */
   printf("TIC TAC TOE\n");
   printf("===========\n\n");
   printf("How to play: This game has 2 players.\n");
   printf("One chooses to play X's and the other chooses to play O's.\n");
   printf("The point of the game is to get 3 in a row of whichever\n");
   printf("letter you chose, horizontally, vertically, or diagonally.\n");
   printf("In this program, the computer will be one of the players.\n");
   printf("The playing board looks like this:\n\n");
   display_board();
   printf("\nWhen prompted for a choice to place your letter, please\n");
   printf("use the number above that corresponds to the place where\n");
   printf("you wish to place your letter as the input.\n\n");
   printf("\nReady?  Lets play!\n\n");

   /* Prompt user to pick X or O */
   printf("Which one would you like?  X or O: ");
   scanf("%c", &XorO);

   /* Check and make sure they entered in an X or an O */
   while(XorO != 'X' && XorO != 'O') {
      printf("\nInvalid choice.  Please enter either X or O.\n");
      printf("It is case sensitive, so please use capital letters.\n");
      printf("X or O? ");
      scanf("%c", &XorO);
   }

   if (XorO == 'X') 
      hplayer = X;
   else
      hplayer = O;

   /* Player has picked either X's or O's   */
   /* We are ready to start the game.       */
   /* First thing is to empty out the board */
   /* I use the _ as a blank                */
   strcpy(board, "_________");

   while((status = winner()) == None) {
      /* First move */
      if(strcmp(board, "_________") == 0 && XorO != 'X') {
         /* If player isn't X, computer player gets FIRST MOVE */
         /* Get move and show board */
         computer_move();
         display_board();
      }
      /* Prompt for human player to make a move */
      printf("5");
      if(hplayer == X)
         printf("Where do you want to place your X? ");
      else 
         printf("Where do you want to place your O? ");
      scanf("%d", &choice);

      while (!validchoice) {
         if(choice <= 8 && choice >= 0)
            if(board[choice] == '_') {
               board[choice] = (hplayer == X) ? 'X' : 'O';
               validchoice = 1;
            }
            else {
               printf("Illegal Move! There is a %c there", board[choice]);
               printf("\nPlease choose again: ");
               scanf("%d", &choice);
            }
         else {
            printf("Please input a choice (0-8): ");
            scanf("%d", &choice);    
         }
      }
      /* Reset validchoice */
      validchoice = 0;
      if((status = winner()) == None) {
         computer_move();
         display_board();
      }
      else {
         printf("GAME OVER!  %d", status);
      }
   }
   return 0;

}
