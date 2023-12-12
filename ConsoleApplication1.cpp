#include <iostream>
#include <cmath>
#include <conio.h>
#include <SDL.h>

using namespace std;

float XX = 56.5;
float YY = 64;
float ZZ = 30;
float XX1 = 56.5;
float YY1 = 64;
float ZZ1 = 30;
const int N = 8;
const int N1 = 4;
const int M = 4;
const float DX = 2.0;
const float DY = 2.0;
const float DZ = 2.0;
const float ALPHA = 0.05;
const float S1 = 1.25;
const float S2 = 0.8;
const int A = 300;
const int B = 200;
const int A1 = 300;
const int B1 = 480;

void multiplication1(float line[N][M], float matrix[M][M])
{
	float res[N][M] = { {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} };
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			for (int k = 0; k < M; k++)
			{
				res[i][j] += (line[i][k] * matrix[k][j]);
			}
		}
	}


	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			line[i][j] = res[i][j];
		}
	}
}


void movement1(float line[N][M], float dx, float dy, float dz)
{
	float m_move[M][M] = { {1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {dx,dy,dz,1} };
	multiplication1(line, m_move);
}

void scaling1(float line[N][M], float S)
{
	float m_scale[M][M] = { {S,0,0,0},{0,S,0,0},{0,0,S,0},{0,0,0,1} };
	multiplication1(line, m_scale);
}

void spinX1(float line[N][M], float angle, float k, float n, float m)
{

	float first_rot[M][M] = { {1,0,0,0}, {0,1,0,0},{0,0,1,0},{-k,-n,-m,1} };
	float second_rot[M][M] = { {1,0,0,0}, {0,1,0,0},{0,0,1,0},{k,n,m,1} };
	float m_rotate[M][M] = { {1,0,0,0} ,{0,cos(angle),sin(angle),0},{0,-sin(angle),cos(angle),0},{0,0,0,1} };
	multiplication1(line, first_rot);
	multiplication1(line, m_rotate);
	multiplication1(line, second_rot);

}
void spinY1(float line[N][M], float angle, float k, float n, float m)
{

	float first_rot[M][M] = { {1,0,0,0}, {0,1,0,0},{0,0,1,0},{-k,-n,-m,1} };
	float second_rot[M][M] = { {1,0,0,0}, {0,1,0,0},{0,0,1,0},{k,n,m,1} };
	float m_rotate[M][M] = { {cos(angle),0,sin(angle),0} ,{0,1,0,0},{-sin(angle),0,cos(angle),0},{0,0,0,1} };
	multiplication1(line, first_rot);
	multiplication1(line, m_rotate);
	multiplication1(line, second_rot);

}
void spinZ1(float line[N][M], float angle, float k, float n, float m)
{

	float first_rot[M][M] = { {1,0,0,0}, {0,1,0,0},{0,0,1,0},{-k,-n,-m,1} };
	float second_rot[M][M] = { {1,0,0,0}, {0,1,0,0},{0,0,1,0},{k,n,m,1} };
	float m_rotate[M][M] = { {cos(angle),sin(angle),0,0},{-sin(angle),cos(angle),0,0},{0,0,1,0} ,{0,0,0,1} };
	multiplication1(line, first_rot);
	multiplication1(line, m_rotate);
	multiplication1(line, second_rot);

}

struct Zbuf
{
	float z1 = 0;
	int r = 0;
	int g = 0;
	int b = 0;

};


void zap(Zbuf ZB[A][B])
{
	for (int i = 0; i < A; i++)
		for (int j = 0; j < B; j++)
		{
			ZB[i][j].z1 = 300.0;
			ZB[i][j].r = 40;
			ZB[i][j].g = 40;
			ZB[i][j].b = 40;
		}
}

void vvbuffer(Zbuf ZB[A][B], SDL_Renderer* Render)
{
	for (int i = 0; i < A; i++)
		for (int j = 0; j < B; j++)
		{
			SDL_SetRenderDrawColor(Render, ZB[i][j].r, ZB[i][j].g, ZB[i][j].b, 0);
			SDL_RenderDrawPoint(Render, i, j);
		}
}
void Zs1(Zbuf ZB[A][B], float line[N1][M], int red, int green, int blue)
{
	int ymax, ymin, ysc, e1, e, i;
	int x[4], y[4];
	//Заносим x,y из t в массивы для последующей работы с ними
	for (i = 0; i < 4; i++)
		x[i] = int(line[i][0]), y[i] = int(line[i][1]);
	//Определяем максимальный и минимальный y
	ymax = ymin = y[0];
	if (ymax < y[1]) ymax = y[1]; else if (ymin > y[1]) ymin = y[1];
	if (ymax < y[2]) ymax = y[2]; else if (ymin > y[2]) ymin = y[2];
	if (ymax < y[3]) ymax = y[3]; else if (ymin > y[3]) ymin = y[3];
	ymin = (ymin < 0) ? 0 : ymin;
	ymax = (ymax < B) ? ymax : B;
	int ne;
	int x1, x2, xsc1, xsc2;
	float z1, z2, tc, z;
	//Следующий участок кода перебирает все строки сцены
	//и определяет глубину каждого пикселя
	//для соответствующего треугольника
	for (ysc = ymin; ysc < ymax; ysc++) {
		ne = 0;
		for (int e = 0; e < 4; e++) {
			e1 = e + 1;
			if (e1 == 4) e1 = 0;
			if (y[e] < y[e1]) {
				if (y[e1] <= ysc || ysc < y[e]) continue;
			}
			else if (y[e] > y[e1]) {
				if (y[e1] > ysc || ysc >= y[e]) continue;
			}
			else continue;
			tc = float(y[e] - ysc) / (y[e] - y[e1]);
			if (ne)
			{
				x2 = x[e] + int(tc * (x[e1] - x[e]));
				z2 = line[e][2] + tc * (line[e1][2] - line[e][2]);
			}
			else
			{
				x1 = x[e] + int(tc * (x[e1] - x[e]));
				z1 = line[e][2] + tc * (line[e1][2] - line[e][2]);
				ne = 1;
			}
		}
		if (x2 < x1)
		{
			e = x1; x1 = x2; x2 = e; tc = z1; z1 = z2; z2 = tc;
		}
		xsc1 = (x1 < 0) ? 0 : x1;
		xsc2 = (x2 < A) ? x2 : A;
		for (int xsc = xsc1; xsc < xsc2; xsc++) {
			tc = float(x1 - xsc) / (x1 - x2);
			//printf("tc = %.5f\t", tc);
			z = z1 + tc * (z2 - z1);
			//printf("z = %.5f\t", z);
			//printf("z1 = %.5f\t", z1);
			//printf("z2 = %.5f\t\n", z2);
			//printf("z = %.2f\t", z);

			//Если полученная глубина пиксела меньше той,
			//что находится в Z-Буфере - заменяем храняшуюся на новую.
			if (z <= ZB[xsc][ysc].z1)
			{
				ZB[xsc][ysc].z1 = z;
				ZB[xsc][ysc].r = red;
				ZB[xsc][ysc].g = green;
				ZB[xsc][ysc].b = blue;
			}
		}
	}

}


void multiplication(float line[N][M], float matrix[M][M])
{
	float res[N][M] = { {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} };
	for (int i = 0; i < N1; i++)
	{
		for (int j = 0; j < M; j++)
		{
			for (int k = 0; k < M; k++)
			{
				res[i][j] += (line[i][k] * matrix[k][j]);
			}
		}
	}


	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			line[i][j] = res[i][j];
		}
	}
}



