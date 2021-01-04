#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")
using namespace std;

void StartUp(); //初始化函数
void Show(); //画图函数
void UpdateWithoutInput(); //没有输入，更新
void UpdateWithInput(); //有输入，更新

#define PI 3.14159265359 //圆周率 画图用
#define time_sleep 300 //定义按键反馈时间
#define WIDTH 1520 //页面宽度
#define HEIGHT 785 //页面长度
#define sn_width 82 //人物宽度
#define sn_height 94 //人物高度
#define yasuo_width 100 //斜着的亚索的宽
#define yasuo_height 100 //斜着的亚索的长
#define yasuo_widthd 110 //亚索的宽
#define yasuo_heightd 88 //亚索的长
#define timo_width 72 //斜着的提莫的宽
#define timo_height 60 //斜着的提莫的长
#define zed_width 133.5 //斜着的劫的宽
#define zed_height 121.5 //斜着的劫的长
#define zed_widthd 145.5 //劫的宽
#define zed_heightd 85 //劫的长
#define bullet_width 28.5 //秘术射击宽度
#define bullet_height 58.5 //秘术射击长度

//定义图片
IMAGE game_bk;
IMAGE start_menu;
IMAGE option_bk;
IMAGE tips_bk;
IMAGE pause_bk;
IMAGE gameover_bk;
IMAGE skillshot;
IMAGE skillshot_cover;
IMAGE fireball;
IMAGE fireball_cover;
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

int start = 0; //判断是否进入游戏
int click_tips = 0; //是否点击提示
int click_rank = 0; //是否点击rank
int click_quit = 0; //是否点击退出
int click_continue = 0; //是否继续游戏
int click_backtomenu = 0; //是否返回
int click_mode1 = 0; //是否选择模式1
int click_mode2 = 0; //是否选择模式2
int start_mode1 = 0; //是否开始模式1
int start_mode2 = 0; //是否开始模式2
int pause_exit = 0; //是否在暂停页面退出
int isbreak1 = 0; //模式1是否打破纪录
int isbreak2 = 0; //模式2是否打破纪录
double ez_x = WIDTH / 2; //初始化ez x坐标
double ez_y = HEIGHT / 2; //初始化ez y坐标
int score1 = 0; //模式1的分数
int score2 = 0; //模式2的分数
int is = 0; //子弹是否需要绘制
int eis[6]; //敌人是否可用
int eif = 1; //记录使用e的开始时间
int qif = 1; //记录使用q的开始时间
int alive = 1; //是否失败
int iseze = 1; //e是否可用
int isezq = 1; //q是否可用
int flashis = 0; //闪现图案是否生成s
double flash_x, flash_y; //闪现的x,y坐标
double xe = 30, xq = 30; //定义闪现图片的X Y坐标
double en_x[6], en_y[6]; //当前位置
int Enexist[6]; //敌人是否被击中
double eza_x, eza_y; //子弹的坐标
double eza_dir = 'w'; //初始化子弹的方向
int bulldis = 1; //子弹状态
int Enmax; //敌人目前最大数量
int Ennum; //敌人目前数量
MOUSEMSG m; //鼠标消息

//游戏重新开始时重新定义
void Init()
{
	start = 0; //判断是否进入游戏
	click_tips = 0; //是否点击提示
	click_rank = 0; //是否点击rank
	click_quit = 0; //是否点击退出
	click_continue = 0; //是否继续游戏
	click_backtomenu = 0; //是否返回
	click_mode1 = 0; //是否选择模式1
	click_mode2 = 0; //是否选择模式2
	start_mode1 = 0; //是否开始模式1
	start_mode2 = 0; //是否开始模式2
	pause_exit = 0; //是否在暂停页面退出
	isbreak1 = 0; //模式1是否打破纪录
	isbreak2 = 0; //模式2是否打破纪录
	ez_x = WIDTH / 2; //初始化ez x坐标
	ez_y = HEIGHT / 2; //初始化ez y坐标
	score1 = 0; //模式1的分数
	score2 = 0; //模式2的分数
	is = 0; //子弹是否需要绘制
	eif = 1; //记录使用e的开始时间
	qif = 1; //记录使用q的开始时间
	alive = 1; //是否失败
	iseze = 1; //e是否可用
	isezq = 1; //q是否可用
	flashis = 0; //闪现图案是否生成
	for (int i = 0; i < 6; i++)
	{
		en_x[i] = 10000; //当前位置
		en_y[i] = 10000;
		eis[i] = 1; //敌人是否被击中
		Enexist[i] = 0; //敌人是否可用
	}
	bulldis = 1; //子弹状态
	Enmax = 2; //敌人目前最大数量
	Ennum = 1; //敌人目前数量
}

//更改窗口标题
void changetitle()
{
	HWND hand = GetHWnd(); //获取窗口句柄
	SetWindowText(hand, _T("League of Legends")); //设置窗口标题
}

