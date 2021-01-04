#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <Windows.h>
using namespace std;
#pragma comment(lib, "Winmm.lib")

void StartUp();
void Show();
void UpdateWithoutInput();
void UpdateWithInput();

#define PI 3.14159265359
#define WIDTH 1520
#define HEIGHT 785
#define sn_width 91*0.9 //人物宽度
#define sn_height 104 *0.9//人物高度
#define yasuo_width 100
#define yasuo_height 100
#define timo_width 72
#define timo_height 60
#define zed_width 89*1.5
#define zed_height 81*1.5
#define bullet_width 19 * 1.5
#define bullet_height 39 * 1.5

IMAGE game_bk;
IMAGE start_menu;
IMAGE option_bk;
IMAGE tips_bk;
IMAGE rank_bk;
IMAGE pause_bk;
IMAGE gameover_bk;
IMAGE play;
IMAGE play_cover;
IMAGE tips;
IMAGE tips_cover;
IMAGE continu;
IMAGE continu_cover;
IMAGE quit;
IMAGE quit_cover;
IMAGE rank1;
IMAGE rank_cover;
IMAGE backtomenu;
IMAGE backtomenu_cover;
IMAGE sn;
IMAGE sn_up;
IMAGE sn_down;
IMAGE sn_left;
IMAGE sn_right;
IMAGE timo;
IMAGE timo_upleft;
IMAGE timo_upright;
IMAGE timo_downleft;
IMAGE timo_downright;
IMAGE yasuo;
IMAGE yasuo_upleft;
IMAGE yasuo_upright;
IMAGE yasuo_downleft;
IMAGE yasuo_downright;
IMAGE zed;
IMAGE zed_upleft;
IMAGE zed_upright;
IMAGE zed_downleft;
IMAGE zed_downright;
IMAGE bullets;
IMAGE bullet_up;
IMAGE bullet_down;
IMAGE bullet_left;
IMAGE bullet_right;
IMAGE fire;
IMAGE fire_upleft;
IMAGE fire_upright;
IMAGE fire_downleft;
IMAGE fire_downright;

int click_start = 0;
int click_tips = 0;
int click_rank = 0;
int click_quit = 0;
int click_continue = 0;
int click_backtomenu = 0;
int start = 0;
int pause_exit = 0;
double ez_x = WIDTH / 2;
double ez_y = HEIGHT / 2;
int score = 0;
int is = 0;
int eis[6] = { 1 };
int eif = 1; //e是否可用
int qif = 1;
int alive = 1;
int iseze = 1;
int isezq = 1;
double en_x[6] = { 10000 }, en_y[6] = { 10000 }; //当前位置
int Enexist[6] = { 0 };
double eza_x, eza_y;
double eza_dir = 'w';
int bulldis = 1;
static int Enmax = 1;
static int Ennum = 1;

MOUSEMSG m;

