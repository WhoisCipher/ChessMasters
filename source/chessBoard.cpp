#include"chessBoard.h"
#include"SFML/Graphics.hpp"
using namespace sf;
using namespace std;

ChessBoard::	ChessBoard() {
position = { -1,-1 };
float scale = 0.45;
color = PieceColor::Black;
pieceType = Piece::None;
pinned = false;
}
Sprite& ChessBoard::getSprite() { return sprite; }
PieceColor ChessBoard::getPieceColor() { return PieceColor::Black; }
Piece ChessBoard::getPiece() { return Piece::Pawn; }
void ChessBoard::setPinned(bool pin) { pinned = pin; }
void ChessBoard::draw(RenderWindow& window) {};
void ChessBoard::setPosition(int i, int j) {}
void ChessBoard::moveCounter() {}
int ChessBoard::getMoves() { return 0; }
int ChessBoard::getRookMoves() { return 0; }
int ChessBoard::getKnightMoves() { return 0; }
int ChessBoard::getBishopMoves() { return 0; }
int ChessBoard::getQueenMoves() { return 0; }
int ChessBoard::getKingMoves() { return 0; }
int ChessBoard::getPawnMoves() { return 0; }
void ChessBoard::suspendGame() { color = PieceColor::GameOver; }
bool ChessBoard::QueenSideCastle() { return false; }
bool ChessBoard::KingSideCastle() { return false; }
bool ChessBoard::enPassantAvailable() { return false; }
bool ChessBoard::contains(Vector2f point) const { return sprite.getGlobalBounds().contains(point); }
vector<Vector2i> ChessBoard::getMovesForCheck(Vector2i currentPos, ChessBoard* board[][8]) { std::vector<Vector2i> legalMoves; return legalMoves; }
vector<Vector2i> ChessBoard::getLegalMoves(Vector2i currentPos, ChessBoard* board[][8]) { std::vector<Vector2i> legalMoves; return legalMoves; }