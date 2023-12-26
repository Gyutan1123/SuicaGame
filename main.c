#include <stdio.h>
#include <stdlib.h>
#define BOARD_SIZE 5
#define MAXPOINTNUM 25
#define TRUE 1
#define FALSE 0
typedef struct point
{
    int x; // x座標
    int y; // y座標
    int v; // 進化度
} Point;

typedef struct board
{
    char field[BOARD_SIZE][BOARD_SIZE + 1];
} Board;

void print_board(Board b, Point *Points, int PointNum)
{
    for (int i = 0; i < PointNum; i++)
    {
        Point p = Points[i];
        b.field[p.y][p.x] = p.v + '0';
    }
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        printf("%s\r\n", b.field[i]);
    }
}

int main(int argc, char *argv[])
{

    Board b = {.field = {
                   "-----",
                   "-----",
                   "-----",
                   "-----",
                   "-----"}};

    Point Points[BOARD_SIZE * BOARD_SIZE];
    Point p = {.x = 0, .y = 0, .v = 1}; // 現在操作しているオブジェクト
    Points[0] = p;
    int NowPoints = 1;

    int c = '_';                   // Dummy
    system("/bin/stty raw onlcr"); // enterを押さなくてもキー入力を受け付けるようになる
    int DropFlag = FALSE;
    while (1)
    {
        system("clear");
        printf("Press '.' to close\r\n");
        printf("You pressed '%c'\r\n", c);

        if (DropFlag)
        {
            p.x = 0;
            p.y = 0;
            p.v = 1;
            Points[NowPoints] = p;
            NowPoints++;
            DropFlag = FALSE;
        }
        else
        {
            if (c == 'd' && (p.x < BOARD_SIZE - 1))
            {
                p.x++;
                Points[NowPoints - 1] = p;
            }
            if (c == 'a' && (p.x > 0))
            {
                p.x--;
                Points[NowPoints - 1] = p;
            }
            if (c == 's')
            {
                p.y = BOARD_SIZE - 1;
                Points[NowPoints - 1] = p;
                DropFlag = TRUE;
            }
        }

        print_board(b, Points, NowPoints);

        if ((c = getchar()) == '.')
        { // '.' を押すと抜ける
            break;
        }
    }

    system("/bin/stty cooked"); // 後始末

    return 0;
}