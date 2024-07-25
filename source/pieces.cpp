#include"pieces.h"
#include"chessBoard.h"
#include<SFML/Graphics.hpp>
using namespace std;
using namespace sf;

//Pawn
Pawn::Pawn(const Texture& texture, PieceColor pieceColor) {
    color = pieceColor;
    sprite.setTexture(texture);
    sprite.setScale(scale, scale);
}
PieceColor Pawn::getPieceColor() { return color; }
Piece Pawn::getPiece() const { return piece; }
void Pawn::setPinned(bool pin) { pinned = pin; }
void Pawn::draw(RenderWindow& window) { window.draw(sprite); }
void Pawn::setPosition(int i, int j) { sprite.setPosition(j * 80, i * 80); }
void Pawn::moveCounter() { moves++; }
int Pawn::getMoves() const { return moves; }
int Pawn::getPawnMoves() const { return moves; }
bool Pawn::enPassantAvailable() { return isEnPassant; }
bool Pawn::isBlackPawn(Vector2i position) { return position.x == 1; }
std::vector<Vector2i> Pawn::getMovesForCheck(Vector2i currentPos, ChessBoard* board[][8]) {
    std::vector<Vector2i> diagonalMoves;

    int direction = (color == PieceColor::White) ? 1 : -1;

    // Diagonal moves
    int dx[2] = { 1, -1 };
    int dy = direction;

    for (int i = 0; i < 2; ++i) {
        int newX = currentPos.x + dx[i];
        int newY = currentPos.y + dy;

        if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
            ChessBoard* targetSquare = board[newX][newY];
            if (targetSquare != nullptr && targetSquare->getPieceColor() != color) {
                diagonalMoves.push_back(Vector2i(newX, newY));
            }
        }
    }

    return diagonalMoves;
}
std::vector<Vector2i> Pawn::getLegalMoves(Vector2i currentPos, ChessBoard* board[][8]) {
    std::vector<Vector2i> legalMoves;
    ChessBoard* currentPiece = board[currentPos.x][currentPos.y];

    Vector2i kingPos;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] != nullptr) {
                board[i][j]->setPinned(false);
            }
        }
    }
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] != nullptr && board[i][j]->getPiece() == Piece::King && board[i][j]->getPieceColor() == color) {
                kingPos = Vector2i(i, j);
                break;
            }
        }
    }

    int towardsX = kingPos.x - currentPos.x;
    int towardsY = kingPos.y - currentPos.y;
    int xDir = (towardsX == 0) ? 0 : ((towardsX > 0) ? -1 : 1);
    int yDir = (towardsY == 0) ? 0 : ((towardsY > 0) ? -1 : 1);
    bool diagonalMovement = (abs(towardsX) == abs(towardsY));

    bool isClear = true;

    int xDirToKing = (kingPos.x > currentPos.x) ? 1 : (kingPos.x < currentPos.x) ? -1 : 0;
    int yDirToKing = (kingPos.y > currentPos.y) ? 1 : (kingPos.y < currentPos.y) ? -1 : 0;

    for (int x = currentPos.x + xDirToKing, y = currentPos.y + yDirToKing; x != kingPos.x || y != kingPos.y; x += xDirToKing, y += yDirToKing) {
        if (board[x][y] != nullptr) {
            isClear = false;
            break;
        }
    }

    if (isClear) {
        if (towardsX == 0 || towardsY == 0 || diagonalMovement) {
            int x = currentPos.x + xDir;
            int y = currentPos.y + yDir;

            if (diagonalMovement) {
                while (x >= 0 && x < 8 && y >= 0 && y < 8) { // Check within board bounds
                    if (board[x][y] != nullptr) {
                        if (board[x][y]->getPiece() != Piece::King && board[x][y]->getPieceColor() != color) {
                            Piece opponentPiece = board[x][y]->getPiece();
                            if (opponentPiece == Piece::Bishop || opponentPiece == Piece::Queen) {
                                pinned = true;
                                break;
                            }
                        }
                        break;
                    }
                    x += xDir;
                    y += yDir;
                }
            }
            else {
                int x = currentPos.x + xDir;
                int y = currentPos.y + yDir;

                while (x >= 0 && x < 8 && y >= 0 && y < 8) { // Check within board bounds
                    if (board[x][y] != nullptr) {
                        if (board[x][y]->getPieceColor() != color) {
                            Piece opponentPiece = board[x][y]->getPiece();
                            if (opponentPiece == Piece::Rook || opponentPiece == Piece::Queen) {
                                pinned = true;
                                diagonalMovement = false;
                                break;
                            }
                        }
                        break;
                    }
                    x += xDir;
                    y += yDir;
                }
            }
        }
    }

    if (!pinned) {
        int forwardDirection = (color == PieceColor::White) ? -1 : 1;
        int startingRow = (color == PieceColor::White) ? 8 - 2 : 1;

        int forwardOneX = currentPos.x + forwardDirection;
        int forwardTwoX = forwardOneX + forwardDirection;

        if (!hunYouChecked) {
            if (currentPiece->getPawnMoves() == 1) {
                int forwardDirection = (currentPiece->getPieceColor() == PieceColor::White) ? 1 : -1;
                int enPassantX = currentPos.x + forwardDirection;
                int enPassantY = currentPos.y - 1;

                if (enPassantX >= 0 && enPassantX < 8 && enPassantY >= 0 && enPassantY < 8) {
                    if (board[enPassantX][enPassantY] != nullptr &&
                        board[enPassantX][enPassantY]->getPiece() == Piece::Pawn &&
                        board[enPassantX][enPassantY]->getPieceColor() != currentPiece->getPieceColor()) {
                        legalMoves.push_back(Vector2i(enPassantX, enPassantY));
                    }
                }
            }


            if (forwardOneX >= 0 && forwardOneX < 8 && board[forwardOneX][currentPos.y] == nullptr) {
                legalMoves.push_back(Vector2i(forwardOneX, currentPos.y));

                if (currentPos.x == startingRow && board[forwardTwoX][currentPos.y] == nullptr) {
                    legalMoves.push_back(Vector2i(forwardTwoX, currentPos.y));
                }
            }

            int leftDiagonalX = forwardOneX;
            int rightDiagonalX = forwardOneX;
            int captureY = currentPos.y - 1;

            if (leftDiagonalX >= 0 && leftDiagonalX < 8) {
                if (captureY >= 0 && captureY < 8) {
                    if (board[leftDiagonalX][captureY] != nullptr) {
                        if (board[leftDiagonalX][captureY]->getPieceColor() != color) {
                            legalMoves.push_back(Vector2i(leftDiagonalX, captureY));
                        }
                    }
                }
            }

            captureY = currentPos.y + 1;
            if (rightDiagonalX >= 0 && rightDiagonalX < 8) {
                if (captureY >= 0 && captureY < 8) {
                    if (board[rightDiagonalX][captureY] != nullptr) {
                        if (board[rightDiagonalX][captureY]->getPieceColor() != color) {
                            legalMoves.push_back(Vector2i(rightDiagonalX, captureY));
                        }
                    }
                }
            }
        }
        else {
            Vector2i kingPos;
            std::vector<Vector2i> attackedSquares;

            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (board[i][j] != nullptr && board[i][j]->getPiece() == Piece::King && board[i][j]->getPieceColor() == color) {
                        kingPos = Vector2i(i, j);
                    }
                }
            }

            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (board[i][j] != nullptr && board[i][j]->getPieceColor() != color) {
                        std::vector<Vector2i> opponentMoves = board[i][j]->getLegalMoves(Vector2i(i, j), board);

                        if (board[i][j]->getPiece() == Piece::Knight) {
                            for (const auto& move : opponentMoves) {
                                if (move == kingPos) {
                                    attackedSquares.push_back(Vector2i(i, j));
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (board[i][j] != nullptr && board[i][j]->getPieceColor() != color && board[i][j]->getPiece() != Piece::Knight) {
                        std::vector<Vector2i> opponentMoves = board[i][j]->getLegalMoves(Vector2i(i, j), board);

                        for (const auto& move : opponentMoves) {
                            if (move == kingPos) {
                                attackedSquares.push_back(Vector2i(i, j));

                                int dx = kingPos.x - i;
                                int dy = kingPos.y - j;
                                int xDir = (dx == 0) ? 0 : ((dx > 0) ? 1 : -1);
                                int yDir = (dy == 0) ? 0 : ((dy > 0) ? 1 : -1);

                                int x = i + xDir;
                                int y = j + yDir;
                                while (x != kingPos.x || y != kingPos.y) {
                                    attackedSquares.push_back(Vector2i(x, y));
                                    x += xDir;
                                    y += yDir;
                                }
                                attackedSquares.push_back(kingPos);

                                break;
                            }
                        }
                    }
                }
            }
            int forwardDirection = (color == PieceColor::White) ? -1 : 1;
            int forwardX = currentPos.x + forwardDirection;
            int leftDiagonalX = forwardX;
            int rightDiagonalX = forwardX;
            int leftDiagonalY = currentPos.y - 1;
            int rightDiagonalY = currentPos.y + 1;

            if (forwardX >= 0 && forwardX < 8 && board[forwardX][currentPos.y] == nullptr) {
                if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(forwardX, currentPos.y)) != attackedSquares.end()) {
                    legalMoves.push_back(Vector2i(forwardX, currentPos.y));
                }
            }

            if (leftDiagonalX >= 0 && leftDiagonalX < 8 && leftDiagonalY >= 0 && leftDiagonalY < 8) {
                if (board[leftDiagonalX][leftDiagonalY] != nullptr && board[leftDiagonalX][leftDiagonalY]->getPieceColor() != color) {
                    if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(leftDiagonalX, leftDiagonalY)) != attackedSquares.end()) {
                        legalMoves.push_back(Vector2i(leftDiagonalX, leftDiagonalY));
                    }
                }
            }

            if (rightDiagonalX >= 0 && rightDiagonalX < 8 && rightDiagonalY >= 0 && rightDiagonalY < 8) {
                if (board[rightDiagonalX][rightDiagonalY] != nullptr && board[rightDiagonalX][rightDiagonalY]->getPieceColor() != color) {
                    if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(rightDiagonalX, rightDiagonalY)) != attackedSquares.end()) {
                        legalMoves.push_back(Vector2i(rightDiagonalX, rightDiagonalY));
                    }
                }
            }
        }
    }
    else {
        if (diagonalMovement) {
            if (board[currentPos.x + xDir][currentPos.y + yDir] != nullptr &&
                board[currentPos.x + xDir][currentPos.y + yDir]->getPieceColor() != color) {
                legalMoves.push_back(Vector2i(currentPos.x + xDir, currentPos.y + yDir));
            }
        }
        else {
            if (board[currentPos.x + xDir][currentPos.y] == nullptr) {
                legalMoves.push_back(Vector2i(currentPos.x + xDir, currentPos.y));
                if (currentPos.x == (color == PieceColor::White ? 6 : 1) && board[currentPos.x + 2 * xDir][currentPos.y] == nullptr) {
                    legalMoves.push_back(Vector2i(currentPos.x + 2 * xDir, currentPos.y));
                }
            }
        }
        return legalMoves;
    }

    return legalMoves;
}

