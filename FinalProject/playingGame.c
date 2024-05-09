#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "board.h"
#include "pieces.h"
#include "playingGame.h"
#include "endingGame.h"

void playGame(int redgoesFirst, struct Player *blackteam, struct Player *redteam)
{   
    int turn = 1;
    if(!redgoesFirst) //if black team goes first
    {
        do
        {
            if(turn%2 == 1)
            {
                printf("It's black team's turn!\n");
                // printf("x: %dy: %d\n", blackteam->soldiers[2].x, blackteam->soldiers[2].y);
                userMove(blackteam, redteam); //black team goes, then red team goes
                boardPrint();
            }
            else
            {
                printf("It's red team's turn!\n");
                userMove(redteam, blackteam);
                boardPrint();
            }
            turn++;
        } while (!endGame(blackteam, redteam));
    }
    else
    {
        do
        {
            if(turn%2 == 1)
            {
                printf("It's red team's turn!\n");
                userMove(redteam, blackteam);
                boardPrint();
            }
            else
            {
                printf("It's black team's turn!\n");
                userMove(blackteam, redteam); //black team goes, then red team goes
                boardPrint();
            }
            turn++;
        } while (!endGame(blackteam, redteam));
    }
}
void userMove(struct Player *player, struct Player *otherplayer)
{
    int isValid = 1;
    int startx, starty, endx, endy;
    char piece[2];
    int checkTrue = checkPrevention(player, otherplayer, player->king.x, player->king.y, 0);
    printf("Pieces are represented by B or R (Black or Red) followed by their abbreviated piece name.\n");
    printf("Chariot: C\tHorse: H\tElephant: E\tGuard: G\tKing: K\t\tCannon/Artillery: A\tSoldier: S\n");
    do
    {
        if(checkTrue)
        {
            printf("You are currently in check!\n");
        }
        printf("What piece would you like to move, and where? (Format: piecename start(x) start(y) end(x) end(y))\n");
        scanf("%s", piece);
        scanf(" %d %d %d %d", &startx, &starty, &endx, &endy);
        getchar();
        if((piece[0] == 'B' || piece[0] == 'R') && (piece[1] == 'C' || piece[1] == 'H' || piece[1] == 'E' || piece[1] == 'G' || piece[1] == 'K' 
        || piece[1] == 'A' || piece[1] == 'S') && startx >= 0 && startx <= 8 && starty >= 0 && starty <= 9 && endx >= 0 && endx <= 8 && endy >= 0 && endy <= 9
        && (startx != endx || starty != endy))
        {
            if((player->team=='B' && piece[0] == 'R') || player->team=='R' && piece[0] == 'B') //check for making sure you cannot move the enemy team's pieces
            {
                printf("Invalid Input. You cannot move the other team's pieces!\n");
            }
            else
            {
                if(piece[1] == 'C') //if we selected a chariot
                {
                    int pieceExists = 0; //we're checking if the piece among the other two pieces exists
                    int chariotNum = -1; //we're checking the number of the chariot for later indexing purposes
                    for(int i=0; i<2; i++)
                    {
                        if(player->chariots[i].x == startx && player->chariots[i].y == starty && player->chariots[i].isalive == 1) //if there is a piece that is alive at the coordinates chosen by the user
                        {
                            pieceExists = 1;
                            chariotNum = i;
                        }
                    }
                    if(pieceExists)
                    {
                        chariotMove(player, otherplayer, startx, starty, endx, endy, chariotNum, piece[0], &isValid);
                    }
                    else
                    {
                        printf("Invalid Input. There is no chariot at that location!\n");
                    }
                }
                else if (piece[1] == 'H') //if we selected a horse
                {
                    int pieceExists = 0; //we're checking if the piece among the other two pieces exists
                    int horseNum = -1;
                    for(int i=0; i<2; i++)
                    {
                        if(player->horses[i].x == startx && player->horses[i].y == starty && player->horses[i].isalive == 1) //if there is a piece that is alive at the coordinates chosen by the user
                        {
                            pieceExists = 1;
                            horseNum = i;
                        }
                    }
                    if(pieceExists)
                    {
                        horseMove(player, otherplayer, startx, starty, endx, endy, horseNum, piece[0], &isValid);
                    }
                    else
                    {
                        printf("Invalid Input. There is no horse at that location!\n");
                    }
                }
                else if(piece[1] == 'E') //if we selected the elephant
                {
                    int pieceExists = 0; //we're checking if the piece among the other two pieces exists
                    int elephantNum = -1;
                    for(int i=0; i<2; i++)
                    {
                        if(player->elephants[i].x == startx && player->elephants[i].y == starty && player->elephants[i].isalive == 1) //if there is a piece that is alive at the coordinates chosen by the user
                        {
                            pieceExists = 1;
                            elephantNum = i;
                        }
                    }
                    if(pieceExists)
                    {
                        if((endy<=4 && player->team == 'B') || (endy>=5 && player->team == 'R')) //the elephant can't cross the river
                        {
                            elephantMove(player, otherplayer, startx, starty, endx, endy, elephantNum, piece[0], &isValid);
                        }
                        else
                        {
                            printf("Invalid Input. The elephant cannot cross the river (too fat)!\n");
                        }
                    }
                    else
                    {
                        printf("Invalid Input. There is no elephant in that location (or at all) on the board!\n");
                    }
                }
                else if(piece[1] == 'G') //if we selected the guard
                {
                    int pieceExists = 0; //we're checking if the piece among the other two pieces exists
                    int guardNum = -1;
                    for(int i=0; i<2; i++)
                    {
                        if(player->guards[i].x == startx && player->guards[i].y == starty && player->guards[i].isalive == 1) //if there is a piece that is alive at the coordinates chosen by the user
                        {
                            pieceExists = 1;
                            guardNum = i;
                        }
                    }
                    if(pieceExists) //the guard can't move outside the "castle"
                    {
                        if((endx>=3 && endx<=5) && ((endy<=2 && player->team == 'B') || (endy>=7 && player->team == 'R')))
                        {
                            guardMove(player, otherplayer, startx, starty, endx, endy, guardNum, piece[0], &isValid);
                        }
                        else
                            printf("Invalid Input. The guard cannot move outside the castle!\n");
                    }
                    else
                    {
                        printf("Invalid Input. There is no guard at that location!\n");
                    }
                }
                else if(piece[1] == 'K') //if we selected the king
                {
                    int pieceExists = 0; //we're checking if the piece among the other two pieces exists
                    for(int i=0; i<2; i++)
                    {
                        if(player->king.x == startx && player->king.y == starty && player->king.isalive == 1) //if there is a piece that is alive at the coordinates chosen by the user
                        {
                            pieceExists = 1;
                        }
                    }
                    if(pieceExists)
                    {
                        if((endx>=3 && endx<= 5) && ((endy<=2 && player->team == 'B') || (endy>=7 && player->team == 'R')) && checkPrevention(player, otherplayer, endx, endy, 0) == 0)
                        {
                            kingMove(player, otherplayer, startx, starty, endx, endy, piece[0], &isValid);
                        }
                        else if (checkPrevention(player, otherplayer, endx, endy, 0))
                        {
                            // printf("%d %d %d %d\n", checkPrevention(player, otherplayer, player->king.x, player->king.y), player->king.x, player->king.y, checkPrevention(player, otherplayer, endx, endy));
                            printf("Invalid Input. You cannot put yourself into check!\n");
                        }
                        else
                        {
                            printf("Invalid Input. The king cannot move outside the castle!\n");
                        }
                    }
                    else
                    {
                        printf("Invalid Input. There is no guard at that location!\n");
                    }
                }
                else if(piece[1] == 'A') //if we selected a black cannon
                {
                    int pieceExists = 0; //we're checking if the piece among the other two pieces exists
                    int cannonNum = -1;
                    for(int i=0; i<2; i++)
                    {
                        if(player->artillery[i].x == startx && player->artillery[i].y == starty && player->artillery[i].isalive == 1) //if there is a piece that is alive at the coordinates chosen by the user
                        {
                            pieceExists = 1;
                            cannonNum = i;
                        }
                    }
                    if(pieceExists)
                    {
                        cannonMove(player, otherplayer, startx, starty, endx, endy, cannonNum, piece[0], &isValid);
                    }
                    else
                    {
                        printf("Invalid Input. there is no cannon at that location\n");
                    }
                }
                else //if we selected the soldier
                {
                    int pieceExists = 0; //we're checking if the piece among the other two pieces exists
                    int soldierNum = -1;
                    for(int i=0; i<5; i++)
                    {
                        if(player->soldiers[i].x == startx && player->soldiers[i].y == starty && player->soldiers[i].isalive == 1) //if there is a piece that is alive at the coordinates chosen by the user
                        {
                            pieceExists = 1;
                            soldierNum = i;
                        }
                    }
                    if(pieceExists)
                    {
                        soldierMove(player, otherplayer, startx, starty, endx, endy, soldierNum, piece[0], &isValid);
                    }
                    else
                    {
                        printf("Invalid Input. There is no soldier at that location!\n");
                    }
                }
                
            }
        }
        else if(startx == endx && starty == endy)
        {
            printf("Invalid Input. You forgot to move the piece (you must move a piece!)\n");
        }
        else
        {
            printf("Invalid Input. You inputted an invalid piece and/or an invalid position that doesn't exist on the board.\n");
        }
} while (isValid);
}
void takepiece(struct Player *player, struct Player *otherplayer, int x, int y, int pieceNum)
{
    int enemypieceNum = -1;
    char enemypiecetype;
    
    if(otherplayer->chariots[0].x == x && otherplayer->chariots[0].y == y) //checking which piece is taken out
    {
        enemypieceNum = 0;
        enemypiecetype = 'C';
    }
    else if(otherplayer->chariots[1].x == x && otherplayer->chariots[1].y == y)
    {
        enemypieceNum = 1;
        enemypiecetype = 'C';
    }
    else if(otherplayer->horses[0].x == x && otherplayer->horses[0].y == y) 
    {
        enemypieceNum = 0;
        enemypiecetype = 'H';
    }
    else if(otherplayer->horses[1].x == x && otherplayer->horses[1].y == y)
    {
        enemypieceNum = 1;
        enemypiecetype = 'H';
    }
    else if(otherplayer->elephants[0].x == x && otherplayer->elephants[0].y == y)
    {
        enemypieceNum = 0;
        enemypiecetype = 'E';
    }
    else if(otherplayer->elephants[1].x == x && otherplayer->elephants[1].y == y)
    {
        enemypieceNum = 1;
        enemypiecetype = 'E';
    }
    else if(otherplayer->guards[0].x == x && otherplayer->guards[0].y == y) 
    {
        enemypieceNum = 0;
        enemypiecetype = 'G';
    }
    else if(otherplayer->guards[1].x == x && otherplayer->guards[1].y == y)
    {
        enemypieceNum = 1;
        enemypiecetype = 'G';
    }
    else if(otherplayer->king.x == x && otherplayer->king.y == y)
    {
        enemypiecetype = 'K';
    }
    else if(otherplayer->soldiers[0].x == x && otherplayer->soldiers[0].y == y)
    {
        enemypieceNum = 0;
        enemypiecetype = 'S';
    }
    else if(otherplayer->soldiers[1].x == x && otherplayer->soldiers[1].y == y)
    {
        enemypieceNum = 1;
        enemypiecetype = 'S';
    }
    else if(otherplayer->soldiers[2].x == x && otherplayer->soldiers[2].y == y)
    {
        enemypieceNum = 2;
        enemypiecetype = 'S';
    }
    else if(otherplayer->soldiers[3].x == x && otherplayer->soldiers[3].y == y)
    {
        enemypieceNum = 3;
        enemypiecetype = 'S';
    }
    else if(otherplayer->soldiers[4].x == x && otherplayer->soldiers[4].y == y)
    {
        enemypieceNum = 4;
        enemypiecetype = 'S';
    }
    else if(otherplayer->artillery[0].x == x && otherplayer->artillery[0].y == y) 
    {
        enemypieceNum = 0;
        enemypiecetype = 'A';
    }
    else if(otherplayer->artillery[1].x == x && otherplayer->artillery[1].y == y)
    {
        enemypieceNum = 1;
        enemypiecetype = 'A';
    }

    if(enemypiecetype == 'C')
    {
        otherplayer->chariots[enemypieceNum].x = -1;
        otherplayer->chariots[enemypieceNum].y = -1;
        otherplayer->chariots[enemypieceNum].isalive = 0; //removing the piece from the game
    }
    else if(enemypiecetype == 'H')
    {
        otherplayer->horses[enemypieceNum].x = -1;
        otherplayer->horses[enemypieceNum].y = -1;
        otherplayer->horses[enemypieceNum].isalive = 0;
    }
    else if(enemypiecetype == 'E')
    {
        otherplayer->elephants[enemypieceNum].x = -1;
        otherplayer->elephants[enemypieceNum].y = -1;        
        otherplayer->elephants[enemypieceNum].isalive = 0;
    }
    else if(enemypiecetype == 'G')
    {
        otherplayer->guards[enemypieceNum].x = -1;
        otherplayer->guards[enemypieceNum].y = -1;
        otherplayer->guards[enemypieceNum].isalive = 0;
    }
    else if(enemypiecetype == 'K')
    {
        otherplayer->king.x = -1;
        otherplayer->king.y = -1;
        otherplayer->king.isalive = 0;
    }
    else if(enemypiecetype == 'S')
    {
        otherplayer->soldiers[enemypieceNum].x = -1;
        otherplayer->soldiers[enemypieceNum].y = -1;
        otherplayer->soldiers[enemypieceNum].isalive = 0;
    }
    else if(enemypiecetype == 'A')
    {
        otherplayer->artillery[enemypieceNum].x = -1;
        otherplayer->artillery[enemypieceNum].y = -1;
        otherplayer->artillery[enemypieceNum].isalive = 0;
    }
}
void updatePosition(struct Player *player, int startx, int starty, int endx, int endy, int pieceNum, char team, char pieceType)
{
    board[starty][startx][0] = '-'; //change the piece's start position to an empty space on the board
    board[starty][startx][1] = '-';

    board[endy][endx][0] = team; //change the piece's end position on the board
    board[endy][endx][1] = pieceType;

  if(pieceType == 'C') //depending on which piece's position is being updated, update the piece's position in the struct
    {
        player->chariots[pieceNum].x = endx;
        player->chariots[pieceNum].y = endy;
    }
    else if(pieceType == 'H')
    {
        player->horses[pieceNum].x = endx;
        player->horses[pieceNum].y = endy;
    }
    else if(pieceType == 'E')
    {
        player->elephants[pieceNum].x = endx;
        player->elephants[pieceNum].y = endy;     
    }
    else if(pieceType == 'G')
    {
        player->guards[pieceNum].x = endx;
        player->guards[pieceNum].y = endy;
    }
    else if(pieceType == 'K')
    {
        player->king.x = endx;
        player->king.y = endy;
    }
    else if(pieceType == 'S')
    {
        player->soldiers[pieceNum].x = endx;
        player->soldiers[pieceNum].y = endy;
    }
    else if(pieceType == 'A')
    {
        player->artillery[pieceNum].x = endx;
        player->artillery[pieceNum].y = endy;
    }
}
void chariotMove(struct Player *player, struct Player *otherplayer, int startx, int starty, int endx, int endy, int chariotNum, char Team, int *isValid)
{
    int changex = endx-startx;
    int changey = endy-starty;
    int pieceinWay = 0; 
    if(changex == 0) //the chariot can only move directly horizotal or directly vertical, so we're just making sure the user's attempt is either of these options
    {
        if(changey < 0) //if we're moving upwards
        {
            for(int i=starty; i>=endy; i--) //checks if an ally piece is in the way by looping through all the possible positions and checking if the spaces contain ally pieces
            {
                if((board[i][startx][0] == 'B' && Team == 'B') || (board[i][startx][0] == 'R' && Team == 'R')) 
                {
                    pieceinWay++;
                }
            }
            for(int i=starty; i>endy; i--) //checks if an enemy piece is in the way; doesn't check the end (x, y) because that's a separate case where we're taking out an enemy piece
            {
                if((board[i][startx][0] == 'R' && Team == 'B') || (board[i][startx][0] == 'B' && Team == 'R'))
                {
                    pieceinWay++; 
                }
            }
        }
        else//if we're moving downwards
        {
            for(int i=starty; i<=endy; i++) 
            {
                if((board[i][startx][0] == 'B' && Team == 'B') || (board[i][startx][0] == 'R' && Team == 'R'))
                {
                    pieceinWay++;
                }
            }
            for(int i=starty; i<endy; i++) //checks if an enemy piece is in the way
            {
                if((board[i][startx][0] == 'R' && Team == 'B') || (board[i][startx][0] == 'B' && Team == 'R'))
                {
                    pieceinWay++; 
                }
            }
        }
        if(pieceinWay > 1) //if a piece was in the way
        {
            printf("Invalid Input. There is a piece in the way!\n");
        }
        //checks if the player moved to take out an enemy piece
        else if((board[endy][endx][0] == 'R' && Team == 'B') || board[endy][endx][0] == 'B' && Team == 'R')
        {
            //premptive update of the board to see if even after our move, we are still in check
            board[starty][startx][0] = '-'; //change the piece's start position to an empty space on the board
            board[starty][startx][1] = '-';

            char temp = board[endy][endx][0]; //storing the end piece's values in case the move doesn't remove us from check
            char temp2 = board[endy][endx][1];

            board[endy][endx][0] = player->team; //change the piece's end position on the board
            board[endy][endx][1] = 'C';
            // boardPrint();
            if(checkPrevention(player, otherplayer, player->king.x, player->king.y, 0)) //if the move was invalid because it didn't prevent us from being in check, then we restore the previous board
            {
                board[starty][startx][0] = player->team; //change the piece's start position to an empty space on the board
                board[starty][startx][1] = 'C';

                board[endy][endx][0] = temp; //change the piece's end position on the board
                board[endy][endx][1] = temp2;
                printf("Invalid Input. That move either doesn't escape check or puts you into check!\n");
                // boardPrint();
            }
            else
            {
                takepiece(player, otherplayer, endx, endy, chariotNum);
                updatePosition(player, startx, starty, endx, endy, chariotNum, Team, 'C');
                *isValid = 0;
            }

        }
        else //if the move was valid but no pieces were taken out
        {
            board[starty][startx][0] = '-'; //change the piece's start position to an empty space on the board
            board[starty][startx][1] = '-';

            board[endy][endx][0] = player->team; //change the piece's end position on the board
            board[endy][endx][1] = 'C';
            if(checkPrevention(player, otherplayer, player->king.x, player->king.y, 0)) //if the move was invalid because it didn't prevent us from being in check, then we restore the previous board
            {
                board[starty][startx][0] = player->team; //change the piece's start position to an empty space on the board
                board[starty][startx][1] = 'C';

                board[endy][endx][0] = '-'; //change the piece's end position on the board
                board[endy][endx][1] = '-';
                printf("Invalid Input. That move either doesn't escape check or puts you into check!\n");
            }
            else
            {
                updatePosition(player, startx, starty, endx, endy, chariotNum, Team, 'C');
                *isValid = 0;
            }
        }
    }
    else if(changey == 0) //same exact code, but changed for moving horizontally
    {
        if(changex < 0) //if we're moving leftwards
        {
            for(int i=startx; i>=endx; i--) //checks if an ally piece is in the way
            {
                if((board[starty][i][0] == 'B' && Team == 'B') || (board[starty][i][0] == 'R' && Team == 'R'))
                {
                    pieceinWay++;
                }
            }
            for(int i=startx; i>endx; i--) //checks if an enemy piece is in the way
            {
                if((board[starty][i][0] == 'R' && Team == 'B') || (board[starty][i][0] == 'B' && Team == 'R'))
                {
                    pieceinWay++; 
                }
            }
        }
        else//if we're moving rightwards
        {
            for(int i=startx; i<=endx; i++) //if we're moving upwards
            {
                if((board[starty][i][0] == 'B' && Team == 'B') || (board[starty][i][0] == 'R' && Team == 'R'))
                {
                    pieceinWay++;
                }
            }
            for(int i=startx; i<endx; i++) //checks if an enemy piece is in the way
            {
                if((board[starty][i][0] == 'R' && Team == 'B') || (board[starty][i][0] == 'B' && Team == 'R'))
                {
                    pieceinWay++; 
                }
            }
        }
        if(pieceinWay > 1) //if a piece was in the way
        {
            printf("Invalid Input. There is a piece in the way!\n");
        }
        //checks if the player moved to take out an enemy piece
        else if((board[endy][endx][0] == 'R' && Team == 'B') || (board[endy][endx][0] == 'B' && Team == 'R'))
        {   
            board[starty][startx][0] = '-'; //change the piece's start position to an empty space on the board
            board[starty][startx][1] = '-';

            char temp = board[endy][endx][0]; //storing the end piece's values in case the move doesn't remove us from check
            char temp2 = board[endy][endx][1];

            board[endy][endx][0] = player->team; //change the piece's end position on the board
            board[endy][endx][1] = 'C';
            // boardPrint();
            if(checkPrevention(player, otherplayer, player->king.x, player->king.y, 0)) //if the move was invalid because it didn't prevent us from being in check, then we restore the previous board
            {
                board[starty][startx][0] = player->team; //change the piece's start position to an empty space on the board
                board[starty][startx][1] = 'C';

                board[endy][endx][0] = '-'; //change the piece's end position on the board
                board[endy][endx][1] = '-';
                printf("Invalid Input. That move either doesn't escape check or puts you into check!\n");
            }
            else
            {
                takepiece(player, otherplayer, endx, endy, chariotNum);
                updatePosition(player, startx, starty, endx, endy, chariotNum, Team, 'C');
                *isValid = 0;
            }
        }
        else //if the move was valid but no pieces were taken out
        {
            board[starty][startx][0] = '-'; //change the piece's start position to an empty space on the board
            board[starty][startx][1] = '-';

            board[endy][endx][0] = player->team; //change the piece's end position on the board
            board[endy][endx][1] = 'H';
            if(checkPrevention(player, otherplayer, player->king.x, player->king.y, 0)) //if the move was invalid because it didn't prevent us from being in check, then we restore the previous board
            {
                board[starty][startx][0] = player->team; //change the piece's start position to an empty space on the board
                board[starty][startx][1] = 'H';

                board[endy][endx][0] = '-'; //change the piece's end position on the board
                board[endy][endx][1] = '-';
                printf("Invalid Input. That move either doesn't escape check or puts you into check!\n");
            }
            else
            {
                updatePosition(player, startx, starty, endx, endy, chariotNum, Team, 'C');
                *isValid = 0;
            }
        }
    }
    else
    {
        printf("Invalid Input. The chariot can only move directly vertical or directly horizontal!\n");
    }
}
void horseMove(struct Player *player, struct Player *otherplayer, int startx, int starty, int endx, int endy, int horseNum, char Team, int *isValid)
{
    int changex = endx-startx;
    int changey = endy-starty;
    int pieceinWay = 0; 
    //checking all 8 possible movements of the horse
    if((abs(changex) == 2 && abs(changey) == 1) || (abs(changex) == 1 && abs(changey) == 2)) //we must move in an L shape for the movement to be valid
    {
        if(changex == 1 && changey == -2) //up, diagonal up right
        {
            if(board[starty-1][startx][0] != '-') //checking upward 1 space movement to see if there's a piece in the way
            {
                pieceinWay++;
            }
            if(board[endy][endx-1][0] != '-' && board[endy+1][endx][0] != '-') //checking the diagonal movement to see if there's 2 pieces preventing the movement
            {
                pieceinWay++;
            }
            if((board[endy][endx][0] == 'B' && Team == 'B') || (board[endy][endx][0] == 'R' && Team == 'R')) //if there's a friendly piece in the way of the end position
            {
                pieceinWay++;
            }
        }
        else if(changex == 2 && changey == -1) //right, diagonal up right
        {
            if(board[starty][startx+1][0] != '-')
            {
                pieceinWay++;
            }
            if(board[endy+1][endx][0] != '-' && board[endy][endx-1][0] != '-')
            {
                pieceinWay++;
            }
            if((board[endy][endx][0] == 'B' && Team == 'B') ||(board[endy][endx][0] == 'R' && Team == 'R'))
            {
                pieceinWay++;
            }
        }
        else if(changex == 2 && changey == 1) //right, diagonal down right
        {
            if(board[starty][startx+1][0] != '-')
            {
                pieceinWay++;
            }
            if(board[endy-1][endx][0] != '-' && board[endy][endx-1][0] != '-')
            {
                pieceinWay++;
            }
            if((board[endy][endx][0] == 'B' && Team == 'B') ||(board[endy][endx][0] == 'R' && Team == 'R'))
            {
                pieceinWay++;
            }
        }
        else if(changex == 1 && changey == 2) //down, diagonal down right
        {
            if(board[starty+1][startx][0] != '-')
            {
                pieceinWay++;
            }
            if(board[endy][endx-1][0] != '-' && board[endy-1][endx][0] != '-')
            {
                pieceinWay++;
            }
            if((board[endy][endx][0] == 'B' && Team == 'B') ||(board[endy][endx][0] == 'R' && Team == 'R'))
            {
                pieceinWay++;
            }
        }
        else if(changex == -1 && changey == 2) //down, diagonal down left
        {
            if(board[starty+1][startx][0] != '-')
            {
                pieceinWay++;
            }
            if(board[endy][endx+1][0] != '-' && board[endy-1][endx][0] != '-')
            {
                pieceinWay++;
            }
            if((board[endy][endx][0] == 'B' && Team == 'B') ||(board[endy][endx][0] == 'R' && Team == 'R'))
            {
                pieceinWay++;
            }
        }
        else if(changex == -2 && changey == 1) //left, diagonal down left
        {
            if(board[starty][startx-1][0] != '-')
            {
                pieceinWay++;
            }
            if(board[endy-1][endx][0] != '-' && board[endy][endx+1][0] != '-')
            {
                pieceinWay++;
            }
            if((board[endy][endx][0] == 'B' && Team == 'B') ||(board[endy][endx][0] == 'R' && Team == 'R'))
            {
                pieceinWay++;
            }
        }
        else if(changex == -2 && changey == -1) //left, diagonal up left
        {
            if(board[starty][startx-1][0] != '-')
            {
                pieceinWay++;
            }
            if(board[endy+1][endx][0] != '-' && board[endy][endx+1][0] != '-')
            {
                pieceinWay++;
            }
            if((board[endy][endx][0] == 'B' && Team == 'B') ||(board[endy][endx][0] == 'R' && Team == 'R'))
            {
                pieceinWay++;
            }
        }
        else if(changex == -1 && changey == -2) //up, diagonal up left
        {
            if(board[starty-1][startx][0] != '-') //checking upward 1 space movement to see if there's a piece in the way
            {
                pieceinWay++;
            }
            if(board[endy][endx+1][0] != '-' && board[endy+1][endx][0] != '-') //checking the diagonal movement to see if there's 2 pieces preventing the movement
            {
                pieceinWay++;
            }
            if((board[endy][endx][0] == 'B' && Team == 'B') || (board[endy][endx][0] == 'R' && Team == 'R')) //if there's a friendly piece in the way of the end position
            {
                pieceinWay++;
            }
        }

        if(pieceinWay > 0)
        {
            printf("Invalid input. There is a piece in the way!\n");
        }
        else if((board[endy][endx][0] == 'R' && Team == 'B') || (board[endy][endx][0] == 'B' && Team == 'R'))
        {
            board[starty][startx][0] = '-'; //change the piece's start position to an empty space on the board
            board[starty][startx][1] = '-';

            char temp = board[endy][endx][0]; //storing the end piece's values in case the move doesn't remove us from check
            char temp2 = board[endy][endx][1];

            board[endy][endx][0] = player->team; //change the piece's end position on the board
            board[endy][endx][1] = 'H';
            // boardPrint();
            if(checkPrevention(player, otherplayer, player->king.x, player->king.y, 0)) //if the move was invalid because it didn't prevent us from being in check, then we restore the previous board
            {
                board[starty][startx][0] = player->team; //change the piece's start position to an empty space on the board
                board[starty][startx][1] = 'H';

                board[endy][endx][0] = temp; //change the piece's end position on the board
                board[endy][endx][1] = temp2;
                printf("Invalid Input. That move either doesn't escape check or puts you into check!\n");
            }
            else
            {
                takepiece(player, otherplayer, endx, endy, horseNum);
                updatePosition(player, startx, starty, endx, endy, horseNum, Team, 'H');
                *isValid = 0;
            }
        }
        else
        {
            board[starty][startx][0] = '-'; //change the piece's start position to an empty space on the board
            board[starty][startx][1] = '-';

            board[endy][endx][0] = player->team; //change the piece's end position on the board
            board[endy][endx][1] = 'H';
            if(checkPrevention(player, otherplayer, player->king.x, player->king.y, 0)) //if the move was invalid because it didn't prevent us from being in check, then we restore the previous board
            {
                board[starty][startx][0] = player->team; //change the piece's start position to an empty space on the board
                board[starty][startx][1] = 'H';

                board[endy][endx][0] = '-'; //change the piece's end position on the board
                board[endy][endx][1] = '-';
                printf("Invalid Input. That move either doesn't escape check or puts you into check!\n");
            }
            else
            {
                updatePosition(player, startx, starty, endx, endy, horseNum, Team, 'H');
                *isValid = 0;
            }
        }
    }
    else //if the movement is not one of the 8 possible movements, then it's invalid
    {
        printf("Invalid input. The horse can only move 1 space horizontal and vertical, followed by a diagonal movement w/ a component in the same direction as the first movement!\n");
    }
}
void elephantMove(struct Player *player, struct Player *otherplayer, int startx, int starty, int endx, int endy, int elephantNum, char Team, int *isValid)
{
    int changex = endx-startx;
    int changey = endy-starty;
    int pieceinWay = 0; 
    if(abs(changex) == 2 && abs(changey) == 2) //we must move 2 spaces diagoanl for the movement to be valid
    {
        //checking all 4 possible movements of the elephant
        if(changex == 2 && changey == -2) //up right diagonal
        {
            if((board[starty-1][startx][0] != '-') && (board[starty][startx+1][0] != '-'))
            {
                pieceinWay++;
            }
            if((board[starty-2][startx+1][0] != '-') && (board[starty-1][startx+2][0] != '-'))
            {
                pieceinWay++;
            }
            if((board[endy][endx][0] == 'B' && Team == 'B') || (board[endy][endx][0] == 'R' && Team == 'R'))
            {
                pieceinWay++;
            }
        }
        else if(changex == 2 && changey == 2) //down right diagonal
        {
            if((board[starty+1][startx][0] != '-') && (board[starty][startx+1][0] != '-'))
            {
                pieceinWay++;
            }
            if((board[starty+2][startx+1][0] != '-') && (board[starty+1][startx+2][0] != '-'))
            {
                pieceinWay++;
            }
            if((board[endy][endx][0] == 'B' && Team == 'B') || (board[endy][endx][0] == 'R' && Team == 'R'))
            {
                pieceinWay++;
            }
        }
        else if(changex == -2 && changey == 2) //down left diagonal
        {
            if((board[starty+1][startx][0] != '-') && (board[starty][startx-1][0] != '-'))
            {
                pieceinWay++;
            }
            if((board[starty+2][startx-1][0] != '-') && (board[starty+1][startx-2][0] != '-'))
            {
                pieceinWay++;
            }
            if((board[endy][endx][0] == 'B' && Team == 'B') || (board[endy][endx][0] == 'R' && Team == 'R'))
            {
                pieceinWay++;
            }
        }
        else if (changex == -2 && changey == -2) //up left diagonal
        {
            if((board[starty-1][startx][0] != '-') && (board[starty][startx-1][0] != '-'))
            {
                pieceinWay++;
            }
            if((board[starty-2][startx-1][0] != '-') && (board[starty-1][startx-2][0] != '-'))
            {
                pieceinWay++;
            }
            if((board[endy][endx][0] == 'B' && Team == 'B') || (board[endy][endx][0] == 'R' && Team == 'R'))
            {
                pieceinWay++;
            }
        }

        if(pieceinWay > 0)
        {
            printf("Invalid input. There is a piece in the way!\n");
        }
        else if((board[endy][endx][0] == 'R' && Team == 'B') || (board[endy][endx][0] == 'B' && Team == 'R'))
        {
            board[starty][startx][0] = '-'; //change the piece's start position to an empty space on the board
            board[starty][startx][1] = '-';

            char temp = board[endy][endx][0]; //storing the end piece's values in case the move doesn't remove us from check
            char temp2 = board[endy][endx][1];

            board[endy][endx][0] = player->team; //change the piece's end position on the board
            board[endy][endx][1] = 'E';
            // boardPrint();
            if(checkPrevention(player, otherplayer, player->king.x, player->king.y, 0)) //if the move was invalid because it didn't prevent us from being in check, then we restore the previous board
            {
                board[starty][startx][0] = player->team; //change the piece's start position to an empty space on the board
                board[starty][startx][1] = 'E';

                board[endy][endx][0] = temp; //change the piece's end position on the board
                board[endy][endx][1] = temp2;
                printf("Invalid Input. That move either doesn't escape check or puts you into check!\n");
                // boardPrint();
                // printf("startx: %d\tstarty: %d\tendx: %d\tendy: %d\n", startx, starty, endx, endy);
            }
            else
            {
                takepiece(player, otherplayer, endx, endy, elephantNum);
                updatePosition(player, startx, starty, endx, endy, elephantNum, Team, 'E');
                *isValid = 0;
            }
        }
        else
        {
            board[starty][startx][0] = '-'; //change the piece's start position to an empty space on the board
            board[starty][startx][1] = '-';

            board[endy][endx][0] = player->team; //change the piece's end position on the board
            board[endy][endx][1] = 'E';
            // boardPrint();
            // printf("startx: %d\tstarty: %d\tendx: %d\tendy: %d\tfirst:%c\tsecond:%c\n", startx, starty, endx, endy, board[2][8][0], board[2][8][1]);
            if(checkPrevention(player, otherplayer, player->king.x, player->king.y, 0)) //if the move was invalid because it didn't prevent us from being in check, then we restore the previous board
            {
                board[starty][startx][0] = player->team; //change the piece's start position to an empty space on the board
                board[starty][startx][1] = 'E';

                board[endy][endx][0] = '-'; //change the piece's end position on the board
                board[endy][endx][1] = '-';
                printf("Invalid Input. That move either doesn't escape check or puts you into check!\n");
                // boardPrint();
            //  printf("startx: %d\tstarty: %d\tendx: %d\tendy: %d\t%c %c\n", startx, starty, endx, endy, board[2][8][0], board[2][8][1]);
            }
            else
            {
                updatePosition(player, startx, starty, endx, endy, elephantNum, Team, 'E');
                *isValid = 0;
            }
        }        
    }
    else //if the movement is not one of the 4 possible movements then its' invalid
    {
        printf("Invalid input. The elephant can only move 2 diagonal spaces!\n");
    }
}
void guardMove(struct Player *player, struct Player *otherplayer, int startx, int starty, int endx, int endy, int guardNum, char Team, int *isValid)
{
    int changex = endx-startx;
    int changey = endy-starty;
    int pieceinWay = 0; 

    if(abs(changex) == 1 && abs(changey) == 1) //we must move 1 space diagoanl for the movement to be valid
    {
        //checking all 4 possible movements of the guard
        if(changex == 1 && changey == -1) //up right diagonal
        {
            if((board[starty-1][startx][0] != '-') && (board[starty][startx+1][0] != '-'))
            {
                pieceinWay++;
            }
            if((board[endy][endx][0] == 'B' && Team == 'B') || (board[endy][endx][0] == 'R' && Team == 'R'))
            {
                pieceinWay++;
            }
        }
        else if(changex == 1 && changey == 1) //down right diagonal
        {
            if((board[starty+1][startx][0] != '-') && (board[starty][startx+1][0] != '-'))
            {
                pieceinWay++;
            }
            if((board[endy][endx][0] == 'B' && Team == 'B') || (board[endy][endx][0] == 'R' && Team == 'R'))
            {
                pieceinWay++;
            }
        }
        else if(changex == -1 && changey == 1) //down left diagonal
        {
            if((board[starty+1][startx][0] != '-') && (board[starty][startx-1][0] != '-'))
            {
                pieceinWay++;
            }
            if((board[endy][endx][0] == 'B' && Team == 'B') || (board[endy][endx][0] == 'R' && Team == 'R'))
            {
                pieceinWay++;
            }
        }
        else if (changex == -1 && changey == -1) //up left diagonal
        {
            if((board[starty-1][startx][0] != '-') && (board[starty][startx-1][0] != '-'))
            {
                pieceinWay++;
            }
            if((board[endy][endx][0] == 'B' && Team == 'B') || (board[endy][endx][0] == 'R' && Team == 'R'))
            {
                pieceinWay++;
            }
        }

        if(pieceinWay > 0)
        {
            printf("Invalid input. There is a piece in the way!\n");
        }
        else if((board[endy][endx][0] == 'R' && Team == 'B') || (board[endy][endx][0] == 'B' && Team == 'R'))
        {
            board[starty][startx][0] = '-'; //change the piece's start position to an empty space on the board
            board[starty][startx][1] = '-';

            char temp = board[endy][endx][0]; //storing the end piece's values in case the move doesn't remove us from check
            char temp2 = board[endy][endx][1];

            board[endy][endx][0] = player->team; //change the piece's end position on the board
            board[endy][endx][1] = 'G';
            // boardPrint();
            if(checkPrevention(player, otherplayer, player->king.x, player->king.y, 0)) //if the move was invalid because it didn't prevent us from being in check, then we restore the previous board
            {
                board[starty][startx][0] = player->team; //change the piece's start position to an empty space on the board
                board[starty][startx][1] = 'G';

                board[endy][endx][0] = temp; 
                board[endy][endx][1] = temp2;
                printf("Invalid Input. That move either doesn't escape check or puts you into check!\n");
            }
            else
            {
                takepiece(player, otherplayer, endx, endy, guardNum);
                updatePosition(player, startx, starty, endx, endy, guardNum, Team, 'G');
                *isValid = 0;
            }
        }
        else
        {
            board[starty][startx][0] = '-'; //change the piece's start position to an empty space on the board
            board[starty][startx][1] = '-';

            board[endy][endx][0] = player->team; //change the piece's end position on the board
            board[endy][endx][1] = 'G';
            if(checkPrevention(player, otherplayer, player->king.x, player->king.y, 0)) //if the move was invalid because it didn't prevent us from being in check, then we restore the previous board
            {
                board[starty][startx][0] = player->team; //change the piece's start position to an empty space on the board
                board[starty][startx][1] = 'G';

                board[endy][endx][0] = '-'; //change the piece's end position on the board
                board[endy][endx][1] = '-';
                printf("Invalid Input. That move either doesn't escape check or puts you into check!\n");
            }
            else
            {
                updatePosition(player, startx, starty, endx, endy, guardNum, Team, 'G');
                *isValid = 0;
            }
        }        
    }
    else //if the movement is not one of the 4 possible movements then its' invalid
    {
        printf("Invalid input. The guard can only move 1 diagonal space!\n");
    }
}
void kingMove(struct Player *player, struct Player *otherplayer, int startx, int starty, int endx, int endy, char Team, int *isValid)
{
    int changex = endx-startx;
    int changey = endy-starty;
    int pieceinWay = 0; 

    if(abs(changex) + abs(changey) == 1) //we must move 1 space vertically or horizontally
    {   
        if((board[endy][endx][0] == 'B' && Team == 'B') || (board[endy][endx][0] == 'R' && Team == 'R'))
        {
            pieceinWay++;
        }

        if(pieceinWay > 0)
        {
            printf("Invalid input. There is a piece in the way!\n");
        }
        else if((board[endy][endx][0] == 'R' && Team == 'B') || (board[endy][endx][0] == 'B' && Team == 'R'))
        {
            board[starty][startx][0] = '-'; //change the piece's start position to an empty space on the board
            board[starty][startx][1] = '-';

            char temp = board[endy][endx][0]; //storing the end piece's values in case the move doesn't remove us from check
            char temp2 = board[endy][endx][1];

            board[endy][endx][0] = player->team; //change the piece's end position on the board
            board[endy][endx][1] = 'K';
            // boardPrint();
            if(checkPrevention(player, otherplayer, player->king.x, player->king.y, 0)) //if the move was invalid because it didn't prevent us from being in check, then we restore the previous board
            {
                board[starty][startx][0] = player->team; //change the piece's start position to an empty space on the board
                board[starty][startx][1] = 'K';

                board[endy][endx][0] = temp; //change the piece's end position on the board
                board[endy][endx][1] = temp2;
                printf("Invalid Input. That move either doesn't escape check or puts you into check!\n");
            }
            else
            {
                takepiece(player, otherplayer, endx, endy, 0);
                updatePosition(player, startx, starty, endx, endy, 0, Team, 'K');
                *isValid = 0;
            }
        }
        else
        {
            board[starty][startx][0] = '-'; //change the piece's start position to an empty space on the board
            board[starty][startx][1] = '-';

            board[endy][endx][0] = player->team; //change the piece's end position on the board
            board[endy][endx][1] = 'K';
            if(checkPrevention(player, otherplayer, player->king.x, player->king.y, 0)) //if the move was invalid because it didn't prevent us from being in check, then we restore the previous board
            {
                board[starty][startx][0] = player->team; //change the piece's start position to an empty space on the board
                board[starty][startx][1] = 'K';

                board[endy][endx][0] = '-'; //change the piece's end position on the board
                board[endy][endx][1] = '-';
                printf("Invalid Input. That move either doesn't escape check or puts you into check!\n");
            }
            else
            {
                updatePosition(player, startx, starty, endx, endy, 0, Team, 'K');
                *isValid = 0;
            }
        }        
    }
    else //if the movement is not one of the 4 possible movements then its' invalid
    {
        printf("Invalid input. The king can only move 1 space vertically or horizontally!\n");
    }
}
void cannonMove(struct Player *player, struct Player *otherplayer, int startx, int starty, int endx, int endy, int cannonNum, char Team, int *isValid)
{
    int changex = endx-startx;
    int changey = endy-starty;
    int pieceinWay = 0; 
    int pieceType = 0; 

    if(changex == 0) //the cannon can only move directly horizotal or directly vertical, so we're just making sure the user's attempt is either of these options
    {
        if(changey < 0) //if we're moving upwards
        {
            for(int i=starty; i>=endy; i--) //checks if an ally piece is in the way by looping through all the possible positions and checking if the spaces contain ally pieces
            {
                if((board[i][startx][0] == 'B' && Team == 'B') || (board[i][startx][0] == 'R' && Team == 'R')) 
                {
                    pieceinWay++;
                }
            }
            for(int i=starty; i>endy; i--) //checks if an enemy piece is in the way; doesn't check the end (x, y) because that's a separate case where we're taking out an enemy piece
            {
                if((board[i][startx][0] == 'R' && Team == 'B') || (board[i][startx][0] == 'B' && Team == 'R'))
                {
                    pieceinWay++; 
                }
            }
        }
        else//if we're moving downwards
        {
            for(int i=starty; i<=endy; i++) 
            {
                if((board[i][startx][0] == 'B' && Team == 'B') || (board[i][startx][0] == 'R' && Team == 'R'))
                {
                    pieceinWay++;
                }
            }
            for(int i=starty; i<endy; i++) //checks if an enemy piece is in the way
            {
                if((board[i][startx][0] == 'R' && Team == 'B') || (board[i][startx][0] == 'B' && Team == 'R'))
                {
                    pieceinWay++; 
                }
            }
        }
        if(pieceinWay > 2) //if the player tried to jump over 2 pieces
        {
            printf("Invalid Input. You cannot jump over more than two pieces!\n");
        }
        //checks if the player moved to take out an enemy piece
        else if(((board[endy][endx][0] == 'R' && Team == 'B') || (board[endy][endx][0] == 'B' && Team == 'R')) && pieceinWay == 1) //if we jump over a piece onto an enemy piece, we take it out
        {
            board[starty][startx][0] = '-'; //change the piece's start position to an empty space on the board
            board[starty][startx][1] = '-';

            char temp = board[endy][endx][0]; //storing the end piece's values in case the move doesn't remove us from check
            char temp2 = board[endy][endx][1];

            board[endy][endx][0] = player->team; //change the piece's end position on the board
            board[endy][endx][1] = 'A';
            // boardPrint();
            if(checkPrevention(player, otherplayer, player->king.x, player->king.y, 0)) //if the move was invalid because it didn't prevent us from being in check, then we restore the previous board
            {
                board[starty][startx][0] = player->team; //change the piece's start position to an empty space on the board
                board[starty][startx][1] = 'A';

                board[endy][endx][0] = temp; //change the piece's end position on the board
                board[endy][endx][1] = temp2;
                printf("Invalid Input. That move either doesn't escape check or puts you into check!\n");
            }
            else
            {
                takepiece(player, otherplayer, endx, endy, cannonNum);
                updatePosition(player, startx, starty, endx, endy, cannonNum, Team, 'A');
                *isValid = 0;
            }
        }
        else if(((board[endy][endx][0] == 'B' && Team == 'B') || (board[endy][endx][0] == 'R' && Team == 'R')) && pieceinWay == 1)
        //if we attempt to jump over a piece but we're jumping onto our own piece
        {
            printf("Invalid Input. You cannot jump onto your own piece!\n");
        }
        else if(board[endy][endx][0] == '-' && pieceinWay == 2)
        {
            printf("Invalid Input. If you're jumping over a piece, it must be in order to take out an enemy piece!\n");
        }
        else //if we move normally with no jumps attempted
        {
            board[starty][startx][0] = '-'; //change the piece's start position to an empty space on the board
            board[starty][startx][1] = '-';

            board[endy][endx][0] = player->team; //change the piece's end position on the board
            board[endy][endx][1] = 'A';
            if(checkPrevention(player, otherplayer, player->king.x, player->king.y, 0)) //if the move was invalid because it didn't prevent us from being in check, then we restore the previous board
            {
                board[starty][startx][0] = player->team; //change the piece's start position to an empty space on the board
                board[starty][startx][1] = 'A';

                board[endy][endx][0] = '-'; //change the piece's end position on the board
                board[endy][endx][1] = '-';
                printf("Invalid Input. That move either doesn't escape check or puts you into check!\n");
            }
            else
            {
                updatePosition(player, startx, starty, endx, endy, cannonNum, Team, 'A');
                *isValid = 0;
            }
        }
    }
    else if(changey == 0) //same exact code, but changed for moving horizontally
    {
        if(changex < 0) //if we're moving leftwards
        {
            for(int i=startx; i>=endx; i--) //checks if an ally piece is in the way
            {
                if((board[starty][i][0] == 'B' && Team == 'B') || (board[starty][i][0] == 'R' && Team == 'R'))
                {
                    pieceinWay++;
                }
            }
            for(int i=startx; i>endx; i--) //checks if an enemy piece is in the way
            {
                if((board[starty][i][0] == 'R' && Team == 'B') || (board[starty][i][0] == 'B' && Team == 'R'))
                {
                    pieceinWay++; 
                }
            }
        }
        else//if we're moving rightwards
        {
            for(int i=startx; i<=endx; i++) //if we're moving upwards
            {
                if((board[starty][i][0] == 'B' && Team == 'B') || (board[starty][i][0] == 'R' && Team == 'R'))
                {
                    pieceinWay++;
                }
            }
            for(int i=startx; i<endx; i++) //checks if an enemy piece is in the way
            {
                if((board[starty][i][0] == 'R' && Team == 'B') || (board[starty][i][0] == 'B' && Team == 'R'))
                {
                    pieceinWay++; 
                }
            }
        }
        if(pieceinWay > 2) //if the player tried to jump over 2 pieces
        {
            printf("Invalid Input. You cannot jump over more than two pieces!\n");
        }
        //checks if the player moved to take out an enemy piece
        else if(((board[endy][endx][0] == 'R' && Team == 'B') || (board[endy][endx][0] == 'B' && Team == 'R')) && pieceinWay == 1) //if we jump over a piece onto an enemy piece, we take it out
        {
            board[starty][startx][0] = '-'; //change the piece's start position to an empty space on the board
            board[starty][startx][1] = '-';

            char temp = board[endy][endx][0]; //storing the end piece's values in case the move doesn't remove us from check
            char temp2 = board[endy][endx][1];

            board[endy][endx][0] = player->team; //change the piece's end position on the board
            board[endy][endx][1] = 'A';
            // boardPrint();
            if(checkPrevention(player, otherplayer, player->king.x, player->king.y, 0)) //if the move was invalid because it didn't prevent us from being in check, then we restore the previous board
            {
                board[starty][startx][0] = player->team; //change the piece's start position to an empty space on the board
                board[starty][startx][1] = 'A';

                board[endy][endx][0] = temp; //change the piece's end position on the board
                board[endy][endx][1] = temp2;
                printf("Invalid Input. That move either doesn't escape check or puts you into check!\n");
            }
            else
            {
                takepiece(player, otherplayer, endx, endy, cannonNum);
                updatePosition(player, startx, starty, endx, endy, cannonNum, Team, 'A');
                *isValid = 0;
            }
        }
        else if(((board[endy][endx][0] == 'B' && Team == 'B') || (board[endy][endx][0] == 'R' && Team == 'R')) && pieceinWay == 1)
        //if we attempt to jump over a piece but we're jumping onto our own piece
        {
            printf("Invalid Input. You cannot jump onto your own piece!\n");
        }
        else //if we move normally with no jumps attempted
        {
            board[starty][startx][0] = '-'; //change the piece's start position to an empty space on the board
            board[starty][startx][1] = '-';

            board[endy][endx][0] = player->team; //change the piece's end position on the board
            board[endy][endx][1] = 'A';
            if(checkPrevention(player, otherplayer, player->king.x, player->king.y, 0)) //if the move was invalid because it didn't prevent us from being in check, then we restore the previous board
            {
                board[starty][startx][0] = player->team; //change the piece's start position to an empty space on the board
                board[starty][startx][1] = 'A';

                board[endy][endx][0] = '-'; //change the piece's end position on the board
                board[endy][endx][1] = '-';
                printf("Invalid Input. That move either doesn't escape check or puts you into check!\n");
            }
            else
            {
                updatePosition(player, startx, starty, endx, endy, cannonNum, Team, 'A');
                *isValid = 0;
            }
        }
    }
    else
    {
        printf("Invalid Input. The cannon can only move directly vertical or directly horizontal!\n");
    }
}
void soldierMove(struct Player *player, struct Player *otherplayer, int startx, int starty, int endx, int endy, int soldierNum, char Team, int *isValid)
{
    int changex = endx-startx;
    int changey = endy-starty;
    int pieceinWay = 0; 
    int acrossRiver = 0;
    if(starty>4 && Team == 'B') //checking if the soldier, depending on whether it's red or black, is across the river
    {
        acrossRiver = 1;
    }
    else if(starty < 5 && Team == 'R')
    {
        acrossRiver = 1;
    }

    if(abs(changex) + abs(changey) == 1) //we must move 1 space "forward" or horizontally
    {
        if((board[endy][endx][0] == 'B' && Team == 'B') || (board[endy][endx][0] == 'R' && Team == 'R')) 
        {
            pieceinWay++;
        }

        if(pieceinWay > 0)
        {
            printf("Invalid input. There is a piece in the way!\n");
        }
        else if((changey<0 && Team == 'B') || (changey>0 && Team == 'R')) //if the player attempts to move backwards
        {
            printf("Invalid input. The soldier cannot move backwards!\n");
        }
        else if(changex != 0 && acrossRiver != 1) //if the soldier isn't across the river, then you're not allowed to move horizontally
        {
            printf("Invalid input. You cannot move horizontally until you cross the river!\n");
        }
        else if((board[endy][endx][0] == 'R' && Team == 'B') || (board[endy][endx][0] == 'B' && Team == 'R'))
        {
            board[starty][startx][0] = '-'; //change the piece's start position to an empty space on the board
            board[starty][startx][1] = '-';

            char temp = board[endy][endx][0]; //storing the end piece's values in case the move doesn't remove us from check
            char temp2 = board[endy][endx][1];

            board[endy][endx][0] = player->team; //change the piece's end position on the board
            board[endy][endx][1] = 'S';
            // boardPrint();
            if(checkPrevention(player, otherplayer, player->king.x, player->king.y, 0)) //if the move was invalid because it didn't prevent us from being in check, then we restore the previous board
            {
                board[starty][startx][0] = player->team; //change the piece's start position to an empty space on the board
                board[starty][startx][1] = 'S';

                board[endy][endx][0] = temp; //change the piece's end position on the board
                board[endy][endx][1] = temp2;
                printf("Invalid Input. That move either doesn't escape check or puts you into check!\n");
            }
            else
            {
                takepiece(player, otherplayer, endx, endy, soldierNum);
                updatePosition(player, startx, starty, endx, endy, soldierNum, Team, 'S');
                *isValid = 0;
            }
        }
        else
        {
            board[starty][startx][0] = '-'; //change the piece's start position to an empty space on the board
            board[starty][startx][1] = '-';

            board[endy][endx][0] = player->team; //change the piece's end position on the board
            board[endy][endx][1] = 'S';
            if(checkPrevention(player, otherplayer, player->king.x, player->king.y, 0)) //if the move was invalid because it didn't prevent us from being in check, then we restore the previous board
            {
                board[starty][startx][0] = player->team; //change the piece's start position to an empty space on the board
                board[starty][startx][1] = 'S';

                board[endy][endx][0] = '-'; //change the piece's end position on the board
                board[endy][endx][1] = '-';
                printf("Invalid Input. That move either doesn't escape check or puts you into check!\n");
            }
            else
            {
                updatePosition(player, startx, starty, endx, endy, soldierNum, Team, 'S');
                *isValid = 0;
            }
        }   
    }
    else
    {
        printf("Invalid input. The soldier can only move 1 space vertically or horizontally at a time!\n");
    }
}