// 载入PNG图并去透明部分
void drawAlpha(IMAGE* picture, int picture_x, int picture_y) //x为载入图片的X坐标，y为Y坐标
{
	DWORD* dst = GetImageBuffer(); //GetImageBuffer()函数，用于获取绘图设备的显存指针
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
void LoadImage()
{
	loadimage(&game_bk, _T("./images/game_bk.jpg"));
	loadimage(&start_menu, _T("./images/begin_menu.jpg"));
	loadimage(&tips_bk, _T("./images/tips_bk_ch.jpg"));
	loadimage(&pause_bk, _T("./images/pause_bk.jpg"));
	loadimage(&gameover_bk, _T("./images/gameover_bk.jpg"));
	loadimage(&skillshot, _T("./images/skillshot_ch.jpg"), 370, 100);
	loadimage(&skillshot_cover, _T("./images/skillshot_cover_ch.jpg"), 370, 100);
	loadimage(&fireball, _T("./images/fireball_ch.jpg"), 185, 100);
	loadimage(&fireball_cover, _T("./images/fireball_cover_ch.jpg"), 185, 100);
	loadimage(&tips, _T("./images/tips_ch.jpg"), 185, 100);
	loadimage(&tips_cover, _T("./images/tips_cover_ch.jpg"), 185, 100);
	loadimage(&continu, _T("./images/continue_ch.jpg"), 185, 100);
	loadimage(&continu_cover, _T("./images/continue_cover_ch.jpg"), 185, 100);
	loadimage(&quit, _T("./images/quit_ch.jpg"), 185, 110);
	loadimage(&quit_cover, _T("./images/quit_cover_ch.jpg"), 185, 110);
	loadimage(&rank1, _T("./images/rank_ch.jpg"), 185, 100);
	loadimage(&rank_cover, _T("./images/rank_cover_ch.jpg"), 185, 100);
	loadimage(&backtomenu, _T("./images/backtomenu_ch.jpg"), 185, 100);
	loadimage(&backtomenu_cover, _T("./images/backtomenu_cover_ch.jpg"), 185, 100);
}

//定义开始界面类
class Menu {
public:
	void Tips() // 提示页面
	{
		putimage(0, 0, &tips_bk);
		FlushBatchDraw();
		Sleep(2);
		system("pause");
	}

	void Quit() //退出
	{
		EndBatchDraw(); //结束绘制
		mciSendString(_T("close bgm"), NULL, 0, NULL);
		closegraph();
		exit(0);
	}

	void Rank() //得分
	{
		FILE* fp;
		int top1, top2;
		TCHAR c1[10], c2[10];
		fp = fopen("./rank.dat", "r");
		fscanf(fp, "%d %d", &top1, &top2); //读档
		fclose(fp);
		_stprintf(c1, _T("%d"), top1);  //整型转成字符串
		_stprintf(c2, _T("%d"), top2); //整型转成字符串
		setlinecolor(RGB(255, 255, 255));
		setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 4);
		setfillcolor(RGB(0, 39, 48));
		fillrectangle(380, 157, 380 + 760, 157 + 471);
		LOGFONT f;	                            //定义字体属性结构体
		gettextstyle(&f);					   //获取当前字体设置
		f.lfHeight = 50;				   	  //设置字体高度为 50
		_tcscpy(f.lfFaceName, _T("黑体"));   //设置字体为黑体
		f.lfQuality = ANTIALIASED_QUALITY;	//设置输出效果为抗锯齿  
		f.lfWeight = FW_BLACK;             //设置字符粗细
		settextstyle(&f);				  //设置字体样式
		settextcolor(RGB(246, 232, 22)); //设置字体颜色
		setbkmode(TRANSPARENT);         //设置字体背景为透明
		outtextxy(WIDTH / 2 - 80, HEIGHT / 2 - 190, _T("最高分"));
		outtextxy(WIDTH / 2 - 270, HEIGHT / 2 - 100, _T("秘术射击：")); //模式一记录
		outtextxy(WIDTH / 2 + 130, HEIGHT / 2 - 100, c1);
		outtextxy(WIDTH / 2 - 270, HEIGHT / 2, _T("火球：")); //模式一记录
		outtextxy(WIDTH / 2 + 130, HEIGHT / 2, c2);
		FlushBatchDraw();
		Sleep(1000);
	}

	void BeginMenu() //当点击游戏模式后， 进入游戏，打开游戏背景音乐
	{
		while (start == 0)
		{
			MenuInput();
			MenuShow();
		}
		mciSendString(_T("open ./music/game_bgm.mp3 alias bgm"), NULL, 0, NULL);
		mciSendString(_T("play bgm repeat"), NULL, 0, NULL);
	}

	void MenuShow()
	{
		putimage(0, 0, &start_menu);
		if (click_mode1 == 1) //鼠标放在skillshot上，贴上反色图片，形成反馈
			putimage(700, 50, &skillshot_cover);
		else
			putimage(700, 50, &skillshot);
		if (click_mode2 == 1) //鼠标放在fireball上，贴上反色图片，形成反馈
			putimage(700, 200, &fireball_cover);
		else
			putimage(700, 200, &fireball);
		if (click_tips == 1) //鼠标放在tips上，贴上反色图片，形成反馈
			putimage(700, 350, &tips_cover);
		else
			putimage(700, 350, &tips);
		if (click_rank == 1) //鼠标放在rank上，贴上反色图片，形成反馈
			putimage(700, 500, &rank_cover);
		else
			putimage(700, 500, &rank1);
		if (click_quit == 1) //鼠标放在quit上，贴上反色图片，形成反馈
			putimage(700, 650, &quit_cover);
		else
			putimage(700, 650, &quit);
		FlushBatchDraw();
		Sleep(2);
	}

	void MenuInput()
	{
		while (MouseHit())
		{
			m = GetMouseMsg();
			if (m.x >= 700 && m.x <= 700 + 370 && m.y >= 50 && m.y <= 150) //skillshot
			{
				click_mode1 = 1;
				if (m.uMsg == WM_LBUTTONDOWN) //鼠标在按键范围内且左键按下去，播放按键音效
				{
					mciSendString(_T("close click"), NULL, 0, NULL);
					mciSendString(_T("open ./music/click.mp3 alias click"), NULL, 0, NULL);
					mciSendString(_T("play click"), NULL, 0, NULL);
					Sleep(time_sleep);
					start = 1;
					start_mode1 = 1;
				}
			}
			else
				click_mode1 = 0;
			if (m.x >= 700 && m.x <= 700 + 185 && m.y >= 200 && m.y <= 300) //fireball
			{
				click_mode2 = 1;
				if (m.uMsg == WM_LBUTTONDOWN) //鼠标在按键范围内且左键按下去，播放按键音效
				{
					mciSendString(_T("close click"), NULL, 0, NULL);
					mciSendString(_T("open ./music/click.mp3 alias click"), NULL, 0, NULL);
					mciSendString(_T("play click"), NULL, 0, NULL);
					Sleep(time_sleep);
					start = 1;
					start_mode2 = 1;
				}
			}
			else
				click_mode2 = 0;
			if (m.x >= 700 && m.x <= 700 + 185 && m.y >= 350 && m.y <= 450) //点击tips
			{
				click_tips = 1;
				if (m.uMsg == WM_LBUTTONDOWN) //鼠标在按键范围内且左键按下去，播放按键音效
				{
					mciSendString(_T("close click"), NULL, 0, NULL);
					mciSendString(_T("open ./music/click.mp3 alias click"), NULL, 0, NULL);
					mciSendString(_T("play click"), NULL, 0, NULL);
					Sleep(time_sleep);
					Tips(); //打开提示
				}
			}
			else
				click_tips = 0;
			if (m.x >= 700 && m.x <= 700 + 185 && m.y >= 500 && m.y <= 600) //点击rank
			{
				click_rank = 1;
				if (m.uMsg == WM_LBUTTONDOWN) //鼠标在按键范围内且左键按下去，播放按键音效
				{
					mciSendString(_T("close click"), NULL, 0, NULL);
					mciSendString(_T("open ./music/click.mp3 alias click"), NULL, 0, NULL);
					mciSendString(_T("play click"), NULL, 0, NULL);
					Sleep(time_sleep);
					Rank(); //打开rank
				}
			}
			else
				click_rank = 0;
			if (m.x >= 700 && m.x <= 700 + 185 && m.y >= 650 && m.y <= 750)
			{
				click_quit = 1;
				if (m.uMsg == WM_LBUTTONDOWN) //鼠标在按键范围内且左键按下去，播放按键音效
				{
					mciSendString(_T("close click"), NULL, 0, NULL);
					mciSendString(_T("open ./music/click.mp3 alias click"), NULL, 0, NULL);
					mciSendString(_T("play click"), NULL, 0, NULL);
					Sleep(time_sleep);
					Quit(); //退出游戏
				}
			}
			else
				click_quit = 0;
		}
	}
};
Menu menu;

//定义游戏结束界面类
class Gameover {
public:
	void GameoverShow()
	{
		putimage(0, 0, &gameover_bk);
		if (isbreak1 == 1 || isbreak2 == 1) //打破记录，输出new record!
		{
			setbkmode(TRANSPARENT);
			settextcolor(RGB(246, 232, 22));
			settextstyle(130, 0, _T("Consolas"));
			outtextxy(399, 30, _T("NEW RECORD!"));
		}
		if (click_quit == 1) //quit
			putimage(400, 500, &quit_cover);
		else
			putimage(400, 500, &quit);
		if (click_backtomenu == 1) //backtomenu
			putimage(400, 350, &backtomenu_cover);
		else
			putimage(400, 350, &backtomenu);
		TCHAR s[20];
		if (start_mode1 == 1) //模式一得分输出
		{
			_stprintf(s, _T("SCORE:%d"), score1);
			setbkmode(TRANSPARENT);
			settextcolor(RGB(246, 232, 22));
			settextstyle(130, 0, _T("黑体"));
			outtextxy(500, 150, s);
		}
		else if (start_mode2 == 1) //模式二得分输出
		{
			_stprintf(s, _T("SCORE:%d"), score2);
			setbkmode(TRANSPARENT);
			settextcolor(RGB(246, 232, 22));
			settextstyle(130, 0, _T("黑体"));
			outtextxy(500, 150, s);
		}
		FlushBatchDraw();
	}

	void GameoverRank() //游戏结束读档，判断是否打破纪录
	{
		FILE* fp;
		int before1, before2;   // 之前的纪录
		fp = fopen("./rank.dat", "r");
		fscanf(fp, "%d %d", &before1, &before2); //读档
		fclose(fp);
		if (score1 > before1)
		{
			isbreak1 = 1;
			FlushBatchDraw();
			fp = fopen("./rank.dat", "w");
			fprintf(fp, "%d %d", score1, before2); //写入存档
			fclose(fp);
			Sleep(1000);
		}
		if (score2 > before2)
		{
			isbreak2 = 1;
			FlushBatchDraw();
			fp = fopen("./rank.dat", "w");
			fprintf(fp, "%d %d", before1, score2); //写入存档
			fclose(fp);
			Sleep(1000);
		}
	}