//Knight
Knight::Knight(const Texture& texture, PieceColor pieceColor) {
    color = pieceColor;
    sprite.setTexture(texture);
    sprite.setScale(scale, scale);
}
PieceColor Knight::getPieceColor() { return color; }
Piece Knight::getPiece() { return Piece::Knight; }
void Knight::setPinned(bool pin) { pinned = pin; }
void Knight::draw(RenderWindow& window) { window.draw(sprite); }
void Knight::setPosition(int i, int j) { sprite.setPosition(j * 80, i * 80); }
void Knight::moveCounter() { moves++; }
int Knight::getMoves() { return moves; }
int Knight::getKnightMoves() { return moves; }
vector<Vector2i> Knight::getMovesForCheck(Vector2i currentPos, ChessBoard* board[][8]) {
    std::vector<Vector2i> invalidateKingMoves;

    int knightMoves[][2] = { {-2, -1}, {-2, 1}, {2, -1}, {2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2} };

    for (int i = 0; i < 8; ++i) {
        int dx = knightMoves[i][0];
        int dy = knightMoves[i][1];

        int newX = currentPos.x + dx;
        int newY = currentPos.y + dy;

        if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
            if (board[newX][newY] == nullptr || board[newX][newY]->getPieceColor() == color) {
                invalidateKingMoves.push_back(Vector2i(newX, newY));
            }

        }
    }
    return invalidateKingMoves;
}
vector<Vector2i> Knight::getLegalMoves(Vector2i currentPos, ChessBoard* board[][8]) {
    std::vector<Vector2i> legalMoves;
    int dx[8] = { 1, 1, 2, 2, -1, -1, -2, -2 };
    int dy[8] = { 2, -2, 1, -1, 2, -2, 1, -1 };

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] != nullptr) {
                board[i][j]->setPinned(false);
            }
        }
    }

    Vector2i kingPos;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] != nullptr && board[i][j]->getPiece() == Piece::King && board[i][j]->getPieceColor() == color) {
                kingPos = Vector2i(i, j);
                break;
            }
        }
    }

    int towardsX = kingPos.x - currentPos.x;
    int towardsY = kingPos.y - currentPos.y;
    int xDir = (towardsX == 0) ? 0 : ((towardsX > 0) ? -1 : 1);
    int yDir = (towardsY == 0) ? 0 : ((towardsY > 0) ? -1 : 1);
    bool diagonalMovement = (abs(towardsX) == abs(towardsY));

    bool isClear = true;

    int xDirToKing = (kingPos.x > currentPos.x) ? 1 : (kingPos.x < currentPos.x) ? -1 : 0;
    int yDirToKing = (kingPos.y > currentPos.y) ? 1 : (kingPos.y < currentPos.y) ? -1 : 0;

    for (int x = currentPos.x + xDirToKing, y = currentPos.y + yDirToKing; x != kingPos.x || y != kingPos.y; x += xDirToKing, y += yDirToKing) {
        if (board[x][y] != nullptr) {
            isClear = false;
            break;
        }
    }

    if (isClear) {
        if (towardsX == 0 || towardsY == 0 || diagonalMovement) {
            int x = currentPos.x + xDir;
            int y = currentPos.y + yDir;

            if (diagonalMovement) {
                while (x >= 0 && x < 8 && y >= 0 && y < 8) { // Check within board bounds
                    if (board[x][y] != nullptr) {
                        if (board[x][y]->getPiece() != Piece::King && board[x][y]->getPieceColor() != color) {
                            Piece opponentPiece = board[x][y]->getPiece();
                            if (opponentPiece == Piece::Bishop || opponentPiece == Piece::Queen) {
                                pinned = true;
                                break;
                            }
                        }
                        break;
                    }
                    x += xDir;
                    y += yDir;
                }
            }
            else {
                int x = currentPos.x + xDir;
                int y = currentPos.y + yDir;

                while (x >= 0 && x < 8 && y >= 0 && y < 8) { // Check within board bounds
                    if (board[x][y] != nullptr) {
                        if (board[x][y]->getPieceColor() != color) {
                            Piece opponentPiece = board[x][y]->getPiece();
                            if (opponentPiece == Piece::Rook || opponentPiece == Piece::Queen) {
                                pinned = true;
                                diagonalMovement = false;
                                break;
                            }
                        }
                        break;
                    }
                    x += xDir;
                    y += yDir;
                }
            }
        }
    }

    if (!pinned) {
        if (!hunYouChecked) {
            for (int i = 0; i < 8; ++i) {
                int newX = currentPos.x + dx[i];
                int newY = currentPos.y + dy[i];
                if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
                    if (board[newX][newY] == nullptr || board[newX][newY]->getPieceColor() != color) {
                        legalMoves.push_back(Vector2i(newX, newY));
                    }
                }
            }
        }
        else {
            Vector2i kingPos;
            std::vector<Vector2i> attackedSquares;

            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (board[i][j] != nullptr && board[i][j]->getPiece() == Piece::King && board[i][j]->getPieceColor() == color) {
                        kingPos = Vector2i(i, j);
                    }
                }
            }

            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (board[i][j] != nullptr && board[i][j]->getPieceColor() != color) {
                        std::vector<Vector2i> opponentMoves = board[i][j]->getLegalMoves(Vector2i(i, j), board);

                        if (board[i][j]->getPiece() == Piece::Knight) {
                            for (const auto& move : opponentMoves) {
                                if (move == kingPos) {
                                    attackedSquares.push_back(Vector2i(i, j));
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (board[i][j] != nullptr && board[i][j]->getPieceColor() != color && board[i][j]->getPiece() != Piece::Knight) {
                        std::vector<Vector2i> opponentMoves = board[i][j]->getLegalMoves(Vector2i(i, j), board);

                        for (const auto& move : opponentMoves) {
                            if (move == kingPos) {
                                attackedSquares.push_back(Vector2i(i, j));

                                int dx = kingPos.x - i;
                                int dy = kingPos.y - j;
                                int xDir = (dx == 0) ? 0 : ((dx > 0) ? 1 : -1);
                                int yDir = (dy == 0) ? 0 : ((dy > 0) ? 1 : -1);

                                int x = i + xDir;
                                int y = j + yDir;
                                while (x != kingPos.x || y != kingPos.y) {
                                    attackedSquares.push_back(Vector2i(x, y));
                                    x += xDir;
                                    y += yDir;
                                }
                                attackedSquares.push_back(kingPos);

                                break;
                            }
                        }
                    }
                }
            }


            for (int i = 0; i < 8; ++i) {
                int newX = currentPos.x + dx[i];
                int newY = currentPos.y + dy[i];
                if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
                    if (board[newX][newY] == nullptr || board[newX][newY]->getPieceColor() != color) {
                        if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(newX, newY)) != attackedSquares.end())
                            legalMoves.push_back(Vector2i(newX, newY));
                    }
                }
            }

        }
    }

    return legalMoves;
}

