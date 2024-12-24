#include <iostream>
#include <vector>
#include <conio.h> // Pentru a captura tastele
#include <windows.h>
#include <ctime>
#include<algorithm>

using namespace std;

// Dimensiunea hărții
const int HEIGHT = 20;
const int WIDTH = 30;

// Simboluri pentru joc
const char WALL = '#';
const char EMPTY = ' ';
const char PLAYER = 'W';
const char PROJECTILE = '^';
const char ENEMY = '@';

// Structuri pentru obiecte din joc
struct Position {
    int x, y;
};

// Configurări joc
vector<Position> projectiles;
vector<Position> enemies;
char map[HEIGHT][WIDTH];
Position player = {WIDTH / 2, HEIGHT - 2};
bool gameOver = false;
int lives = 3;
int score = 0;

void setCursorPosition(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(hConsole, position);
}

// Funcție pentru resetarea hărții
void resetMap() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (y == 0 || y == HEIGHT - 1 || x == 0 || x == WIDTH - 1) {
                map[y][x] = WALL;
            } else {
                map[y][x] = EMPTY;
            }
        }
    }
    map[player.y][player.x] = PLAYER;

    for (auto &p : projectiles) {
        map[p.y][p.x] = PROJECTILE;
    }

    for (auto &e : enemies) {
        map[e.y][e.x] = ENEMY;
    }
}

// Funcție pentru afișarea hărții
void displayMap() {
    setCursorPosition(0, 0); // Repoziționăm cursorul la începutul ecranului
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            cout << map[y][x];
        }
        cout << endl;
    }
    cout << "Score: " << score << "  Lives: " << lives << endl;
}


// Funcție pentru mișcarea jucătorului
void movePlayer(int dx) {
    int newX = player.x + dx;
    if (newX > 0 && newX < WIDTH - 1 && map[player.y][newX] == EMPTY) {
        map[player.y][player.x] = EMPTY; // Ștergem poziția curentă
        player.x = newX;                // Actualizăm poziția jucătorului
        map[player.y][player.x] = PLAYER; // Setăm noua poziție
    }
}


// Funcție pentru tragerea proiectilelor
void shoot() {
    projectiles.push_back({player.x, player.y - 1});
}

// Funcție pentru actualizarea proiectilelor
void updateProjectiles() {
    vector<Position> newProjectiles;
    for (auto &p : projectiles) {
        map[p.y][p.x] = EMPTY;
        p.y--;
        if (p.y > 0) {
            if (map[p.y][p.x] == ENEMY) {
                map[p.y][p.x] = EMPTY;
                score += 10;
                enemies.erase(remove_if(enemies.begin(), enemies.end(), [&](Position e) {
                    return e.x == p.x && e.y == p.y;
                }), enemies.end());
            } else {
                newProjectiles.push_back(p);
            }
        }
    }
    projectiles = newProjectiles;
}

// Funcție pentru generarea inamicilor
void spawnEnemies() {
    if (rand() % 10 < 3) { // Aproximativ 30% șanse de a genera un inamic
        int x = 1 + rand() % (WIDTH - 2);
        enemies.push_back({x, 1});
    }
}

// Funcție pentru actualizarea inamicilor
void updateEnemies() {
    vector<Position> newEnemies;
    for (auto &e : enemies) {
        map[e.y][e.x] = EMPTY;
        e.y++;
        if (e.y == player.y && e.x == player.x) {
            lives--;
            if (lives <= 0) {
                gameOver = true;
            }
        } else if (e.y < HEIGHT - 1) {
            newEnemies.push_back(e);
        }
    }
    enemies = newEnemies;
}

// Logica principală a jocului
void gameLoop() {
    while (!gameOver) {
        if (_kbhit()) {
            switch (_getch()) {
                case 'a':
                    movePlayer(-1);
                    break;
                case 'd':
                    movePlayer(1);
                    break;
                case ' ':
                    shoot();
                    break;
            }
        }

        updateProjectiles();
        updateEnemies();
        spawnEnemies();
        resetMap();
        displayMap();

        Sleep(100);
    }

    system("cls");
    cout << "Game Over! Your score: " << score << endl;
}

int main() {
    srand(static_cast<unsigned>(time(0)));
    resetMap();
    displayMap();
    gameLoop();
    return 0;
}
