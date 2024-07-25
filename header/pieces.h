#pragma once

#include"chessBoard.h"
#include<SFML/Graphics.hpp>
using namespace std;
using namespace sf;

class Pawn : public ChessBoard {
private:
    Piece piece = Piece::Pawn;
    bool isEnPassant = false;
public:
    Pawn(const Texture& texture, PieceColor pieceColor);

    PieceColor getPieceColor();
    Piece getPiece() const;

    void setPinned(bool pin);
    void draw(RenderWindow& window);
    void setPosition(int i, int j);
    void moveCounter();

    int getMoves() const;
    int getPawnMoves() const;
    bool enPassantAvailable();
    bool isBlackPawn(Vector2i position );

    std::vector<Vector2i> getMovesForCheck(Vector2i currentPos, ChessBoard* board[][8]);
    std::vector<Vector2i> getLegalMoves(Vector2i currentPos, ChessBoard* board[][8]);
};
class Knight : public ChessBoard {
private:
    Piece piece = Piece::Knight;
public:
    Knight(const Texture& texture, PieceColor pieceColor);

    PieceColor getPieceColor();
    Piece getPiece();

    void setPinned(bool pin);
    void draw(RenderWindow& window);
    void setPosition(int i, int j);
    void moveCounter();

    int getMoves();
    int getKnightMoves();

    vector<Vector2i> getMovesForCheck(Vector2i currentPos, ChessBoard* board[][8]);
    vector<Vector2i> getLegalMoves(Vector2i currentPos, ChessBoard* board[][8]);
};
class Rook : public ChessBoard {
private:
    Piece piece = Piece::Rook;
public:
    Rook(const Texture& texture, PieceColor pieceColor);

    PieceColor getPieceColor();
    Piece getPiece();

    void setPinned(bool pin);
    void draw(RenderWindow& window);
    void setPosition(int i, int j);
    void moveCounter();

    int getMoves();
    int getRookMoves();

    vector<Vector2i> getMovesForCheck(Vector2i currentPos, ChessBoard* board[][8]);
    vector<Vector2i> getLegalMoves(Vector2i currentPos, ChessBoard* board[][8]);
};
class Bishop : public ChessBoard {
private:
    Piece piece = Piece::Bishop;
public:
    Bishop(const Texture& texture, PieceColor pieceColor);

    PieceColor getPieceColor();
    Piece getPiece();

    void setPinned(bool pin);
    void draw(RenderWindow& window);
    void setPosition(int i, int j);
    void moveCounter();

    int getMoves();
    int getBishopMoves();

    vector<Vector2i> getMovesForCheck(Vector2i currentPos, ChessBoard* board[][8]);
    vector<Vector2i> getLegalMoves(Vector2i currentPos, ChessBoard* board[][8]);
};
class Queen : public ChessBoard {
private:
    Piece piece = Piece::Queen;
public:
    Queen(const Texture& texture, PieceColor pieceColor);

    PieceColor getPieceColor();
    Piece getPiece();

    void setPinned(bool pin);
    void draw(RenderWindow& window);
    void setPosition(int i, int j);
    void moveCounter();

    int getMoves();
    int getQueenMoves();

    vector<Vector2i> getMovesForCheck(Vector2i currentPos, ChessBoard* board[][8]);
    vector<Vector2i> getLegalMoves(Vector2i currentPos, ChessBoard* board[][8]);
};
class King : public ChessBoard {
private:
    Piece piece = Piece::King;
    bool canCastleQueenSide = false;
    bool canCastleKingSide = false;
public:
    King(const Texture& texture, PieceColor pieceColor);

    PieceColor getPieceColor();
    Piece getPiece();

    void draw(RenderWindow& window);
    void setPosition(int i, int j);
    void moveCounter();

    int getMoves();
    int getKingMoves();
    bool QueenSideCastle();
    bool KingSideCastle();

    std::vector<Vector2i> getMovesForCheck(Vector2i currentPos, ChessBoard* board[][8]);
    std::vector<Vector2i> getLegalMoves(Vector2i currentPos, ChessBoard* board[][8]);
};