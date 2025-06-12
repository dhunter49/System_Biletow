#include <Windows.h>
#include <conio.h>
#include <iomanip>

// Sets console output text to given color
// ONLY WINDOWS
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) return;
    if (!SetConsoleTextAttribute(hConsole, color)) return;
}

// Gets the console dimensions
// ONLY WINDOWS
void getConsoleDimensions(int& lines, int& columns) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    // Get the handle to the console output
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (hConsole != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        lines = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
    else {
        // Fallback values if the API call fails
        columns = 80;
        lines = 25;
    }
}

// Hides/Shows visibility of text cursor
// true - visible, false - hidden
// ONLY WINDOWS
void setConsoleCursorVisibility(bool visible) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) return;

    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = visible;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

// Clears console screen 
// ONLY WINDOWS
void clearScreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) {
        // Fallback screen value
        system("CLS");
        return;
    }
    COORD coordScreen = { 0, 0 };
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    FillConsoleOutputCharacter(hConsole, ' ', dwConSize, coordScreen, &cCharsWritten);
    SetConsoleCursorPosition(hConsole, coordScreen);
}

// Sets cursor position to (x, y)
// ONLY WINDOWS
void gotoXY(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) 
        throw std::runtime_error("Problem z konsol¹");
    COORD coordScreen = { static_cast<short>(x), static_cast<short>(y) };
    SetConsoleCursorPosition(hConsole, coordScreen);
}

// Waits until ESC key is pressed
void waitForEsc() {
    while (_getch() != 27) {}
}