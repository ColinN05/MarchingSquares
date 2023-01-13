#include <windows.h>
#include <GL/glew.h>
#include <iostream>
#include "Window.h"
#include <vector>
#include <cmath>

bool Window::Destroyed;

struct Point
{
	float x,y;
};

struct Edge
{
	Point p1, p2;
};

void DrawEdge(Edge& e)
{
	glBegin(GL_LINES);
	glVertex2f(e.p1.x,e.p1.y);
	glVertex2f(e.p2.x,e.p2.y);
	glEnd();
}

float func(float x, float y)
{
	return x*x - y * y - 0.1f;
}

int main()
{
	Window win = Window();

	auto glewInitResult = glewInit();
	if (glewInitResult != GLEW_OK)
	{
		std::cout << "GLEW failed to initialize!" << glewGetErrorString(glewInitResult) << '\n';
		return -1;
	}

	glClearColor(0.1,0.1,0.1,1);


	float x0 = 0.0f;
	float y0 = 0.0f;
	float r = 1.0f;
	
	while (!Window::Destroyed)
	{

	if (GetAsyncKeyState(0x57)) // W
	{
		y0 += 0.03f * r;
	}
	if (GetAsyncKeyState(0x41)) // A
	{
		x0 -= 0.03f * r;
	}
	if (GetAsyncKeyState(0x53)) // S
	{
		y0 -= 0.03f * r;
	}
	if (GetAsyncKeyState(0x44)) // D
	{
		x0 += 0.03f * r;
	}	
	if (GetAsyncKeyState(VK_UP))
	{
		r /= 1.03f;
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		r *= 1.03f;
	}
		///////////////////////

	float isoval = 0.0f;

	const float stepX = 0.01f;
	const float stepY = 0.01f;
	const int n = 2/stepX;
	const int m = 2/stepY;
	bool A[(int)(2/stepX)][(int)(2/stepY)] = {};
	Point coords[(int)(2/stepX)][(int)(2/stepY)] = {};
	
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			float x = i * stepX * r - (-x0+r);
			float y = j * stepY * r - (-y0+r);
			float val = func(x,y);

			if (val < isoval)
			{
				A[i][j]=1;
			}
			else
			{
				A[i][j]=0;
			}

			
			Point p = {x,y};
			coords[i][j] = p;
		}
	}

	std::vector<Edge> countour_edges = {};
	
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < m - 1; j++)
		{
			bool a = A[i][j];
			bool b = A[i+1][j];
			bool c = A[i+1][j+1];
			bool d = A[i][j+1];
			int k = 8 * a + 4 * b + 2 * c + 1 * d;

			Point p3 = {((coords[i+1][j].x-coords[i][j].x)/2+coords[i][j].x-x0)/r,((coords[i+1][j].y-coords[i][j].y)/2+coords[i][j].y-y0)/r};
			Point p2 = {((coords[i+1][j+1].x-coords[i+1][j].x)/2+coords[i+1][j].x-x0)/r,((coords[i+1][j+1].y-coords[i+1][j].y)/2+coords[i+1][j].y-y0)/r};
			Point p1 = {((coords[i][j+1].x-coords[i+1][j+1].x)/2+coords[i+1][j+1].x-x0)/r,((coords[i][j+1].y-coords[i+1][j+1].y)/2+coords[i+1][j+1].y-y0)/r};
			Point p4 = {((coords[i][j].x-coords[i][j+1].x)/2+coords[i][j+1].x-x0)/r,((coords[i][j].y-coords[i][j+1].y)/2+coords[i][j+1].y-y0)/r};

			Edge e = {{0.0f,0.0f},{0.0f,0.0f}};
			
			switch (k)
			{
				case 0b0000:{break;}//nothing
				case 0b0001:
				{
					e = {p1,p4};
					break;
				}
				case 0b0010:
				{
					e = {p1,p2};
					break;
				}
				case 0b0011:
				{
					e = {p2,p4};
					break;
				}
				case 0b0100:
				{
					e = {p2,p3};
					break;
				}
				case 0b0101:
				{
					Edge f = {p1,p4};
					countour_edges.push_back(f);
					e = {p2,p3};
					break;
				}
				case 0b0110:
				{
					e = {p1,p3};
					break;
				}
				case 0b0111:
				{
					e = {p3, p4};
					break;
				}
				case 0b1000:
				{
					e = {p3,p4};
					break;
				}
				case 0b1001:
				{
					e = {p1,p3};
					break;
				}
				case 0b1010:
				{
					Edge f = {p3,p4};
					countour_edges.push_back(f);
					e = {p1,p2};
					break;
				}
				case 0b1011:
				{
					e = {p2,p3};
					break;
				}
				case 0b1100:
				{
					e = {p2,p4};
					break;
				}
				case 0b1101:
				{	
					e = {p1,p2};
					break;
				}
				case 0b1110:
				{
					e = {p1,p4};
					break;
				}
				case 0b1111:
				{
					//nothing
					break;
				}
			}
 			countour_edges.push_back(e);
			
		}
	}

///////////////////////

		win.Update();

		glClear(GL_COLOR_BUFFER_BIT);

		for (Edge& e : countour_edges)
		{
			DrawEdge(e);
		}

		win.SwapBuffers();
	}
}