//Rook
Rook::Rook(const Texture& texture, PieceColor pieceColor) {
    color = pieceColor;
    sprite.setTexture(texture);
    sprite.setScale(scale, scale);
}
PieceColor Rook::getPieceColor() { return color; }
Piece Rook::getPiece() { return Piece::Rook; }
void Rook::setPinned(bool pin) { pinned = pin; }
void Rook::draw(RenderWindow& window) { window.draw(sprite); }
void Rook::setPosition(int i, int j) { sprite.setPosition(j * 80, i * 80); }
void Rook::moveCounter() { moves++; }
int Rook::getMoves() { return moves; }
int Rook::getRookMoves() { return moves; }
vector<Vector2i> Rook::getMovesForCheck(Vector2i currentPos, ChessBoard* board[][8]) {
    std::vector<Vector2i> invalidateKingMoves;

    for (int i = currentPos.x - 1; i >= 0; --i) {
        if (board[i][currentPos.y] == nullptr) {
            invalidateKingMoves.push_back(Vector2i(i, currentPos.y));
        }
        else {
            if (board[i][currentPos.y]->getPieceColor() != color && board[i][currentPos.y]->getPiece() == Piece::King) {
                invalidateKingMoves.push_back(Vector2i(i - 1, currentPos.y));
            }
            else if (board[i][currentPos.y]->getPieceColor() == color) {
                invalidateKingMoves.push_back(Vector2i(i, currentPos.y));
            }
            break;
        }
    }
    for (int i = currentPos.x + 1; i < 8; ++i) {
        if (board[i][currentPos.y] == nullptr) {
            invalidateKingMoves.push_back(Vector2i(i, currentPos.y));
        }
        else {
            if (board[i][currentPos.y]->getPieceColor() != color && board[i][currentPos.y]->getPiece() == Piece::King) {
                invalidateKingMoves.push_back(Vector2i(i + 1, currentPos.y));
            }
            else if (board[i][currentPos.y]->getPieceColor() == color) {
                invalidateKingMoves.push_back(Vector2i(i, currentPos.y));
            }
            break;
        }
    }
    for (int j = currentPos.y - 1; j >= 0; --j) {
        if (board[currentPos.x][j] == nullptr) {
            invalidateKingMoves.push_back(Vector2i(currentPos.x, j));
        }
        else {
            if (board[currentPos.x][j]->getPieceColor() != color && board[currentPos.x][j]->getPiece() == Piece::King) {
                invalidateKingMoves.push_back(Vector2i(currentPos.x, j - 1));
            }
            else if (board[currentPos.x][j]->getPieceColor() == color) {
                invalidateKingMoves.push_back(Vector2i(currentPos.x, j));
            }
            break;
        }
    }
    for (int j = currentPos.y + 1; j < 8; ++j) {
        if (board[currentPos.x][j] == nullptr) {
            invalidateKingMoves.push_back(Vector2i(currentPos.x, j));
        }
        else {
            if (board[currentPos.x][j]->getPieceColor() != color && board[currentPos.x][j]->getPiece() == Piece::King) {
                invalidateKingMoves.push_back(Vector2i(currentPos.x, j + 1));
            }
            else if (board[currentPos.x][j]->getPieceColor() == color) {
                invalidateKingMoves.push_back(Vector2i(currentPos.x, j));
            }
            break;
        }
    }

    return invalidateKingMoves;
}
vector<Vector2i> Rook::getLegalMoves(Vector2i currentPos, ChessBoard* board[][8]) {
    std::vector<Vector2i> legalMoves;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] != nullptr) {
                board[i][j]->setPinned(false);
            }
        }
    }

    Vector2i kingPos;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] != nullptr && board[i][j]->getPiece() == Piece::King && board[i][j]->getPieceColor() == color) {
                kingPos = Vector2i(i, j);
                break;
            }
        }
    }
    int towardsX = kingPos.x - currentPos.x;
    int towardsY = kingPos.y - currentPos.y;
    int xDir = (towardsX == 0) ? 0 : ((towardsX > 0) ? -1 : 1);
    int yDir = (towardsY == 0) ? 0 : ((towardsY > 0) ? -1 : 1);
    bool diagonalMovement = (abs(towardsX) == abs(towardsY));

    bool isClear = true;

    int xDirToKing = (kingPos.x > currentPos.x) ? 1 : (kingPos.x < currentPos.x) ? -1 : 0;
    int yDirToKing = (kingPos.y > currentPos.y) ? 1 : (kingPos.y < currentPos.y) ? -1 : 0;

    for (int x = currentPos.x + xDirToKing, y = currentPos.y + yDirToKing; x != kingPos.x || y != kingPos.y; x += xDirToKing, y += yDirToKing) {
        if (board[x][y] != nullptr) {
            isClear = false;
            break;
        }
    }

    if (isClear) {
        if (towardsX == 0 || towardsY == 0 || diagonalMovement) {
            int x = currentPos.x + xDir;
            int y = currentPos.y + yDir;

            if (diagonalMovement) {
                while (x >= 0 && x < 8 && y >= 0 && y < 8) { // Check within board bounds
                    if (board[x][y] != nullptr) {
                        if (board[x][y]->getPiece() != Piece::King && board[x][y]->getPieceColor() != color) {
                            Piece opponentPiece = board[x][y]->getPiece();
                            if (opponentPiece == Piece::Bishop || opponentPiece == Piece::Queen) {
                                pinned = true;
                                break;
                            }
                        }
                        break;
                    }
                    x += xDir;
                    y += yDir;
                }
            }
            else {
                int x = currentPos.x + xDir;
                int y = currentPos.y + yDir;

                while (x >= 0 && x < 8 && y >= 0 && y < 8) { // Check within board bounds
                    if (board[x][y] != nullptr) {
                        if (board[x][y]->getPieceColor() != color) {
                            Piece opponentPiece = board[x][y]->getPiece();
                            if (opponentPiece == Piece::Rook || opponentPiece == Piece::Queen) {
                                pinned = true;
                                diagonalMovement = false;
                                break;
                            }
                        }
                        break;
                    }
                    x += xDir;
                    y += yDir;
                }
            }
        }
    }

    if (!pinned) {
        if (!hunYouChecked) {
            for (int i = currentPos.x - 1; i >= 0; --i) {
                if (board[i][currentPos.y] == nullptr) {
                    legalMoves.push_back(Vector2i(i, currentPos.y));
                }
                else {
                    if (board[i][currentPos.y]->getPieceColor() != color) {
                        legalMoves.push_back(Vector2i(i, currentPos.y));
                    }
                    break;
                }
            }
            for (int i = currentPos.x + 1; i < 8; ++i) {
                if (board[i][currentPos.y] == nullptr) {
                    legalMoves.push_back(Vector2i(i, currentPos.y));
                }
                else {
                    if (board[i][currentPos.y]->getPieceColor() != color) {
                        legalMoves.push_back(Vector2i(i, currentPos.y));
                    }
                    break;
                }
            }
            for (int j = currentPos.y - 1; j >= 0; --j) {
                if (board[currentPos.x][j] == nullptr) {
                    legalMoves.push_back(Vector2i(currentPos.x, j));
                }
                else {
                    if (board[currentPos.x][j]->getPieceColor() != color) {
                        legalMoves.push_back(Vector2i(currentPos.x, j));
                    }
                    break;
                }
            }
            for (int j = currentPos.y + 1; j < 8; ++j) {
                if (board[currentPos.x][j] == nullptr) {
                    legalMoves.push_back(Vector2i(currentPos.x, j));
                }
                else {
                    if (board[currentPos.x][j]->getPieceColor() != color) {
                        legalMoves.push_back(Vector2i(currentPos.x, j));
                    }
                    break;
                }
            }
        }
        else {
            Vector2i kingPos;
            std::vector<Vector2i> attackedSquares;

            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (board[i][j] != nullptr && board[i][j]->getPiece() == Piece::King && board[i][j]->getPieceColor() == color) {
                        kingPos = Vector2i(i, j);
                    }
                }
            }

            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (board[i][j] != nullptr && board[i][j]->getPieceColor() != color) {
                        std::vector<Vector2i> opponentMoves = board[i][j]->getLegalMoves(Vector2i(i, j), board);

                        if (board[i][j]->getPiece() == Piece::Knight) {
                            for (const auto& move : opponentMoves) {
                                if (move == kingPos) {
                                    attackedSquares.push_back(Vector2i(i, j));
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (board[i][j] != nullptr && board[i][j]->getPieceColor() != color && board[i][j]->getPiece() != Piece::Knight) {
                        std::vector<Vector2i> opponentMoves = board[i][j]->getLegalMoves(Vector2i(i, j), board);

                        for (const auto& move : opponentMoves) {
                            if (move == kingPos) {
                                attackedSquares.push_back(Vector2i(i, j));

                                int dx = kingPos.x - i;
                                int dy = kingPos.y - j;
                                int xDir = (dx == 0) ? 0 : ((dx > 0) ? 1 : -1);
                                int yDir = (dy == 0) ? 0 : ((dy > 0) ? 1 : -1);

                                int x = i + xDir;
                                int y = j + yDir;
                                while (x != kingPos.x || y != kingPos.y) {
                                    attackedSquares.push_back(Vector2i(x, y));
                                    x += xDir;
                                    y += yDir;
                                }
                                attackedSquares.push_back(kingPos);

                                break;
                            }
                        }
                    }
                }
            }


            for (int i = currentPos.x - 1; i >= 0; --i) {
                if (board[i][currentPos.y] == nullptr) {
                    if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(i, currentPos.y)) != attackedSquares.end())
                        legalMoves.push_back(Vector2i(i, currentPos.y));
                }
                else {
                    if (board[i][currentPos.y]->getPieceColor() != color) {
                        if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(i, currentPos.y)) != attackedSquares.end())
                            legalMoves.push_back(Vector2i(i, currentPos.y));
                    }
                    break;
                }
            }
            for (int i = currentPos.x + 1; i < 8; ++i) {
                if (board[i][currentPos.y] == nullptr) {
                    if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(i, currentPos.y)) != attackedSquares.end())
                        legalMoves.push_back(Vector2i(i, currentPos.y));
                }
                else {
                    if (board[i][currentPos.y]->getPieceColor() != color) {
                        if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(i, currentPos.y)) != attackedSquares.end())
                            legalMoves.push_back(Vector2i(i, currentPos.y));
                    }
                    break;
                }
            }
            for (int j = currentPos.y - 1; j >= 0; --j) {
                if (board[currentPos.x][j] == nullptr) {
                    if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(currentPos.x, j)) != attackedSquares.end())
                        legalMoves.push_back(Vector2i(currentPos.x, j));
                }
                else {
                    if (board[currentPos.x][j]->getPieceColor() != color) {
                        if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(currentPos.x, j)) != attackedSquares.end())
                            legalMoves.push_back(Vector2i(currentPos.x, j));
                    }
                    break;
                }
            }
            for (int j = currentPos.y + 1; j < 8; ++j) {
                if (board[currentPos.x][j] == nullptr) {
                    if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(currentPos.x, j)) != attackedSquares.end())
                        legalMoves.push_back(Vector2i(currentPos.x, j));
                }
                else {
                    if (board[currentPos.x][j]->getPieceColor() != color) {
                        if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(currentPos.x, j)) != attackedSquares.end())
                            legalMoves.push_back(Vector2i(currentPos.x, j));
                    }
                    break;
                }
            }
        }
    }
    else {
        if (!hunYouChecked) {
            if (!diagonalMovement) {
                for (int i = currentPos.x + xDir; i >= 0 && i < 8; i += xDir) {
                    // Check if there's a piece at the new position
                    if (board[i][currentPos.y] == nullptr) {
                        legalMoves.push_back(Vector2i(i, currentPos.y)); // Add legal move
                    }
                    else {
                        // Stop generating moves if there's a piece of the same color
                        if (board[i][currentPos.y]->getPieceColor() == color) {
                            break;
                        }
                        legalMoves.push_back(Vector2i(i, currentPos.y)); // Add legal move and continue
                        break;
                    }
                }

                // Generate legal moves along the vertical direction in the direction of the pin
                for (int j = currentPos.y + yDir; j >= 0 && j < 8; j += yDir) {
                    // Check if there's a piece at the new position
                    if (board[currentPos.x][j] == nullptr) {
                        legalMoves.push_back(Vector2i(currentPos.x, j)); // Add legal move
                    }
                    else {
                        // Stop generating moves if there's a piece of the same color
                        if (board[currentPos.x][j]->getPieceColor() == color) {
                            break;
                        }
                        legalMoves.push_back(Vector2i(currentPos.x, j)); // Add legal move and continue
                        break;
                    }
                }
            }
        }
    }
    return legalMoves;
}

