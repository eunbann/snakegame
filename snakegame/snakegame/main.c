
#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#pragma warning(disable : 4996)

//*********************************
//��� ����
//*********************************
#define EXT_KEY   0xffffffe0 //Ȯ��Ű �νİ� s
#define KEY_LEFT  0x4b
#define KEY_RIGHT  0x4d
#define KEY_UP   0x48
#define KEY_DOWN  0x50

enum {
	BLACK,      /*  0 : ���� */
	DARK_BLUE,    /*  1 : ��ο� �Ķ� */
	DARK_GREEN,    /*  2 : ��ο� �ʷ� */
	DARK_SKY_BLUE,  /*  3 : ��ο� �ϴ� */
	DARK_RED,    /*  4 : ��ο� ���� */
	DARK_VOILET,  /*  5 : ��ο� ���� */
	DARK_YELLOW,  /*  6 : ��ο� ��� */
	GRAY,      /*  7 : ȸ�� */
	DARK_GRAY,    /*  8 : ��ο� ȸ�� */
	BLUE,      /*  9 : �Ķ� */
	GREEN,      /* 10 : �ʷ� */
	SKY_BLUE,    /* 11 : �ϴ� */
	RED,      /* 12 : ���� */
	VOILET,      /* 13 : ���� */
	YELLOW,      /* 14 : ��� */
	WHITE,      /* 15 : �Ͼ� */
};

//*********************************
//����ü ����
//*********************************
struct BODY {
	int x;
	int y;
	struct BODY* next;
};

//*********************************
//������������
//*********************************
int abs_x, abs_y;
int food_x, food_y;  // ���� �Դ� ����
int speed;  //���� �̵��ӵ�
int score;  //������� ����
int length; //������ ����
int direction;  //���� ���� �Ӹ��� ���ϴ� ����. (����:0, ��:1,����:2,������:3,�Ʒ�:4)
int last_direction; //���� ���ۿ��� ������ ���� //�ѽ��ۿ��� ������ �ι��ٲܽ� �浹 ������� ����
char screen[20][35];
struct BODY* snake_head;
struct BODY* snake_tail;


//*********************************
//�Լ� ����
//*********************************

int make_food(); //���̸� �����Ѵ�.
int free_all();  //���α׷��� �������� �Ҵ�� �޸𸮸� ��� �����Ѵ�.
int move_snake(int dir);
int make_tail(int dir);
int gotoxy(int x, int y);
void SetColor(int color);
int show_gameover();
int show_game_frame();
int strike_check(int x, int y); //�̵��Ϸ��� ��ǥ
int init();     //�������� �ʱ�ȭ
void game_explain(); //���� ����
int MainMenu();//���� ȭ��
int Count(int* Second, int* time); //�ð�����


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
				if (return_tmp == 2) //������ �þ
				{
					return_tmp = make_tail(direction);
				}
				else {
					fflush(stdin);

					return_tmp = move_snake(direction); //�ܼ��� �̵�
				}
				last_direction = direction;
				if (return_tmp == 1)  //�浹�Ҷ�
				{
					show_gameover();
					free_all();
					break;
				}
				if (return_tmp == 2)  //���̸� �Ծ�����
				{
					if (length % 5 == 0)
						speed--;

					if (speed < 1)
						speed = 10;

					length++;
					make_food();
					i = 5; //���ǵ���� ���� �ʱ�ȭ
				}
			}
			SetColor(BLACK);
			gotoxy(77, 23); //Ŀ���̵��Լ�
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

	gotoxy(10, 0);       // 15�ʰ� ������ ��(second) color RED�� ���� 
	if (*Second < 10)
	{
		WHITE;
		printf("%d �ʰ� �������ϴ�.", ++(*Second));
	}
	else
	{
		RED;
		printf("%d ", ++(*Second));
		WHITE;
		printf("�ʰ� �������ϴ�.");
	}

	if (*Second > 12) return 0; // ���ѽð� 20�� ����� ���� �й� 

}