void FillColor(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, SDL_Renderer* Render)
{
	int miny = fminf(y1, fminf(y2, fminf(y3, y4)));
	int maxy = fmaxf(y1, fmaxf(y2, fmaxf(y3, y4)));
	int minx = fminf(x1, fminf(x2, fminf(x3, x4)));
	int maxx = fmaxf(x1, fmaxf(x2, fmaxf(x3, x4)));
	int y;
	int x;
	int a;
	int b;
	int c;
	int d;

	for (int y = miny; y <= maxy; y++)
	{
		for (int x = maxx; x >= minx; x--)
		{
			a = (x1 - x) * (y2 - y1) - (x2 - x1) * (y1 - y);
			b = (x2 - x) * (y3 - y2) - (x3 - x2) * (y2 - y);
			c = (x3 - x) * (y4 - y3) - (x4 - x3) * (y3 - y);
			d = (x4 - x) * (y1 - y4) - (x1 - x4) * (y4 - y);

			if ((a < 0 && b < 0 && c < 0 && d < 0) || (a > 0 && b > 0 && c > 0 && d > 0))
			{
				SDL_RenderDrawPoint(Render, x, y);
			}
		}
		x = maxx;
	}
}

void shadow(float lin[N][M], SDL_Renderer* render)
{

	float win[N][M] = { {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1} };
	int xLight = 50;
	int yLight = -100;
	int zLight = 0;
	int xPl[3];
	int yPl[3];
	int zPl[3];
	xPl[0] = 0; yPl[0] = 180; zPl[0] = -80;
	xPl[1] = 300; yPl[1] = 180; zPl[1] = 20;
	xPl[2] = 150; yPl[2] = 300; zPl[2] = 180;

	int A = (yPl[1] - yPl[0]) * (zPl[2] - zPl[0]) - (yPl[2] - yPl[0]) * (zPl[1] - zPl[0]);
	int B = (zPl[1] - zPl[0]) * (xPl[2] - xPl[0]) - (zPl[2] - zPl[0]) * (xPl[1] - xPl[0]);
	int C = (xPl[1] - xPl[0]) * (yPl[2] - yPl[0]) - (xPl[2] - xPl[0]) * (yPl[1] - yPl[0]);
	int D = -(A * xPl[0] + B * yPl[0] + C * zPl[0]);
	int m = 0;
	int n = 0;
	int p = 0;
	int xSh[8], ySh[8], zSh[8];
	float t = 0;
	for (int i = 0; i < 8; i++)
	{
		m = xLight - lin[i][0];
		n = yLight - lin[i][1];
		p = zLight - lin[i][2];
		t = (float)(-A * lin[i][0] - B * lin[i][1] - C * lin[i][2] - D) / (A * m + B * n + C * p);
		xSh[i] = m * t + lin[i][0];
		ySh[i] = n * t + lin[i][1];
		zSh[i] = p * t + lin[i][2];
		SDL_SetRenderDrawColor(render, 255, 255, 255, 255);



	}
	win[0][0] = xSh[0]; win[0][1] = ySh[0]; win[0][2] = zSh[0];
	win[1][0] = xSh[1]; win[1][1] = ySh[1]; win[1][2] = zSh[1];
	win[2][0] = xSh[2]; win[2][1] = ySh[2]; win[2][2] = zSh[2];
	win[3][0] = xSh[3]; win[3][1] = ySh[3]; win[3][2] = zSh[3];
	win[4][0] = xSh[4]; win[4][1] = ySh[4]; win[4][2] = zSh[4];
	win[5][0] = xSh[5]; win[5][1] = ySh[5]; win[5][2] = zSh[5];
	win[6][0] = xSh[6]; win[6][1] = ySh[6]; win[6][2] = zSh[6];
	win[7][0] = xSh[7]; win[7][1] = ySh[7]; win[7][2] = zSh[7];


	FillColor(win[0][0], win[0][1], win[1][0], win[1][1], win[2][0], win[2][1], win[3][0], win[3][1], render);
	FillColor(win[0][0], win[0][1], win[4][0], win[4][1], win[5][0], win[5][1], win[1][0], win[1][1], render);
	FillColor(win[0][0], win[0][1], win[3][0], win[3][1], win[7][0], win[7][1], win[4][0], win[4][1], render);
	FillColor(win[4][0], win[4][1], win[5][0], win[5][1], win[6][0], win[6][1], win[7][0], win[7][1], render);
	FillColor(win[5][0], win[5][1], win[1][0], win[1][1], win[2][0], win[2][1], win[6][0], win[6][1], render);
	FillColor(win[3][0], win[3][1], win[7][0], win[7][1], win[6][0], win[6][1], win[2][0], win[2][1], render);


}


