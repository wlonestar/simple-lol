//使用了老版本的 c 语言函数，这些函数不能保证安全，编译器会报错，使用宏定义处理
#define _CRT_SECURE_NO_WARNINGS 1  
//调用标准输入输出流函数
#include <iostream>                
//调用 easyx 绘图库
#include <graphics.h>              
//通过控制台进行数据输入和数据输出的函数，UNIX 和 Linux 平台的 c 编译器通常不包含此头文件
#include <conio.h>                 
//进行三角函数、乘方、取绝对值运算
#include <math.h>                  
//调用与时间处理相关的头文件
#include <time.h>                  
//调用 Windows API ，修改输出的控制台应用标题
#include <Windows.h>               
//调用 Windows 图形设备界面
#include <mmsystem.h>              
//导入 winmm.lib 库，支持对 Windows 多媒体的编程
#pragma comment(lib, "Winmm.lib")  
//使用标准命名空间
using namespace std;               

/*--------------- 声明游戏实现核心函数 ---------------*/
void StartUp();            //初始化函数
void Show();               //画图函数
void UpdateWithoutInput(); //没有输入时更新
void UpdateWithInput();    //有输入时更新
/*--------------------------------------------------*/

/*--------------- 定义游戏配置数据 ---------------*/
#define time_sleep      300     //定义按键反馈时间
#define WIDTH          1520     //页面宽度
#define HEIGHT          785     //页面长度
#define sn_width         82     //人物宽度
#define sn_height        94     //人物高度
#define yasuo_width     100     //斜着的亚索的宽
#define yasuo_height    100     //斜着的亚索的长
#define yasuo_widthd    110     //亚索的宽
#define yasuo_heightd    88     //亚索的长
#define timo_width       72     //斜着的提莫的宽
#define timo_height      60     //斜着的提莫的长
#define zed_width       134     //斜着的劫的宽
#define zed_height      122	    //斜着的劫的长
#define zed_widthd      146     //劫的宽
#define zed_heightd      85     //劫的长
#define bullet_width     29     //秘术射击宽度
#define bullet_height    59     //秘术射击长度
/*-----------------------------------------------*/

/*--------------- 定义图片对象 ---------------*/
IMAGE game_bk;           //游戏背景图
IMAGE tips;              //tips 按钮图片
IMAGE tips_cover;        //按钮触发反色图片
IMAGE quit;              //quit 按钮图片
IMAGE quit_cover;        //按钮触发反色图片
IMAGE rank1;             //rank 按钮图片
IMAGE rank_cover;        //按钮触发反色图片
IMAGE backtomenu;        //back to menu 按钮图片
IMAGE backtomenu_cover;  //按钮触发反色图片
/*-------------------------------------------*/

/*--------------- 定义全局变量 ---------------*/
bool start;               //判断是否进入游戏
bool click_tips;          //是否点击提示
bool click_rank;          //是否点击 rank
bool click_quit;          //是否点击退出
bool click_continue;      //是否继续游戏
bool click_backtomenu;    //是否返回
bool click_mode1;         //是否选择模式 1
bool click_mode2;         //是否选择模式 2
bool start_mode1;         //是否开始模式 1
bool start_mode2;         //是否开始模式 2
bool pause_exit;          //是否在暂停页面退出
bool isbreak1;            //模式 1 是否打破纪录
bool isbreak2;            //模式 2 是否打破纪录
double ez_x;              //初始化 ez x 坐标
double ez_y;              //初始化 ez y 坐标
int score1;               //模式 1 的分数
int score2;               //模式 2 的分数
bool is;                  //子弹是否需要绘制
bool eis[6];              //敌人是否可用
bool eif;                 //记录使用 e 的开始时间
bool qif;                 //记录使用 q 的开始时间
bool alive;               //是否失败
bool iseze;               //e 是否可用
bool isezq;               //q 是否可用
bool flashis;             //闪现图案是否生成
double flash_x, flash_y;  //闪现的 x, y 坐标
double xe = 30, xq = 30;  //定义闪现图片的 X Y 坐标
double en_x[6], en_y[6];  //当前位置
bool Enexist[6];          //敌人是否被击中
double eza_x, eza_y;      //子弹的坐标
char eza_dir;             //初始化子弹的方向
bool bulldis;             //子弹状态
int Enmax;                //敌人目前最大数量
int Ennum;                //敌人目前数量
/*-------------------------------------------*/

/*----- 定义鼠标消息结构体 -----*/
MOUSEMSG m;

/*--------------- 游戏重新开始时重新赋值 ---------------*/
void Init() { 
    //利用系统时间来改变系统的种子值，为 rand() 函数提供不同的种子值，产生不同的随机数序列
	srand(time(NULL));
	start = false;
	click_tips = false;
	click_rank = false;
	click_quit = false;
	click_continue = false;
	click_backtomenu = false;
	click_mode1 = false;
	click_mode2 = false;
	start_mode1 = false;
	start_mode2 = false;
	pause_exit = false;
	isbreak1 = false;
	isbreak2 = false;
	ez_x = WIDTH / 2;
	ez_y = HEIGHT / 2;
	score1 = 0;
	score2 = 0;
	is = false;
	eif = true;
	qif = true;
	alive = true;
	iseze = true;
	isezq = true;
	flashis = false;
	for (int i = 0; i < 6; i++) {
		en_x[i] = 10000; en_y[i] = 10000;
		eis[i] = true;
		Enexist[i] = false;
	}
	eza_dir = 'w';
	bulldis = true;
	Enmax = 1;
	Ennum = 1;
}

/*--------------- 更改窗口标题 ---------------*/
void changetitle() {
	HWND hand = GetHWnd();                         //获取窗口句柄
	SetWindowText(hand, _T("League of Legends"));  //设置窗口标题
}
/*-------------------------------------------*/

/*--------------- 载入PNG图并去除透明部分 ---------------*/
void drawAlpha(IMAGE* picture, int picture_x, int picture_y) {
    //GetImageBuffer() 函数，用于获取绘图设备的显存指针
	DWORD* dst = GetImageBuffer();         
	DWORD* src = GetImageBuffer(picture);  //获取 picture 的显存指针
	int src_width = picture->getwidth();
	int src_height = picture->getheight();
	int dst_width = getwidth();
	int dst_height = getheight();
	int im_width, im_height;
	if (picture_x + src_width > dst_width) 
        im_width = dst_width - picture_x;
	else im_width = src_width;
	if (picture_y + src_height > dst_height) 
        im_height = dst_height - picture_y;
	else im_height = src_height;
	if (picture_x < 0) {
		src = src - picture_x;
		im_width = im_width + picture_x;
		picture_x = 0;
	}
	if (picture_y < 0) {
		src = src - src_width * picture_y;
		im_height = im_height + picture_y;
		picture_y = 0;
	}
	dst = dst + dst_width * picture_y + picture_x;
	for (int i = 0; i < im_height; i++) {
		for (int j = 0; j < im_width; j++) {
			int src_r = ((src[j] & 0xff0000) >> 16); // >> 操作符，提取数据
			int src_g = ((src[j] & 0xff00) >> 8);
			int src_b = src[j] & 0xff;
			int src_a = ((src[j] & 0xff000000) >> 24);
			int dst_r = ((dst[j] & 0xff0000) >> 16);
			int dst_g = ((dst[j] & 0xff00) >> 8);
			int dst_b = dst[j] & 0xff;
			dst[j] = ((src_r + dst_r * (255 - src_a) / 255) << 16) | 
                     ((src_g + dst_g * (255 - src_a) / 255) << 8) | 
                      (src_b + dst_b * (255 - src_a) / 255);
		}
		dst = dst + dst_width;
		src = src + src_width;
	}
}
/*-----------------------------------------------------*/

/*--------------- 载入图片 ---------------*/
void LoadImage() {
	loadimage(&game_bk, _T("./images/game_bk.jpg"));
	loadimage(&tips, _T("./images/tips_ch.jpg"), 185, 100);
	loadimage(&tips_cover, _T("./images/tips_cover_ch.jpg"), 185, 100);
	loadimage(&quit, _T("./images/quit_ch.jpg"), 185, 110);
	loadimage(&quit_cover, _T("./images/quit_cover_ch.jpg"), 185, 110);
	loadimage(&rank1, _T("./images/rank_ch.jpg"), 185, 100);
	loadimage(&rank_cover, _T("./images/rank_cover_ch.jpg"), 185, 100);
	loadimage(&backtomenu, _T("./images/backtomenu_ch.jpg"), 185, 100);
	loadimage(&backtomenu_cover, _T("./images/backtomenu_cover_ch.jpg"), 
                185, 100);
}
/*---------------------------------------*/

