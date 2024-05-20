#include "Board.h"
#include <cmath>
#include <stdexcept>
#include <sstream>

void Board::parsePiecePlacement(const std::string& placement) {
    auto charToPieceType = [&](char ch) {
        switch (ch) {
            case 'p': return PieceType::PAWN;
            case 'r': return PieceType::ROOK;
            case 'n': return PieceType::KNIGHT;
            case 'b': return PieceType::BISHOP;
            case 'q': return PieceType::QUEEN;
            case 'k': return PieceType::KING;
            default: throw std::invalid_argument("Invalid piece type");
        }
    };

    std::cout << "parsePiecePlacement, placement = " << placement << std::endl;
    int whiteID = 0, blackID = 0;
    int r = 0, c = 0;
    for (size_t i = 0; i < placement.size(); i++) {
        char ch = placement[i];
        if (ch == '/') {
            r++;
            c = 0;
        }
        else if (std::isdigit(ch)) {
            c += ch - '0';
        }
        else {
            bool isWhite = std::isupper(ch);
            PieceType type = charToPieceType(std::tolower(ch));
            if (isWhite) {
                if (type == PieceType::KING) {
                    whiteKingPos = c + r*ncol;
                    board[c + r*ncol] = createPiece<true>(type, r, c, 15);
                    board[c + r*ncol]->_atInitialPos = false;
                } else {
                    board[c + r*ncol] = createPiece<true>(type, r, c, whiteID++);
                    board[c + r*ncol]->_atInitialPos = false;
                }

            }
            else {
                if (type == PieceType::KING) {
                    blackKingPos = c + r*ncol;
                    board[c + r*ncol] = createPiece<false>(type, r, c, 15);
                    board[c + r*ncol]->_atInitialPos = false;
                } else {
                    board[c + r*ncol] = createPiece<false>(type, r, c, blackID++);
                    board[c + r*ncol]->_atInitialPos = false;
                }
            }
            c++;
        }
    }
}

void Board::parseActiveColor(const std::string& color) {
    if (color == "w") 
        isWhiteToMove = true;
    else if (color == "b") 
        isWhiteToMove = false;
    else 
        throw std::runtime_error("Invalid active color");
}

void Board::parseCastlingAvailability(const std::string& castlingAvailability) {
    bool whiteCanCastleQueenSide=false, whiteCanCatsleKingSide=false, blackCanCastleQueenSide=false, blackCanCatsleKingSide=false;

    for (char ch : castlingAvailability) {
        switch (ch) {
            case 'K': whiteCanCatsleKingSide = true; break;
            case 'Q': whiteCanCastleQueenSide = true; break;
            case 'k': blackCanCatsleKingSide = true; break;
            case 'q': blackCanCastleQueenSide = true; break;
            case '-': break;
            default: throw std::runtime_error("Invalid castling availability");
        }
    }
    
    if (whiteCanCastleQueenSide || whiteCanCatsleKingSide) {
        if (board[4] == nullptr || board[4]->_type != PieceType::KING || !board[4]->_isWhite) 
            throw std::runtime_error("Invalid castling availability");

        board[4]->_atInitialPos = true; 
        if (whiteCanCastleQueenSide) {
            if (board[0] == nullptr || board[0]->_type != PieceType::ROOK || !board[0]->_isWhite) 
                throw std::runtime_error("Invalid castling availability");
            board[0]->_atInitialPos = true;  
        }
        else if (whiteCanCatsleKingSide) {
            if (board[7] == nullptr || board[7]->_type != PieceType::ROOK || !board[7]->_isWhite) 
                throw std::runtime_error("Invalid castling availability");
            board[7]->_atInitialPos = true;    
        }
    }

    if (blackCanCastleQueenSide || blackCanCatsleKingSide) {
        if (board[4 + 7*ncol] == nullptr || board[4 + 7*ncol]->_type != PieceType::KING || board[4 + 7*ncol]->_isWhite) 
            throw std::runtime_error("Invalid castling availability");

        board[4 + 7*ncol]->_atInitialPos = true; 

        if (blackCanCastleQueenSide) {
            if (board[0 + 7*ncol] == nullptr || board[0 + 7*ncol]->_type != PieceType::ROOK || board[0 + 7*ncol]->_isWhite) 
                throw std::runtime_error("Invalid castling availability");
            board[0 + 7*ncol]->_atInitialPos = true;  
        }
        else if (blackCanCatsleKingSide) {
            if (board[7 + 7*ncol] == nullptr || board[7 + 7*ncol]->_type != PieceType::ROOK || board[7 + 7*ncol]->_isWhite) 
                throw std::runtime_error("Invalid castling availability");
            board[7 + 7*ncol]->_atInitialPos = true;    
        }
    }    
    
}

void Board::parseEnPassantTargetSquare(const std::string& enPassantTargetSquare){
    if (enPassantTargetSquare == "-") 
        return;
    
    int c = enPassantTargetSquare[0] - 'a';
    int r = enPassantTargetSquare[1] - '1';
    if (r == 3) {
        lastMove.curPos =   ncol + c;
        lastMove.newPos = 4*ncol + c;
    }
    else if (r == 6) {
        lastMove.curPos = 7*ncol + c;
        lastMove.newPos = 5*ncol + c;
    }
    else 
        throw std::runtime_error("Invalid en passant target square");
}

