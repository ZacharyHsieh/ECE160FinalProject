#include <stdio.h>
#include "board.h"
#include "pieces.h"

int userFirst()
{
    char turnChoice;
    int isValid = 1;
    do
    {
        printf("Would Red like to go first(Y or N)?\n");
        scanf("%c", &turnChoice);
        getchar();
        if(turnChoice == 'Y' || turnChoice == 'N')
        {
            isValid = 0;
        }
        else
        {
            printf("Invalid Input\n");
        }
    } while (isValid);
    
    if(turnChoice == 'Y')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}  

struct Player playerInit(char team)
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
    else //if we're initialiing a player in control of the red pieces
    {
        edgepieceY = 9;
        soldierY = 6;
        cannonY = 7;
    }
    struct Chariot chariots[2];
    chariots[0] = (struct Chariot) {0, edgepieceY, team, 1, 0};
    chariots[1] = (struct Chariot) {8, edgepieceY, team, 1, 0};
    
    struct Horse horses[2];
    horses[0] = (struct Horse) {1, edgepieceY, team, 1, 0};
    horses[1] = (struct Horse) {7, edgepieceY, team, 1, 0};

    struct Elephant elephants[2];
    elephants[0] = (struct Elephant) {2, edgepieceY, team, 1, 0};
    elephants[1] = (struct Elephant) {6, edgepieceY, team, 1, 0};

    struct Guard guards[2];
    guards[0] = (struct Guard) {3, edgepieceY, team, 1, 0};
    guards[1] = (struct Guard) {5, edgepieceY, team, 1, 0};

    struct King king = {4, edgepieceY, 0, 1, 0};

    struct Infantry soldiers[5];
    for(int i=0; i<5; i++)
    {
        soldiers[i] = (struct Infantry) {2 * i, soldierY, team, 1, 0};
    }

    struct Cannon cannons[2];
    cannons[0] = (struct Cannon) {1, cannonY, team, 1, 0};
    cannons[1] = (struct Cannon) {7, cannonY, team, 1, 0};
    
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