	void BeginGameover() //如果生命值为0，游戏结束，播放得分音效
	{
		int n = 0;
		if (alive == 0) // 生命值为0
		{
			Sleep(1000);
			mciSendString(_T("close score"), NULL, 0, NULL);
			mciSendString(_T("open ./music/show_score.mp3 alias score"), NULL, 0, NULL);
			mciSendString(_T("play score"), NULL, 0, NULL);
			GameoverRank();
			while (1)
			{
				GameoverShow();
				GameoverInput();
			}
		}
	}

	void GameoverInput()
	{
		while (MouseHit())
		{
			m = GetMouseMsg();
			if (m.x >= 400 && m.x <= 400 + 185 && m.y >= 500 && m.y <= 600) //点击quit
			{
				click_quit = 1;
				if (m.uMsg == WM_LBUTTONDOWN)
				{
					mciSendString(_T("close click"), NULL, 0, NULL);
					mciSendString(_T("open ./music/click.mp3 alias click"), NULL, 0, NULL);
					mciSendString(_T("play click"), NULL, 0, NULL);
					Sleep(time_sleep);
					GameoverQuit(); //退出游戏
				}
			}
			else
				click_quit = 0;
			if (m.x >= 400 && m.x <= 400 + 185 && m.y >= 350 && m.y <= 450) //点击backtomenu
			{
				click_backtomenu = 1;
				if (m.uMsg == WM_LBUTTONDOWN)
				{
					mciSendString(_T("close click"), NULL, 0, NULL);
					mciSendString(_T("open ./music/click.mp3 alias click"), NULL, 0, NULL);
					mciSendString(_T("play click"), NULL, 0, NULL);
					Sleep(time_sleep);
					start = 0;
					GameoverBackMenu();
				}
			}
			else
				click_backtomenu = 0;
		}
	}

	void GameoverBackMenu() // 回到主菜单
	{
		mciSendString(_T("close bgm"), NULL, 0, NULL);
		//重新开始
		StartUp(); //初始化
		menu.BeginMenu(); //页面初始化
		while (1)
		{
			Show(); //绘制函数
			UpdateWithoutInput(); //数据更新
			UpdateWithInput(); //数据更新
		}
	}

	void GameoverQuit() //退出游戏
	{
		EndBatchDraw(); //结束绘制
		mciSendString(_T("close bgm"), NULL, 0, NULL); //关闭背景音乐
		closegraph(); //关闭画布
		exit(0);
	}
};
Gameover gameover;

//定义游戏暂停界面类
class Pause {
public:
	void BeginPause()
	{
		while (pause_exit == 0) //当游戏进行中按下p键
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
		Sleep(2);
	}

	void PauseInput()
	{
		while (MouseHit())
		{
			m = GetMouseMsg();
			if (m.x >= 200 && m.x <= 200 + 185 && m.y >= 300 && m.y <= 400) //tips
			{
				click_tips = 1;
				if (m.uMsg == WM_LBUTTONDOWN) //鼠标在按键范围内且左键按下去，播放按键音效
				{
					mciSendString(_T("close click"), NULL, 0, NULL);
					mciSendString(_T("open ./music/click.mp3 alias click"), NULL, 0, NULL);
					mciSendString(_T("play click"), NULL, 0, NULL);
					Sleep(time_sleep);
					menu.Tips(); //打开提示
				}
			}
			else
				click_tips = 0;
			if (m.x >= 200 && m.x <= 200 + 185 && m.y >= 450 && m.y <= 550) //continue
			{
				click_continue = 1;
				if (m.uMsg == WM_LBUTTONDOWN) //鼠标在按键范围内且左键按下去，播放按键音效
				{
					mciSendString(_T("close click"), NULL, 0, NULL);
					mciSendString(_T("open ./music/click.mp3 alias click"), NULL, 0, NULL);
					mciSendString(_T("play click"), NULL, 0, NULL);
					Sleep(time_sleep);
					pause_exit = 1;
				}
			}
			else
				click_continue = 0;
			if (m.x >= 200 && m.x <= 200 + 185 && m.y >= 600 && m.y <= 700) //backtomenu
			{
				click_backtomenu = 1;
				if (m.uMsg == WM_LBUTTONDOWN) //鼠标在按键范围内且左键按下去，播放按键音效
				{
					mciSendString(_T("close click"), NULL, 0, NULL);
					mciSendString(_T("open ./music/click.mp3 alias click"), NULL, 0, NULL);
					mciSendString(_T("play click"), NULL, 0, NULL);
					Sleep(time_sleep);
					gameover.GameoverBackMenu();
				}
			}
			else
				click_backtomenu = 0;
		}
	}
};
Pause pause;

//定义ez类
class Hero {
private:
	//定义图片
	IMAGE sn;
	IMAGE sn_up;
	IMAGE sn_down;
	IMAGE sn_left;
	IMAGE sn_right;
	IMAGE flash;
	IMAGE sn_upx[15];
	IMAGE sn_downx[15];
	IMAGE sn_leftx[15];
	IMAGE sn_rightx[15];
	double ez_v = 1; //速度
	double disx, disy;
	char ez_direction = 'w'; //初始化ez的方向
	char eze = 'w'; //技能1：闪现
	bool movestates = false;
public:
	void InitHero()
	{
		//加载图片
		loadimage(&sn_up, _T("./images/sn_up.png"), sn_width, sn_height);
		loadimage(&sn_down, _T("./images/sn_down.png"), sn_width, sn_height);
		loadimage(&sn_left, _T("./images/sn_left.png"), sn_width, sn_height);
		loadimage(&sn_right, _T("./images/sn_right.png"), sn_width, sn_height);
		loadimage(&sn_upx[1], _T("./images/sn_up1.png"), sn_width, sn_height);
		loadimage(&sn_upx[2], _T("./images/sn_up2.png"), sn_width, sn_height);
		loadimage(&sn_upx[3], _T("./images/sn_up3.png"), sn_width, sn_height);
		loadimage(&sn_upx[4], _T("./images/sn_up4.png"), sn_width, sn_height);
		loadimage(&sn_upx[5], _T("./images/sn_up5.png"), sn_width, sn_height);
		loadimage(&sn_upx[6], _T("./images/sn_up6.png"), sn_width, sn_height);
		loadimage(&sn_upx[7], _T("./images/sn_up7.png"), sn_width, sn_height);
		loadimage(&sn_upx[8], _T("./images/sn_up8.png"), sn_width, sn_height);
		loadimage(&sn_upx[9], _T("./images/sn_up9.png"), sn_width, sn_height);
		loadimage(&sn_upx[10], _T("./images/sn_up10.png"), sn_width, sn_height);
		loadimage(&sn_upx[11], _T("./images/sn_up11.png"), sn_width, sn_height);
		loadimage(&sn_upx[12], _T("./images/sn_up12.png"), sn_width, sn_height);
		loadimage(&sn_upx[13], _T("./images/sn_up13.png"), sn_width, sn_height);
		loadimage(&sn_upx[14], _T("./images/sn_up14.png"), sn_width, sn_height);
		loadimage(&sn_downx[1], _T("./images/sn_down1.png"), sn_width, sn_height);
		loadimage(&sn_downx[2], _T("./images/sn_down2.png"), sn_width, sn_height);
		loadimage(&sn_downx[3], _T("./images/sn_down3.png"), sn_width, sn_height);
		loadimage(&sn_downx[4], _T("./images/sn_down4.png"), sn_width, sn_height);
		loadimage(&sn_downx[5], _T("./images/sn_down5.png"), sn_width, sn_height);
		loadimage(&sn_downx[6], _T("./images/sn_down6.png"), sn_width, sn_height);
		loadimage(&sn_downx[7], _T("./images/sn_down7.png"), sn_width, sn_height);
		loadimage(&sn_downx[8], _T("./images/sn_down8.png"), sn_width, sn_height);
		loadimage(&sn_downx[9], _T("./images/sn_down9.png"), sn_width, sn_height);
		loadimage(&sn_downx[10], _T("./images/sn_down10.png"), sn_width, sn_height);
		loadimage(&sn_downx[11], _T("./images/sn_down11.png"), sn_width, sn_height);
		loadimage(&sn_downx[12], _T("./images/sn_down12.png"), sn_width, sn_height);
		loadimage(&sn_downx[13], _T("./images/sn_down13.png"), sn_width, sn_height);
		loadimage(&sn_downx[14], _T("./images/sn_down14.png"), sn_width, sn_height);
		loadimage(&sn_leftx[1], _T("./images/sn_left1.png"), sn_width, sn_height);
		loadimage(&sn_leftx[2], _T("./images/sn_left2.png"), sn_width, sn_height);
		loadimage(&sn_leftx[3], _T("./images/sn_left3.png"), sn_width, sn_height);
		loadimage(&sn_leftx[4], _T("./images/sn_left4.png"), sn_width, sn_height);
		loadimage(&sn_leftx[5], _T("./images/sn_left5.png"), sn_width, sn_height);
		loadimage(&sn_leftx[6], _T("./images/sn_left6.png"), sn_width, sn_height);
		loadimage(&sn_leftx[7], _T("./images/sn_left7.png"), sn_width, sn_height);
		loadimage(&sn_leftx[8], _T("./images/sn_left8.png"), sn_width, sn_height);
		loadimage(&sn_leftx[9], _T("./images/sn_left9.png"), sn_width, sn_height);
		loadimage(&sn_leftx[10], _T("./images/sn_left10.png"), sn_width, sn_height);
		loadimage(&sn_leftx[11], _T("./images/sn_left11.png"), sn_width, sn_height);
		loadimage(&sn_leftx[12], _T("./images/sn_left12.png"), sn_width, sn_height);
		loadimage(&sn_leftx[13], _T("./images/sn_left13.png"), sn_width, sn_height);
		loadimage(&sn_leftx[14], _T("./images/sn_left14.png"), sn_width, sn_height);
		loadimage(&sn_rightx[1], _T("./images/sn_right1.png"), sn_width, sn_height);
		loadimage(&sn_rightx[2], _T("./images/sn_right2.png"), sn_width, sn_height);
		loadimage(&sn_rightx[3], _T("./images/sn_right3.png"), sn_width, sn_height);
		loadimage(&sn_rightx[4], _T("./images/sn_right4.png"), sn_width, sn_height);
		loadimage(&sn_rightx[5], _T("./images/sn_right5.png"), sn_width, sn_height);
		loadimage(&sn_rightx[6], _T("./images/sn_right6.png"), sn_width, sn_height);
		loadimage(&sn_rightx[7], _T("./images/sn_right7.png"), sn_width, sn_height);
		loadimage(&sn_rightx[8], _T("./images/sn_right8.png"), sn_width, sn_height);
		loadimage(&sn_rightx[9], _T("./images/sn_right9.png"), sn_width, sn_height);
		loadimage(&sn_rightx[10], _T("./images/sn_right10.png"), sn_width, sn_height);
		loadimage(&sn_rightx[11], _T("./images/sn_right11.png"), sn_width, sn_height);
		loadimage(&sn_rightx[12], _T("./images/sn_right12.png"), sn_width, sn_height);
		loadimage(&sn_rightx[13], _T("./images/sn_right13.png"), sn_width, sn_height);
		loadimage(&sn_rightx[14], _T("./images/sn_right14.png"), sn_width, sn_height);
		loadimage(&flash, _T("./images/flash.png"));
		eze = 'w'; //初始化e的方向
		ez_direction = 'w'; //初始化ez的方向
	}