void Board::init(std::string fenString) {
    std::istringstream iss(fenString);
    std::string part;
    std::vector<std::string> parts;
    
    while (getline(iss, part, ' ')) 
        parts.push_back(part);

    if (parts.size() != 6) 
        throw std::runtime_error("Invalid FEN string");
    
    parsePiecePlacement(parts[0]);
    parseActiveColor(parts[1]);
    parseCastlingAvailability(parts[2]);
    parseEnPassantTargetSquare(parts[3]);

    setupPieces();
}

template<bool isWhite>
void Board::makeMove(const Move& move) {
    lastMovePieceChangeID = -1;
    Piece* p = board[move.newPos].get();
    if (p != nullptr) {
        p->_onBoard = false;
        lastMovePieceChangeID = board[move.newPos]->_id;
    }

    lastMoveFromInitPos = board[move.curPos]->_atInitialPos;
    board[move.curPos]->_atInitialPos = false;

    int c = move.newPos%ncol;
    int r = move.newPos/ncol;

    if (move.newPieceType != NONE) {
        int id = board[move.curPos]->_id;
        board[move.newPos] = createPiece<isWhite>(move.newPieceType, r, c, id);
        board[move.newPos]->_atInitialPos = false;
        board[move.curPos] = nullptr;

        std::vector<std::shared_ptr<Piece> >& pieces = (isWhite) ? whitePieces : blackPieces;
        pieces[id] = board[move.newPos];
    }
    else {
        // en passant
        if (board[move.curPos]->_type == PAWN && move.curPos%ncol != c && board[move.newPos] == nullptr) {
            board[lastMove.newPos]->_onBoard = false;
            lastMovePieceChangeID = board[lastMove.newPos]->_id;
            board[lastMove.newPos] = nullptr;     
        }
        board[move.newPos] = board[move.curPos]; // move piece to new cell
        board[move.newPos]->updatePosition(move.newPos); // reset piece position
        board[move.curPos] = nullptr; // remove current piece on the old cell
        
        if (board[move.newPos]->_type == KING) {
            if (isWhite) 
                whiteKingPos = move.newPos;
            else 
                blackKingPos = move.newPos;
            
            // castling (move rook)
            if (std::abs(move.curPos%ncol - c) == 2) {
                int rook_c = (ncol - c -1 < c) ? ncol-1 : 0;
                int ind = rook_c + r*ncol;
                int ind_new = (ncol - c -1 < c) ?  move.newPos - 1 : move.newPos + 1;
                board[ind_new] = board[ind];
                board[ind_new]->updatePosition(ind_new);
                board[ind_new]->_atInitialPos = false;
                board[ind] = nullptr;     
            }
        }
    }

    lastMove2 = lastMove;
    lastMove = move;
    isWhiteToMove = !isWhiteToMove;
}

// call makeMove first before calling this function
template<bool isWhite>
void Board::undoLastMove() {    
    int c = lastMove.newPos%ncol;
    int r = lastMove.newPos/ncol;

    if (lastMove.newPieceType != NONE) {
        int id = board[lastMove.newPos]->_id;
        board[lastMove.newPos].reset();
        if (lastMovePieceChangeID >= 0) {
            board[lastMove.newPos] = (isWhite) ? whitePieces[lastMovePieceChangeID] : blackPieces[lastMovePieceChangeID];
            board[lastMove.newPos]->_onBoard = true;
        }
        board[lastMove.curPos] = createPiece<isWhite>(PieceType::PAWN, lastMove.curPos/ncol, lastMove.curPos%ncol, id);
        board[lastMove.curPos]->_atInitialPos = lastMoveFromInitPos;
        
        std::vector<std::shared_ptr<Piece> >& pieces = (isWhite) ? whitePieces : blackPieces;
        pieces[id] = board[lastMove.curPos];
    
    }
    else {
        board[lastMove.curPos] = board[lastMove.newPos];
        board[lastMove.curPos]->updatePosition(lastMove.curPos);  
        board[lastMove.curPos]->_atInitialPos = lastMoveFromInitPos;
        board[lastMove.newPos] = nullptr;
        if (lastMovePieceChangeID >= 0) {
            std::vector<std::shared_ptr<Piece> >& pieces = (isWhite) ? blackPieces : whitePieces; 
            pieces[lastMovePieceChangeID]->_onBoard = true;
            board[pieces[lastMovePieceChangeID]->getPosition()] = pieces[lastMovePieceChangeID];
        }  

        // castling (move rook)
        if (board[lastMove.curPos]->_type == KING) {
            if (isWhite) 
                whiteKingPos = lastMove.curPos;
            else 
                blackKingPos = lastMove.curPos;
                
            if (std::abs(lastMove.curPos%ncol - c) == 2) {
                int rook_c = (ncol - c -1 < c) ? ncol-1 : 0;
                int ind = rook_c + r*ncol;
                int ind_new = (ncol - c -1 < c) ?  lastMove.newPos - 1 : lastMove.newPos + 1;
                board[ind] = board[ind_new]; 
                board[ind]->updatePosition(ind);
                board[ind]->_atInitialPos = true;
                board[ind_new] = nullptr;   
            }
        }
        
    }
    
    lastMovePieceChangeID = -1;
    lastMove = lastMove2;
    isWhiteToMove = !isWhiteToMove;
}

template void Board::makeMove<true>(const Move& move);
template void Board::makeMove<false>(const Move& move);
template void Board::undoLastMove<true>();
template void Board::undoLastMove<false>();