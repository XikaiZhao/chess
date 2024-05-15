#include "Player.h"

int main()
{
    Board board;
    Player<true> whitePlayer(&board);
    Player<false> blackPlayer(&board);

    return 0;
}