int checkPrevention(struct Player *player, struct Player *otherplayer, int x, int y, int use)
{
    for(int i=0; i<2; i++) //we're going to check all the enemy pieces excluding elephant and guard that are alive and see if they are in a position that can kill the king in 1 move
    {
        if(otherplayer->chariots[i].isalive == 1) //checking the chariots
        {
            int pieceinWay = 0;
            int xdiff = x - otherplayer->chariots[i].x;
            int ydiff = y - otherplayer->chariots[i].y;
            if(xdiff == 0) //the chariot can only move directly horizotal or directly vertical, so we're going to check the horizontal and vertical positions relative to the king
            {
                // printf("Moving vertical Check\n");
                if(ydiff < 0) //if we're downwards relative to the king's potential position
                {
                    for(int j=otherplayer->chariots[i].y-1; j>y; j--) //checks if there are pieces in the way from the chariot placing the king in check
                    {
                        if(board[j][otherplayer->chariots[i].x][0] != '-')
                        {
                            pieceinWay++;
                        }
                    }
                }
                else//if we're upwards relative to the king
                {
                    for(int j=otherplayer->chariots[i].y+1; j<y; j++) 
                    {
                        if(board[j][otherplayer->chariots[i].x][0] != '-')
                        {
                            pieceinWay++;
                        }
                    }

                }
                if(pieceinWay == 0) //if there were no pieces in the way and the king is along a path of movement, then it's a check
                {
                    if(use ==0)
                    {
                        otherplayer->chariots[i].check = 1;
                    }
                    return 1;
                }
            }
            else if(ydiff == 0) //same exact code, but changed for moving horizontally
            {
                if(xdiff < 0) //if we're rightwards relative to the king
                {
                    for(int j=otherplayer->chariots[i].x-1; j>x; j--)
                    {
                        if(board[otherplayer->chariots[i].y][j][0] != '-')
                        {
                            pieceinWay++;
                        }
                    }
                }
                else//if we're leftwards relative to the king
                {
                    for(int j=otherplayer->chariots[i].x+1; j<=x; j++)
                    {
                        if(board[otherplayer->chariots[i].y][j][0] != '-')
                        {
                            pieceinWay++;
                        }
                    }
                }
                if(pieceinWay == 0 && use == 0) 
                {
                    if(use ==0)
                    {
                        otherplayer->chariots[i].check = 1;
                    }
                    return 1;
                }

            }
        }
    }
    for(int i=0; i<2; i++) //checking the horses
    {
        if(otherplayer->horses[i].isalive == 1)
        {
            int pieceinWay = 0;
            int xdiff = x - otherplayer->horses[i].x;
            int ydiff = y - otherplayer->horses[i].y;
            if((abs(xdiff) == 2 && abs(ydiff) == 1) || (abs(xdiff) == 1 && abs(ydiff) == 2)) //we must move in an L shape for the movement to be valid
            {
                if(xdiff == 1 && ydiff == -2) //up, diagonal up right
                {
                    if(board[otherplayer->horses[i].y-1][otherplayer->horses[i].x][0] != '-') //checking upward 1 space movement to see if there's a piece in the way
                    {
                        pieceinWay++;
                    }
                    if(board[y][x-1][0] != '-' && board[y+1][x][0] != '-') //checking the diagonal movement to see if there's 2 pieces preventing the movement
                    {
                        pieceinWay++;
                    }
                }
                else if(xdiff == 2 && ydiff == -1) //right, diagonal up right
                {
                    if(board[otherplayer->horses[i].y][otherplayer->horses[i].x+1][0] != '-')
                    {
                        pieceinWay++;
                    }
                    if(board[y+1][x][0] != '-' && board[y][x-1][0] != '-')
                    {
                        pieceinWay++;
                    }
                }
                else if(xdiff == 2 && ydiff == 1) //right, diagonal down right
                {
                    if(board[otherplayer->horses[i].y][otherplayer->horses[i].x+1][0] != '-')
                    {
                        pieceinWay++;
                    }
                    if(board[y-1][x][0] != '-' && board[y][x-1][0] != '-')
                    {
                        pieceinWay++;
                    }
                }
                else if(xdiff == 1 && ydiff == 2) //down, diagonal down right
                {
                    if(board[otherplayer->horses[i].y+1][otherplayer->horses[i].x][0] != '-')
                    {
                        pieceinWay++;
                    }
                    if(board[y][x-1][0] != '-' && board[y-1][x][0] != '-')
                    {
                        pieceinWay++;
                    }
                }
                else if(xdiff == -1 && ydiff == 2) //down, diagonal down left
                {
                    if(board[otherplayer->horses[i].y+1][otherplayer->horses[i].x][0] != '-')
                    {
                        pieceinWay++;
                    }
                    if(board[y][x+1][0] != '-' && board[y-1][x][0] != '-')
                    {
                        pieceinWay++;
                    }
                }
                else if(xdiff == -2 && ydiff == 1) //left, diagonal down left
                {
                    if(board[otherplayer->horses[i].y][otherplayer->horses[i].x-1][0] != '-')
                    {
                        pieceinWay++;
                    }
                    if(board[y-1][x][0] != '-' && board[y][x+1][0] != '-')
                    {
                        pieceinWay++;
                    }
                }
                else if(xdiff == -2 && ydiff == -1) //left, diagonal up left
                {
                    if(board[otherplayer->horses[i].y][otherplayer->horses[i].x-1][0] != '-')
                    {
                        pieceinWay++;
                    }
                    if(board[y+1][x][0] != '-' && board[y][x+1][0] != '-')
                    {
                        pieceinWay++;
                    }
                }
                else if(xdiff == -1 && ydiff == -2) //up, diagonal up left
                {
                    if(board[otherplayer->horses[i].y-1][otherplayer->horses[i].x][0] != '-') //checking upward 1 space movement to see if there's a piece in the way
                    {
                        pieceinWay++;
                    }
                    if(board[y][x+1][0] != '-' && board[y+1][x][0] != '-') //checking the diagonal movement to see if there's 2 pieces preventing the movement
                    {
                        pieceinWay++;
                    }
                }
                if(pieceinWay == 0)
                {
                    if(use ==0)
                    {
                        otherplayer->horses[i].check = 1;
                    }
                    return 1;
                }
            }
        }
    }
    for(int j=0; j<2; j++) //checking both cannons
    {
        if(otherplayer->artillery[j].isalive == 1)
        {
            int changex = x-otherplayer->artillery[j].x;
            int changey = y-otherplayer->artillery[j].y;
            int pieceinWay = 0; 
            int pieceType = 0; 

            if(changex == 0) //the cannon can only move directly horizotal or directly vertical, so we're just making sure the user's attempt is either of these options
            {
                if(changey < 0) //if we're below the king
                {
                    for(int i=otherplayer->artillery[j].y-1; i>y; i--) //checks if an ally piece is in the way by looping through all the possible positions and checking if the spaces contain ally pieces
                    {
                        if(board[i][otherplayer->artillery[j].x][0] != '-') 
                        {
                            pieceinWay++;
                        }
                    }
                }
                else//if we're above the king
                {
                    for(int i=otherplayer->artillery[j].y+1; i<y; i++) 
                    {
                        if(board[i][otherplayer->artillery[j].x][0] != '-')
                        {
                            pieceinWay++;
                        }
                    }
                }
                //checks if the cannon is in position to take out the king; aka there's 1 space in between them
                if(pieceinWay == 1) 
                {
                    if(use ==0)
                    {
                        otherplayer->artillery[j].check = 1;
                    }
                    return 1;
                }
            }
            else if(changey == 0) //same exact code, but changed for moving horizontally
            {
                if(changex < 0) //if we're to the right of the king
                {
                    for(int i=otherplayer->artillery[j].x-1; i>x; i--) //checks if an ally piece is in the way
                    {
                        if(board[otherplayer->artillery[j].y][i][0] != '-')
                        {
                            pieceinWay++;
                        }
                    }
                }
                else//if we're to the left of the king
                {
                    for(int i=otherplayer->artillery[j].x+1; i<x; i++) 
                    {
                        if(board[otherplayer->artillery[j].y][i][0] != '-')
                        {
                            pieceinWay++;
                        }
                    }
                }
                if(pieceinWay == 1) 
                {
                    if(use ==0)
                    {
                        otherplayer->artillery[j].check = 1;
                    }
                    return 1;
                }
            }
        }
    }
    for(int j=0; j<5; j++)//checking the soldiers
    {            
        if(otherplayer->soldiers[j].isalive == 1)
        {
            int changex = x-otherplayer->soldiers[j].x;
            int changey = y-otherplayer->soldiers[j].y;
            // printf("changex: %d\tchangey: %d\n", changex, changey);
            if(abs(changex) + abs(changey) == 1) //we must be 1 space away from the king
            {
                // printf(".");
                if((changey>=0 && otherplayer->team == 'B') || (changey<=0 && otherplayer->team == 'R')) //checking if the soldier is in proximity to be able to attack the potential king's position
                {
                    if(use ==0)
                    {
                        otherplayer->artillery[j].check = 1;
                    }
                    return 1;
                }
            }
        }
    }
    if(use == 1 && ((y<=4 && player->team=='R') || (y>=5 && player->team=='B')))//checking the elephants if use==1
    {
        for(int j=0; j<2; j++)
        {
            if(otherplayer->elephants[j].isalive == 1)
            {
                int changex = x-otherplayer->elephants[j].x;
                int changey = y-otherplayer->elephants[j].y;
                int pieceinWay = 0;
                if(abs(changex) == 2 && abs(changey) == 2) //we must move 2 spaces diagoanl for the movement to be valid
                {
                    //checking all 4 possible movements of the elephant
                    if(changex == 2 && changey == -2) //up right diagonal
                    {
                        if((board[otherplayer->elephants[j].y-1][otherplayer->elephants[j].x][0] != '-') && (board[otherplayer->elephants[j].y][otherplayer->elephants[j].x+1][0] != '-'))
                        {
                            pieceinWay++;
                        }
                        if((board[otherplayer->elephants[j].y-2][otherplayer->elephants[j].x+1][0] != '-') && (board[otherplayer->elephants[j].y-1][otherplayer->elephants[j].x+2][0] != '-'))
                        {
                            pieceinWay++;
                        }
                    }
                    else if(changex == 2 && changey == 2) //down right diagonal
                    {
                        if((board[otherplayer->elephants[j].y+1][otherplayer->elephants[j].x][0] != '-') && (board[otherplayer->elephants[j].y][otherplayer->elephants[j].x+1][0] != '-'))
                        {
                            pieceinWay++;
                        }
                        if((board[otherplayer->elephants[j].y+2][otherplayer->elephants[j].x+1][0] != '-') && (board[otherplayer->elephants[j].y+1][otherplayer->elephants[j].x+2][0] != '-'))
                        {
                            pieceinWay++;
                        }
                    }
                    else if(changex == -2 && changey == 2) //down left diagonal
                    {
                        if((board[otherplayer->elephants[j].y+1][otherplayer->elephants[j].x][0] != '-') && (board[otherplayer->elephants[j].y][otherplayer->elephants[j].x-1][0] != '-'))
                        {
                            pieceinWay++;
                        }
                        if((board[otherplayer->elephants[j].y+2][otherplayer->elephants[j].x-1][0] != '-') && (board[otherplayer->elephants[j].y+1][otherplayer->elephants[j].x-2][0] != '-'))
                        {
                            pieceinWay++;
                        }
                    }
                    else if (changex == -2 && changey == -2) //up left diagonal
                    {
                        if((board[otherplayer->elephants[j].y-1][otherplayer->elephants[j].x][0] != '-') && (board[otherplayer->elephants[j].y][otherplayer->elephants[j].x-1][0] != '-'))
                        {
                            pieceinWay++;
                        }
                        if((board[otherplayer->elephants[j].y-2][otherplayer->elephants[j].x-1][0] != '-') && (board[otherplayer->elephants[j].y-1][otherplayer->elephants[j].x-2][0] != '-'))
                        {
                            pieceinWay++;
                        }
                    }

                    if(pieceinWay == 0)
                    {
                        return 1;
                    }
                }
            }
        }
    }
    if(use == 1 && (x>=3 && x <= 5) && ((y<=2 && y>=0 && player->team == 'R') || (y>=7 && y<=9 && player->team == 'B')))//checking guards if use==1
    {
        for(int j=0; j<2; j++)
        {
            if(otherplayer->guards[j].isalive == 1)
            {
                int changex = x-otherplayer->guards[j].x;
                int changey = y-otherplayer->guards[j].y;
                int pieceinWay = 0;

                if(changex == 1 && changey == -1) //up right diagonal
                {
                    if((board[otherplayer->guards[j].y-1][otherplayer->guards[j].x][0] != '-') && (board[otherplayer->guards[j].y][otherplayer->guards[j].x+1][0] != '-'))
                    {
                        pieceinWay++;
                    }
                }
                else if(changex == 1 && changey == 1) //down right diagonal
                {
                    if((board[otherplayer->guards[j].y+1][otherplayer->guards[j].x][0] != '-') && (board[otherplayer->guards[j].y][otherplayer->guards[j].x+1][0] != '-'))
                    {
                        pieceinWay++;
                    }
                }
                else if(changex == -1 && changey == 1) //down left diagonal
                {
                    if((board[otherplayer->guards[j].y+1][otherplayer->guards[j].x][0] != '-') && (board[otherplayer->guards[j].y][otherplayer->guards[j].x-1][0] != '-'))
                    {
                        pieceinWay++;
                    }
                }
                else if (changex == -1 && changey == -1) //up left diagonal
                {
                    if((board[otherplayer->guards[j].y-1][otherplayer->guards[j].x][0] != '-') && (board[otherplayer->guards[j].y][otherplayer->guards[j].x-1][0] != '-'))
                    {
                        pieceinWay++;
                    }
                }
                if(pieceinWay == 0)
                {
                    return 1;
                }
            }
        }    
    }

    return 0;
}