	void ShowHero() //显示ez的函数
	{
		drawAlpha(&sn, ez_x, ez_y); //画出ez
		if (flashis) //如果按下闪现
			drawAlpha(&flash, flash_x, flash_y); //绘制闪现的图片
	}

	void UpdateHero()
	{
		if (movestates)
		{
			static int wait = 1;
			static int i = 1;
			if (ez_direction == 'w') //如果方向向上
			{
				sn = sn_upx[i];//ez图片变为向上
				if (wait == 28)
				{
					i++;
					wait = 1;
				}
				if (i == 15)
					i = 1;
			}
			else if (ez_direction == 'a') //如果方向向左
			{
				sn = sn_leftx[i]; //ez图片变为向左
				if (wait == 28)
				{
					i++;
					wait = 1;
				}
				if (i == 15)
					i = 1;
			}
			else if (ez_direction == 's') //如果方向向下
			{
				sn = sn_downx[i]; //ez图片变为向下
				if (wait == 28)
				{
					i++;
					wait = 1;
				}
				if (i == 15)
					i = 1;
			}
			else if (ez_direction == 'd') //如果方向向右
			{
				sn = sn_rightx[i]; //ez图片变为向右
				if (wait == 28)
				{
					i++;
					wait = 1;
				}
				if (i == 15)
					i = 1;
			}
			for (int j = 0; j < 6; j++) //依次判断是否碰到其他英雄
			{
				if (j % 3 == 0) //第一种敌人
				{
					disx = abs(ez_x - 41 - en_x[j] + 36);
					disy = abs(ez_y - 47 - en_y[j] + 30);
				}
				else if (j % 3 == 1) //第二种敌人
				{
					disx = abs(ez_x - 41 - en_x[j] + 67);
					disy = abs(ez_y - 47 - en_y[j] + 60);
				}
				else if (j % 3 == 2) //第三种敌人
				{
					disx = abs(ez_x - 41 - en_x[j] + 55);
					disy = abs(ez_y - 47 - en_y[j] + 44);
				}
				if (disx <= 36 && disy <= 30 && j % 3 == 0)
					alive = 0; //失败
				else if (disx <= 40 && disy <= 30 && j % 3 == 1)
					alive = 0;
				else if (disx <= 55 && disy <= 44 && j % 3 == 2)
					alive = 0;
			}
			wait++;
		}
		else
		{
			if (ez_direction == 'w') //如果方向向上
			{
				sn = sn_up;//ez图片变为向上
			}
			else if (ez_direction == 'a') //如果方向向左
			{
				sn = sn_left; //ez图片变为向左
			}
			else if (ez_direction == 's') //如果方向向下
			{
				sn = sn_down; //ez图片变为向下
			}
			else if (ez_direction == 'd') //如果方向向右
			{
				sn = sn_right; //ez图片变为向右
			}
			for (int j = 0; j < 6; j++) //依次判断是否碰到其他英雄
			{
				if (j % 3 == 0) //第一种敌人
				{
					disx = abs(ez_x - 41 - en_x[j] + 36);
					disy = abs(ez_y - 47 - en_y[j] + 30);
				}
				else if (j % 3 == 1) //第二种敌人
				{
					disx = abs(ez_x - 41 - en_x[j] + 67);
					disy = abs(ez_y - 47 - en_y[j] + 60);
				}
				else if (j % 3 == 2) //第三种敌人
				{
					disx = abs(ez_x - 41 - en_x[j] + 55);
					disy = abs(ez_y - 47 - en_y[j] + 44);
				}
				if (disx <= 36 && disy <= 30 && j % 3 == 0)
					alive = 0; //失败
				else if (disx <= 40 && disy <= 30 && j % 3 == 1)
					alive = 0;
				else if (disx <= 55 && disy <= 44 && j % 3 == 2)
					alive = 0;
			}
		}
	}
		
	void ezE() //闪现函数
	{
		if ((GetAsyncKeyState(0x45) & 0x8000) && iseze == 1) //按下e
		{
			mciSendString(_T("close flash"), NULL, 0, NULL); //闪现音效
			mciSendString(_T("open ./music/flash.mp3 alias flash"), NULL, 0, NULL);
			mciSendString(_T("play flash"), NULL, 0, NULL);
			flashis = 1; //按下后允许绘制闪现图片
			flash_x = ez_x; //获取闪现图片的坐标
			flash_y = ez_y; //获取闪现图片的坐标
			if (eze == 'a') //如果方向向左
			{
				ez_x -= 200; //向左位移200
				iseze = 0; //e进入冷却
			}
			else if (eze == 'd') //如果方向向右
			{
				ez_x += 200; //向右位移200
				iseze = 0; //e进入冷却
			}
			else if (eze == 'w') //如果方向向上
			{
				ez_y -= 200; //向前位移200
				iseze = 0; //e进入冷却
			}
			else if (eze == 's') //如果方向向下
			{
				ez_y += 200; //向后位移200
				iseze = 0; //e进入冷却
			}
		}
	}

