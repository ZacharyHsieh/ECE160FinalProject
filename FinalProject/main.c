#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include "board.h"
#include "pieces.h"
#include "startGame.h"
#include "playingGame.h"
#include "endingGame.h"


void testCheckBoardInit() //temporary function that creates a board easier for checking
{
    for(int i=0; i<10; i++)
    {
        for(int j=0; j<9; j++)
        {
            if(i==0)
            {
                if(j==3||j==5)
                {
                    board[i][j][0] = 'B';
                    board[i][j][1] = 'G';
                }
                if(j==4)
                {
                    board[i][j][0] = 'B';
                    board[i][j][1] = 'K';
                }
                if(j==2 || j==6)
                {
                    board[i][j][0] = 'B';
                    board[i][j][1] = 'E';
                }
                if(j==0 || j==8)
                {
                    board[i][j][0] = 'B';
                    board[i][j][1] = 'C';
                }
                if(j==1)
                {
                    board[i][j][0] = 'B';
                    board[i][j][1] = 'H';
                }

            }
            else if(j==5 && i==9)
            {
                board[i][j][0] = 'B';
                board[i][j][1] = 'S';
            }
            else if(i==3 && (j==2 || j==4 || j==6 ||j==8)) //Black Soldier; S is for soldier
            {
                board[i][j][0] = 'B';
                board[i][j][1] = 'S';
            }
            else if(i==9)
            {
                if(j==8 || j==0)
                {
                    board[i][j][0] = 'R';
                    board[i][j][1] = 'C';
                }
                if(j==4)
                {
                    board[i][j][0] = 'R';
                    board[i][j][1] = 'K';
                }
                if(j==3 || j==5)
                {
                    board[i][j][0] = 'R';
                    board[i][j][1] = 'G';
                }
                if(j==2 || j==6)
                {
                    board[i][j][0] = 'R';
                    board[i][j][1] = 'E';
                }
                if(j==1 || j==7)
                {
                    board[i][j][0] = 'R';
                    board[i][j][1] = 'H';
                }
            }
            else
            {
                board[i][j][0] = '-';
                board[i][j][0] = '-';
            }
        }
    }
}
struct Player testplayerInit(char team)
{
    int edgepieceY; //the Y position of pieces on the (top or bottom) edge depends on the team
    int soldierY; //the Y position of the pieces on the (top or bottom) edge depends on the team
    int cannonY; //the Y position of the cannon depends on the team
    if(team == 'B') //if we're initializing a player in control of the black pieces
    {
        edgepieceY = 0;
        soldierY = 3;
        cannonY = 2;
    }
    else //if we're initializing a player in control of the red pieces
    {
        edgepieceY = 9;
        soldierY = 6;
        cannonY = 7;
    }
    struct Guard guards[2];
    guards[0] = (struct Guard) {3, edgepieceY, team, 1};
    guards[1] = (struct Guard) {5, edgepieceY, team, 0};

    struct King king = {4, edgepieceY, 0, 1};

    struct Chariot chariots[2];
    chariots[0] = (struct Chariot) {0, edgepieceY, team, 1};
    chariots[1] = (struct Chariot) {8, edgepieceY, team, 1};

    struct Horse horses[2];
    horses[0] = (struct Horse) {1, edgepieceY, team, 1};
    horses[1] = (struct Horse) {7, edgepieceY, team, 1};

    struct Elephant elephants[2];
    elephants[0] = (struct Elephant) {2, edgepieceY, team, 1};
    elephants[1] = (struct Elephant) {6, edgepieceY, team, 1};

    struct Cannon cannons[2];
    cannons[0] = (struct Cannon) {1, cannonY, team, 1};
    cannons[1] = (struct Cannon) {7, cannonY, team, 1};

    struct Infantry soldiers[5];
    for(int i=0; i<5; i++)
    {
        if(i==0 && team=='B')
        {
            soldiers[i] = (struct Infantry) {5, 9, team, 1};
        }
        else
        {
            soldiers[i] = (struct Infantry) {2 * i, soldierY, team, 1};
        }
    }

    struct Player createdteam = 
    {
        .chariots = {chariots[0], chariots[1]},
        .horses = {horses[0], horses[1]},
        .elephants = {elephants[0], elephants[1]},
        .guards = {guards[0], guards[1]},
        .king = king,
        .soldiers = {soldiers[0], soldiers[1], soldiers[2], soldiers[3], soldiers[4]},
        .artillery = {cannons[0], cannons[1]},
        .team = team
    };
    return createdteam;
}


int main()
{
    boardInit();
    boardPrint();
    struct Player blackTeam = playerInit('B');
    struct Player redTeam = playerInit('R');

    playGame(userFirst(), &blackTeam, &redTeam);
    return 0;
}

