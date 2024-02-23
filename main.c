#include <time.h>
#include "raylib.h"

#define ROWS 3
#define COLS 3
#define CELLSIZE 200

#define DARK_PURPULE CLITERAL(Color){ 15, 2, 52 , 255}
#define LIGHT_PURPULE CLITERAL(Color){185, 22, 132, 255}
#define TRANSPARENT_PURPULE CLITERAL(Color) { 15, 2, 52, 200}

typedef enum {
    EMPTY = 0,
    X,
    O
} CellValue;

typedef struct {
    int i;
    int j;
    CellValue value;
} Cell;


typedef struct {
    Vector2 position;
    Vector2 size;
    Color color;
} Rect;

typedef enum {
    PLAYERX,
    PLAYERO,
    END
} GameState;

GameState state;
const int screenWidth = 1000;
const int screenHeight = 900;
static int seed_set = 0;

Texture2D background;
Texture2D cross;
Texture2D circle;

Cell grid[ROWS][COLS];
static Rect InnerBox = {(Vector2){200, 200}, (Vector2){600, 600}, LIGHT_PURPULE};
CellValue winner;

void UpdateDrawFrame(void);
void drawGame(void);
void InitGame(void);
void GridInit(void);
int PointInRect(Vector2 pt, Rect rect);
int CellMark(int i, int j, CellValue value);
void CellDraw(Cell cell);
void DrawLineWithStroke(int startPosX, int startPosY, int endPosX, int endPosY, int lineWidth, Color lineColor);
int IsGameover(CellValue *w);
void drawGrid(void);
int randint(int a, int b);

int main(void)
{
    InitGame();
    InitWindow(screenWidth, screenHeight, "Tic Tac Toe");
    SetTargetFPS(60);
    background = LoadTexture("resources/textures/background.png");
    circle = LoadTexture("resources/textures/circle.png");
    cross  = LoadTexture("resources/textures/cross.png");
    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }

    UnloadTexture(background);
    UnloadTexture(circle);
    UnloadTexture(cross);
    CloseWindow();
    return 0;
}

void UpdateDrawFrame(void)
{
    int game_over = IsGameover(&winner);
    char *text;
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && !game_over)
    {
        Vector2 pos = GetMousePosition();
        if(PointInRect(pos, InnerBox))
        {
            int j = (pos.x - InnerBox.position.x) / InnerBox.position.x;
            int i = (pos.y - InnerBox.position.y) / InnerBox.position.y;
            if (CellMark(i, j, state == PLAYERO ? O : X))
                state = (state == PLAYERX) ? PLAYERO : PLAYERX; 
        }
    }
    else if(game_over) state = END;
    if (game_over && IsKeyPressed(KEY_R))
        InitGame();
    BeginDrawing();
        drawGrid();
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < ROWS; j++)
                CellDraw(grid[i][j]);
        if (state == END)
        {
            //DrawRectangle(int posX, int posY, int width, int height, Color color)
            DrawRectangle(0, 0, screenWidth, screenHeight, TRANSPARENT_PURPULE);
            if (winner == EMPTY)
                // DrawText(const char *text, int posX, int posY, int fontSize, Color color);  
                text = "It's A Draw!";
            else
                text = (winner == X) ? "Player X is The winner!" : "Player O Is the winner";
            DrawText(text, 150, 300, 60, WHITE);
            DrawText("Press r to restart", 300, 400, 40, WHITE);
        }
        else {
            if (state == PLAYERX)
                DrawText("PLAYER X GO!", 500, 820, 40, WHITE);
            else
                DrawText("PLAYER O GO!", 500, 820, 40, WHITE);
        }
    EndDrawing();
}