	void MoveHero() //ez移动函数
	{
		if (!is) //如果q没有绘制
			eza_dir = ez_direction; //子弹方向始终与ez方向一致
		if (iseze) //如果e可用
			eze = ez_direction; //e的方向始终与ez方向一致
		ezE(); //调用e函数
		if ((GetAsyncKeyState(0x41) & 0x8000)) //如果按下a
		{
			movestates = true;
			ez_x -= ez_v; //向左移动
			ez_direction = 'a'; //定义方向向左
			if (ez_x <= 0) //如果到边界
			{
				ez_direction = 'd'; //不断改变方向 使其静止 从而到达不能走出边界的目的
				ez_x += ez_v;
				ez_direction = 'a';
			}
		}
		if ((GetAsyncKeyState(0x44) & 0x8000)) //如果按下d
		{
			movestates = true;
			ez_x += ez_v; //向右移动
			ez_direction = 'd'; //定义方向向右
			if (ez_x >= WIDTH - double(sn_width)) //如果到达边界
			{
				ez_direction = 'a'; //不断改变方向 使其静止 从而到达不能走出边界的目的
				ez_x -= ez_v;
				ez_direction = 'd';
			}
		}
		if (GetAsyncKeyState(0x57) & 0x8000) //如果按下w
		{
			movestates = true;
			ez_y -= ez_v; //向上移动
			ez_direction = 'w'; //定义方向向上
			if (ez_y <= 0) //如果到达边界
			{
				ez_direction = 's'; //不断改变方向 使其静止 从而到达不能走出边界的目的
				ez_y += ez_v;
				ez_direction = 'w';
			}
		}
		if ((GetAsyncKeyState(0x53) & 0x8000)) //如果按下s
		{
			movestates = true;
			ez_y += ez_v; //向下移动
			ez_direction = 's'; //定义方向向下
			if (ez_y >= HEIGHT - double(sn_height)) //如果到达边界
			{
				ez_direction = 'w'; //不断改变方向 使其静止 从而到达不能走出边界的目的
				ez_y -= ez_v;
				ez_direction = 's';
			}
		}
	}

	void ShowSkill() //秘术射击（q)函数
	{
		//设置冷却时间条
		setbkmode(TRANSPARENT);
		settextcolor(RGB(255, 255, 255));
		settextstyle(20, 0, _T("consolar"));
		outtextxy(180, 25, _T("E"));
		if (iseze == 1) //当e为可用时
		{
			//绘制绿色 满的 冷却条
			setlinecolor(RGB(0, 0, 0));
			setfillcolor(RGB(0, 255, 0));
			fillrectangle(30, 30, 170, 40);
			xe = 30;
		}
		else if (iseze == 0) //当e不可用时
		{
			if (xe < 170) //在进度条满之前 进度条归0 变为红色 且逐步增加
			{
				xe += 0.08;
				setlinecolor(RGB(0, 0, 0));
				setfillcolor(RED);
				fillrectangle(30, 30, xe, 40);
			}
			else //当进度条满了 而实际冷却时间没好时 仍为红色
			{
				setlinecolor(RGB(0, 0, 0));
				setfillcolor(RED);
				fillrectangle(30, 30, xe, 40);
			}
		}
		if (start_mode1 == 1) //在模式skillshot中
		{
			//绘制q的冷却时间条
			setbkmode(TRANSPARENT);
			settextcolor(RGB(255, 255, 255));
			settextstyle(20, 0, _T("consolar"));
			outtextxy(180, 50, _T("Q"));
			if (isezq == 1) //如果q可用
			{
				//绘制绿色 满的冷却条
				setlinecolor(RGB(0, 0, 0));
				setfillcolor(RGB(0, 255, 0));
				fillrectangle(30, 60, 170, 70);
				xq = 30;
			}
			else if (isezq == 0) //如果q不可用
			{
				if (xq < 170) //在进度条满之前 进度条归0 变为红色 且逐步增加
				{
					xq += 0.7;
					setlinecolor(RGB(0, 0, 0));
					setfillcolor(RED);
					fillrectangle(30, 60, xq, 70);
				}
				else //当进度条满了 而实际冷却时间没好时 仍为红色
				{
					setlinecolor(RGB(0, 0, 0));
					setfillcolor(RED);
					fillrectangle(30, 60, xq, 70);
				}
			}
		}
	}
};
Hero hero;

//定义敌人类
class Enemy {
private:
	//定义图片
	IMAGE timo;
	IMAGE timo_up;
	IMAGE timo_down;
	IMAGE timo_left;
	IMAGE timo_right;
	IMAGE timo_upleft;
	IMAGE timo_upright;
	IMAGE timo_downleft;
	IMAGE timo_downright;
	IMAGE yasuo;
	IMAGE yasuo_up;
	IMAGE yasuo_down;
	IMAGE yasuo_left;
	IMAGE yasuo_right;
	IMAGE yasuo_upleft;
	IMAGE yasuo_upright;
	IMAGE yasuo_downleft;
	IMAGE yasuo_downright;
	IMAGE zed;
	IMAGE zed_up;
	IMAGE zed_down;
	IMAGE zed_left;
	IMAGE zed_right;
	IMAGE zed_upleft;
	IMAGE zed_upright;
	IMAGE zed_downleft;
	IMAGE zed_downright;
	double x[6], y[6]; //距离之差
	double encos[6], ensin[6]; //角度
	double en_vx[6], en_vy[6]; //分速度
	double en_v[6] = { 0.3 }; //总速度
public:
	void InitEnemy()
	{
		//加载图片
		loadimage(&timo_up, _T("./images/timo_up.png"), timo_width, timo_height);
		loadimage(&timo_down, _T("./images/timo_down.png"), timo_width, timo_height);
		loadimage(&timo_left, _T("./images/timo_left.png"), timo_width, timo_height);
		loadimage(&timo_right, _T("./images/timo_right.png"), timo_width, timo_height);
		loadimage(&timo_upleft, _T("./images/timo_upleft.png"), timo_width, timo_height);
		loadimage(&timo_upright, _T("./images/timo_upright.png"), timo_width, timo_height);
		loadimage(&timo_downleft, _T("./images/timo_downleft.png"), timo_width, timo_height);
		loadimage(&timo_downright, _T("./images/timo_downright.png"), timo_width, timo_height);
		loadimage(&yasuo_up, _T("./images/yasuo_up.png"), yasuo_heightd, yasuo_widthd);
		loadimage(&yasuo_down, _T("./images/yasuo_down.png"), yasuo_heightd, yasuo_widthd);
		loadimage(&yasuo_left, _T("./images/yasuo_left.png"), yasuo_widthd, yasuo_heightd);
		loadimage(&yasuo_right, _T("./images/yasuo_right.png"), yasuo_widthd, yasuo_heightd);
		loadimage(&yasuo_upleft, _T("./images/yasuo_upleft.png"), yasuo_width, yasuo_height);
		loadimage(&yasuo_upright, _T("./images/yasuo_upright.png"), yasuo_width, yasuo_height);
		loadimage(&yasuo_downleft, _T("./images/yasuo_downleft.png"), yasuo_width, yasuo_height);
		loadimage(&yasuo_downright, _T("./images/yasuo_downright.png"), yasuo_width, yasuo_height);
		loadimage(&zed_up, _T("./images/zed_up.png"), zed_heightd, zed_widthd);
		loadimage(&zed_down, _T("./images/zed_down.png"), zed_heightd, zed_widthd);
		loadimage(&zed_left, _T("./images/zed_left.png"), zed_widthd, zed_heightd);
		loadimage(&zed_right, _T("./images/zed_right.png"), zed_widthd, zed_heightd);
		loadimage(&zed_upleft, _T("./images/zed_upleft.png"), zed_width, zed_height);
		loadimage(&zed_upright, _T("./images/zed_upright.png"), zed_width, zed_height);
		loadimage(&zed_downleft, _T("./images/zed_downleft.png"), zed_width, zed_height);
		loadimage(&zed_downright, _T("./images/zed_downright.png"), zed_width, zed_height);
		Enexist[0] = 1;
		for (int i = 0; i < 6; i++) //初始化由两边生成敌人
		{
			if (i % 2 == 1)
				en_x[i] = -50;
			else
				en_x[i] = 1570;
			en_y[i] = rand() % 785;
			en_v[i] = (rand() % 2) / 10 + 0.3; //对速度的初始化加入随机性
		}
	}

