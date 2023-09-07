#include<stdio.h>
#include<conio.h>		//控制台输入输出
#include<graphics.h>	
#include<time.h>
#define MAX 100		//蛇的最大节数

//坐标点
typedef struct pointXY
{
	int x;
	int y;
}MYPOINT;

HWND hwnd = NULL;	//标识一个窗口

//蛇
struct mySnake
{
	int num;	//蛇的节数
	MYPOINT xy[MAX];
	char position;		//只是表示方向，标记此时蛇行进的方向
}snake;

//食物
struct myFood
{
	MYPOINT food_XY;
	int flag;		//食物存在的标记
	int eatscore = 0;	//食物的分数
}food;

//枚举方向
enum mov_Position{right,left,down,up};

//整个项目的流程
//1、初始化蛇
void initSnake()
{
	snake.num = 3;
	for (int i = 0, x = 30,y=30; i < snake.num; i++,x+=10)
	{
		snake.xy[snake.num - 1 - i].x = x;
		snake.xy[snake.num - 1 - i].y = y;
	}
	snake.position = right;

	food.flag = 0;	//初始化食物
}
//2、绘制蛇
void drawSnake()
{
	for (int i = 0; i < snake.num; i++)
	{
		setlinecolor(RED);
		setfillcolor(GREEN);
		fillrectangle(snake.xy[i].x, snake.xy[i].y,snake.xy[i].x+10, snake.xy[i].y+10);
	}
}
//3、移动蛇
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

//4、按键用户
void ketDown()
{
	char userkey = _getch();	//不回显输入
	switch (userkey)			//键码  W:72 S:80 A:75 D:77
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

//初始化食物
void initfood()
{
	food.food_XY.x = rand() % 60 * 10;		//只能是10的整数倍，蛇头才能对齐
	food.food_XY.y = rand() % 40 * 10;
	food.flag = 1;
	//食物不能出现在蛇的身上
	for (int i = 0; i < snake.num; i++)
	{
		if (snake.xy[i].x == food.food_XY.x && snake.xy[i].y == food.food_XY.y)
		{
			food.food_XY.x = rand() % 60 * 10;		//只能是10的整数倍，蛇头才能对齐
			food.food_XY.y = rand() % 40 * 10;
		}
	}
}

//绘制食物
void drawFood()
{
	fillrectangle(food.food_XY.x, food.food_XY.y, food.food_XY.x + 10, food.food_XY.y + 10);
}

//吃食物
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

//蛇死亡判断
int snakeDie()
{
	//显示分数
	char grade[100] = { 0 };
	sprintf_s(grade, "%d", food.eatscore);
	setbkmode(TRANSPARENT);		//设置背景方式为透明
	settextcolor(RED);
	outtextxy(580, 20, "分数：");
	outtextxy(620, 20, grade);
	//判断蛇死亡
	if (snake.xy[0].x > 640 || snake.xy[0].x < 0 || snake.xy[0].y>480 || snake.xy[0].y < 0)
	{
		MessageBox(hwnd, "游戏结束", "撞墙死亡", 0);			//弹出消息窗口
		return 1;
	}

	for (int i = 1; i < snake.num; i++)
	{
		if (snake.xy[0].x == snake.xy[i].x && snake.xy[0].y == snake.xy[i].y)
		{
			MessageBox(hwnd, "游戏结束", "撞自己", 0);			//弹出消息窗口
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
	cleardevice();		//设置后要刷新
	initSnake();
	while (1)
	{
		cleardevice();		//刷掉路径
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
		Sleep(100);		//控制进程的速度
		while (_kbhit())		//存在按键操作,返回非零值
		{
			ketDown();
		}
		/*if (snake.xy[0].x == 0 || snake.xy[0].x == 600 || snake.xy[0].y == 0 || snake.xy[0].y == 400)
		{
			break;
		}*/
	}
	closegraph();		//释放创建窗口所需要的资源
	printf("Game Over\n");
	system("pause");
	return 0;
}