void drawGrid(void)
{
    int x = InnerBox.position.x;
    int y = InnerBox.position.y;
    int w = InnerBox.size.x;
    int h = InnerBox.size.y;   

    ClearBackground(WHITE);
    DrawTexture(background,0, 0, WHITE);
    DrawText("Tic Tac Toe", 340, 40, 50, WHITE);
    DrawLineWithStroke(340, 90, 660, 90, 4, LIGHT_PURPULE);
    DrawRectangleV(InnerBox.position, InnerBox.size, InnerBox.color);

    DrawLineWithStroke(x + CELLSIZE, y, x + CELLSIZE, h + y, 4, DARK_PURPULE);
    DrawLineWithStroke(x + (CELLSIZE * 2), y, x + (CELLSIZE * 2), h + y, 4, DARK_PURPULE);

    DrawLineWithStroke(x, (y + CELLSIZE), (x + w), (y + CELLSIZE), 4, DARK_PURPULE);
    DrawLineWithStroke(x, (CELLSIZE * 2) + y, (x + w), (CELLSIZE * 2) + y, 4, DARK_PURPULE);
}

void GridInit(void)
{
	for (int i = 0; i < COLS; i++)
	{
		for (int j = 0; j < ROWS; j++)
		{
			grid[i][j] = (Cell)
			{
				.i = i,
				.j = j,
				.value = EMPTY
			};
		}
	}
}

void InitGame(void)
{
    state = randint(0, 100) >= 50 ? PLAYERO : PLAYERX ;
    GridInit();
    winner = EMPTY;
}

int PointInRect(Vector2 pt, Rect rect)
{
    return pt.x >= rect.position.x && pt.x < rect.position.x + rect.size.x &&
           pt.y >= rect.position.y && pt.y < rect.position.y + rect.size.y;
}

int CellMark(int i, int j, CellValue value)
{
    if (grid[i][j].value != EMPTY || value == EMPTY)
        return 0;

    grid[i][j].value = value;
    return 1;
}

void CellDraw(Cell cell)
{
    Vector2 pos = (Vector2){InnerBox.position.x + (cell.j * InnerBox.position.x), InnerBox.position.y + (cell.i * InnerBox.position.y)};
    switch(cell.value)
    {
        case O:
            DrawTexture(circle, pos.x, pos.y, WHITE);
            break;
        case X:
            DrawTexture(cross, pos.x, pos.y, WHITE);
            break;
        default:
            break;
    }
}

void DrawLineWithStroke(int startPosX, int startPosY, int endPosX, int endPosY, int lineWidth, Color lineColor)
{
    DrawLine(startPosX, startPosY, endPosX, endPosY, lineColor);

    for (int i = 1; i <= lineWidth; i++)
    {
        DrawLine(startPosX + i, startPosY, endPosX + i, endPosY, lineColor );
        DrawLine(startPosX - i, startPosY, endPosX - i, endPosY, lineColor);
        DrawLine(startPosX, startPosY + i, endPosX, endPosY + i, lineColor);
        DrawLine(startPosX, startPosY - i, endPosX, endPosY - i, lineColor);
    }
}

int IsGameover(CellValue *w) 
{
    for (int i = 0; i < ROWS; ++i) {
        if ((grid[i][0].value != EMPTY && grid[i][0].value == grid[i][1].value && grid[i][1].value == grid[i][2].value) ||
            (grid[0][i].value != EMPTY && grid[0][i].value == grid[1][i].value && grid[1][i].value == grid[2][i].value)) {
            *w = grid[i][0].value;
            return 1;
        }
    }
    
    if ((grid[0][0].value != EMPTY && grid[1][1].value == grid[0][0].value && grid[2][2].value == grid[1][1].value) ||
        (grid[0][2].value != EMPTY && grid[1][1].value == grid[0][2].value && grid[2][0].value == grid[0][2].value)) {
        *w = grid[1][1].value;
        return 1;
    }
    
    // Check for a draw
    int filledCells = 0;
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (grid[i][j].value != EMPTY)
                filledCells++;
        }
    }
    if (filledCells == ROWS * COLS) {
        *w = EMPTY;
        return 1;
    }
    
    *w = EMPTY;
    return 0;
}

int randint(int a, int b)
{
    if (!seed_set) {
        srand(time(NULL));
        seed_set = 1;
    }

    return a + rand() % (b - a + 1);
}