void movement(float line[N][M], float dx, float dy, float dz)
{
	float m_move[M][M] = { {1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {dx,dy,dz,1} };
	multiplication(line, m_move);
}

void scaling(float line[N][M], float S)
{
	float m_scale[M][M] = { {S,0,0,0},{0,S,0,0},{0,0,S,0},{0,0,0,1} };
	multiplication(line, m_scale);
}

void spinX(float line[N][M], float angle, float k, float n, float m)
{

	float first_rot[M][M] = { {1,0,0,0}, {0,1,0,0},{0,0,1,0},{-k,-n,-m,1} };
	float second_rot[M][M] = { {1,0,0,0}, {0,1,0,0},{0,0,1,0},{k,n,m,1} };
	float m_rotate[M][M] = { {1,0,0,0} ,{0,cos(angle),sin(angle),0},{0,-sin(angle),cos(angle),0},{0,0,0,1} };
	multiplication(line, first_rot);
	multiplication(line, m_rotate);
	multiplication(line, second_rot);

}
void spinY(float line[N][M], float angle, float k, float n, float m)
{

	float first_rot[M][M] = { {1,0,0,0}, {0,1,0,0},{0,0,1,0},{-k,-n,-m,1} };
	float second_rot[M][M] = { {1,0,0,0}, {0,1,0,0},{0,0,1,0},{k,n,m,1} };
	float m_rotate[M][M] = { {cos(angle),0,sin(angle),0} ,{0,1,0,0},{-sin(angle),0,cos(angle),0},{0,0,0,1} };
	multiplication(line, first_rot);
	multiplication(line, m_rotate);
	multiplication(line, second_rot);

}
void spinZ(float line[N][M], float angle, float k, float n, float m)
{

	float first_rot[M][M] = { {1,0,0,0}, {0,1,0,0},{0,0,1,0},{-k,-n,-m,1} };
	float second_rot[M][M] = { {1,0,0,0}, {0,1,0,0},{0,0,1,0},{k,n,m,1} };
	float m_rotate[M][M] = { {cos(angle),sin(angle),0,0},{-sin(angle),cos(angle),0,0},{0,0,1,0} ,{0,0,0,1} };
	multiplication(line, first_rot);
	multiplication(line, m_rotate);
	multiplication(line, second_rot);

}


int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) == 0)
	{
		SDL_Window* Window = NULL;
		SDL_Renderer* Render = NULL;
		if (SDL_CreateWindowAndRenderer(A1, B1, 0, &Window, &Render) == 0)
		{

			SDL_SetRenderDrawColor(Render, 0, 0, 0, 0);
			SDL_RenderClear(Render);

			char c;


			float MM[N1][M] = { {50,50,20,1},{70,50,20,1},{60,80,20,1},{40,80,20,1} };
			float MM1[N1][M] = { {70,50,20,1},{70,50,40,1},{60,80,40,1},{60,80,20,1} };
			float MM2[N1][M] = { {50,50,40,1},{70,50,40,1},{60,80,40,1},{40,80,40,1} };
			float MM3[N1][M] = { {50,50,20,1},{50,50,40,1},{40,80,40,1},{40,80,20,1} };
			float MM4[N1][M] = { {50,50,40,1},{70,50,40,1},{70,50,20,1},{50,50,20,1} };
			float MM5[N1][M] = { {40,80,40,1},{60,80,40,1},{60,80,20,1},{40,80,20,1} };

			float KK[N1][M] = { {53,58,5,1},{63,58,5,1},{63,68,5,1},{53,68,5,1} };
			float KK1[N1][M] = { {63,58,5,1},{63,58,55,1},{63,68,56,1},{63,68,5,1} };
			float KK2[N1][M] = { {53,58,55,1},{63,58,55,1},{63,68,55,1},{53,68,55,1} };
			float KK3[N1][M] = { {53,58,5,1},{53,58,55,1},{53,68,55,1},{53,68,5,1} };
			float KK4[N1][M] = { {53,58,55,1},{63,58,55,1},{63,58,5,1},{53,58,5,1} };
			float KK5[N1][M] = { {53,68,55,1},{63,68,55,1},{63,68,5,1},{53,68,5,1} };


			float PP[N][M] = { {50,50,40,1},{70,50,40,1},{60,80,40,1},{40,80,40,1},{50,50,20,1},{70,50,20,1},{60,80,20,1},{40,80,20,1} };
			float PPP[N][M] = { {53,58,55,1},{63,58,55,1},{63,68,55,1},{53,68,55,1},{53,58,5,1},{63,58,5,1},{63,68,5,1},{53,68,5,1} };


			system("cls");
			cout << "\n\n\tWASD - movementXY\n\tCV - movmentOZ\n\tQE - spin OX\n\tRT - spin OY\n\tFG - spin OZ\n\n";
			cout << "\tZ - increase\n\tX - decrease\n\n";
			cout << "\nExit - 0\n";

			int exit = 1;
			Zbuf ZBB[A][B];
			while (exit == 1)
			{
				shadow(PP, Render);
				shadow(PPP, Render);
				zap(ZBB);
				vvbuffer(ZBB, Render);


				Zs1(ZBB, MM, 255, 255, 0);
				Zs1(ZBB, MM1, 255, 220, 0);
				Zs1(ZBB, MM2, 255, 190, 0);
				Zs1(ZBB, MM3, 255, 155, 0);
				Zs1(ZBB, MM4, 255, 125, 0);
				Zs1(ZBB, MM5, 255, 100, 0);

				Zs1(ZBB, KK, 150, 200, 0);
				Zs1(ZBB, KK1, 155, 10, 40);
				Zs1(ZBB, KK2, 10, 200, 80);
				Zs1(ZBB, KK3, 60, 150, 120);
				Zs1(ZBB, KK4, 0, 80, 160);
				Zs1(ZBB, KK5, 50, 200, 200);

				vvbuffer(ZBB, Render);
				//Draw_figure1(PP, Render);
				//printf("z = %.2f\t", PP[0][2]);
				//Draw_figure1(PP, Render);
				SDL_RenderPresent(Render);

				c = _getch();
				switch (c)
				{
				case 'w': //w - вверх
					movement(MM, 0, -DY, 0);
					movement(MM1, 0, -DY, 0);
					movement(MM2, 0, -DY, 0);
					movement(MM3, 0, -DY, 0);
					movement(MM4, 0, -DY, 0);
					movement(MM5, 0, -DY, 0);

					movement(KK, 0, -DY, 0);
					movement(KK1, 0, -DY, 0);
					movement(KK2, 0, -DY, 0);
					movement(KK3, 0, -DY, 0);
					movement(KK4, 0, -DY, 0);
					movement(KK5, 0, -DY, 0);


					movement1(PP, 0, -DY, 0);
					movement1(PPP, 0, -DY, 0);
					YY = YY - DY;
					YY1 = YY1 - DY;
					break;

				case 'a': //a - влево

					movement(MM, -DX, 0, 0);
					movement(MM1, -DX, 0, 0);
					movement(MM2, -DX, 0, 0);
					movement(MM3, -DX, 0, 0);
					movement(MM4, -DX, 0, 0);
					movement(MM5, -DX, 0, 0);

					movement(KK, -DX, 0, 0);
					movement(KK1, -DX, 0, 0);
					movement(KK2, -DX, 0, 0);
					movement(KK3, -DX, 0, 0);
					movement(KK4, -DX, 0, 0);
					movement(KK5, -DX, 0, 0);


					movement1(PP, -DX, 0, 0);
					movement1(PPP, -DX, 0, 0);
					XX = XX - DX;
					XX1 = XX1 - DX;
					break;

				case 's': //s - вниз

					movement(MM, 0, DY, 0);
					movement(MM1, 0, DY, 0);
					movement(MM2, 0, DY, 0);
					movement(MM3, 0, DY, 0);
					movement(MM4, 0, DY, 0);
					movement(MM5, 0, DY, 0);

					movement(KK, 0, DY, 0);
					movement(KK1, 0, DY, 0);
					movement(KK2, 0, DY, 0);
					movement(KK3, 0, DY, 0);
					movement(KK4, 0, DY, 0);
					movement(KK5, 0, DY, 0);


					movement1(PP, 0, DY, 0);
					movement1(PPP, 0, DY, 0);
					YY = YY + DY;
					YY1 = YY1 + DY;
					break;

				case 'd': //d - вправо

					movement(MM, DX, 0, 0);
					movement(MM1, DX, 0, 0);
					movement(MM2, DX, 0, 0);
					movement(MM3, DX, 0, 0);
					movement(MM4, DX, 0, 0);
					movement(MM5, DX, 0, 0);

					movement(KK, DX, 0, 0);
					movement(KK1, DX, 0, 0);
					movement(KK2, DX, 0, 0);
					movement(KK3, DX, 0, 0);
					movement(KK4, DX, 0, 0);
					movement(KK5, DX, 0, 0);


					movement1(PP, DX, 0, 0);
					movement1(PPP, DX, 0, 0);
					XX = XX + DX;
					XX1 = XX1 + DX;
					break;

				case 'c': //с - на
					movement(MM, 0, 0, DZ);
					movement(MM1, 0, 0, DZ);
					movement(MM2, 0, 0, DZ);
					movement(MM3, 0, 0, DZ);
					movement(MM4, 0, 0, DZ);
					movement(MM5, 0, 0, DZ);

					movement(KK, 0, 0, DZ);
					movement(KK1, 0, 0, DZ);
					movement(KK2, 0, 0, DZ);
					movement(KK3, 0, 0, DZ);
					movement(KK4, 0, 0, DZ);
					movement(KK5, 0, 0, DZ);


					movement1(PP, 0, 0, DZ);
					movement1(PPP, 0, 0, DZ);
					ZZ = ZZ + DZ;
					ZZ1 = ZZ1 + DZ;
					break;

				case 'v': //v - отнас
					movement(MM, 0, 0, -DZ);
					movement(MM1, 0, 0, -DZ);
					movement(MM2, 0, 0, -DZ);
					movement(MM3, 0, 0, -DZ);
					movement(MM4, 0, 0, -DZ);
					movement(MM5, 0, 0, -DZ);

					movement(KK, 0, 0, -DZ);
					movement(KK1, 0, 0, -DZ);
					movement(KK2, 0, 0, -DZ);
					movement(KK3, 0, 0, -DZ);
					movement(KK4, 0, 0, -DZ);
					movement(KK5, 0, 0, -DZ);


					movement1(PP, 0, 0, -DZ);
					movement1(PPP, 0, 0, -DZ);
					ZZ = ZZ - DZ;
					ZZ1 = ZZ1 - DZ;
					break;

				case 'q': //q - поврот
					spinX(MM, ALPHA, XX, YY, ZZ);
					spinX(MM1, ALPHA, XX, YY, ZZ);
					spinX(MM2, ALPHA, XX, YY, ZZ);
					spinX(MM3, ALPHA, XX, YY, ZZ);
					spinX(MM4, ALPHA, XX, YY, ZZ);
					spinX(MM5, ALPHA, XX, YY, ZZ);

					spinX(KK, ALPHA, XX, YY, ZZ);
					spinX(KK1, ALPHA, XX, YY, ZZ);
					spinX(KK2, ALPHA, XX, YY, ZZ);
					spinX(KK3, ALPHA, XX, YY, ZZ);
					spinX(KK4, ALPHA, XX, YY, ZZ);
					spinX(KK5, ALPHA, XX, YY, ZZ);


					spinX1(PP, ALPHA, XX1, YY1, ZZ1);
					spinX1(PPP, ALPHA, XX1, YY1, ZZ1);
					break;

				case 'e': //e - поворот
					spinX(MM, -ALPHA, XX, YY, ZZ);
					spinX(MM1, -ALPHA, XX, YY, ZZ);
					spinX(MM2, -ALPHA, XX, YY, ZZ);
					spinX(MM3, -ALPHA, XX, YY, ZZ);
					spinX(MM4, -ALPHA, XX, YY, ZZ);
					spinX(MM5, -ALPHA, XX, YY, ZZ);

					spinX(KK, -ALPHA, XX, YY, ZZ);
					spinX(KK1, -ALPHA, XX, YY, ZZ);
					spinX(KK2, -ALPHA, XX, YY, ZZ);
					spinX(KK3, -ALPHA, XX, YY, ZZ);
					spinX(KK4, -ALPHA, XX, YY, ZZ);
					spinX(KK5, -ALPHA, XX, YY, ZZ);


					spinX1(PP, -ALPHA, XX1, YY1, ZZ1);
					spinX1(PPP, -ALPHA, XX1, YY1, ZZ1);
					break;

				case 'r': //r - поврот
					spinY(MM, ALPHA, XX, YY, ZZ);
					spinY(MM1, ALPHA, XX, YY, ZZ);
					spinY(MM2, ALPHA, XX, YY, ZZ);
					spinY(MM3, ALPHA, XX, YY, ZZ);
					spinY(MM4, ALPHA, XX, YY, ZZ);
					spinY(MM5, ALPHA, XX, YY, ZZ);

					spinY(KK, ALPHA, XX, YY, ZZ);
					spinY(KK1, ALPHA, XX, YY, ZZ);
					spinY(KK2, ALPHA, XX, YY, ZZ);
					spinY(KK3, ALPHA, XX, YY, ZZ);
					spinY(KK4, ALPHA, XX, YY, ZZ);
					spinY(KK5, ALPHA, XX, YY, ZZ);


					spinY1(PP, ALPHA, XX1, YY1, ZZ1);
					spinY1(PPP, ALPHA, XX1, YY1, ZZ1);
					break;

				case 't': //t - поворот
					spinY(MM, -ALPHA, XX, YY, ZZ);
					spinY(MM1, -ALPHA, XX, YY, ZZ);
					spinY(MM2, -ALPHA, XX, YY, ZZ);
					spinY(MM3, -ALPHA, XX, YY, ZZ);
					spinY(MM4, -ALPHA, XX, YY, ZZ);
					spinY(MM5, -ALPHA, XX, YY, ZZ);

					spinY(KK, -ALPHA, XX, YY, ZZ);
					spinY(KK1, -ALPHA, XX, YY, ZZ);
					spinY(KK2, -ALPHA, XX, YY, ZZ);
					spinY(KK3, -ALPHA, XX, YY, ZZ);
					spinY(KK4, -ALPHA, XX, YY, ZZ);
					spinY(KK5, -ALPHA, XX, YY, ZZ);


					spinY1(PP, -ALPHA, XX1, YY1, ZZ1);
					spinY1(PPP, -ALPHA, XX1, YY1, ZZ1);
					break;

				case 'f': //q - поврот
					spinZ(MM, ALPHA, XX, YY, ZZ);
					spinZ(MM1, ALPHA, XX, YY, ZZ);
					spinZ(MM2, ALPHA, XX, YY, ZZ);
					spinZ(MM3, ALPHA, XX, YY, ZZ);
					spinZ(MM4, ALPHA, XX, YY, ZZ);
					spinZ(MM5, ALPHA, XX, YY, ZZ);

					spinZ(KK, ALPHA, XX, YY, ZZ);
					spinZ(KK1, ALPHA, XX, YY, ZZ);
					spinZ(KK2, ALPHA, XX, YY, ZZ);
					spinZ(KK3, ALPHA, XX, YY, ZZ);
					spinZ(KK4, ALPHA, XX, YY, ZZ);
					spinZ(KK5, ALPHA, XX, YY, ZZ);


					spinZ1(PP, ALPHA, XX1, YY1, ZZ1);
					spinZ1(PPP, ALPHA, XX1, YY1, ZZ1);
					break;

				case 'g': //e - поворот
					spinZ(MM, -ALPHA, XX, YY, ZZ);
					spinZ(MM1, -ALPHA, XX, YY, ZZ);
					spinZ(MM2, -ALPHA, XX, YY, ZZ);
					spinZ(MM3, -ALPHA, XX, YY, ZZ);
					spinZ(MM4, -ALPHA, XX, YY, ZZ);
					spinZ(MM5, -ALPHA, XX, YY, ZZ);

					spinZ(KK, -ALPHA, XX, YY, ZZ);
					spinZ(KK1, -ALPHA, XX, YY, ZZ);
					spinZ(KK2, -ALPHA, XX, YY, ZZ);
					spinZ(KK3, -ALPHA, XX, YY, ZZ);
					spinZ(KK4, -ALPHA, XX, YY, ZZ);
					spinZ(KK5, -ALPHA, XX, YY, ZZ);


					spinZ1(PP, -ALPHA, XX1, YY1, ZZ1);
					spinZ1(PPP, -ALPHA, XX1, YY1, ZZ1);
					break;


				case 'z': //увеличение
					scaling(MM, S1);
					scaling(MM1, S1);
					scaling(MM2, S1);
					scaling(MM3, S1);
					scaling(MM4, S1);
					scaling(MM5, S1);

					scaling(KK, S1);
					scaling(KK1, S1);
					scaling(KK2, S1);
					scaling(KK3, S1);
					scaling(KK4, S1);
					scaling(KK5, S1);


					scaling1(PP, S1);
					scaling1(PPP, S1);
					XX = XX * S1;
					YY = YY * S1;
					ZZ = ZZ * S1;

					XX1 = XX1 * S1;
					YY1 = YY1 * S1;
					ZZ1 = ZZ1 * S1;
					break;

				case 'x': //уменьшение
					scaling(MM, S2);
					scaling(MM1, S2);
					scaling(MM2, S2);
					scaling(MM3, S2);
					scaling(MM4, S2);
					scaling(MM5, S2);

					scaling(KK, S2);
					scaling(KK1, S2);
					scaling(KK2, S2);
					scaling(KK3, S2);
					scaling(KK4, S2);
					scaling(KK5, S2);


					scaling1(PP, S2);
					scaling1(PPP, S2);
					XX = XX * S2;
					YY = YY * S2;
					ZZ = ZZ * S2;

					XX1 = XX1 * S2;
					YY1 = YY1 * S2;
					ZZ1 = ZZ1 * S2;
					break;

				case '0':
					exit = 0;
					break;
				}

				SDL_SetRenderDrawColor(Render, 0, 0, 0, 0);
				SDL_RenderClear(Render);
				//Draw_figure1(PP, Render);
				shadow(PP, Render);
				shadow(PPP, Render);
				zap(ZBB);
				vvbuffer(ZBB, Render);


				Zs1(ZBB, MM, 255, 255, 0);
				Zs1(ZBB, MM1, 255, 220, 0);
				Zs1(ZBB, MM2, 255, 190, 0);
				Zs1(ZBB, MM3, 255, 155, 0);
				Zs1(ZBB, MM4, 255, 125, 0);
				Zs1(ZBB, MM5, 255, 100, 0);

				Zs1(ZBB, KK, 150, 200, 0);
				Zs1(ZBB, KK1, 155, 10, 40);
				Zs1(ZBB, KK2, 10, 200, 80);
				Zs1(ZBB, KK3, 60, 150, 120);
				Zs1(ZBB, KK4, 0, 80, 160);
				Zs1(ZBB, KK5, 50, 200, 200);
				vvbuffer(ZBB, Render);

				SDL_RenderPresent(Render);

			}
		}
	}
	SDL_Quit();
	return 0;
}
