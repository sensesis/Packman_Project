#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define BLACK   0
#define BLUE1   1
#define GREEN1   2
#define CYAN1   3
#define RED1   4
#define MAGENTA1 5
#define YELLOW1   6
#define GRAY1   7
#define GRAY2   8
#define BLUE2   9
#define GREEN2   10
#define CYAN2   11
#define RED2   12
#define MAGENTA2 13
#define YELLOW2   14
#define WHITE   15

#define ESC 0x1b //  ESC ������ ����

#define SPECIAL1 0xe0 // Ư��Ű�� 0xe0 + key ������ �����ȴ�.
#define SPECIAL2 0x00 // keypad ��� 0x00 + key �� �����ȴ�.

#define UP  0x48 // Up key�� 0xe0 + 0x48 �ΰ��� ���� ���´�.
#define DOWN 0x50
#define LEFT 0x4b
#define RIGHT 0x4d

#define UP2     'w'//player2 �� AWSD �� ����Ű ���
#define DOWN2   's'
#define LEFT2   'a'
#define RIGHT2  'd'

#define WIDTH 80
#define HEIGHT 24

#define PACMAN 4 // �Ѹ� ĳ����
#define COIN 0 // coin

#define MONSTER1 50 // ���� 1
#define MONSTER2 60 // ���� 2
#define MONSTER3 70 // ���� 3
#define MONSTER4 80 // ���� 4

#define HORIZ 1 // ����
#define VERTIC 2 // ����

#define HORIZ_W 11 // ���� ���
#define VERTIC_W 22 // ���� ���

#define MAP1 6 // ���� ���� ��
#define MAP2 7 // ���� ������ ��
#define MAP3 8 // ���� ���� �Ʒ�
#define MAP4 9 // ���� ������ �Ʒ�

#define W_MAP1 66 // ���� ���� �� ���
#define W_MAP2 77 // ���� ������ �� ���
#define W_MAP3 88 // ���� ���� �Ʒ� ���
#define W_MAP4 99 // ���� ������ �Ʒ� ���

#define BLANK 10 // ����
#define BLANK_2 80 // ����
#define ITEM 20 // ������
#define CHARRY 30 // ������ ü��

#define MAP2_SIZE_X 46
#define MAP2_SIZE_Y 46

int Delay = 50; // ������
int mon_frame_sync = 15;
int high_score_play_1 = 100; // ���� �ִ� ����
int keep_moving = 0; // �����̰� �ִ��� �������� �Ǵ��ϴ� ����
unsigned char ssh; // �� �ձ� ���� ���ڸ� �޴� ����
int life_number = 3; // �Ѹ� ���
int monster_score = 100; // ���� ������ ����
int countTime = 80; // �Ѹ� ��Ƹ��� �� �ִ� ��������


typedef struct element {
    int x, y;
    int val;
    int ans;
    int dir;
}element;

typedef struct QueueNode {
    element data;
    struct QueueNode* link;
}Node;

typedef struct QueueList {
    Node* front;
    Node* rear;
}Queue;

void initQueue(Queue* q) {
   
    q->front = NULL;
    q->rear = NULL;
}

int isEmtpy(Queue* q) {
    
    if (q->front == NULL && q->rear == NULL) {
        return TRUE;
    }
    return FALSE;
}

void push(Queue* q, element item) {
    
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = item;
    node->link = NULL;

    if (isEmtpy(q)) {
        q->front = node;
        q->rear = node;
    }
    else {
        q->rear->link = node;
        q->rear = node;
    }
}

element pop(Queue* q) {
    Node* tmp = q->front;
    element data = tmp->data;

    if (!isEmtpy(q)) {
        if (q->front == q->rear) {
            q->front = NULL;
            q->rear = NULL;
        }
        else {
            q->front = q->front->link;
        }
        remove(tmp);
    }

    return data;
}


void gotoxy(int x, int y) //���� ���ϴ� ��ġ�� Ŀ�� �̵�
{
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);// WIN32API �Լ��Դϴ�. �̰� ���ʿ� �����
}

void textcolor(int fg_color, int bg_color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);
}

