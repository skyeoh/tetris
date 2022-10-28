#include <iostream>
#include <thread>
#include <ncurses.h>

using namespace std;

wstring tetromino[7];

int nFieldWidth = 12;
int nFieldHeight = 18;
unsigned char* pField = nullptr;

int nScreenWidth = 80;              // console screen size x (columns)
int nScreenHeight = 30;             // console screen size y (rows)
wchar_t* pScreen = nullptr;

int rotatePiece(int px, int py, int r)
{
    switch (r % 4)
    {
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
                int pi = rotatePiece(px, py, nRotation);

                // Get index into field
                int fi = (nPosY + py) * nFieldWidth + (nPosX + px);

                if (nPosX + px >= 0 && nPosX + px < nFieldWidth)
                    if (nPosY + py >= 0 && nPosY + py < nFieldHeight)
                        if (tetromino[nTetromino][pi] == L'X' && pField[fi] != 0)
                            return false;   // fail on first hit
        }

    return true;
}

void drawCurrentPieceOnScreen(int nCurrentPiece, int nCurrentRotation, int nCurrentX, int nCurrentY, int nScreenWidth, wchar_t* pScreen)
{
    for (int px = 0; px < 4; px++)
        for (int py = 0; py < 4; py++)
            if (tetromino[nCurrentPiece][rotatePiece(px, py, nCurrentRotation)] == L'X')
                pScreen[(nCurrentY + py + 2) * nScreenWidth + (nCurrentX + px + 2)] = L" ABCDEFG=#"[nCurrentPiece + 1];
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

void displayScreen(int nScreenWidth, int nScreenHeight, wchar_t* pScreen, WINDOW* window)
{
    for (int x = 0; x < nScreenWidth; x++)
        for (int y = 0; y < nScreenHeight; y++)
            mvwaddch(window, y, x, pScreen[y * nScreenWidth + x]);

    wrefresh(window);
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

    // Initialize display
    initscr();
    refresh();

    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    noecho();

    int xMax, yMax;
    getmaxyx(stdscr, yMax, xMax);
    WINDOW* window = newwin(nScreenHeight, nScreenWidth, 0.5 * (yMax - nScreenHeight), 0.5 * (xMax - nScreenWidth));

    // Game loop
    bool bGameOver = false;

    int nCurrentPiece = 0;
    int nCurrentRotation = 0;
    int nCurrentX = nFieldWidth / 2;
    int nCurrentY = 0;

    int bKey;

    while (!bGameOver)
    {
            // GAME TIMING ====================================================
            this_thread::sleep_for(50ms);

            // USER INPUT =====================================================
            bKey = getch();

            // GAME LOGIC =====================================================
            switch (bKey)
            {
                case KEY_LEFT:
                    if (doesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) nCurrentX--;
                    break;
                case KEY_RIGHT:
                    if (doesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) nCurrentX++;
                    break;
                case KEY_DOWN:
                    if (doesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) nCurrentY++;
                    break;
                case KEY_UP:
                    if (doesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) nCurrentRotation++;
                    break;
            }

            // RENDER OUTPUT ==================================================

            // Draw field
            drawPlayingFieldOnScreen(nFieldWidth, nFieldHeight, nScreenWidth, pScreen, pField);

            // Draw current piece
            drawCurrentPieceOnScreen(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY, nScreenWidth, pScreen);

            // Display frame
            displayScreen(nScreenWidth, nScreenHeight, pScreen, window);
    }

    // Clean up
    endwin();
    delete [] pField;
    delete [] pScreen;

    return 0;
}
