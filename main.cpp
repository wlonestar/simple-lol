#include <iostream>
#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <mmstream.h>
#include<time.h>
#pragma comment(lib, "Winmm.lib")

#define PI 3.14159265359
#define WIDTH 1520
#define HEIGHT 785
#define sn_width 91//人物宽度
#define sn_height 104//人物高度
#define bullet_width 19*1.5
#define bullet_height 39*1.5

IMAGE background;
IMAGE begin_menu;
IMAGE begin_option;
IMAGE option;
IMAGE sn;
IMAGE sn_up;
IMAGE sn_down;
IMAGE sn_left;
IMAGE sn_right;
IMAGE timo;
IMAGE bullets;
IMAGE bullet_up;
IMAGE bullet_down;
IMAGE bullet_left;
IMAGE bullet_right;
IMAGE zed;
IMAGE yasuo;

static int Enmax = 1;
static int Ennum = 1;

// 载入PNG图并去透明部分
void drawAlpha(IMAGE* picture, int  picture_x, int picture_y) //x为载入图片的X坐标，y为Y坐标
{
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD* src = GetImageBuffer(picture); //获取picture的显存指针
	int src_width = picture->getwidth();
	int src_height = picture->getheight();
	int dst_width = getwidth();
	int dst_height = getheight();
	int im_width, im_height;
	if (picture_x + src_width > dst_width)
		im_width = dst_width - picture_x;
	else
		im_width = src_width;
	if (picture_y + src_height > dst_height)
		im_height = dst_height - picture_y;
	else
		im_height = src_height;
	if (picture_x < 0)
	{
		src = src - picture_x;
		im_width = im_width + picture_x;
		picture_x = 0;
	}
	if (picture_y < 0)
	{
		src = src - src_width * picture_y;
		im_height = im_height + picture_y;
		picture_y = 0;
	}
	dst = dst + dst_width * picture_y + picture_x;
	for (int i = 0; i < im_height; i++)
	{
		for (int j = 0; j < im_width; j++)
		{
			int src_r = ((src[j] & 0xff0000) >> 16);
			int src_g = ((src[j] & 0xff00) >> 8);
			int src_b = src[j] & 0xff;
			int src_a = ((src[j] & 0xff000000) >> 24);
			int dst_r = ((dst[j] & 0xff0000) >> 16);
			int dst_g = ((dst[j] & 0xff00) >> 8);
			int dst_b = dst[j] & 0xff;
			dst[j] = ((src_r + dst_r * (255 - src_a) / 255) << 16) | ((src_g + dst_g * (255 - src_a) / 255) << 8) | (src_b + dst_b * (255 - src_a) / 255);
		}
		dst = dst + dst_width;
		src = src + src_width;
	}
}

//载入图片
void loadimage()
{
	loadimage(&background, _T("./images/bk.jpg"));
	loadimage(&begin_menu, _T("./images/menu.jpg"));
	loadimage(&begin_option, _T("./images/begin_option.png"));
	loadimage(&option, _T("./images/option.jpg"), WIDTH, HEIGHT);
	loadimage(&sn_up, _T("./images/sn_up.png"), sn_width, sn_height);
	loadimage(&sn_down, _T("./images/sn_down.png"), sn_width, sn_height);
	loadimage(&sn_left, _T("./images/sn_left.png"), sn_width, sn_height);
	loadimage(&sn_right, _T("./images/sn_right.png"), sn_width, sn_height);
	loadimage(&timo, _T("./images/timo.png"), 125, 100);
	loadimage(&zed, _T("./images/zed.png"), 359*0.75, 256*0.75);
	loadimage(&yasuo, _T("./images/yasuo.png"));
	loadimage(&bullet_up, _T("./images/bullet_up.png"), bullet_width, bullet_height);
	loadimage(&bullet_down, _T("./images/bullet_down.png"), bullet_width, bullet_height);
	loadimage(&bullet_left, _T("./images/bullet_left.png"), bullet_height, bullet_width);
	loadimage(&bullet_right, _T("./images/bullet_right.png"), bullet_height, bullet_width);
}

double ez_x = WIDTH / 2;
double ez_y = HEIGHT / 2;
double ez_v = 1.2;//速度
int is = 0;
int eis[6] = { 1 };
int Enexist[6] = { 0 };
int score = 0;
char ez_direction = 'w';
char eze = 'w';
int iseze = 1;
int eif = 1;
int alive = 1;
int bulldis = 1;

double en_x[6] = { 10000 }, en_y[6] = { 10000 };//当前位置
double x[6], y[6];//距离之差
double encos[6], ensin[6];
double en_vx[6], en_vy[6];
double en_v[6] = { 0.3 };

