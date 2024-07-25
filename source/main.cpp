#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<SFML/System/Clock.hpp>
#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include"chessBoard.h"
#include"pieces.h"

#define BOARD_SIZE 8
#define TILE_SIZE 80
#define WINDOW_WIDTH ((BOARD_SIZE * TILE_SIZE) + 400)
#define WINDOW_HEIGHT (BOARD_SIZE * TILE_SIZE)

using namespace sf;

enum class PieceSet { Cburnett, Alpha, Merida, Kosal };
enum class GameState { Menu, Game };

const Color highlightColor(123, 40, 107);
const Color boardLight(240, 217, 182);
const Color boardDark(181, 136, 99, 255);

void drawChessboard(RenderWindow& window);
void drawSidePane(RenderWindow& window, PieceColor currentPlayer, float blackTimeRemaining, float whiteTimeRemaining, int gameOver, char[], char[]);
void drawHighlight(RenderWindow& window, const std::vector<Vector2i>& highlightedPositions);
void kingInCheck(ChessBoard* board[][8], PieceColor currentPlayer, bool& checkmate, bool& stalemate, bool& check);
void renderPromotionBox(RenderWindow& window, Vector2f position, PieceColor color, PieceSet pieceSet);
Piece handlePawnPromotion(RenderWindow& window, PieceColor color, int newX, int newY, PieceSet pieceSet,ChessBoard* board[BOARD_SIZE][BOARD_SIZE]);
void game(RenderWindow&, PieceSet pieceSet);

