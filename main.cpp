#include "Player.h"

int main()
{

    Board board;

    Player<true> whitePlayer(&board);

    Player<false> blackPlayer(&board);



    board.printBoard();



    return 0;
}