	void MoveEnemy() //敌人移动函数
	{
		for (int i = 0; i < 6; i++) //循环遍历每一个敌人的参数
		{
			if (Enexist[i]) //如果敌人可用
			{
				encos[i] = (ez_x - en_x[i]) / sqrt((ez_x - en_x[i]) * (ez_x - en_x[i]) + (ez_y - en_y[i]) * (ez_y - en_y[i])); //角度
				ensin[i] = (ez_y - en_y[i]) / sqrt((ez_x - en_x[i]) * (ez_x - en_x[i]) + (ez_y - en_y[i]) * (ez_y - en_y[i])); //角度
				en_vx[i] = encos[i] * en_v[i]; //分速度
				en_vy[i] = ensin[i] * en_v[i]; //分速度
				en_x[i] += en_vx[i]; //实现移动
				en_y[i] += en_vy[i]; //实现移动
				if (eis[i] == 0) //如果被击中
				{
					int p = rand() % 4; //引入随机数
					if (p % 4 == 1) //此情况下在屏幕右方重新生成
					{
						en_x[i] = 1770;
						en_y[i] = rand() % 785;
						eis[i] = 1; //改变是否被击中的状态
					}
					else if (p % 4 == 2) //此情况下在屏幕左方重新生成
					{
						en_x[i] = -170;
						en_y[i] = rand() % 785;
						eis[i] = 1; //改变是否被击中的状态
					}
					else if (p % 4 == 3) //此情况下在屏幕上方重新生成
					{
						en_y[i] = -170;
						en_x[i] = rand() % 1520;
						eis[i] = 1; //改变是否被击中的状态
					}
					else if (p % 4 == 0) //此情况下在屏幕下方重新生成
					{
						en_y[i] = 950;
						en_x[i] = rand() % 1520;
						eis[i] = 1; //改变是否被击中的状态
					}
					if (en_v[i] <= 0.85) //如果敌人速度小于阙值
						en_v[i] += 0.066; //每被击中一次 增加速度
				}
			}
		}
	}

	void attack() //击中判断的函数
	{
		double disx, disy; //定义距离变量
		for (int j = 0; j < Enmax; j++) //遍历敌人参数
		{
			//计算子弹和敌人距离
			if (j % 3 == 0) //第一种敌人
			{
				disx = abs(eza_x - en_x[j] - 36);
				disy = abs(eza_y - en_y[j] - 30);
			}
			else if (j % 3 == 1) //第二种敌人
			{
				disx = abs(eza_x - en_x[j] - 45 * 1.5);
				disy = abs(eza_y - en_y[j] - 60);
			}
			else if (j % 3 == 2) //第三种敌人
			{
				disx = abs(eza_x - en_x[j] - 50);
				disy = abs(eza_y - en_y[j] - 50);
			}
			//每种敌人图片大小不一样   对应参数也不一样
			if (disx <= 36 && disy <= 30 && j % 3 == 0) //当子弹击中敌人后
			{
				eis[j] = 0; //敌人被击中
				eza_x = -1000; //移开子弹
				eza_y = -1000; //移开子弹
				score1++; //skillshot模式得分加一
				bulldis = 0; //子弹状态改变
			}
			if (disx <= 60 && disy <= 60 && j % 3 == 1) //当子弹击中敌人后
			{
				eis[j] = 0; //敌人被击中
				eza_x = -1000; //移开子弹
				eza_y = -1000; //移开子弹
				score1++; //skillshot模式得分加一
				bulldis = 0; //子弹状态改变
			}
			if (disx <= 50 && disy <= 50 && j % 3 == 2) //当子弹击中敌人后
			{
				eis[j] = 0; //敌人被击中
				eza_x = -1000; //移开子弹
				eza_y = -1000; //移开子弹
				score1++; //skillshot模式得分加一
				bulldis = 0; //子弹状态改变
			}
		}
	}

	void ShowEnemy() //敌人绘制函数
	{
		for (int i = 0; i < 6; i++) //遍历敌人  当敌人处于相对ez的不同位置时 显示不同的图片
		{
			if (Enexist[i] && i % 3 == 0) //第一种敌人
			{
				if (en_x[i] >= ez_x - 100 && en_x[i] < ez_x + 100 && en_y[i] < ez_y)
					timo = timo_down;
				else if (en_x[i] >= ez_x - 100 && en_x[i] < ez_x + 100 && en_y[i] >= ez_y)
					timo = timo_up;
				else if (en_x[i] < ez_x && en_y[i] >= ez_y - 100 && en_y[i] < ez_y + 100)
					timo = timo_left;
				else if (en_x[i] >= ez_x && en_y[i] >= ez_y - 100 && en_y[i] < ez_y + 100)
					timo = timo_right;
				else if (en_x[i] >= ez_x && en_y[i] >= ez_y)
					timo = timo_upleft;
				else if (en_x[i] >= ez_x && en_y[i] < ez_y)
					timo = timo_downleft;
				else if (en_x[i] < ez_x && en_y[i] >= ez_y)
					timo = timo_upright;
				else if (en_x[i] < ez_x && en_y[i] < ez_y)
					timo = timo_downright;
				drawAlpha(&timo, en_x[i], en_y[i]); //绘制敌人
			}
			else if (Enexist[i] && i % 3 == 1) //第二种敌人
			{
				if (en_x[i] >= ez_x - 100 && en_x[i] < ez_x + 100 && en_y[i] < ez_y)
					zed = zed_down;
				else if (en_x[i] >= ez_x - 100 && en_x[i] < ez_x + 100 && en_y[i] >= ez_y)
					zed = zed_up;
				else if (en_x[i] < ez_x && en_y[i] >= ez_y - 100 && en_y[i] < ez_y + 100)
					zed = zed_left;
				else if (en_x[i] >= ez_x && en_y[i] >= ez_y - 100 && en_y[i] < ez_y + 100)
					zed = zed_right;
				else if (en_x[i] >= ez_x && en_y[i] >= ez_y)
					zed = zed_upleft;
				else if (en_x[i] >= ez_x && en_y[i] < ez_y)
					zed = zed_downleft;
				else if (en_x[i] < ez_x && en_y[i] >= ez_y)
					zed = zed_upright;
				else if (en_x[i] < ez_x && en_y[i] < ez_y)
					zed = zed_downright;
				drawAlpha(&zed, en_x[i], en_y[i]); //绘制敌人
			}
			else if (Enexist[i] && i % 3 == 2) //第三种敌人
			{
				if (en_x[i] >= ez_x - 100 && en_x[i] < ez_x + 100 && en_y[i] < ez_y)
					yasuo = yasuo_down;
				else if (en_x[i] >= ez_x - 100 && en_x[i] < ez_x + 100 && en_y[i] >= ez_y)
					yasuo = yasuo_up;
				else if (en_x[i] < ez_x && en_y[i] >= ez_y - 100 && en_y[i] < ez_y + 100)
					yasuo = yasuo_right;
				else if (en_x[i] >= ez_x && en_y[i] >= ez_y - 100 && en_y[i] < ez_y + 100)
					yasuo = yasuo_left;
				else if (en_x[i] >= ez_x && en_y[i] >= ez_y)
					yasuo = yasuo_upleft;
				else if (en_x[i] >= ez_x && en_y[i] < ez_y)
					yasuo = yasuo_downleft;
				else if (en_x[i] < ez_x && en_y[i] >= ez_y)
					yasuo = yasuo_upright;
				else if (en_x[i] < ez_x && en_y[i] < ez_y)
					yasuo = yasuo_downright;
				drawAlpha(&yasuo, en_x[i], en_y[i]); //绘制敌人
			}
		}
	}
};
Enemy enemy;