/*--------------- 精确到微秒的延时类 ---------------*/
class MyTimer {
private:
	LARGE_INTEGER m_clk; //保存时钟信息
	LONGLONG m_oldclk;	 //保存开始时钟和结束时钟
	int m_freq;			 //时钟频率(时钟时间换算率)，时间差

public:
	MyTimer() {
		QueryPerformanceFrequency(&m_clk);
		m_freq = (int)m_clk.QuadPart / 1000;
		m_oldclk = 0; // 获得计数器的时钟频率
	}
    
    //延时
	void Sleep(int ms) {
		unsigned int c = ms * m_freq;
		if (m_oldclk == 0) {
			QueryPerformanceCounter(&m_clk); //开始计时
			m_oldclk = m_clk.QuadPart;       //获得开始时钟
		}
		m_oldclk += c;
		QueryPerformanceCounter(&m_clk);
		if (m_clk.QuadPart > m_oldclk)
			m_oldclk = m_clk.QuadPart;
		else
			do {
				::Sleep(1);
				QueryPerformanceCounter(&m_clk); //获得终止时钟
			} while (m_clk.QuadPart < m_oldclk);
	}
    //重置延时器
	void Reset() {
		m_oldclk = 0;
	}
};
MyTimer timer;

/*--------------- 游戏加载动画类 ---------------*/
class Load {
private:
	int len = 1320;   //进度条总长度
	IMAGE begin_pic;  //开始加载界面图片

public:
    //开始界面加载动画
	void beginAnimation() {
		TCHAR s[20];
		setbkmode(TRANSPARENT); //设置字体背景透明
		settextcolor(RGB(255, 255, 255));
		settextstyle(20, 0, _T("Consolas"));
		for (int y = 0; y <= len; y += 5) {
			cleardevice(); //清屏函数
			loadimage(&begin_pic, _T("./images/begin.jpg"));
			putimage(0, 0, &begin_pic); //放置背景图片
			outtextxy(WIDTH / 2 - 10, HEIGHT / 2 + 200 - 30, _T("loading..."));
			setfillcolor(RGB(255, 255, 255));
			solidrectangle((WIDTH - len) / 2, HEIGHT / 2 + 200, 
                           (WIDTH - len) / 2 + y, HEIGHT / 2 + 200 + 0.9);
            //浮点型转换成字符串
			_stprintf(s, _T("%.0f"), ((float)y / (float)len) * 100.0); 
			outtextxy(WIDTH - 130 - 5, HEIGHT / 2 + 200 + 10, s); // 输出加载百分比
            //输出百分号
			outtextxy(WIDTH - 130 + 23, HEIGHT / 2 + 200 + 10, _T("\%")); 
            //输出开发者信息
			outtextxy(WIDTH / 2 - 130, HEIGHT - 50, 
                        _T("Powered by LZY, WJL and XYX.")); 
			timer.Sleep(20);
			FlushBatchDraw();
		}
		timer.Sleep(1500);
	}
	
    //游戏中途加载动画
	void animation() {
		setbkcolor(RGB(0, 38, 47));
		cleardevice(); //清屏函数
		setbkmode(TRANSPARENT); //设置字体背景透明
		settextcolor(RGB(255, 255, 255));
		settextstyle(20, 0, _T("Consolas"));
		outtextxy(WIDTH / 2 - 10, HEIGHT / 2 + 200 - 30, _T("loading..."));
		for (int y = 0; y < len; y += 20) {
			setfillcolor(RGB(255, 255, 255));
			solidrectangle((WIDTH - len) / 2, HEIGHT / 2 + 200, 
                           (WIDTH - len) / 2 + y, HEIGHT / 2 + 200 + 0.9);
			timer.Sleep(20);
			FlushBatchDraw();
		}
		timer.Sleep(300);
	}
};
Load load;

/*--------------- 定义开始界面对象 ---------------*/
class Menu {
private:
	/*----- 定义图片对象 -----*/
	IMAGE tips_bk;         //提示界面背景图
	IMAGE start_menu;      //选择界面背景图
	IMAGE skillshot;       //skillshot 按钮图片
	IMAGE skillshot_cover; //按钮触发反色图片
	IMAGE fireball;        //fireball 按钮图片
	IMAGE fireball_cover;  //按钮触发反色图片

public:
	void initMenu() {
		loadimage(&tips_bk, _T("./images/tips_bk_ch.jpg"));
		loadimage(&start_menu, _T("./images/begin_menu.jpg"));
		loadimage(&skillshot, _T("./images/skillshot_ch.jpg"), 
                    370, 100);
		loadimage(&skillshot_cover, _T("./images/skillshot_cover_ch.jpg"), 
                    370, 100);
		loadimage(&fireball, _T("./images/fireball_ch.jpg"), 
                    185, 100);
		loadimage(&fireball_cover, _T("./images/fireball_cover_ch.jpg"), 
                    185, 100);
	}
    
    //提示页面
	void Tips() {
		putimage(0, 0, &tips_bk);
		FlushBatchDraw();
		timer.Sleep(2);
		system("pause"); //防止程序一闪而过
	}
    //退出
	void Quit() {
		load.animation();
		EndBatchDraw(); //结束绘制
		mciSendString(_T("close bgm"), NULL, 0, NULL); //关闭背景音乐 bgm
		closegraph();
		exit(0); //正常运行程序并退出程序
	}
    //得分
	void Rank() {
		FILE* fp; //声明 fp 是用来指向 FILE 类型对象的指针
		int top1, top2; //历史最高分
		TCHAR c1[10], c2[10];
		fp = fopen("./rank.dat", "r");     //打开 rank.dat 文本文件，参数 r 表示只允许读数据
		fscanf(fp, "%d %d", &top1, &top2); //获取文档内容
		fclose(fp);
		_stprintf(c1, _T("%d"), top1); //整型转成字符串
		_stprintf(c2, _T("%d"), top2); //整型转成字符串
		setlinecolor(RGB(255, 255, 255));
		setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 4); //定义矩形边框样式和宽度
		setfillcolor(RGB(0, 38, 47));
		fillrectangle(380, 157 + 40, 380 + 760, 157 + 471);
		LOGFONT f;	                       //定义字体属性结构体
		gettextstyle(&f);				   //获取当前字体设置
		f.lfHeight = 50;				   //设置字体高度为 50
		_tcscpy(f.lfFaceName, _T("黑体")); //设置字体为黑体
		f.lfQuality = ANTIALIASED_QUALITY; //设置输出效果为抗锯齿  
		f.lfWeight = FW_BLACK;             //设置字符粗细
		settextstyle(&f);				   //设置字体样式
		settextcolor(RGB(246, 232, 22));   //设置字体颜色
		setbkmode(TRANSPARENT);            //设置字体背景为透明
		outtextxy(WIDTH / 2 - 80, HEIGHT / 2 - 190 + 40, _T("最高分"));
		outtextxy(WIDTH / 2 - 270, HEIGHT / 2 - 100 + 40, _T("秘术射击：")); //模式一记录
		outtextxy(WIDTH / 2 + 130, HEIGHT / 2 - 100 + 40, c1);
		outtextxy(WIDTH / 2 - 270, HEIGHT / 2 + 40, _T("火球：")); //模式一记录
		outtextxy(WIDTH / 2 + 130, HEIGHT / 2 + 40, c2);
		FlushBatchDraw();
		timer.Sleep(1000);
	}

    //当点击游戏模式后， 加载，进入游戏，打开游戏背景音乐
	void BeginMenu() {
		while (!start) {
			MenuInput();
			MenuShow();
		}
		load.animation();
        //打开 game_bgm.mp3 文件，记为 bgm
		mciSendString(_T("open ./music/game_bgm.mp3 alias bgm"), NULL, 0, NULL); 
		mciSendString(_T("play bgm repeat"), NULL, 0, NULL); //重复播放 bgm
	}

	void MenuShow()	{
		putimage(0, 0, &start_menu);
        //鼠标放在 skillshot 上，贴上反色图片，形成反馈
		if (click_mode1) putimage(700, 50, &skillshot_cover);
		else putimage(700, 50, &skillshot);
        //鼠标放在 fireball 上，贴上反色图片，形成反馈
		if (click_mode2) putimage(700, 200, &fireball_cover);
		else putimage(700, 200, &fireball);
        //鼠标放在 tips 上，贴上反色图片，形成反馈
		if (click_tips) putimage(700, 350, &tips_cover);
		else putimage(700, 350, &tips);
        //鼠标放在 rank 上，贴上反色图片，形成反馈
		if (click_rank) putimage(700, 500, &rank_cover);
		else putimage(700, 500, &rank1);
        //鼠标放在 quit 上，贴上反色图片，形成反馈
		if (click_quit) putimage(700, 650, &quit_cover);
		else putimage(700, 650, &quit);
		FlushBatchDraw();
		timer.Sleep(2);
	}

	void MenuInput() {
		while (MouseHit()) {
			m = GetMouseMsg();
			if (m.x >= 700 && m.x <= 700 + 370 && m.y >= 50 && m.y <= 150) {
				click_mode1 = true;
                //鼠标在按键范围内且左键按下去，播放按键音效
				if (m.uMsg == WM_LBUTTONDOWN) {
                    //先关闭音乐，实现多次播放
					mciSendString(_T("close click"), NULL, 0, NULL); 
					mciSendString(_T("open ./music/click.mp3 alias click"), 
                                    NULL, 0, NULL);
					mciSendString(_T("play click"), NULL, 0, NULL);
					timer.Sleep(time_sleep);
					start = true;
					start_mode1 = true;
				}
			}
			else click_mode1 = false;
			if (m.x >= 700 && m.x <= 700 + 185 && m.y >= 200 && m.y <= 300) {
				click_mode2 = true;
                //鼠标在按键范围内且左键按下去，播放按键音效
				if (m.uMsg == WM_LBUTTONDOWN) {
					mciSendString(_T("close click"), NULL, 0, NULL);
					mciSendString(_T("open ./music/click.mp3 alias click"), 
                                    NULL, 0, NULL);
					mciSendString(_T("play click"), NULL, 0, NULL);
					timer.Sleep(time_sleep);
					start = true;
					start_mode2 = true;
				}
			}
			else click_mode2 = false;
			if (m.x >= 700 && m.x <= 700 + 185 && m.y >= 350 && m.y <= 450) {
				click_tips = true;
                //鼠标在按键范围内且左键按下去，播放按键音效
				if (m.uMsg == WM_LBUTTONDOWN) {
					mciSendString(_T("close click"), NULL, 0, NULL);
					mciSendString(_T("open ./music/click.mp3 alias click"), 
                                    NULL, 0, NULL);
					mciSendString(_T("play click"), NULL, 0, NULL);
					timer.Sleep(time_sleep);
					Tips(); //打开提示
				}
			}
			else click_tips = false;
			if (m.x >= 700 && m.x <= 700 + 185 && m.y >= 500 && m.y <= 600) {
				click_rank = true;
                //鼠标在按键范围内且左键按下去，播放按键音效
				if (m.uMsg == WM_LBUTTONDOWN) {
					mciSendString(_T("close click"), NULL, 0, NULL);
					mciSendString(_T("open ./music/click.mp3 alias click"), 
                                    NULL, 0, NULL);
					mciSendString(_T("play click"), NULL, 0, NULL);
					timer.Sleep(time_sleep);
					Rank(); //打开rank
				}
			}
			else click_rank = false;
			if (m.x >= 700 && m.x <= 700 + 185 && m.y >= 650 && m.y <= 750) {
				click_quit = true;
                //鼠标在按键范围内且左键按下去，播放按键音效
				if (m.uMsg == WM_LBUTTONDOWN) {
					mciSendString(_T("close click"), NULL, 0, NULL);
					mciSendString(_T("open ./music/click.mp3 alias click"), 
                                    NULL, 0, NULL);
					mciSendString(_T("play click"), NULL, 0, NULL);
					timer.Sleep(time_sleep);
					Quit(); //退出游戏
				}
			}
			else click_quit = false;
		}
	}
};
Menu menu;