// 载入PNG图并去透明部分
void drawAlpha(IMAGE* picture, int picture_x, int picture_y) //x为载入图片的X坐标，y为Y坐标
{
	DWORD* dst = GetImageBuffer(); // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
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
	loadimage(&game_bk, _T("./images/game_bk.jpg"));
	loadimage(&start_menu, _T("./images/begin_menu.jpg"));
	loadimage(&option_bk, _T("./images/option_bk.jpg"));
	loadimage(&tips_bk, _T("./images/tips_bk.jpg"));
	loadimage(&rank_bk, _T("./images/rank_bk.jpg"));
	loadimage(&pause_bk, _T("./images/pause_bk.jpg"));
	loadimage(&gameover_bk, _T("./images/gameover_bk.jpg"));
	loadimage(&play, _T("./images/start.jpg"), 347, 100);
	loadimage(&play_cover, _T("./images/start_cover.jpg"), 347, 100);
	loadimage(&tips, _T("./images/tips.jpg"), 278, 100);
	loadimage(&tips_cover, _T("./images/tips_cover.jpg"), 278, 100);
	loadimage(&continu, _T("./images/continue.jpg"), 540, 100);
	loadimage(&continu_cover, _T("./images/continue_cover.jpg"), 540, 100);
	loadimage(&quit, _T("./images/quit.jpg"), 310, 110);
	loadimage(&quit_cover, _T("./images/quit_cover.jpg"), 310, 110);
	loadimage(&rank1, _T("./images/rank.jpg"), 276, 100);
	loadimage(&rank_cover, _T("./images/rank_cover.jpg"), 276, 100);
	loadimage(&backtomenu, _T("./images/backtomenu.jpg"), 815, 100);
	loadimage(&backtomenu_cover, _T("./images/backtomenu_cover.jpg"), 815, 100);
	loadimage(&sn_up, _T("./images/sn_up.png"), sn_width, sn_height);
	loadimage(&sn_down, _T("./images/sn_down.png"), sn_width, sn_height);
	loadimage(&sn_left, _T("./images/sn_left.png"), sn_width, sn_height);
	loadimage(&sn_right, _T("./images/sn_right.png"), sn_width, sn_height);
	loadimage(&bullet_up, _T("./images/bullet_up.png"), bullet_width, bullet_height);
	loadimage(&bullet_down, _T("./images/bullet_down.png"), bullet_width, bullet_height);
	loadimage(&bullet_left, _T("./images/bullet_left.png"), bullet_height, bullet_width);
	loadimage(&bullet_right, _T("./images/bullet_right.png"), bullet_height, bullet_width);
	loadimage(&fire_upleft, _T("./images/fire_upleft.png"));
	loadimage(&fire_upright, _T("./images/fire_upright.png"));
	loadimage(&fire_downleft, _T("./images/fire_downleft.png"));
	loadimage(&fire_downright, _T("./images/fire_downright.png"));
	loadimage(&timo_upleft, _T("./images/timo_upleft.png"), timo_width, timo_height);
	loadimage(&timo_upright, _T("./images/timo_upright.png"), timo_width, timo_height);
	loadimage(&timo_downleft, _T("./images/timo_downleft.png"), timo_width, timo_height);
	loadimage(&timo_downright, _T("./images/timo_downright.png"), timo_width, timo_height);
	loadimage(&yasuo_upleft, _T("./images/yasuo_upleft.png"), yasuo_width, yasuo_height);
	loadimage(&yasuo_upright, _T("./images/yasuo_upright.png"), yasuo_width, yasuo_height);
	loadimage(&yasuo_downleft, _T("./images/yasuo_downleft.png"), yasuo_width, yasuo_height);
	loadimage(&yasuo_downright, _T("./images/yasuo_downright.png"), yasuo_width, yasuo_height);
	loadimage(&zed_upleft, _T("./images/zed_upleft.png"), zed_width, zed_height);
	loadimage(&zed_upright, _T("./images/zed_upright.png"), zed_width, zed_height);
	loadimage(&zed_downleft, _T("./images/zed_downleft.png"), zed_width, zed_height);
	loadimage(&zed_downright, _T("./images/zed_downright.png"), zed_width, zed_height);
}

class Menu {
public:
	void Tips()
	{
		putimage(0, 0, &tips_bk);
		FlushBatchDraw();
		Sleep(2);
		system("pause");
	}

	void Quit()
	{
		EndBatchDraw();
		closegraph();
		exit(0);
	}

	void Rank()
	{
		FILE* fp;
		int highest;
		TCHAR c[10];
		fp = fopen(".\\rank.zsc", "r");
		fscanf(fp, "TOP：%d", &highest);
		fclose(fp);
		putimage(200, 150, &rank_bk);
		settextstyle(30, 0, _T("宋体"));
		outtextxy(WIDTH / 2 - 110, HEIGHT / 2, _T("TOP："));
		outtextxy(WIDTH / 2, HEIGHT / 2, c);
		FlushBatchDraw();
		settextstyle(15, 0, _T("宋体"));
		Sleep(100);
	}

	void BeginMenu()
	{
		while (start == 0)
		{
			MenuInput();
			MenuShow();
		}
	}

	void MenuShow()
	{
		putimage(0, 0, &start_menu);
		if (click_start == 1) //start
			putimage(700, 150, &play_cover);
		else
			putimage(700, 150, &play);
		if (click_tips == 1) //tips
			putimage(700, 300, &tips_cover);
		else
			putimage(700, 300, &tips);
		if (click_rank == 1) //rank
			putimage(700, 450, &rank_cover);
		else
			putimage(700, 450, &rank1);
		if (click_quit == 1) //quit
			putimage(700, 600, &quit_cover);
		else
			putimage(700, 600, &quit);
		FlushBatchDraw();
		Sleep(2);
	}

