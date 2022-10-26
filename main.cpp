#include <iostream>
using namespace std;

wstring tetromino[7];

int nFieldWidth = 12;
int nFieldHeight = 18;
unsigned char* pField = nullptr;

int nScreenWidth = 80;              // console screen size x (columns)
int nScreenHeight = 30;             // console screen size y (rows)
wchar_t* pScreen = nullptr;

int rotate(int px, int py, int r)
{
    switch (r % 4) {
        case 0: return py * 4 + px;         // 0 degrees
        case 1: return 12 + py - (px * 4);  // 90 degrees
        case 2: return 15 - (py * 4) - px;  // 180 degrees
        case 3: return 3 - py + (px * 4);   // 270 degrees
    }

    return 0;
}

bool doesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY)
{
    for (int px = 0; px < 4; px++)
        for (int py = 0; py < 4; py++)
        {
                // Get index into piece
                int pi = rotate(px, py, nRotation);

                // Get index into field
                int fi = (nPosY + py) * nFieldWidth + (nPosX + px);

                if (nPosX + px >= 0 && nPosX + px < nFieldWidth)
                    if (nPosY + py >= 0 && nPosY + py < nFieldHeight)
                        if (tetromino[nTetromino][pi] == L'X' && pField[fi] != 0)
                            return false;   // fail on first hit
        }

    return true;
}

void initializePlayingField(int nFieldWidth, int nFieldHeight, unsigned char* pField)
{
    for (int x = 0; x < nFieldWidth; x++)
        for (int y = 0; y < nFieldHeight; y++)
            pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0; 
}

void drawPlayingFieldOnScreen(int nFieldWidth, int nFieldHeight, int nScreenWidth, wchar_t* pScreen, unsigned char* pField)
{
    for (int x = 0; x < nFieldWidth; x++)
        for (int y = 0; y < nFieldHeight; y++)
            pScreen[(y + 2) * nScreenWidth + (x + 2)] = L" ABCDEFG=#"[pField[y * nFieldWidth + x]];
}

void initializeScreen(int nScreenWidth, int nScreenHeight, wchar_t* pScreen)
{
    for (int i = 0; i < (nScreenWidth * nScreenHeight); i++) pScreen[i] = L' ';
}

int main()
{
    // Create assets
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");

    tetromino[1].append(L"..X.");
    tetromino[1].append(L".XX.");
    tetromino[1].append(L".X..");
    tetromino[1].append(L"....");

    tetromino[2].append(L".X..");
    tetromino[2].append(L".XX.");
    tetromino[2].append(L"..X.");
    tetromino[2].append(L"....");

    tetromino[3].append(L"....");
    tetromino[3].append(L".XX.");
    tetromino[3].append(L".XX.");
    tetromino[3].append(L"....");

    tetromino[4].append(L"..X.");
    tetromino[4].append(L".XX.");
    tetromino[4].append(L"..X.");
    tetromino[4].append(L"....");

    tetromino[5].append(L"....");
    tetromino[5].append(L".XX.");
    tetromino[5].append(L"..X.");
    tetromino[5].append(L"..X.");

    tetromino[6].append(L"....");
    tetromino[6].append(L".XX.");
    tetromino[6].append(L".X..");
    tetromino[6].append(L".X..");

    // Initialize playing field
    pField = new unsigned char[nFieldWidth * nFieldHeight];
    initializePlayingField(nFieldWidth, nFieldHeight, pField);

    // Initialize screen
    pScreen = new wchar_t[nScreenWidth * nScreenHeight];
    initializeScreen(nScreenWidth, nScreenHeight, pScreen);

    bool bGameOver = false;

    while (!bGameOver)
    {
            // GAME TIMING ====================================================


            // USER INPUT =====================================================


            // GAME LOGIC =====================================================


            // RENDER OUTPUT ==================================================

            // Draw field
            drawPlayingFieldOnScreen(nFieldWidth, nFieldHeight, nScreenWidth, pScreen, pField);

            // Display frame

    }

    delete [] pField;
    delete [] pScreen;

    return 0;
}