/*--------------- 定义游戏结束界面对象 ---------------*/
class Gameover {
private:
	/*----- 定义图片对象 -----*/
	IMAGE gameover_bk; //游戏结束界面背景图

public:
	void initGameover() {
		loadimage(&gameover_bk, _T("./images/gameover_bk.jpg"));
	}

	void GameoverShow() {
		putimage(0, 0, &gameover_bk);
        //打破记录，输出 new record!
		if (isbreak1 || isbreak2) {
			setbkmode(TRANSPARENT);
			settextcolor(RGB(246, 232, 22));
			settextstyle(130, 0, _T("Consolas"));
			outtextxy(399 + 50, 30, _T("NEW RECORD!"));
		}
        //quit
		if (click_quit) putimage(400, 500, &quit_cover);
		else putimage(400, 500, &quit);
        //backtomenu
		if (click_backtomenu) putimage(400, 350, &backtomenu_cover);
		else putimage(400, 350, &backtomenu);
		TCHAR s[20];
        
		if (start_mode1) { //模式一得分输出
			_stprintf(s, _T("SCORE:%d"), score1);
			setbkmode(TRANSPARENT);
			settextcolor(RGB(246, 232, 22));
			settextstyle(130, 0, _T("黑体"));
			outtextxy(500, 150, s);
		} else if (start_mode2) { //模式二得分输出
			_stprintf(s, _T("SCORE:%d"), score2);
			setbkmode(TRANSPARENT);
			settextcolor(RGB(246, 232, 22));
			settextstyle(130, 0, _T("黑体"));
			outtextxy(500, 150, s);
		}
		FlushBatchDraw();
	}

    //游戏结束读档，判断是否打破纪录
	void GameoverRank() {
		FILE* fp;
		int before1, before2; //之前的纪录
        //打开 rank.dat 文本文件，参数 r 表示只允许读数据
		fp = fopen("./rank.dat", "r"); 
		fscanf(fp, "%d %d", &before1, &before2); //读取文档内容
		fclose(fp);
		if (score1 > before1) {
			isbreak1 = true;
			FlushBatchDraw();
			fp = fopen("./rank.dat", "w"); //参数 w 表示写入数据
			fprintf(fp, "%d %d", score1, before2); //写入新纪录
			fclose(fp);
			timer.Sleep(1000);
		}
		if (score2 > before2) {
			isbreak2 = true;
			FlushBatchDraw();
			fp = fopen("./rank.dat", "w"); //参数 w 表示写入数据
			fprintf(fp, "%d %d", before1, score2); //写入新纪录
			fclose(fp);
			timer.Sleep(1000);
		}
	}
    
    //如果生命值为 0 ，游戏结束，播放得分音效
	void BeginGameover() {
        // 生命值为0
		if (!alive) {
			Sleep(1000);
			mciSendString(_T("close score"), NULL, 0, NULL);
			mciSendString(_T("open ./music/show_score.mp3 alias score"), 
                            NULL, 0, NULL);
			mciSendString(_T("play score"), NULL, 0, NULL);
			GameoverRank();
			while (1) {
				GameoverShow();
				GameoverInput();
			}
		}
	}

	void GameoverInput() {
		while (MouseHit()) {
			m = GetMouseMsg();
			if (m.x >= 400 && m.x <= 400 + 185 && m.y >= 500 && m.y <= 600) {
				click_quit = true;
                //点击 quit
				if (m.uMsg == WM_LBUTTONDOWN) {
					mciSendString(_T("close click"), NULL, 0, NULL);
					mciSendString(_T("open ./music/click.mp3 alias click"), 
                                    NULL, 0, NULL);
					mciSendString(_T("play click"), NULL, 0, NULL);
					timer.Sleep(time_sleep);
					GameoverQuit(); //退出游戏
				}
			}
			else click_quit = false;
			if (m.x >= 400 && m.x <= 400 + 185 && m.y >= 350 && m.y <= 450) {
				click_backtomenu = true;
                //点击 backtomenu
				if (m.uMsg == WM_LBUTTONDOWN) {
					mciSendString(_T("close click"), NULL, 0, NULL);
					mciSendString(_T("open ./music/click.mp3 alias click"), 
                                    NULL, 0, NULL);
					mciSendString(_T("play click"), NULL, 0, NULL);
					timer.Sleep(time_sleep);
					start = false;
					GameoverBackMenu();
				}
			}
			else click_backtomenu = false;
		}
	}

    // 回到主菜单
	void GameoverBackMenu() {
		load.animation();
		mciSendString(_T("close bgm"), NULL, 0, NULL); //关闭背景音乐
		/*--------------- 重新开始 ---------------*/
		StartUp(); //初始化
		menu.BeginMenu(); //页面初始化
		while (1) {
			Show(); //绘制函数
			UpdateWithoutInput(); //数据更新
			UpdateWithInput(); //数据更新
		}
	}

    //退出游戏
	void GameoverQuit() {
		EndBatchDraw(); //结束绘制
		mciSendString(_T("close bgm"), NULL, 0, NULL); //关闭背景音乐
		closegraph(); //关闭画布
		exit(0);
	}
};
Gameover gameover;