//Bishop
Bishop::Bishop(const Texture& texture, PieceColor pieceColor) {
    color = pieceColor;
    sprite.setTexture(texture);
    sprite.setScale(scale, scale);
}
PieceColor Bishop::getPieceColor() { return color; }
Piece Bishop::getPiece() { return Piece::Bishop; }
void Bishop::setPinned(bool pin) { pinned = pin; }
void Bishop::draw(RenderWindow& window) { window.draw(sprite); }
void Bishop::setPosition(int i, int j) { sprite.setPosition(j * 80, i * 80); }
void Bishop::moveCounter() { moves++; }
int Bishop::getMoves() { return moves; }
int Bishop::getBishopMoves() { return moves; }
vector<Vector2i> Bishop::getMovesForCheck(Vector2i currentPos, ChessBoard* board[][8]) {
    std::vector<Vector2i> invalidateKingMovement;
    for (int i = currentPos.x + 1, j = currentPos.y + 1; i < 8 && j < 8; ++i, ++j) {
        if (board[i][j] == nullptr) {
            invalidateKingMovement.push_back(Vector2i(i, j));
        }
        else {
            if (board[i][j]->getPieceColor() != color && board[i][j]->getPiece() == Piece::King) {
                invalidateKingMovement.push_back(Vector2i(i + 1, j + 1));
            }
            else if (board[i][j]->getPieceColor() == color) {
                invalidateKingMovement.push_back(Vector2i(i, j));
            }
            break;
        }
    }
    for (int i = currentPos.x - 1, j = currentPos.y + 1; i >= 0 && j < 8; --i, ++j) {
        if (board[i][j] == nullptr) {
            invalidateKingMovement.push_back(Vector2i(i, j));
        }
        else {
            if (board[i][j]->getPieceColor() != color && board[i][j]->getPiece() == Piece::King) {
                invalidateKingMovement.push_back(Vector2i(i - 1, j + 1));
            }
            else if (board[i][j]->getPieceColor() == color) {
                invalidateKingMovement.push_back(Vector2i(i, j));
            }
            break;
        }
    }
    for (int i = currentPos.x + 1, j = currentPos.y - 1; i < 8 && j >= 0; ++i, --j) {
        if (board[i][j] == nullptr) {
            invalidateKingMovement.push_back(Vector2i(i, j));
        }
        else {
            if (board[i][j]->getPieceColor() != color && board[i][j]->getPiece() == Piece::King) {
                invalidateKingMovement.push_back(Vector2i(i + 1, j - 1));
            }
            else if (board[i][j]->getPieceColor() == color) {
                invalidateKingMovement.push_back(Vector2i(i, j));
            }
            break;
        }
    }
    for (int i = currentPos.x - 1, j = currentPos.y - 1; i >= 0 && j >= 0; --i, --j) {
        if (board[i][j] == nullptr) {
            invalidateKingMovement.push_back(Vector2i(i, j));
        }
        else {
            if (board[i][j]->getPieceColor() != color && board[i][j]->getPiece() == Piece::King) {
                invalidateKingMovement.push_back(Vector2i(i - 1, j - 1));
            }
            else if (board[i][j]->getPieceColor() == color) {
                invalidateKingMovement.push_back(Vector2i(i, j));
            }
            break;
        }
    }

    return invalidateKingMovement;
}
vector<Vector2i> Bishop::getLegalMoves(Vector2i currentPos, ChessBoard* board[][8]) {
    std::vector<Vector2i> legalMoves;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] != nullptr) {
                board[i][j]->setPinned(false);
            }
        }
    }

    Vector2i kingPos;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] != nullptr && board[i][j]->getPiece() == Piece::King && board[i][j]->getPieceColor() == color) {
                kingPos = Vector2i(i, j);
                break;
            }
        }
    }

    int towardsX = kingPos.x - currentPos.x;
    int towardsY = kingPos.y - currentPos.y;
    int xDir = (towardsX == 0) ? 0 : ((towardsX > 0) ? -1 : 1);
    int yDir = (towardsY == 0) ? 0 : ((towardsY > 0) ? -1 : 1);
    bool diagonalMovement = (abs(towardsX) == abs(towardsY));

    bool isClear = true;

    int xDirToKing = (kingPos.x > currentPos.x) ? 1 : (kingPos.x < currentPos.x) ? -1 : 0;
    int yDirToKing = (kingPos.y > currentPos.y) ? 1 : (kingPos.y < currentPos.y) ? -1 : 0;

    for (int x = currentPos.x + xDirToKing, y = currentPos.y + yDirToKing; x != kingPos.x || y != kingPos.y; x += xDirToKing, y += yDirToKing) {
        if (board[x][y] != nullptr) {
            isClear = false;
            break;
        }
    }

    if (isClear) {
        if (towardsX == 0 || towardsY == 0 || diagonalMovement) {
            int x = currentPos.x + xDir;
            int y = currentPos.y + yDir;

            if (diagonalMovement) {
                while (x >= 0 && x < 8 && y >= 0 && y < 8) { // Check within board bounds
                    if (board[x][y] != nullptr) {
                        if (board[x][y]->getPiece() != Piece::King && board[x][y]->getPieceColor() != color) {
                            Piece opponentPiece = board[x][y]->getPiece();
                            if (opponentPiece == Piece::Bishop || opponentPiece == Piece::Queen) {
                                pinned = true;
                                break;
                            }
                        }
                        break;
                    }
                    x += xDir;
                    y += yDir;
                }
            }
            else {
                int x = currentPos.x + xDir;
                int y = currentPos.y + yDir;

                while (x >= 0 && x < 8 && y >= 0 && y < 8) { // Check within board bounds
                    if (board[x][y] != nullptr) {
                        if (board[x][y]->getPieceColor() != color) {
                            Piece opponentPiece = board[x][y]->getPiece();
                            if (opponentPiece == Piece::Rook || opponentPiece == Piece::Queen) {
                                pinned = true;
                                diagonalMovement = false;
                                break;
                            }
                        }
                        break;
                    }
                    x += xDir;
                    y += yDir;
                }
            }
        }
    }

    if (!pinned) {
        if (!hunYouChecked) {
            for (int i = currentPos.x + 1, j = currentPos.y + 1; i < 8 && j < 8; ++i, ++j) {
                if (board[i][j] == nullptr) {
                    legalMoves.push_back(Vector2i(i, j));
                }
                else {
                    if (board[i][j]->getPieceColor() != color) {
                        legalMoves.push_back(Vector2i(i, j));
                    }
                    break;
                }
            }
            for (int i = currentPos.x - 1, j = currentPos.y + 1; i >= 0 && j < 8; --i, ++j) {
                if (board[i][j] == nullptr) {
                    legalMoves.push_back(Vector2i(i, j));
                }
                else {
                    if (board[i][j]->getPieceColor() != color) {
                        legalMoves.push_back(Vector2i(i, j));
                    }
                    break;
                }
            }
            for (int i = currentPos.x + 1, j = currentPos.y - 1; i < 8 && j >= 0; ++i, --j) {
                if (board[i][j] == nullptr) {
                    legalMoves.push_back(Vector2i(i, j));
                }
                else {
                    if (board[i][j]->getPieceColor() != color) {
                        legalMoves.push_back(Vector2i(i, j));
                    }
                    break;
                }
            }
            for (int i = currentPos.x - 1, j = currentPos.y - 1; i >= 0 && j >= 0; --i, --j) {
                if (board[i][j] == nullptr) {
                    legalMoves.push_back(Vector2i(i, j));
                }
                else {
                    if (board[i][j]->getPieceColor() != color) {
                        legalMoves.push_back(Vector2i(i, j));
                    }
                    break;
                }
            }
        }
        else {
            Vector2i kingPos;
            std::vector<Vector2i> attackedSquares;

            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (board[i][j] != nullptr && board[i][j]->getPiece() == Piece::King && board[i][j]->getPieceColor() == color) {
                        kingPos = Vector2i(i, j);
                    }
                }
            }

            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (board[i][j] != nullptr && board[i][j]->getPieceColor() != color) {
                        std::vector<Vector2i> opponentMoves = board[i][j]->getLegalMoves(Vector2i(i, j), board);

                        if (board[i][j]->getPiece() == Piece::Knight) {
                            for (const auto& move : opponentMoves) {
                                if (move == kingPos) {
                                    attackedSquares.push_back(Vector2i(i, j));
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (board[i][j] != nullptr && board[i][j]->getPieceColor() != color && board[i][j]->getPiece() != Piece::Knight) {
                        std::vector<Vector2i> opponentMoves = board[i][j]->getLegalMoves(Vector2i(i, j), board);

                        for (const auto& move : opponentMoves) {
                            if (move == kingPos) {
                                attackedSquares.push_back(Vector2i(i, j));

                                int dx = kingPos.x - i;
                                int dy = kingPos.y - j;
                                int xDir = (dx == 0) ? 0 : ((dx > 0) ? 1 : -1);
                                int yDir = (dy == 0) ? 0 : ((dy > 0) ? 1 : -1);

                                int x = i + xDir;
                                int y = j + yDir;
                                while (x != kingPos.x || y != kingPos.y) {
                                    attackedSquares.push_back(Vector2i(x, y));
                                    x += xDir;
                                    y += yDir;
                                }
                                attackedSquares.push_back(kingPos);

                                break;
                            }
                        }
                    }
                }
            }

            for (int i = currentPos.x + 1, j = currentPos.y + 1; i < 8 && j < 8; ++i, ++j) {
                if (board[i][j] == nullptr) {
                    if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(i, j)) != attackedSquares.end()) {
                        legalMoves.push_back(Vector2i(i, j));
                    }
                }
                else {
                    if (board[i][j]->getPieceColor() != color) {
                        if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(i, j)) != attackedSquares.end()) {
                            legalMoves.push_back(Vector2i(i, j));
                        }
                    }
                    break;
                }
            }
            for (int i = currentPos.x - 1, j = currentPos.y + 1; i >= 0 && j < 8; --i, ++j) {
                if (board[i][j] == nullptr) {
                    if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(i, j)) != attackedSquares.end()) {
                        legalMoves.push_back(Vector2i(i, j));
                    }
                }
                else {
                    if (board[i][j]->getPieceColor() != color) {
                        if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(i, j)) != attackedSquares.end()) {
                            legalMoves.push_back(Vector2i(i, j));
                        }
                    }
                    break;
                }
            }
            for (int i = currentPos.x + 1, j = currentPos.y - 1; i < 8 && j >= 0; ++i, --j) {
                if (board[i][j] == nullptr) {
                    if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(i, j)) != attackedSquares.end())
                        legalMoves.push_back(Vector2i(i, j));
                }
                else {
                    if (board[i][j]->getPieceColor() != color) {
                        if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(i, j)) != attackedSquares.end())
                            legalMoves.push_back(Vector2i(i, j));
                    }
                    break;
                }
            }
            for (int i = currentPos.x - 1, j = currentPos.y - 1; i >= 0 && j >= 0; --i, --j) {
                if (board[i][j] == nullptr) {
                    if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(i, j)) != attackedSquares.end())
                        legalMoves.push_back(Vector2i(i, j));
                }
                else {
                    if (board[i][j]->getPieceColor() != color) {
                        if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(i, j)) != attackedSquares.end())
                            legalMoves.push_back(Vector2i(i, j));
                    }
                    break;
                }
            }
        }
    }
    else {
        if (!hunYouChecked) {
            if (diagonalMovement) {
                // Generate legal moves along the diagonal in the direction of the pin
                for (int i = 1; i < 8; ++i) {
                    int x1 = currentPos.x + i * xDir;
                    int y1 = currentPos.y + i * yDir;

                    // Check if the new position is within the board bounds
                    if (x1 < 0 || x1 >= 8 || y1 < 0 || y1 >= 8) {
                        break; // Stop if the position is out of bounds
                    }

                    // Check if there's a piece at the new position
                    if (board[x1][y1] == nullptr) {
                        legalMoves.push_back(Vector2i(x1, y1)); // Add legal move
                    }
                    else {
                        // Stop generating moves if there's a piece of the same color
                        if (board[x1][y1]->getPieceColor() == color) {
                            break;
                        }
                        legalMoves.push_back(Vector2i(x1, y1)); // Add legal move and continue
                        break; // Stop after adding the first diagonal move
                    }
                }

                // Generate legal moves along the diagonal in the opposite direction of the pin
                for (int i = 1; i < 8; ++i) {
                    int x1 = currentPos.x - i * xDir;
                    int y1 = currentPos.y - i * yDir;

                    // Check if the new position is within the board bounds
                    if (x1 < 0 || x1 >= 8 || y1 < 0 || y1 >= 8) {
                        break; // Stop if the position is out of bounds
                    }

                    // Check if there's a piece at the new position
                    if (board[x1][y1] == nullptr) {
                        legalMoves.push_back(Vector2i(x1, y1)); // Add legal move
                    }
                    else {
                        // Stop generating moves if there's a piece of the same color
                        if (board[x1][y1]->getPieceColor() == color) {
                            break;
                        }
                        legalMoves.push_back(Vector2i(x1, y1)); // Add legal move and continue
                        break; // Stop after adding the first diagonal move
                    }
                }
            }
        }
    }

    return legalMoves;
}

