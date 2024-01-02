#include <stdio.h>
#include <stdlib.h>
#define BOARD_SIZE 5
#define MAXPOINTNUM 1000
#define TRUE 1
#define FALSE 0
#define RIGHT 2
#define LEFT 3

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

void reset_Board(Board b) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        b.field[i][i] = "-";
    }
}

enum {
    WaitDrop,
    WaitNextPoint,
    WaitEvolve,
};

int main(int argc, char *argv[]) {
    THIS *pThis = malloc(sizeof(THIS));

    pThis->MainStep = WaitDrop;
    Board b = {.field = {"-----", "-----", "-----", "-----", "-----"}};
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
        reset_Board(b);
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
                    pThis->NowPoint->y = BOARD_SIZE - 1;
                    pThis->EvolvePointIndex = EvolvePointIndex(
                        b, pThis->NowPoint, Points, NowPointNum);
                    if (pThis->EvolvePointIndex != -1) {
                        pThis->MainStep = WaitEvolve;
                    } else {
                        pThis->MainStep = WaitNextPoint;
                    }
                }
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
                break;
            case WaitEvolve:
                pThis->NowPoint->Survive = FALSE;
                Points[pThis->EvolvePointIndex]->v++;
                pThis->MainStep = WaitNextPoint;
                break;
        }

        print_board(b, Points, NowPointNum);

        if ((c = getchar()) == '.') {  // '.' を押すと抜ける
            break;
        }
    }

    system("/bin/stty cooked");  // 後始末

    return 0;
}

int EvolvePointIndex(Board b, Point *p, Point **Points, int PointNum) {
    int ret;
    int IsEvolveExist = FALSE;
    for (int i = 0; i < PointNum; i++) {
        Point *q = Points[i];
        if (!q->Survive) {
            break;
        }
        if (q->y == p->y && q->x == p->x - 1)  // 右側
        {
            return i;
        }

        if (q->y == p->y && q->x == p->x + 1)  // 左側
        {
            IsEvolveExist = TRUE;
            ret = i;
        }
    }
    if (IsEvolveExist) {
        return ret;
    } else {
        return -1;
    }
}