#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <mmstream.h>
#pragma comment(lib, "Winmm.lib")

#define PI 3.14159265359
#define WIDTH 1520
#define HEIGHT 785
#define sn_width 91//人物宽度
#define sn_height 104//人物高度

IMAGE background;
IMAGE sn;
IMAGE sn_up;
IMAGE sn_down;
IMAGE sn_left;
IMAGE sn_right;
IMAGE sn_northeast;
IMAGE sn_southeast;
IMAGE sn_northwest;
IMAGE sn_southwest;
IMAGE yasuo;

// 载入PNG图并去透明部分
void drawAlpha(IMAGE* picture, int  picture_x, int picture_y) //x为载入图片的X坐标，y为Y坐标
{
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //获取picture的显存指针
	int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
	int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
	int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
	int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
	int dstX = 0;    //在显存里像素的角标
	for (int iy = 0; iy < picture_height; iy++) // 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; //在显存里像素的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
			}
		}
	}
}

void loadimage()
{
	loadimage(&background, _T("./images/bk.jpg"));
	loadimage(&sn, _T("./images/sn.png"), sn_width, sn_height);
	loadimage(&sn_up, _T("./images/sn_up.png"), sn_width, sn_height);
	loadimage(&sn_down, _T("./images/sn_down.png"), sn_width, sn_height);
	loadimage(&sn_left, _T("./images/sn_left.png"), sn_width, sn_height);
	loadimage(&sn_right, _T("./images/sn_right.png"), sn_width, sn_height);
	loadimage(&sn_northeast, _T("./images/sn_northeast.png"), sn_width, sn_height);
	loadimage(&sn_southeast, _T("./images/sn_southeast.png"), sn_width, sn_height);
	loadimage(&sn_northwest, _T("./images/sn_northwest.png"), sn_width, sn_height);
	loadimage(&sn_southwest, _T("./images/sn_southwest.png"), sn_width, sn_height);
	loadimage(&yasuo, _T("./images/yasuo.png"));
}

class Hero {
public:
	double Enx, Eny, Env;
	double Esin, Ecos, Envx, Envy;
	double ez_x = WIDTH / 2;
	double ez_y = HEIGHT / 2;
	double ez_v = 1;//速度
	double eza_x, eza_y, eza_v, eza_l = 0, eza_dir = 'w';
	int is = 0;
	char ez_direction = 'w';

	void ShowHero() 
	{
		drawAlpha(&sn, ez_x, ez_y);
	}

	void UpdateHero()
	{
		if (ez_direction == 'w') 
			sn = sn_up;
		else if (ez_direction == 'a') 
			sn = sn_left;
		else if (ez_direction == 's') 
			sn = sn_down;
		else if (ez_direction == 'd') 
			sn = sn_right;
	}

	void MoveHero()
	{
		if ((GetAsyncKeyState(0x41) & 0x8000)) 
		{ //a
			ez_x -= ez_v;
			ez_direction = 'a';
			if (!is)
				eza_dir = 'a';
		}
		if ((GetAsyncKeyState(0x44) & 0x8000)) 
		{ //d
			ez_x += ez_v;
			ez_direction = 'd';
			if (!is)
				eza_dir = 'd';
		}
		if (GetAsyncKeyState(0x57) & 0x8000) 
		{  // w
			ez_y -= ez_v;
			ez_direction = 'w';
			if (!is)
				eza_dir = 'w';
		}
		if ((GetAsyncKeyState(0x53) & 0x8000)) 
		{ //s
			ez_y += ez_v;
			ez_direction = 's';
			if (!is)
				eza_dir = 's';
		}
	}

	void StartUpBullet()
	{
		eza_v = 3;
		eza_x = ez_x;
		eza_y = ez_y;
	}

	void UpdataBullet()
	{
		if (eza_dir == 'a')
		{
			eza_x -= eza_v;
			eza_l += eza_v;
		}
		else if (eza_dir == 'd')
		{
			eza_x += eza_v;
			eza_l += eza_v;
		}
		else if (eza_dir == 'w')
		{
			eza_y -= eza_v;
			eza_l += eza_v;
		}
		else if (eza_dir == 's')
		{
			eza_y += eza_v;
			eza_l += eza_v;
		}
		if (eza_l >= 800)
		{
			is = 0;
			eza_l = 0;
		}
	}

	void BulletMove()
	{
		if ((GetAsyncKeyState(0x51) & 0x8000) && is == 0)
		{
			is = 1;
			eza_x = ez_x;
			eza_y = ez_y;
		}
		drawAlpha(&yasuo, Enx, Eny);
	}

	void ShowBullet()
	{
		if (is == 1)
			drawAlpha(&yasuo, eza_x, eza_y);
	}

	void EnStartUp()
	{
		Enx = 200;
		Eny = 200;
		Env = 0.3;
	}

	void ShowEnemy()
	{
		drawAlpha(&yasuo, Enx, Eny);
	}
	
	void EnMove()
	{
		Ecos = (ez_x - Enx) / sqrt((ez_x - Enx) * (ez_x - Enx) + (ez_y - Eny) * (ez_y - Eny));
		Esin = (ez_y - Eny) / sqrt((ez_x - Enx) * (ez_x - Enx) + (ez_y - Eny) * (ez_y - Eny));
		Envx = Ecos * Env;
		Envy = Esin * Env;
		Enx += Envx;
		Eny += Envy;
	}
};
Hero hero;

void StartUp()
{
	initgraph(WIDTH, HEIGHT);
	hero.EnStartUp();
	hero.StartUpBullet();
	changetitle();
	loadimage();
	BeginBatchDraw();
}

void Show()
{
	putimage(0, 0, &background);
	hero.ShowHero();
	hero.ShowEnemy();
	hero.ShowBullet();
	FlushBatchDraw();
}

void UpdateWithoutInput()
{
	static int waitIndex = 1;
	waitIndex++;
	if (waitIndex == 2)
	{
		hero.MoveHero();
		hero.EnMove();
		hero.UpdataBullet();
		hero.BulletMove();
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
	while (1)
	{
		UpdateWithInput();
		UpdateWithoutInput();
		Show();
	}
	return 0;
}