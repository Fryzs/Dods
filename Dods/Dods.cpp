#include <iostream>
#include <vector>
#include <windows.h>
using namespace std;

int matrix = 10;
vector<int> Dx = { 1, 0, -1, 0 };
vector<int> Dy = { 0, 1, 0, -1 };
vector<vector<int>> GameArea(matrix, vector<int>(matrix, 0));
vector<vector<bool>> Data(matrix, vector<bool>(matrix, true));
vector<vector<int>> CordDots;
int EnemyPlayer = 2;
int Player = 1;
int ScoreFPl = 0;
int ScoreSPl = 0;
int PlX, PlY;

// Перевірка меж
bool isValid(int x, int y, int matrix) {
    return x >= 0 && x < matrix && y >= 0 && y < matrix;
}

// Повторне заповнення
void FillTrue(vector<vector<bool>>& Data) {
    for (int i = 0; i < matrix; i++) {
        for (int j = 0; j < matrix; j++) {
            Data[i][j] = true;
        }
    }
}
//Очищення консолі
void clearConsole() {
    system("cls");
}
// Перевіра чи точка оточена з 4 сторін
bool IsRomb(int x, int y, const vector<vector<int>>& GameArea, int Player) {
    int HowOne = 0;
    for (int i = 0; i < 4; i++) {
        int nx = x + Dx[i];
        int ny = y + Dy[i];
        if (isValid(nx, ny, matrix) && GameArea[nx][ny] == Player) {
            HowOne++;
        }
    }
    return HowOne == 4;
}

