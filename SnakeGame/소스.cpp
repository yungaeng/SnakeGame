#include <windows.h>
#include <vector>
const int boardSize = 8;
const int cellSize = 80;

class Piece
{
public:
    short pieceX, pieceY;
    bool isRed;
public:
    Piece() : pieceX(0), pieceY(0), isRed(true) {}
    Piece(int x, int y, bool isred) : pieceX(x), pieceY(y), isRed(isred) {}

    void Draw(HDC hdc) const
    {
        HBRUSH brush = CreateSolidBrush(isRed ? RGB(255, 0, 0) : RGB(0, 0, 255));
        RECT rect = { pieceX * cellSize + 20, pieceY * cellSize + 20, (pieceX + 1) * cellSize - 20, (pieceY + 1) * cellSize - 20 };
        FillRect(hdc, &rect, brush);
        DeleteObject(brush);
    }
    void Move(short x, short y)
    {
        pieceX = x;
        pieceY = y;
    }

private:
    void BackGround();
};
class Board
{
public:
    std::vector <Piece> pieces;
    int turn = 1;

public:
    Board() {
        Piece p(1, 1, 1);
        pieces.emplace_back(p);
    };

    void Draw(HDC hdc) {
        for (int row = 0; row < boardSize; ++row) {
            for (int col = 0; col < boardSize; ++col) {
                HBRUSH brush = CreateSolidBrush((row + col) % 2 == 0 ? RGB(255, 255, 255) : RGB(0, 0, 0));
                RECT rect = { col * cellSize, row * cellSize, (col + 1) * cellSize, (row + 1) * cellSize };
                FillRect(hdc, &rect, brush);
                DeleteObject(brush);
            }
        }

        for (auto& p : pieces)
            p.Draw(hdc);
    }
};