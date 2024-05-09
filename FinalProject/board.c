#include "board.h"
#include<stdio.h>

char board[10][9][2];

void boardInit()
{
    for(int i=0; i<10; i++)
    {
        for(int j=0; j<9; j++)
        {
            if(i==0) //top row is the starting position for black pieces
            {
                if(j==0 || j==8) //chariot
                {
                    board[i][j][0] = 'B'; //chariot; B stands for black piece and C stands for chariot
                    board[i][j][1] = 'C';
                }
                if(j==1 || j==7) //horse
                {
                    board[i][j][0] = 'B'; //H stands for horse
                    board[i][j][1] = 'H';
                }
                if(j==2 || j==6) //Elephant
                {
                    board[i][j][0] = 'B'; //elephant
                    board[i][j][1] = 'E';
                }
                if(j==3 || j==5) //Guard
                {
                    board[i][j][0] = 'B'; //guard
                    board[i][j][1] = 'G';
                }
                if(j==4) //King
                {
                    board[i][j][0] = 'B'; //king
                    board[i][j][1] = 'K';
                }
            }
            else if((i==2 && j==1) || (i==2 && j==7))//Black Cannon; A stands for artillery/cannon
            {
                board[i][j][0] = 'B';
                board[i][j][1] = 'A';
            }
            else if((i==3 && j==0) || (i==3 && j==2) || (i==3 && j==4) || (i==3 && j==6) || (i==3 && j==8)) //Black Soldier; S is for soldier
            {
                board[i][j][0] = 'B';
                board[i][j][1] = 'S';
            }
            else if(i==9) //Red Pieces are the on bottom row
            {
                if(j==0 || j==8) //chariot
                {
                    board[i][j][0] = 'R'; //chariot; R stands for red piece and C stands for chariot
                    board[i][j][1] = 'C';
                }
                if(j==1 || j==7) //horse
                {
                    board[i][j][0] = 'R'; //H stands for horse
                    board[i][j][1] = 'H';
                }
                if(j==2 || j==6) //Elephant
                {
                    board[i][j][0] = 'R'; //elephant
                    board[i][j][1] = 'E';
                }
                if(j==3 || j==5) //Guard
                {
                    board[i][j][0] = 'R'; //guard
                    board[i][j][1] = 'G';
                }
                if(j==4) //King
                {
                    board[i][j][0] = 'R'; //king
                    board[i][j][1] = 'K';
                }
            }
            else if((i==7 && j==1) || (i==7 && j==7)) //red cannons
            {
                board[i][j][0] = 'R';
                board[i][j][1] = 'A';
            }
            else if((i==6 && j==0) || (i==6 && j==2) || (i==6 && j==4) || (i==6 && j==6) || (i==6 && j==8))//red soldiers
            {
                board[i][j][0] = 'R';
                board[i][j][1] = 'S';
            }
            else //standard board representation
            {
                board[i][j][0] = '-';
                board[i][j][1] = '-';
            }

        }
    }
}

void boardPrint()
{
    for(int i=0; i<10; i++)
    {
        for(int j=0; j<9; j++)
        {
            if(board[i][j][0] == 'B') //if the piece is black, check to see which black piece to print
            {
                if(board[i][j][1] == 'C') //chariot
                {
                    printf("車");
                }
                else if(board[i][j][1] == 'H') //horse
                {
                    printf("馬");
                }
                else if(board[i][j][1] == 'E') //Elephant
                {
                    printf("象");
                }
                else if(board[i][j][1] == 'G') //Guard
                {
                    printf("士");
                }
                else if(board[i][j][1] == 'K') //King
                {
                    printf("將");
                }
                else if(board[i][j][1] == 'A')//Black Cannon
                {
                    printf("砲");
                }
                else if(board[i][j][1] == 'S') //Black Soldier
                {
                    printf("卒");
                }
            }

            else if(board[i][j][0] == 'R') //If the piece is red, check which red piece to print
            {
                printf("\033[0;31m"); //set text color to red
                if(board[i][j][1] == 'C') //chariot
                {
                    printf("俥");
                    printf("\033[0m"); //reset text color
                }
                else if(board[i][j][1] == 'H') //horse //horse
                {
                    printf("傌");
                    printf("\033[0m");
                }
                else if(board[i][j][1] == 'E') //Elephant
                {
                    printf("相");
                    printf("\033[0m");
                }
                else if(board[i][j][1] == 'G') //Guard //Guard
                {
                    printf("仕");
                    printf("\033[0m");
                }
                else if(board[i][j][1] == 'K') //King
                {
                    printf("帥");
                    printf("\033[0m");
                }
                else if(board[i][j][1] == 'A') //artillery
                {
                    printf("炮");
                    printf("\033[0m");
                }
                else if(board[i][j][1] == 'S') //soldier
                {
                    printf("兵");
                    printf("\033[0m");
                }
            }
            else //standard board representation
            {
                printf("➕");
            }

        }
        printf("\n");
    }
}