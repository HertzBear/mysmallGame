#include<stdio.h>
#include<conio.h>		//����̨�������
#include<graphics.h>	
#include<time.h>
#define MAX 100		//�ߵ�������

//�����
typedef struct pointXY
{
	int x;
	int y;
}MYPOINT;

HWND hwnd = NULL;	//��ʶһ������

//��
struct mySnake
{
	int num;	//�ߵĽ���
	MYPOINT xy[MAX];
	char position;		//ֻ�Ǳ�ʾ���򣬱�Ǵ�ʱ���н��ķ���
}snake;

//ʳ��
struct myFood
{
	MYPOINT food_XY;
	int flag;		//ʳ����ڵı��
	int eatscore = 0;	//ʳ��ķ���
}food;

//ö�ٷ���
enum mov_Position{right,left,down,up};

//������Ŀ������
//1����ʼ����
void initSnake()
{
	snake.num = 3;
	for (int i = 0, x = 30,y=30; i < snake.num; i++,x+=10)
	{
		snake.xy[snake.num - 1 - i].x = x;
		snake.xy[snake.num - 1 - i].y = y;
	}
	snake.position = right;

	food.flag = 0;	//��ʼ��ʳ��
}
//2��������
void drawSnake()
{
	for (int i = 0; i < snake.num; i++)
	{
		setlinecolor(RED);
		setfillcolor(GREEN);
		fillrectangle(snake.xy[i].x, snake.xy[i].y,snake.xy[i].x+10, snake.xy[i].y+10);
	}
}
//3���ƶ���
void moveSnake()
{
	for (int i = snake.num - 1; i > 0; i--)
	{
		snake.xy[i].x = snake.xy[i - 1].x;
		snake.xy[i].y = snake.xy[i - 1].y;
	}

	switch (snake.position)
	{
	case right:
		snake.xy[0].x += 10;
		break;
	case left:
		snake.xy[0].x -= 10;
		break;
	case down:
		snake.xy[0].y += 10;
		break;
	case up:
		snake.xy[0].y -= 10;
		break;
	}
}

//4�������û�
void ketDown()
{
	char userkey = _getch();	//����������
	switch (userkey)			//����  W:72 S:80 A:75 D:77
	{
	case'w':
	case 'W':
	case 72:
		if (snake.position != down)
		{
			snake.position = up;
		}
		break;
	case 's':
	case 'S':
	case '80':
		if (snake.position != up)
		{
			snake.position = down;
		}
		break;
	case 'a':
	case 'A':
	case 75:
		if (snake.position != right)
		{
			snake.position = left;
		}
		break;
	case 'd':
	case 'D':
	case 77:
		if (snake.position != left)
		{
			snake.position = right;
		}
		break;
	}

}

//��ʼ��ʳ��
void initfood()
{
	food.food_XY.x = rand() % 60 * 10;		//ֻ����10������������ͷ���ܶ���
	food.food_XY.y = rand() % 40 * 10;
	food.flag = 1;
	//ʳ�ﲻ�ܳ������ߵ�����
	for (int i = 0; i < snake.num; i++)
	{
		if (snake.xy[i].x == food.food_XY.x && snake.xy[i].y == food.food_XY.y)
		{
			food.food_XY.x = rand() % 60 * 10;		//ֻ����10������������ͷ���ܶ���
			food.food_XY.y = rand() % 40 * 10;
		}
	}
}

//����ʳ��
void drawFood()
{
	fillrectangle(food.food_XY.x, food.food_XY.y, food.food_XY.x + 10, food.food_XY.y + 10);
}

//��ʳ��
void eatFood()
{
	int f_x = food.food_XY.x;
	int f_y = food.food_XY.y;
	if (snake.xy[0].x==f_x&&snake.xy[0].y==f_y)
	{
		snake.num++;
		food.eatscore += 10;
		food.flag = 0;
	}
}

//�������ж�
int snakeDie()
{
	//��ʾ����
	char grade[100] = { 0 };
	sprintf_s(grade, "%d", food.eatscore);
	setbkmode(TRANSPARENT);		//���ñ�����ʽΪ͸��
	settextcolor(RED);
	outtextxy(580, 20, "������");
	outtextxy(620, 20, grade);
	//�ж�������
	if (snake.xy[0].x > 640 || snake.xy[0].x < 0 || snake.xy[0].y>480 || snake.xy[0].y < 0)
	{
		MessageBox(hwnd, "��Ϸ����", "ײǽ����", 0);			//������Ϣ����
		return 1;
	}

	for (int i = 1; i < snake.num; i++)
	{
		if (snake.xy[0].x == snake.xy[i].x && snake.xy[0].y == snake.xy[i].y)
		{
			MessageBox(hwnd, "��Ϸ����", "ײ�Լ�", 0);			//������Ϣ����
			return 1;
		}
	}
	return 0;
}

int main()
{
	srand((unsigned int)time(NULL));
	hwnd=initgraph(640, 480);
	setbkcolor(YELLOW);
	cleardevice();		//���ú�Ҫˢ��
	initSnake();
	while (1)
	{
		cleardevice();		//ˢ��·��
		if (food.flag == 0)
		{
			initfood();
		}
		drawFood();
		drawSnake();
		if (snakeDie())
			break;
		eatFood();
		moveSnake();
		Sleep(100);		//���ƽ��̵��ٶ�
		while (_kbhit())		//���ڰ�������,���ط���ֵ
		{
			ketDown();
		}
		/*if (snake.xy[0].x == 0 || snake.xy[0].x == 600 || snake.xy[0].y == 0 || snake.xy[0].y == 400)
		{
			break;
		}*/
	}
	closegraph();		//�ͷŴ�����������Ҫ����Դ
	printf("Game Over\n");
	system("pause");
	return 0;
}