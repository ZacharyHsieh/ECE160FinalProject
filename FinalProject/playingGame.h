void playGame(int redgoesFirst, struct Player *blackteam, struct Player *redteam); //plays the game
void userMove(struct Player *player, struct Player *otherplayer);
void chariotMove(struct Player *player, struct Player *otherplayer, int startx, int starty, int endx, int endy, int chariotNum, char Team, int *isValid);
void horseMove(struct Player *player, struct Player *otherplayer, int startx, int starty, int endx, int endy, int horseNum, char Team, int *isValid);
void elephantMove(struct Player *player, struct Player *otherplayer, int startx, int starty, int endx, int endy, int elephantNum, char Team, int *isValid);
void guardMove(struct Player *player, struct Player *otherplayer, int startx, int starty, int endx, int endy, int guardNum, char Team, int *isValid);
void kingMove(struct Player *player, struct Player *otherplayer, int startx, int starty, int endx, int endy, char Team, int *isValid);
void cannonMove(struct Player *player, struct Player *otherplayer, int startx, int starty, int endx, int endy, int cannonNum, char Team, int *isValid);
void soldierMove(struct Player *player, struct Player *otherplayer, int startx, int starty, int endx, int endy, int cannonNum, char Team, int *isValid);
void takepiece(struct Player *player, struct Player *otherplayer, int x, int y, int pieceNum);
void updatePosition(struct Player *player, int startx, int starty, int endx, int endy, int pieceNum, char team, char pieceType);
int checkPrevention(struct Player *player, struct Player *otherplayer, int x, int y, int use);