/*--------------- 定义游戏暂停界面对象 ---------------*/
class Pause {
private:
	/*----- 定义图片对象 -----*/
	IMAGE continu;       //continue 按钮图片
	IMAGE continu_cover; //按钮触发反色图片
	IMAGE pause_bk;      //暂停界面背景图

public:
	void initPause() {
		loadimage(&continu, _T("./images/continue_ch.jpg"), 185, 100);
		loadimage(&continu_cover, _T("./images/continue_cover_ch.jpg"), 
                    185, 100);
		loadimage(&pause_bk, _T("./images/pause_bk.jpg"));
	}

	void BeginPause() {
        //当游戏进行中按下p键
		while (!pause_exit) {
			PauseShow();
			PauseInput();
		}
		pause_exit = false;
	}

	void PauseShow(){
		putimage(0, 0, &pause_bk);
        //tips
		if (click_tips) putimage(200, 300, &tips_cover);
		else putimage(200, 300, &tips);
        //continue
		if (click_continue) putimage(200, 450, &continu_cover);
		else putimage(200, 450, &continu);
        //backtomenu
		if (click_backtomenu) putimage(200, 600, &backtomenu_cover);
		else putimage(200, 600, &backtomenu);
		FlushBatchDraw();
		Sleep(2);
	}

	void PauseInput() {
		while (MouseHit()) {
			m = GetMouseMsg();
			if (m.x >= 200 && m.x <= 200 + 185 && m.y >= 300 && m.y <= 400) {
				click_tips = true;
                //鼠标在按键范围内且左键按下去，播放按键音效
				if (m.uMsg == WM_LBUTTONDOWN) {
					mciSendString(_T("close click"), NULL, 0, NULL);
					mciSendString(_T("open ./music/click.mp3 alias click"), 
                                    NULL, 0, NULL);
					mciSendString(_T("play click"), NULL, 0, NULL);
					timer.Sleep(time_sleep);
					menu.Tips(); //打开提示
				}
			}
			else click_tips = false;
			if (m.x >= 200 && m.x <= 200 + 185 && m.y >= 450 && m.y <= 550) {
				click_continue = true;
                //鼠标在按键范围内且左键按下去，播放按键音效
				if (m.uMsg == WM_LBUTTONDOWN) {
					mciSendString(_T("close click"), NULL, 0, NULL);
					mciSendString(_T("open ./music/click.mp3 alias click"), 
                                    NULL, 0, NULL);
					mciSendString(_T("play click"), NULL, 0, NULL);
					timer.Sleep(time_sleep);
					pause_exit = true;
				}
			}
			else click_continue = false;
			if (m.x >= 200 && m.x <= 200 + 185 && m.y >= 600 && m.y <= 700) {
				click_backtomenu = true;
                //鼠标在按键范围内且左键按下去，播放按键音效
				if (m.uMsg == WM_LBUTTONDOWN) {
					mciSendString(_T("close click"), NULL, 0, NULL);
					mciSendString(_T("open ./music/click.mp3 alias click"), 
                                    NULL, 0, NULL);
					mciSendString(_T("play click"), NULL, 0, NULL);
					timer.Sleep(time_sleep);
					gameover.GameoverBackMenu(); //返回开始界面
				}
			}
			else click_backtomenu = false;
		}
	}
};
Pause pause;

/*--------------- 定义 ez 对象 ---------------*/
class Hero {
private:
	/*----- 定义图片对象 -----*/
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
	double ez_v; //速度
	double disx, disy;
	char ez_direction; //初始化ez的方向
	char eze; //技能1：闪现
	bool moveState;

public:
	void InitHero() {
		//加载图片
		loadimage(&sn_up, _T("./images/sn_up.png"), 
                    sn_width, sn_height);
		loadimage(&sn_down, _T("./images/sn_down.png"), 
                    sn_width, sn_height);
		loadimage(&sn_left, _T("./images/sn_left.png"), 
                    sn_width, sn_height);
		loadimage(&sn_right, _T("./images/sn_right.png"), 
                    sn_width, sn_height);
		loadimage(&sn_upx[1], _T("./images/sn_up1.png"), 
                    sn_width, sn_height);
		loadimage(&sn_upx[2], _T("./images/sn_up2.png"), 
                    sn_width, sn_height);
		loadimage(&sn_upx[3], _T("./images/sn_up3.png"), 
                    sn_width, sn_height);
		loadimage(&sn_upx[4], _T("./images/sn_up4.png"), 
                    sn_width, sn_height);
		loadimage(&sn_upx[5], _T("./images/sn_up5.png"), 
                    sn_width, sn_height);
		loadimage(&sn_upx[6], _T("./images/sn_up6.png"), 
                    sn_width, sn_height);
		loadimage(&sn_upx[7], _T("./images/sn_up7.png"), 
                    sn_width, sn_height);
		loadimage(&sn_upx[8], _T("./images/sn_up8.png"), 
                    sn_width, sn_height);
		loadimage(&sn_upx[9], _T("./images/sn_up9.png"), 
                    sn_width, sn_height);
		loadimage(&sn_upx[10], _T("./images/sn_up10.png"), 
                    sn_width, sn_height);
		loadimage(&sn_upx[11], _T("./images/sn_up11.png"), 
                    sn_width, sn_height);
		loadimage(&sn_upx[12], _T("./images/sn_up12.png"), 
                    sn_width, sn_height);
		loadimage(&sn_upx[13], _T("./images/sn_up13.png"), 
                    sn_width, sn_height);
		loadimage(&sn_upx[14], _T("./images/sn_up14.png"), 
                    sn_width, sn_height);
		loadimage(&sn_downx[1], _T("./images/sn_down1.png"), 
                    sn_width, sn_height);
		loadimage(&sn_downx[2], _T("./images/sn_down2.png"), 
                    sn_width, sn_height);
		loadimage(&sn_downx[3], _T("./images/sn_down3.png"), 
                    sn_width, sn_height);
		loadimage(&sn_downx[4], _T("./images/sn_down4.png"), 
                    sn_width, sn_height);
		loadimage(&sn_downx[5], _T("./images/sn_down5.png"), 
                    sn_width, sn_height);
		loadimage(&sn_downx[6], _T("./images/sn_down6.png"), 
                    sn_width, sn_height);
		loadimage(&sn_downx[7], _T("./images/sn_down7.png"), 
                    sn_width, sn_height);
		loadimage(&sn_downx[8], _T("./images/sn_down8.png"), 
                    sn_width, sn_height);
		loadimage(&sn_downx[9], _T("./images/sn_down9.png"), 
                    sn_width, sn_height);
		loadimage(&sn_downx[10], _T("./images/sn_down10.png"), 
                    sn_width, sn_height);
		loadimage(&sn_downx[11], _T("./images/sn_down11.png"), 
                    sn_width, sn_height);
		loadimage(&sn_downx[12], _T("./images/sn_down12.png"), 
                    sn_width, sn_height);
		loadimage(&sn_downx[13], _T("./images/sn_down13.png"), 
                    sn_width, sn_height);
		loadimage(&sn_downx[14], _T("./images/sn_down14.png"), 
                    sn_width, sn_height);
		loadimage(&sn_leftx[1], _T("./images/sn_left1.png"), 
                    sn_width, sn_height);
		loadimage(&sn_leftx[2], _T("./images/sn_left2.png"), 
                    sn_width, sn_height);
		loadimage(&sn_leftx[3], _T("./images/sn_left3.png"), 
                    sn_width, sn_height);
		loadimage(&sn_leftx[4], _T("./images/sn_left4.png"), 
                    sn_width, sn_height);
		loadimage(&sn_leftx[5], _T("./images/sn_left5.png"), 
                    sn_width, sn_height);
		loadimage(&sn_leftx[6], _T("./images/sn_left6.png"), 
                    sn_width, sn_height);
		loadimage(&sn_leftx[7], _T("./images/sn_left7.png"), 
                    sn_width, sn_height);
		loadimage(&sn_leftx[8], _T("./images/sn_left8.png"), 
                    sn_width, sn_height);
		loadimage(&sn_leftx[9], _T("./images/sn_left9.png"), 
                    sn_width, sn_height);
		loadimage(&sn_leftx[10], _T("./images/sn_left10.png"), 
                    sn_width, sn_height);
		loadimage(&sn_leftx[11], _T("./images/sn_left11.png"), 
                    sn_width, sn_height);
		loadimage(&sn_leftx[12], _T("./images/sn_left12.png"), 
                    sn_width, sn_height);
		loadimage(&sn_leftx[13], _T("./images/sn_left13.png"), 
                    sn_width, sn_height);
		loadimage(&sn_leftx[14], _T("./images/sn_left14.png"), 
                    sn_width, sn_height);
		loadimage(&sn_rightx[1], _T("./images/sn_right1.png"), 
                    sn_width, sn_height);
		loadimage(&sn_rightx[2], _T("./images/sn_right2.png"), 
                    sn_width, sn_height);
		loadimage(&sn_rightx[3], _T("./images/sn_right3.png"), 
                    sn_width, sn_height);
		loadimage(&sn_rightx[4], _T("./images/sn_right4.png"), 
                    sn_width, sn_height);
		loadimage(&sn_rightx[5], _T("./images/sn_right5.png"), 
                    sn_width, sn_height);
		loadimage(&sn_rightx[6], _T("./images/sn_right6.png"), 
                    sn_width, sn_height);
		loadimage(&sn_rightx[7], _T("./images/sn_right7.png"), 
                    sn_width, sn_height);
		loadimage(&sn_rightx[8], _T("./images/sn_right8.png"), 
                    sn_width, sn_height);
		loadimage(&sn_rightx[9], _T("./images/sn_right9.png"), 
                    sn_width, sn_height);
		loadimage(&sn_rightx[10], _T("./images/sn_right10.png"), 
                    sn_width, sn_height);
		loadimage(&sn_rightx[11], _T("./images/sn_right11.png"), 
                    sn_width, sn_height);
		loadimage(&sn_rightx[12], _T("./images/sn_right12.png"), 
                    sn_width, sn_height);
		loadimage(&sn_rightx[13], _T("./images/sn_right13.png"), 
                    sn_width, sn_height);
		loadimage(&sn_rightx[14], _T("./images/sn_right14.png"), 
                    sn_width, sn_height);
		loadimage(&flash, _T("./images/flash.png"));
		ez_v = 1;
		eze = 'w'; //初始化e的方向
		ez_direction = 'w'; //初始化ez的方向
		disx = 0;
		disy = 0;
		moveState = false;
	}

