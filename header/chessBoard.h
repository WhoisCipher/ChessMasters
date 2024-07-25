#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

enum class PieceColor { Black, White, GameOver };
enum class Piece { None, Pawn, Rook, Knight, Bishop, Queen, King };

class ChessBoard {
protected:
    sf::Vector2f position;
    sf::Sprite sprite;
    PieceColor color;
    Piece pieceType;
    float scale = 1;
    int moves = 0;
    bool pinned;

public:
    ChessBoard();
    bool hunYouChecked = false;

    sf::Sprite& getSprite();
    virtual PieceColor getPieceColor();
    virtual Piece getPiece();

    virtual void setPinned(bool pin);
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void setPosition(int i, int j);
    virtual void moveCounter();

    virtual int getMoves();
    virtual int getRookMoves();
    virtual int getKnightMoves();
    virtual int getBishopMoves();
    virtual int getQueenMoves();
    virtual int getKingMoves();
    virtual int getPawnMoves();

    void suspendGame();

    virtual bool QueenSideCastle();
    virtual bool KingSideCastle();
    virtual bool enPassantAvailable();

    bool contains(sf::Vector2f point) const;

    virtual std::vector<sf::Vector2i> getMovesForCheck(sf::Vector2i currentPos, ChessBoard* board[][8]);
    virtual std::vector<sf::Vector2i> getLegalMoves(sf::Vector2i currentPos, ChessBoard* board[][8]);
};