int MainMenu()
{
	int nMenu;
	SetColor(GRAY);
	gotoxy(15, 3);
	printf("�����  ��    ��  �����  ��    ��  �����            ��  �����");
	gotoxy(15, 4);
	printf("��        ���  ��  ��    ��  ��  ��    ��                  ��  ��    ��");
	gotoxy(15, 5);
	printf("�����  ��  ���  �����  ���      �����  �����  ��  �����");
	gotoxy(15, 6);
	printf("      ��  ��  ���  ��    ��  ��  ��    ��                  ��        ��");
	gotoxy(15, 7);
	printf("�����  ��    ��  ��    ��  ��    ��  �����            ��  �����");


	gotoxy(40, 13);  printf("1. ���� ����");
	gotoxy(40, 15); printf("2. ���� ���");
	gotoxy(40, 20); printf("�� �޴��� �����ϼ��� [   ]");
	gotoxy(63, 20); scanf("%d", &nMenu); fflush(stdin);
	return nMenu;
}

void game_explain() {
	char keytemp;

	SetColor(RED);
	gotoxy(15, 3);
	printf("������������������������ ���� ���丮 ����������������������");
	gotoxy(15, 4);
	printf("��            �� �ʳ�..             ��");
	gotoxy(15, 5);
	printf("��    �ڷγ� ���̷����� �ɰ�����    ��");
	gotoxy(15, 6);
	printf("��   ���ۿ� ������ ���ϴ� ��Ȳ..    ��");
	gotoxy(15, 7);
	printf("��   ������ ������ �ķ��� ������    ��");
	gotoxy(15, 8);
	printf("��     ���� ���� ���ؿ;� �Ѵ�!     ��");
	gotoxy(15, 9);
	printf("��     �ڷγ� ���̷����� ����...    ��");
	gotoxy(15, 10);
	printf("��    ��Ʈ���� �ķ��� ���ؿ��� !    ��");
	gotoxy(15, 11);
	printf("������������������������������������������������������������������������");


	SetColor(GRAY);
	gotoxy(15, 20);
	printf("                   ���� : [RIGHT] Ű");

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
	printf("������������������������ ���� ���� ����������������������");
	gotoxy(15, 4);
	printf("�� �ִ��� ���� �ķ��� Ȯ���ϼ���! ��");
	gotoxy(15, 5);
	printf("�� ���� Ű�� �̵� ������ �����ϰ� ��");
	gotoxy(15, 6);
	printf("��      �ķ��� Ȯ���ϸ� ����!     ��");
	gotoxy(15, 7);
	printf("��������������������������������������������������������������������");
	gotoxy(15, 10);
	printf("             - ���� -            ");
	gotoxy(15, 11);
	printf(" �ڱ� ���� ��ų� ���̷��� ���� ����");
	gotoxy(15, 12);
	printf(" ������ ���� ��� ������ ����˴ϴ�.");

	SetColor(GRAY);
	gotoxy(15, 20);
	printf("                   ���� : [RIGHT] Ű");

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
	printf("              [ ���۹� ]");
	gotoxy(15, 5);
	printf("         �̵�: ��, ��, ��, ��");
	gotoxy(15, 8);
	printf("             [ ���� ��� ]");
	gotoxy(16, 10);
	printf("�� ����Ű�� �̿��Ͽ� �ķ�(��) Ȯ���ϱ�");
	gotoxy(18, 11);
	printf("�� ���̸� ���� ������ ���� �����");
	gotoxy(10, 12);
	printf("�� ���̳� �ڽ��� �� �Ϻο� �Ӹ��� �ε����� GAME OVER!");
	gotoxy(15, 20);
	printf("                   ���� ���� : [RIGHT] Ű");

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

	//�������� �ʱ�ȭ
	length = 1;
	abs_x = 2;
	abs_y = 2;
	speed = 25;
	direction = 3;
	last_direction = direction;

	//���ʱⰪ �Է�
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
	printf("������������������������������");
	gotoxy(15, 9);
	printf("��**************************��");
	gotoxy(15, 10);
	printf("��*        GAME OVER       *��");
	gotoxy(15, 11);
	printf("��**************************��");
	gotoxy(15, 12);
	printf("������������������������������");

	Sleep(1000);

	SetColor(RED);
	gotoxy(15, 8);
	printf("������������ �ڷγ� ���� ��Ģ ������������");
	gotoxy(15, 9);
	printf("�� 1. ��ȸ�� �Ÿ� �α� ��Ű�� ��");
	gotoxy(15, 10);
	printf("��     2. ����ũ �����ϱ�     ��");
	gotoxy(15, 11);
	printf("��      3. �� ���� �ı�       ��");
	gotoxy(15, 12);
	printf("������������������������������������������������������������");

	SetColor(GRAY);
	gotoxy(51, 23);
	printf("�ٽ� ���� : [RIGHT] Ű");

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
	printf("��������������������������������������������������������������������������������������������������������������������������������������������������");
	for (i = 2; i < 22; i++)
	{
		gotoxy(0, i);
		printf("��");
		gotoxy(72, i);
		printf("��");
	}
	gotoxy(0, 20 + abs_y);
	printf("��������������������������������������������������������������������������������������������������������������������������������������������������");

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
	if (i == 1) //�浹�Ҷ�
	{
		return 1;
	}

	gotoxy((snake_tail->x * 2) + abs_x, snake_tail->y + abs_y); //�̵����� �ܻ������
	printf("  ");



	gotoxy(77, 23);  //ȭ�� ������ ��������
	fflush(stdout);

	SetColor(GRAY);
	gotoxy((snake_head->x * 2) + abs_x, snake_head->y + abs_y); //�̵����� �Ӹ���� �ܻ������
	printf("��");
	gotoxy(tmp_x * 2 + abs_x, tmp_y + abs_y); //�Ӹ���� ����Ʈ�ϱ�
	printf("��");
	gotoxy(77, 23);  //ȭ�� ������ ��������
	SetColor(BLACK);


	tmp_body = (struct BODY*)malloc(sizeof(struct BODY)); //�Ӹ��κ��� ���ο� ��常���
	tmp_body->x = tmp_x;
	tmp_body->y = tmp_y;
	tmp_body->next = snake_head;
	snake_head = tmp_body;


	while (tmp_body->next != snake_tail)  //������� �����
		tmp_body = tmp_body->next;

	tmp_body->next = NULL;
	free(snake_tail);
	snake_tail = tmp_body;
	return i;
}