//定义子弹Q类
class Bullet {
private:
	//定义图片
	IMAGE bullets;
	IMAGE bullet_up;
	IMAGE bullet_down;
	IMAGE bullet_left;
	IMAGE bullet_right;
	double eza_v; //定义子弹速度
	double eza_l = 0; //定义子弹里程数
public:
	void InitBullet()
	{
		//加载图片
		loadimage(&bullet_up, _T("./images/bullet_up.png"), bullet_width, bullet_height);
		loadimage(&bullet_down, _T("./images/bullet_down.png"), bullet_width, bullet_height);
		loadimage(&bullet_left, _T("./images/bullet_left.png"), bullet_height, bullet_width);
		loadimage(&bullet_right, _T("./images/bullet_right.png"), bullet_height, bullet_width);
		eza_v = 3.2; //定义子弹速度
		eza_x = ez_x; //定位子弹发射时坐标
		eza_y = ez_y; //定位子弹发射时坐标
	}

	void ShowBullet() //绘制子弹函数
	{
		if (is) //需要绘制子弹时
			drawAlpha(&bullets, eza_x, eza_y); //绘制子弹
	}

	void UpdataBullet() //更新子弹数据
	{
		//定义子弹方向始终和ez方向一样，并使用对应的子弹图片
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
		if (is) //如果需要绘制子弹
			eza_l += eza_v; //记录子弹里程数
		if (!bulldis) //击中时 或到达里程数时
		{
			eza_l = 0; //里程归0
			bulldis = 1; //子弹击中状态改变
		}
		if (eza_l >= 600) //里程到达600时
		{
			is = 0; //不再绘制子弹
			eza_l = 0; //里程归0
			eza_x = -1000; //移开子弹
			eza_y = -1000; //移开子弹
		}
	}

	void MoveBullet() //子弹移动函数
	{
		if ((GetAsyncKeyState(0x51) & 0x8000) && is == 0 && isezq == 1) //按下q
		{
			//加入音效
			mciSendString(_T("close biu"), NULL, 0, NULL);
			mciSendString(_T("open ./music/biu.mp3 alias biu"), NULL, 0, NULL);
			mciSendString(_T("play biu"), NULL, 0, NULL);
			eza_x = ez_x + 28; //子弹移动
			eza_y = ez_y + 28; //子弹移动
			is = 1; //绘制子弹
			isezq = 0; //q进入冷却
		}
	}
};
Bullet bullet;

//定义火球类 对应第二种模式
class Fireball {
private:
	//引入图片
	IMAGE fire;
	IMAGE fire_up;
	IMAGE fire_down;
	IMAGE fire_left;
	IMAGE fire_right;
	IMAGE fire_upleft;
	IMAGE fire_upright;
	IMAGE fire_downleft;
	IMAGE fire_downright;
	//定义火球参数和变量
	double fire_x[6], fire_y[6], fire_v = 2, firedir[6];
	double firecos[6], firesin[6], firevx[6], firevy[6];
public:
	void InitFireball()
	{
		//加载图片
		loadimage(&fire_up, _T("./images/fire_up.png"));
		loadimage(&fire_down, _T("./images/fire_down.png"));
		loadimage(&fire_left, _T("./images/fire_left.png"));
		loadimage(&fire_right, _T("./images/fire_right.png"));
		loadimage(&fire_upleft, _T("./images/fire_upleft.png"));
		loadimage(&fire_upright, _T("./images/fire_upright.png"));
		loadimage(&fire_downleft, _T("./images/fire_downleft.png"));
		loadimage(&fire_downright, _T("./images/fire_downright.png"));
		for (int i = 0; i < 6; i++) //遍历火球
		{
			firecos[i] = (ez_x - fire_x[i]) / sqrt((ez_x - fire_x[i]) * (ez_x - fire_x[i]) + (ez_y - fire_y[i]) * (ez_y - fire_y[i])); //计算角度
			firesin[i] = (ez_y - fire_y[i]) / sqrt((ez_x - fire_x[i]) * (ez_x - fire_x[i]) + (ez_y - fire_y[i]) * (ez_y - fire_y[i])); //计算角度
			firevx[i] = firecos[i] * fire_v; //计算分速度
			firevy[i] = firesin[i] * fire_v; //计算分速度
			int p = rand() % 4; //引入随机数
			if (p == 0) //此情况下 火球在屏幕左方生成
			{
				fire_x[i] = -200;
				fire_y[i] = rand() % 700;
			}
			if (p == 1) //此情况下 火球在屏幕右方生成
			{
				fire_x[i] = 1720;
				fire_y[i] = rand() % 700;
			}
			if (p == 2) //此情况下 火球在屏幕上方生成
			{
				fire_x[i] = rand() % 1500;
				fire_y[i] = -200;
			}
			if (p == 3) //此情况下 火球在屏幕下方生成
			{
				fire_x[i] = rand() % 1500;
				fire_y[i] = 985;
			}
			//根据火球初始化的位置 确定方向
			if (fire_x[i] < 800 && fire_y[i] < 390)
				firedir[i] = 0;
			else if (fire_x[i] < 800 && fire_y[i] > 390)
				firedir[i] = 1;
			else if (fire_x[i] > 800 && fire_y[i] < 390)
				firedir[i] = 2;
			else if (fire_x[i] > 800 && fire_y[i] > 390)
				firedir[i] = 3;
		}
	}

	void ShowFireball() //绘制火球图片
	{
		for (int i = 0;i < 6;i++) //遍历火球参数
		{
			//根据火球不同的初始化位置来调用不同的图片
			if (firedir[i] == 0)
				drawAlpha(&fire_downright, fire_x[i], fire_y[i]);
			else if (firedir[i] == 1)
				drawAlpha(&fire_upright, fire_x[i], fire_y[i]);
			else if (firedir[i] == 2)
				drawAlpha(&fire_downleft, fire_x[i], fire_y[i]);
			else if (firedir[i] == 3)
				drawAlpha(&fire_upleft, fire_x[i], fire_y[i]);
			else if (firedir[i] == 4)
				drawAlpha(&fire_right, fire_x[i], fire_y[i]);
			else if (firedir[i] == 5)
				drawAlpha(&fire_up, fire_x[i], fire_y[i]);
			else if (firedir[i] == 6)
				drawAlpha(&fire_left, fire_x[i], fire_y[i]);
			else if (firedir[i] == 7)
				drawAlpha(&fire_down, fire_x[i], fire_y[i]);
		}
	}