// Основна функція для перевірки замкнутої фігури
bool IsValidFigureBig(int x, int y, vector<vector<bool>>& Data, const vector<vector<int>>& GameArea, int Player) {
    int HowOne = 0, HowFalse = 0;
    bool Check = false;

    if (!isValid(x, y, matrix) || !Data[x][y]) {
        return false;
    }

    // Перевіряємо 4 сусіди
    for (int i = 0; i < 4; i++) {
        int nx = x + Dx[i];
        int ny = y + Dy[i];
        if (isValid(nx, ny, matrix)) {
            if (GameArea[nx][ny] != Player) {
                for (int j = 0; j < 4; j++) {
                    int nnx = nx + Dx[j];
                    int nny = ny + Dy[j];
                    if (isValid(nnx, nny, matrix) && GameArea[nnx][nny] == Player) {
                        HowOne++;
                    }
                }if (HowOne != 0) {
                    if (HowOne != 3 && Data[nx][ny]) {
                        Data[nx][ny] = false;
                        IsValidFigureBig(nx, ny, Data, GameArea, Player);
                    }
                    else {
                        HowFalse++;
                    }
                    if (HowFalse >= 2) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
//Коли тільки 2 точки
bool IsValidFigureSmall(int x, int y,  vector<vector<int>>& GameArea, int Player, int EnemyPlayer) {
    int HowPlayerOne = 0;
    int HowPlayerSec = 0;
    for (int i = 0; i < 4; i++) {
        int nx = x + Dx[i];
        int ny = y + Dy[i];
        if (isValid(nx, ny, matrix)) {

            if (GameArea[nx][ny] == Player) {
                HowPlayerOne++;
            }
            if (GameArea[nx][ny] == EnemyPlayer) {
                for (int j = 0; j < 4; j++) {
                    int nnx = nx + Dx[j];
                    int nny = ny + Dy[j];
                    if (isValid(nnx, nny, matrix)) {
                        if (GameArea[nnx][nny] == Player) {
                            HowPlayerSec++;
                        }
                    }
                }
            }

        }
       
    }
    return (HowPlayerOne == 3 && HowPlayerSec == 3);
}
// Рахуємо кількість точок в фігурі
int CountTwosInsideFigure(int x, int y, vector<vector<bool>>& Data, const vector<vector<int>>& GameArea, int Player, int EnemyPlayer) {

    // Якщо точка поза межами або не є частиною фігури (0), або вже відвідана
    if (!isValid(x, y, matrix) || GameArea[x][y] == EnemyPlayer || Data[x][y]) {
        return 0;
    }

    // Відмічаємо клітинку як відвідану
    Data[x][y] = true;

    int count = 0;

    // Якщо клітинка має значення "2", додаємо до лічильника
    if (GameArea[x][y] == Player) {
        count++;
    }

    // Обходимо тільки сусідів, які є частиною фігури (мають значення "1" або "2")
    for (int i = 0; i < 4; i++) {
        int nx = x + Dx[i];
        int ny = y + Dy[i];

        if (isValid(nx, ny, matrix) && (GameArea[nx][ny] == 0 || GameArea[nx][ny] == Player)) {
            count += CountTwosInsideFigure(nx, ny, Data, GameArea, EnemyPlayer, Player);
        }
    }
    FillTrue(Data);
    return count;
}

// Ставимо точку
void Step(vector<vector<int>>& GameArea, int Player) {
    int x, y;
    cout << "(" << Player << ") Enter your X coordinate: ";
    cin >> x;
    PlX = x;
    cout << "(" << Player << ") Enter your Y coordinate: ";
    cin >> y;
    PlY = y;
    if (isValid(PlX, PlY, matrix) && GameArea[PlX][PlY] == 0) {
        GameArea[PlX][PlY] = Player;
    }
    else {
        cout << "Invalid move. Please try again." << endl;
        Step(GameArea, Player);
    }
}

// Пошук точок противника поруч
vector<vector<int>> Serch(int x, int y, int EnemyPlayer) {
    vector<vector<int>> result(4, vector<int>(2, -1));
    int Cordenats = 0;
    for (int i = 0; i < 4; i++) {
        int nx = x + Dx[i];
        int ny = y + Dy[i];
        if (isValid(nx, ny, matrix) && GameArea[nx][ny] == EnemyPlayer) {
            result[Cordenats][0] = nx;
            result[Cordenats][1] = ny;
            Cordenats++;
        }
    }
    return result;
}

// Вивод масиву
void DisplayGameArea(const vector<vector<int>> GameArea) {
    cout << "(1)Score:  " << ScoreFPl << endl;
    cout << "(2)Score:  " << ScoreSPl << endl;
    cout << "    ";
    for (int i = 0; i < matrix; i++) {
        cout << "x ";
    }

    cout << endl;
    cout << "    ";
    for (int i = 0; i < matrix; i++) {
        cout << i << " ";
    }
    cout << endl;
    cout << "   ";
    for (int i = 0; i < matrix; i++) {
        cout << " _";
    }
    cout << endl;

    for (int i = 0; i < matrix; i++) {
        cout << "y" << i << " |";
        for (int j = 0; j < matrix; j++) {
            cout << GameArea[j][i] << " ";
        }
        cout << endl;
    }
}

int GameCore(int x, int y) {
    if (IsRomb(x, y, GameArea, Player)) {
        return 1;
    }
    else if (IsValidFigureSmall(x, y, GameArea, Player, EnemyPlayer)) {
        return 2;
    }
    else if (IsValidFigureBig(x, y, Data, GameArea, Player)) {
        FillTrue(Data);
        return CountTwosInsideFigure(x, y, Data, GameArea, Player, EnemyPlayer);
    }
    
    else  
    FillTrue(Data);
    return 0; 


}

void  StepGame(int &Player, int &EnemyPlayer) {
    while (!(GetAsyncKeyState(VK_F3) & 0x8000)) {
        clearConsole();
        DisplayGameArea(GameArea);
        Step(GameArea, Player);
        CordDots = Serch(PlX, PlY, EnemyPlayer);
        for (int i = 0; i < 4; i++) {
            if (CordDots[i][0] >= 0) {
                ScoreFPl += GameCore(CordDots[i][0], CordDots[i][1]);
            }
        }
        Player = 2;
        EnemyPlayer = 1;
        ScoreSPl += GameCore(PlX, PlY);

        clearConsole();
        DisplayGameArea(GameArea);
        Step(GameArea, Player);
        CordDots = Serch(PlX, PlY, EnemyPlayer);
        for (int i = 0; i < 4; i++) {
            if (CordDots[i][0] >= 0) {
                ScoreSPl += GameCore(CordDots[i][0], CordDots[i][1]);
            }
        }
        Player = 1;
        EnemyPlayer = 2;
        ScoreFPl += GameCore(PlX, PlY);
    }
 

}

void Menu() {
    int choice;
    do {
        cout << "              Dots Game\n";
        cout << "1. Start\n";
        cout << "Enter your choice: ";
        cin >> choice;
        if (choice == 1) StepGame(Player, EnemyPlayer);
    } while (choice != 0);
}

int main() {
    Menu();
    return 0;
}
