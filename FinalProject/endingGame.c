#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "board.h"
#include "pieces.h"
#include "playingGame.h"
#include "endingGame.h"

int checkMate(struct Player *player, struct Player *otherplayer)
{
    if(!checkPrevention(player, otherplayer, player->king.x, player->king.y, 0)) //if we're not already in check, it's imposssible to be in check mate
    {
        return 0;
    }

    //the next check we can do is to see if the king is able to escape the check by moving; to do this, we're going to check all possible movements for the king
    int kingup = player->king.y-1; //checking if the king can escape check by moving up
    if((kingup >= 0 && player->team == 'B') || (kingup >=7 && player->team == 'R')) //the king is locked in the castle, so if the move for the king moves him outside his castle, then it's invalid and we don't bother checking it
    {
        if(!futureMoveCheck(player, otherplayer, player->king.x, kingup, 'K')) //if moving the king up brings us out of check, then we can't be in checkmate
        {
            return 0;
        }
    }
    int kingdown = player->king.y+1; //checking if the king can escape check by moving down
    if((kingdown <= 2 && player->team == 'B') || (kingup <=9 && player->team == 'R')) //the king is locked in the castle, so if the move for the king moves him outside his castle, then it's invalid and we don't bother checking it
    {
        if(!futureMoveCheck(player, otherplayer, player->king.x, kingdown, 'K')) //if moving the king down brings us out of check, then we can't be in checkmate
        {
            return 0;
        }
    }
    int kingright = player->king.x+1; //checking if the king can escape check by moving right
    if(kingright <=5) //the king is locked in the castle, so if the move for the king moves him outside his castle, then it's invalid and we don't bother checking it
    {
        if(!futureMoveCheck(player, otherplayer, kingright, player->king.y, 'K')) //if moving the king right brings us out of check, then we can't be in checkmate
        {
            return 0;
        }
    }
    int kingleft = player->king.x-1; //checking if the king can escape check by moving left
    if(kingleft >= 3) //the king is locked in the castle, so if the move for the king moves him outside his castle, then it's invalid and we don't bother checking it
    {
        if(!futureMoveCheck(player, otherplayer, kingleft, player->king.y, 'K')) //if moving the king left brings us out of check, then we can't be in checkmate
        {
            return 0;
        }
    }

    /* the last check is the find out a way in which we can take out any pieces that have us in check. To do this, we first need to discover which pieces
    actually have our player in check, and after we find that, we need to see if we have an existing piece that can immediately take out the pieces keeping us 
    in check*/
    for(int j=0; j<2; j++) //checking if there/which chariots on the opposite team that have the king in check
    {
        if(otherplayer->chariots[j].check)
        {
            if(checkPrevention(otherplayer, player, otherplayer->chariots[j].x, otherplayer->chariots[j].y, 1)) //checking if we can take out a chariot that has us in check
            {
                return 0;
            }
        }
    }
    for(int j=0; j<2; j++) //checking the horses
    {
        if(otherplayer->horses[j].check)
        {
            if(checkPrevention(otherplayer, player, otherplayer->horses[j].x, otherplayer->horses[j].y, 1)) //checking if we can take out a horse that has us in check
            {
                return 0;
            }
        }
    }
    //the king check is uneccesary, as the king can't actually die in this game.

    for(int j=0; j<2; j++) //checking the cannons
    {
        if(otherplayer->artillery[j].check)
        {
            if(checkPrevention(otherplayer, player, otherplayer->chariots[j].x, otherplayer->chariots[j].y, 1)) //checking if we can take out a cannon that has us in check
            {
                return 0;
            }
        }
    }
    for(int j=0; j<5; j++) //checking the soldiers
    {
        if(otherplayer->soldiers[j].check)
        {
            if(checkPrevention(otherplayer, player, otherplayer->chariots[j].x, otherplayer->chariots[j].y, 1)) //checking if we can take out a soldier that has us in check
            {
                return 0;
            }
        }
    }

    return 1;
}
int futureMoveCheck(struct Player *player, struct Player *otherplayer, int x, int y, char pieceType) //basically generalized and modified version of code written in each piece's move function
{
    //if our hypothetical move takes out an enemy piece
    if((board[y][x][0] == 'R' && player->team == 'B') || (board[y][x][0] == 'B' && player->team == 'R'))
    {
        board[y][x][0] = '-'; //change the piece's start position to an empty space on the board
        board[y][x][1] = '-';

        char temp = board[y][x][0]; //storing the end piece's values in case the move doesn't remove us from check
        char temp2 = board[y][x][1];

        board[y][x][0] = player->team; //change the piece's end position on the board
        board[y][x][1] = pieceType;
        // boardPrint();
        if(!checkPrevention(player, otherplayer, player->king.x, player->king.y, 0))  //if there's a move that can get us out of check from a given piece, then we are no longer in check if that move is made
        {
            board[y][x][0] = player->team; //change the piece's start position to an empty space on the board
            board[y][x][1] = pieceType;

            board[y][x][0] = temp; //change the piece's end position on the board
            board[y][x][1] = temp2;
            return 0;
        }   
        else //if we're still in check despite said move being made, then we just restore the hypothetical move we did
        {
            board[y][x][0] = player->team; //change the piece's start position to an empty space on the board
            board[y][x][1] = pieceType;

            board[y][x][0] = temp; //change the piece's end position on the board
            board[y][x][1] = temp2;
        }
    }
    else if(board[y][x][0] == '-') //the other valid move we can make is if we're just moving to an empty spot
    {
        board[y][x][0] = '-'; //change the piece's start position to an empty space on the board
        board[y][x][1] = '-';

        board[y][x][0] = player->team; //change the piece's end position on the board
        board[y][x][1] = pieceType;
        if(!checkPrevention(player, otherplayer, player->king.x, player->king.y, 0)) //same logic as earlier if statement
        {
            board[y][x][0] = player->team; //change the piece's start position to an empty space on the board
            board[y][x][1] = pieceType;

            board[y][x][0] = '-'; //change the piece's end position on the board
            board[y][x][1] = '-';
            return 0;
        }
        else
        {
            board[y][x][0] = player->team; //change the piece's start position to an empty space on the board
            board[y][x][1] = pieceType;

            board[y][x][0] = '-'; //change the piece's end position on the board
            board[y][x][1] = '-';
        }
    }
    return 1; //returns 1 if the move we're making with a given piece results in us still be in check
}
int endGame(struct Player *blackteam, struct Player *redteam)
{
    if(checkMate(blackteam, redteam)) //if red team placed black team in checkmate, then red team wins
    {
        blackteam->king.isalive == 0;
        printf("Red wins!\n");
        return 1;
    }
    else if(checkMate(redteam, blackteam)) //if the reverse happens
    {
        redteam->king.isalive == 1;
        printf("Black Wins!\n");
        return 1;
    }
    else if(forcedStalemate(redteam, blackteam) || forcedStalemate(blackteam, redteam)) //if the game is a forced stalemate
    {
        printf("Forced Stalemate!\n");
        return 1;
    }
    return 0;
}
int forcedStalemate(struct Player *player, struct Player *otherplayer)
{
    //checking if the king's have direct line of sight
    if(player->king.x - otherplayer->king.x == 0)
    {
        int pieceinWay = 0;
        if(player->team == 'B') //if we're on black team, we're gonna increment and check downwards
        {
            for(int i=player->king.y + 1; i<otherplayer->king.y; i++) //checking if there's any pieces blocking their line of sight
            {
                if(board[i][player->king.x][0] != '-')
                {
                    pieceinWay++;
                }
            }
        }
        else //if we're on the red team, we're gonna increment and check upwards
        {
            for(int i=player->king.y-1; i>otherplayer->king.y; i--)
            {
                if(board[i][player->king.x][0] != '-')
                {
                    pieceinWay++;
                }
            }
        }
        if(pieceinWay == 0) //if there were no pieces blocking line of sight, then this means that the king who moved there or plans on moving there is in check
        {
            return 1;
        }
    }
    return 0;
}

