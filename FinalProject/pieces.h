struct Chariot{
    int x;
    int y;
    char team;
    int isalive;
    int check;
    
};
struct Horse{
    int x;
    int y;
    char team;
    int isalive;
    int check;
};
struct Elephant{
    int x;
    int y;
    char team;
    int isalive;
    int check;
};
struct Guard{
    int x;
    int y;
    char team;
    int isalive;
    int check;
};
struct King{
    int x;
    int y;
    char team;
    int isalive;
    int check;
};
struct Cannon{
    int x;
    int y;
    char team;
    int isalive;
    int check;
};
struct Infantry{
    int x;
    int y;
    char team;
    int isalive;
    int check;
}; 
struct Player{
    struct Chariot chariots[2];
    struct Horse horses[2];
    struct Elephant elephants[2];
    struct Guard guards[2];
    struct King king;
    struct Infantry soldiers[5];
    struct Cannon artillery[2];
    char team; //team B is black, team R is red
};