double eza_v;
double eza_x, eza_y;
double eza_l = 0;
double eza_dir = 'w';

class Hero {
public:

	void ShowHero() 
	{
		drawAlpha(&sn, ez_x, ez_y);
	}

	void UpdateHero()
	{
		int i;
		if (ez_direction == 'w')
			sn = sn_up;
		else if (ez_direction == 'a')
			sn = sn_left;
		else if (ez_direction == 's') 
			sn = sn_down;
		else if (ez_direction == 'd') 
			sn = sn_right;
		for (i = 0;i < 6;i++)
		{
			if (sqrt((ez_x - en_x[i]) * (ez_x - en_x[i]) + (ez_y - en_y[i]) * (ez_y - en_y[i]) <= 1000))
				alive = 0;
		}
	}

	void ezE()
	{
		if ((GetAsyncKeyState(0x45) & 0x8000) && iseze == 1 && score>7)
		{
			if (eze == 'a')
			{
				ez_x -= 200;
				iseze = 0;
			}
			else if (eze == 'd')
			{
				ez_x += 200;
				iseze = 0;
			}
			else if (eze == 'w')
			{
				ez_y -= 200;
				iseze = 0;
			}
			else if (eze == 's')
			{
				ez_y += 200;
				iseze = 0;
			}
		}
	}

	void MoveHero()
	{
		if (!is)
			eza_dir = ez_direction;
		if (iseze)
			eze = ez_direction;
		ezE();
		if ((GetAsyncKeyState(0x41) & 0x8000)) //a
		{
			ez_x -= ez_v;
			ez_direction = 'a';
			if (ez_x <= 0)
			{
				ez_direction = 'd';
				ez_x += ez_v;
				ez_direction = 'a';
			}
		}
		if ((GetAsyncKeyState(0x44) & 0x8000)) //d
		{
			ez_x += ez_v;
			ez_direction = 'd';
			if (ez_x >= WIDTH - double(sn_width))
			{
				ez_direction = 'a';
				ez_x -= ez_v;
				ez_direction = 'd';
			}
		}
		if (GetAsyncKeyState(0x57) & 0x8000) //w
		{
			ez_y -= ez_v;
			ez_direction = 'w';
			if (ez_y <= 0)
			{
				ez_direction = 's';
				ez_y += ez_v;
				ez_direction = 'w';
			}
		}
		if ((GetAsyncKeyState(0x53) & 0x8000)) //s
		{
			ez_y += ez_v;
			ez_direction = 's';
			if (ez_y >= HEIGHT - double(sn_height))
			{
				ez_direction = 'w';
				ez_y -= ez_v;
				ez_direction = 's';
			}
		}
	}
};

class Enemy {
public:

	void InitEnemy()
	{
		Enexist[0] = 1;
		int i;
		for (i = 0;i < 6;i++)
		{
			if (i % 2 == 1)
				en_x[i] = -50;
			else
				en_x[i] = 1570;
			en_y[i] = rand() % 785;
			en_v[i] = (rand()%5) / 10 + 0.2;
		}
	}

	void MoveEnemy()
	{
		int i = 0;
		for (i = 0;i < 6;i++)
		{
			if (Enexist[i])
			{
				encos[i] = (ez_x - en_x[i]) / sqrt((ez_x - en_x[i]) * (ez_x - en_x[i]) + (ez_y - en_y[i]) * (ez_y - en_y[i]));
				ensin[i] = (ez_y - en_y[i]) / sqrt((ez_x - en_x[i]) * (ez_x - en_x[i]) + (ez_y - en_y[i]) * (ez_y - en_y[i]));
				en_vx[i] = encos[i] * en_v[i];
				en_vy[i] = ensin[i] * en_v[i];
				en_x[i] += en_vx[i];
				en_y[i] += en_vy[i];
				if (eis[i] == 0)
				{
					int p = rand() % 4;
					if (p % 4 == 1)
					{
						en_x[i] = 1770;
						en_y[i] = rand() % 785;
						eis[i] = 1;
					}
					else if (p % 4 == 2)
					{
						en_x[i] = -170;
						en_y[i] = rand() % 785;
						eis[i] = 1;
					}
					else if (p % 4 == 3)
					{
						en_y[i] = -170;
						en_x[i] = rand() % 1520;
						eis[i] = 1;
					}
					else if (p % 4 == 0)
					{
						en_y[i] = 950;
						en_x[i] = rand() % 1520;
						eis[i] = 1;
					}
					if(en_v[i]<=1.05)
						en_v[i] += 0.1;
				}
			}
			else
				break;
		}
	}