	void MenuInput()
	{
		while (MouseHit())
		{
			m = GetMouseMsg();
			if (m.x >= 700 && m.x <= 700 + 347 && m.y >= 150 && m.y <= 250) // 点击start
			{
				click_start = 1;
				if (m.uMsg == WM_LBUTTONDOWN)
					start = 1;
			}
			else
				click_start = 0;
			if (m.x >= 700 && m.x <= 700 + 278 && m.y >= 300 && m.y <= 400) // 点击tips
			{
				click_tips = 1;
				if (m.uMsg == WM_LBUTTONDOWN)
					Tips();     // 打开提示
			}
			else
				click_tips = 0;
			if (m.x >= 700 && m.x <= 700 + 376 && m.y >= 450 && m.y <= 550) // 点击rank
			{
				click_rank = 1;
				if (m.uMsg == WM_LBUTTONDOWN)
					Rank();   //  打开rank
			}
			else
				click_rank = 0;
			if (m.x >= 700 && m.x <= 700 + 334 && m.y >= 600 && m.y <= 700) // 点击quit
			{
				click_quit = 1;
				if (m.uMsg == WM_LBUTTONDOWN)
					Quit();   //  退出游戏
			}
			else
				click_quit = 0;
		}
	}

};
Menu menu;

class Gameover {
public:
	void GameoverShow()
	{
		putimage(0, 0, &gameover_bk);
		if (click_quit == 1) // quit菜单
			putimage(200, 450, &quit_cover);               
		else
			putimage(200, 450, &quit);
		if (click_backtomenu == 1)
			putimage(200, 300, &backtomenu_cover);              
		else
			putimage(200, 300, &backtomenu);
		FlushBatchDraw();
	}

	void GameoverRank()
	{
		FILE* fp;
		int rank_old;   // 之前的纪录
		fp = fopen("./rank.zsc", "r");
		fscanf(fp, "TOP：%d", &rank_old);
		fclose(fp);
		if (score > rank_old) {
			settextcolor(BLUE);  //更改字体样式
			settextstyle(45, 0, _T("宋体"));
			outtextxy(WIDTH / 2 - 170, HEIGHT / 2 - 40, _T("打破纪录"));
			settextcolor(YELLOW);  //更改字体样式
			settextstyle(15, 0, _T("宋体"));
			FlushBatchDraw();
			fp = fopen("./rank.zsc", "w");
			fprintf(fp, "TOP：%d", score);
			fclose(fp);
			Sleep(5000);
		}
	}

	void BeginGameover()
	{
		int n = 0;
		if (alive == 0) // 生命值为0
		{
			Sleep(1000);
			while (1)
			{
				GameoverShow(n);
				GameoverInput();
			}
		}
	}

	void GameoverInput()
	{
		while (MouseHit())
		{
			m = GetMouseMsg();
			if (m.x >= 200 && m.x <= 200 + 334 && m.y >= 450 && m.y <= 550) // 点击quit
			{
				click_quit = 1;
				if (m.uMsg == WM_LBUTTONDOWN)
					GameoverQuit();   //  退出游戏
			}
			else
				click_quit = 0;
			if (m.x >= 200 && m.x <= 200 + 815 && m.y >= 300 && m.y <= 400) // 点击backtomenu
			{
				click_backtomenu = 1;
				if (m.uMsg == WM_LBUTTONDOWN)
				{
					GameoverBackMenu();
					start = 0;
				}
			}
			else
				click_backtomenu = 0;
		}
	}

	void GameoverBackMenu() // 回到主菜单
	{
		StartUp();
		menu.BeginMenu();
		while (1)
		{
			Show();
			UpdateWithoutInput();
			UpdateWithInput();
		}
	}

	void GameoverQuit()
	{
		EndBatchDraw();
		closegraph();
		exit(0);
	}

};
Gameover gameover;

class Pause {
public:
	void BeginPause()
	{
		while (pause_exit == 0)
		{
			PauseShow();
			PauseInput();
		}
		pause_exit = 0;
	}

