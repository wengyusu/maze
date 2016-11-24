
#include <iostream>
#include <time.h>
#include <vector>
#include <graphics.h>
#include <conio.h>
#define random(x) (rand()%x)

using namespace std;

const int movex[4] = { -1, 0, 1, 0 };
const int movey[4] = { 0, 1, 0, -1 };

struct node {
	int step, fa, x, y;
};

//char wall = 22;

enum Status {
	success = 0,
	fail = 1
};
int n, m;
int start_x, start_y, end_x, end_y;
int **maze, **used;

node *mazedata;
int m_maze[9][12] =
{
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1 },
	{ 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1 },
	{ 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1 },
	{ 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1 },
	{ 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 },
	{ 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1 },
	{ 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};

void my_maze()
{
	maze = new int*[n + 2];
	used = new int*[n + 2];
	for (int i = 0; i <= n + 1; i++)
	{
		maze[i] = new int[m + 2];
		used[i] = new int[m + 2];
	}

	for (int i = 0; i <= n + 1; i++)
		for (int j = 0; j <= m + 1; j++)
			used[i][j] = 0;

	for (int i = 0; i <= n + 1; i++)
		for (int j = 0; j <= m + 1; j++)
			maze[i][j] = m_maze[i][j];
}
Status init()
{
	Status status = success;
	cout << "请输入迷宫大小 N * M, （输入 0 0 使用默认迷宫）" << endl;
	cin >> n >> m;
	if (n == 0 && m == 0)
	{
		
		end_x = 7; end_y = 10;
		n = 7;
		m = 10;
		my_maze();
		for (int i = 1; i <= n; i++)
		{
			for (int j = 1; j <= m; j++)
				cout << maze[i][j] << " ";
			cout << endl;
		}
	}
	else
	{

	}
	return status;
}

void ans_print(node ans, vector <vector <int>> &maze)
{
	if (ans.x != start_x || ans.y != start_y)
		ans_print(mazedata[ans.fa],maze);
	//printf("(%d,%d) ", ans.x, ans.y);
	maze[ans.x][ans.y] = 2;
}
Status bfs(int n,int m,vector <vector <int>> &maze)
{
	start_x = start_y = 1;
	end_x = n; end_y = m;
	used = new int*[n + 2];
	for (int i = 0; i <= n + 1; i++)
	{
		used[i] = new int[m + 2];
	}

	for (int i = 0; i <= n + 1; i++)
		for (int j = 0; j <= m + 1; j++)
			used[i][j] = 0;

	Status status = success;
	mazedata = new node[(n + 2)*(m + 2)];
	int head = 1, end = 1;
	mazedata[head] = { 1, 0 , start_x, start_y
	};
	while (head <= end)
	{
		for (int i = 0; i < 4; i++)
		{
			int nx = mazedata[head].x + movex[i];
			int ny = mazedata[head].y + movey[i];
			if (maze[nx][ny] == 0 && used[nx][ny] == 0)
			{
				used[nx][ny] = 1;
				mazedata[++end] = { mazedata[head].step + 1, head, nx, ny };
				if (nx == end_x && ny == end_y)
				{
					//printf("从(%d,%d),到(%d,%d)的最短路径长度为:%d\n", start_x, start_y, end_x, end_y, mazedata[end].step);
					ans_print(mazedata[end],maze);
					return status;
				}
			}
		}
		head++;
	}
	status = fail;
	return status;
}

void free_pointer()
{
	//for (int i = 0; i <= n + 1; i++)
	//{
	//	delete[] maze[i];
	//	delete[] used[i];
	//}
	//delete[] maze;
	delete[] used;
	delete[] mazedata;

}
int wallinfo[101][101] = {0}; //0代表有墙，1代表没有
vector <vector <int> > cmaze(21, vector <int>(21, 1));
int pre[101];

int find(int x) {
	int r = x,s=x,t=x;

	while (pre[r] != r)
		r = pre[r];
	while (pre[s] != r) {
		t = s;
		pre[s] = r;
		s = pre[t];
	}
	return r;
}

bool isnotconnect() {
	bool status =false;
	int flag = find(1);
	int i;
	for (i = 2; i <= 100; i++) {
		if (flag != find(i))
			return true;
	}
	return status;
}
void join(int x, int y) {
	int fx = find(x), fy = find(y);
	if (fx != fy) {
		pre[fx] = fy;
		wallinfo[x][y] = 1;
	}
	
}

void createmaze() {
	int i,j;
	for (i = 0; i <= 100; i++)
		pre[i] = i;
	int rannum, direction, target=-1;

	while (isnotconnect()) {
		rannum = random(99) + 2;
		direction = random(4);
		if (direction == 0) {   //left
			if (rannum % 10 != 1) {
				target = rannum - 1;
				join(target, rannum);
			
			}
		}
		else if (direction == 1) { //right
			if (rannum % 10 != 0) {
				target = rannum + 1;
				join(rannum, target);
			}
		}
		else if (direction == 2) {  //up
			if (rannum > 10) {
				target = rannum - 10;
				join(target, rannum);
		}
		}
		else { //down
			if (rannum < 91) {
				target = rannum + 10;
				join(rannum, target);
			}
		}
			
	}
		int start = find(1), end = find(100),pos;
		for (i = 0; i < 10; i++) {
			for (j = 1; j <= 10; j++) {
				pos = 10*i+j;
				if (find(pos) == start || find(pos) == end)
					cmaze[2*i+1][2*j-1] =0;
				else
					cmaze[2 * i + 1][2 * j - 1] = 1; //cout << "■";
				if(pos%10!=0){
				if (wallinfo[pos][pos+1]==0) {
					cmaze[2 * i + 1][2*j] = 1;//cout << "■";
				}
				else {
					cmaze[2 * i + 1][2 * j] = 0;//cout << " " << " ";
				}
				}
			}
			cout << endl;
			for (j = 1; j <= 10; j++) {
				pos = i * 10 + j;
				if (pos < 91) {
					if (wallinfo[pos][pos+10]==0) {
						cmaze[2*i+2][2*j-1] = 1; //cout << "■" ;
					}
					else {
						cmaze[2 * i + 2][2 * j - 1] = 0; //cout << " " << " ";
					}
				}
			}
			cout << endl;
		}
}

void output() {
	initgraph(640, 480);
	
	setbkcolor(BLACK);
	cleardevice();
	int i, j;
	for (i = 0; i < 21; i++) {
		for (j = 0; j <21; j++) {
			switch (cmaze[i][j]) {
			case 0: {
				//setfillcolor(BLACK);
				//solidrectangle(20 * j, 20 * i, 20 * (j + 1), 20 * (i + 1));
				break;
			}
			case 2: {
				setfillcolor(RED);
				solidcircle(20*j+10,20*i+10,8);
				break;
			}
			default: {
				setfillcolor(BLUE);
				solidrectangle(20*j, 20*i, 20*(j+1), 20*(i+1));
			}
			}
		}
	}
	_getch();
	closegraph();
}
int main() {
	srand((int)time(0));
	//if (init()) { free_pointer(); return 0; }
	//if (bfs())
	//	cout << "没有找到可通过路径" << endl;
	//free_pointer();
	//output();
	createmaze();
	cmaze[0][1] = cmaze[19][20]= 0;
	output();
	cmaze[0][1] = cmaze[19][20]= 1;
	bfs(19, 19, cmaze);
	cmaze[0][1] = cmaze[19][20]= 2;

	output();
	free_pointer();

	return 0;
}