int main() {
    RenderWindow menu(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Chess Masters", Style::Titlebar | Style::Close);

    Texture bgTexture;
    Sprite bg;
    Font font;
    font.loadFromFile("external/timer.ttf");

    PieceSet piece = PieceSet::Cburnett;
    GameState state = GameState::Menu;

    while (menu.isOpen()) {
        sf::Event event;
        while (menu.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                menu.close();
            else if (event.type == sf::Event::MouseButtonPressed) {
                Vector2f mousePos = menu.mapPixelToCoords(Mouse::getPosition(menu));

                if (mousePos.y >= 471  && mousePos.x >= 898) {
                    state = GameState::Game;
                }
                else if (mousePos.y >= 373 && mousePos.y <= 470) {
                    int deltaX = mousePos.x - 266;
                    int box = 97;
                    int boxSpacing = 40;

                    if (deltaX >= 0 && deltaX <= box) {
                        piece = PieceSet::Cburnett;
                    }
                    else if (deltaX > box + boxSpacing && deltaX <= 2 * box + boxSpacing) {
                        piece = PieceSet::Alpha;
                    }
                    else if (deltaX > 2 * box + 2 * boxSpacing && deltaX <= 3 * box + 2 * boxSpacing) {
                        piece = PieceSet::Merida;
                    }
                    else if (deltaX > 3 * box + 3 * boxSpacing && deltaX <= 4 * box + 3 * boxSpacing) {
                        piece = PieceSet::Kosal;
                    }
                }
            }
        }

        menu.clear();

        if (piece == PieceSet::Cburnett) {
            bgTexture.loadFromFile("external/bg/bgC.png");
            bg.setTexture(bgTexture);
        }
        else if (piece == PieceSet::Alpha) {
            bgTexture.loadFromFile("external/bg/bgA.png");
            bg.setTexture(bgTexture);
        }
        else if (piece == PieceSet::Merida) {
            bgTexture.loadFromFile("external/bg/bgM.png");
            bg.setTexture(bgTexture);
        }
        else if (piece == PieceSet::Kosal) {
            bgTexture.loadFromFile("external/bg/bgK.png");
            bg.setTexture(bgTexture);
        }
        menu.draw(bg);
        if (state == GameState::Game)
            game(menu, piece);
        menu.display();
    }
}

//all game code
void game(RenderWindow& window, PieceSet pieceSet) {
    Texture whitePawnTexture, blackPawnTexture, whiteRookTexture, blackRookTexture, whiteKnightTexture, blackKnightTexture,
        whiteBishopTexture, blackBishopTexture, whiteQueenTexture, blackQueenTexture, whiteKingTexture, blackKingTexture, checkHighlightTexture;
    checkHighlightTexture.loadFromFile("external/check.png");
    
    if (pieceSet == PieceSet::Cburnett) {
        whitePawnTexture.loadFromFile("external/pieces/cburnett/wP.png");
        blackPawnTexture.loadFromFile("external/pieces/cburnett/bP.png");
        whiteRookTexture.loadFromFile("external/pieces/cburnett/wR.png");
        blackRookTexture.loadFromFile("external/pieces/cburnett/bR.png");
        whiteKnightTexture.loadFromFile("external/pieces/cburnett/wN.png");
        blackKnightTexture.loadFromFile("external/pieces/cburnett/bN.png");
        whiteBishopTexture.loadFromFile("external/pieces/cburnett/wB.png");
        blackBishopTexture.loadFromFile("external/pieces/cburnett/bB.png");
        whiteQueenTexture.loadFromFile("external/pieces/cburnett/wQ.png");
        blackQueenTexture.loadFromFile("external/pieces/cburnett/bQ.png");
        whiteKingTexture.loadFromFile("external/pieces/cburnett/wK.png");
        blackKingTexture.loadFromFile("external/pieces/cburnett/bK.png");
    }
    else if (pieceSet==PieceSet::Alpha) {
        whitePawnTexture.loadFromFile("external/pieces/alpha/wP.png");
        blackPawnTexture.loadFromFile("external/pieces/alpha/bP.png");
        whiteRookTexture.loadFromFile("external/pieces/alpha/wR.png");
        blackRookTexture.loadFromFile("external/pieces/alpha/bR.png");
        whiteKnightTexture.loadFromFile("external/pieces/alpha/wN.png");
        blackKnightTexture.loadFromFile("external/pieces/alpha/bN.png");
        whiteBishopTexture.loadFromFile("external/pieces/alpha/wB.png");
        blackBishopTexture.loadFromFile("external/pieces/alpha/bB.png");
        whiteQueenTexture.loadFromFile("external/pieces/alpha/wQ.png");
        blackQueenTexture.loadFromFile("external/pieces/alpha/bQ.png");
        whiteKingTexture.loadFromFile("external/pieces/alpha/wK.png");
        blackKingTexture.loadFromFile("external/pieces/alpha/bK.png");
    }
    else if (pieceSet == PieceSet::Merida) {
        whitePawnTexture.loadFromFile("external/pieces/merida/wP.png");
        blackPawnTexture.loadFromFile("external/pieces/merida/bP.png");
        whiteRookTexture.loadFromFile("external/pieces/merida/wR.png");
        blackRookTexture.loadFromFile("external/pieces/merida/bR.png");
        whiteKnightTexture.loadFromFile("external/pieces/merida/wN.png");
        blackKnightTexture.loadFromFile("external/pieces/merida/bN.png");
        whiteBishopTexture.loadFromFile("external/pieces/merida/wB.png");
        blackBishopTexture.loadFromFile("external/pieces/merida/bB.png");
        whiteQueenTexture.loadFromFile("external/pieces/merida/wQ.png");
        blackQueenTexture.loadFromFile("external/pieces/merida/bQ.png");
        whiteKingTexture.loadFromFile("external/pieces/merida/wK.png");
        blackKingTexture.loadFromFile("external/pieces/merida/bK.png");
    }
    else if (pieceSet == PieceSet::Kosal) {
        whitePawnTexture.loadFromFile("external/pieces/kosal/wP.png");
        blackPawnTexture.loadFromFile("external/pieces/kosal/bP.png");
        whiteRookTexture.loadFromFile("external/pieces/kosal/wR.png");
        blackRookTexture.loadFromFile("external/pieces/kosal/bR.png");
        whiteKnightTexture.loadFromFile("external/pieces/kosal/wN.png");
        blackKnightTexture.loadFromFile("external/pieces/kosal/bN.png");
        whiteBishopTexture.loadFromFile("external/pieces/kosal/wB.png");
        blackBishopTexture.loadFromFile("external/pieces/kosal/bB.png");
        whiteQueenTexture.loadFromFile("external/pieces/kosal/wQ.png");
        blackQueenTexture.loadFromFile("external/pieces/kosal/bQ.png");
        whiteKingTexture.loadFromFile("external/pieces/kosal/wK.png");
        blackKingTexture.loadFromFile("external/pieces/kosal/bK.png");
    }

    Rook whiteRook1(whiteRookTexture, PieceColor::White), blackRook1(blackRookTexture, PieceColor::Black);
    Rook whiteRook2(whiteRookTexture, PieceColor::White), blackRook2(blackRookTexture, PieceColor::Black);
    Knight whiteKnight1(whiteKnightTexture, PieceColor::White), blackKnight1(blackKnightTexture, PieceColor::Black);
    Knight whiteKnight2(whiteKnightTexture, PieceColor::White), blackKnight2(blackKnightTexture, PieceColor::Black);
    Bishop whiteBishop1(whiteBishopTexture, PieceColor::White), blackBishop1(blackBishopTexture, PieceColor::Black);
    Bishop whiteBishop2(whiteBishopTexture, PieceColor::White), blackBishop2(blackBishopTexture, PieceColor::Black);
    Queen whiteQueen(whiteQueenTexture, PieceColor::White), blackQueen(blackQueenTexture, PieceColor::Black);
    King whiteKing(whiteKingTexture, PieceColor::White), blackKing(blackKingTexture, PieceColor::Black);
    Pawn whitePawn1(whitePawnTexture, PieceColor::White), blackPawn1(blackPawnTexture, PieceColor::Black);
    Pawn whitePawn2(whitePawnTexture, PieceColor::White), blackPawn2(blackPawnTexture, PieceColor::Black);
    Pawn whitePawn3(whitePawnTexture, PieceColor::White), blackPawn3(blackPawnTexture, PieceColor::Black);
    Pawn whitePawn4(whitePawnTexture, PieceColor::White), blackPawn4(blackPawnTexture, PieceColor::Black);
    Pawn whitePawn5(whitePawnTexture, PieceColor::White), blackPawn5(blackPawnTexture, PieceColor::Black);
    Pawn whitePawn6(whitePawnTexture, PieceColor::White), blackPawn6(blackPawnTexture, PieceColor::Black);
    Pawn whitePawn7(whitePawnTexture, PieceColor::White), blackPawn7(blackPawnTexture, PieceColor::Black);
    Pawn whitePawn8(whitePawnTexture, PieceColor::White), blackPawn8(blackPawnTexture, PieceColor::Black);

    ChessBoard* board[BOARD_SIZE][BOARD_SIZE] = {
            {&blackRook1, &blackKnight1, &blackBishop1, &blackQueen, &blackKing , &blackBishop2,&blackKnight2, &blackRook2},
            {&blackPawn1, &blackPawn2, &blackPawn3, &blackPawn4, &blackPawn5, &blackPawn6, &blackPawn7, &blackPawn8},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
            {&whitePawn1, &whitePawn2, &whitePawn3, &whitePawn4, &whitePawn5, &whitePawn6, &whitePawn7, &whitePawn8},
            {&whiteRook1, &whiteKnight1, &whiteBishop1, &whiteQueen, &whiteKing, &whiteBishop2, &whiteKnight2, &whiteRook2 }
    };


    char whiteCaptures[15] = "";
    char blackCaptures[15] = "";
    int whitePlayersCaptured = 0;
    int blackPlayersCaptured = 0;
    bool checked = false;
    bool checkedCpy = false;
    bool checkmate = false;
    bool stalemate = false;
    int gameOver = 0;
    bool isPieceSelected = false;
    Vector2i selectedPiecePos(-1, -1);
    Vector2f offset;
    Vector2i kingPos(-1, -1);
    ChessBoard* selectedPiece = nullptr;
    PieceColor playerColor = PieceColor::White;

    float gameTimer = 600.0;
    Clock whiteTimer;
    Clock blackTimer;
    float whiteTimeRemaining = gameTimer;
    float blackTimeRemaining = gameTimer;

    SoundBuffer moveBuffer, promotionBuffer, captureBuffer, checkBuffer, castleBuffer, mateBuffer, drawBuffer, lowTimeBuffer;

    // Sound Effects
    moveBuffer.loadFromFile("external/move.ogg");
    Sound move(moveBuffer);
    captureBuffer.loadFromFile("external/capture.ogg");
    Sound capture(captureBuffer);
    checkBuffer.loadFromFile("external/check.mp3");
    Sound check(checkBuffer);
    castleBuffer.loadFromFile("external/castle.mp3");
    Sound castle(castleBuffer);
    promotionBuffer.loadFromFile("external/promote.mp3");
    Sound promotion(promotionBuffer);
    mateBuffer.loadFromFile("external/checkmate.mp3");
    Sound mate(mateBuffer);
    drawBuffer.loadFromFile("external/checkmate.mp3");
    Sound draw(drawBuffer);
    lowTimeBuffer.loadFromFile("external/LowTime.mp3");
    Sound LowTime(lowTimeBuffer);
    int P1lowTime = 0, P2lowTime = 0;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {

            if (event.type == Event::Closed) {
                window.close();
            }

            else if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
                    for (int i = 0; i < BOARD_SIZE; ++i) {
                        for (int j = 0; j < BOARD_SIZE; ++j) {
                            if (board[i][j] != nullptr && board[i][j]->contains(mousePos)) {
                                if (board[i][j]->getPieceColor() == playerColor) {
                                    isPieceSelected = true;
                                    selectedPiece = board[i][j];
                                    selectedPiecePos = Vector2i(i, j);
                                    offset = mousePos - board[i][j]->getSprite().getPosition();
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            else if (event.type == Event::MouseButtonReleased) {



                if (event.mouseButton.button == Mouse::Left && isPieceSelected) {
                    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
                    int newX = static_cast<int>(mousePos.y / TILE_SIZE);
                    int newY = static_cast<int>(mousePos.x / TILE_SIZE);

                    bool isLegalMove = false;
                    Vector2i enPassantPosition(-1, -1);
                    bool isEnPassantCapture = false;

                    for (const auto& legalMove : selectedPiece->getLegalMoves(selectedPiecePos, board)) {
                        if (legalMove == Vector2i(newX, newY)) {
                            isLegalMove = true;
                            if (selectedPiece->getPiece() == Piece::Pawn && newX != selectedPiecePos.x) {
                                if (selectedPiece->enPassantAvailable()) {
                                    isEnPassantCapture = true;
                                    enPassantPosition = Vector2i(newX, newY);
                                }
                            }
                            break;
                        }
                    }


                    bool pieceCaptured = false;
                    bool pawnPromoted = false;
                    bool castling = false;
                    if (isLegalMove) {

                        // Track Captured Piece
                        if (board[newX][newY] != nullptr && board[newX][newY]->getPiece() == Piece::Queen) {
                            if (board[newX][newY] != nullptr && board[newX][newY]->getPieceColor() == PieceColor::White)
                                blackCaptures[blackPlayersCaptured++] = 'w';
                            else if (board[newX][newY] != nullptr && board[newX][newY]->getPieceColor() == PieceColor::Black)
                                whiteCaptures[whitePlayersCaptured++] = 'w';
                            pieceCaptured = true;
                        }
                        else if (board[newX][newY] != nullptr && board[newX][newY]->getPiece() == Piece::Bishop) {
                            if (board[newX][newY] != nullptr && board[newX][newY]->getPieceColor() == PieceColor::White)
                                blackCaptures[blackPlayersCaptured++] = 'v';
                            else if (board[newX][newY] != nullptr && board[newX][newY]->getPieceColor() == PieceColor::Black)
                                whiteCaptures[whitePlayersCaptured++] = 'v';
                            pieceCaptured = true;
                        }
                        else if (board[newX][newY] != nullptr && board[newX][newY]->getPiece() == Piece::Knight) {
                            if (board[newX][newY] != nullptr && board[newX][newY]->getPieceColor() == PieceColor::White)
                                blackCaptures[blackPlayersCaptured++] = 'm';
                            else if (board[newX][newY] != nullptr && board[newX][newY]->getPieceColor() == PieceColor::Black)
                                whiteCaptures[whitePlayersCaptured++] = 'm';
                            pieceCaptured = true;
                        }
                        else if (board[newX][newY] != nullptr && board[newX][newY]->getPiece() == Piece::Rook) {
                            if (board[newX][newY] != nullptr && board[newX][newY]->getPieceColor() == PieceColor::White)
                                blackCaptures[blackPlayersCaptured++] = 't';
                            else if (board[newX][newY] != nullptr && board[newX][newY]->getPieceColor() == PieceColor::Black)
                                whiteCaptures[whitePlayersCaptured++] = 't';
                            pieceCaptured = true;
                        }
                        else if (board[newX][newY] != nullptr && board[newX][newY]->getPiece() == Piece::Pawn) {
                            if (board[newX][newY] != nullptr && board[newX][newY]->getPieceColor() == PieceColor::White)
                                blackCaptures[blackPlayersCaptured++] = 'o';
                            else if (board[newX][newY] != nullptr && board[newX][newY]->getPieceColor() == PieceColor::Black)
                                whiteCaptures[whitePlayersCaptured++] = 'o';
                            pieceCaptured = true;
                        }

                        selectedPiece->moveCounter();
                        Vector2f finalPosition(newY * TILE_SIZE, newX * TILE_SIZE);
                        board[newX][newY] = board[selectedPiecePos.x][selectedPiecePos.y];
                        board[selectedPiecePos.x][selectedPiecePos.y] = nullptr;
                        board[newX][newY]->getSprite().setPosition(finalPosition);

                        if (selectedPiece->getPiece() == Piece::Pawn) {

                            if (isEnPassantCapture) {
                                int capturedPawnX, capturedPawnY;
                                if (selectedPiece->getPieceColor() == PieceColor::White && selectedPiece->getPawnMoves() != 0) {
                                    capturedPawnX = newX + 1;
                                    capturedPawnY = newY;
                                }
                                else {
                                    capturedPawnX = newX - 1;
                                    capturedPawnY = newY;
                                }
                                board[capturedPawnX][capturedPawnY] = nullptr; // Remove captured pawn
                            }

                            if ((newX == BOARD_SIZE - 1 && selectedPiece->getPieceColor() == PieceColor::Black) ||
                                (newX == 0 && selectedPiece->getPieceColor() == PieceColor::White)) {

                                // Promote pawn and handle user selection
                                Piece promotedPiece = handlePawnPromotion(window, selectedPiece->getPieceColor(), newX, newY, pieceSet,board);

                                // Create the promoted piece based on the user's selection
                                switch (promotedPiece) {
                                case Piece::Knight:
                                    board[newX][newY] = new Knight((selectedPiece->getPieceColor() == PieceColor::White) ? whiteKnightTexture : blackKnightTexture, selectedPiece->getPieceColor());
                                    break;
                                case Piece::Rook:
                                    board[newX][newY] = new Rook((selectedPiece->getPieceColor() == PieceColor::White) ? whiteRookTexture : blackRookTexture, selectedPiece->getPieceColor());
                                    break;
                                case Piece::Bishop:
                                    board[newX][newY] = new Bishop((selectedPiece->getPieceColor() == PieceColor::White) ? whiteBishopTexture : blackBishopTexture, selectedPiece->getPieceColor());
                                    break;
                                case Piece::Queen:
                                    board[newX][newY] = new Queen((selectedPiece->getPieceColor() == PieceColor::White) ? whiteQueenTexture : blackQueenTexture, selectedPiece->getPieceColor());
                                    break;
                                default:
                                    break;
                                }

                                // Set the position of the promoted piece
                                if (board[newX][newY] != nullptr) {
                                    board[newX][newY]->setPosition(newX, newY);
                                }

                                // Clear the position of the pawn being promoted
                                board[selectedPiecePos.x][selectedPiecePos.y] = nullptr;
                                pawnPromoted = true;
                            }
                        }

                        else if (selectedPiece->getPiece() == Piece::King) {
                            
                            // Castling
                            if (selectedPiece->getPieceColor() == PieceColor::White && selectedPiece->getMoves() == 1) {
                                if (board[newX][newY + 1] != nullptr && board[newX][newY + 1]->getPiece() == Piece::Rook) {
                                    board[7][7]->moveCounter();
                                    board[7][5] = board[7][7];
                                    board[7][7] = nullptr;
                                    castling = true;
                                }
                                else if (board[newX][newY - 2] != nullptr && board[newX][newY - 2]->getPiece() == Piece::Rook) {
                                    board[7][0]->moveCounter();
                                    board[7][3] = board[7][0];
                                    board[7][0] = nullptr;
                                    castling = true;
                                }
                            }
                            if (selectedPiece->getPieceColor() == PieceColor::Black && selectedPiece->getMoves() == 1) {
                                if (board[newX][newY + 1] != nullptr && board[newX][newY + 1]->getPiece() == Piece::Rook) {
                                    board[0][7]->moveCounter();
                                    board[0][5] = board[0][7];
                                    board[0][7] = nullptr;
                                    castling = true;
                                }
                                else if (board[newX][newY - 2] != nullptr && board[newX][newY - 2]->getPiece() == Piece::Rook) {
                                    board[0][0]->moveCounter();
                                    board[0][3] = board[0][0];
                                    board[0][0] = nullptr;
                                    castling = true;
                                }
                            }
                            
                        }

                        playerColor = (playerColor == PieceColor::White) ? PieceColor::Black : PieceColor::White;
                        kingInCheck(board, playerColor, checkmate, stalemate, checked);

                        // Sound Effects
                        if (checkmate) {
                            mate.play();
                            if (playerColor == PieceColor::Black) {
                                for (int i = 0; i < BOARD_SIZE; ++i) {
                                    for (int j = 0; j < BOARD_SIZE; ++j) {
                                        if (board[i][j] != nullptr) {
                                            board[i][j]->suspendGame();
                                        }
                                        gameOver = 2;
                                    }
                                }
                            }
                            else if (playerColor == PieceColor::White) {
                                for (int i = 0; i < BOARD_SIZE; ++i) {
                                    for (int j = 0; j < BOARD_SIZE; ++j) {
                                        if (board[i][j] != nullptr)
                                            board[i][j]->suspendGame();
                                        gameOver = 1;
                                    }
                                }
                            }
                        }
                        else if (checked) {
                            check.play();
                        }
                        else if (stalemate) {
                            mate.play();
                            for (int i = 0; i < BOARD_SIZE; ++i) {
                                for (int j = 0; j < BOARD_SIZE; ++j) {
                                    if (board[i][j] != nullptr)
                                        board[i][j]->suspendGame();
                                    gameOver = 3;
                                }
                            }
                        }
                        else if (pawnPromoted) {
                            promotion.play();
                        }
                        else if (castling) {
                            castle.play();
                        }
                        else if (pieceCaptured) {
                            capture.play();
                        }
                        else {
                            move.play();
                        }
                    }
                    isPieceSelected = false;
                    checkedCpy = checked;
                    checked = false;
                }
            }
        }

        //Update GameTimer
        if (playerColor == PieceColor::White && gameOver == 0) {
            whiteTimeRemaining -= whiteTimer.getElapsedTime().asSeconds();
            whiteTimer.restart();
            blackTimer.restart();
            if (whiteTimeRemaining < 60 && P1lowTime < 1) {
                LowTime.play();
                P1lowTime++;
            }
            if (whiteTimeRemaining <= 0) {
                mate.play();
                for (int i = 0; i < BOARD_SIZE; ++i) {
                    for (int j = 0; j < BOARD_SIZE; ++j) {
                        if (board[i][j] != nullptr)
                            board[i][j]->suspendGame();
                        gameOver = 4;
                    }
                }
            }
        }
        else if (playerColor == PieceColor::Black && gameOver == 0) {
            blackTimeRemaining -= blackTimer.getElapsedTime().asSeconds();
            blackTimer.restart();
            whiteTimer.restart();
            if (blackTimeRemaining < 60 && P2lowTime < 1) {
                LowTime.play();
                P2lowTime++;
            }
            if (blackTimeRemaining <= 0) {
                mate.play();
                for (int i = 0; i < BOARD_SIZE; ++i) {
                    for (int j = 0; j < BOARD_SIZE; ++j) {
                        if (board[i][j] != nullptr)
                            board[i][j]->suspendGame();
                        gameOver = 5;
                    }
                }
            }
        }

        window.clear();

        //Draw Chessboard
        drawChessboard(window);
        drawSidePane(window, playerColor, blackTimeRemaining, whiteTimeRemaining, gameOver, whiteCaptures, blackCaptures);

        //Draw Pieces
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i][j] == nullptr)
                    continue;
                board[i][j]->setPosition(i, j);
                board[i][j]->draw(window);
            }
        }

        std::vector<Vector2i> highlightedPositions;
        if (isPieceSelected) {
            highlightedPositions = selectedPiece->getLegalMoves(selectedPiecePos, board);
            drawHighlight(window, highlightedPositions);
        }

        window.display();
    }
}
void drawChessboard(RenderWindow& window) {
    bool isLight = true;
    RectangleShape tile(Vector2f(TILE_SIZE, TILE_SIZE));
    Font font;
    font.loadFromFile("external/rank-file.ttf");

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            tile.setPosition(i * TILE_SIZE, j * TILE_SIZE);
            tile.setFillColor(isLight ? boardLight : boardDark);
            isLight = !isLight;
            window.draw(tile);
        }
        isLight = !isLight;
    }
    for (int i = 0; i < BOARD_SIZE; ++i) {
        Text fileText(char('a' + i), font, 14);
        fileText.setPosition(i * TILE_SIZE + 65, (BOARD_SIZE - 1) * TILE_SIZE + 60);
        fileText.setFillColor(Color::Black);
        window.draw(fileText);
    }

    for (int j = 0; j < BOARD_SIZE; ++j) {
        Text rankText(std::to_string(8 - j), font, 14);
        rankText.setPosition(TILE_SIZE - 75, j * TILE_SIZE + 5);
        rankText.setFillColor(Color::Black);
        window.draw(rankText);
    }
}
void drawSidePane(sf::RenderWindow& window, PieceColor currentPlayer, float blackTimeRemaining, float whiteTimeRemaining, int gameOver, char whitePlayersCaptures[], char blackPlayersCaptures[]) {
    Texture BackgroundTexture;
    BackgroundTexture.loadFromFile("external/sidePane.png");
    Sprite Background(BackgroundTexture);
    Background.setPosition(640, 0);
    window.draw(Background);

    Texture infoBarP2Texture;
    infoBarP2Texture.loadFromFile("external/infoBarP2Move.png");
    Sprite infoBarP2(infoBarP2Texture);
    infoBarP2.setPosition(640, 110);

    Texture infoBarP2TextureWarning;
    infoBarP2TextureWarning.loadFromFile("external/infoBarP2MoveWarning.png");
    Sprite infoBarP2Warning(infoBarP2TextureWarning);
    infoBarP2Warning.setPosition(640, 110);

    Texture infoBarP1TextureWarning;
    infoBarP1TextureWarning.loadFromFile("external/infoBarP1MoveWarning.png");
    Sprite infoBarP1Warning(infoBarP1TextureWarning);
    infoBarP1Warning.setPosition(640, 110);

    Texture infoBarP1Texture;
    infoBarP1Texture.loadFromFile("external/infoBarP1Move.png");
    Sprite infoBarP1(infoBarP1Texture);
    infoBarP1.setPosition(640, 110);


    if (currentPlayer == PieceColor::Black) {
        if (blackTimeRemaining < 60.0) {
            window.draw(infoBarP1Warning);
        }
        else {
            window.draw(infoBarP1);
        }
    }
    else {
        if (whiteTimeRemaining < 60.0) {
            window.draw(infoBarP2Warning);
        }
        else {
            window.draw(infoBarP2);
        }
    }

    Color textColor(164, 160, 160);
    Font timer, announcement, chess;
    timer.loadFromFile("external/timer.ttf");
    announcement.loadFromFile("external/announcement.otf");
    chess.loadFromFile("external/chessFont.ttf");

    Text blackWinsCM("Black Wins by CheckMate", announcement, 20);
    blackWinsCM.setPosition(690, 300);
    blackWinsCM.setFillColor(textColor);

    Text whiteWinsCM("White Wins by CheckMate", announcement, 20);
    whiteWinsCM.setPosition(690, 300);
    whiteWinsCM.setFillColor(textColor);

    Text blackWinsTO("Black Wins by Time-Out", announcement, 20);
    blackWinsTO.setPosition(690, 300);
    blackWinsTO.setFillColor(textColor);

    Text whiteWinsTO("White Wins by Time-Out", announcement, 20);
    whiteWinsTO.setPosition(690, 300);
    whiteWinsTO.setFillColor(textColor);

    Text draw( "1/2 - 1/2 (Draw)", announcement, 36);
    draw.setPosition(690, 285);
    draw.setFillColor(textColor);

    Text whiteTimerText;
    whiteTimerText.setFont(timer);
    whiteTimerText.setCharacterSize(36);
    whiteTimerText.setFillColor(textColor);

    Text blackTimerText;
    blackTimerText.setFont(timer);
    blackTimerText.setCharacterSize(36);
    blackTimerText.setFillColor(textColor);

    int whiteMinutes = static_cast<int>(whiteTimeRemaining) / 60;
    int whiteSeconds = static_cast<int>(whiteTimeRemaining) % 60;
    int blackMinutes = static_cast<int>(blackTimeRemaining) / 60;
    int blackSeconds = static_cast<int>(blackTimeRemaining) % 60;
    whiteTimerText.setString(std::to_string(whiteMinutes) + ":" + (whiteSeconds < 10 ? "0" : "") + std::to_string(whiteSeconds));
    blackTimerText.setString(std::to_string(blackMinutes) + ":" + (blackSeconds < 10 ? "0" : "") + std::to_string(blackSeconds));

    // Set positions for timer text
    blackTimerText.setPosition(690, 120);
    whiteTimerText.setPosition(690, blackTimerText.getPosition().y + 335);

    Text player1("Player 2", timer, 28);
    player1.setPosition(blackTimerText.getPosition().x + 15, blackTimerText.getPosition().y + 55);
    player1.setFillColor(textColor);

    Text player2("Player 1", timer, 28);
    player2.setPosition(blackTimerText.getPosition().x + 15, blackTimerText.getPosition().y + 285);
    player2.setFillColor(textColor);

    Text player1Captures(whitePlayersCaptures, chess, 20);
    player1Captures.setPosition(whiteTimerText.getPosition().x, whiteTimerText.getPosition().y+60);
    player1Captures.setFillColor(textColor);

    Text player2Captures(blackPlayersCaptures, chess, 20);
    player2Captures.setPosition(blackTimerText.getPosition().x, blackTimerText.getPosition().y-60);
    player2Captures.setFillColor(textColor);

    // Draw timers
    window.draw(whiteTimerText);
    window.draw(blackTimerText);

    if (gameOver == 1) {
        window.draw(blackWinsCM);
    }
    else if (gameOver == 2) {
        window.draw(whiteWinsCM);
    }
    else if (gameOver == 4) {
        window.draw(blackWinsTO);
    }
    else if (gameOver == 5) {
        window.draw(whiteWinsTO);
    }
    else if (gameOver == 3) {
        window.draw(draw);
    }
    window.draw(player1Captures);
    window.draw(player2Captures);
    window.draw(player1);
    window.draw(player2);
}
void drawHighlight(RenderWindow& window, const std::vector<Vector2i>& highlightedPositions) {
    for (const Vector2i& pos : highlightedPositions) {
        int row = pos.x;
        int col = pos.y;
        if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) {
            CircleShape highlightBox(9, 300);
            highlightBox.setPosition(col * TILE_SIZE + 30, row * TILE_SIZE + 30);
            highlightBox.setFillColor(highlightColor);
            window.draw(highlightBox);
        }
    }
}
void kingInCheck(ChessBoard* board[][8], PieceColor currentPlayer, bool& checkmate, bool& stalemate, bool& check) {
    std::vector<Vector2i> boardMoves;
    Vector2i kingPos(-1, -1);

    //Find the king's position
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (board[i][j] != nullptr && board[i][j]->getPiece() == Piece::King && board[i][j]->getPieceColor() == currentPlayer) {
                    kingPos = Vector2i(i, j);
                    break;
                }
            }
            if (kingPos.x != -1 && kingPos.y != -1) break;
        }

    bool isChecked = false;
    if (kingPos.x != -1 && kingPos.y != -1) { // Ensure king position was found
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (board[i][j] != nullptr && board[i][j]->getPieceColor() != currentPlayer) {
                    std::vector<Vector2i> moves = board[i][j]->getLegalMoves(Vector2i(i, j), board);
                    for (const auto& move : moves) {
                        if (move.x == kingPos.x && move.y == kingPos.y) {
                            isChecked = true;
                            break;
                        }
                    }
                }
                if (isChecked) break;
            }
            if (isChecked) break;
        }
    }

    //Update the checked status for pieces
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (board[i][j] != nullptr && board[i][j]->getPieceColor() == currentPlayer) {
                    board[i][j]->hunYouChecked = isChecked;
                }
            }
        }

    if (isChecked) {
        check = true;

        //Check for checkmate
            bool hasLegalMoves = false;
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (board[i][j] != nullptr && board[i][j]->getPieceColor() == currentPlayer) {
                    std::vector<Vector2i> moves = board[i][j]->getLegalMoves(Vector2i(i, j), board);
                    if (!moves.empty()) {
                        hasLegalMoves = true;
                        break;
                    }
                }
            }
            if (hasLegalMoves) break;
        }
        if (!hasLegalMoves) {
            checkmate = true;
        }
    }
    else {
        //Check for stalemate
            bool hasLegalMoves = false;
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (board[i][j] != nullptr && board[i][j]->getPieceColor() == currentPlayer) {
                    std::vector<Vector2i> moves = board[i][j]->getLegalMoves(Vector2i(i, j), board);
                    if (!moves.empty()) {
                        hasLegalMoves = true;
                        break;
                    }
                }
            }
            if (hasLegalMoves) break;
        }
        if (!hasLegalMoves) {
            stalemate = true;
        }
    }
}