	void PauseShow()
	{
		putimage(0, 0, &pause_bk);
		if (click_tips == 1) //tips
			putimage(200, 300, &tips_cover);
		else
			putimage(200, 300, &tips);
		if (click_continue == 1) //continue
			putimage(200, 450, &continu_cover);
		else
			putimage(200, 450, &continu);
		if (click_backtomenu == 1) //backtomenu
			putimage(200, 600, &backtomenu_cover);
		else
			putimage(200, 600, &backtomenu);
		FlushBatchDraw();
	}

	void PauseInput()
	{
		while (MouseHit())
		{
			m = GetMouseMsg();
			if (m.x >= 200 && m.x <= 200 + 278 && m.y >= 300 && m.y <= 400) // 点击tips
			{
				click_tips = 1;
				if (m.uMsg == WM_LBUTTONDOWN)
					menu.Tips();     // 打开提示
			}
			else
				click_tips = 0;
			if (m.x >= 200 && m.x <= 200 + 540 && m.y >= 450 && m.y <= 550) // 点击continue
			{
				click_continue = 1;
				if (m.uMsg == WM_LBUTTONDOWN)
					pause_exit = 1;
			}
			else
				click_continue = 0;
			if (m.x >= 200 && m.x <= 200 + 815 && m.y >= 600 && m.y <= 700) // 点击backtomenu
			{
				click_backtomenu = 1;
				if (m.uMsg == WM_LBUTTONDOWN)
					gameover.GameoverBackMenu();
			}
			else
				click_backtomenu = 0;
		}
	}

};
Pause pause;

class Hero {
private:
	double ez_v = 1; //速度
	char ez_direction = 'w';
	char eze = 'w'; //技能1：闪现

public:
	void ShowHero()
	{
		drawAlpha(&sn, ez_x, ez_y);
	}

	void UpdateHero()
	{
		int i
		if (ez_direction == 'w')
			sn = sn_up;
		else if (ez_direction == 'a')
			sn = sn_left;
		else if (ez_direction == 's')
			sn = sn_down;
		else if (ez_direction == 'd')
			sn = sn_right;
		for (i = 0; i < 6; i++)
			if (sqrt((ez_x - en_x[i]) * (ez_x - en_x[i]) + (ez_y - en_y[i]) * (ez_y - en_y[i]) <= 1000))
				alive = 0;
	}

	void ezE()
	{
		if ((GetAsyncKeyState(0x45) & 0x8000) && iseze == 1 && score > 7) //e
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

	void ShowSkill()
	{
		setbkmode(TRANSPARENT);
		settextcolor(RGB(255, 255, 255));
		settextstyle(20, 0, _T("consolar"));
		outtextxy(180, 25, _T("E"));

		if (iseze == 1)
		{
			setlinecolor(RGB(0, 0, 0));
			setfillcolor(RGB(0, 255, 0));
			fillrectangle(30, 30, 170, 40);
		}
	}

};
Hero hero;

class Enemy {
private:
	double x[6], y[6]; //距离之差
	double encos[6], ensin[6];
	double en_vx[6], en_vy[6];
	double en_v[6] = { 0.3 };

public:
	void InitEnemy()
	{
		Enexist[0] = 1;
		for (int i = 0; i < 6; i++)
		{
			if (i % 2 == 1)
				en_x[i] = -50;
			else
				en_x[i] = 1570;
			en_y[i] = rand() % 785;
			en_v[i] = (rand() % 2) / 10 + 0.3;
		}
	}