void removeCursor(void) { // Ŀ���� �Ⱥ��̰� �Ѵ�
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curInfo.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

int map2[MAP2_SIZE_Y][MAP2_SIZE_X] = {
6,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,
2,6,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,80,6,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,2,
2,2,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,80,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,2,2,
2,2,0,6,1,1,1,7,0,6,1,1,1,1,1,1,1,1,1,7,0,2,80,2,0,6,1,1,1,1,1,1,1,1,1,7,0,6,1,1,1,1,7,0,2,2,
2,2,0,2,80,80,80,2,0,2,80,80,80,80,80,80,80,80,80,2,0,2,80,2,0,2,80,80,80,80,80,80,80,80,80,2,0,2,80,80,80,80,2,0,2,2,
2,2,0,8,1,1,1,9,0,8,1,1,1,1,1,1,1,1,1,9,0,8,1,9,0,8,1,1,1,1,1,1,1,1,1,9,0,8,1,1,1,1,9,0,2,2,
2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,
2,2,0,6,1,1,1,7,0,6,7,0,6,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,0,6,7,0,6,1,1,1,1,7,0,2,2,
2,2,0,8,1,1,1,9,0,2,2,0,8,1,1,1,1,1,1,1,1,7,80,6,1,1,1,1,1,1,1,1,9,0,2,2,0,8,1,1,1,1,9,0,2,2,
2,2,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,0,0,2,80,2,0,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,2,2,
2,8,1,1,1,1,1,7,0,2,8,1,1,1,1,1,1,1,1,7,10,2,10,2,10,6,1,1,1,1,1,1,1,1,9,2,0,6,1,1,1,1,1,1,9,2,
8,1,1,1,1,1,7,2,0,2,6,1,1,1,1,1,1,1,1,9,10,8,1,9,10,8,1,1,1,1,1,1,1,1,7,2,0,2,6,1,1,1,1,1,1,9,
10,10,10,10,10,10,2,2,0,2,2,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,2,2,0,2,2,10,10,10,10,10,10,10,
1,1,1,1,1,1,9,2,0,2,2,10,6,1,1,1,1,1,1,1,7,10,10,10,6,1,1,1,1,1,1,1,7,10,2,2,0,2,8,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,9,0,8,9,10,2,6,1,1,1,1,1,1,9,10,10,10,8,1,1,1,1,1,1,7,2,10,8,9,0,8,1,1,1,1,1,1,1,1,
10,10,10,10,10,10,10,10,0,50,10,10,2,2,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,70,2,2,10,10,10,0,10,10,10,10,10,10,10,10,10,
1,1,1,1,1,1,1,7,0,6,7,10,2,8,1,1,1,1,1,1,7,10,10,10,6,1,1,1,1,1,1,9,2,10,6,7,0,6,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,7,2,0,2,2,10,8,1,1,1,1,1,1,1,9,10,10,10,8,1,1,1,1,1,1,1,9,10,2,2,0,2,6,1,1,1,1,1,1,1,
10,10,10,10,10,10,2,2,0,2,2,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,2,2,0,2,2,10,10,10,10,10,10,10,
6,1,1,1,1,1,9,2,0,2,2,10,6,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,10,2,2,0,2,8,1,1,1,1,1,1,7,
2,6,1,1,1,1,1,9,0,8,9,10,8,1,1,1,1,1,1,1,1,7,80,6,1,1,1,1,1,1,1,1,9,10,8,9,0,8,1,1,1,1,1,1,7,2,
2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,80,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,
2,2,0,6,1,1,1,7,0,6,1,1,1,1,1,1,1,1,1,7,0,2,80,2,0,6,1,1,1,1,1,1,1,1,1,7,0,6,1,1,1,1,7,0,2,2,
2,2,0,8,1,1,7,2,0,8,1,1,1,1,1,1,1,1,1,9,0,8,1,9,0,8,1,1,1,1,1,1,1,1,1,9,0,2,6,1,1,1,9,0,2,2,
2,2,0,0,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,10,10,4,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,2,2,
2,8,1,1,7,0,2,2,0,6,7,0,6,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,0,6,7,0,2,2,0,6,1,1,1,9,2,
2,6,1,1,9,0,8,9,0,2,2,0,8,1,1,1,1,1,1,1,1,7,80,6,1,1,1,1,1,1,1,1,9,0,2,2,0,8,9,0,8,1,1,1,7,2,
2,2,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,0,0,2,80,2,0,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,2,2,
2,2,0,6,1,1,1,1,1,9,8,1,1,1,1,1,1,1,1,7,0,2,80,2,0,6,1,1,1,1,1,1,1,1,9,8,1,1,1,1,1,1,7,0,2,2,
2,2,0,8,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,0,8,1,9,0,8,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,0,2,2,
2,2,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,2,2,
2,8,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,2,
8,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,
};

int cpmap2[MAP2_SIZE_Y][MAP2_SIZE_X];

int visited[MAP2_SIZE_Y][MAP2_SIZE_X];

void clearVisited()
{
    for (int i = 0; i < MAP2_SIZE_Y; i++) {
        for (int j = 0; j < MAP2_SIZE_X; j++) {
            visited[i][j] = 0;
        }
    }
}

void cpdata() {

    for (int i = 0; i < MAP2_SIZE_X; i++) {

        for (int j = 0; j < MAP2_SIZE_Y; j++) {

            cpmap2[i][j] = map2[i][j];
        }
    }
}

void LIFE(int life_number) {

    int x = 52, y = 45;
    int num = 0;

    for (int i = life_number; i > 0; i--) {

        gotoxy(x + num, y);
        printf("                       ");

        gotoxy(x + num, y);
        textcolor(YELLOW1, YELLOW1);
        printf("  ");
        textcolor(WHITE, BLACK);

        num += 4;
    }

    if (life_number == 0) {

        gotoxy(x + num, y);
        printf("                       ");
    }

}

typedef struct Player {

    int x, y; // �迭 ���� ��ġ
    bool b_item;
}Player;

typedef struct Monster {

    int x, y; // �迭 ���� ��ġ
    bool coin;
    bool charry;

}Monster;

Player p1;
Monster m1;
Monster m2;
Monster m3;


int checkWall(int x, int y) {
    int val = cpmap2[y][x];
    
    if (val == HORIZ || val == VERTIC || val == MAP1 || val == MAP2 || val == MAP3 || val == MAP4) {
        return 1;
    }
    return 0;
}

int bfs(Monster m) {
   
    Queue q;
    initQueue(&q);
    element ghost = { m.x, m.y, 0, 0 };
    push(&q, ghost);

    while (!isEmtpy(&q)) {
        element curr = pop(&q);
        if (curr.val == PACMAN) {
            return curr.dir;
        }

        int x = curr.x;
        int y = curr.y;
        int ans = curr.ans;
        visited[y][x] = TRUE;

        if (y - 1 >= 0 && !checkWall(x, y - 1) && visited[y - 1][x] == FALSE) {
            curr.y = y - 1;
            curr.val = cpmap2[y - 1][x];
            curr.ans = ans + 1;
            if (ans == 0) {
                curr.dir = 1;
            }
            push(&q, curr);
        }
        if (y + 1 <= MAP2_SIZE_Y && !checkWall(x, y + 1) && visited[y + 1][x] == FALSE) {
            curr.y = y + 1;
            curr.val = cpmap2[y + 1][x];
            curr.ans = ans + 1;
            if (ans == 0) {
                curr.dir = 2;
            }
            push(&q, curr);
        }
        curr.y = y;
        if (x - 1 >= 0 && !checkWall(x - 1, y) && visited[y][x - 1] == FALSE) {
            curr.x = x - 1;
            curr.val = cpmap2[y][x - 1];
            curr.ans = ans + 1;
            if (ans == 0) {
                curr.dir = 3;
            }
            push(&q, curr);
        }
        if (x + 1 <= MAP2_SIZE_X && !checkWall(x + 1, y) && visited[y][x + 1] == FALSE) {
            curr.x = x + 1;
            curr.val = cpmap2[y][x + 1];
            curr.ans = ans + 1;
            if (ans == 0) {
                curr.dir = 4;
            }
            push(&q, curr);
        }
    }
}

int collision_detection(int x, int y, Player p) {
    // ������ ����� ��� ����ó��
    if (p.x + x < 0 || p.y + y < 0 || p.x + x > MAP2_SIZE_X - 1 || p.y + y > MAP2_SIZE_Y - 1) return 1;

    if (cpmap2[p.y + y][p.x + x] == BLANK || cpmap2[p.y + y][p.x + x] == COIN || cpmap2[p.y + y][p.x + x] == ITEM
        || cpmap2[p.y + y][p.x + x] == CHARRY) return 0;

    if (p1.b_item == true) {

        if (cpmap2[p.y + y][p.x + x] == BLANK || cpmap2[p.y + y][p.x + x] == COIN || cpmap2[p.y + y][p.x + x] == ITEM
            || cpmap2[p.y + y][p.x + x] == MONSTER1 || cpmap2[p.y + y][p.x + x] == MONSTER2 || cpmap2[p.y + y][p.x + x] == MONSTER3
            || cpmap2[p.y + y][p.x + x] == CHARRY) return 0;
    }
    return 1;
}

int collision_detection_m(int x, int y, Monster m) {
    // ������ ����� ��� ����ó��
    if (m.x + x < 0 || m.y + y < 0 || m.x + x > MAP2_SIZE_X - 1 || m.y + y > MAP2_SIZE_Y - 1) return 1;

    if (cpmap2[m.y + y][m.x + x] == BLANK || cpmap2[m.y + y][m.x + x] == COIN || cpmap2[m.y + y][m.x + x] == PACMAN || cpmap2[m.y + y][m.x + x] == CHARRY) return 0;

    return 1;
}

void move_character(unsigned char ch, int* score_play_1) {

    int player;
    int isItem = 0;
    int x = 0, y = 0;


    if (ch == SPECIAL1 || ch == SPECIAL2) {
        ch = _getch();
        ssh = ch;
        switch (ch) {
        case UP:
        case DOWN:
        case LEFT:
        case RIGHT:
            keep_moving = 1;
            player = PACMAN;
            break;
        default: // ����Ű�� �ƴϸ� �����
            keep_moving = 0;
        }
    }

    if (keep_moving) { // �����̰� ������
        switch (ch) {
            // player 2
        case UP:
            if (p1.y > 0)
                y = -1;
            break;
        case DOWN:
            if (p1.y < MAP2_SIZE_Y)
                y = +1;
            break;
        case LEFT:
            if (p1.x > 0)
                x = -1;
            break;
        case RIGHT:
            if (p1.x < MAP2_SIZE_X)
                x = +1;
            break;
        }

        // �浹 ó��
        if (!collision_detection(x, y, p1)) {
            // �迭���� ĳ���� ��ġ ����

            if (p1.x + x == MAP2_SIZE_X - 1) {

                if (cpmap2[p1.y + y][0] == COIN)
                    *score_play_1 += 10;

                else if (cpmap2[p1.y + y][0] == CHARRY)
                    *score_play_1 += 50;

                else if (cpmap2[p1.y + y][0] == MONSTER1) {

                    *score_play_1 += monster_score;
                    monster_score += 100;

                    m1.x = 9;
                    m1.y = 15;

                    cpmap2[m1.y][m1.x] = MONSTER1;
                }


                else if (cpmap2[p1.y + y][0] == MONSTER3) {

                    *score_play_1 += monster_score;
                    monster_score += 100;

                    m3.x = 30;
                    m3.y = 15;

                    cpmap2[m3.y][m3.x] = MONSTER3;
                }


                cpmap2[p1.y + y][0] = PACMAN;
                cpmap2[p1.y][p1.x] = BLANK;

                p1.x = 0;
            }

            else if (p1.x + x == 0) {

                if (cpmap2[p1.y + y][MAP2_SIZE_X - 1] == COIN)
                    *score_play_1 += 10;

                else if (cpmap2[p1.y + y][MAP2_SIZE_X - 1] == CHARRY)
                    *score_play_1 += 50;

                else if (cpmap2[p1.y + y][MAP2_SIZE_X - 1] == MONSTER1) {

                    *score_play_1 += monster_score;
                    monster_score += 100;

                    m1.x = 9;
                    m1.y = 15;

                    cpmap2[m1.y][m1.x] = MONSTER1;
                    //[p1.y + y] [MAP2_SIZE_X - 1] = BLANK;
                }

                else if (cpmap2[p1.y + y][MAP2_SIZE_X - 1] == MONSTER3) {

                    *score_play_1 += monster_score;
                    monster_score += 100;

                    m3.x = 30;
                    m3.y = 15;

                    cpmap2[m3.y][m3.x] = MONSTER3;
                    //[p1.y + y] [MAP2_SIZE_X - 1] = BLANK;
                }

                cpmap2[p1.y + y][MAP2_SIZE_X - 1] = PACMAN;
                cpmap2[p1.y][p1.x] = BLANK;

                p1.x = MAP2_SIZE_X - 1;
            }

            else {

                if (cpmap2[p1.y + y][p1.x + x] == COIN)
                    *score_play_1 += 10;

                else if (cpmap2[p1.y + y][p1.x + x] == CHARRY)
                    *score_play_1 += 50;

                else if (cpmap2[p1.y + y][p1.x + x] == MONSTER1) {

                    *score_play_1 += monster_score;
                    monster_score += 100;

                    m1.x = 9;
                    m1.y = 15;

                    cpmap2[m1.y][m1.x] = MONSTER1;
                }

                else if (cpmap2[p1.y + y][p1.x + x] == MONSTER3) {

                    *score_play_1 += monster_score;
                    monster_score += 100;

                    m3.x = 30;
                    m3.y = 15;

                    cpmap2[m3.y][m3.x] = MONSTER3;
                }

                if (cpmap2[p1.y + y][p1.x + x] == ITEM) {
                    p1.b_item = true;
                    countTime = 55;
                }

                cpmap2[p1.y + y][p1.x + x] = PACMAN;
                cpmap2[p1.y][p1.x] = BLANK;

                if (high_score_play_1 <= *score_play_1)
                    high_score_play_1 = *score_play_1;

                // player ��ġ�� ����
                p1.x += x;
                p1.y += y;
            }
        }
    }
}

void moveGhost1() {

    int xx = 0, yy = 0;

    clearVisited();
    if (p1.b_item == false) {
        int dir = bfs(m1);

        switch (dir) {
        case 1: yy--; break; // ��
        case 2: yy++; break; // ��
        case 3: xx--; break; // ��
        case 4: xx++; break; // ��
        }
    }

    if (p1.b_item == true) {

        if (abs(m1.x - p1.x) > abs(m1.y - p1.y)) {

            if (m1.x < p1.x)
                xx--;

            else if (m1.x > p1.x)
                xx++;
        }

        else {

            if (m1.y < p1.y)
                yy--;

            else if (m1.y > p1.y)
                yy++;

        }
    }

    if (!collision_detection_m(xx, yy, m1)) {
        // �迭���� ĳ���� ��ġ ����

        if (m1.coin == true)
            cpmap2[m1.y][m1.x] = COIN;

        if (cpmap2[m1.y + yy][m1.x + xx] == COIN) {

            m1.coin = true;
            cpmap2[m1.y + yy][m1.x + xx] = MONSTER1;

            if (cpmap2[m1.y][m1.x] == COIN) {

                cpmap2[m1.y][m1.x] = COIN;
            }

            else {

                cpmap2[m1.y][m1.x] = BLANK;
            }
        }


        else if (cpmap2[m1.y + yy][m1.x + xx] == PACMAN) {

            m1.coin = false;
            cpmap2[m1.y + yy][m1.x + xx] = MONSTER1;
            cpmap2[m1.y][m1.x] = BLANK;

            p1.x = 23;
            p1.y = 24;

            life_number -= 1;
        }

        else {
            m1.coin = false;
            cpmap2[m1.y + yy][m1.x + xx] = MONSTER1;
            cpmap2[m1.y][m1.x] = BLANK;
        }

        // player ��ġ�� ����
        m1.x += xx;
        m1.y += yy;
    }
}

void moveGhost3() {

    int xx = 0, yy = 0;

    clearVisited();
    if (p1.b_item == false) {
        int dir = bfs(m3);

        switch (dir) {
        case 1: yy--; break; // ��
        case 2: yy++; break; // ��
        case 3: xx--; break; // ��
        case 4: xx++; break; // ��
        }

    }

    if (p1.b_item == true) {

        if (abs(m3.x - p1.x) > abs(m3.y - p1.y)) {

            if (m3.x < p1.x)
                xx--;

            else if (m3.x > p1.x)
                xx++;
        }

        else {

            if (m3.y < p1.y)
                yy--;

            else if (m3.y > p1.y)
                yy++;

        }
    }

    if (!collision_detection_m(xx, yy, m3)) {
        // �迭���� ĳ���� ��ġ ����

        if (m3.coin == true)
            cpmap2[m3.y][m3.x] = COIN;

        if (cpmap2[m3.y + yy][m3.x + xx] == COIN) {

            m3.coin = true;
            cpmap2[m3.y + yy][m3.x + xx] = MONSTER3;

            if (cpmap2[m3.y][m3.x] == COIN) {

                cpmap2[m3.y][m3.x] = COIN;
            }

            else {

                cpmap2[m3.y][m3.x] = BLANK;
            }
        }


        else if (cpmap2[m3.y + yy][m3.x + xx] == PACMAN) {

            m3.coin = false;
            cpmap2[m3.y + yy][m3.x + xx] = MONSTER3;
            cpmap2[m3.y][m3.x] = BLANK;

            p1.x = 23;
            p1.y = 24;

            life_number -= 1;
        }

        else {
            m3.coin = false;
            cpmap2[m3.y + yy][m3.x + xx] = MONSTER3;
            cpmap2[m3.y][m3.x] = BLANK;
        }

        // player ��ġ�� ����
        m3.x += xx;
        m3.y += yy;
    }
}

void showscore(int* score_play_1) {

    textcolor(WHITE, BLACK);
    gotoxy(63, 10);
    printf("1UP");

    gotoxy(64, 11);
    printf("%d", *score_play_1);
}

void highshowscore() {

    textcolor(WHITE, BLACK);
    gotoxy(90, 10);
    printf("HIGH SCORE");

    gotoxy(94, 11);
    printf("%d", high_score_play_1);
}

void map22() {
    int i = 0, x = 0, y = 0;
    char ch = NULL;

    // while (1) {
         //cls(WHITE, BLACK);
    for (int y = 0; y < MAP2_SIZE_X - 13; y++) {
        gotoxy(50, y + 12); // y�� ó�� ��ġ�� Ȯ���ϰ�, ���� ������ overwirte�Ѵ�.
        for (int x = 0; x < MAP2_SIZE_Y; x++) {
            //  gotoxy(x * 2, y);
            if (cpmap2[y][x] == HORIZ) {
                textcolor(BLUE1, BLACK);
                printf("��");
            }

            else if (cpmap2[y][x] == VERTIC) {
                textcolor(BLUE1, BLACK);
                printf("��");
            }

            else if (cpmap2[y][x] == 11) {
                textcolor(WHITE, BLACK);
                printf("��");
            }

            else if (cpmap2[y][x] == MAP1) {
                textcolor(BLUE1, BLACK);
                printf("��");
            }

            else if (cpmap2[y][x] == MAP2) {
                textcolor(BLUE1, BLACK);
                printf("��");
            }
            else if (cpmap2[y][x] == MAP3) {
                textcolor(BLUE1, BLACK);
                printf("��");
            }
            else if (cpmap2[y][x] == MAP4) {
                textcolor(BLUE1, BLACK);
                printf("��");
            }

            else if (cpmap2[y][x] == W_MAP1) {
                textcolor(WHITE, BLACK);
                printf("��");
            }

            else if (cpmap2[y][x] == W_MAP2) {
                textcolor(WHITE, BLACK);
                printf("��");
            }

            else if (cpmap2[y][x] == W_MAP3) {
                textcolor(WHITE, BLACK);
                printf("��");
            }

            else if (cpmap2[y][x] == W_MAP4) {
                textcolor(WHITE, BLACK);
                printf("��");
            }

            else if (cpmap2[y][x] == PACMAN) {

                textcolor(YELLOW1, YELLOW1);
                printf("  ");

            }

            else if (cpmap2[y][x] == MONSTER1) {


                if (p1.b_item == true) {

                    textcolor(BLUE1, BLUE1);
                    printf("  ");
                }

                else {

                    textcolor(MAGENTA2, MAGENTA2);
                    printf("  ");
                }
            }

            else if (cpmap2[y][x] == MONSTER2) {


                if (p1.b_item == true) {

                    textcolor(BLUE1, BLUE1);
                    printf("  ");
                }

                else {

                    textcolor(RED1, RED1);
                    printf("  ");
                }
            }

            else if (cpmap2[y][x] == MONSTER3) {


                if (p1.b_item == true) {

                    textcolor(BLUE1, BLUE1);
                    printf("  ");
                }

                else {

                    textcolor(CYAN2, CYAN2);
                    printf("  ");
                }
            }

            else if (cpmap2[y][x] == ITEM) {

                textcolor(GREEN1, BLACK);
                printf(" o");
            }

            else if (cpmap2[y][x] == COIN) {

                textcolor(YELLOW2, BLACK);
                printf(" *");
            }


            else if (cpmap2[y][x] == BLANK) {

                textcolor(BLACK, BLACK);
                printf("  ");
            }

            else if (cpmap2[y][x] == BLANK_2) {

                textcolor(BLACK, BLACK);
                printf("  ");
            }

            else if (cpmap2[y][x] == CHARRY) {

                textcolor(RED2, RED2);
                printf("  ");
            }

        }
    }
    i = i ? 0 : 1;
}


bool mapclear() {

    for (int y = 0; y < MAP2_SIZE_X - 13; y++) {

        for (int x = 0; x < MAP2_SIZE_Y; x++) {


            if (cpmap2[y][x] == COIN) {

                return false;
            }
        }
    }

    return true;
}

void printGG(int x, int y) {

    gotoxy(x, y);     printf(" �âââââ� ");
    gotoxy(x, y + 1); printf("�ââââââ�");
    gotoxy(x, y + 2); printf("�â�      �â�");
    gotoxy(x, y + 3); printf("�â�          ");
    gotoxy(x, y + 4); printf("�â�  �âââ�");
    gotoxy(x, y + 5); printf("�â�  �âââ�");
    gotoxy(x, y + 6); printf("�â�      �â�");
    gotoxy(x, y + 7); printf("�ââââââ�");
    gotoxy(x, y + 8); printf(" �âââââ�");
}

void printAA(int x, int y) {

    gotoxy(x, y);     printf(" �âââââ� ");
    gotoxy(x, y + 1); printf("�ââââââ�");
    gotoxy(x, y + 2); printf("�â�      �â�");
    gotoxy(x, y + 3); printf("�â�      �â�");
    gotoxy(x, y + 4); printf("�ââââââ�");
    gotoxy(x, y + 5); printf("�ââââââ�");
    gotoxy(x, y + 6); printf("�â�      �â�");
    gotoxy(x, y + 7); printf("�â�      �â�");
    gotoxy(x, y + 8); printf("�â�      �â�");
}

void printMM(int x, int y) {

    gotoxy(x, y);     printf("�âââââââ� ");
    gotoxy(x, y + 1); printf("�âââââââ�");
    gotoxy(x, y + 2); printf("�â�  �â�  �â�");
    gotoxy(x, y + 3); printf("�â�  �â�  �â�");
    gotoxy(x, y + 4); printf("�â�  �â�  �â�");
    gotoxy(x, y + 5); printf("�â�  �â�  �â�");
    gotoxy(x, y + 6); printf("�â�  �â�  �â�");
    gotoxy(x, y + 7); printf("�â�  �â�  �â�");
    gotoxy(x, y + 8); printf("�â�  �â�  �â�");
}

void printEE(int x, int y) {

    gotoxy(x, y);     printf("�âââââââ� ");
    gotoxy(x, y + 1); printf("�âââââââ�");
    gotoxy(x, y + 2); printf("�â�");
    gotoxy(x, y + 3); printf("�â�");
    gotoxy(x, y + 4); printf("�âââââââ�");
    gotoxy(x, y + 5); printf("�âââââââ�");
    gotoxy(x, y + 6); printf("�â�");
    gotoxy(x, y + 7); printf("�âââââââ�");
    gotoxy(x, y + 8); printf("�âââââââ�");
}

void printOO(int x, int y) {

    gotoxy(x, y);     printf(" �ââââââ� ");
    gotoxy(x, y + 1); printf("�âââââââ�");
    gotoxy(x, y + 2); printf("�â�        �â� ");
    gotoxy(x, y + 3); printf("�â�        �â�");
    gotoxy(x, y + 4); printf("�â�        �â�");
    gotoxy(x, y + 5); printf("�â�        �â�");
    gotoxy(x, y + 6); printf("�â�        �â� ");
    gotoxy(x, y + 7); printf("�âââââââ�");
    gotoxy(x, y + 8); printf(" �ââââââ�");
}

void printVV(int x, int y) {

    gotoxy(x, y);     printf("�â�        �â� ");
    gotoxy(x, y + 1); printf("�â�        �â�");
    gotoxy(x, y + 2); printf("�â�        �â�");
    gotoxy(x, y + 3); printf("�â�        �â�");
    gotoxy(x, y + 4); printf("�â�        �â�");
    gotoxy(x, y + 5); printf("�â�        �â�");
    gotoxy(x, y + 6); printf(" �â�      �â�");
    gotoxy(x, y + 7); printf("  �â�    �â�");
    gotoxy(x, y + 8); printf("     �ââ�");
}

void printRR(int x, int y) {

    gotoxy(x, y);     printf(" �âââââ� ");
    gotoxy(x, y + 1); printf("�ââââââ�");
    gotoxy(x, y + 2); printf("�â�      �â�");
    gotoxy(x, y + 3); printf("�â�      �â�");
    gotoxy(x, y + 4); printf("�ââââââ�");
    gotoxy(x, y + 5); printf("�âââââ�");
    gotoxy(x, y + 6); printf("�â�     �â�");
    gotoxy(x, y + 7); printf("�â�      �â�");
    gotoxy(x, y + 8); printf("�â�       �â�");
}

void printWW(int x, int y) {

    gotoxy(x, y);     printf("�â�    �â�    �â� ");
    gotoxy(x, y + 1); printf("�â�    �â�    �â�");
    gotoxy(x, y + 2); printf("�â�    �â�    �â�");
    gotoxy(x, y + 3); printf("�â�    �â�    �â�");
    gotoxy(x, y + 4); printf("�â�    �â�    �â�");
    gotoxy(x, y + 5); printf("�â�    �â�    �â�");
    gotoxy(x, y + 6); printf("�â�    �â�    �â�");
    gotoxy(x, y + 7); printf(" �ââââââââ�");
    gotoxy(x, y + 8); printf("   �ââââââ�");
}

void printII(int x, int y) {

    gotoxy(x, y);     printf("�ââââ�");
    gotoxy(x, y + 1); printf("  �ââ�");
    gotoxy(x, y + 2); printf("  �ââ�");
    gotoxy(x, y + 3); printf("  �ââ�");
    gotoxy(x, y + 4); printf("  �ââ�");
    gotoxy(x, y + 5); printf("  �ââ�");
    gotoxy(x, y + 6); printf("  �ââ�");
    gotoxy(x, y + 7); printf("  �ââ�");
    gotoxy(x, y + 8); printf("�ââââ�");
}

void printNN(int x, int y) {

    gotoxy(x, y);     printf("�â�         �â� ");
    gotoxy(x, y + 1); printf("�âââ�     �â�");
    gotoxy(x, y + 2); printf("�â� �â�    �â�");
    gotoxy(x, y + 3); printf("�â�   �â�  �â�");
    gotoxy(x, y + 4); printf("�â�    �â� �â�");
    gotoxy(x, y + 5); printf("�â�     �âââ�");
    gotoxy(x, y + 6); printf("�â�       �ââ�");
    gotoxy(x, y + 7); printf("�â�         �â�");
    gotoxy(x, y + 8); printf("�â�         �â�");
}

void Winner_screen(int xx, int yy) {

    char ch;

    for (int i = 0; i < 50; i++) {

        if (_kbhit() == 1) {  // Ű���尡 ������ ������
            ch = _getch(); // key ���� �д´�
            // ESC ������ ���α׷� ����
            if (ch == ESC) {

                exit(0);
            }
        }

        if (i % 2 == 0) {

            textcolor(WHITE, BLACK);
            printWW(xx, yy);
            printII(xx + 24, yy);
            printNN(xx + 39, yy);
            printNN(xx + 59, yy);
            printEE(xx + 80, yy);
            printRR(xx + 100, yy);
        }


        else {

            textcolor(GREEN1, BLACK);
            printWW(xx, yy);
            printII(xx + 24, yy);
            printNN(xx + 39, yy);
            printNN(xx + 59, yy);
            printEE(xx + 80, yy);
            printRR(xx + 100, yy);
        }
        Sleep(300);
    }
}

void gameover_screen(int xx, int yy) {

    char ch;

    for (int i = 0; i < 50; i++) {

        if (_kbhit() == 1) {  // Ű���尡 ������ ������
            ch = _getch(); // key ���� �д´�
            // ESC ������ ���α׷� ����
            if (ch == ESC) {

                exit(0);
            }
        }

        if (i % 2 == 0) {

            textcolor(WHITE, BLACK);
            printGG(xx, yy);
            printAA(xx + 16, yy);
            printMM(xx + 32, yy);
            printEE(xx + 50, yy);
            printOO(xx + 68, yy);
            printVV(xx + 86, yy);
            printEE(xx + 104, yy);
            printRR(xx + 122, yy);
        }


        else {

            textcolor(YELLOW2, BLACK);
            printGG(xx, yy);
            printAA(xx + 16, yy);
            printMM(xx + 32, yy);
            printEE(xx + 50, yy);
            printOO(xx + 68, yy);
            printVV(xx + 86, yy);
            printEE(xx + 104, yy);
            printRR(xx + 122, yy);
        }

        Sleep(300);
    }
}

void printP(int x, int y) {

    textcolor(RED2, BLACK);
    gotoxy(x, y);     printf("�âââââââ�   ");
    gotoxy(x, y + 1); printf("�ââââââââ� ");
    gotoxy(x, y + 2); printf("�â�          �â� ");
    gotoxy(x, y + 3); printf("�â�          �â� ");
    gotoxy(x, y + 4); printf("�ââââââââ� ");
    gotoxy(x, y + 5); printf("�âââââââ�   ");
    gotoxy(x, y + 6); printf("�â�              ");
    gotoxy(x, y + 7); printf("�â�              ");
    gotoxy(x, y + 8); printf("�â�              ");
}

void printA(int x, int y) {

    textcolor(YELLOW2, BLACK);
    gotoxy(x, y);     printf("        �ââ�         ");
    gotoxy(x, y + 1); printf("       �â� �â�       ");
    gotoxy(x, y + 2); printf("      �â�   �â�      ");
    gotoxy(x, y + 3); printf("     �â�     �â�      ");
    gotoxy(x, y + 4); printf("    �â�       �â�     ");
    gotoxy(x, y + 5); printf("   �ââââââââ�    ");
    gotoxy(x, y + 6); printf("  �âââââââââ�   ");
    gotoxy(x, y + 7); printf(" �â�              �â�  ");
    gotoxy(x, y + 8); printf("�â�                �â� ");
}

void printC(int x, int y) {

    textcolor(YELLOW1, BLACK);
    gotoxy(x, y);     printf("     �âââââ�       ");
    gotoxy(x, y + 1); printf("   �âââââââ� ");
    gotoxy(x, y + 2); printf(" �âââââââ�       ");
    gotoxy(x, y + 3); printf("�âââââ�         ");
    gotoxy(x, y + 4); printf("�ââââ�       ");
    gotoxy(x, y + 5); printf("�âââââ�     ");
    gotoxy(x, y + 6); printf(" �ââââââ�      ");
    gotoxy(x, y + 7); printf("   �âââââââ� ");
    gotoxy(x, y + 8); printf("    �âââââ�");
}

void printK(int x, int y) {

    textcolor(BLUE2, BLACK);
    gotoxy(x, y);     printf("�â�      �ââ� ");
    gotoxy(x, y + 1); printf("�â�    �ââ�   ");
    gotoxy(x, y + 2); printf("�â�  �â�       ");
    gotoxy(x, y + 3); printf("�â� �â�        ");
    gotoxy(x, y + 4); printf("�âââ�         ");
    gotoxy(x, y + 5); printf("�â� �â�        ");
    gotoxy(x, y + 6); printf("�â�   �â�      ");
    gotoxy(x, y + 7); printf("�â�    �ââ�   ");
    gotoxy(x, y + 8); printf("�â�      �ââ� ");
}

void printM(int x, int y) {

    textcolor(BLUE1, BLACK);
    gotoxy(x, y);     printf("�â�              �â� ");
    gotoxy(x, y + 1); printf("�ââ�           �ââ� ");
    gotoxy(x, y + 2); printf("�âââ�       �âââ�  ");
    gotoxy(x, y + 3); printf("�â� �â�     �â� �â� ");
    gotoxy(x, y + 4); printf("�â�  �â�   �â�  �â� ");
    gotoxy(x, y + 5); printf("�â�   �â� �â�   �â� ");
    gotoxy(x, y + 6); printf("�â�   �âââ�    �â� ");
    gotoxy(x, y + 7); printf("�â�    �ââ�     �â� ");
    gotoxy(x, y + 8); printf("�â�     �â�      �â� ");
}

void printN(int x, int y) {

    textcolor(BLUE2, BLACK);
    gotoxy(x, y);     printf("�ââ�          �â� ");
    gotoxy(x, y + 1); printf("�âââ�        �â� ");
    gotoxy(x, y + 2); printf("�â� �ââ�     �â�  ");
    gotoxy(x, y + 3); printf("�â�   �ââ�   �â�  ");
    gotoxy(x, y + 4); printf("�â�    �ââ�  �â� ");
    gotoxy(x, y + 5); printf("�â�     �ââ� �â�  ");
    gotoxy(x, y + 6); printf("�â�        �âââ� ");
    gotoxy(x, y + 7); printf("�â�          �ââ� ");
    gotoxy(x, y + 8); printf("�â�            �â� ");
}

void Pacman_screen(int xx, int yy, int cursor) {

    char ch;

    printP(xx, yy);
    printA(xx + 20, yy);
    printC(xx + 45, yy);
    printK(xx + 70, yy);
    printM(xx + 92, yy);
    printA(xx + 120, yy);
    printN(xx + 148, yy);
    gotoxy(175, 20);

    textcolor(WHITE, BLACK);
    printf("made by kitaejin");

    textcolor(WHITE, BLACK);
    gotoxy(86, 32);
    printf("��  ��  ��  ��");
    //gotoxy(86, 36);
    //printf("2  ��  ��  ��  ��");
    gotoxy(86, 36);
    printf("�� �� �� ��");
    gotoxy(86, 40);
    printf("��  ��  ��");
    gotoxy(81, cursor);
    printf(">>");
}

int menu(void) {

    int cursor = 32;
    int num = 0;
    while (1) {

        Pacman_screen(10, 10, cursor);

        for (int i = 0; i < 10; i++) {

            if (_kbhit()) {
                int input = _getch();

                if (input == 13) { // ���� = 13��

                    if (cursor == 32) {

                        for (i = 0; i < 5; i++)      //���ý� ��¦�Ÿ� 
                        {
                            gotoxy(86, 32);
                            printf("                ");
                            Sleep(100);
                            gotoxy(86, 32);
                            printf("��  ��  ��  ��");;
                            Sleep(100);
                        }
                        system("cls");
                        return 1;      //���� ���ý� 1��ȯ
                    }

                    else if (cursor == 36) {

                        for (i = 0; i < 5; i++)      //���ý� ��¦�Ÿ� 
                        {
                            gotoxy(86, 36);
                            printf("                 ");
                            Sleep(100);
                            gotoxy(86, 36);
                            printf("�� �� �� ��");
                            Sleep(100);
                        }
                        system("cls");
                        return 2;         //���� ���ý� 1��ȯ
                    }

                    else if (cursor == 40) {

                        for (i = 0; i < 5; i++)      //���ý� ��¦�Ÿ� 
                        {
                            gotoxy(86, 40);
                            printf("                 ");
                            Sleep(100);
                            gotoxy(86, 40);
                            printf("�� �� ��");
                            Sleep(100);
                        }
                        system("cls");
                        return 0;      //���� ���ý� 1��ȯ
                    }
                }

                else {

                    switch (input) {

                    case UP: {

                        textcolor(BLUE2, BLACK);
                        gotoxy(81, cursor);
                        printf("  ");

                        if (cursor != 32)
                            cursor = cursor - 4;

                        else
                            cursor = 40;

                        break;
                    }

                    case DOWN: {

                        gotoxy(81, cursor);
                        printf("  ");

                        if (cursor != 40)
                            cursor = cursor + 4;
                        else
                            cursor = 32;
                        break;
                    }
                    }
                }
            }
            Sleep(10);
        }
    }

    if (num == 0)
    {
        gotoxy(81, cursor);
        printf(">>");
        num = 1;
    }

    else
    {
        gotoxy(81, cursor);
        printf("  ");
        num = 0;
    }

}

void menual(int x, int y) {

    gotoxy(x, y);
    printf("");
}

void emoticon() {

    gotoxy(20, 8);
    textcolor(YELLOW1, YELLOW1);
    printf("  ");

    textcolor(WHITE, BLACK);
    gotoxy(22, 7);
    printf(";;;");

    gotoxy(32, 8);
    textcolor(RED1, RED1);
    printf("  ");

    gotoxy(37, 8);
    textcolor(MAGENTA2, MAGENTA2);
    printf("  ");

    gotoxy(40, 8);
    textcolor(CYAN2, CYAN2);
    printf("  ");

    gotoxy(43, 8);
    textcolor(WHITE, BLACK);
    printf("~~~~~~~~");

    gotoxy(152, 53);
    textcolor(YELLOW1, YELLOW1);
    printf("    ");
    gotoxy(152, 54);
    printf("    ");

    gotoxy(157, 54);
    textcolor(YELLOW2, BLACK);
    printf(" *");

    gotoxy(159, 54);
    textcolor(YELLOW2, BLACK);
    printf(" *");

    gotoxy(161, 54);
    textcolor(YELLOW2, BLACK);
    printf(" *");

    gotoxy(168, 54);
    textcolor(BLUE1, BLUE1);
    printf("  ");

    gotoxy(172, 54);
    textcolor(BLUE1, BLUE1);
    printf("  ");

    gotoxy(176, 54);
    textcolor(BLUE1, BLUE1);
    printf("  ");

    gotoxy(178, 53);
    textcolor(WHITE, BLACK);
    printf(";;;");

}

void gamemenual() {

    gotoxy(24, 10);
    textcolor(YELLOW1, YELLOW1);
    printf("  ");
    gotoxy(26, 9);
    textcolor(WHITE, BLACK);
    printf(";;;");

    gotoxy(36, 10);
    textcolor(RED1, RED1);
    printf("  ");

    gotoxy(40, 10);
    textcolor(MAGENTA2, MAGENTA2);
    printf("  ");

    gotoxy(44, 10);
    textcolor(CYAN2, CYAN2);
    printf("  ");

    gotoxy(47, 10);
    textcolor(WHITE, BLACK);
    printf("~~~~~~~~");

    gotoxy(152, 53);
    textcolor(YELLOW1, YELLOW1);
    printf("    ");
    gotoxy(152, 54);
    printf("    ");

    gotoxy(157, 54);
    textcolor(YELLOW2, BLACK);
    printf(" *");

    gotoxy(159, 54);
    textcolor(YELLOW2, BLACK);
    printf(" *");

    gotoxy(161, 54);
    textcolor(YELLOW2, BLACK);
    printf(" *");

    gotoxy(168, 54);
    textcolor(BLUE1, BLUE1);
    printf("  ");

    gotoxy(172, 54);
    textcolor(BLUE1, BLUE1);
    printf("  ");

    gotoxy(176, 54);
    textcolor(BLUE1, BLUE1);
    printf("  ");

    gotoxy(178, 53);
    textcolor(WHITE, BLACK);
    printf(";;;");



    gotoxy(23, 12);
    textcolor(YELLOW2, BLACK);
    printf("������������������������������������������������������������������������������������������������������������������������������������������������������������");

    gotoxy(23, 52);
    textcolor(YELLOW2, BLACK);
    printf("������������������������������������������������������������������������������������������������������������������������������������������������������������");

    gotoxy(85, 15);
    textcolor(WHITE, BLACK);
    printf("�� Pacman ���� ���� ��");

    gotoxy(160, 15);
    textcolor(WHITE, BLACK);
    printf("�ƹ�Ű: �ڷΰ���");


    gotoxy(35, 25);
    printf("1. �÷��̾�� 4���� ������ �̿��� �Ķ��� ������ ������ �̷� �ȿ��� �Ѹ��� �����Ѵ�.");

    gotoxy(35, 29);
    printf("2. �̷� �ȿ��� 2������ ���Ͱ� �����ϴµ�, �̵� ���͵��� ������ ���ϸ鼭 �̷� ���� ��ġ�� ��Ű�� �� ������ ������ �̱�Եȴ�.");

    gotoxy(35, 33);
    printf("3. �Ѹ��� ���Ϳ��� ������ ����� �ϳ� �پ���, �Ѹ��� ����� �������� ���� ������ �ȴ�. ");

    gotoxy(35, 37);
    printf("4. �������� ���� �� ���ʹ� 10�ʵ��� �������� �Ǹ� ���͸� ��Ƹ����� ������ ��´�.");

    gotoxy(35, 41);
    printf("5. 10�ʸ��� ü���� ������ ü���� ������ ������ ��´�. ���Ͱ� ü���� ������ �� ü���� �Ҹ��ϰԵȴ�.");

    gotoxy(35, 45);
    printf("6. �Ѹ��� ���� ���� �����Ӱ� �Ѿ �� ������ ���ʹ� �Ѿ �� ����.");

}

void show_charry(void) {

    int x, y;


    for (int i = 0; i < 10; i++) {

        x = rand() % MAP2_SIZE_X;
        y = rand() % MAP2_SIZE_Y;

        if (cpmap2[y][x] == BLANK) {
            cpmap2[y][x] = CHARRY;
            break;
        }

        else {

            x = rand() % MAP2_SIZE_X;
            y = rand() % MAP2_SIZE_Y;
            i--;
        }
    }
}

int main() {

    system("mode con cols=200 lines=60");
    //int mapnum2 = 15;
    int score = 0; // ���ھ� ����
    unsigned int gohsttime = 0; // ���� �ӵ� ����
    unsigned int charrytime = 0; // ü�� ���� �ӵ� ����

    m1.coin = false;
    p1.b_item = false;
    bool gameclear = false;
    removeCursor();

    p1.x = 23;
    p1.y = 24;

    m1.x = 9;
    m1.y = 15;

    m3.x = 30;
    m3.y = 15;
    cpdata();
    int* score_play_1 = &score; // player1�� score
    unsigned char ch;


    while (1) {
      
        system("cls");
        emoticon();
        int start = menu();

        switch (start) {

        case 1:

            while (1) {

                gohsttime++; // ���� �ӵ�
                charrytime++; // ���� �ӵ�
                countTime -= 1;

                if (countTime < 0)
                    p1.b_item = false;


                if (life_number < 0)
                    break;

                if (mapclear() == true || ch == UP2) {  // �� ��� ������ winner!!
                    Winner_screen(38, 23);
                    exit(1);
                }

                gameclear = mapclear();

                showscore(score_play_1);
                highshowscore();
                map22();
                LIFE(life_number);


                if (charrytime % 55 == 0) {

                    show_charry();
                }

                if (p1.b_item == false) {

                    moveGhost1();
                    //moveGhost2();
                    moveGhost3();
                }

                if (p1.b_item == true) {

                    if (gohsttime % 2 == 0) {
                        moveGhost1();
                        //moveGhost2();
                        moveGhost3();
                    }
                }


                if (_kbhit() == 1) {  // Ű���尡 ������ ������
                    ch = _getch(); // key ���� �д´�
                    // ESC ������ ���α׷� ����
                    if (ch == ESC) {
                        //exit(0);
                        break;
                    }
                    else {
                        move_character(ch, score_play_1);
                    }

                    Sleep(Delay);

                }

                else if (keep_moving == 1) {

                    move_character(ssh, score_play_1);
                    Sleep(Delay);
                }
            }

            gameover_screen(27, 23);

        case 2:
            gamemenual();


            //while (1) {
            ch = _getch(); // key ���� �д´�
            break;


        case 3:
            return 0;
        }

    }

    return 0;
}