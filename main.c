#include <stdio.h>
#include "raylib.h"

#define SIZE 3

typedef struct {
    Vector2 position;
    Texture2D texture;
    int flag;
} Box;

typedef enum {
    EMPTY  = 0,
    CROSS  = 1,
    CIRCLE = 2
} flags;



static Box Board[SIZE][SIZE];
const int screenWidth  = 300;
const int screenHeight = 300;
flags currentTurn = CROSS;

void InitGame()
{
    for (int i = 0; i < SIZE; i++)    
    {
        for (int j = 0; j < SIZE; ++j)
        {
            Board[i][j].flag = EMPTY;
            Board[i][j].position = (Vector2){i * (screenHeight / SIZE), j * (screenWidth / SIZE)};
        }
    }
}

void draw()
{
    ClearBackground(RAYWHITE);
    //DrawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color color);
    DrawLine(100, 5, 100, screenHeight - 5, GRAY);
    DrawLine(200, 5, 200, screenHeight - 5, GRAY);

    DrawLine(5, 100, screenWidth - 5, 100, GRAY);
    DrawLine(5, 200, screenWidth - 5, 200, GRAY);
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++) {
            if (Board[i][j].flag != EMPTY) 
                DrawTexture(Board[i][j].texture, Board[i][j].position.x, Board[i][j].position.y, RAYWHITE);
        }

}

void drawWinner(int winner)
{
    char *text = "Tie Game!";
    if (winner == CROSS)
        text = "CROSS WINS!";
    else if (winner == CIRCLE)
        text = "CIRCLE WINS!";
//DrawText(const char *text, int posX, int posY, int fontSize, Color color); 
    DrawText(text, 30, 90, 40, RED);    
}

int CheckWinner(int player)
{
    for (int i = 0; i < SIZE; i++)
        if ((Board[i][0].flag == player && Board[i][1].flag == player && Board[i][2].flag == player) ||
           (Board[0][i].flag == player && Board[1][i].flag == player && Board[2][i].flag == player))
           return 1; 
    if ((Board[0][0].flag == player && Board[1][1].flag == player && Board[2][2].flag == player) || (Board[0][2].flag == player && Board[1][1].flag == player && Board[2][0].flag == player))
        return 1;
    return 0;
}
int Tie(void)
{
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (Board[i][j].flag == EMPTY)
                return 0;
    return 1;
}
int main(void)
{
    InitWindow(screenWidth, screenHeight, "Tic Tac Toe");
    SetTargetFPS(60);
    Texture2D  cross  = LoadTexture("resources/textures/cross.png");
    Texture2D  circle = LoadTexture("resources/textures/circle.png");
    InitGame();

    int game_over = 0;
    flags winner = EMPTY;
    while (!WindowShouldClose())
    {

        // handle user input
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !game_over)
        {
            Vector2 pn = GetMousePosition(); 
            int dx = pn.x / 100;
            int dy = pn.y / 100;
            
            if (Board[dx][dy].flag == EMPTY)
            {
                if(currentTurn == CROSS)
                {
                    Board[dx][dy].texture =  cross;
                    Board[dx][dy].flag = CROSS;
                    currentTurn = CIRCLE;
                }
                else
                {
                    Board[dx][dy].texture = circle;
                    Board[dx][dy].flag = CIRCLE;
                    currentTurn = CROSS;
                }

            } 
       }
       // check if there is a winner! 
       if(CheckWinner(CROSS) && !game_over)
       {
            winner = CROSS;
            game_over = 1; 
       }
       else if (CheckWinner(CIRCLE) && !game_over)
       {
            winner = CIRCLE;
            game_over = 1;
       }
       // Tie
       else if (Tie()) {
          game_over = 1; 
       }
       BeginDrawing();
           draw();
           if (game_over)
           {
               drawWinner(winner);
           }    
       EndDrawing();
    }
    return 0;
}