	void MoveEnemy()
	{
		for (int i = 0; i < 6; i++)
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
					if (en_v[i] <= 0.85)
						en_v[i] += 0.066;
				}
			}
			else
				break;
		}
	}

	void attack()
	{
		double disx, disy;
		for (int j = 0; j < Enmax; j++)
		{
			if (j % 3 == 0)
			{
				disx = abs(eza_x - en_x[j] - 36);
				disy = abs(eza_y - en_y[j] - 30);
			}
			else if (j % 3 == 1)
			{
				disx = abs(eza_x - en_x[j] - 45 * 1.5);
				disy = abs(eza_y - en_y[j] - 60);
			}
			else if (j % 3 == 2)
			{
				disx = abs(eza_x - en_x[j] - 50);
				disy = abs(eza_y - en_y[j] - 50);
			}
			if (disx <= 36 && disy <= 30 && j % 3 == 0)
			{
				is = 0;
				eis[j] = 0;
				eza_x = -1000;
				eza_y = -1000;
				score++;
				bulldis = 0;
			}
			else if (disx <= 60 && disy <= 60 && j % 3 == 1)
			{
				is = 0;
				eis[j] = 0;
				eza_x = -1000;
				eza_y = -1000;
				score++;
				bulldis = 0;
			}
			else if (disx <= 50 && disy <= 50 && j % 3 == 2)
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
		IMAGE pic;
		for (int i = 0; i < 6; i++)
		{
			if (Enexist[i] && i % 3 == 0)
			{
				if (en_x[i] >= ez_x && en_y[i] >= ez_y)
					timo = timo_upleft;
				else if (en_x[i] >= ez_x && en_y[i] < ez_y)
					timo = timo_downleft;
				else if (en_x[i] < ez_x && en_y[i] >= ez_y)
					timo = timo_upright;
				else if (en_x[i] < ez_x && en_y[i] < ez_y)
					timo = timo_downright;
				drawAlpha(&timo, en_x[i], en_y[i]);
			}
			else if (Enexist[i] && i % 3 == 1)
			{
				if (en_x[i] >= ez_x && en_y[i] >= ez_y)
					zed = zed_upleft;
				else if (en_x[i] >= ez_x && en_y[i] < ez_y)
					zed = zed_downleft;
				else if (en_x[i] < ez_x && en_y[i] >= ez_y)
					zed = zed_upright;
				else if (en_x[i] < ez_x && en_y[i] < ez_y)
					zed = zed_downright;
				drawAlpha(&zed, en_x[i], en_y[i]);
			}
			else if (Enexist[i] && i % 3 == 2)
			{
				if (en_x[i] >= ez_x && en_y[i] >= ez_y)
					yasuo = yasuo_upleft;
				else if (en_x[i] >= ez_x && en_y[i] < ez_y)
					yasuo = yasuo_downleft;
				else if (en_x[i] < ez_x && en_y[i] >= ez_y)
					yasuo = yasuo_upright;
				else if (en_x[i] < ez_x && en_y[i] < ez_y)
					yasuo = yasuo_downright;
				drawAlpha(&yasuo, en_x[i], en_y[i]);
			}
		}
	}

};
Enemy enemy;

class Bullet {
private:
	double eza_v;
	double eza_l = 0;

public:
	void InitBullet()
	{
		eza_v = 3.2;
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
		if (eza_l >= 600)
		{
			is = 0;
			eza_l = 0;
			eza_x = -1000;
			eza_y = -1000;
		}
	}

	void MoveBullet()
	{
		if ((GetAsyncKeyState(0x51) & 0x8000) && is == 0 && isezq == 1) //q
		{
			eza_x = ez_x + 28;
			eza_y = ez_y + 28;
			is = 1;
			isezq = 0;
		}
	}

};
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

void UpdateWithoutInput()
{
	static int lastSecond = 0; //记录前一次运行的秒数
	static int nowSecond = 0; //记录当前运行了多少秒
	static clock_t start = clock(); //记录第一次运行时刻
	clock_t now = clock(); //获得当前时刻
	static int estartsecond = 0;
	static int elastsecond = 0;
	static int qstartsecond = 0;
	static int qlastsecond = 0;
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
	nowSecond = (int(now - start) / CLOCKS_PER_SEC);
	if (isezq == 0)
	{
		qlastsecond = nowSecond;
		if (qif)
		{
			qstartsecond = nowSecond;
			qif = 0;
		}
		if (qlastsecond == qstartsecond + 1)
		{
			isezq = 1;
			qif = 1;
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
	if ((GetAsyncKeyState(0x50) & 0x8000)) // p
		pause.BeginPause();
	if ((GetAsyncKeyState(0x54) & 0x8000)) //t
		menu.Tips();
	gameover.BeginGameover();
}

void Show()
{
	cleardevice();
	putimage(0, 0, &game_bk);
	hero.ShowHero();
	hero.ShowSkill();
	enemy.ShowEnemy();
	bullet.UpdataBullet();
	bullet.ShowBullet();
	FlushBatchDraw();
}

int main()
{
	StartUp();
	menu.BeginMenu();
	while (1)
	{
		Show();
		UpdateWithoutInput();
		UpdateWithInput();
	}
	return 0;
}