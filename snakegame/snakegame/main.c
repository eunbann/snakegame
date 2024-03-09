
#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#pragma warning(disable : 4996)

//*********************************
//상수 선언
//*********************************
#define EXT_KEY   0xffffffe0 //확장키 인식값 s
#define KEY_LEFT  0x4b
#define KEY_RIGHT  0x4d
#define KEY_UP   0x48
#define KEY_DOWN  0x50

enum {
	BLACK,      /*  0 : 검정 */
	DARK_BLUE,    /*  1 : 어두운 파랑 */
	DARK_GREEN,    /*  2 : 어두운 초록 */
	DARK_SKY_BLUE,  /*  3 : 어두운 하늘 */
	DARK_RED,    /*  4 : 어두운 빨강 */
	DARK_VOILET,  /*  5 : 어두운 보라 */
	DARK_YELLOW,  /*  6 : 어두운 노랑 */
	GRAY,      /*  7 : 회색 */
	DARK_GRAY,    /*  8 : 어두운 회색 */
	BLUE,      /*  9 : 파랑 */
	GREEN,      /* 10 : 초록 */
	SKY_BLUE,    /* 11 : 하늘 */
	RED,      /* 12 : 빨강 */
	VOILET,      /* 13 : 보라 */
	YELLOW,      /* 14 : 노랑 */
	WHITE,      /* 15 : 하양 */
};

//*********************************
//구조체 선언
//*********************************
struct BODY {
	int x;
	int y;
	struct BODY* next;
};

//*********************************
//전역변수선언
//*********************************
int abs_x, abs_y;
int food_x, food_y;  // 뱀이 먹는 먹이
int speed;  //뱀의 이동속도
int score;  //점수계산 변수
int length; //꼬리의 길이
int direction;  //현재 뱀의 머리가 향하는 방향. (멈춤:0, 위:1,왼쪽:2,오른쪽:3,아래:4)
int last_direction; //이전 스템에서 움직인 방향 //한스템에서 방향을 두번바꿀시 충돌 디버깅을 위함
char screen[20][35];
struct BODY* snake_head;
struct BODY* snake_tail;


//*********************************
//함수 선언
//*********************************

int make_food(); //먹이를 생성한다.
int free_all();  //프로그램을 끝내기전 할당된 메모리를 모두 해제한다.
int move_snake(int dir);
int make_tail(int dir);
int gotoxy(int x, int y);
void SetColor(int color);
int show_gameover();
int show_game_frame();
int strike_check(int x, int y); //이동하려는 좌표
int init();     //각종변수 초기화
void game_explain(); //게임 설명
int MainMenu();//시작 화면
int Count(int* Second, int* time); //시간제한


int main(int argc, char* argv[])
{
	int nMainMenu = 1;
	int i;
	char keytemp;
	int return_tmp = 0;
	int time, second = 0;


	init();

	gotoxy(77, 23);
	printf(" \b");
	nMainMenu = MainMenu();
	if (nMainMenu == 1)
	{
		show_game_frame();
		make_food();
	
	}
	else if (nMainMenu == 2)
	{
		system("cls");
		game_explain();

	}

	while (1)
	{
		gotoxy(77, 23);
		printf(" \b");
		show_game_frame();
		make_food();

		for (i = 1; 1; i++)
		{
			if (kbhit())
			{
				keytemp = getche();

				if (keytemp == EXT_KEY)
				{
					printf("\b");
					keytemp = getche();
					switch (keytemp)
					{
					case KEY_UP:
						if (last_direction != 4)
							direction = 1;

						break;
					case KEY_LEFT:
						if (last_direction != 3)
							direction = 2;
						break;
					case KEY_RIGHT:
						if (last_direction != 2)
							direction = 3;

						break;
					case KEY_DOWN:
						if (last_direction != 1)
							direction = 4;
						break;

					}

				}
			}

			if (i % speed == 0)
			{
				if (return_tmp == 2) //꼬리가 늘어남
				{
					return_tmp = make_tail(direction);
				}
				else {
					fflush(stdin);

					return_tmp = move_snake(direction); //단순히 이동
				}
				last_direction = direction;
				if (return_tmp == 1)  //충돌할때
				{
					show_gameover();
					free_all();
					break;
				}
				if (return_tmp == 2)  //먹이를 먹었을때
				{
					if (length % 5 == 0)
						speed--;

					if (speed < 1)
						speed = 10;

					length++;
					make_food();
					i = 5; //스피드관련 변수 초기화
				}
			}
			SetColor(BLACK);
			gotoxy(77, 23); //커서이동함수
			Sleep(10);
			SetColor(BLACK);
			gotoxy(77, 23);
		}
		
		init();
	}
	return 0;
}