    //显示ez的函数
	void ShowHero() {
		drawAlpha(&sn, ez_x, ez_y); //画出ez
        //如果按下闪现
		if (flashis) drawAlpha(&flash, flash_x, flash_y); //绘制闪现的图片
	}

	void UpdateHero() {
		if (moveState) {
			static int wait = 1;
			static int i = 1;
            
			if (ez_direction == 'w') { //如果方向向上
				sn = sn_upx[i];//ez图片变为向上
				if (wait == 28) {
					i++;
					wait = 1;
				}
				if (i == 15) i = 1;
			} else if (ez_direction == 'a') { //如果方向向左
				sn = sn_leftx[i]; //ez图片变为向左
				if (wait == 28) {
					i++;
					wait = 1;
				}
				if (i == 15) i = 1;
			} else if (ez_direction == 's') { //如果方向向下
				sn = sn_downx[i]; //ez图片变为向下
				if (wait == 28)	{
					i++;
					wait = 1;
				}
				if (i == 15) i = 1;
			}
			else if (ez_direction == 'd') { //如果方向向右
				sn = sn_rightx[i]; //ez图片变为向右
				if (wait == 28) {
					i++;
					wait = 1;
				}
				if (i == 15) i = 1;
			}
            //依次判断是否碰到其他英雄
			for (int j = 0; j < 6; j++) {
				if (j % 3 == 0) { //第一种敌人
					disx = abs(ez_x - 41 - en_x[j] + 36);
					disy = abs(ez_y - 47 - en_y[j] + 30);
				} else if (j % 3 == 1) { //第二种敌人
					disx = abs(ez_x - 41 - en_x[j] + 67);
					disy = abs(ez_y - 47 - en_y[j] + 60);
				} else if (j % 3 == 2) { //第三种敌人
					disx = abs(ez_x - 41 - en_x[j] + 55);
					disy = abs(ez_y - 47 - en_y[j] + 44);
				}
				if (disx <= 36 && disy <= 30 && j % 3 == 0) alive = 0; //失败
				else if (disx <= 40 && disy <= 30 && j % 3 == 1) alive = 0;
				else if (disx <= 55 && disy <= 44 && j % 3 == 2) alive = 0;
			}
			wait++;
		} else {
            //如果方向向上
			if (ez_direction == 'w') sn = sn_upx[4];//ez图片变为向上
            //如果方向向左
			else if (ez_direction == 'a') sn = sn_leftx[4]; //ez图片变为向左
            //如果方向向下
			else if (ez_direction == 's') sn = sn_downx[4]; //ez图片变为向下
            //如果方向向右
			else if (ez_direction == 'd') sn = sn_rightx[4]; //ez图片变为向右
            //依次判断是否碰到其他英雄
			for (int j = 0; j < 6; j++) {
				if (j % 3 == 0) { //第一种敌人
					disx = abs(ez_x - 41 - en_x[j] + 36);
					disy = abs(ez_y - 47 - en_y[j] + 30);
				} else if (j % 3 == 1) { //第二种敌人
					disx = abs(ez_x - 41 - en_x[j] + 67);
					disy = abs(ez_y - 47 - en_y[j] + 60);
				} else if (j % 3 == 2) { //第三种敌人
					disx = abs(ez_x - 41 - en_x[j] + 55);
					disy = abs(ez_y - 47 - en_y[j] + 44);
				}
				if (disx <= 36 && disy <= 30 && j % 3 == 0) alive = 0; //失败
				else if (disx <= 40 && disy <= 30 && j % 3 == 1) alive = 0;
				else if (disx <= 55 && disy <= 44 && j % 3 == 2) alive = 0;
			}
		}
	}

    //闪现函数
	void ezE() {
        //按下e/k
		if (((GetAsyncKeyState(0x45) & 0x8000) || 
            (GetAsyncKeyState(0x4B) & 0x8000)) && iseze) {
			mciSendString(_T("close flash"), NULL, 0, NULL); //闪现音效
			mciSendString(_T("open ./music/flash.mp3 alias flash"), 
                            NULL, 0, NULL);
			mciSendString(_T("play flash"), NULL, 0, NULL);
			flashis = true; //按下后允许绘制闪现图片
			flash_x = ez_x; //获取闪现图片的坐标
			flash_y = ez_y; //获取闪现图片的坐标
			if (eze == 'a') { //如果方向向左
				ez_x -= 200; //向左位移200
				iseze = 0; //e进入冷却
			} else if (eze == 'd') { //如果方向向右
				ez_x += 200; //向右位移200
				iseze = 0; //e进入冷却
			} else if (eze == 'w') { //如果方向向上
				ez_y -= 200; //向前位移200
				iseze = 0; //e进入冷却
			} else if (eze == 's') { //如果方向向下
				ez_y += 200; //向后位移200
				iseze = 0; //e进入冷却
			}
		}
	}

    //ez移动函数
	void MoveHero() {
		if (!is) //如果q没有绘制
			eza_dir = ez_direction; //子弹方向始终与ez方向一致
		if (iseze) //如果e可用
			eze = ez_direction; //e的方向始终与ez方向一致
		ezE(); //调用e函数
        //如果按下a
		if ((GetAsyncKeyState(0x41) & 0x8000)) {
			moveState = true; 
			ez_x -= ez_v; //向左移动
			ez_direction = 'a'; //定义方向向左
            //如果到边界
			if (ez_x <= 0) {
				ez_direction = 'd'; //不断改变方向 使其静止 从而到达不能走出边界的目的
				ez_x += ez_v;
				ez_direction = 'a';
			}
		}
        //如果按下d
		if ((GetAsyncKeyState(0x44) & 0x8000)) {
			moveState = true;
			ez_x += ez_v; //向右移动
			ez_direction = 'd'; //定义方向向右
            //如果到达边界
			if (ez_x >= WIDTH - double(sn_width)) {
				ez_direction = 'a'; //不断改变方向 使其静止 从而到达不能走出边界的目的
				ez_x -= ez_v;
				ez_direction = 'd';
			}
		}
        //如果按下w
		if (GetAsyncKeyState(0x57) & 0x8000) {
			moveState = true;
			ez_y -= ez_v; //向上移动
			ez_direction = 'w'; //定义方向向上
            //如果到达边界
			if (ez_y <= 0) {
				ez_direction = 's'; //不断改变方向 使其静止 从而到达不能走出边界的目的
				ez_y += ez_v;
				ez_direction = 'w';
			}
		}
        //如果按下s
		if ((GetAsyncKeyState(0x53) & 0x8000)) {
			moveState = true;
			ez_y += ez_v; //向下移动
			ez_direction = 's'; //定义方向向下
            //如果到达边界
			if (ez_y >= HEIGHT - double(sn_height)) {
				ez_direction = 'w'; //不断改变方向 使其静止 从而到达不能走出边界的目的
				ez_y -= ez_v;
				ez_direction = 's';
			}
		}
	}

