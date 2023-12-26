#include <stdio.h>
#include <stdlib.h>
#define BOARD_SIZE 5
#define MAXPOINTNUM 1000
#define TRUE 1
#define FALSE 0
#define RIGHT 2
#define LEFT 3
typedef struct point
{
    int x;       // x座標
    int y;       // y座標
    int v;       // 進化度
    int Survive; // 生きてるか
} Point;

typedef struct board
{
    char field[BOARD_SIZE][BOARD_SIZE + 1];
} Board;

int EvolvePointIndex(Board b, Point p, Point *Points, int PointNum);

void print_board(Board b, Point *Points, int PointNum)
{
    for (int i = 0; i < PointNum; i++)
    {
        Point p = Points[i];
        if (p.Survive)
        {
            b.field[p.y][p.x] = p.v + '0';
        }
    }
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        printf("%s\r\n", b.field[i]);
    }
}

void reset_Board(Board b)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        b.field[i][i] = "-";
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
    Point p = {.x = 0, .y = 0, .v = 1, .Survive = TRUE}; // 現在操作しているオブジェクト
    Points[0] = p;
    int NowPoints = 1;

    int c = '_';                   // Dummy
    system("/bin/stty raw onlcr"); // enterを押さなくてもキー入力を受け付けるようになる
    int DropFlag = FALSE;
    while (1)
    {
        reset_Board(b);
        system("clear");
        printf("Press '.' to close\r\n");
        printf("You pressed '%c'\r\n", c);

        if (DropFlag)
        {
            p.x = 0;
            p.y = 0;
            p.v = 1;
            p.Survive = TRUE;
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

                int Index = EvolvePointIndex(b, p, Points, NowPoints);
                if (Index != -1)
                {
                    p.Survive = FALSE;
                    Points[Index].v++;
                }

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

int EvolvePointIndex(Board b, Point p, Point *Points, int PointNum)
{
    int ret;
    int IsEvolveExist = FALSE;
    for (int i = 0; i < PointNum; i++)
    {
        Point q = Points[i];
        if (!q.Survive)
        {
            break;
        }
        if (q.y == p.y && q.x == p.x - 1) // 右側
        {
            return i;
        }

        if (q.y == p.y && q.x == p.x + 1) // 左側
        {
            IsEvolveExist = TRUE;
            ret = i;
        }
    }
    if (IsEvolveExist)
    {
        return ret;
    }
    else
    {
        return -1;
    }
}