	void attack()
	{
		int j;
		double disx, disy;
		for (j = 0;j < Enmax;j++)
		{
			if (j % 3 == 0)
			{
				disx = abs(eza_x - en_x[j] - 47.5);
				disy = abs(eza_y - en_y[j] - 30);
			}
			else if (j % 3 == 1)
			{
				disx = abs(eza_x - en_x[j] - 120);
				disy = abs(eza_y - en_y[j] - 66);
			}
			else if (j % 3 == 2)
			{
				disx = abs(eza_x - en_x[j] - 106);
				disy = abs(eza_y - en_y[j] - 55);
			}
			if (disx <= 40 && disy <= 30 && j%3==0)
			{
				is = 0;
				eis[j] = 0;
				eza_x = -1000;
				eza_y = -1000;
				score++;
				bulldis = 0;
			}
			if (disx <= 50 && disy <= 45 && j % 3 == 1)
			{
				is = 0;
				eis[j] = 0;
				eza_x = -1000;
				eza_y = -1000;
				score++;
				bulldis = 0;
			}
			if (disx <= 50 && disy <= 45 && j % 3 == 2)
			{
				is = 0;
				eis[j] = 0;
				eza_x = -1000;
				eza_y = -1000;
				score++;
				bulldis = 0;
			}
		}
	}

	void ShowEnemy()
	{
		int i;
		for (i = 0;i < 6;i++)
		{
			if (Enexist[i] && i%3==0)
				drawAlpha(&timo, en_x[i], en_y[i]);
			else if (Enexist[i] && i % 3 == 1)
				drawAlpha(&zed, en_x[i], en_y[i]);
			else if (Enexist[i] && i % 3 == 2)
				drawAlpha(&yasuo, en_x[i], en_y[i]);
		}
	}

};

class Bullet {
public:

	void InitBullet()
	{
		eza_v = 4.2;
		eza_x = ez_x;
		eza_y = ez_y;
	}

	void ShowBullet() 
	{
		if (is)
			drawAlpha(&bullets, eza_x, eza_y);
	}


	void UpdataBullet()
	{
		if (eza_dir == 'a')
		{
			eza_x -= eza_v;
			bullets = bullet_left;
		}
		else if (eza_dir == 'd')
		{
			eza_x += eza_v;
			bullets = bullet_right;
		}
		else if (eza_dir == 'w')
		{
			eza_y -= eza_v;
			bullets = bullet_up;
		}
		else if (eza_dir == 's')
		{
			eza_y += eza_v;
			bullets = bullet_down;
		}
		if (is)
			eza_l += eza_v;
		if (!bulldis)
		{
			eza_l = 0;
			bulldis = 1;
		}
		else if (eza_l >= 600)
		{
			is = 0;
			eza_l = 0;
			eza_x = -1000;
			eza_y = -1000;
		}
	}

	void MoveBullet()
	{
		if ((GetAsyncKeyState(0x51) & 0x8000) && is == 0) //q
		{
			is = 1;
			eza_x = ez_x + 28;
			eza_y = ez_y + 28;
		}
	}

};

Hero hero;
Enemy enemy;
Bullet bullet;

void StartUp()
{
	initgraph(WIDTH, HEIGHT);
	changetitle();
	loadimage();
	enemy.InitEnemy();
	bullet.InitBullet();
	BeginBatchDraw();
}

void Show()
{
	putimage(0, 0, &background);
	hero.ShowHero();
	enemy.ShowEnemy();
	bullet.UpdataBullet();
	bullet.ShowBullet();
	FlushBatchDraw();
}

void UpdateWithoutInput()
{
	static int lastSecond = 0;//记录前一次运行的秒数
	static int nowSecond = 0;//记录当前运行了多少秒
	static clock_t start = clock();//记录第一次运行时刻
	clock_t now = clock();//获得当前时刻
	static int estartsecond = 0;
	static int elastsecond = 0;
	if (nowSecond == lastSecond + 5 && Enmax <= 6)
	{
		lastSecond = nowSecond;
		Enmax += 1;
		Enexist[Enmax - 1] = 1;
	}
	nowSecond = (int(now - start) / CLOCKS_PER_SEC);
	if (iseze == 0)
	{
		elastsecond = nowSecond;
		if (eif)
		{
			estartsecond = nowSecond;
			eif = 0;
		}
		if (elastsecond == estartsecond + 5)
		{
			iseze = 1;
			eif = 1;
		}
	}
	static int waitIndex = 1;
	waitIndex++;
	if (waitIndex == 2)
	{
		hero.MoveHero();
		enemy.MoveEnemy();
		bullet.MoveBullet();
		enemy.attack();
		waitIndex = 1;
	}
}

void UpdateWithInput()
{
	hero.UpdateHero();
}

int main(void)
{
	StartUp();
	while (alive)
	{
		Show();
		UpdateWithInput();
		UpdateWithoutInput();
	}
	return 0;
}