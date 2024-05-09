int checkMate(struct Player *player, struct Player *otherplayer);
int futureMoveCheck(struct Player *player, struct Player *otherplayer, int x, int y, char pieceType);
int endGame(struct Player *blackteam, struct Player *redteam);
int forcedStalemate(struct Player *player, struct Player *otherplayer);