    //秘术射击（q)函数
	void ShowSkill() {
		//设置冷却时间条
		setbkmode(TRANSPARENT);
		settextcolor(RGB(255, 255, 255));
		settextstyle(20, 0, _T("consolas"));
		outtextxy(180, 25, _T("E / K"));
		if (iseze) { //当e为可用时
			//绘制绿色 满的 冷却条
			setlinecolor(RGB(0, 0, 0));
			setfillcolor(RGB(0, 255, 0));
			solidrectangle(30, 30, 170, 40);
			xe = 30;
		} else if (!iseze) { //当e不可用时
			if (xe < 170) { //在进度条满之前 进度条归0 变为红色 且逐步增加
				xe += 0.08;
				setlinecolor(RGB(0, 0, 0));
				setfillcolor(RED);
				solidrectangle(30, 30, xe, 40);
			} else { //当进度条满了 而实际冷却时间没好时 仍为红色
				setlinecolor(RGB(0, 0, 0));
				setfillcolor(RED);
				solidrectangle(30, 30, xe, 40);
			}
		}
        //在模式skillshot中
		if (start_mode1) {
			//绘制q的冷却时间条
			setbkmode(TRANSPARENT);
			settextcolor(RGB(255, 255, 255));
			settextstyle(20, 0, _T("consolas"));
			outtextxy(180, 50, _T("Q / J"));
			if (isezq) { //如果q可用
				//绘制绿色 满的冷却条
				setlinecolor(RGB(0, 0, 0));
				setfillcolor(RGB(0, 255, 0));
				solidrectangle(30, 60, 170, 70);
				xq = 30;
			} else if (!isezq) { //如果q不可用
				if (xq < 170) { //在进度条满之前 进度条归0 变为红色 且逐步增加
					xq += 0.7;
					setlinecolor(RGB(0, 0, 0));
					setfillcolor(RED);
					solidrectangle(30, 60, xq, 70);
				} else { //当进度条满了 而实际冷却时间没好时 仍为红色
				
					setlinecolor(RGB(0, 0, 0));
					setfillcolor(RED);
					solidrectangle(30, 60, xq, 70);
				}
			}
		}
	}
};
Hero hero;

/*--------------- 定义敌人对象 ---------------*/
class Enemy {
private:
	/*----- 定义图片对象 -----*/
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
	double en_v[6]; //总速度

public:
	void InitEnemy() {
		//加载图片
		loadimage(&timo_up, _T("./images/timo_up.png"), 
                    timo_width, timo_height);
		loadimage(&timo_down, _T("./images/timo_down.png"), 
                    timo_width, timo_height);
		loadimage(&timo_left, _T("./images/timo_left.png"), 
                    timo_width, timo_height);
		loadimage(&timo_right, _T("./images/timo_right.png"), 
                    timo_width, timo_height);
		loadimage(&timo_upleft, _T("./images/timo_upleft.png"), 
                    timo_width, timo_height);
		loadimage(&timo_upright, _T("./images/timo_upright.png"), 
                    timo_width, timo_height);
		loadimage(&timo_downleft, _T("./images/timo_downleft.png"), 
                    timo_width, timo_height);
		loadimage(&timo_downright, _T("./images/timo_downright.png"), 
                    timo_width, timo_height);
		loadimage(&yasuo_up, _T("./images/yasuo_up.png"), 
                    yasuo_heightd, yasuo_widthd);
		loadimage(&yasuo_down, _T("./images/yasuo_down.png"), 
                    yasuo_heightd, yasuo_widthd);
		loadimage(&yasuo_left, _T("./images/yasuo_left.png"), 
                    yasuo_widthd, yasuo_heightd);
		loadimage(&yasuo_right, _T("./images/yasuo_right.png"), 
                    yasuo_widthd, yasuo_heightd);
		loadimage(&yasuo_upleft, _T("./images/yasuo_upleft.png"), 
                    yasuo_width, yasuo_height);
		loadimage(&yasuo_upright, _T("./images/yasuo_upright.png"), 
                    yasuo_width, yasuo_height);
		loadimage(&yasuo_downleft, _T("./images/yasuo_downleft.png"), 
                    yasuo_width, yasuo_height);
		loadimage(&yasuo_downright, _T("./images/yasuo_downright.png"),     
                    yasuo_width, yasuo_height);
		loadimage(&zed_up, _T("./images/zed_up.png"), 
                    zed_heightd, zed_widthd);
		loadimage(&zed_down, _T("./images/zed_down.png"), 
                    zed_heightd, zed_widthd);
		loadimage(&zed_left, _T("./images/zed_left.png"), 
                    zed_widthd, zed_heightd);
		loadimage(&zed_right, _T("./images/zed_right.png"), 
                    zed_widthd, zed_heightd);
		loadimage(&zed_upleft, _T("./images/zed_upleft.png"), 
                    zed_width, zed_height);
		loadimage(&zed_upright, _T("./images/zed_upright.png"), 
                    zed_width, zed_height);
		loadimage(&zed_downleft, _T("./images/zed_downleft.png"), 
                    zed_width, zed_height);
		loadimage(&zed_downright, _T("./images/zed_downright.png"), 
                    zed_width, zed_height);
		Enexist[0] = true;
        //初始化由两边生成敌人
		for (int i = 0; i < 6; i++) {
			if (i % 2 == 1) en_x[i] = -50;
			else en_x[i] = 1570;
			en_y[i] = rand() % 785;
			en_v[i] = (rand() % 2) / 10 + 0.3; //对速度的初始化加入随机性
		}
	}

    //敌人移动函数
	void MoveEnemy() {
        //循环遍历每一个敌人的参数
		for (int i = 0; i < 6; i++) {
            //如果敌人可用
			if (Enexist[i]) {
				encos[i] = (ez_x - en_x[i]) 
                    / sqrt((ez_x - en_x[i]) * (ez_x - en_x[i]) 
                    + (ez_y - en_y[i]) * (ez_y - en_y[i])); //角度
				ensin[i] = (ez_y - en_y[i]) 
                    / sqrt((ez_x - en_x[i]) * (ez_x - en_x[i]) 
                    + (ez_y - en_y[i]) * (ez_y - en_y[i])); //角度
				en_vx[i] = encos[i] * en_v[i]; //分速度
				en_vy[i] = ensin[i] * en_v[i]; //分速度
				en_x[i] += en_vx[i]; //实现移动
				en_y[i] += en_vy[i]; //实现移动
                //如果被击中
				if (!eis[i]) {
					int p = rand() % 4; //引入随机数
					if (p % 4 == 1) { //此情况下在屏幕右方重新生成
						en_x[i] = 1770;
						en_y[i] = rand() % 785;
						eis[i] = true; //改变是否被击中的状态
					} else if (p % 4 == 2) { //此情况下在屏幕左方重新生成
						en_x[i] = -170;
						en_y[i] = rand() % 785;
						eis[i] = true; //改变是否被击中的状态
					} else if (p % 4 == 3) { //此情况下在屏幕上方重新生成
						en_y[i] = -170;
						en_x[i] = rand() % 1520;
						eis[i] = true; //改变是否被击中的状态
					} else if (p % 4 == 0) { //此情况下在屏幕下方重新生成
						en_y[i] = 950;
						en_x[i] = rand() % 1520;
						eis[i] = true; //改变是否被击中的状态
					}
                    //如果敌人速度小于阙值
					if (en_v[i] <= 0.85) en_v[i] += 0.066; //每被击中一次 增加速度
				}
			}
		}
	}

    //击中判断的函数
	void attack() {
		double disx, disy; //定义距离变量
        //遍历敌人参数
		for (int j = 0; j < Enmax; j++) {
			//计算子弹和敌人距离
			if (j % 3 == 0) { //第一种敌人
				disx = abs(eza_x - en_x[j] - 36);
				disy = abs(eza_y - en_y[j] - 30);
			} else if (j % 3 == 1) { //第二种敌人
				disx = abs(eza_x - en_x[j] - 45 * 1.5);
				disy = abs(eza_y - en_y[j] - 60);
			} else if (j % 3 == 2) { //第三种敌人
				disx = abs(eza_x - en_x[j] - 50);
				disy = abs(eza_y - en_y[j] - 50);
			}
			//每种敌人图片大小不一样   对应参数也不一样
            //当子弹击中敌人后
			if (disx <= 36 && disy <= 30 && j % 3 == 0) {
				eis[j] = false; //敌人被击中
				eza_x = -1000; //移开子弹
				eza_y = -1000; //移开子弹
				score1++; //skillshot模式得分加一
				bulldis = false; //子弹状态改变
			}
            //当子弹击中敌人后
			if (disx <= 60 && disy <= 60 && j % 3 == 1) {
				eis[j] = false; //敌人被击中
				eza_x = -1000; //移开子弹
				eza_y = -1000; //移开子弹
				score1++; //skillshot模式得分加一
				bulldis = false; //子弹状态改变
			}
            //当子弹击中敌人后
			if (disx <= 50 && disy <= 50 && j % 3 == 2) {
				eis[j] = false; //敌人被击中
				eza_x = -1000; //移开子弹
				eza_y = -1000; //移开子弹
				score1++; //skillshot模式得分加一
				bulldis = false; //子弹状态改变
			}
		}
	}