void renderPromotionBox(RenderWindow& window, Vector2f position, PieceColor color, PieceSet pieceSet) {
    RectangleShape shadow(Vector2f(80 * 8, 80 * 8));
    shadow.setFillColor(Color(40, 36, 33, 190));
    window.draw(shadow);

    RectangleShape promotionBox(Vector2f(260, 80));
    promotionBox.setPosition(position.x, position.y);
    promotionBox.setFillColor(Color(31, 99, 40));

    window.draw(promotionBox);

    Texture knightTexture, rookTexture, bishopTexture, queenTexture;
    if (pieceSet == PieceSet::Cburnett) {
        if (color == PieceColor::Black) {
            knightTexture.loadFromFile("external/pieces/cburnett/bN.png");
            rookTexture.loadFromFile("external/pieces/cburnett/bR.png");
            bishopTexture.loadFromFile("external/pieces/cburnett/bB.png");
            queenTexture.loadFromFile("external/pieces/cburnett/bQ.png");
        }
        else {
            knightTexture.loadFromFile("external/pieces/cburnett/wN.png");
            rookTexture.loadFromFile("external/pieces/cburnett/wR.png");
            bishopTexture.loadFromFile("external/pieces/cburnett/wB.png");
            queenTexture.loadFromFile("external/pieces/cburnett/wQ.png");
        }
    }
    else if (pieceSet == PieceSet::Alpha) {
        if (color == PieceColor::Black) {
            knightTexture.loadFromFile("external/pieces/alpha/bN.png");
            rookTexture.loadFromFile("external/pieces/alpha/bR.png");
            bishopTexture.loadFromFile("external/pieces/alpha/bB.png");
            queenTexture.loadFromFile("external/pieces/alpha/bQ.png");
        }
        else {
            knightTexture.loadFromFile("external/pieces/alpha/wN.png");
            rookTexture.loadFromFile("external/pieces/alpha/wR.png");
            bishopTexture.loadFromFile("external/pieces/alpha/wB.png");
            queenTexture.loadFromFile("external/pieces/alpha/wQ.png");
        }
    }
    else if (pieceSet == PieceSet::Merida) {
        if (color == PieceColor::Black) {
            knightTexture.loadFromFile("external/pieces/merida/bN.png");
            rookTexture.loadFromFile("external/pieces/merida/bR.png");
            bishopTexture.loadFromFile("external/pieces/merida/bB.png");
            queenTexture.loadFromFile("external/pieces/merida/bQ.png");
        }
        else {
            knightTexture.loadFromFile("external/pieces/merida/wN.png");
            rookTexture.loadFromFile("external/pieces/merida/wR.png");
            bishopTexture.loadFromFile("external/pieces/merida/wB.png");
            queenTexture.loadFromFile("external/pieces/merida/wQ.png");
        }
    }
    else if (pieceSet == PieceSet::Kosal) {
        if (color == PieceColor::Black) {
            knightTexture.loadFromFile("external/pieces/kosal/bN.png");
            rookTexture.loadFromFile("external/pieces/kosal/bR.png");
            bishopTexture.loadFromFile("external/pieces/kosal/bB.png");
            queenTexture.loadFromFile("external/pieces/aplha/bQ.png");
        }
        else {
            knightTexture.loadFromFile("external/pieces/kosal/wN.png");
            rookTexture.loadFromFile("external/pieces/kosal/wR.png");
            bishopTexture.loadFromFile("external/pieces/kosal/wB.png");
            queenTexture.loadFromFile("external/pieces/kosal/wQ.png");
        }
    }

    Sprite knight(knightTexture), rook(rookTexture), bishop(bishopTexture), queen(queenTexture);
    knight.setPosition(promotionBox.getPosition().x, position.y);
    rook.setPosition(knight.getPosition().x + 60, position.y);
    bishop.setPosition(rook.getPosition().x + 60, position.y);
    queen.setPosition(bishop.getPosition().x + 60, position.y);
    
    window.draw(knight);
    window.draw(rook);
    window.draw(bishop);
    window.draw(queen);

    window.display();
}
Piece handlePawnPromotion(RenderWindow& window, PieceColor color, int newX, int newY, PieceSet pieceSet, ChessBoard* board[BOARD_SIZE][BOARD_SIZE]) {
    renderPromotionBox(window, Vector2f(newY * TILE_SIZE, newX * TILE_SIZE), color, pieceSet);
    Piece promotedPieceType = Piece::Queen;
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::MouseButtonReleased) {
                if (event.mouseButton.button == Mouse::Left) {
                    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

                    if (mousePos.y >= newX * TILE_SIZE && mousePos.y <= (newX + 1) * TILE_SIZE) {
                        int deltaX = mousePos.x - newY * TILE_SIZE;
                        if (deltaX >= 0 && deltaX <= 60) {
                            return Piece::Knight;
                        }
                        else if (deltaX > 60 && deltaX <= 2 * 60) {
                            return Piece::Rook;
                        }
                        else if (deltaX > 2 * 60 && deltaX <= 3 * 60) {
                            return Piece::Bishop;
                        }
                        else if (deltaX > 3 * 60 && deltaX <= 4 * 60) {
                            return Piece::Queen;
                        }
                    }
                }
            }
            else if (event.type == Event::Closed) {
                window.close();
                return promotedPieceType;
            }
        }
    }
    return promotedPieceType;
}   