//Queen
Queen::Queen(const Texture& texture, PieceColor pieceColor) {
    color = pieceColor;
    sprite.setTexture(texture);
    sprite.setScale(scale, scale);
}
PieceColor Queen::getPieceColor() { return color; }
Piece Queen::getPiece() { return Piece::Queen; }
void Queen::setPinned(bool pin) { pinned = pin; }
void Queen::draw(RenderWindow& window) { window.draw(sprite); }
void Queen::setPosition(int i, int j) { sprite.setPosition(j * 80, i * 80); }
void Queen::moveCounter() { moves++; }
int Queen::getMoves() { return moves; }
int Queen::getQueenMoves() { return moves; }
vector<Vector2i> Queen::getMovesForCheck(Vector2i currentPos, ChessBoard* board[][8]) {
    std::vector<Vector2i> invalidateKingMovement;
    //Rook
    for (int i = currentPos.x - 1; i >= 0; --i) {
        if (board[i][currentPos.y] == nullptr) {
            invalidateKingMovement.push_back(Vector2i(i, currentPos.y));
        }
        else {
            if (board[i][currentPos.y]->getPieceColor() != color && board[i][currentPos.y]->getPiece() == Piece::King) {
                invalidateKingMovement.push_back(Vector2i(i - 1, currentPos.y));
            }
            else if (board[i][currentPos.y]->getPieceColor() == color) {
                invalidateKingMovement.push_back(Vector2i(i, currentPos.y));
            }
            break;
        }
    }
    for (int i = currentPos.x + 1; i < 8; ++i) {
        if (board[i][currentPos.y] == nullptr) {
            invalidateKingMovement.push_back(Vector2i(i, currentPos.y));
        }
        else {
            if (board[i][currentPos.y]->getPieceColor() != color && board[i][currentPos.y]->getPiece() == Piece::King) {
                invalidateKingMovement.push_back(Vector2i(i + 1, currentPos.y));
            }
            else if (board[i][currentPos.y]->getPieceColor() == color) {
                invalidateKingMovement.push_back(Vector2i(i, currentPos.y));
            }
            break;
        }
    }
    for (int j = currentPos.y - 1; j >= 0; --j) {
        if (board[currentPos.x][j] == nullptr) {
            invalidateKingMovement.push_back(Vector2i(currentPos.x, j));
        }
        else {
            if (board[currentPos.x][j]->getPieceColor() != color && board[currentPos.x][j]->getPiece() == Piece::King) {
                invalidateKingMovement.push_back(Vector2i(currentPos.x, j - 1));
            }
            else if (board[currentPos.x][j]->getPieceColor() == color) {
                invalidateKingMovement.push_back(Vector2i(currentPos.x, j));
            }
            break;
        }
    }
    for (int j = currentPos.y + 1; j < 8; ++j) {
        if (board[currentPos.x][j] == nullptr) {
            invalidateKingMovement.push_back(Vector2i(currentPos.x, j));
        }
        else {
            if (board[currentPos.x][j]->getPieceColor() != color && board[currentPos.x][j]->getPiece() == Piece::King) {
                invalidateKingMovement.push_back(Vector2i(currentPos.x, j + 1));
            }
            else if (board[currentPos.x][j]->getPieceColor() == color) {
                invalidateKingMovement.push_back(Vector2i(currentPos.x, j));
            }
            break;
        }
    }

    //Bishop
    for (int i = currentPos.x + 1, j = currentPos.y + 1; i < 8 && j < 8; ++i, ++j) {
        if (board[i][j] == nullptr) {
            invalidateKingMovement.push_back(Vector2i(i, j));
        }
        else {
            if (board[i][j]->getPieceColor() != color && board[i][j]->getPiece() == Piece::King) {
                invalidateKingMovement.push_back(Vector2i(i + 1, j + 1));
            }
            else if (board[i][j]->getPieceColor() == color) {
                invalidateKingMovement.push_back(Vector2i(i, j));
            }
            break;
        }
    }
    for (int i = currentPos.x - 1, j = currentPos.y + 1; i >= 0 && j < 8; --i, ++j) {
        if (board[i][j] == nullptr) {
            invalidateKingMovement.push_back(Vector2i(i, j));
        }
        else {
            if (board[i][j]->getPieceColor() != color && board[i][j]->getPiece() == Piece::King) {
                invalidateKingMovement.push_back(Vector2i(i - 1, j + 1));
            }
            else if (board[i][j]->getPieceColor() == color) {
                invalidateKingMovement.push_back(Vector2i(i, j));
            }
            break;
        }
    }
    for (int i = currentPos.x + 1, j = currentPos.y - 1; i < 8 && j >= 0; ++i, --j) {
        if (board[i][j] == nullptr) {
            invalidateKingMovement.push_back(Vector2i(i, j));
        }
        else {
            if (board[i][j]->getPieceColor() != color && board[i][j]->getPiece() == Piece::King) {
                invalidateKingMovement.push_back(Vector2i(i + 1, j - 1));
            }
            else if (board[i][j]->getPieceColor() == color) {
                invalidateKingMovement.push_back(Vector2i(i, j));
            }
            break;
        }
    }
    for (int i = currentPos.x - 1, j = currentPos.y - 1; i >= 0 && j >= 0; --i, --j) {
        if (board[i][j] == nullptr) {
            invalidateKingMovement.push_back(Vector2i(i, j));
        }
        else {
            if (board[i][j]->getPieceColor() != color && board[i][j]->getPiece() == Piece::King) {
                invalidateKingMovement.push_back(Vector2i(i - 1, j - 1));
            }
            else if (board[i][j]->getPieceColor() == color) {
                invalidateKingMovement.push_back(Vector2i(i, j));
            }
            break;
        }
    }


    return invalidateKingMovement;
}
vector<Vector2i> Queen::getLegalMoves(Vector2i currentPos, ChessBoard* board[][8]) {
    std::vector<Vector2i> legalMoves;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] != nullptr) {
                board[i][j]->setPinned(false);
            }
        }
    }

    Vector2i kingPos;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] != nullptr && board[i][j]->getPiece() == Piece::King && board[i][j]->getPieceColor() == color) {
                kingPos = Vector2i(i, j);
                break;
            }
        }
    }
    int towardsX = kingPos.x - currentPos.x;
    int towardsY = kingPos.y - currentPos.y;
    int xDir = (towardsX == 0) ? 0 : ((towardsX > 0) ? -1 : 1);
    int yDir = (towardsY == 0) ? 0 : ((towardsY > 0) ? -1 : 1);
    bool diagonalMovement = (abs(towardsX) == abs(towardsY));

    bool isClear = true;

    int xDirToKing = (kingPos.x > currentPos.x) ? 1 : (kingPos.x < currentPos.x) ? -1 : 0;
    int yDirToKing = (kingPos.y > currentPos.y) ? 1 : (kingPos.y < currentPos.y) ? -1 : 0;

    for (int x = currentPos.x + xDirToKing, y = currentPos.y + yDirToKing; x != kingPos.x || y != kingPos.y; x += xDirToKing, y += yDirToKing) {
        if (board[x][y] != nullptr) {
            isClear = false;
            break;
        }
    }

    if (isClear) {
        if (towardsX == 0 || towardsY == 0 || diagonalMovement) {
            int x = currentPos.x + xDir;
            int y = currentPos.y + yDir;

            if (diagonalMovement) {
                while (x >= 0 && x < 8 && y >= 0 && y < 8) { // Check within board bounds
                    if (board[x][y] != nullptr) {
                        if (board[x][y]->getPiece() != Piece::King && board[x][y]->getPieceColor() != color) {
                            Piece opponentPiece = board[x][y]->getPiece();
                            if (opponentPiece == Piece::Bishop || opponentPiece == Piece::Queen) {
                                pinned = true;
                                break;
                            }
                        }
                        break;
                    }
                    x += xDir;
                    y += yDir;
                }
            }
            else {
                int x = currentPos.x + xDir;
                int y = currentPos.y + yDir;

                while (x >= 0 && x < 8 && y >= 0 && y < 8) { // Check within board bounds
                    if (board[x][y] != nullptr) {
                        if (board[x][y]->getPieceColor() != color) {
                            Piece opponentPiece = board[x][y]->getPiece();
                            if (opponentPiece == Piece::Rook || opponentPiece == Piece::Queen) {
                                pinned = true;
                                diagonalMovement = false;
                                break;
                            }
                        }
                        break;
                    }
                    x += xDir;
                    y += yDir;
                }
            }
        }
    }

    if (!pinned) {
        if (!hunYouChecked) {
            //Rook Move
            for (int i = currentPos.x - 1; i >= 0; --i) {
                if (board[i][currentPos.y] == nullptr) {
                    legalMoves.push_back(Vector2i(i, currentPos.y));
                }
                else {
                    if (board[i][currentPos.y]->getPieceColor() != color) {
                        legalMoves.push_back(Vector2i(i, currentPos.y));
                    }
                    break;
                }
            }
            for (int i = currentPos.x + 1; i < 8; ++i) {
                if (board[i][currentPos.y] == nullptr) {
                    legalMoves.push_back(Vector2i(i, currentPos.y));
                }
                else {
                    if (board[i][currentPos.y]->getPieceColor() != color) {
                        legalMoves.push_back(Vector2i(i, currentPos.y));
                    }
                    break;
                }
            }
            for (int j = currentPos.y - 1; j >= 0; --j) {
                if (board[currentPos.x][j] == nullptr) {
                    legalMoves.push_back(Vector2i(currentPos.x, j));
                }
                else {
                    if (board[currentPos.x][j]->getPieceColor() != color) {
                        legalMoves.push_back(Vector2i(currentPos.x, j));
                    }
                    break;
                }
            }
            for (int j = currentPos.y + 1; j < 8; ++j) {
                if (board[currentPos.x][j] == nullptr) {
                    legalMoves.push_back(Vector2i(currentPos.x, j));
                }
                else {
                    if (board[currentPos.x][j]->getPieceColor() != color) {
                        legalMoves.push_back(Vector2i(currentPos.x, j));
                    }
                    break;
                }
            }

            //Bishop Move
            for (int i = currentPos.x + 1, j = currentPos.y + 1; i < 8 && j < 8; ++i, ++j) {
                if (board[i][j] == nullptr) {
                    legalMoves.push_back(Vector2i(i, j));
                }
                else {
                    if (board[i][j]->getPieceColor() != color) {
                        legalMoves.push_back(Vector2i(i, j));
                    }
                    break;
                }
            }
            for (int i = currentPos.x - 1, j = currentPos.y + 1; i >= 0 && j < 8; --i, ++j) {
                if (board[i][j] == nullptr) {
                    legalMoves.push_back(Vector2i(i, j));
                }
                else {
                    if (board[i][j]->getPieceColor() != color) {
                        legalMoves.push_back(Vector2i(i, j));
                    }
                    break;
                }
            }
            for (int i = currentPos.x + 1, j = currentPos.y - 1; i < 8 && j >= 0; ++i, --j) {
                if (board[i][j] == nullptr) {
                    legalMoves.push_back(Vector2i(i, j));
                }
                else {
                    if (board[i][j]->getPieceColor() != color) {
                        legalMoves.push_back(Vector2i(i, j));
                    }
                    break;
                }
            }
            for (int i = currentPos.x - 1, j = currentPos.y - 1; i >= 0 && j >= 0; --i, --j) {
                if (board[i][j] == nullptr) {
                    legalMoves.push_back(Vector2i(i, j));
                }
                else {
                    if (board[i][j]->getPieceColor() != color) {
                        legalMoves.push_back(Vector2i(i, j));
                    }
                    break;
                }
            }
        }
        else {

            Vector2i kingPos;
            std::vector<Vector2i> attackedSquares;

            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (board[i][j] != nullptr && board[i][j]->getPiece() == Piece::King && board[i][j]->getPieceColor() == color) {
                        kingPos = Vector2i(i, j);
                    }
                }
            }

            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (board[i][j] != nullptr && board[i][j]->getPieceColor() != color) {
                        std::vector<Vector2i> opponentMoves = board[i][j]->getLegalMoves(Vector2i(i, j), board);

                        if (board[i][j]->getPiece() == Piece::Knight) {
                            for (const auto& move : opponentMoves) {
                                if (move == kingPos) {
                                    attackedSquares.push_back(Vector2i(i, j));
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (board[i][j] != nullptr && board[i][j]->getPieceColor() != color && board[i][j]->getPiece() != Piece::Knight) {
                        std::vector<Vector2i> opponentMoves = board[i][j]->getLegalMoves(Vector2i(i, j), board);

                        for (const auto& move : opponentMoves) {
                            if (move == kingPos) {
                                attackedSquares.push_back(Vector2i(i, j));

                                int dx = kingPos.x - i;
                                int dy = kingPos.y - j;
                                int xDir = (dx == 0) ? 0 : ((dx > 0) ? 1 : -1);
                                int yDir = (dy == 0) ? 0 : ((dy > 0) ? 1 : -1);

                                int x = i + xDir;
                                int y = j + yDir;
                                while (x != kingPos.x || y != kingPos.y) {
                                    attackedSquares.push_back(Vector2i(x, y));
                                    x += xDir;
                                    y += yDir;
                                }
                                attackedSquares.push_back(kingPos);

                                break;
                            }
                        }
                    }
                }
            }


            //Bishop Move
            for (int i = currentPos.x + 1, j = currentPos.y + 1; i < 8 && j < 8; ++i, ++j) {
                if (board[i][j] == nullptr) {
                    if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(i, j)) != attackedSquares.end()) {
                        legalMoves.push_back(Vector2i(i, j));
                    }
                }
                else {
                    if (board[i][j]->getPieceColor() != color) {
                        if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(i, j)) != attackedSquares.end()) {
                            legalMoves.push_back(Vector2i(i, j));
                        }
                    }
                    break;
                }
            }
            for (int i = currentPos.x - 1, j = currentPos.y + 1; i >= 0 && j < 8; --i, ++j) {
                if (board[i][j] == nullptr) {
                    if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(i, j)) != attackedSquares.end()) {
                        legalMoves.push_back(Vector2i(i, j));
                    }
                }
                else {
                    if (board[i][j]->getPieceColor() != color) {
                        if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(i, j)) != attackedSquares.end()) {
                            legalMoves.push_back(Vector2i(i, j));
                        }
                    }
                    break;
                }
            }
            for (int i = currentPos.x + 1, j = currentPos.y - 1; i < 8 && j >= 0; ++i, --j) {
                if (board[i][j] == nullptr) {
                    if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(i, j)) != attackedSquares.end())
                        legalMoves.push_back(Vector2i(i, j));
                }
                else {
                    if (board[i][j]->getPieceColor() != color) {
                        if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(i, j)) != attackedSquares.end())
                            legalMoves.push_back(Vector2i(i, j));
                    }
                    break;
                }
            }
            for (int i = currentPos.x - 1, j = currentPos.y - 1; i >= 0 && j >= 0; --i, --j) {
                if (board[i][j] == nullptr) {
                    if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(i, j)) != attackedSquares.end())
                        legalMoves.push_back(Vector2i(i, j));
                }
                else {
                    if (board[i][j]->getPieceColor() != color) {
                        if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(i, j)) != attackedSquares.end())
                            legalMoves.push_back(Vector2i(i, j));
                    }
                    break;
                }
            }

            //Rook Move
            for (int i = currentPos.x - 1; i >= 0; --i) {
                if (board[i][currentPos.y] == nullptr) {
                    if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(i, currentPos.y)) != attackedSquares.end())
                        legalMoves.push_back(Vector2i(i, currentPos.y));
                }
                else {
                    if (board[i][currentPos.y]->getPieceColor() != color) {
                        if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(i, currentPos.y)) != attackedSquares.end())
                            legalMoves.push_back(Vector2i(i, currentPos.y));
                    }
                    break;
                }
            }
            for (int i = currentPos.x + 1; i < 8; ++i) {
                if (board[i][currentPos.y] == nullptr) {
                    if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(i, currentPos.y)) != attackedSquares.end())
                        legalMoves.push_back(Vector2i(i, currentPos.y));
                }
                else {
                    if (board[i][currentPos.y]->getPieceColor() != color) {
                        if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(i, currentPos.y)) != attackedSquares.end())
                            legalMoves.push_back(Vector2i(i, currentPos.y));
                    }
                    break;
                }
            }
            for (int j = currentPos.y - 1; j >= 0; --j) {
                if (board[currentPos.x][j] == nullptr) {
                    if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(currentPos.x, j)) != attackedSquares.end())
                        legalMoves.push_back(Vector2i(currentPos.x, j));
                }
                else {
                    if (board[currentPos.x][j]->getPieceColor() != color) {
                        if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(currentPos.x, j)) != attackedSquares.end())
                            legalMoves.push_back(Vector2i(currentPos.x, j));
                    }
                    break;
                }
            }
            for (int j = currentPos.y + 1; j < 8; ++j) {
                if (board[currentPos.x][j] == nullptr) {
                    if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(currentPos.x, j)) != attackedSquares.end())
                        legalMoves.push_back(Vector2i(currentPos.x, j));
                }
                else {
                    if (board[currentPos.x][j]->getPieceColor() != color) {
                        if (std::find(attackedSquares.begin(), attackedSquares.end(), Vector2i(currentPos.x, j)) != attackedSquares.end())
                            legalMoves.push_back(Vector2i(currentPos.x, j));
                    }
                    break;
                }
            }
        }

    }
    else {
        if (!hunYouChecked) {
            int x = currentPos.x + xDir;
            int y = currentPos.y + yDir;

            while (x >= 0 && x < 8 && y >= 0 && y < 8) {
                if (board[x][y] == nullptr) {
                    legalMoves.push_back(Vector2i(x, y));
                }
                else {
                    if (board[x][y]->getPieceColor() != color) {
                        legalMoves.push_back(Vector2i(x, y));
                    }
                    break;
                }
                x += xDir;
                y += yDir;
            }

            return legalMoves;

        }
    }

    return legalMoves;

}