int Count(int* Second, int* time)
{

	*time = GetTickCount();

	gotoxy(10, 0);       // 15초가 지나면 초(second) color RED로 변경 
	if (*Second < 10)
	{
		WHITE;
		printf("%d 초가 지났습니다.", ++(*Second));
	}
	else
	{
		RED;
		printf("%d ", ++(*Second));
		WHITE;
		printf("초가 지났습니다.");
	}

	if (*Second > 12) return 0; // 제한시간 20초 경과시 술래 패배 

}

int MainMenu()
{
	int nMenu;
	SetColor(GRAY);
	gotoxy(15, 3);
	printf("■■■■  ■    ■  ■■■■  ■    ■  ■■■■            ■  ■■■■");
	gotoxy(15, 4);
	printf("■        ■■  ■  ■    ■  ■  ■    ■                  ■  ■    ■");
	gotoxy(15, 5);
	printf("■■■■  ■  ■■  ■■■■  ■■      ■■■■  ■■■■  ■  ■■■■");
	gotoxy(15, 6);
	printf("      ■  ■  ■■  ■    ■  ■  ■    ■                  ■        ■");
	gotoxy(15, 7);
	printf("■■■■  ■    ■  ■    ■  ■    ■  ■■■■            ■  ■■■■");


	gotoxy(40, 13);  printf("1. 게임 시작");
	gotoxy(40, 15); printf("2. 게임 방법");
	gotoxy(40, 20); printf("▷ 메뉴를 선택하세요 [   ]");
	gotoxy(63, 20); scanf("%d", &nMenu); fflush(stdin);
	return nMenu;
}

void game_explain() {
	char keytemp;

	SetColor(RED);
	gotoxy(15, 3);
	printf("┏━━━━━━━━━━━ 게임 스토리 ━━━━━━━━━━┓");
	gotoxy(15, 4);
	printf("┃            먼 훗날..             ┃");
	gotoxy(15, 5);
	printf("┃    코로나 바이러스가 심각해져    ┃");
	gotoxy(15, 6);
	printf("┃   집밖에 나가지 못하는 상황..    ┃");
	gotoxy(15, 7);
	printf("┃   하지만 집에는 식량이 떨어져    ┃");
	gotoxy(15, 8);
	printf("┃     먹을 것을 구해와야 한다!     ┃");
	gotoxy(15, 9);
	printf("┃     코로나 바이러스를 피해...    ┃");
	gotoxy(15, 10);
	printf("┃    마트에서 식량을 구해오자 !    ┃");
	gotoxy(15, 11);
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");


	SetColor(GRAY);
	gotoxy(15, 20);
	printf("                   다음 : [RIGHT] 키");

	while (1)
	{
		if (kbhit())
		{
			keytemp = getche();

			if (keytemp == EXT_KEY)
			{
				printf("\b");
				keytemp = getche();
				if (keytemp == KEY_RIGHT) {
					break;
				}
			}
		}
	}
	system("cls");

	SetColor(RED);
	gotoxy(15, 3);
	printf("┏━━━━━━━━━━━ 게임 설명 ━━━━━━━━━━┓");
	gotoxy(15, 4);
	printf("┃ 최대한 많은 식량을 확보하세요! ┃");
	gotoxy(15, 5);
	printf("┃ 방향 키로 이동 방향을 조정하고 ┃");
	gotoxy(15, 6);
	printf("┃      식량을 확보하면 성공!     ┃");
	gotoxy(15, 7);
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
	gotoxy(15, 10);
	printf("             - 주의 -            ");
	gotoxy(15, 11);
	printf(" 자기 몸에 닿거나 바이러스 안전 구역");
	gotoxy(15, 12);
	printf(" 밖으로 나갈 경우 게임은 종료됩니다.");

	SetColor(GRAY);
	gotoxy(15, 20);
	printf("                   다음 : [RIGHT] 키");

	while (1)
	{
		if (kbhit())
		{
			keytemp = getche();

			if (keytemp == EXT_KEY)
			{
				printf("\b");
				keytemp = getche();
				if (keytemp == KEY_RIGHT) {
					break;
				}
			}
		}
	}
	system("cls");

	SetColor(GRAY);
	gotoxy(15, 3);
	printf("              [ 조작법 ]");
	gotoxy(15, 5);
	printf("         이동: ←, →, ↑, ↓");
	gotoxy(15, 8);
	printf("             [ 게임 방법 ]");
	gotoxy(16, 10);
	printf("▷ 방향키를 이용하여 식량(◆) 확보하기");
	gotoxy(18, 11);
	printf("▷ 먹이를 먹을 때마다 몸통 길어짐");
	gotoxy(10, 12);
	printf("▷ 벽이나 자신의 몸 일부에 머리를 부딪히면 GAME OVER!");
	gotoxy(15, 20);
	printf("                   게임 시작 : [RIGHT] 키");

	while (1)
	{
		if (kbhit())
		{
			keytemp = getche();

			if (keytemp == EXT_KEY)
			{
				printf("\b");
				keytemp = getche();
				if (keytemp == KEY_RIGHT) {
					break;
				}
			}
		}
	}
	system("cls");
	init();
	show_game_frame();
	make_food();

}


