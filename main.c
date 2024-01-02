#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define BOARD_SIZE 5
#define MAXPOINTNUM 1000
#define TRUE 1
#define FALSE 0
#define RIGHT 2
#define LEFT 3

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

typedef struct point {
    int x;        // x座標
    int y;        // y座標
    int v;        // 進化度
    int Survive;  // 生きてるか
} Point;

typedef struct this {
    Point *NowPoint;
    int MainStep;
    int EvolvePointIndex;
}
THIS;

typedef struct board {
    char field[BOARD_SIZE][BOARD_SIZE + 1];
} Board;

int EvolvePointIndex(Board b, Point *p, Point **Points, int PointNum);
int LimitYtoDrop(Point *p, Point **Points, int PointNum);
void print_board(Board b, Point **Points, int PointNum) {
    for (int i = 0; i < PointNum; i++) {
        Point *p = Points[i];
        if (p->Survive) {
            b.field[p->y][p->x] = p->v + '0';
        }
    }
    for (int i = 0; i < BOARD_SIZE; ++i) {
        printf("%s\r\n", b.field[i]);
    }
}

void reset_Board(Board *b) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            b->field[i][j] = '-';
        }
    }
}

enum {
    WaitDrop,
    WaitNextPoint,
    WaitEvolve,
    GameOver,
};

int main(int argc, char *argv[]) {
    THIS *pThis = malloc(sizeof(THIS));

    pThis->MainStep = WaitDrop;
    Board b;
    reset_Board(&b);
    Point *Points[MAXPOINTNUM];
    for (int i = 0; i < MAXPOINTNUM; i++) {
        Points[i] = (Point *)malloc(sizeof(Point));
    }
    Point p = {
        .x = 0,
        .y = 0,
        .v = 1,
        .Survive = TRUE,
    };  // 現在操作しているオブジェクト
    Points[0]->x = 0;
    Points[0]->y = 0;
    Points[0]->v = 1;
    Points[0]->Survive = TRUE;
    Point *q;
    pThis->NowPoint = Points[0];
    int NowPointNum = 1;

    int c = '_';  // Dummy
    system(
        "/bin/stty raw onlcr");  // enterを押さなくてもキー入力を受け付けるようになる
    int DropFlag = FALSE;
    while (1) {
        reset_Board(&b);
        system("clear");
        printf("Press '.' to close\r\n");
        printf("You pressed '%c'\r\n", c);

        switch (pThis->MainStep) {
            case WaitDrop:
                if (c == 'd' && (pThis->NowPoint->x < BOARD_SIZE - 1)) {
                    pThis->NowPoint->x++;
                }
                if (c == 'a' && (pThis->NowPoint->x > 0)) {
                    pThis->NowPoint->x--;
                }
                if (c == 's') {
                    pThis->NowPoint->y =
                        LimitYtoDrop(pThis->NowPoint, Points, NowPointNum);
                    if (pThis->NowPoint->y < 0) {
                        pThis->MainStep = GameOver;
                        system("clear");
                        printf("GameOver\r\n");
                        printf("Press '.' to close\r\n");
                        break;
                    }
                    pThis->EvolvePointIndex = EvolvePointIndex(
                        b, pThis->NowPoint, Points, NowPointNum);
                    if (pThis->EvolvePointIndex != -1) {
                        pThis->MainStep = WaitEvolve;
                    } else {
                        pThis->MainStep = WaitNextPoint;
                    }
                }
                print_board(b, Points, NowPointNum);
                break;
            case WaitNextPoint:
                q = Points[NowPointNum];
                q->x = 0;
                q->y = 0;
                q->v = 1;
                q->Survive = TRUE;
                pThis->NowPoint = q;
                NowPointNum++;
                pThis->MainStep = WaitDrop;
                print_board(b, Points, NowPointNum);
                break;
            case WaitEvolve:
                pThis->NowPoint->Survive = FALSE;
                pThis->NowPoint = Points[pThis->EvolvePointIndex];
                Points[pThis->EvolvePointIndex]->v++;
                pThis->EvolvePointIndex = EvolvePointIndex(
                    b, pThis->NowPoint, Points, NowPointNum);
                if (pThis->EvolvePointIndex == -1) {
                    pThis->MainStep = WaitNextPoint;
                }
                print_board(b, Points, NowPointNum);
                break;
            case GameOver:
                system("clear");
                printf("GameOver\r\n");
                printf("Press '.' to close\r\n");
                break;
        }

        // printf("Index : %d\r\n", pThis->EvolvePointIndex);
        // printf("step : %d\r\n", pThis->MainStep);
        // printf("PointNum : %d\r\n", NowPointNum);

        if ((c = getchar()) == '.') {  // '.' を押すと抜ける
            break;
        }
    }

    system("/bin/stty cooked");  // 後始末

    return 0;
}

int EvolvePointIndex(Board b, Point *p, Point **Points, int PointNum) {
    int retD = -1;
    int retL = -1;
    int retR = -1;

    Point *q;
    for (int i = 0; i < PointNum; i++) {
        q = Points[i];
        if (!q->Survive || q->v != p->v) {
            continue;
        }

        if (q->y == p->y + 1 && q->x == p->x)  // 下側
        {
            retD = i;
        }

        if (q->y == p->y && q->x == p->x - 1)  // 左側
        {
            retL = i;
        }

        if (q->y == p->y && q->x == p->x + 1)  // 右側
        {
            retR = i;
        }
    }
    if (retD != -1) {
        return retD;
    } else if (retL != -1) {
        return retL;
    } else if (retR != -1) {
        return retR;
    } else {
        return -1;
    }
}

int LimitYtoDrop(Point *p, Point **Points, int PointNum) {
    int ret = BOARD_SIZE - 1;
    Point *q;
    for (int i = 0; i < PointNum - 1; i++) {
        q = Points[i];
        if (!q->Survive || q->x != p->x) {
            continue;
        }
        ret = MIN(ret, q->y - 1);
    }
    return ret;
}