//King
King::King(const Texture& texture, PieceColor pieceColor) {
    color = pieceColor;
    sprite.setTexture(texture);
    sprite.setScale(scale, scale);
    moves = 0;
}
PieceColor King::getPieceColor() { return color; }
Piece King::getPiece() { return Piece::King; }
void King::draw(RenderWindow& window) { window.draw(sprite); }
void King::setPosition(int i, int j) { sprite.setPosition(j * 80, i * 80); }
void King::moveCounter() { moves++; }
int King::getMoves() { return moves; }
int King::getKingMoves() { return moves; }
bool King::QueenSideCastle() { return canCastleQueenSide; }
bool King::KingSideCastle() { return canCastleKingSide; }
vector<Vector2i> King::getMovesForCheck(Vector2i currentPos, ChessBoard* board[][8]) {
        std::vector<Vector2i> invalidateKingMoves;

        int dx[8] = { 1, 1, 1, 0, 0, -1, -1, -1 };
        int dy[8] = { 1, 0, -1, 1, -1, 1, 0, -1 };

        std::vector<Vector2i> attackedSquares;


        for (int i = 0; i < 8; ++i) {
            int newX = currentPos.x + dx[i];
            int newY = currentPos.y + dy[i];

            if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
                if (board[newX][newY] == nullptr || board[newX][newY]->getPieceColor() == color) {
                    invalidateKingMoves.push_back(Vector2i(newX, newY));
                }
            }
        }

        return invalidateKingMoves;
    }