int init()
{
	srand(time(NULL));

	//전역변수 초기화
	length = 1;
	abs_x = 2;
	abs_y = 2;
	speed = 25;
	direction = 3;
	last_direction = direction;

	//뱀초기값 입력
	snake_tail = (struct BODY*)malloc(sizeof(struct BODY));
	snake_tail->x = 5;
	snake_tail->y = 3;
	snake_head = snake_tail;
	snake_tail = (struct BODY*)malloc(sizeof(struct BODY));
	snake_tail->x = 4;
	snake_tail->y = 3;
	snake_tail->next = NULL;
	snake_head->next = snake_tail;
	return 0;
}

int gotoxy(int x, int y)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.Y = y;
	pos.X = x;
	SetConsoleCursorPosition(hConsole, pos);
	return 0;
}

void SetColor(int color)

{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

int show_gameover()
{
	char keytemp;

	SetColor(RED);
	gotoxy(15, 8);
	printf("┏━━━━━━━━━━━━━┓");
	gotoxy(15, 9);
	printf("┃**************************┃");
	gotoxy(15, 10);
	printf("┃*        GAME OVER       *┃");
	gotoxy(15, 11);
	printf("┃**************************┃");
	gotoxy(15, 12);
	printf("┗━━━━━━━━━━━━━┛");

	Sleep(1000);

	SetColor(RED);
	gotoxy(15, 8);
	printf("┏━━━━━ 코로나 예방 수칙 ━━━━━┓");
	gotoxy(15, 9);
	printf("┃ 1. 사회적 거리 두기 지키기 ┃");
	gotoxy(15, 10);
	printf("┃     2. 마스크 착용하기     ┃");
	gotoxy(15, 11);
	printf("┃      3. 손 자주 씻기       ┃");
	gotoxy(15, 12);
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");

	SetColor(GRAY);
	gotoxy(51, 23);
	printf("다시 시작 : [RIGHT] 키");

	while (1)
	{
		if (kbhit())
		{
			keytemp = getche();

			if (keytemp == EXT_KEY)
			{
				printf("\b");
				keytemp = getche();
				if (keytemp == KEY_RIGHT) {
					break;
				}
			}
		}
	}
	fflush(stdin);
	fflush(stdout);
	system("cls");
	return 0;
}

int show_game_frame()
{
	int i;
	SetColor(GRAY);
	system("cls");
	void L_time();
	gotoxy(0, 1);
	printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
	for (i = 2; i < 22; i++)
	{
		gotoxy(0, i);
		printf("┃");
		gotoxy(72, i);
		printf("┃");
	}
	gotoxy(0, 20 + abs_y);
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");

	gotoxy(4, 0);
	printf("SNAKE-19");

	return 0;
}

int move_snake(int dir)
{
	int tmp_x, tmp_y;
	int i;
	struct BODY* tmp_body;


	tmp_x = snake_head->x;
	tmp_y = snake_head->y;
	switch (direction)
	{
	case 1:
		tmp_y--;
		break;
	case 2:
		tmp_x--;
		break;
	case 3:
		tmp_x++;
		break;
	case 4:
		tmp_y++;
		break;
	}
	i = strike_check(tmp_x, tmp_y);
	if (i == 1) //충돌할때
	{
		return 1;
	}

	gotoxy((snake_tail->x * 2) + abs_x, snake_tail->y + abs_y); //이동후의 잔상지우기
	printf("  ");



	gotoxy(77, 23);  //화면 깨짐을 막기위함
	fflush(stdout);

	SetColor(GRAY);
	gotoxy((snake_head->x * 2) + abs_x, snake_head->y + abs_y); //이동후의 머리모양 잔상지우기
	printf("○");
	gotoxy(tmp_x * 2 + abs_x, tmp_y + abs_y); //머리모양 프린트하기
	printf("●");
	gotoxy(77, 23);  //화면 깨짐을 막기위함
	SetColor(BLACK);


	tmp_body = (struct BODY*)malloc(sizeof(struct BODY)); //머리부분의 새로운 노드만들기
	tmp_body->x = tmp_x;
	tmp_body->y = tmp_y;
	tmp_body->next = snake_head;
	snake_head = tmp_body;


	while (tmp_body->next != snake_tail)  //꼬리노드 지우기
		tmp_body = tmp_body->next;

	tmp_body->next = NULL;
	free(snake_tail);
	snake_tail = tmp_body;
	return i;
}

int strike_check(int x, int y)
{
	struct BODY* tmp_body;
	if (x < 0 || x>34 || y < 0 || y>19)  //머리가 벽에 부닥침
		return 1;
	tmp_body = snake_head;
	while (tmp_body != NULL)
	{
		if ((tmp_body->x == x) && (tmp_body->y == y))
			return 1;
		tmp_body = tmp_body->next;
	}
	if (food_x == x && food_y == y) //먹이를 물었음
		return 2;
	return 0;
}

int make_food()
{
	food_y = rand() % 20;
	food_x = rand() % 35;
	while (strike_check(food_x, food_y) == 1)
	{
		food_y = rand() % 20;
		food_x = rand() % 35;
	}

	gotoxy(77, 23);  //화면 깨짐을 막기위함
	fflush(stdout);

	SetColor(RED); //색 지정
	gotoxy(food_x * 2 + abs_x, food_y + abs_y);
	printf("◆");
	SetColor(BLACK);
	return 0;
}


int free_all()
{
	struct BODY* tmp1, * tmp2;

	tmp1 = snake_head;
	tmp2 = snake_head->next;
	while (tmp2 != NULL)
	{
		free(tmp1);
		tmp1 = tmp2;
		tmp2 = tmp2->next;
	}
	return 0;
}

int make_tail(int dir)
{
	int tmp_x, tmp_y;
	int i;
	struct BODY* tmp_body;


	tmp_x = snake_head->x;
	tmp_y = snake_head->y;
	switch (direction)
	{
	case 1:
		tmp_y--;
		break;
	case 2:
		tmp_x--;
		break;
	case 3:
		tmp_x++;
		break;
	case 4:
		tmp_y++;
		break;
	}
	i = strike_check(tmp_x, tmp_y);
	if (i == 1) //충돌할때
	{
		return 1;
	}





	gotoxy(77, 23);  //화면 깨짐을 막기위함
	fflush(stdout);

	SetColor(GRAY);
	gotoxy((snake_head->x * 2) + abs_x, snake_head->y + abs_y); //이동후의 머리모양 잔상지우기
	printf("○");
	gotoxy(tmp_x * 2 + abs_x, tmp_y + abs_y); //머리모양 프린트하기
	printf("●");
	gotoxy(77, 23);  //화면 깨짐을 막기위함
	SetColor(BLACK);


	tmp_body = (struct BODY*)malloc(sizeof(struct BODY)); //머리부분의 새로운 노드만들기
	tmp_body->x = tmp_x;
	tmp_body->y = tmp_y;
	tmp_body->next = snake_head;
	snake_head = tmp_body;



	return i;
}