int strike_check(int x, int y)
{
	struct BODY* tmp_body;
	if (x < 0 || x>34 || y < 0 || y>19)  //�Ӹ��� ���� �δ�ħ
		return 1;
	tmp_body = snake_head;
	while (tmp_body != NULL)
	{
		if ((tmp_body->x == x) && (tmp_body->y == y))
			return 1;
		tmp_body = tmp_body->next;
	}
	if (food_x == x && food_y == y) //���̸� ������
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

	gotoxy(77, 23);  //ȭ�� ������ ��������
	fflush(stdout);

	SetColor(RED); //�� ����
	gotoxy(food_x * 2 + abs_x, food_y + abs_y);
	printf("��");
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
	if (i == 1) //�浹�Ҷ�
	{
		return 1;
	}





	gotoxy(77, 23);  //ȭ�� ������ ��������
	fflush(stdout);

	SetColor(GRAY);
	gotoxy((snake_head->x * 2) + abs_x, snake_head->y + abs_y); //�̵����� �Ӹ���� �ܻ������
	printf("��");
	gotoxy(tmp_x * 2 + abs_x, tmp_y + abs_y); //�Ӹ���� ����Ʈ�ϱ�
	printf("��");
	gotoxy(77, 23);  //ȭ�� ������ ��������
	SetColor(BLACK);


	tmp_body = (struct BODY*)malloc(sizeof(struct BODY)); //�Ӹ��κ��� ���ο� ��常���
	tmp_body->x = tmp_x;
	tmp_body->y = tmp_y;
	tmp_body->next = snake_head;
	snake_head = tmp_body;



	return i;
}