vector<Vector2i> King::getLegalMoves(Vector2i currentPos, ChessBoard* board[][8]) {
        std::vector<Vector2i> legalMoves;
        ChessBoard* KingSideRook = board[currentPos.x][currentPos.y + 3];
        ChessBoard* QueenSideRook = board[currentPos.x][currentPos.y - 4];

        // Castling
        if (board[currentPos.x][currentPos.y + 1] == nullptr && board[currentPos.x][currentPos.y + 2] == nullptr) {
            if (moves == 0 && KingSideRook != nullptr && KingSideRook->getRookMoves() == 0) {
                legalMoves.push_back(Vector2i(currentPos.x, currentPos.y + 2));
                canCastleKingSide = true;
            }
        }
        if (board[currentPos.x][currentPos.y - 1] == nullptr && board[currentPos.x][currentPos.y - 2] == nullptr &&
            board[currentPos.x][currentPos.y - 3] == nullptr) {
            if (moves == 0 && QueenSideRook != nullptr && QueenSideRook->getRookMoves() == 0) {
                legalMoves.push_back(Vector2i(currentPos.x, currentPos.y - 2));
                canCastleQueenSide = true;
            }
        }

        int dx[8] = { 1, 1, 1, 0, 0, -1, -1, -1 };
        int dy[8] = { 1, 0, -1, 1, -1, 1, 0, -1 };

        std::vector<Vector2i> attackedSquares;


        for (int i = 0; i < 8; ++i) {
            int newX = currentPos.x + dx[i];
            int newY = currentPos.y + dy[i];

            if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
                if (board[newX][newY] == nullptr || board[newX][newY]->getPieceColor() != color) {
                    legalMoves.push_back(Vector2i(newX, newY));
                }
            }
        }


        std::vector<Vector2i> filteredMoves;
        for (const auto& move : legalMoves) {
            bool isAttacked = false;

            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (board[i][j] != nullptr && board[i][j]->getPieceColor() != color) {
                        if (board[i][j]->getPiece() == Piece::Pawn) {
                            if (abs(i - move.x) == 1 && abs(j - move.y) == 1) {
                                isAttacked = true;
                                break;
                            }
                        }
                        else {
                            std::vector<Vector2i> opponentMoves = board[i][j]->getMovesForCheck(Vector2i(i, j), board);
                            if (std::find(opponentMoves.begin(), opponentMoves.end(), move) != opponentMoves.end()) {
                                isAttacked = true;
                                break;
                            }
                        }
                    }
                }
                if (isAttacked) {
                    break;
                }
            }

            if (!isAttacked) {
                filteredMoves.push_back(move);
            }
        }
        return filteredMoves;
    }