	void BallMove() //火球移动函数
	{
		for (int i = 0; i < 6; i++) //遍历火球参数
		{
			fire_x[i] += firevx[i]; //移动火球
			fire_y[i] += firevy[i]; //移动火球
			if (fire_x[i] >= 1800 || fire_x[i] <= -250 || fire_y[i] >= 1000 || fire_y[i] <= -250) //当火球飞出屏幕时
			{
				score2++;
				int p = rand() % 4; //引入随机数
				if (p == 0) //此情况下 在屏幕左边生成
				{
					fire_x[i] = -200;
					fire_y[i] = rand() % 700;
					firecos[i] = (ez_x - fire_x[i]) / sqrt((ez_x - fire_x[i]) * (ez_x - fire_x[i]) + (ez_y - fire_y[i]) * (ez_y - fire_y[i]));
					firesin[i] = (ez_y - fire_y[i]) / sqrt((ez_x - fire_x[i]) * (ez_x - fire_x[i]) + (ez_y - fire_y[i]) * (ez_y - fire_y[i]));
					firevx[i] = firecos[i] * fire_v;
					firevy[i] = firesin[i] * fire_v;
				}
				else if (p == 1) //此情况下 在屏幕右边生成
				{
					fire_x[i] = 1720;
					fire_y[i] = rand() % 700;
					firecos[i] = (ez_x - fire_x[i]) / sqrt((ez_x - fire_x[i]) * (ez_x - fire_x[i]) + (ez_y - fire_y[i]) * (ez_y - fire_y[i]));
					firesin[i] = (ez_y - fire_y[i]) / sqrt((ez_x - fire_x[i]) * (ez_x - fire_x[i]) + (ez_y - fire_y[i]) * (ez_y - fire_y[i]));
					firevx[i] = firecos[i] * fire_v;
					firevy[i] = firesin[i] * fire_v;
				}
				else if (p == 2) //此情况下 在屏幕上边生成
				{
					fire_x[i] = rand() % 1500;
					fire_y[i] = -200;
					firecos[i] = (ez_x - fire_x[i]) / sqrt((ez_x - fire_x[i]) * (ez_x - fire_x[i]) + (ez_y - fire_y[i]) * (ez_y - fire_y[i]));
					firesin[i] = (ez_y - fire_y[i]) / sqrt((ez_x - fire_x[i]) * (ez_x - fire_x[i]) + (ez_y - fire_y[i]) * (ez_y - fire_y[i]));
					firevx[i] = firecos[i] * fire_v;
					firevy[i] = firesin[i] * fire_v;
				}
				else if (p == 3) //此情况下 在屏幕下边生成
				{
					fire_x[i] = rand() % 1500;
					fire_y[i] = 985;
					firecos[i] = (ez_x - fire_x[i]) / sqrt((ez_x - fire_x[i]) * (ez_x - fire_x[i]) + (ez_y - fire_y[i]) * (ez_y - fire_y[i]));
					firesin[i] = (ez_y - fire_y[i]) / sqrt((ez_x - fire_x[i]) * (ez_x - fire_x[i]) + (ez_y - fire_y[i]) * (ez_y - fire_y[i]));
					firevx[i] = firecos[i] * fire_v;
					firevy[i] = firesin[i] * fire_v;
				}
				//根据角度计算火球的图片方向
				if (firecos[i] > 0.2 && firesin[i] > 0.2)
					firedir[i] = 0;
				else if (firecos[i] > 0.2 && firesin[i] < -0.2)
					firedir[i] = 1;
				else if (firecos[i] < -0.2 && firesin[i] > 0.2)
					firedir[i] = 2;
				else if (firecos[i] < -0.2 && firesin[i] < -0.2)
					firedir[i] = 3;
				else if (firesin[i] > 0.2 && firecos[i] < 0.2 && firecos[i]>-0.2)
					firedir[i] = 7;
				else if (firesin[i] < -0.2 && firecos[i] < 0.2 && firecos[i]>-0.2)
					firedir[i] = 5;
				else if (firecos[i] > 0.2 && firesin[i] < 0.2 && firesin[i]>-0.2)
					firedir[i] = 4;
				else if (firecos[i] < -0.2 && firesin[i] < 0.2 && firesin[i]>-0.2)
					firedir[i] = 6;
			}
		}
		for (int i = 0;i < 6;i++)
		{
			if (firedir[i] == 1 || firedir[i] == 2 || firedir[i] == 3 || firedir[i] == 0)
			{
				if (sqrt((ez_x - 41 - fire_x[i] + 50) * (ez_x - 41 - fire_x[i] + 50) + (ez_y - 47 - fire_y[i] + 55) * (ez_y - 47 - fire_y[i] + 55)) <= 65)
					alive = 0;
			}
			else if (firedir[i] == 6 || firedir[i] == 4)
			{
				if (sqrt((ez_x - 41 - fire_x[i] + 65) * (ez_x - 41 - fire_x[i] + 65) + (ez_y - 47 - fire_y[i] + 32.5) * (ez_y - 47 - fire_y[i] + 32.5)) <= 65)
					alive = 0;
			}
			else if (firedir[i] == 5 || firedir[i] == 7)
			{
				if (sqrt((ez_x - 41 - fire_x[i] + 32.5) * (ez_x - 41 - fire_x[i] + 32.5) + (ez_y - 47 - fire_y[i] + 65) * (ez_y - 47 - fire_y[i] + 65)) <= 60)
					alive = 0;
			}
		}
	}
};
Fireball ball;

void StartUp() //初始化函数
{
	initgraph(WIDTH, HEIGHT); //初始画布
	changetitle();
	LoadImage(); //加载图片
	//调用初始化函数
	Init();
	hero.InitHero();
	enemy.InitEnemy();
	bullet.InitBullet();
	ball.InitFireball();
	BeginBatchDraw(); //开始批量绘制
}

void UpdateWithoutInput() //没有数据输入
{
	static int lastSecond = 0; //记录前一次运行的秒数
	static int nowSecond = 0; //记录当前运行了多少秒
	static clock_t start = clock(); //记录第一次运行时刻
	clock_t now = clock(); //获得当前时刻
	static int estartsecond = 0; //初始化e的开始时间
	static int elastsecond = 0; //初始化e的持续时间
	static int qstartsecond = 0; //初始化q的开始时间
	static int qlastsecond = 0; //初始化q的结束时间
	if (nowSecond == lastSecond + 5 && Enmax <= 6) //随着时间的增加 增加最大敌人数 并设置上限
	{
		lastSecond = nowSecond; //更新时间数据
		Enmax += 1; //敌人最大数量加一
		Enexist[Enmax - 1] = 1; //设置新增加敌人可用
	}
	nowSecond = (int(now - start) / CLOCKS_PER_SEC); //更新时间数据
	if (iseze == 0) //如果e进入冷却
	{
		elastsecond = nowSecond; //更新时间数据
		if (eif) //第一次记录e开始时间
		{
			estartsecond = nowSecond; //更新e开始冷却的数据
			eif = 0; //防止重复更新数据
		}
		if (elastsecond == estartsecond + 10) //闪现10秒冷却时间
		{
			iseze = 1; //e状态改为可用
			eif = 1; //下一次更新e开始冷却的数据可用
		}
		if (elastsecond == estartsecond + 1) //闪现图片绘制1秒
			flashis = 0; //一秒后 闪现图片不绘制
	}
	if (start_mode1 == 1) //在选择模式1后
	{
		nowSecond = (int(now - start) / CLOCKS_PER_SEC);//更新时间
		if (isezq == 0) //q不可用时
		{
			qlastsecond = nowSecond; //更新时间数据
			if (qif) //第一次记录q开始时间
			{
				qstartsecond = nowSecond; //更新e开始冷却的数据
				qif = 0; //防止重复更新数据
			}
			if (qlastsecond == qstartsecond + 1) //q的冷却为一秒
			{
				isezq = 1; //q状态改为可用
				qif = 1; //下一次更新q开始冷却的数据可用
			}
		}
	}
	//改变帧数
	static int waitIndex = 1;
	waitIndex++;
	if (waitIndex == 2)
	{
		hero.MoveHero();
		if (start_mode1 == 1) //选择模式一 调用所需函数
		{
			enemy.MoveEnemy();
			bullet.MoveBullet();
			enemy.attack();
		}
		else if (start_mode2 == 1) //选择模式二 调用所需函数
		{
			ball.BallMove();
		}
		waitIndex = 1;
	}
}

void UpdateWithInput() //有数据输入时
{
	hero.UpdateHero();
	if ((GetAsyncKeyState(0x50) & 0x8000)) //按下p键，游戏暂停
	{
		pause.BeginPause(); //进入暂停页面
	}
	if ((GetAsyncKeyState(0x54) & 0x8000)) //按下t键，游戏暂停，查看提示
	{
		menu.Tips(); //进入提示页面
	}
	gameover.BeginGameover();
}

void Show() //绘制函数
{
	cleardevice();
	putimage(0, 0, &game_bk); //游戏背景图片
	hero.ShowHero();  //绘制hero
	hero.ShowSkill(); //绘制hero状态条
	if (start_mode1 == 1) //选择模式一 调用相关函数
	{
		enemy.ShowEnemy();
		bullet.UpdataBullet();
		bullet.ShowBullet();
	}
	else if (start_mode2 == 1) //选择模式二 调用相关函数
	{
		ball.ShowFireball();
	}
	FlushBatchDraw();
}

int main() //主函数
{
	StartUp(); //初始化
	menu.BeginMenu(); //页面初始化
	while (1)
	{
		Show(); //绘制函数
		UpdateWithoutInput(); //数据更新
		UpdateWithInput(); //数据更新
	}
	return 0;
}