    //敌人绘制函数
	void ShowEnemy() {
        //遍历敌人  当敌人处于相对ez的不同位置时 显示不同的图片
		for (int i = 0; i < 6; i++) {
			if (Enexist[i] && i % 3 == 0) { //第一种敌人
				if (en_x[i] >= ez_x - 100 && en_x[i] < ez_x + 100 && en_y[i] < ez_y)
					timo = timo_down;
				else if (en_x[i] >= ez_x - 100 && en_x[i] < ez_x + 100 && 
                        en_y[i] >= ez_y)
					timo = timo_up;
				else if (en_x[i] < ez_x && en_y[i] >= ez_y - 100 && 
                        en_y[i] < ez_y + 100)
					timo = timo_left;
				else if (en_x[i] >= ez_x && en_y[i] >= ez_y - 100 && 
                        en_y[i] < ez_y + 100)
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
			} else if (Enexist[i] && i % 3 == 1) { //第二种敌人
				if (en_x[i] >= ez_x - 100 && en_x[i] < ez_x + 100 && 
                        en_y[i] < ez_y)
					zed = zed_down;
				else if (en_x[i] >= ez_x - 100 && en_x[i] < ez_x + 100 && 
                        en_y[i] >= ez_y)
					zed = zed_up;
				else if (en_x[i] < ez_x && en_y[i] >= ez_y - 100 && 
                        en_y[i] < ez_y + 100)
					zed = zed_left;
				else if (en_x[i] >= ez_x && en_y[i] >= ez_y - 100 && 
                        en_y[i] < ez_y + 100)
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
			} else if (Enexist[i] && i % 3 == 2) { //第三种敌人
				if (en_x[i] >= ez_x - 100 && en_x[i] < ez_x + 100 && 
                        en_y[i] < ez_y)
					yasuo = yasuo_down;
				else if (en_x[i] >= ez_x - 100 && en_x[i] < ez_x + 100 && 
                        en_y[i] >= ez_y)
					yasuo = yasuo_up;
				else if (en_x[i] < ez_x && en_y[i] >= ez_y - 100 && 
                        en_y[i] < ez_y + 100)
					yasuo = yasuo_right;
				else if (en_x[i] >= ez_x && en_y[i] >= ez_y - 100 && 
                        en_y[i] < ez_y + 100)
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

/*--------------- 定义子弹对象 ---------------*/
class Bullet {
private:
	/*----- 定义图片对象 -----*/
	IMAGE bullets;
	IMAGE bullet_up;
	IMAGE bullet_down;
	IMAGE bullet_left;
	IMAGE bullet_right;
	double eza_v; //定义子弹速度
	double eza_l; //定义子弹里程数

public:
	void InitBullet() {
		//加载图片
		loadimage(&bullet_up, _T("./images/bullet_up.png"), 
                    bullet_width, bullet_height);
		loadimage(&bullet_down, _T("./images/bullet_down.png"), 
                    bullet_width, bullet_height);
		loadimage(&bullet_left, _T("./images/bullet_left.png"), 
                    bullet_height, bullet_width);
		loadimage(&bullet_right, _T("./images/bullet_right.png"), 
                    bullet_height, bullet_width);
		eza_v = 3.2; //定义子弹速度
		eza_x = ez_x; //定位子弹发射时坐标
		eza_y = ez_y; //定位子弹发射时坐标
		eza_l = 0;
	}

    //绘制子弹函数
	void ShowBullet() {
        //需要绘制子弹时
		if (is) drawAlpha(&bullets, eza_x, eza_y); //绘制子弹
	}

    //更新子弹数据
	void UpdataBullet() {
		//定义子弹方向始终和ez方向一样，并使用对应的子弹图片
		if (eza_dir == 'a') {
			eza_x -= eza_v;
			bullets = bullet_left;
		} else if (eza_dir == 'd') {
			eza_x += eza_v;
			bullets = bullet_right;
		} else if (eza_dir == 'w') {
			eza_y -= eza_v;
			bullets = bullet_up;
		} else if (eza_dir == 's') {
			eza_y += eza_v;
			bullets = bullet_down;
		}
        //如果需要绘制子弹
		if (is) eza_l += eza_v; //记录子弹里程数
        //击中时 或到达里程数时
		if (!bulldis) {
			eza_l = 0; //里程归0
			bulldis = 1; //子弹击中状态改变
		}
        //里程到达600时
		if (eza_l >= 600) {
			is = false; //不再绘制子弹
			eza_l = 0; //里程归0
			eza_x = -1000; //移开子弹
			eza_y = -1000; //移开子弹
		}
	}

    //子弹移动函数
	void MoveBullet() {
        //按下q/j
		if (((GetAsyncKeyState(0x51) & 0x8000) || 
            (GetAsyncKeyState(0x4A) & 0x8000)) && is == 0 && isezq == 1) {
			//加入音效
			mciSendString(_T("close biu"), NULL, 0, NULL);
			mciSendString(_T("open ./music/biu.mp3 alias biu"), NULL, 0, NULL);
			mciSendString(_T("play biu"), NULL, 0, NULL);
			eza_x = ez_x + 28; //子弹移动
			eza_y = ez_y + 28; //子弹移动
			is = true; //绘制子弹
			isezq = false; //q进入冷却
		}
	}
};
Bullet bullet;

/*--------------- 定义火球对象 对应第二种模式 ---------------*/
class Fireball {
private:
	/*----- 定义图片对象 -----*/
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
	double fire_x[6], fire_y[6], firedir[6], fire_v;
	double firecos[6], firesin[6], firevx[6], firevy[6];

public:
	void InitFireball() {
		//加载图片
		loadimage(&fire_up, _T("./images/fire_up.png"));
		loadimage(&fire_down, _T("./images/fire_down.png"));
		loadimage(&fire_left, _T("./images/fire_left.png"));
		loadimage(&fire_right, _T("./images/fire_right.png"));
		loadimage(&fire_upleft, _T("./images/fire_upleft.png"));
		loadimage(&fire_upright, _T("./images/fire_upright.png"));
		loadimage(&fire_downleft, _T("./images/fire_downleft.png"));
		loadimage(&fire_downright, _T("./images/fire_downright.png"));
		fire_v = 2;
        //遍历火球
		for (int i = 0; i < 6; i++) {
			firecos[i] = (ez_x - fire_x[i]) 
                        / sqrt((ez_x - fire_x[i]) * (ez_x - fire_x[i]) 
                        + (ez_y - fire_y[i]) * (ez_y - fire_y[i])); //计算角度
			firesin[i] = (ez_y - fire_y[i]) 
                        / sqrt((ez_x - fire_x[i]) * (ez_x - fire_x[i]) 
                        + (ez_y - fire_y[i]) * (ez_y - fire_y[i])); //计算角度
			firevx[i] = firecos[i] * fire_v; //计算分速度
			firevy[i] = firesin[i] * fire_v; //计算分速度
			int p = rand() % 4; //引入随机数
            //此情况下 火球在屏幕左方生成
			if (p == 0) {
				fire_x[i] = -200;
				fire_y[i] = rand() % 700;
			}
            //此情况下 火球在屏幕右方生成
			if (p == 1) {
				fire_x[i] = 1720;
				fire_y[i] = rand() % 700;
			}
            //此情况下 火球在屏幕上方生成
			if (p == 2) {
				fire_x[i] = rand() % 1500;
				fire_y[i] = -200;
			}
            //此情况下 火球在屏幕下方生成
			if (p == 3) {
				fire_x[i] = rand() % 1500;
				fire_y[i] = 985;
			}
			//根据火球初始化的位置 确定方向
			if (fire_x[i] < 800 && fire_y[i] < 390) firedir[i] = 0;
			else if (fire_x[i] < 800 && fire_y[i] > 390) firedir[i] = 1;
			else if (fire_x[i] > 800 && fire_y[i] < 390) firedir[i] = 2;
			else if (fire_x[i] > 800 && fire_y[i] > 390) firedir[i] = 3;
		}
	}

    //绘制火球图片
	void ShowFireball() {
        //遍历火球参数
		for (int i = 0;i < 6;i++) {
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

    //火球移动函数
	void BallMove() {
        //遍历火球参数
		for (int i = 0; i < 6; i++) {
			fire_x[i] += firevx[i]; //移动火球
			fire_y[i] += firevy[i]; //移动火球
            //当火球飞出屏幕时
			if (fire_x[i] >= 1800 || fire_x[i] <= -250 || 
                fire_y[i] >= 1000 || fire_y[i] <= -250) {
				score2++;
				int p = rand() % 4; //引入随机数
				if (p == 0) { //此情况下 在屏幕左边生成
					fire_x[i] = -200;
					fire_y[i] = rand() % 700;
					firecos[i] = (ez_x - fire_x[i]) 
                        / sqrt((ez_x - fire_x[i]) * (ez_x - fire_x[i]) 
                        + (ez_y - fire_y[i]) * (ez_y - fire_y[i]));
					firesin[i] = (ez_y - fire_y[i]) 
                        / sqrt((ez_x - fire_x[i]) * (ez_x - fire_x[i]) 
                        + (ez_y - fire_y[i]) * (ez_y - fire_y[i]));
					firevx[i] = firecos[i] * fire_v;
					firevy[i] = firesin[i] * fire_v;
				} else if (p == 1) { //此情况下 在屏幕右边生成
					fire_x[i] = 1720;
					fire_y[i] = rand() % 700;
					firecos[i] = (ez_x - fire_x[i]) 
                        / sqrt((ez_x - fire_x[i]) * (ez_x - fire_x[i]) 
                        + (ez_y - fire_y[i]) * (ez_y - fire_y[i]));
					firesin[i] = (ez_y - fire_y[i]) 
                        / sqrt((ez_x - fire_x[i]) * (ez_x - fire_x[i]) 
                        + (ez_y - fire_y[i]) * (ez_y - fire_y[i]));
					firevx[i] = firecos[i] * fire_v;
					firevy[i] = firesin[i] * fire_v;
				} else if (p == 2) { //此情况下 在屏幕上边生成
					fire_x[i] = rand() % 1500;
					fire_y[i] = -200;
					firecos[i] = (ez_x - fire_x[i]) 
                        / sqrt((ez_x - fire_x[i]) * (ez_x - fire_x[i]) 
                        + (ez_y - fire_y[i]) * (ez_y - fire_y[i]));
					firesin[i] = (ez_y - fire_y[i]) 
                        / sqrt((ez_x - fire_x[i]) * (ez_x - fire_x[i]) 
                        + (ez_y - fire_y[i]) * (ez_y - fire_y[i]));
					firevx[i] = firecos[i] * fire_v;
					firevy[i] = firesin[i] * fire_v;
				} else if (p == 3) { //此情况下 在屏幕下边生成
					fire_x[i] = rand() % 1500;
					fire_y[i] = 985;
					firecos[i] = (ez_x - fire_x[i]) 
                        / sqrt((ez_x - fire_x[i]) * (ez_x - fire_x[i]) 
                        + (ez_y - fire_y[i]) * (ez_y - fire_y[i]));
					firesin[i] = (ez_y - fire_y[i]) 
                        / sqrt((ez_x - fire_x[i]) * (ez_x - fire_x[i]) 
                        + (ez_y - fire_y[i]) * (ez_y - fire_y[i]));
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
		for (int i = 0; i < 6; i++) {
			if (firedir[i] == 1 || firedir[i] == 2 || firedir[i] == 3 || firedir[i] == 0) {
				if (sqrt((ez_x - 41 - fire_x[i] + 50) * (ez_x - 41 - fire_x[i] + 50) 
                    + (ez_y - 47 - fire_y[i] + 55) * (ez_y - 47 - fire_y[i] + 55)) <= 65)
					alive = false;
			} else if (firedir[i] == 6 || firedir[i] == 4) {
				if (sqrt((ez_x - 41 - fire_x[i] + 65) * (ez_x - 41 - fire_x[i] + 65) 
                    + (ez_y - 47 - fire_y[i] + 32.5) * (ez_y - 47 - fire_y[i] + 32.5)) <= 65)
					alive = false;
			} else if (firedir[i] == 5 || firedir[i] == 7) {
				if (sqrt((ez_x - 41 - fire_x[i] + 32.5) * (ez_x - 41 - fire_x[i] + 32.5) 
                    + (ez_y - 47 - fire_y[i] + 65) * (ez_y - 47 - fire_y[i] + 65)) <= 60)
					alive = false;
			}
		}
	}
};
Fireball ball;

/*----------------------------------------------*/
/*--------------- 主函数，游戏框架 ---------------*/
/*----------------------------------------------*/

/*--------------- 初始化函数 ---------------*/
void StartUp() {
	initgraph(WIDTH, HEIGHT); //初始画布
	changetitle();
	LoadImage(); //加载图片
	//调用初始化函数
	Init();
	menu.initMenu();
	gameover.initGameover();
	pause.initPause();
	hero.InitHero();
	enemy.InitEnemy();
	bullet.InitBullet();
	ball.InitFireball();
	BeginBatchDraw(); //开始批量绘制
}

/*--------------- 没有数据输入时更新 ---------------*/
void UpdateWithoutInput() {
	static int lastSecond = 0; //记录前一次运行的秒数
	static int nowSecond = 0; //记录当前运行了多少秒
	static clock_t start = clock(); //记录第一次运行时刻
	clock_t now = clock(); //获得当前时刻
	static int estartsecond = 0; //初始化e的开始时间
	static int elastsecond = 0; //初始化e的持续时间
	static int qstartsecond = 0; //初始化q的开始时间
	static int qlastsecond = 0; //初始化q的结束时间
    //随着时间的增加 增加最大敌人数 并设置上限
	if (nowSecond == lastSecond + 5 && Enmax <= 6) {
		lastSecond = nowSecond; //更新时间数据
		Enmax += 1; //敌人最大数量加一
		Enexist[Enmax - 2] = true; //设置新增加敌人可用
	}
	nowSecond = (int(now - start) / CLOCKS_PER_SEC); //更新时间数据
    //如果e进入冷却
	if (!iseze) {
		elastsecond = nowSecond; //更新时间数据
        //第一次记录e开始时间
		if (eif) {
			estartsecond = nowSecond; //更新e开始冷却的数据
			eif = false; //防止重复更新数据
		}
        //闪现10秒冷却时间
		if (elastsecond == estartsecond + 10) {
			iseze = true; //e状态改为可用
			eif = true; //下一次更新e开始冷却的数据可用
		}
		if (elastsecond == estartsecond + 1) //闪现图片绘制1秒
			flashis = false; //一秒后 闪现图片不绘制
	}
    //在选择模式1后
	if (start_mode1) {
		nowSecond = (int(now - start) / CLOCKS_PER_SEC);//更新时间
        //q不可用时
		if (!isezq) {
			qlastsecond = nowSecond; //更新时间数据
            //第一次记录q开始时间
			if (qif) {
				qstartsecond = nowSecond; //更新e开始冷却的数据
				qif = false; //防止重复更新数据
			}
            //q的冷却为一秒
			if (qlastsecond == qstartsecond + 1) {
				isezq = true; //q状态改为可用
				qif = true; //下一次更新q开始冷却的数据可用
			}
		}
	}
	//改变帧数
	static int waitIndex = 1;
	waitIndex++;
	if (waitIndex == 2) {
		hero.MoveHero();
		if (start_mode1) { //选择模式一 调用所需函数
			enemy.MoveEnemy();
			bullet.MoveBullet();
			enemy.attack();
		} else if (start_mode2) { //选择模式二 调用所需函数
			ball.BallMove();
		waitIndex = 1;
	}
}

/*--------------- 有数据输入时更新 ---------------*/
void UpdateWithInput() {
	hero.UpdateHero();
	if ((GetAsyncKeyState(0x50) & 0x8000)) //按下p键，游戏暂停
		pause.BeginPause();                //进入暂停页面
	if ((GetAsyncKeyState(0x54) & 0x8000)) //按下t键，游戏暂停，查看提示
		menu.Tips();                       //进入提示页面
	gameover.BeginGameover();
}

/*--------------- 绘制函数 ---------------*/
void Show() {
	cleardevice();
	putimage(0, 0, &game_bk); //游戏背景图片
	hero.ShowHero();  //绘制hero
	hero.ShowSkill(); //绘制hero状态条
	if (start_mode1) { //选择模式一 调用相关函数
		enemy.ShowEnemy();
		bullet.UpdataBullet();
		bullet.ShowBullet();
	} else if (start_mode2) //选择模式二 调用相关函数
		ball.ShowFireball();
	FlushBatchDraw();
}

/*--------------- 主函数 ---------------*/
int main(int argc, char* argv[]) {
	StartUp(); //初始化
	load.beginAnimation();
	menu.BeginMenu(); //页面初始化
	while (1) {
		Show(); //绘制函数
		UpdateWithoutInput(); //数据更新
		UpdateWithInput(); //数据更新
	}
	return 0;
}
