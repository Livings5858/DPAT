#pragma comment(lib,"msimg32.lib")
#include<iostream>
#include<windows.h>
#include"resource.h"
using namespace std;
#include"tchar.h"
#define MAX(a,b)				(a>b?a:b)
#define IDC_QUIT				251
#define IDC_PROTEIN				252
#define IDC_CTREE				253
#define IDC_DNA					254
#define IDC_BLOSUM62			255
#define IDC_PAM250				256
#define WndClassName1 _T("YIQUAN_DTGHF_W_1")
#define WndClassName2 _T("YIQUAN_DTGHF_W_2")
#define WndClassName3 _T("YIQUAN_DTGHF_W_3")
#define WndClassName4 _T("YIQUAN_DTGHF_W_4")
#define WndClassName5 _T("YIQUAN_DTGHF_W_5")
#define WndClassName6 _T("YIQUAN_DTGHF_W_6")
#define WndClassName7 _T("YIQUAN_DTGHF_W_7")
#define wWidth0		512
#define wHeight0	350
#define wWidth1		700
#define wHeight1	500
#define wWidth2		700
#define wHeight2	500
#define wWidth4		820
#define wHeight4	460
#define wWidth5		868
#define wHeight5	550
#define wWidth6		800
#define wHeight6	600
#define wWidth7		700
#define wHeight7	500

#define num_box		6
//--------------------------------------------------------------------
int word_x=20;
int word_y=20;
int Edit_x1=300;									//edit 左侧坐标
int Edit_y1=50;										//edit 上侧坐标
int Edit_x2;										//edit 右侧坐标
int Edit_y2;										//edit 下侧坐标
int Box_Length=25;									//方格边长
int Box_Line=num_box;								//方格行数
int box_x=0;										//矩阵大小
int the_same=3;										//匹配
int the_different=-1;								//错配
int the_empty=-2;									//空位
int the_same_p=3;									//匹配
int the_empty_p=-11;								//空位
int x;												//长度
int y;												//高度
char list_x[40]="GCATGCATATATATAATA";							//第一序列
char list_y[40]="GCATATATATATATGCAT";							//第二序列
char list_1[512];
char list_2[512];
int lenx=strlen(list_x);							//第一组序列长度
int leny=strlen(list_y);							//第二组序列长度
int  cx   =  GetSystemMetrics( SM_CXFULLSCREEN );	//屏幕长
int  cy   =  GetSystemMetrics( SM_CYFULLSCREEN );	//屏幕高（不包括任务栏）

char box_head_buff[100][4];							//建树矩阵的标题
char box_buff[98][98][4];							//建树矩阵的内容（字符串）
float box_num[98][98];								//建树矩阵内容（数字）
struct box_many
{
	float box_fnum[98][98];							//建树矩阵（过程）
	char  box_head[98][98];							//矩阵标题
	int len;										//矩阵大小
	float num;										//合并数字
}box_Many[98];
int i_treebox=0;
HWND hWnd1,hWnd2,hWnd3,hWnd4,hWnd5,hWnd6,hWnd7;
HWND hWrite1;
HWND hWrite2;
HWND hWrite3;
HWND hWrite4;
HWND hWrite5;
HWND hWrite[100][100];
HWND hBtn1,hBtn2;
HMENU hMenu,hMenu1;
HINSTANCE hInstance_1,happ;
float MT=80;										//放大倍数
int fl=1;											//是否显示二维码的flag
struct Tload										//存储树中间过程的位置和大小
{
	float a;											//上边长
	float b;											//中间长
	float c;											//下边长
	float num_a;									//上值
	float num_c;									//下值
	int w;											//宽度
};
Tload tload[98];
int i_tb=0;											//建树位值
/*链表*/
struct NODE
{
	int sum;
	NODE*last1;//->x横向
	NODE*last2;//->y纵向
	NODE*last3;//->xy斜向
};

NODE a[20][20];										//节点
char stand[3][4];									//输入存储数组
struct made//回溯坐标
{
	int x;
	int y;
};
made made_me[20][40];								//回溯序列终版
char made_meb1[20][20];								//回溯序列组第一条链
char made_meb2[20][20];								//回溯序列组第二条链
made me[20][40];									//回溯序列临时
int made_i=0;										//回溯序列个数

int x_t1=0,x_t2=0,x_t3=0,y_t1=0,y_t2=0,y_t3=0;
HBITMAP P_W4_1,P_W4_2,P_W4_3;
HBITMAP P_W1_1,P_W1_2,P_W1_3;
HBITMAP P_W7_1,P_W7_2,P_W7_3;
HBITMAP P_W5_1,P_W5_2,P_W5_3;
//------------------------------------------------------------------
LRESULT CALLBACK WinProc1(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
LRESULT CALLBACK WinProc2(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
LRESULT CALLBACK WinProc3(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
LRESULT CALLBACK WinProc4(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
LRESULT CALLBACK WinProc5(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
LRESULT CALLBACK WinProc6(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
LRESULT CALLBACK WinProc7(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
//------------------------------------------------------------------
void line(HDC hdc,int x1,int y1,int x2,int y2);					//画线封装声明
DWORD WINAPI fun_time(LPVOID lpParameter);						//缓冲窗口计时线程声明
int Dbox(float box_nums[98][98],int len);						//矩阵处理函数声明
char * float2str(float val, int precision, char *buf);			//float to str
int buildtree(HDC mdc,int rat,int ray,int i);					//非加权组平均法建树
int Init_btree();												//非加权组平均法建树初始化
int TreebuildProcess(HDC hdc,int rax,int ray);					//非加权组平均法建树过程
//------------------------------------------------------------------
//清空结构体
void ClearStruct(unsigned char *pta,int size)
{
	while(size>0)
	{
		*pta++=0;
		size--;
	}
}
/*DNA比对*/					int check()
{
	//NODE a[20][20];
	for(int i=0;i<20;i++)
		for(int j=0;j<20;j++)
			ClearStruct((unsigned char*)&a[i][j],sizeof(NODE));
	a[0][0].last1=a[0][0].last2=a[0][0].last3=NULL;
	a[0][0].sum=0;
	for(int i=1;i<=lenx;i++)
	{
		a[0][i].sum=a[0][i-1].sum+the_empty;
		a[0][i].last1=&a[0][i-1];
		a[0][i].last2=a[0][i].last3=NULL;
	}
	for(int i=1;i<=leny;i++)
	{
		a[i][0].sum=a[i-1][0].sum+the_empty;
		a[i][0].last2=&a[i-1][0];
		a[i][0].last1=a[i][0].last3=NULL;

	}
	for(int i=1;i<=leny;i++)
		for(int j=1;j<=lenx;j++)
		{
			if(list_x[j-1]==list_y[i-1])
			{
				if(((a[i-1][j-1].sum+the_same)>(a[i-1][j].sum+the_empty))&&((a[i-1][j-1].sum+the_same)>(a[i][j-1].sum+the_empty)))
				{
					a[i][j].sum=a[i-1][j-1].sum+the_same;
					a[i][j].last1=a[i][j].last2=a[i][j].last3=NULL;
					a[i][j].last3=&a[i-1][j-1];
				}
				else if(((a[i-1][j-1].sum+the_same)<(a[i-1][j].sum+the_empty))&&((a[i-1][j].sum+the_empty)>(a[i][j-1].sum+the_empty)))
				{
					a[i][j].sum=a[i-1][j].sum+the_empty;
					a[i][j].last1=a[i][j].last2=a[i][j].last3=NULL;
					a[i][j].last2=&a[i-1][j];
				}
				else if(((a[i][j-1].sum+the_empty)>(a[i-1][j].sum+the_empty))&&((a[i-1][j-1].sum+the_same)<(a[i][j-1].sum+the_empty)))
				{
					a[i][j].sum=a[i][j-1].sum+the_empty;
					a[i][j].last1=a[i][j].last2=a[i][j].last3=NULL;
					a[i][j].last1=&a[i][j-1];
				}
				//(a[i-1][j-1].sum+the_same)==(a[i][j-1].sum+the_empty)
				else if(((a[i-1][j-1].sum+the_same)>(a[i-1][j].sum+the_empty))&&((a[i-1][j-1].sum+the_same)==(a[i][j-1].sum+the_empty)))
				{
					a[i][j].sum=a[i][j-1].sum+the_empty;
					a[i][j].last1=a[i][j].last2=a[i][j].last3=NULL;
					a[i][j].last1=&a[i][j-1];
					a[i][j].last3=&a[i-1][j-1];
				}
				//(a[i-1][j-1].sum+the_same)==(a[i-1][j].sum+the_empty)
				else if(((a[i-1][j-1].sum+the_same)==(a[i-1][j].sum+the_empty))&&((a[i-1][j-1].sum+the_same)>(a[i][j-1].sum+the_empty)))
				{
					a[i][j].sum=a[i-1][j-1].sum+the_same;
					a[i][j].last1=a[i][j].last2=a[i][j].last3=NULL;
					a[i][j].last2=&a[i-1][j];
					a[i][j].last3=&a[i-1][j-1];
				}
				//(a[i-1][j].sum+the_empty)==(a[i][j-1].sum+the_empty)
				else if(((a[i-1][j-1].sum+the_same)<(a[i-1][j].sum+the_empty))&&((a[i-1][j].sum+the_empty)==(a[i][j-1].sum+the_empty)))
				{
					a[i][j].sum=a[i][j-1].sum+the_empty;
					a[i][j].last1=a[i][j].last2=a[i][j].last3=NULL;
					a[i][j].last1=&a[i][j-1];
					a[i][j].last2=&a[i-1][j];
				}
				//全等
				else if(((a[i-1][j-1].sum+the_same)==(a[i-1][j].sum+the_empty))&&((a[i-1][j-1].sum+the_same)==(a[i][j-1].sum+the_empty)))
				{
					a[i][j].sum=a[i][j-1].sum+the_empty;
					a[i][j].last1=&a[i][j-1];
					a[i][j].last3=&a[i-1][j-1];
					a[i][j].last2=&a[i-1][j];
				}
			}
			else
			{
				if(((a[i-1][j-1].sum+the_different)>(a[i-1][j].sum+the_empty))&&((a[i-1][j-1].sum+the_different)>(a[i][j-1].sum+the_empty)))
				{
					a[i][j].sum=a[i-1][j-1].sum+the_different;
					a[i][j].last1=a[i][j].last2=a[i][j].last3=NULL;
					a[i][j].last3=&a[i-1][j-1];
				}
				else if(((a[i-1][j-1].sum+the_different)<(a[i-1][j].sum+the_empty))&&((a[i-1][j].sum+the_empty)>(a[i][j-1].sum+the_empty)))
				{
					a[i][j].sum=a[i-1][j].sum+the_empty;
					a[i][j].last1=a[i][j].last2=a[i][j].last3=NULL;
					a[i][j].last2=&a[i-1][j];
				}
				else if(((a[i][j-1].sum+the_empty)>(a[i-1][j].sum+the_empty))&&((a[i-1][j-1].sum+the_different)<(a[i][j-1].sum+the_empty)))
				{
					a[i][j].sum=a[i][j-1].sum+the_empty;
					a[i][j].last1=a[i][j].last2=a[i][j].last3=NULL;
					a[i][j].last1=&a[i][j-1];
				}
				//(a[i-1][j-1].sum+the_same)==(a[i][j-1].sum+the_empty)
				else if(((a[i-1][j-1].sum+the_different)>(a[i-1][j].sum+the_empty))&&((a[i-1][j-1].sum+the_different)==(a[i][j-1].sum+the_empty)))
				{
					a[i][j].sum=a[i][j-1].sum+the_empty;
					a[i][j].last1=a[i][j].last2=a[i][j].last3=NULL;
					a[i][j].last1=&a[i][j-1];
					a[i][j].last3=&a[i-1][j-1];
				}
				//(a[i-1][j-1].sum+the_same)==(a[i-1][j].sum+the_empty)
				else if(((a[i-1][j-1].sum+the_different)==(a[i-1][j].sum+the_empty))&&((a[i-1][j-1].sum+the_different)>(a[i][j-1].sum+the_empty)))
				{
					a[i][j].sum=a[i-1][j-1].sum+the_different;
					a[i][j].last1=a[i][j].last2=a[i][j].last3=NULL;
					a[i][j].last2=&a[i-1][j];
					a[i][j].last3=&a[i-1][j-1];
				}
				//(a[i-1][j].sum+the_empty)==(a[i][j-1].sum+the_empty)
				else if(((a[i-1][j-1].sum+the_different)<(a[i-1][j].sum+the_empty))&&((a[i-1][j].sum+the_empty)==(a[i][j-1].sum+the_empty)))
				{
					a[i][j].sum=a[i][j-1].sum+the_empty;
					a[i][j].last1=a[i][j].last2=a[i][j].last3=NULL;
					a[i][j].last1=&a[i][j-1];
					a[i][j].last2=&a[i-1][j];
				}
				//全等
				else if(((a[i-1][j-1].sum+the_different)==(a[i-1][j].sum+the_empty))&&((a[i-1][j-1].sum+the_different)==(a[i][j-1].sum+the_empty)))
				{
					a[i][j].sum=a[i][j-1].sum+the_empty;
					a[i][j].last1=&a[i][j-1];
					a[i][j].last3=&a[i-1][j-1];
					a[i][j].last2=&a[i-1][j];
				}
			}
		}
	//for(int i=0;i<=leny;i++)
	//{
	//	for(int j=0;j<=lenx;j++)
	//	{
	//		//cout<<a[i][j].sum<<"	";
	//		printf("%3d",a[i][j].sum);
	//	}
	//	cout<<"\n";
	//}
	//NODE b;
	//b=a[5][6];
	//if(b.last1!=NULL)cout<<"1";
	//if(b.last2!=NULL)cout<<"2";
	//if(b.last3!=NULL)cout<<"3";
	return 0;
}
/*蛋白质比对*/				int check_p(int nt_b)
{
	//NODE a[20][20];
	for(int i=0;i<20;i++)
		for(int j=0;j<20;j++)
			ClearStruct((unsigned char*)&a[i][j],sizeof(NODE));
	if(nt_b==1)
	{
		int Blosum_62[20][20]=
		{
			9,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
			-1,-4,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
			-1,1,5,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
			-3,-1,-1,7,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
			0,1,0,-1,4,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
			-3,0,-2,-2,0,6,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
			-3,1,0,-2,-2,0,6,20,20,20,20,20,20,20,20,20,20,20,20,20,
			-3,0,-1,-1,-2,-1,1,6,20,20,20,20,20,20,20,20,20,20,20,20,
			-4,0,-1,-1,-1,-2,0,2,5,20,20,20,20,20,20,20,20,20,20,20,
			-3,0,-1,-1,-1,-2,0,0,2,5,20,20,20,20,20,20,20,20,20,20,

			-3,-1,-2,-2,-2,-2,1,-1,0,0,8,20,20,20,20,20,20,20,20,20,
			-3,-1,-1,-2,-1,-2,0,-2,0,1,0,5,20,20,20,20,20,20,20,20,
			-3,0,-1,-1,-1,-2,0,-1,1,1,-1,2,5,20,20,20,20,20,20,20,

			-1,-1,-1,-2,-1,-3,-2,-3,-2,0,-2,-1,-1,5,20,20,20,20,20,20,
			-1,-2,-1,-3,-1,-4,-3,-3,-3,-3,-3,-3,-3,1,4,20,20,20,20,20,
			-1,2,-1,-3,-1,-4,-3,-4,-3,-2,-3,-2,-2,2,2,4,20,20,20,20,
			-1,-2,0,-2,0,-3,-3,-3,-2,-2,-3,-3,-2,1,3,1,4,20,20,20,

			-2,-2,-2,-4,-2,-3,-3,-3,-3,-3,-1,-3,-3,0,0,0,-1,6,20,20,
			-2,-2,-2,-3,-2,-3,-2,-3,-2,-1,2,-2,-2,-1,-1,-1,-1,3,7,20,
			-2,-3,-2,-4,-3,-2,-4,-4,-3,-2,-2,-2,-3,-1,-3,-2,-3,1,2,11
		};
		char Protein[21]={"CSTPAGNDEQHRKMILVFYW"};
		/*char b[2];
		b[0]=Blosum_62[16][5]+'0';
		b[1]='\0';
		MessageBox(NULL,b,"6",MB_OK);*/
		a[0][0].last1=a[0][0].last2=a[0][0].last3=NULL;
		a[0][0].sum=0;
		for(int i=1;i<=lenx;i++)
		{
			a[0][i].sum=a[0][i-1].sum+the_empty_p;
			a[0][i].last1=&a[0][i-1];
			a[0][i].last2=a[0][i].last3=NULL;
		}
		for(int i=1;i<=leny;i++)
		{
			a[i][0].sum=a[i-1][0].sum+the_empty_p;
			a[i][0].last2=&a[i-1][0];
			a[i][0].last1=a[i][0].last3=NULL;

		}
		for(int i=1;i<=leny;i++)
			for(int j=1;j<=lenx;j++)
			{
				/*if(list_x[j-1]==list_y[i-1])*/
				int m,n;
				for(m=0;m<20;m++)
				if(list_x[j-1]==Protein[m])
					break;
				for(n=0;n<20;n++)
				if(list_y[i-1]==Protein[n])
					break;
				if(Blosum_62[m][n]==20)
				the_same_p=Blosum_62[n][m];
				else                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
				the_same_p=Blosum_62[m][n];
				{
					if(((a[i-1][j-1].sum+the_same_p)>(a[i-1][j].sum+the_empty_p))&&((a[i-1][j-1].sum+the_same_p)>(a[i][j-1].sum+the_empty_p)))
					{
						a[i][j].sum=a[i-1][j-1].sum+the_same_p;
						a[i][j].last1=a[i][j].last2=a[i][j].last3=NULL;
						a[i][j].last3=&a[i-1][j-1];
					}
					else if(((a[i-1][j-1].sum+the_same_p)<(a[i-1][j].sum+the_empty_p))&&((a[i-1][j].sum+the_empty_p)>(a[i][j-1].sum+the_empty_p)))
					{
						a[i][j].sum=a[i-1][j].sum+the_empty_p;
						a[i][j].last1=a[i][j].last2=a[i][j].last3=NULL;
						a[i][j].last2=&a[i-1][j];
					}
					else if(((a[i][j-1].sum+the_empty_p)>(a[i-1][j].sum+the_empty_p))&&((a[i-1][j-1].sum+the_same_p)<(a[i][j-1].sum+the_empty_p)))
					{
						a[i][j].sum=a[i][j-1].sum+the_empty_p;
						a[i][j].last1=a[i][j].last2=a[i][j].last3=NULL;
						a[i][j].last1=&a[i][j-1];
					}
					//(a[i-1][j-1].sum+the_same)==(a[i][j-1].sum+the_empty)
					else if(((a[i-1][j-1].sum+the_same_p)>(a[i-1][j].sum+the_empty_p))&&((a[i-1][j-1].sum+the_same_p)==(a[i][j-1].sum+the_empty_p)))
					{
						a[i][j].sum=a[i][j-1].sum+the_empty_p;
						a[i][j].last1=a[i][j].last2=a[i][j].last3=NULL;
						a[i][j].last1=&a[i][j-1];
						a[i][j].last3=&a[i-1][j-1];
					}
					//(a[i-1][j-1].sum+the_same)==(a[i-1][j].sum+the_empty)
					else if(((a[i-1][j-1].sum+the_same_p)==(a[i-1][j].sum+the_empty_p))&&((a[i-1][j-1].sum+the_same_p)>(a[i][j-1].sum+the_empty_p)))
					{
						a[i][j].sum=a[i-1][j-1].sum+the_same_p;
						a[i][j].last1=a[i][j].last2=a[i][j].last3=NULL;
						a[i][j].last2=&a[i-1][j];
						a[i][j].last3=&a[i-1][j-1];
					}
					//(a[i-1][j].sum+the_empty)==(a[i][j-1].sum+the_empty)
					else if(((a[i-1][j-1].sum+the_same_p)<(a[i-1][j].sum+the_empty_p))&&((a[i-1][j].sum+the_empty_p)==(a[i][j-1].sum+the_empty_p)))
					{
						a[i][j].sum=a[i][j-1].sum+the_empty_p;
						a[i][j].last1=a[i][j].last2=a[i][j].last3=NULL;
						a[i][j].last1=&a[i][j-1];
						a[i][j].last2=&a[i-1][j];
					}
					//全等
					else if(((a[i-1][j-1].sum+the_same_p)==(a[i-1][j].sum+the_empty_p))&&((a[i-1][j-1].sum+the_same_p)==(a[i][j-1].sum+the_empty_p)))
					{
						a[i][j].sum=a[i][j-1].sum+the_empty_p;
						a[i][j].last1=&a[i][j-1];
						a[i][j].last3=&a[i-1][j-1];
						a[i][j].last2=&a[i-1][j];
					}
				}
				//else
				//{
				//	if(((a[i-1][j-1].sum+the_different)>(a[i-1][j].sum+the_empty))&&((a[i-1][j-1].sum+the_different)>(a[i][j-1].sum+the_empty)))
				//	{
				//		a[i][j].sum=a[i-1][j-1].sum+the_different;
				//		a[i][j].last1=a[i][j].last2=a[i][j].last3=NULL;
				//		a[i][j].last3=&a[i-1][j-1];
				//	}
				//	else if(((a[i-1][j-1].sum+the_different)<(a[i-1][j].sum+the_empty))&&((a[i-1][j].sum+the_empty)>(a[i][j-1].sum+the_empty)))
				//	{
				//		a[i][j].sum=a[i-1][j].sum+the_empty;
				//		a[i][j].last1=a[i][j].last2=a[i][j].last3=NULL;
				//		a[i][j].last2=&a[i-1][j];
				//	}
				//	else if(((a[i][j-1].sum+the_empty)>(a[i-1][j].sum+the_empty))&&((a[i-1][j-1].sum+the_different)<(a[i][j-1].sum+the_empty)))
				//	{
				//		a[i][j].sum=a[i][j-1].sum+the_empty;
				//		a[i][j].last1=a[i][j].last2=a[i][j].last3=NULL;
				//		a[i][j].last1=&a[i][j-1];
				//	}
				//	//(a[i-1][j-1].sum+the_same)==(a[i][j-1].sum+the_empty)
				//	else if(((a[i-1][j-1].sum+the_different)>(a[i-1][j].sum+the_empty))&&((a[i-1][j-1].sum+the_different)==(a[i][j-1].sum+the_empty)))
				//	{
				//		a[i][j].sum=a[i][j-1].sum+the_empty;
				//		a[i][j].last1=a[i][j].last2=a[i][j].last3=NULL;
				//		a[i][j].last1=&a[i][j-1];
				//		a[i][j].last3=&a[i-1][j-1];
				//	}
				//	//(a[i-1][j-1].sum+the_same)==(a[i-1][j].sum+the_empty)
				//	else if(((a[i-1][j-1].sum+the_different)==(a[i-1][j].sum+the_empty))&&((a[i-1][j-1].sum+the_different)>(a[i][j-1].sum+the_empty)))
				//	{
				//		a[i][j].sum=a[i-1][j-1].sum+the_different;
				//		a[i][j].last1=a[i][j].last2=a[i][j].last3=NULL;
				//		a[i][j].last2=&a[i-1][j];
				//		a[i][j].last3=&a[i-1][j-1];
				//	}
				//	//(a[i-1][j].sum+the_empty)==(a[i][j-1].sum+the_empty)
				//	else if(((a[i-1][j-1].sum+the_different)<(a[i-1][j].sum+the_empty))&&((a[i-1][j].sum+the_empty)==(a[i][j-1].sum+the_empty)))
				//	{
				//		a[i][j].sum=a[i][j-1].sum+the_empty;
				//		a[i][j].last1=a[i][j].last2=a[i][j].last3=NULL;
				//		a[i][j].last1=&a[i][j-1];
				//		a[i][j].last2=&a[i-1][j];
				//	}
				//	//全等
				//	else if(((a[i-1][j-1].sum+the_different)==(a[i-1][j].sum+the_empty))&&((a[i-1][j-1].sum+the_different)==(a[i][j-1].sum+the_empty)))
				//	{
				//		a[i][j].sum=a[i][j-1].sum+the_empty;
				//		a[i][j].last1=&a[i][j-1];
				//		a[i][j].last3=&a[i-1][j-1];
				//		a[i][j].last2=&a[i-1][j];
				//	}
				//}
			}
	
		//for(int i=0;i<=leny;i++)
		//{
		//	for(int j=0;j<=lenx;j++)
		//	{
		//		//cout<<a[i][j].sum<<"	";
		//		printf("%3d",a[i][j].sum);
		//	}
		//	cout<<"\n";
		//}
		//NODE b;
		//b=a[5][6];
		//if(b.last1!=NULL)cout<<"1";
		//if(b.last2!=NULL)cout<<"2";
		//if(b.last3!=NULL)cout<<"3";
		return 0;
	}
	else if(nt_b==2)
	{

		int PAM250[22][22]=
		{
			
			2 ,-2, 0 ,0 ,-2 ,0, 0 ,1, -1, -1, -2 ,-1 ,-1 ,-3 ,1 ,1 ,1 ,-6 ,-3 ,0 ,2 ,1 ,					//1
			-2 ,6, 0, -1 ,-4 ,1, -1, -3, 2, -2, -3, 3 ,0 ,-4 ,0 ,0 ,-1, 2 ,-4, -2, 1 ,2,					//2
			0 ,0 ,2 ,2 ,-4, 1 ,1, 0 ,2, -2 ,-3, 1 ,-2 ,-3 ,0 ,1 ,0 ,-4 ,-2 ,-2 ,4 ,3 ,						//3
			0 ,-1 ,2 ,4, -5 ,2 ,3 ,1, 1 ,-2 ,-4 ,0 ,-3 ,-6 ,-1, 0 ,0 ,-7, -4, -2, 5 ,4,						//4
			-2 ,-4, -4 ,-5 ,12 ,-5 ,-5, -3 ,-3, -2, -6, -5, -5, -4, -3 ,0 ,-2 ,-8 ,0 ,-2, -3, -4,			//5
			0 ,1, 1 ,2 ,-5, 4 ,2 ,-1 ,3, -2, -2 ,1 ,-1, -5, 0, -1, -1 ,-5 ,-4, -2, 3, 5 ,					//6
			0 ,-1 ,1 ,3 ,-5 ,2, 4, 0 ,1 ,-2 ,-3, 0 ,-2 ,-5, -1, 0, 0 ,-7, -4, -2, 4 ,5,						//7
			1, -3 ,0, 1, -3, -1, 0 ,5 ,-2, -3, -4, -2, -3 ,-5, 0, 1 ,0 ,-7 ,-5 ,-1 ,2 ,1,					//8
			-1 ,2 ,2,1 ,-3 ,3, 1, -2, 6 ,-2 ,-2 ,0 ,-2 ,-2 ,0 ,-1 ,-1 ,-3 ,0 ,-2 ,3 ,3 ,					//9
			-1 ,-2 ,-2 ,-2, -2 ,-2 ,-2 ,-3 ,-2 ,5 ,2, -2 ,2, 1, -2 ,-1, 0 ,-5, -1, 4, -1, -1,				//10
			-2 ,-3, -3, -4 ,-6 ,-2 ,-3 ,-4 ,-2 ,2 ,6 ,-3 ,4 ,2, -3 ,-3, -2 ,-2 ,-1 ,2 ,-2 ,-1 ,				//11
			-1 ,3 ,1 ,0 ,-5, 1, 0, -2, 0 ,-2 ,-3 ,5 ,0 ,-5 ,-1 ,0, 0 ,-3, -4, -2, 2 ,2 ,					//12
			-1 ,0 ,-2, -3 ,-5 ,-1, -2 ,-3 ,-2 ,2 ,4 ,0, 6, 0 ,-2, -2 ,-1, -4 ,-2, 2, -1, 0 ,				//13	
			-3, -4, -3, -6 ,-4 ,-5 ,-5 ,-5 ,-2 ,1 ,2 ,-5 ,0, 9 ,-5 ,-3 ,-3, 0, 7, -1, -3 ,-4,				//14
			1, 0, 0, -1, -3, 0 ,-1, 0, 0 ,-2, -3 ,-1, -2, -5, 6, 1, 0, -6 ,-5, -1, 1, 1,					//15
			1 ,0 ,1, 0, 0 ,-1 ,0 ,1, -1 ,-1, -3 ,0 ,-2 ,-3 ,1 ,2 ,1, -2, -3 ,-1, 2 ,1 ,						//16	
			1 ,-1 ,0 ,0 ,-2 ,-1 ,0 ,0 ,-1 ,0, -2, 0, -1 ,-3 ,0 ,1, 3 ,-5 ,-3, 0, 2, 1,						//17
			-6, 2, -4, -7 ,-8 ,-5 ,-7 ,-7 ,-3, -5 ,-2 ,-3 ,-4 ,0, -6, -2, -5, 17, 0 ,-6, -4, -4 ,			//18
			-3 ,-4 ,-2, -4, 0 ,-4 ,-4 ,-5 ,0, -1, -1 ,-4, -2, 7 ,-5, -3, -3 ,0, 10 ,-2 ,-2 ,-3 ,			//19	
			0, -2 ,-2 ,-2, -2, -2 ,-2 ,-1, -2, 4 ,2, -2, 2 ,-1, -1, -1 ,0, -6 ,-2 ,4 ,0 ,0 ,				//20
			2, 1, 4 ,5 ,-3, 3 ,4 ,2 ,3 ,-1 ,-2 ,2 ,-1 ,-3 ,1, 2, 2 ,-4 ,-2 ,0 ,6 ,5 ,						//21	
			1 ,2 ,3, 4, -4, 5 ,5, 1, 3 ,-1, -1, 2 ,0 ,-4, 1 ,1 ,1, -4, -3, 0 ,5 ,6							//22
		};
		char Protein[23]={"ARNDCQEGHILKMFPSTWYVBZ"};
		/*char b[2];
		b[0]=Blosum_62[16][5]+'0';
		b[1]='\0';
		MessageBox(NULL,b,"6",MB_OK);*/
		a[0][0].last1=a[0][0].last2=a[0][0].last3=NULL;
		a[0][0].sum=0;
		for(int i=1;i<=lenx;i++)
		{
			a[0][i].sum=a[0][i-1].sum+the_empty_p;
			a[0][i].last1=&a[0][i-1];
			a[0][i].last2=a[0][i].last3=NULL;
		}
		for(int i=1;i<=leny;i++)
		{
			a[i][0].sum=a[i-1][0].sum+the_empty_p;
			a[i][0].last2=&a[i-1][0];
			a[i][0].last1=a[i][0].last3=NULL;

		}
		for(int i=1;i<=leny;i++)
			for(int j=1;j<=lenx;j++)
			{
				/*if(list_x[j-1]==list_y[i-1])*/
				int m,n;
				for(m=0;m<22;m++)
				if(list_x[j-1]==Protein[m])
					break;
				for(n=0;n<22;n++)
				if(list_y[i-1]==Protein[n])
					break;
				/*if(Blosum_62[m][n]==20)
				the_same_p=Blosum_62[n][m];
				else                          */                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
				the_same_p=PAM250[m][n];
				{
					if(((a[i-1][j-1].sum+the_same_p)>(a[i-1][j].sum+the_empty_p))&&((a[i-1][j-1].sum+the_same_p)>(a[i][j-1].sum+the_empty_p)))
					{
						a[i][j].sum=a[i-1][j-1].sum+the_same_p;
						a[i][j].last1=a[i][j].last2=a[i][j].last3=NULL;
						a[i][j].last3=&a[i-1][j-1];
					}
					else if(((a[i-1][j-1].sum+the_same_p)<(a[i-1][j].sum+the_empty_p))&&((a[i-1][j].sum+the_empty_p)>(a[i][j-1].sum+the_empty_p)))
					{
						a[i][j].sum=a[i-1][j].sum+the_empty_p;
						a[i][j].last1=a[i][j].last2=a[i][j].last3=NULL;
						a[i][j].last2=&a[i-1][j];
					}
					else if(((a[i][j-1].sum+the_empty_p)>(a[i-1][j].sum+the_empty_p))&&((a[i-1][j-1].sum+the_same_p)<(a[i][j-1].sum+the_empty_p)))
					{
						a[i][j].sum=a[i][j-1].sum+the_empty_p;
						a[i][j].last1=a[i][j].last2=a[i][j].last3=NULL;
						a[i][j].last1=&a[i][j-1];
					}
					//(a[i-1][j-1].sum+the_same)==(a[i][j-1].sum+the_empty)
					else if(((a[i-1][j-1].sum+the_same_p)>(a[i-1][j].sum+the_empty_p))&&((a[i-1][j-1].sum+the_same_p)==(a[i][j-1].sum+the_empty_p)))
					{
						a[i][j].sum=a[i][j-1].sum+the_empty_p;
						a[i][j].last1=a[i][j].last2=a[i][j].last3=NULL;
						a[i][j].last1=&a[i][j-1];
						a[i][j].last3=&a[i-1][j-1];
					}
					//(a[i-1][j-1].sum+the_same)==(a[i-1][j].sum+the_empty)
					else if(((a[i-1][j-1].sum+the_same_p)==(a[i-1][j].sum+the_empty_p))&&((a[i-1][j-1].sum+the_same_p)>(a[i][j-1].sum+the_empty_p)))
					{
						a[i][j].sum=a[i-1][j-1].sum+the_same_p;
						a[i][j].last1=a[i][j].last2=a[i][j].last3=NULL;
						a[i][j].last2=&a[i-1][j];
						a[i][j].last3=&a[i-1][j-1];
					}
					//(a[i-1][j].sum+the_empty)==(a[i][j-1].sum+the_empty)
					else if(((a[i-1][j-1].sum+the_same_p)<(a[i-1][j].sum+the_empty_p))&&((a[i-1][j].sum+the_empty_p)==(a[i][j-1].sum+the_empty_p)))
					{
						a[i][j].sum=a[i][j-1].sum+the_empty_p;
						a[i][j].last1=a[i][j].last2=a[i][j].last3=NULL;
						a[i][j].last1=&a[i][j-1];
						a[i][j].last2=&a[i-1][j];
					}
					//全等
					else if(((a[i-1][j-1].sum+the_same_p)==(a[i-1][j].sum+the_empty_p))&&((a[i-1][j-1].sum+the_same_p)==(a[i][j-1].sum+the_empty_p)))
					{
						a[i][j].sum=a[i][j-1].sum+the_empty_p;
						a[i][j].last1=&a[i][j-1];
						a[i][j].last3=&a[i-1][j-1];
						a[i][j].last2=&a[i-1][j];
					}
				}
			}

		return 0;
	}
	return -1;
}
/*初始化回溯用的数组*/		void initMade_me()
{
	made_i=0;
	for(int i=0;i<20;i++)
		for(int j=0;j<40;j++)
		{
			made_me[i][j].x=-1;
			made_me[i][j].y=-1;
		}
	
}
/*回溯*/					int search_back(int me_i,int me_j,int num_x,int num_y)//回溯函数
{
	
	NODE*p=&a[num_y][num_x];
	/*me[0][0].x=leny;
	me[0][0].y=lenx;*/
	if(p->last1!=NULL&&p->last2==NULL&&p->last3==NULL)
	{
		me[me_i][me_j].x=num_x--;
		me[me_i][me_j++].y=num_y;
		//p=p->last1;
		search_back(me_i,me_j,num_x,num_y);
	}
	else if(p->last1==NULL&&p->last2!=NULL&&p->last3==NULL)
	{
		me[me_i][me_j].x=num_x;
		me[me_i][me_j++].y=num_y--;
		//p=p->last1;
		search_back(me_i,me_j,num_x,num_y);
	}
	else if(p->last1==NULL&&p->last2==NULL&&p->last3!=NULL)
	{
		me[me_i][me_j].x=num_x--;
		me[me_i][me_j++].y=num_y--;
		//p=p->last1;
		search_back(me_i,me_j,num_x,num_y);
	}
	else if(p->last1!=NULL&&p->last2!=NULL&&p->last3==NULL)
	{
		for(int i=0;i<me_j;i++)
		{
			me[me_i+1][i].x=me[me_i][i].x;
			me[me_i+1][i].y=me[me_i][i].y;
		}
		me[me_i+1][me_j].x=num_x;
		me[me_i+1][me_j].y=num_y;
		search_back(me_i+1,me_j+1,num_x,num_y-1);
		me[me_i][me_j].x=num_x--;
		me[me_i][me_j++].y=num_y;
		//p=p->last1;
		search_back(me_i,me_j,num_x,num_y);
	}
	else if(p->last1!=NULL&&p->last2==NULL&&p->last3!=NULL)
	{
		for(int i=0;i<me_j;i++)
		{
			me[me_i+1][i].x=me[me_i][i].x;
			me[me_i+1][i].y=me[me_i][i].y;
		}
		me[me_i+1][me_j].x=num_x;
		me[me_i+1][me_j].y=num_y;
		search_back(me_i+1,me_j+1,num_x-1,num_y-1);
		me[me_i][me_j].x=num_x--;
		me[me_i][me_j++].y=num_y;
		//p=p->last1;
		search_back(me_i,me_j,num_x,num_y);
	}
	else if(p->last1==NULL&&p->last2!=NULL&&p->last3!=NULL)
	{
		for(int i=0;i<me_j;i++)
		{
			me[me_i+1][i].x=me[me_i][i].x;
			me[me_i+1][i].y=me[me_i][i].y;
		}
		me[me_i+1][me_j].x=num_x;
		me[me_i+1][me_j].y=num_y;
		search_back(me_i+1,me_j+1,num_x-1,num_y-1);
		me[me_i][me_j].x=num_x--;
		me[me_i][me_j++].y=num_y;
		//p=p->last1;
		search_back(me_i,me_j,num_x,num_y);
	}
	else if(p->last1!=NULL&&p->last2!=NULL&&p->last3!=NULL)
	{
		for(int i=0;i<me_j;i++)
		{
			me[me_i+1][i].x=me[me_i][i].x;
			me[me_i+1][i].y=me[me_i][i].y;
		}
		me[me_i+1][me_j].x=num_x;
		me[me_i+1][me_j].y=num_y;
		search_back(me_i+1,me_j+1,num_x,num_y-1);
		for(int i=0;i<me_j;i++)
		{
			me[me_i+2][i].x=me[me_i][i].x;
			me[me_i+2][i].y=me[me_i][i].y;
		}
		me[me_i+2][me_j].x=num_x;
		me[me_i+2][me_j].y=num_y;
		search_back(me_i+2,me_j+1,num_x-1,num_y-1);
		me[me_i][me_j].x=num_x--;
		me[me_i][me_j++].y=num_y;
		//p=p->last1;
		search_back(me_i,me_j,num_x,num_y);
	}
	else if(p->last1==NULL&&p->last2==NULL&&p->last3==NULL)
	{
		me[me_i][me_j].x=num_x;
		me[me_i][me_j].y=num_y;
		for(int i=0;i<=me_j;i++)
		{
			made_me[made_i][i].x=me[me_i][i].x;
			made_me[made_i][i].y=me[me_i][i].y;
		}
		made_i++;
		return 0;
	}
	else
		return 0;
	return -1;
}
/*画树*/					int drawtree(HDC hdc,int x,int y,int a,int b,int c)
{
	line(hdc,x,y,x+a,y);
	line(hdc,x,y,x,y+b);
	line(hdc,x,y+b,x+c,y+b);
	return 0;
}
/*注册窗口*/				int  InitWndclass(HINSTANCE hInstance)
{
	WNDCLASS wndclass;	//创建一个窗口类
	wndclass.cbClsExtra = 0;	//窗口类无扩展
	wndclass.cbWndExtra = 0;	//窗口实例无扩展
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//窗口白色
	wndclass.hCursor = LoadCursor(NULL,/*IDC_HAND*/IDC_ARROW/*NULL*/);	//手型	//箭头
	wndclass.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));	//默认最小化图标
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WinProc1;	//定义窗口处理函数
	wndclass.lpszClassName =WndClassName1;	//窗口类名
	wndclass.lpszMenuName = NULL;	//窗口无菜单
	wndclass.style = CS_HREDRAW|CS_PARENTDC;	//设置窗口类型
	//注册窗口类
	if(!RegisterClass(&wndclass))  //判断窗口是否注册成功
		return -1;
	wndclass.lpfnWndProc = WinProc2;	//定义窗口处理函数
	wndclass.lpszClassName =WndClassName2;	//窗口类名
	//注册窗口类
	if(!RegisterClass(&wndclass))  //判断窗口是否注册成功
		return -2;
	wndclass.lpfnWndProc = WinProc3;	//定义窗口处理函数
	wndclass.lpszClassName =WndClassName3;	//窗口类名
	//注册窗口类
	if(!RegisterClass(&wndclass))  //判断窗口是否注册成功
		return -3;
	wndclass.lpfnWndProc = WinProc4;	//定义窗口处理函数
	wndclass.lpszClassName =WndClassName4;	//窗口类名
	//注册窗口类
	if(!RegisterClass(&wndclass))  //判断窗口是否注册成功
		return -4;
	wndclass.lpfnWndProc = WinProc5;	//定义窗口处理函数
	wndclass.lpszClassName =WndClassName5;	//窗口类名
	//注册窗口类
	if(!RegisterClass(&wndclass))  //判断窗口是否注册成功
		return -5;
	wndclass.lpfnWndProc = WinProc6;	//定义窗口处理函数
	wndclass.lpszClassName =WndClassName6;	//窗口类名
	//注册窗口类
	if(!RegisterClass(&wndclass))  //判断窗口是否注册成功
		return -6;
	wndclass.lpfnWndProc = WinProc7;	//定义窗口处理函数
	wndclass.lpszClassName =WndClassName7;	//窗口类名
	//注册窗口类
	if(!RegisterClass(&wndclass))  //判断窗口是否注册成功
		return -6;
	return 0;
}
/*DNA比对输入*/				void CreateW1(HWND *hWnd,HINSTANCE hInstance)//DNA比对输入
{
	
	*hWnd=CreateWindow(WndClassName1,NULL,/*WS_POPUP*/WS_OVERLAPPEDWINDOW^WS_THICKFRAME,(cx-wWidth1)/2,(cy-wHeight1)/2,wWidth1,wHeight1,NULL,NULL,hInstance,NULL);//创建窗口
	ShowWindow(*hWnd,SW_SHOW);
	UpdateWindow(*hWnd);
	::SetFocus(hWrite1);
	MSG msg;
	while(GetMessage(&msg,NULL,0,0))
	{
		if(msg.message==WM_KEYDOWN&&msg.wParam==VK_TAB)
			{
				HWND hWnd666=::GetFocus();
				int iID=::GetDlgCtrlID(hWnd666);
				if(iID==2)
					::SetFocus(hWrite2);
				else if(iID==3)
					::SetFocus(hWrite3);
				else if(iID==4)
					::SetFocus(hWrite4);
				else if(iID==5)
					::SetFocus(hWrite5);
				else if(iID==6)
					::SetFocus(hWrite1);
			}
		else
		{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		}
	}

}
/*比对结果显示*/			void CreateW2(HWND hWnd,HINSTANCE hInstance)//比对结果显示
{
	hWnd=CreateWindow(WndClassName2,"Acetaldehyde",/*WS_POPUP*/WS_OVERLAPPEDWINDOW/*^WS_THICKFRAME*/,(cx-wWidth2)/2.0,(cy-wHeight2)/2.0,wWidth2,wHeight2,NULL,NULL,hInstance,NULL);//创建窗口
	ShowWindow(hWnd,SW_SHOWNORMAL);
	UpdateWindow(hWnd);
	//CreateWindow("button","按钮",WS_CHILD|WS_BORDER|WS_VISIBLE|BS_PUSHBUTTON,10,10,50,20,hWnd,HMENU(1),hInstance_1,NULL);
	MSG msg;
	//made_i=0;
	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}
/*初始化窗口*/				void CreateW3(HWND *hWnd,HINSTANCE hInstance)//初始化窗口
{
	*hWnd=CreateWindow(WndClassName3,NULL,WS_POPUP/*WS_OVERLAPPEDWINDOW*/,(cx-wWidth0)/2,(cy-wHeight0)/2,wWidth0,wHeight0,NULL,NULL,hInstance,NULL);//创建窗口
	ShowWindow(*hWnd,SW_SHOWNORMAL);
	UpdateWindow(*hWnd);
	MSG msg;
	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}
/*主菜单窗口*/				void CreateW4(HWND *hWnd,HINSTANCE hInstance)//主菜单窗口
{
	*hWnd=CreateWindow(WndClassName4,NULL,WS_POPUP/*WS_OVERLAPPEDWINDOW*/,(cx-wWidth4)/2,(cy-wHeight4)/2,wWidth4,wHeight4,NULL,NULL,hInstance,NULL);//创建窗口
	ShowWindow(*hWnd,SW_SHOWNORMAL);
	UpdateWindow(*hWnd);
	MSG msg;
	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}
/*建树输入窗口*/			void CreateW5(HWND *hWnd,HINSTANCE hInstance)//建树输入窗口
{
	*hWnd=CreateWindow(WndClassName5,NULL,/*WS_POPUP*/WS_OVERLAPPEDWINDOW,(cx-wWidth5)/2,(cy-wHeight5)/2,wWidth5,wHeight5,NULL,NULL,hInstance,NULL);//创建窗口
	ShowWindow(*hWnd,SW_SHOWNORMAL);
	UpdateWindow(*hWnd);
	MSG msg;
	while(GetMessage(&msg,NULL,0,0))
	{

		if(msg.message==WM_KEYDOWN&&msg.wParam==VK_TAB)
			{
				HWND hWnd666=::GetFocus();
				int iID=::GetDlgCtrlID(hWnd666);
				if(iID==666)
				{
					//MessageBox(NULL,"123","456",MB_OK);
					int i;
					int j;
					
					for(i=0;i<Box_Line;i++)
						for(j=0;j<Box_Line;j++)
						{
							if(hWrite[i][j]==hWnd666)
							{
								
								if((i==Box_Line-1)&&(j<Box_Line-2))
								{
									::SetFocus(hWrite[j+2][j+1]);
								break;
								}
								else if((i==j-1)&&(j<Box_Line-1))
								{
									::SetFocus(hWrite[1][j+1]);
								break;
								}
								else if(i<Box_Line-1)
								{
								::SetFocus(hWrite[i+1][j]);
								break;
								}
							}
						}
						
					/*	
						if(j<Box_Line-1)
					::SetFocus(hWrite[i][j+1]);*/
				}
				
			}
		else if(msg.message==WM_KEYDOWN&&msg.wParam==VK_RIGHT)
			{
				HWND hWnd666=::GetFocus();
				int iID=::GetDlgCtrlID(hWnd666);
				if(iID==666)
				{
					//MessageBox(NULL,"123","456",MB_OK);
					int i;
					int j;
					
					for(i=0;i<Box_Line;i++)
						for(j=0;j<Box_Line;j++)
						{
							if(hWrite[i][j]==hWnd666)
							{
								if(i<Box_Line-1)
								::SetFocus(hWrite[i+1][j]);
								break;
							}
						}
				}
				
			}
		else if(msg.message==WM_KEYDOWN&&msg.wParam==VK_UP)
			{
				HWND hWnd666=::GetFocus();
				int iID=::GetDlgCtrlID(hWnd666);
				if(iID==666)
				{
					//MessageBox(NULL,"123","456",MB_OK);
					int i;
					int j;
					
					for(i=0;i<Box_Line;i++)
						for(j=0;j<Box_Line;j++)
						{
							if(hWrite[i][j]==hWnd666)
							{
								if(j>0)
								::SetFocus(hWrite[i][j-1]);
								break;
							}
						}
				}
				
			}
		else if(msg.message==WM_KEYDOWN&&msg.wParam==VK_LEFT)
			{
				HWND hWnd666=::GetFocus();
				int iID=::GetDlgCtrlID(hWnd666);
				if(iID==666)
				{
					//MessageBox(NULL,"123","456",MB_OK);
					int i;
					int j;
					
					for(i=0;i<Box_Line;i++)
						for(j=0;j<Box_Line;j++)
						{
							if(hWrite[i][j]==hWnd666)
							{
								if(i>0)
								::SetFocus(hWrite[i-1][j]);
								break;
							}
						}
				}
				
			}
		else if(msg.message==WM_KEYDOWN&&msg.wParam==VK_DOWN)
			{
				HWND hWnd666=::GetFocus();
				int iID=::GetDlgCtrlID(hWnd666);
				if(iID==666)
				{
					//MessageBox(NULL,"123","456",MB_OK);
					int i;
					int j;
					
					for(i=0;i<Box_Line;i++)
						for(j=0;j<Box_Line;j++)
						{
							if(hWrite[i][j]==hWnd666)
							{
								if(j<Box_Line-1)
								::SetFocus(hWrite[i][j+1]);
								break;
							}
						}
				}
				
			}
		else
		{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		}
	}

}
/*建树输出窗口*/			void CreateW6(HWND *hWnd,HINSTANCE hInstance)//建树输出窗口
{
	*hWnd=CreateWindow(WndClassName6,NULL,/*WS_POPUP*/WS_OVERLAPPEDWINDOW,(cx-wWidth6)/2,(cy-wHeight6)/2,wWidth6,wHeight6,NULL,NULL,hInstance,NULL);//创建窗口
	ShowWindow(*hWnd,SW_SHOWNORMAL);
	UpdateWindow(*hWnd);
	MSG msg;
	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}
/*蛋白质比对输入*/			void CreateW7(HWND *hWnd,HINSTANCE hInstance)//蛋白质比对输入
{
	
	*hWnd=CreateWindow(WndClassName7,NULL,/*WS_POPUP*/WS_OVERLAPPEDWINDOW^WS_THICKFRAME,(cx-wWidth7)/2,(cy-wHeight7)/2,wWidth7,wHeight7,NULL,NULL,hInstance,NULL);//创建窗口
	ShowWindow(*hWnd,SW_SHOW);
	UpdateWindow(*hWnd);
	::SetFocus(hWrite1);
	MSG msg;
	POINT pt;
	RECT Rect,Rect1;
	while(GetMessage(&msg,NULL,0,0))
	{
		if(msg.message==WM_LBUTTONDOWN||msg.message==WM_LBUTTONDBLCLK)
		{
			GetCursorPos(&pt);
			ScreenToClient(*hWnd,&pt);
			/*HWND hWnd666=::GetFocus();
			int iID=::GetDlgCtrlID(hWnd666);
			if(iID==4)*/
			GetWindowRect(*hWnd,&Rect);
			if((Rect.right-Rect.left)!=wWidth7)
			{
				if((pt.x>(cx/2.0+300))&&(pt.x<(cx/2.0+450))&&(pt.y>(cy/2.0-50-100))&&(pt.y<(cy/2.0-50-100+30)))
				
					TrackPopupMenu(hMenu1,TPM_LEFTALIGN,cx/2.0+300,cy/2.0-50-100+30+10,0,*hWnd,&Rect1);
				else
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else if((Rect.right-Rect.left)==wWidth7)
			{
				if((pt.x>480)&&(pt.x<630)&&(pt.y>200)&&(pt.y<230))
				
					TrackPopupMenu(hMenu1,TPM_LEFTALIGN,Rect.left+490,Rect.top+250,0,*hWnd,&Rect1);
				else
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			
		}
		else if(msg.message==WM_KEYDOWN&&msg.wParam==VK_TAB)
			{
				HWND hWnd666=::GetFocus();
				int iID=::GetDlgCtrlID(hWnd666);
				if(iID==2)
					::SetFocus(hWrite2);
				else if(iID==3)
					::SetFocus(hWrite4);
				/*else if(iID==4)
					::SetFocus(hWrite4);
				else if(iID==5)
					::SetFocus(hWrite5);*/
				else if(iID==5)
					::SetFocus(hWrite1);
			}
		else
		{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		}
	}

}

//主函数框
//----------------------------------------------------------------------------------
/*MAIN*/int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)//主函数
{
	hInstance_1=hInstance;
	InitWndclass(hInstance);

	CreateW3(&hWnd3,hInstance);
	CreateW4(&hWnd4,hInstance);


	//CreateW5(&hWnd5,hInstance_1);

	//CreateW1(&hWnd1,hInstance);
	/*check();
	CreateW2(hWnd2,hInstance_1);*/
	return 0;
}

//回调函数框---------------------------------------------------------------------------------------------

/*DNA比对输入*/				LRESULT CALLBACK WinProc1(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	int rad=5;
	PAINTSTRUCT ps;
	static HDC hdc,mdc,mdc1;
	//static HFONT hFont2;    //字体句柄
	static UINT c_x=wWidth1,c_y=wHeight1;
	 static HFONT hFont = CreateFont
                        (
                            20,0,    //高度20, 宽取0表示由系统选择最佳
                            0, 0,    //文本倾斜，与字体倾斜都为0
                            0/*FW_HEAVY*/,    //粗体
                            0,0,0,        //非斜体，无下划线，无中划线
                            GB2312_CHARSET,    //字符集
                            OUT_DEFAULT_PRECIS,        
                            CLIP_DEFAULT_PRECIS,        
                            DEFAULT_QUALITY,        //一系列的默认值
                            DEFAULT_PITCH | FF_DONTCARE,   
                           (TCHAR*)"楷体"    //字体名称
                        );
	static HFONT hFont1 = CreateFont
                        (
                            18,0,    //高度20, 宽取0表示由系统选择最佳
                            0, 0,    //文本倾斜，与字体倾斜都为0
                            0/*FW_HEAVY*/,    //粗体
                            0,0,0,        //非斜体，无下划线，无中划线
                            GB2312_CHARSET,    //字符集
                            OUT_DEFAULT_PRECIS,        
                            CLIP_DEFAULT_PRECIS,        
                            DEFAULT_QUALITY,        //一系列的默认值
                            DEFAULT_PITCH | FF_DONTCARE,   
                           (TCHAR*)"楷体"    //字体名称
                        );
	
	static HFONT hFont2 = CreateFont
                        (
                            20,0,    //高度20, 宽取0表示由系统选择最佳
                            0, 0,    //文本倾斜，与字体倾斜都为0
                            /*0*/FW_HEAVY,    //粗体
                            0,0,0,        //非斜体，无下划线，无中划线
                            GB2312_CHARSET,    //字符集
                            OUT_DEFAULT_PRECIS,        
                            CLIP_DEFAULT_PRECIS,        
                            DEFAULT_QUALITY,        //一系列的默认值
                            DEFAULT_PITCH | FF_DONTCARE,   
                           (TCHAR*)"黑体"    //字体名称
                        );

switch(uMsg)
	{
	case WM_CREATE:
		 hdc=GetDC(hWnd);
		 mdc=CreateCompatibleDC(hdc);
		 mdc1=CreateCompatibleDC(hdc);

		fl=1;
		P_W1_1=LoadBitmap(hInstance_1,MAKEINTRESOURCE(IDB_BITMAP6));
		P_W1_2=LoadBitmap(hInstance_1,MAKEINTRESOURCE(IDB_BITMAP7));
		SelectObject(mdc,P_W1_2);
		SelectObject(mdc1,P_W1_1);
		hBtn1=CreateWindow("button","确认",WS_CHILD|WS_BORDER|WS_VISIBLE|BS_PUSHBUTTON,400,200,80,130,hWnd,HMENU(1),hInstance_1,NULL);
		hWrite1=CreateWindow("edit",NULL,WS_CHILD|WS_BORDER|WS_VISIBLE|ES_MULTILINE|ES_UPPERCASE,330,100,150,30,hWnd,HMENU(2),hInstance_1,NULL);
		hWrite2=CreateWindow("edit",NULL,WS_CHILD|WS_BORDER|WS_VISIBLE|ES_MULTILINE|ES_UPPERCASE,330,150,150,30,hWnd,HMENU(3),hInstance_1,NULL);
		hWrite3=CreateWindow("edit",NULL,WS_CHILD|WS_BORDER|WS_VISIBLE|ES_MULTILINE,330,200,50,30,hWnd,HMENU(4),hInstance_1,NULL);
		hWrite4=CreateWindow("edit",NULL,WS_CHILD|WS_BORDER|WS_VISIBLE|ES_MULTILINE,330,250,50,30,hWnd,HMENU(5),hInstance_1,NULL);
		hWrite5=CreateWindow("edit",NULL,WS_CHILD|WS_BORDER|WS_VISIBLE|ES_MULTILINE,330,300,50,30,hWnd,HMENU(6),hInstance_1,NULL);
			 //hFont = CreateFont
    //                    (
    //                        20,0,    //高度20, 宽取0表示由系统选择最佳
    //                        0, 0,    //文本倾斜，与字体倾斜都为0
    //                        0/*FW_HEAVY*/,    //粗体
    //                        0,0,0,        //非斜体，无下划线，无中划线
    //                        GB2312_CHARSET,    //字符集
    //                        OUT_DEFAULT_PRECIS,        
    //                        CLIP_DEFAULT_PRECIS,        
    //                        DEFAULT_QUALITY,        //一系列的默认值
    //                        DEFAULT_PITCH | FF_DONTCARE,   
    //                       (TCHAR*)"楷体"    //字体名称
    //                    );
			 //hFont2 = CreateFont
    //                    (
    //                        20,0,    //高度20, 宽取0表示由系统选择最佳
    //                        0, 0,    //文本倾斜，与字体倾斜都为0
    //                        /*0*/FW_HEAVY,    //粗体
    //                        0,0,0,        //非斜体，无下划线，无中划线
    //                        GB2312_CHARSET,    //字符集
    //                        OUT_DEFAULT_PRECIS,        
    //                        CLIP_DEFAULT_PRECIS,        
    //                        DEFAULT_QUALITY,        //一系列的默认值
    //                        DEFAULT_PITCH | FF_DONTCARE,   
    //                       (TCHAR*)"黑体"    //字体名称
    //                    );
       SendMessage(hWrite1, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));
	   SendMessage(hWrite2, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));
	   SendMessage(hWrite3, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));
	   SendMessage(hWrite4, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));
	   SendMessage(hWrite5, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));
	   SendMessage(hBtn1, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case 1:
			memset(list_x,'\0',20);
			memset(list_y,'\0',20);
			memset(stand[0],'\0',2);
			memset(stand[1],'\0',2);
			memset(stand[2],'\0',2);
			SendMessage(hWrite1, WM_GETTEXT,  (WPARAM)(GetWindowTextLength(hWrite1)+1),(LPARAM)list_x);
			SendMessage(hWrite2, WM_GETTEXT,  (WPARAM)(GetWindowTextLength(hWrite2)+1),(LPARAM)list_y);
			SendMessage(hWrite3, WM_GETTEXT,  (WPARAM)(GetWindowTextLength(hWrite3)+1),(LPARAM)stand[0]);
			SendMessage(hWrite4, WM_GETTEXT,  (WPARAM)(GetWindowTextLength(hWrite4)+1),(LPARAM)stand[1]);
			SendMessage(hWrite5, WM_GETTEXT,  (WPARAM)(GetWindowTextLength(hWrite5)+1),(LPARAM)stand[2]);
			if(!(GetWindowTextLength(hWrite1)&&GetWindowTextLength(hWrite2)&&GetWindowTextLength(hWrite3)&&GetWindowTextLength(hWrite4)
				&&GetWindowTextLength(hWrite5)))
			{
				MessageBox(NULL,"有数据未输入","Tip",MB_OK|MB_SYSTEMMODAL);
				break;
			}
			the_same=atoi(stand[0]);
			the_different=atoi(stand[1]);
			the_empty=atoi(stand[2]);
			lenx=strlen(list_x);//第一组基因序列长度
			leny=strlen(list_y);//第二组基因序列长度
			if(the_empty>0)
			{
				MessageBox(NULL,"空位应小于等于0","error",MB_OK);
				break;
			}
			check();
			ShowWindow(hWnd1,SW_HIDE);
			CreateW2(hWnd2,hInstance_1);
			break;

		}
		break;
	case WM_LBUTTONDOWN:
		 x=LOWORD(lParam);
		 y=HIWORD(lParam);
		if(fl==1&&x>=80&&x<=100&&y>=c_y-115&&y<=c_y-94)
		{
			TransparentBlt(hdc,0,0,c_x/5,c_y,mdc1,0,0,1100/5,702,RGB(255,255,255));
			fl=0;
		}
		break;
	case WM_PAINT:
		BeginPaint(hWnd,&ps);
			TransparentBlt(hdc,0,0,c_x,c_y,mdc1,0,0,1100,702,RGB(255,255,255));
		if(fl==1)
		{
			BitBlt(hdc,0,c_y-94,94,94,mdc,3,3,SRCCOPY);
			SetTextColor(hdc,RGB(0,100,255));
			SelectObject(hdc,hFont1);
			SetBkMode(hdc,TRANSPARENT);
			TextOut(hdc,2,c_y-115,"支持作者",strlen("支持作者"));
			SetTextColor(hdc,RGB(200,200,0));
			SelectObject(hdc,hFont2);
			TextOut(hdc,80,c_y-115,"×",strlen("×"));
			fl=1;
		}
			SelectObject(hdc,hFont);
			SetTextColor(hdc,RGB(0,255,255));
			SetBkMode(hdc,TRANSPARENT);
			TextOut(hdc,c_x/2.0-10*strlen("DNA序列1")-20,c_y/2.0-150+rad,"DNA序列1",strlen("DNA序列1"));
			TextOut(hdc,c_x/2.0-10*strlen("DNA序列2")-20,c_y/2.0-100+rad,"DNA序列2",strlen("DNA序列2"));
			TextOut(hdc,c_x/2.0-10*strlen("匹配")-20,c_y/2.0-50+rad,"匹配",strlen("匹配"));
			TextOut(hdc,c_x/2.0-10*strlen("错配")-20,c_y/2.0+rad,"错配",strlen("错配"));
			TextOut(hdc,c_x/2.0-10*strlen("空位")-20,c_y/2.0+50+rad,"空位",strlen("空位"));
		EndPaint(hWnd,&ps);
		break;
	case WM_SIZE:
		c_x=LOWORD(lParam);
		c_y=HIWORD(lParam);
		MoveWindow(hWrite1,c_x/2.0,c_y/2.0-150,150,30,false);
		MoveWindow(hWrite2,c_x/2.0,c_y/2.0-100,150,30,false);
		MoveWindow(hWrite3,c_x/2.0,c_y/2.0-50,50,30,false);
		MoveWindow(hWrite4,c_x/2.0,c_y/2.0,50,30,false);
		MoveWindow(hWrite5,c_x/2.0,c_y/2.0+50,50,30,false);
		MoveWindow(hBtn1,c_x/2.0+70,c_y/2.0-50,80,130,true);
		break;
	case WM_CLOSE:
			memset(list_x,'\0',20);
			memset(list_y,'\0',20);
			memset(stand[0],'\0',2);
			memset(stand[1],'\0',2);
			memset(stand[2],'\0',2);
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		ShowWindow(hWnd4,SW_SHOW);
		break;
	default:
		return DefWindowProc(hWnd,uMsg,wParam,lParam);
	}
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}
/*比对结果显示*/			LRESULT CALLBACK WinProc2(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static int i2;
	static int c_x,c_y;
	PAINTSTRUCT ps;
	static HDC hdc;
	static HDC mdc;
	static HFONT hFont,hFont1,hFont2;    //字体句柄
	static HDC mdc1;
	static HBITMAP  hEmptyP;
	static int rax,ray;
	RECT Rect={0,0,10000,10000};
	RECT Rect1;
	switch(uMsg)
	{
	case WM_CREATE:
		rax=0;
		ray=0;
		c_x=wWidth2;
		c_y=wHeight2;
		 SetScrollRange(hWnd, SB_HORZ, 0,200, FALSE);
		initMade_me();
		search_back(0,0,lenx,leny);
		if(lenx>=leny)
		{
			MoveWindow(hWnd,(cx-(200+lenx*50+lenx*25*made_i))/2,cy/2-100-25*leny,200+lenx*50+lenx*25*made_i,200+leny*50+20,true);
			/*c_x=200+lenx*50+lenx*25*made_i;
			c_y=200+leny*50;*/
		}
		else
		{
			MoveWindow(hWnd,(cx-(200+lenx*50+lenx*25*made_i))/2,cy/2-100-25*leny,200+lenx*50+leny*25*made_i,200+leny*50+20,true);
			/*c_x=200+lenx*50+leny*25*made_i;
			c_y=200+leny*50;*/
		}
		 hdc=GetDC(hWnd);
		 mdc=CreateCompatibleDC(hdc);
		 mdc1=CreateCompatibleDC(hdc);
		 hEmptyP=(HBITMAP)CreateCompatibleBitmap(mdc1,10000,10000);
		 SelectObject(mdc1,hEmptyP);
		 FillRect(mdc1, &Rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
		 hFont = CreateFont
                        (
                            20,0,    //高度20, 宽取0表示由系统选择最佳
                            0, 0,    //文本倾斜，与字体倾斜都为0
                            0/*FW_HEAVY*/,    //粗体
                            0,0,0,        //非斜体，无下划线，无中划线
                            GB2312_CHARSET,    //字符集
                            OUT_DEFAULT_PRECIS,        
                            CLIP_DEFAULT_PRECIS,        
                            DEFAULT_QUALITY,        //一系列的默认值
                            DEFAULT_PITCH | FF_DONTCARE,   
                           (TCHAR*)"楷体"    //字体名称
                        );
		  hFont1 = CreateFont
                        (
                            30,0,    //高度20, 宽取0表示由系统选择最佳
                            0, 0,    //文本倾斜，与字体倾斜都为0
                            0/*FW_HEAVY*/,    //粗体
                            0,0,0,        //非斜体，无下划线，无中划线
                            GB2312_CHARSET,    //字符集
                            OUT_DEFAULT_PRECIS,        
                            CLIP_DEFAULT_PRECIS,        
                            DEFAULT_QUALITY,        //一系列的默认值
                            DEFAULT_PITCH | FF_DONTCARE,   
                           (TCHAR*)"黑体"    //字体名称
                        );
		  hFont2 = CreateFont
                        (
                            40,0,    //高度20, 宽取0表示由系统选择最佳
                            0, 0,    //文本倾斜，与字体倾斜都为0
                            0/*FW_HEAVY*/,    //粗体
                            0,0,0,        //非斜体，无下划线，无中划线
                            GB2312_CHARSET,    //字符集
                            OUT_DEFAULT_PRECIS,        
                            CLIP_DEFAULT_PRECIS,        
                            DEFAULT_QUALITY,        //一系列的默认值
                            DEFAULT_PITCH | FF_DONTCARE,   
                           (TCHAR*)"隶书"    //字体名称
                        );
		break;
	case WM_PAINT:
		BeginPaint(hWnd,&ps);
		BitBlt(hdc,0,0,c_x,c_y,mdc1,0,0,SRCCOPY);
		 SelectObject(hdc,hFont);
		SetTextColor(hdc,RGB(0,0,255));
		 for(int i=0;i<=lenx;i++)
		 {
			 char b[2];
			 b[0]=list_x[i];
			 b[1]='\0';
			 TextOut(hdc,rax+150+i*50,50,b,strlen(b));
		 }
		  for(int i=0;i<=leny;i++)
		 {
			 char b[2];
			 b[0]=list_y[i];
			 b[1]='\0';
			 TextOut(hdc,rax+50,150+i*50,b,strlen(b));
		 }
		for(int i=0;i<=lenx;i++)
			for(int j=0;j<=leny;j++)
			{
				char b[10];
				int  m=a[j][i].sum;
				/*if(m>=0&&m<10)
				{
				b[0]=m+'0';
				b[1]='\0';
				}
				if(m<0&&m>-10)
				{
				b[0]='-';
				b[1]=(0-m)+'0';
				b[2]='\0';
				}
				if(m<=-10&&m>-100)
				{
				b[0]='-';
				b[1]=(0-a[j][i].sum)/10+'0';
				b[2]=(0-a[j][i].sum)%10+'0';
				b[3]='\0';
				}
				if(m>=10&&m<100)
				{
				b[0]=m/10+'0';
				b[1]=m%10+'0';
				b[2]='\0';
				}*/
				_itoa_s(m,b,10);
				SelectObject(hdc,hFont);
				SetTextColor(hdc,RGB(205,127,50));
				TextOut(hdc,rax+100+i*50,100+j*50,b,strlen(b));
				SetTextColor(hdc,RGB(205,127,255));

				if(a[j][i].last1!=NULL)
					TextOut(hdc,rax+100+i*50-20,100+j*50,"←",strlen("←"));
				if(a[j][i].last2!=NULL)
					TextOut(hdc,rax+100+i*50,100+j*50-20,"↑",strlen("↑"));
				SelectObject(hdc,hFont1);
				if(a[j][i].last3!=NULL)
					TextOut(hdc,rax+100+i*50-30,100+j*50-30,"↖",strlen("↖"));
				
			}
//回溯
			SetTextColor(hdc,RGB(205,0,0));
			for(int i=0;i<made_i;i++)
			{
				for(int j=0;j<40;j++)
				{
					if(made_me[i][j].x!=-1)
					{
						/*char b[4];
						b[0]=made_me[i][j].x+'0';
						b[1]=' ';
						b[2]=made_me[i][j].y+'0';
						b[3]='\0';
						TextOut(hdc,500+i*100,100+j*50,b,strlen(b));*/
						//SetTextColor(hdc,RGB(205,0,0));
						SelectObject(hdc,hFont);
						if(a[made_me[i][j].y][made_me[i][j].x].last1!=NULL)
							TextOut(hdc,rax+100+(made_me[i][j].x)*50-20,100+(made_me[i][j].y)*50,"←",strlen("←"));
						if(a[made_me[i][j].y][made_me[i][j].x].last2!=NULL)
							TextOut(hdc,rax+100+made_me[i][j].x*50,100+made_me[i][j].y*50-20,"↑",strlen("↑"));
						SelectObject(hdc,hFont1);
						if(a[made_me[i][j].y][made_me[i][j].x].last3!=NULL)
							TextOut(hdc,rax+100+made_me[i][j].x*50-30,100+made_me[i][j].y*50-30,"↖",strlen("↖"));
					}
					else
						break;
				}
			}
//
			 SelectObject(hdc,hFont);
			for(int i=0;i<20;i++)
			{
				memset(made_meb1,'\0',20);
				memset(made_meb2,'\0',20);
			}
			for(int i=0;i<made_i;i++)
			{
				int j=0;
				for(j=0;j<40;j++)
				{
					if(made_me[i][j].x==-1)
						break;
				}
				int i_a=0,i_b=0,i_x1=0,i_x2=0;	
						for(int l=j-1;l>0;l--)
							if(made_me[i][l-1].x-made_me[i][l].x==1)
								made_meb1[i][i_a++]=list_x[i_x1++];
							else
								made_meb1[i][i_a++]='-';
						for(int l=j-1;l>0;l--)
							if(made_me[i][l-1].y-made_me[i][l].y==1)
								made_meb2[i][i_b++]=list_y[i_x2++];
							else
								made_meb2[i][i_b++]='-';

						int mm=0;
						if(i_a>=i_b)
						{
							SetTextColor(hdc,RGB(0,0,255));
							
							/*if(200+lenx*50+i*i_a*20+20*strlen(made_meb1[i])<cx)*/
							{
								TextOut(hdc,rax+200+lenx*50+i*i_a*20,100,made_meb1[i],strlen(made_meb1[i]));
								TextOut(hdc,rax+200+lenx*50+i*i_a*20,130,made_meb2[i],strlen(made_meb2[i]));
								//mm=i;
							}
							/*else
							{
								TextOut(hdc,rax+200+lenx*50+(i-mm)*i_a*20,250,made_meb1[i],strlen(made_meb1[i]));
								TextOut(hdc,rax+200+lenx*50+(i-mm)*i_a*20,280,made_meb2[i],strlen(made_meb2[i]));
							}*/
						}
						else if(i_a<i_b)
						{
							SetTextColor(hdc,RGB(0,0,255));
							//if(200+lenx*50+i*i_b*20+20*strlen(made_meb1[i])<cx)
							{
								TextOut(hdc,rax+200+lenx*50+i*i_b*20,100,made_meb1[i],strlen(made_meb1[i]));
								TextOut(hdc,rax+200+lenx*50+i*i_b*20,130,made_meb2[i],strlen(made_meb2[i]));
								//mm=i;
							}
						/*	else
							{
								TextOut(hdc,rax+200+lenx*50+(i-mm)*i_b*20,250,made_meb1[i],strlen(made_meb1[i]));
								TextOut(hdc,rax+200+lenx*50+(i-mm)*i_b*20,280,made_meb2[i],strlen(made_meb2[i]));
							}*/
						}
			}
			
//画线
		for(int i=0;i<=lenx+2;i++)
		{
			line(hdc,rax+45+i*50,45,rax+45+i*50,45+leny*50+100);
		}
		for(int i=0;i<=leny+2;i++)
		{
			line(hdc,rax+45,45+i*50,rax+45+lenx*50+100,45+i*50);
		}
		EndPaint(hWnd,&ps);
		break;
	case WM_SIZE:
		c_x=LOWORD(lParam);
		c_y=HIWORD(lParam);
			break;
	case WM_HSCROLL:
		switch (LOWORD(wParam))
		{
		case SB_THUMBPOSITION:
				i2=HIWORD(wParam);
				SetScrollPos(hWnd, SB_HORZ,i2, TRUE);
				Rect1.bottom=c_y;
				Rect1.right=c_x;
				Rect1.left=0;
				Rect1.top=0;
				//ray=80-100*i1;
				rax=-30*i2;
				InvalidateRect(hWnd,&Rect1,true);
				break;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		ShowWindow(hWnd1,SW_SHOW);
		break;
	default:
		return DefWindowProc(hWnd,uMsg,wParam,lParam);
	}
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}
/*初始化窗口*/				LRESULT CALLBACK WinProc3(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	HINSTANCE hInstance;
	static HBITMAP p1;
	PAINTSTRUCT ps;
	static HDC hdc;//=GetDC(hWnd);
	static HDC mdc;//=CreateCompatibleDC(hdc);
	static HDC bdc;//=CreateCompatibleDC(hdc);
	HFONT hFont,hFont1;    //字体句柄
	RECT Rect;
	switch(uMsg)
	{
	case WM_CREATE:
		/*Sleep(2000);
		SendMessage(hWnd,WM_CLOSE,0,0);*/
		hdc=GetDC(hWnd);
		mdc=CreateCompatibleDC(hdc);
		bdc=CreateCompatibleDC(hdc);
		hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
		p1=LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP1));
		SelectObject(mdc,p1);
		break;
	case WM_PAINT:
		BeginPaint(hWnd,&ps);
		//mdc=CreateCompatibleDC(hdc);
	//bdc=CreateCompatibleDC(hdc);
	// bmp = CreateCompatibleBitmap(hdc,399,600);
	//SelectObject(mdc,bmp);
	//加载图片
	//p1 = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP1));
		/*p1=(HBITMAP)LoadImage(NULL,"./res/02.bmp",IMAGE_BITMAP,wWidth0,
			wHeight0,LR_LOADFROMFILE);*/
	/*hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
		p1=LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP2));
	SelectObject(mdc,p1);*/
	SelectObject(bdc,p1);
	hFont = CreateFont
                        (
                            10,0,    //高度20, 宽取0表示由系统选择最佳
                            0, 0,    //文本倾斜，与字体倾斜都为0
                            0/*FW_HEAVY*/,    //粗体
                            0,0,0,        //非斜体，无下划线，无中划线
                            GB2312_CHARSET,    //字符集
                            OUT_DEFAULT_PRECIS,        
                            CLIP_DEFAULT_PRECIS,        
                            DEFAULT_QUALITY,        //一系列的默认值
                            DEFAULT_PITCH | FF_DONTCARE,   
                           (TCHAR*)"楷体"    //字体名称
                        );
	hFont1 = CreateFont
                        (
                            12,0,    //高度20, 宽取0表示由系统选择最佳
                            0, 0,    //文本倾斜，与字体倾斜都为0
                            0/*FW_HEAVY*/,    //粗体
                            0,0,0,        //非斜体，无下划线，无中划线
                            GB2312_CHARSET,    //字符集
                            OUT_DEFAULT_PRECIS,        
                            CLIP_DEFAULT_PRECIS,        
                            DEFAULT_QUALITY,        //一系列的默认值
                            DEFAULT_PITCH | FF_DONTCARE,   
                           (TCHAR*)"楷体"    //字体名称
                        );
	/*SelectObject(mdc,hFont);
	SetTextColor(mdc,RGB(205,127,50));
	SetBkMode(mdc,TRANSPARENT);
	TextOut(mdc,10,20,"Version:1.0.0.0",strlen("version:1.0.0.0"));
	TextOut(mdc,10,40,"Author:DC.wang",strlen("Author:DC.wang"));
	SelectObject(mdc,hFont1);
	TextOut(mdc,8,60,"保留所有权利",strlen("保留所有权利"));
	Rect.bottom=100;
	Rect.left=0;
	Rect.right=150;
	Rect.top=80;*/
	//BitBlt(hdc,0,0,wWidth0,wHeight0,mdc,0,0,SRCCOPY);
	//StretchBlt(hdc,0,0,wWidth0,wHeight0,mdc,0,0,1024,691,SRCCOPY);
	TransparentBlt(hdc,0,0,wWidth0,wHeight0,mdc,0,0,1024,691,RGB(255,255,255));
	SelectObject(hdc,hFont);
	SetTextColor(hdc,RGB(205,127,50));
	SetBkMode(hdc,TRANSPARENT);
	TextOut(hdc,10,20,"Version:1.0.0.0",strlen("version:1.0.0.0"));
	TextOut(hdc,10,40,"Author:DC.wang",strlen("Author:DC.wang"));
	SelectObject(hdc,hFont1);
	TextOut(hdc,8,60,"保留所有权利",strlen("保留所有权利"));
	/*SelectObject(hdc,hFont);
	SetTextColor(hdc,RGB(205,127,50));
	SetBkMode(hdc,TRANSPARENT);*/
	//for(int i=0;i<5;i++)
	//{
	//	
	//	InvalidateRect(hWnd,&Rect,true);
	//	//BitBlt(mdc,0,0,1024,691,bdc,0,0,SRCCOPY);
	//	//BitBlt(hdc,0,80,20,150,bdc,0,0,SRCCOPY);
	//	//TextOut(hdc,8,80,"加载中.",strlen("加载中."));
	//	//BitBlt(hdc,0,80,20,150,bdc,0,0,SRCCOPY);
	//	UpdateWindow(hWnd);
	//	Sleep(1000);
	//	InvalidateRect(hWnd,&Rect,true);
	//	//BitBlt(mdc,0,0,1024,691,bdc,0,0,SRCCOPY);
	//	//BitBlt(hdc,0,80,20,150,bdc,0,0,SRCCOPY);
	//	//TextOut(hdc,8,80,"加载中..",strlen("加载中.."));
	//	UpdateWindow(hWnd);
	//	Sleep(1000);
	//	InvalidateRect(hWnd,&Rect,true);
	//	//BitBlt(mdc,0,0,1024,691,bdc,0,0,SRCCOPY);
	//	BitBlt(hdc,0,80,20,150,bdc,0,0,SRCCOPY);
	//	TextOut(hdc,8,80,"加载中...",strlen("加载中..."));
	//	UpdateWindow(hWnd);
	//	Sleep(1000);
	//	InvalidateRect(hWnd,&Rect,true);
	//	//BitBlt(mdc,0,0,1024,691,bdc,0,0,SRCCOPY);
	//	BitBlt(hdc,0,80,20,150,bdc,0,0,SRCCOPY);
	//	TextOut(hdc,8,80,"加载中....",strlen("加载中...."));
	//	UpdateWindow(hWnd);
	//	Sleep(1000);
	//}
		EndPaint(hWnd,&ps);
		/*Sleep(2000);
		SendMessage(hWnd,WM_CLOSE,0,0);*/
		CreateThread(NULL,0,fun_time,NULL,0,NULL);
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		DeleteObject(p1);
		DeleteDC(mdc);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd,uMsg,wParam,lParam);
	}
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}
/*主菜单窗口*/				LRESULT CALLBACK WinProc4(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	HINSTANCE hInstance;
	int rat=50;//80;//微调
	//int rat_y=20;
	int _x,_y;
	RECT Rect;
	PAINTSTRUCT ps;
	static HDC hdc=GetDC(hWnd);
	static HDC mdc=CreateCompatibleDC(hdc);
	static HDC mdc1=CreateCompatibleDC(hdc);
	static int f_1=0,f_2=0,f_3=0,f_4=0;
	static HFONT hFont= CreateFont
                        (
                            30,0,    //高度20, 宽取0表示由系统选择最佳
                            0, 0,    //文本倾斜，与字体倾斜都为0
                            /*0*/FW_HEAVY,    //粗体
                            0,0,0,        //非斜体，无下划线，无中划线
                            GB2312_CHARSET,    //字符集
                            OUT_DEFAULT_PRECIS,        
                            CLIP_DEFAULT_PRECIS,        
                            DEFAULT_QUALITY,        //一系列的默认值
                            DEFAULT_PITCH | FF_DONTCARE,   
                           (TCHAR*)"隶书"    //字体名称
                        );
	static HFONT hFont1= CreateFont
                        (
                            40,0,    //高度20, 宽取0表示由系统选择最佳
                            0, 0,    //文本倾斜，与字体倾斜都为0
                            /*0*/FW_HEAVY,    //粗体
                            0,0,0,        //非斜体，无下划线，无中划线
                            GB2312_CHARSET,    //字符集
                            OUT_DEFAULT_PRECIS,        
                            CLIP_DEFAULT_PRECIS,        
                            DEFAULT_QUALITY,        //一系列的默认值
                            DEFAULT_PITCH | FF_DONTCARE,   
                           (TCHAR*)"隶书"    //字体名称
                        );
	switch(uMsg)
	{
	case WM_CREATE:
		//SelectObject(mdc3,hEmptyP);
		/*P_W4_1=(HBITMAP)LoadImage(NULL,"./res/BK_2.bmp",IMAGE_BITMAP,wWidth4,
			wHeight4,LR_LOADFROMFILE);*/
		/*P_W4_1=(HBITMAP)LoadImage(NULL,"./res/bbk3.bmp",IMAGE_BITMAP,1024,
			640,LR_LOADFROMFILE);*/
		hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
		P_W4_1=LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP2));
		SelectObject(mdc1,P_W4_1);
		hMenu = CreatePopupMenu(); 
		AppendMenu(hMenu,MFT_STRING,IDC_CTREE,"发生树");
		AppendMenu(hMenu,MFT_STRING,IDC_PROTEIN,"蛋白质序列比对");
		AppendMenu(hMenu,MFT_STRING,IDC_DNA,"DNA序列比对");
		AppendMenu(hMenu,MFT_STRING,IDC_QUIT,"退出");
		//SetScrollRange(hWnd, SB_VERT, 0,cy, FALSE);
		//设置 TRUE 滚动条立即重绘 这样才能看到新位置
		//SetScrollPos(hWnd, SB_VERT,1000, TRUE);
		break;
	case WM_RBUTTONDOWN:
			GetWindowRect(hWnd,&Rect);
			TrackPopupMenu(hMenu,TPM_LEFTALIGN,Rect.left,Rect.top,0,hWnd,&Rect);
		break;
	case WM_LBUTTONDOWN:
		if(f_1==1)
		{
			ShowWindow(hWnd4,SW_HIDE);
			CreateW5(&hWnd5,hInstance_1);
		}
		else if(f_2==1)
		{
			ShowWindow(hWnd4,SW_HIDE);
			CreateW1(&hWnd7,hInstance_1);
		}
		else if(f_3==1)
		{
			ShowWindow(hWnd4,SW_HIDE);
			CreateW7(&hWnd1,hInstance_1);
		}
		else if(f_4==1)
			SendMessage(hWnd,WM_CLOSE,0,0);
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_CTREE:
			ShowWindow(hWnd4,SW_HIDE);
			CreateW5(&hWnd5,hInstance_1);
			break;
		case IDC_DNA:
			ShowWindow(hWnd4,SW_HIDE);
			CreateW1(&hWnd7,hInstance_1);
			break;
		case IDC_PROTEIN:
			ShowWindow(hWnd4,SW_HIDE);
			CreateW7(&hWnd1,hInstance_1);
			break;
		case IDC_QUIT:
			SendMessage(hWnd,WM_CLOSE,0,0);
			break;
		}
		break;
	case WM_PAINT:
		BeginPaint(hWnd,&ps);
//加载图片
		BitBlt(hdc,0,0,wWidth4,wHeight4,mdc1,0,0,SRCCOPY);
		//TransparentBlt(hdc,20,20,wWidth4/2,wHeight4/2,mdc2,0,0,wWidth4,wHeight4,RGB(255,255,255));
//文字
		
		/*SetTextColor(hdc,RGB(200,200,0));*/
		SetTextColor(hdc,RGB(192,192,192));
		SelectObject(hdc,hFont);
		SetBkMode(hdc,TRANSPARENT);
		TextOut(hdc,wWidth4/2.0+rat,wHeight4/2.0-100,"系统发生树",strlen("系统发生树"));
		TextOut(hdc,wWidth4/2.0+rat,wHeight4/2.0-50,"DNA双序列比对",strlen("DNA双序列比对"));
		TextOut(hdc,wWidth4/2.0+rat,wHeight4/2.0,"蛋白质双序列比对",strlen("蛋白质双序列比对"));
		SetTextColor(hdc,RGB(100,50,0));
		TextOut(hdc,wWidth4-30,0,"×",strlen("×"));
		
		/*FillRect(mdc3, &Rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
		SelectObject(mdc3, (HPEN)GetStockObject(BLACK_PEN));
		drawtree(mdc3,10,10,100,200,300);
		BitBlt(hdc,0,0,500,500,mdc3,0,0,SRCCOPY);*/
		EndPaint(hWnd,&ps);
		break;
	case WM_MOUSEMOVE:
		_x=LOWORD(lParam);
		_y=HIWORD(lParam);
//1
		if(f_1==0&&_x>wWidth4/2.0+rat&&_x<wWidth4/2.0+rat+15*(strlen("系统发生树")+1)&&_y>wHeight4/2.0-100&&_y<wHeight4/2.0-100+50)
		{
		BitBlt(hdc,wWidth4/2.0+rat,wHeight4/2.0-100,15*(strlen("系统发生树")+1),50,mdc1,wWidth4/2.0+rat,wHeight4/2.0-100,SRCCOPY);
		//SetTextColor(hdc,RGB(200,0,0));
		SetTextColor(hdc,RGB(230,200,200));
		SelectObject(hdc,hFont1);
		SetBkMode(hdc,TRANSPARENT);
		TextOut(hdc,wWidth4/2.0+rat,wHeight4/2.0-100,"系统发生树",strlen("系统发生树"));
		f_1=1;
		}
		if(f_1==1&&!(_x>wWidth4/2.0+rat&&_x<wWidth4/2.0+rat+15*(strlen("系统发生树")+1)&&_y>wHeight4/2.0-100&&_y<wHeight4/2.0-100+50))
		{
		BitBlt(hdc,wWidth4/2.0+rat,wHeight4/2.0-100,20*(strlen("系统发生树")+1),50,mdc1,wWidth4/2.0+rat,wHeight4/2.0-100,SRCCOPY);
		SetTextColor(hdc,RGB(192,192,192));
		//SetTextColor(hdc,RGB(0,200,0));
		SelectObject(hdc,hFont);
		SetBkMode(hdc,TRANSPARENT);
		TextOut(hdc,wWidth4/2.0+rat,wHeight4/2.0-100,"系统发生树",strlen("系统发生树"));
		f_1=0;
		}
//2
		if(f_2==0&&_x>wWidth4/2.0+rat&&_x<wWidth4/2.0+rat+15*(strlen("DNA双序列比对")+1)&&_y>wHeight4/2.0-50&&_y<wHeight4/2.0-50+50)
		{
		BitBlt(hdc,wWidth4/2.0+rat,wHeight4/2.0-100+50,15*(strlen("DNA双序列比对")+1),50,mdc1,wWidth4/2.0+rat,wHeight4/2.0-100+50,SRCCOPY);
		//SetTextColor(hdc,RGB(200,0,0));
		SetTextColor(hdc,RGB(230,200,200));
		SelectObject(hdc,hFont1);
		SetBkMode(hdc,TRANSPARENT);
		TextOut(hdc,wWidth4/2.0+rat,wHeight4/2.0-100+50,"DNA双序列比对",strlen("DNA双序列比对"));
		f_2=1;
		}
		if(f_2==1&&!(_x>wWidth4/2.0+rat&&_x<wWidth4/2.0+rat+15*(strlen("DNA双序列比对")+1)&&_y>wHeight4/2.0-50&&_y<wHeight4/2.0-50+50))
		{
		BitBlt(hdc,wWidth4/2.0+rat,wHeight4/2.0-100+50,20*(strlen("DNA双序列比对")+1),50,mdc1,wWidth4/2.0+rat,wHeight4/2.0-100+50,SRCCOPY);
		//SetTextColor(hdc,RGB(200,200,0));
		//SetTextColor(hdc,RGB(0,200,0));
		SetTextColor(hdc,RGB(192,192,192));
		SelectObject(hdc,hFont);
		SetBkMode(hdc,TRANSPARENT);
		TextOut(hdc,wWidth4/2.0+rat,wHeight4/2.0-100+50,"DNA双序列比对",strlen("DNA双序列比对"));
		f_2=0;
		}
//3
			if(f_3==0&&_x>wWidth4/2.0+rat&&_x<wWidth4/2.0+rat+15*(strlen("蛋白质双序列比对")+1)&&_y>wHeight4/2.0-100+100&&_y<wHeight4/2.0-100+50+100)
		{
		BitBlt(hdc,wWidth4/2.0+rat,wHeight4/2.0-100+100,15*(strlen("蛋白质双序列比对")+1),50,mdc1,wWidth4/2.0+rat,wHeight4/2.0-100+100,SRCCOPY);
		//SetTextColor(hdc,RGB(200,0,0));
		SetTextColor(hdc,RGB(230,200,200));
		SelectObject(hdc,hFont1);
		SetBkMode(hdc,TRANSPARENT);
		TextOut(hdc,wWidth4/2.0+rat,wHeight4/2.0-100+100,"蛋白质双序列比对",strlen("蛋白质双序列比对"));
		f_3=1;
		}
		if(f_3==1&&!(_x>wWidth4/2.0+rat&&_x<wWidth4/2.0+rat+15*(strlen("蛋白质双序列比对")+1)&&_y>wHeight4/2.0-100+100&&_y<wHeight4/2.0-100+50+100))
		{
		BitBlt(hdc,wWidth4/2.0+rat,wHeight4/2.0-100+100,20*(strlen("蛋白质双序列比对")+1),50,mdc1,wWidth4/2.0+rat,wHeight4/2.0-100+100,SRCCOPY);
		//SetTextColor(hdc,RGB(200,200,0));
		SetTextColor(hdc,RGB(192,192,192));
		SelectObject(hdc,hFont);
		SetBkMode(hdc,TRANSPARENT);
		TextOut(hdc,wWidth4/2.0+rat,wHeight4/2.0-100+100,"蛋白质双序列比对",strlen("蛋白质双序列比对"));
		f_3=0;
		}
//关闭
		if(f_4==0&&_x>wWidth4-30&&_x<wWidth4&&_y>0&&_y<30)
		{
		BitBlt(hdc,wWidth4-30,0,30,30,mdc1,wWidth4-30,0,SRCCOPY);
		SetTextColor(hdc,RGB(200,0,0));
		SelectObject(hdc,hFont);
		SetBkMode(hdc,TRANSPARENT);
		TextOut(hdc,wWidth4-30,0,"×",strlen("×"));
		f_4=1;
		}
		if(f_4==1&&!(_x>wWidth4-30&&_x<wWidth4&&_y>0&&_y<30))
		{
		BitBlt(hdc,wWidth4-30,0,30,30,mdc1,wWidth4-30,0,SRCCOPY);
		//SetTextColor(hdc,RGB(200,200,0));
		SetTextColor(hdc,RGB(100,50,0));
		SelectObject(hdc,hFont);
		SetBkMode(hdc,TRANSPARENT);
		TextOut(hdc,wWidth4-30,0,"×",strlen("×"));
		f_4=0;
		}
		break;
	case WM_CLOSE:
		DestroyMenu(hMenu);
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd,uMsg,wParam,lParam);
	}
	return DefWindowProc(hWnd,uMsg,wParam,lParam);


}
/*建树输入窗口*/			LRESULT CALLBACK WinProc5(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	//static int Edit_x1=50;			//edit 左侧坐标
	//static int Edit_y1=20;			//edit 上侧坐标
	//static int Edit_x2;			//edit 右侧坐标
	//static int Edit_y2;			//edit 下侧坐标
	//static int Box_Length=25;		//方格边长
	//static int Box_Line=10;		//方格行数
	static int fl_clear=0;
	static int fl_create=1;
	static HDC hdc,mdc1,mdc2,mdc3;
	static HWND hBtn2;
	PAINTSTRUCT ps;
	static int c_x,c_y;
	static int x1;
	static int y2;
	static char buff_turn[4];
	static HFONT hFont = CreateFont
                        (
                            20,0,    //高度20, 宽取0表示由系统选择最佳
                            0, 0,    //文本倾斜，与字体倾斜都为0
                            0/*FW_HEAVY*/,    //粗体
                            0,0,0,        //非斜体，无下划线，无中划线
                            GB2312_CHARSET,    //字符集
                            OUT_DEFAULT_PRECIS,        
                            CLIP_DEFAULT_PRECIS,        
                            DEFAULT_QUALITY,        //一系列的默认值
                            DEFAULT_PITCH | FF_DONTCARE,   
                           (TCHAR*)"楷体"    //字体名称
                        );
	static HFONT hFont1 = CreateFont
                        (
                            25,0,    //高度20, 宽取0表示由系统选择最佳
                            0, 0,    //文本倾斜，与字体倾斜都为0
                            0/*FW_HEAVY*/,    //粗体
                            0,0,0,        //非斜体，无下划线，无中划线
                            GB2312_CHARSET,    //字符集
                            OUT_DEFAULT_PRECIS,        
                            CLIP_DEFAULT_PRECIS,        
                            DEFAULT_QUALITY,        //一系列的默认值
                            DEFAULT_PITCH | FF_DONTCARE,   
                           (TCHAR*)"楷体"    //字体名称
                        );
	switch(uMsg)
	{
	case WM_CREATE:
//初始化
		c_x=wWidth5;
		c_y=wHeight5;
		hdc=GetDC(hWnd);
		mdc1=CreateCompatibleDC(hdc);
		mdc2=CreateCompatibleDC(hdc);
		mdc3=CreateCompatibleDC(hdc);
		box_x=0;
//载入图片
		//P_W5_1=(HBITMAP)LoadImage(NULL,"./res/BK_2.bmp",IMAGE_BITMAP,1920,1080,LR_LOADFROMFILE);
		P_W5_1=LoadBitmap(hInstance_1,MAKEINTRESOURCE(IDB_BITMAP3));
		SelectObject(mdc1,P_W5_1);
		for(int i=0;i<Box_Line;i++)
			for(int j=0;j<Box_Line;j++)
			{
				if(i==j)
					hWrite[i][j]=CreateWindow("edit",NULL,WS_CHILD|WS_BORDER|WS_VISIBLE|ES_MULTILINE|ES_READONLY|ES_UPPERCASE,
						Edit_x1+i*Box_Length,Edit_y1+j*Box_Length,Box_Length,Box_Length,hWnd,HMENU(666),hInstance_1,NULL);
				else
					hWrite[i][j]=CreateWindow("edit",NULL,WS_CHILD|WS_BORDER|WS_VISIBLE|ES_MULTILINE|ES_UPPERCASE,
						Edit_x1+i*Box_Length,Edit_y1+j*Box_Length,Box_Length,Box_Length,hWnd,HMENU(666),hInstance_1,NULL);
				
			}
			for(int i=1;i<=Box_Line;i++)
			{
				char InitBuff[2];
				InitBuff[0]='A'+i-1;
				InitBuff[1]='\0';
				SetWindowText(hWrite[0][i],InitBuff);
				SetWindowText(hWrite[i][0],InitBuff);
			}
			Edit_x2=Edit_x1+Box_Line*Box_Length;
			Edit_y2=Edit_y1+Box_Line*Box_Length;
		hBtn1=CreateWindow("button","关闭自动填充",WS_CHILD|WS_BORDER|WS_VISIBLE|BS_PUSHBUTTON,20,50,130,30,hWnd,HMENU(1),hInstance_1,NULL);
		hBtn2=CreateWindow("button","确认输入",WS_CHILD|WS_BORDER|WS_VISIBLE|BS_PUSHBUTTON,20,100,130,30,hWnd,HMENU(2),hInstance_1,NULL);
		//hWrite1=CreateWindow("edit",NULL,WS_CHILD/*|WS_BORDER*/|WS_VISIBLE|ES_MULTILINE|ES_UPPERCASE,100,100,150,100,hWnd,HMENU(2),hInstance_1,NULL);
		//hWrite2=CreateWindow("edit",NULL,WS_CHILD/*|WS_BORDER*/|WS_VISIBLE|ES_MULTILINE|ES_UPPERCASE,300,100,150,100,hWnd,HMENU(3),hInstance_1,NULL);
		
       SendMessage(hWrite1, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));
	   SendMessage(hWrite2, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));
	   SendMessage(hBtn1, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));
	    SendMessage(hBtn2, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));
		break;
	case WM_LBUTTONDOWN:
		/*hdc=GetDC(hWnd);
		MoveToEx(hdc,100,100,NULL);
		LineTo(hdc,200,200);
		ReleaseDC(hWnd,hdc);*/
		x=LOWORD(lParam);
		y=HIWORD(lParam);
		if(x>=Edit_x2-10&&x<=Edit_x2+10&&y>=Edit_y2-10&&y<=Edit_y2+10)
		{
			for(int i=0;i<Box_Line;i++)
			{
				hWrite[i][Box_Line]=CreateWindow("edit",NULL,WS_CHILD|WS_BORDER|WS_VISIBLE|ES_MULTILINE|ES_UPPERCASE,
						Edit_x1+i*Box_Length,Edit_y1+Box_Line*Box_Length,Box_Length,Box_Length,hWnd,HMENU(666),hInstance_1,NULL);
			}
			for(int i=0;i<Box_Line;i++)
			{
				hWrite[Box_Line][i]=CreateWindow("edit",NULL,WS_CHILD|WS_BORDER|WS_VISIBLE|ES_MULTILINE|ES_UPPERCASE,
						Edit_x1+Box_Line*Box_Length,Edit_y1+i*Box_Length,Box_Length,Box_Length,hWnd,HMENU(666),hInstance_1,NULL);
			}
			hWrite[Box_Line][Box_Line]=CreateWindow("edit",NULL,WS_CHILD|WS_BORDER|WS_VISIBLE|ES_MULTILINE|ES_READONLY|ES_UPPERCASE,
						Edit_x1+Box_Line*Box_Length,Edit_y1+Box_Line*Box_Length,Box_Length,Box_Length,hWnd,HMENU(666),hInstance_1,NULL);
			if(fl_clear==0)
			{
			for(int i=1;i<=Box_Line;i++)
			{
				char InitBuff[2];
				InitBuff[0]='A'+i-1;
				InitBuff[1]='\0';
				SetWindowText(hWrite[0][i],InitBuff);
				SetWindowText(hWrite[i][0],InitBuff);
			}
			}
			Box_Line+=1;
			Edit_x2=Edit_x1+Box_Line*Box_Length;
			Edit_y2=Edit_y1+Box_Line*Box_Length;
			for(int i=0;i<Box_Line;i++)
			for(int j=0;j<Box_Line;j++){
				ShowWindow(hWrite[i][j],SW_SHOW);
				UpdateWindow(hWrite[i][j]);}
			ShowWindow(hBtn1,SW_SHOW);
			ShowWindow(hBtn2,SW_SHOW);
			UpdateWindow(hBtn1);
			UpdateWindow(hBtn2);
			TextOut(hdc,Edit_x2-5,Edit_y2-10,"+",strlen("+"));
		}
		break;
	case WM_PAINT:
		BeginPaint(hWnd,&ps);
		StretchBlt(hdc,0,0,c_x,c_y,mdc1,0,0,1366/*1920*/,/*1120*//*768*/786,SRCCOPY);
		SelectObject(hdc,hFont1);
		SetTextColor(hdc,RGB(0,255,0));
		SetBkMode(hdc,TRANSPARENT);
		TextOut(hdc,Edit_x2-5,Edit_y2-10,"+",strlen("+"));
		EndPaint(hWnd,&ps);
		break;
	case WM_SIZE:
		c_x=LOWORD(lParam);
		c_y=HIWORD(lParam);

		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case 1:
			if(fl_clear==0)
			{
			for(int i=1;i<=Box_Line;i++)
			{
				char InitBuff[2];
				InitBuff[0]='A'+i-1;
				InitBuff[1]='\0';
				SetWindowText(hWrite[0][i],NULL);
				SetWindowText(hWrite[i][0],NULL);
			}
			fl_clear=1;
			SetWindowText(hBtn1,"开启自动填充");
			}
			else if(fl_clear==1)
			{
				for(int i=1;i<=Box_Line;i++)
				{
					char InitBuff[2];
					InitBuff[0]='A'+i-1;
					InitBuff[1]='\0';
					SetWindowText(hWrite[0][i],InitBuff);
					SetWindowText(hWrite[i][0],InitBuff);
				}
				fl_clear=0;
				SetWindowText(hBtn1,"关闭自动填充");
			}
			break;
		case 2:
			box_x=0;
			for(int i=0;i<100;i++)
				ZeroMemory(box_head_buff[i],2);
			for(int i=0;i<98;i++)
				for(int j=0;j<98;j++)
					ZeroMemory(box_buff[i][j],4);
			for(int i=1;i<=Box_Line;i++)
			{
				//SendMessage(hWrite[0][i], WM_GETTEXT,  2/*(WPARAM)(GetWindowTextLength(hWrite[0][i])+1)*/,(LPARAM)box_head_buff[i-1]);
				char buff_turn[4];
				ZeroMemory(buff_turn,4);
				if(GetWindowTextLength(hWrite[0][i])==0&&GetWindowTextLength(hWrite[i][0])==0)
					break;
				else
				{
					GetWindowText(hWrite[0][i],box_head_buff[i-1],(GetWindowTextLength(hWrite[0][i])+1));
					GetWindowText(hWrite[i][0],buff_turn,(GetWindowTextLength(hWrite[i][0])+1));
					if(strcmp(buff_turn,box_head_buff[i-1])!=0)
					{
					MessageBox(NULL,"横纵坐标输入不一致","error",MB_OK);
					break;
					}
					box_x++;
				}
			}
			if(box_x==0)
			{
				MessageBox(NULL,"未输入横纵坐标","error",MB_OK);
				break;
			}
			/*for(int i=1;i<box_x;i++)
				for(int j=i+1;j<=box_x;j++)
				{
					GetWindowText(hWrite[j][i],box_buff[i][j],(GetWindowTextLength(hWrite[j][i])+1));
					MessageBox(NULL,box_buff[i][j],"66",MB_OK);
				}
			for(int i=1;i<box_x;i++)
				for(int j=1+i;j<=box_x;j++)
				{
					char buff_turn[4];
					ZeroMemory(buff_turn,4);
					if(GetWindowTextLength(hWrite[i][j])==0)
						continue;
					else
					{
						GetWindowText(hWrite[i][j],buff_turn,(GetWindowTextLength(hWrite[i][j])+1));
						if(strcmp(buff_turn,box_buff[j][i])==0)
						{

						}
					}
				}*/

			/*for(int i=1;i<=box_x;i++)
				for(int j=1;j<=box_x;j++)
				{
					if(GetWindowTextLength(hWrite[i][j])!=0)
					{
						GetWindowText(hWrite[i][j],box_buff[i][j],(GetWindowTextLength(hWrite[i][j])+1));
					}
					else
						continue;
				}*/

			for(int i=2;i<=box_x;i++)
			{
				int flag=0;
				for(int j=1;j<i;j++)
			/*for(int i=1;i<=box_x;i++)
			{
				int flag=0;
				for(int j=0;j<i;j++)*/
				{
					if(GetWindowTextLength(hWrite[j][i])!=0&&GetWindowTextLength(hWrite[i][j])!=0)
					{
					ZeroMemory(buff_turn,4);
					GetWindowText(hWrite[j][i],buff_turn,(GetWindowTextLength(hWrite[j][i])+1));
					GetWindowText(hWrite[i][j],box_buff[i][j],(GetWindowTextLength(hWrite[i][j])+1));
					if(strcmp(box_buff[i][j],buff_turn)!=0)
					{
						MessageBox(NULL,"三角阵对应位置要输入相同数据\n可只输入上三角阵或下三角阵","error",MB_OK|MB_SYSTEMMODAL);
						flag=1;
						break;
					}
					}
					if(GetWindowTextLength(hWrite[j][i])==0&&GetWindowTextLength(hWrite[i][j])!=0)
					{
						GetWindowText(hWrite[i][j],box_buff[i][j],(GetWindowTextLength(hWrite[i][j])+1));
					}
					if(GetWindowTextLength(hWrite[j][i])!=0&&GetWindowTextLength(hWrite[i][j])==0)
					{
						GetWindowText(hWrite[j][i],box_buff[i][j],(GetWindowTextLength(hWrite[j][i])+1));
					}
					if(GetWindowTextLength(hWrite[j][i])==0&&GetWindowTextLength(hWrite[i][j])==0)
					{
						MessageBox(NULL,"数据不完全","error",MB_OK|MB_SYSTEMMODAL);
						flag=1;
						break;
					}
				}
				if(flag==1)
				{
					fl_create=0;
						break;
				}
			}
			if(fl_create==1)
			{
				for(int i=0;i<98;i++)
					for(int j=0;j<98;j++)
						box_num[i][j]=-1;
				for(int i=2;i<=box_x;i++)
				for(int j=1;j<i;j++)
					box_num[j-1][i-1]=box_num[i-1][j-1]=atoi(box_buff[i][j]);
				/**************/
				for(int j=0;j<box_x;j++)
					strcpy_s(box_Many[0].box_head[j],box_head_buff[j]);
				//MessageBox(NULL,box_Many[0].box_head[1],"666",MB_OK);

				CreateW6(&hWnd6,hInstance_1);
			}
			break;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		Box_Line=num_box;		//方格行数
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		ShowWindow(hWnd4,SW_SHOW);
		break;
	default:
		return DefWindowProc(hWnd,uMsg,wParam,lParam);
	}
	return DefWindowProc(hWnd,uMsg,wParam,lParam);


}
/*建树输出窗口*/			LRESULT CALLBACK WinProc6(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static int i1,i2=0;
	SCROLLINFO si;
	int _x,_y;
	static int rax,ray;
	static int c_x=wWidth6,c_y=wHeight6;
	static int fl_b=0;
	static int rat=0/*word_x*3*0*/,rat1=0;
	PAINTSTRUCT ps;
	static HBITMAP hEmptyP2;
	static HDC hdc,mdc2;
	static RECT Rect={0,0,10000,10000};
	static RECT Rect1 = { 100,100,10000,10000 };
	static HFONT hFont = CreateFont
                        (
                            20,0,    //高度20, 宽取0表示由系统选择最佳
                            0, 0,    //文本倾斜，与字体倾斜都为0
                            0/*FW_HEAVY*/,    //粗体
                            0,0,0,        //非斜体，无下划线，无中划线
                            GB2312_CHARSET,    //字符集
                            OUT_DEFAULT_PRECIS,        
                            CLIP_DEFAULT_PRECIS,        
                            DEFAULT_QUALITY,        //一系列的默认值
                            DEFAULT_PITCH | FF_DONTCARE,   
                           (TCHAR*)"楷体"    //字体名称
                        );
	static HFONT hFont1 = CreateFont
                        (
                            40,0,    //高度20, 宽取0表示由系统选择最佳
                            0, 0,    //文本倾斜，与字体倾斜都为0
                            0/*FW_HEAVY*/,    //粗体
                            0,0,0,        //非斜体，无下划线，无中划线
                            GB2312_CHARSET,    //字符集
                            OUT_DEFAULT_PRECIS,        
                            CLIP_DEFAULT_PRECIS,        
                            DEFAULT_QUALITY,        //一系列的默认值
                            DEFAULT_PITCH | FF_DONTCARE,   
                           (TCHAR*)"楷体"    //字体名称
                        );
	switch(uMsg)
	{
	case WM_CREATE:
		Init_btree();
		rax=90;
		ray=80;
		i_treebox=0;
		Dbox(box_num,box_x);
		hdc=GetDC(hWnd);
		mdc2=CreateCompatibleDC(hdc);
		hEmptyP2=(HBITMAP)CreateCompatibleBitmap(mdc2,10000,10000);
		SelectObject(mdc2,hEmptyP2);
		FillRect(mdc2, &Rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
		hBtn2=CreateWindow("button","查看过程矩阵",WS_CHILD|WS_BORDER|WS_VISIBLE|BS_PUSHBUTTON,20,20,150,30,hWnd,HMENU(10),hInstance_1,NULL);
		 SendMessage(hBtn2, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));
		 SetScrollRange(hWnd, SB_VERT, 0,20, FALSE);
		 SetScrollRange(hWnd, SB_HORZ, 0,200, FALSE);
		break;
	case WM_PAINT:
		BeginPaint(hWnd,&ps);
//过程
	/*	for(int i=0;i<i_treebox-1;i++)
		{
			for(int j=0;j<box_Many[i].len;j++)
				for(int k=0;k<box_Many[i].len;k++)
				{
					char b[10];
					_itoa_s((int)box_Many[i].box_fnum[j][k],b,10);
					TextOut(hdc,100+300*i+k*50,100+j*50,b,strlen(b));
					
				}
				char a[10];
				_itoa_s((int)box_Many[i].num,a,10);
				TextOut(hdc,100+300*i-20,100-20,a,strlen(a));
				for(int j=0;j<box_Many[i].len;j++)
					TextOut(hdc,100+300*i+50*j,100-20,box_Many[i].box_head[j],strlen(box_Many[i].box_head[j]));
		}*/

		//
		//TreebuildProcess(hdc);
		
		if(fl_b==0)
		{
			BitBlt(hdc,0,50,2000,2000,mdc2,0,0,SRCCOPY);
			BitBlt(hdc,170,0,1830,2000,mdc2,0,0,SRCCOPY);
			BitBlt(hdc,0,0,170,20,mdc2,0,0,SRCCOPY);
			BitBlt(hdc,0,0,20,50,mdc2,0,0,SRCCOPY);
			Init_btree();
			buildtree(hdc,100+rax,20+ray,i_treebox-1);
			/*SelectObject(hdc,hFont1);
			SetTextColor(hdc,RGB(255,0,0));
			SetBkMode(hdc,TRANSPARENT);
			TextOut(hdc,c_x/2.0-50,c_y-60,"←",strlen("←"));
			TextOut(hdc,c_x/2.0,c_y-60,"→",strlen("→"));
			TextOut(hdc,c_x-50,c_y/2.0-50,"↑",strlen("↑"));
			TextOut(hdc,c_x-50,c_y/2.0,"↓",strlen("↓"));*/
		}
		else
		{
			BitBlt(hdc,0,50,2000,2000,mdc2,0,0,SRCCOPY);
			BitBlt(hdc,170,0,1830,2000,mdc2,0,0,SRCCOPY);
			BitBlt(hdc,0,0,170,20,mdc2,0,0,SRCCOPY);
			BitBlt(hdc,0,0,20,50,mdc2,0,0,SRCCOPY);
			TreebuildProcess(hdc,rax,ray);
			/*SelectObject(hdc,hFont1);
			SetTextColor(hdc,RGB(255,0,0));
			SetBkMode(hdc,TRANSPARENT);
			TextOut(hdc,c_x/2.0-50,c_y-60,"←",strlen("←"));
			TextOut(hdc,c_x/2.0,c_y-60,"→",strlen("→"));*/
		}
		EndPaint(hWnd,&ps);
		break;
	case WM_LBUTTONDOWN:
		//_x=LOWORD(lParam);
		//_y=HIWORD(lParam);
		//if(_x>(c_x-150)&&_x<(c_x-100)&&_y>(c_y-50)&&_y<c_y)
		//{
		//	Rect1.bottom=c_y;
		//	Rect1.right=c_x;
		//	Rect1.left=0;
		//	Rect1.top=400;
		//	rax=rax+200;
		//	InvalidateRect(hWnd,&Rect1,true);
		//
		//}
		//if(_x>(c_x-75)&&_x<(c_x)&&_y>(c_y-50)&&_y<c_y)
		//{
		//	/*rax=rax-200;
		//	SendMessage(hWnd,WM_PAINT,0,0);*/
		//	Rect1.bottom=c_y;
		//	Rect1.right=c_x;
		//	Rect1.left=0;
		//	Rect1.top=400;
		//	rax=rax-200;
		//	InvalidateRect(hWnd,&Rect1,true);
		//	
		//}
		//
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case 10:
			if(fl_b==0)
			{
				rax=0;
				ray=80;
				SetScrollPos(hWnd, SB_VERT,0, TRUE);
				SetScrollPos(hWnd, SB_HORZ,0, TRUE);
				BitBlt(hdc,0,50,2000,2000,mdc2,0,0,SRCCOPY);
				BitBlt(hdc,170,0,1830,2000,mdc2,0,0,SRCCOPY);
				BitBlt(hdc,0,0,170,20,mdc2,0,0,SRCCOPY);
				BitBlt(hdc,0,0,20,50,mdc2,0,0,SRCCOPY);
				TreebuildProcess(hdc,rax,ray);
				SendMessage(hBtn2,WM_SETTEXT,0,LPARAM("查看树"));
				/*SelectObject(hdc,hFont1);
				SetTextColor(hdc,RGB(255,0,0));
				SetBkMode(hdc,TRANSPARENT);
				TextOut(hdc,c_x/2.0-50,c_y-60,"←",strlen("←"));
				TextOut(hdc,c_x/2.0,c_y-60,"→",strlen("→"));
				TextOut(hdc,c_x-50,c_y/2.0-50,"↑",strlen("↑"));
				TextOut(hdc,c_x-50,c_y/2.0,"↓",strlen("↓"));*/
				fl_b=1;
			}
			else
			{
				rax=0;
				ray=80;
				SetScrollPos(hWnd, SB_VERT,0, TRUE);
				SetScrollPos(hWnd, SB_HORZ,0, TRUE);
				BitBlt(hdc,0,50,2000,2000,mdc2,0,0,SRCCOPY);
				BitBlt(hdc,170,0,1830,2000,mdc2,0,0,SRCCOPY);
				BitBlt(hdc,0,0,170,20,mdc2,0,0,SRCCOPY);
				BitBlt(hdc,0,0,20,50,mdc2,0,0,SRCCOPY);
				Init_btree();
				buildtree(hdc,100+rax,20+ray,i_treebox-1);
				/*SelectObject(hdc,hFont1);
				SetTextColor(hdc,RGB(255,0,0));
				SetBkMode(hdc,TRANSPARENT);
				TextOut(hdc,c_x/2.0-50,c_y-60,"←",strlen("←"));
				TextOut(hdc,c_x/2.0,c_y-60,"→",strlen("→"));*/
				SendMessage(hBtn2,WM_SETTEXT,0,LPARAM("查看过程矩阵"));
				fl_b=0;
			}
			break;
		}
		break;
	case WM_SIZE:
		c_x=LOWORD(lParam);
		c_y=HIWORD(lParam);
		//cxClient = LOWORD(lParam);//获取客户区尺寸			
		//cyClient = HIWORD(lParam);		
		//if((y_t1-cyClient )>0)
		//iMaxLine = (y_t1-cyClient )/100+1;
		//else
		//{
		//	iMaxLine = 0;
		//									while(1)
		//								{
		//									char b[10];
		//									_itoa_s(y_t1,b,10);
		//								MessageBox(NULL,b,"",MB_OK);
		//								break;
		//								}
		//}
		//si.cbSize = sizeof(si);			
		//si.fMask = SIF_RANGE | SIF_PAGE;			
		//si.nMin = 0;			
		//si.nMax = iMaxLine;			
		//si.nPage = iMaxLine;	
		//SetScrollInfo(hWnd, SB_VERT, &si, true);

		//if((x_t1-cxClient )>0)
		//iMaxLine = (x_t1-cxClient )/100+1;
		//else
		//{
		//	iMaxLine = 0;
		//}
		//si.cbSize = sizeof(si);			
		//si.fMask = SIF_RANGE | SIF_PAGE;			
		//si.nMin = 0;			
		//si.nMax = iMaxLine;			
		//si.nPage = iMaxLine;		
		//SetScrollInfo(hWnd, SB_HORZ, &si, true);	
		//iMaxWidth = cxClient / cxCaps;//每行输出的字数
		break;
	case WM_VSCROLL:
		switch (LOWORD(wParam))
		{
			case SB_THUMBTRACK:
				//MessageBox(NULL,"","",MB_OK);
				i1=HIWORD(wParam);
				SetScrollPos(hWnd, SB_VERT,i1, TRUE);
				//SetScrollPos(hWnd, SB_HORZ,i2, TRUE);
				Rect1.bottom=c_y;
				Rect1.right=c_x;
				Rect1.left=0;
				Rect1.top=0;
				ray=80-100*i1;
				//rax=200*i2;
				InvalidateRect(hWnd,&Rect1,false);
				break;
			
				
		}
		break;
	case WM_HSCROLL:
		switch (LOWORD(wParam))
		{
		case SB_THUMBTRACK:
				i2=HIWORD(wParam);
				SetScrollPos(hWnd, SB_HORZ,i2, TRUE);
				Rect1.bottom=c_y;
				Rect1.right=c_x;
				Rect1.left=0;
				Rect1.top=0;
				//ray=80-100*i1;
				rax=90-30*i2;
				InvalidateRect(hWnd,&Rect1,false);
				break;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd,uMsg,wParam,lParam);
	}
	return DefWindowProc(hWnd,uMsg,wParam,lParam);


}
/*蛋白质比对输入*/			LRESULT CALLBACK WinProc7(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static UINT c_x=wWidth7,c_y=wHeight7;
	static int rat=150;//微调
	static int rat1=300;//微调
	static int rat_y=-100;//微调
	char r_b[10];
	int nt_b=0;
	HBITMAP p1,p2;
	PAINTSTRUCT ps;
	static HDC hdc,mdc,mdc1;
	static HFONT hFont = CreateFont
                        (
                            20,0,    //高度20, 宽取0表示由系统选择最佳
                            0, 0,    //文本倾斜，与字体倾斜都为0
                            0/*FW_HEAVY*/,    //粗体
                            0,0,0,        //非斜体，无下划线，无中划线
                            GB2312_CHARSET,    //字符集
                            OUT_DEFAULT_PRECIS,        
                            CLIP_DEFAULT_PRECIS,        
                            DEFAULT_QUALITY,        //一系列的默认值
                            DEFAULT_PITCH | FF_DONTCARE,   
                           (TCHAR*)"楷体"    //字体名称
                        );
	static HFONT hFont1 = CreateFont
                        (
                            18,0,    //高度20, 宽取0表示由系统选择最佳
                            0, 0,    //文本倾斜，与字体倾斜都为0
                            0/*FW_HEAVY*/,    //粗体
                            0,0,0,        //非斜体，无下划线，无中划线
                            GB2312_CHARSET,    //字符集
                            OUT_DEFAULT_PRECIS,        
                            CLIP_DEFAULT_PRECIS,        
                            DEFAULT_QUALITY,        //一系列的默认值
                            DEFAULT_PITCH | FF_DONTCARE,   
                           (TCHAR*)"楷体"    //字体名称
                        );
	static HFONT hFont2 = CreateFont
                        (
                            20,0,    //高度20, 宽取0表示由系统选择最佳
                            0, 0,    //文本倾斜，与字体倾斜都为0
                            /*0*/FW_HEAVY,    //粗体
                            0,0,0,        //非斜体，无下划线，无中划线
                            GB2312_CHARSET,    //字符集
                            OUT_DEFAULT_PRECIS,        
                            CLIP_DEFAULT_PRECIS,        
                            DEFAULT_QUALITY,        //一系列的默认值
                            DEFAULT_PITCH | FF_DONTCARE,   
                           (TCHAR*)"黑体"    //字体名称
                        );
	 static HFONT hFont3 = CreateFont
                        (
                            20,0,    //高度20, 宽取0表示由系统选择最佳
                            0, 0,    //文本倾斜，与字体倾斜都为0
                            /*0*/FW_HEAVY,    //粗体
                            0,0,0,        //非斜体，无下划线，无中划线
                            GB2312_CHARSET,    //字符集
                            OUT_DEFAULT_PRECIS,        
                            CLIP_DEFAULT_PRECIS,        
                            DEFAULT_QUALITY,        //一系列的默认值
                            DEFAULT_PITCH | FF_DONTCARE,   
                           (TCHAR*)"楷体"    //字体名称
                        );
	 x=LOWORD(lParam);
	 y=HIWORD(lParam);
	 
	switch(uMsg)
	{
	case WM_CREATE:
//初始化HDC
		hdc=GetDC(hWnd);
		mdc=CreateCompatibleDC(hdc);
		mdc1=CreateCompatibleDC(hdc);
		//mdc2=CreateCompatibleDC(hdc);
//加载图片
		P_W7_1=LoadBitmap(hInstance_1,MAKEINTRESOURCE(IDB_BITMAP5));
		P_W7_2=LoadBitmap(hInstance_1,MAKEINTRESOURCE(IDB_BITMAP7));
		SelectObject(mdc1,P_W7_1);
		SelectObject(mdc,P_W7_2);
		
		//初始化全局变量
		fl=1;
		for(int i=0;i<98;i++)
			for(int j=0;j<98;j++)
			{
				memset(box_Many[i].box_head,'\0',98);
			}
		hMenu1 = CreatePopupMenu(); 
		AppendMenu(hMenu1,MFT_STRING,IDC_BLOSUM62,"Blosum62");
		AppendMenu(hMenu1,MFT_STRING,IDC_PAM250,"PAM250");
		hBtn1=CreateWindow("button","确认",WS_CHILD|WS_BORDER|WS_VISIBLE|BS_PUSHBUTTON,400+rat,250,80,30,hWnd,HMENU(1),hInstance_1,NULL);
		hWrite1=CreateWindow("edit",NULL,WS_CHILD|WS_BORDER|WS_VISIBLE|ES_MULTILINE|ES_UPPERCASE,330+rat,100,150,30,hWnd,HMENU(2),hInstance_1,NULL);
		hWrite2=CreateWindow("edit",NULL,WS_CHILD|WS_BORDER|WS_VISIBLE|ES_MULTILINE|ES_UPPERCASE,330+rat,150,150,30,hWnd,HMENU(3),hInstance_1,NULL);
		hWrite3=CreateWindow("edit",NULL,WS_CHILD|WS_BORDER|WS_VISIBLE|ES_MULTILINE|ES_READONLY,330+rat,200,150,30,hWnd,HMENU(4),hInstance_1,NULL);
		hWrite4=CreateWindow("edit",NULL,WS_CHILD|WS_BORDER|WS_VISIBLE|ES_MULTILINE,330+rat,250,50,30,hWnd,HMENU(5),hInstance_1,NULL);
		//hWrite5=CreateWindow("edit",NULL,WS_CHILD|WS_BORDER|WS_VISIBLE|ES_MULTILINE,330,300,50,30,hWnd,HMENU(6),hInstance_1,NULL);
			 //hFont = CreateFont
    //                    (
    //                        20,0,    //高度20, 宽取0表示由系统选择最佳
    //                        0, 0,    //文本倾斜，与字体倾斜都为0
    //                        0/*FW_HEAVY*/,    //粗体
    //                        0,0,0,        //非斜体，无下划线，无中划线
    //                        GB2312_CHARSET,    //字符集
    //                        OUT_DEFAULT_PRECIS,        
    //                        CLIP_DEFAULT_PRECIS,        
    //                        DEFAULT_QUALITY,        //一系列的默认值
    //                        DEFAULT_PITCH | FF_DONTCARE,   
    //                       (TCHAR*)"楷体"    //字体名称
    //                    );
       SendMessage(hWrite1, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));
	   SendMessage(hWrite2, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));
	   SendMessage(hWrite3, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));
	   SendMessage(hWrite4, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));
	   //SendMessage(hWrite5, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));
	   SendMessage(hBtn1, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));
	   SendMessage(hWrite3, WM_SETTEXT,  (WPARAM)(GetWindowTextLength(hWrite3)+1),(LPARAM)"Blosum62");
	 
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_BLOSUM62:
			SendMessage(hWrite3, WM_SETTEXT,  0,(LPARAM)"Blosum62");
			::SetFocus(hWrite4);
			break;
		case IDC_PAM250:
			SendMessage(hWrite3, WM_SETTEXT,  0,(LPARAM)"PAM250");
			::SetFocus(hWrite4);
			break;
		case 1:
			memset(list_x,'\0',20);
			memset(list_y,'\0',20);
			memset(r_b,'\0',10);
			memset(stand[1],'\0',2);
			//memset(stand[2],'\0',2);
			SendMessage(hWrite1, WM_GETTEXT,  (WPARAM)(GetWindowTextLength(hWrite1)+1),(LPARAM)list_x);
			SendMessage(hWrite2, WM_GETTEXT,  (WPARAM)(GetWindowTextLength(hWrite2)+1),(LPARAM)list_y);
			SendMessage(hWrite3, WM_GETTEXT,  (WPARAM)(GetWindowTextLength(hWrite3)+1),(LPARAM)r_b);
			SendMessage(hWrite4, WM_GETTEXT,  (WPARAM)(GetWindowTextLength(hWrite4)+1),(LPARAM)stand[1]);
			//SendMessage(hWrite5, WM_GETTEXT,  (WPARAM)(GetWindowTextLength(hWrite5)+1),(LPARAM)stand[2]);
			if(!(GetWindowTextLength(hWrite1)&&GetWindowTextLength(hWrite2)&&GetWindowTextLength(hWrite3)&&GetWindowTextLength(hWrite4)
				))
			{
				MessageBox(NULL,"有数据未输入","Tip",MB_OK|MB_SYSTEMMODAL);
				break;
			}
			the_empty_p=atoi(stand[1]);
			if(the_empty_p>0)
			{
				MessageBox(NULL,"空位应小于等于0","error",MB_OK);
				break;
			}
			lenx=strlen(list_x);//第一组基因序列长度
			leny=strlen(list_y);//第二组基因序列长度
			if(strcmp(r_b,"Blosum62")==0)
				nt_b=1;
			if(strcmp(r_b,"PAM250")==0)
				nt_b=2;
			check_p(nt_b);
			ShowWindow(hWnd1,SW_HIDE);
			CreateW2(hWnd2,hInstance_1);
			break;

		}
		break;
	case WM_LBUTTONDOWN:
			if(fl==1&&x>=80&&x<=100&&y>=c_y-115&&y<=c_y-94)
		{

			TransparentBlt(hdc,0,0,c_x/5,c_y,mdc1,0,0,670/5,454,RGB(255,255,255));
			fl=0;
		}
		break;
	case WM_PAINT:
		BeginPaint(hWnd,&ps);
		SetBkMode(hdc,TRANSPARENT);
		TransparentBlt(hdc,0,0,c_x,c_y,mdc1,0,0,670,454,RGB(255,255,255));
		if(fl==1)
		{
			BitBlt(hdc,0,c_y-94,94,94,mdc,3,3,SRCCOPY);
			SetTextColor(hdc,RGB(255,0,100));
			SelectObject(hdc,hFont1);
			SetBkMode(hdc,TRANSPARENT);
			TextOut(hdc,2,c_y-115/*wHeight7-115-38*/,"支持作者",strlen("支持作者"));
			SetTextColor(hdc,RGB(200,200,0));
			SelectObject(hdc,hFont1);
			TextOut(hdc,80,c_y-115/*wHeight7-115-38*/,"×",strlen("×"));
			fl=1;
		}
		SelectObject(hdc,hFont3);
		SetTextColor(hdc,RGB(0,100,255));
		if(c_x!=cx)
		{
			TextOut(hdc,210+rat,105,"蛋白质序列1",strlen("蛋白质序列1"));
			TextOut(hdc,210+rat,155,"蛋白质序列2",strlen("蛋白质序列2"));
			TextOut(hdc,240+rat,205,"打分矩阵",strlen("打分矩阵"));
			TextOut(hdc,235+rat,255,"空位(<=0)",strlen("空位(<=0)"));
		}
		else
		{
			TextOut(hdc,c_x/2.0+rat1-10*(strlen("蛋白质序列1")+1),c_y/2.0-150+rat_y,"蛋白质序列1",strlen("蛋白质序列1"));
			TextOut(hdc,c_x/2.0+rat1-10*(strlen("蛋白质序列2")+1),c_y/2.0-100+rat_y,"蛋白质序列2",strlen("蛋白质序列2"));
			TextOut(hdc,c_x/2.0+rat1-10*(strlen("打分矩阵")+1),c_y/2.0-50+rat_y,"打分矩阵",strlen("打分矩阵"));
			TextOut(hdc,c_x/2.0+rat1-10*(strlen("空位(<=0)")+1),c_y/2.0+rat_y,"空位(<=0)",strlen("空位(<=0)"));
		}
		EndPaint(hWnd,&ps);
		break;
	case WM_SIZE:
		c_x=LOWORD(lParam);
		c_y=HIWORD(lParam);
		if(c_x==cx)
		{
			MoveWindow(hWrite1,c_x/2.0+rat1,c_y/2.0-150+rat_y,150,30,true);
			MoveWindow(hWrite2,c_x/2.0+rat1,c_y/2.0-100+rat_y,150,30,true);
			MoveWindow(hWrite3,c_x/2.0+rat1,c_y/2.0-50+rat_y,150,30,true);
			MoveWindow(hWrite4,c_x/2.0+rat1,c_y/2.0+rat_y,50,30,true);
			//MoveWindow(hWrite5,c_x/2.0,c_y/2.0+50,50,30,false);
			MoveWindow(hBtn1,c_x/2.0+70+rat1,c_y/2.0+rat_y,80,30,true);
		}
		else
		{
			MoveWindow(hWrite1,330+rat,100,150,30,true);
			MoveWindow(hWrite2,330+rat,150,150,30,true);
			MoveWindow(hWrite3,330+rat,200,150,30,true);
			MoveWindow(hWrite4,330+rat,250,50,30,true);
			//MoveWindow(hWrite5,c_x/2.0,c_y/2.0+50,50,30,false);
			MoveWindow(hBtn1,400+rat,250,80,30,true);
		}
		break;
	case WM_CLOSE:
			memset(list_x,'\0',20);
			memset(list_y,'\0',20);
			memset(r_b,'\0',10);
			memset(stand[1],'\0',2);
		DestroyMenu(hMenu1);
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		ShowWindow(hWnd4,SW_SHOW);
		break;
	default:
		return DefWindowProc(hWnd,uMsg,wParam,lParam);
	}
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

//其他函数框---------------------------------------------------------------------------------------------------
/*画线封装*/				void line(HDC hdc,int x1,int y1,int x2,int y2)
{
	MoveToEx(hdc,x1,y1,NULL);
	LineTo(hdc,x2,y2);
}
/*缓冲窗口计时线程*/		DWORD WINAPI fun_time(LPVOID lpParameter)
{
	Sleep(2000);
	SendMessage(hWnd3,WM_CLOSE,0,0);
	return 0;
}
/*矩阵处理函数*/			int Dbox(float box_nums[98][98],int len)
{
	float box_nums_1[98][98];
	float t;
	int j_n;
	int box_pa[98];
	int x=-1,y=-1;//行，列
	int fl_db;//第二个循环的flag
//初始化box_nums_1
	for(int i=0;i<98;i++)
		for(int j=0;j<98;j++)
			box_nums_1[i][j]=-1;

//找最小
	 t=/*box_nums[0][1]*/10000;
	for(int i=0;i<len;i++)
		for(int j=0;j<len;j++)
		{
			if(box_nums[i][j]<t&&box_nums[i][j]!=-1)
				t=box_nums[i][j];
			//box_Many[i_treebox].box_fnum[i][j]/*=box_Many[i_treebox].box_fnum[j][i]*/=box_nums[i][j];
		}
		/*if(t==10000)
			MessageBox(NULL,"666","",MB_OK);*/
		fl_db=0;
//找位置
	for(int i=0;i<len;i++)
	{
		for(int j=0;j<len;j++)
		{
			if(box_nums[i][j]==t&&box_nums[i][j]!=-1)
			{
				fl_db=1;
				x=i;
				y=j;
				break;
			}
		}
		if(fl_db==1)
			break;
	}

////存入全局数组
//	for(int i=0;i<len;i++)
//		for(int j=0;j<len;j++)
//			box_Many[i_treebox].box_fnum[i][j]/*=box_Many[i_treebox].box_fnum[j][i]*/=box_nums[i][j];
//	box_Many[i_treebox].len=len;
//	box_Many[i_treebox].num=t;
//	//
//	strcpy_s(box_Many[i_treebox+1].box_head[0],box_Many[i_treebox].box_head[x]);
//	strcat_s(box_Many[i_treebox+1].box_head[0],box_Many[i_treebox].box_head[y]);
//	for(int i=1;i<len-1;i++)
//	{
//		strcpy_s(box_Many[i_treebox+1].box_head[0],box_Many[i_treebox].box_head[box_pa[i]]);
//	}
//	//
//	i_treebox++;
/*
	插入画图，插入标签
	TODO...
*/

		for(int j=0,i=1;j<len;j++)
		{
			if(j!=x&&j!=y)
			{
					box_nums_1[i][0]=box_nums_1[0][i]=(box_nums[x][j]+box_nums[y][j])/2.0;
					box_pa[i]=j;
					i++;
			}
		}
			for(int i=1;i<len-2;i++)
				for(int j=i+1;j<len-1;j++)
					{
						box_nums_1[j][i]=box_nums_1[i][j]=box_nums[box_pa[i]][box_pa[j]];
					}
			/*for(int i=0;i<len-2;i++)
				for(int j=i+1;j<len-1;j++)
				{
					box_nums_1[j][i]=box_nums_1[i][j];
				}*/

	//存入全局数组
	for(int i=0;i<len;i++)
		for(int j=0;j<len;j++)
			box_Many[i_treebox].box_fnum[i][j]/*=box_Many[i_treebox].box_fnum[j][i]*/=box_nums[i][j];
	box_Many[i_treebox].len=len;
	box_Many[i_treebox].num=t;
//
if(x!=-1&&y!=-1)
{
	j_n=0;
	for(int i=0;i>-1;i++)
	{
		if(box_Many[i_treebox].box_head[x][i]=='\0')break;
		if(box_Many[i_treebox].box_head[x][i]!=' ')
			box_Many[i_treebox+1].box_head[0][j_n++]=box_Many[i_treebox].box_head[x][i];
	}
	box_Many[i_treebox+1].box_head[0][j_n++]=' ';
	box_Many[i_treebox+1].box_head[0][j_n]='\0';
//
	//strcpy_s(box_Many[i_treebox+1].box_head[0],box_Many[i_treebox].box_head[x]);
	//strcat_s(box_Many[i_treebox+1].box_head[0],box_Many[i_treebox].box_head[y]);
	for(int i=0;i>-1;i++)
	{
		if(box_Many[i_treebox].box_head[y][i]=='\0')break;
		if(box_Many[i_treebox].box_head[y][i]!=' ')
			box_Many[i_treebox+1].box_head[0][j_n++]=box_Many[i_treebox].box_head[y][i];
	}
	//box_Many[i_treebox+1].box_head[0][j_n]='\0';
	
	//box_Many[i_treebox+1].box_head[0][j_n]='\0';
	for(int i=1;i<len-1;i++)
	{
		//strcpy_s(box_Many[i_treebox+1].box_head[i],box_Many[i_treebox].box_head[box_pa[i]]);
		j_n=0;
		for(int m=0;box_Many[i_treebox].box_head[box_pa[i]][m]!='\0';m++)
		{
		
			if(box_Many[i_treebox].box_head[x][m]!=' ')
				box_Many[i_treebox+1].box_head[i][j_n++]=box_Many[i_treebox].box_head[box_pa[i]][m];
		}
		box_Many[i_treebox+1].box_head[i][j_n]='\0';
	}
}
	i_treebox++;
		
	if(len>1)
		Dbox(box_nums_1,len-1);
	else
		return 0;

	//}
		//for(int m=0,int i=1,int j=i+1;m<len;m++)
		//	if(m!=x&&m!=y)
		//	{
		//			/*for(int i=1;i<len-2;i++)
		//				for(int j=i+1;j<len-1;j++)*/
		//		if(j<len-1)
		//					box_nums_1[i][j++]=
		//	}

}
/*float转字符串*/			char * float2str(float val, int precision, char *buf)
{
    char *cur, *end;
    sprintf_s(buf,10, "%.6f", val);
    if (precision < 6) {
        cur = buf + strlen(buf) - 1;
        end = cur - 6 + precision; 
        while ((cur > end) && (*cur == '0')) {
            *cur = '\0';
            cur--;
        }
    }
    return buf;
}
/*非加权组平均法建树*/		int buildtree(HDC mdc,int rat,int ray,int i)
{

	static HFONT hFont = CreateFont
                        (
                            20,0,    //高度20, 宽取0表示由系统选择最佳
                            0, 0,    //文本倾斜，与字体倾斜都为0
                            0/*FW_HEAVY*/,    //粗体
                            0,0,0,        //非斜体，无下划线，无中划线
                            GB2312_CHARSET,    //字符集
                            OUT_DEFAULT_PRECIS,        
                            CLIP_DEFAULT_PRECIS,        
                            DEFAULT_QUALITY,        //一系列的默认值
                            DEFAULT_PITCH | FF_DONTCARE,   
                           (TCHAR*)"楷体"    //字体名称
                        );
	SelectObject(mdc,hFont);
	SetTextColor(mdc,RGB(0,0,0));
	SetBkMode(mdc,TRANSPARENT);
			char buff1[10],buff2[10];
			memset(buff1,'\0',10);
			memset(buff2,'\0',10);
			int m=0,n=0;
			int k=0;
			int x1=0;
			int x2=0;
			for(int j=0;box_Many[i].box_head[0][j]!='\0';j++)
			{
				if(box_Many[i].box_head[0][j]!=' ')
					buff1[m++]=box_Many[i].box_head[0][j];
				else 
				{
					
					break;
				}
			}
			for(int j=m+1;box_Many[i].box_head[0][j]!='\0';j++)
					buff2[n++]=box_Many[i].box_head[0][j];
			
			for(int m1=0;m1<i;m1++)
			{
				char buf[100];
				int l=0;
				memset(buf,'\0',100);
				for(int h=0;box_Many[m1].box_head[0][h]!='\0';h++)
				{
				if(box_Many[m1].box_head[0][h]!=' ')
					buf[l++]=box_Many[m1].box_head[0][h];
				}
				if(strcmp(buff1,buf)==0)
				{
					x1=m1;	
				}
				if(strcmp(buff2,buf)==0)
				{
					x2=m1;
				}
			}

			if(strlen(buff1)==1&&strlen(buff2)==1)
			{
				for(int j=0;j<box_x;j++)
				{
					char buf[100];
						int l=0;
						memset(buf,'\0',100);
						for(int h=0;box_head_buff[j][h]!='\0';h++)
						{
						if(box_head_buff[j][h]!=' ')
							buf[l++]=box_head_buff[j][h];
						}
					if(strcmp(buf,buff1)==0)
					{
						
						/*tload[i].num_a=box_Many[i-1].num/2.0;
						tload[i].num_c=box_Many[i-1].num/2.0;
						tload[i].a=MT*tload[i].num_a;
						tload[i].c=MT*tload[i].num_c;
						tload[i].b=word_x*3;
						tload[i].w=MAX(tload[i].a+word_x,tload[i].c+word_x);*/
						char b1[10],b2[10];
						float2str(tload[i].num_a,2,b1);
						float2str(tload[i].num_c,2,b2);
						TextOut(mdc,rat+tload[i].a-word_x*3,ray+0,b1,strlen(b1));
						TextOut(mdc,rat+tload[i].c-word_x*3,ray+tload[i].b+word_y-word_y,b2,strlen(b2));
						TextOut(mdc,rat+tload[i].a,ray+word_y/2.0,buff1,strlen(buff1));
						TextOut(mdc,rat+tload[i].c,ray+tload[i].b+word_y/2.0,buff2,strlen(buff2));
						drawtree(mdc,rat+0,ray+word_y,tload[i].a,tload[i].b,tload[i].c);
						return 0;
						//break;
					}
				}	
			}

			else if(strlen(buff1)==1&&strlen(buff2)!=1)
			{
				
				for(int j=0;j<box_x;j++)
					{
					char buf[100];
						int l=0;
						memset(buf,'\0',100);
						for(int h=0;box_head_buff[j][h]!='\0';h++)
						{
						if(box_head_buff[j][h]!=' ')
							buf[l++]=box_head_buff[j][h];
						}
					if(strcmp(buf,buff1)==0)
					{
						/*tload[i].num_a=box_Many[i-1].num/2.0;
						tload[i].num_c=(box_Many[i-1].num-box_Many[x2-1].num)/2.0;
						tload[i].a=MT*tload[i].num_a;
						tload[i].c=MT*tload[i].num_c;
						tload[i].b=word_x*3+tload[x2].b;
						tload[i].w=tload[i].b+tload[x2].w;*/
						char b1[10],b2[10];
						float2str(tload[i].num_a,2,b1);
						float2str(tload[i].num_c,2,b2);
						TextOut(mdc,rat+tload[i].a-word_x*3,ray+0,b1,strlen(b1));
						if(tload[i].num_c!=0)
						TextOut(mdc,rat+tload[i].c-word_x*3,ray+tload[i].b+word_y-word_y,b2,strlen(b2));
						TextOut(mdc,rat+tload[i].a,ray+word_y/2.0,buff1,strlen(buff1));
						drawtree(mdc,rat+0,ray+word_y,tload[i].a,tload[i].b,tload[i].c);
						buildtree(mdc,rat+rat+tload[i].c, ray+tload[i].b+word_y,x2);
						return 0;
						break;
					}
				}
			}
			else if(strlen(buff2)==1&&strlen(buff1)!=1)
			{
				
				for(int j=0;j<box_x;j++)
					{
					char buf[100];
						int l=0;
						memset(buf,'\0',100);
						for(int h=0;box_head_buff[j][h]!='\0';h++)
						{
						if(box_head_buff[j][h]!=' ')
							buf[l++]=box_head_buff[j][h];
						}
					if(strcmp(buf,buff2)==0)
					{
						/*tload[i].num_a=(box_Many[i-1].num-box_Many[x1-1].num)/2.0;
						tload[i].num_c=box_Many[i-1].num/2.0;
						tload[i].a=MT*tload[i].num_a;
						tload[i].c=MT*tload[i].num_c;
						tload[i].b=word_x*3+tload[x1].b;
						tload[i].w=tload[i].a+tload[x1].w;*/
						char b1[10],b2[10];
						float2str(tload[i].num_a,2,b1);
						float2str(tload[i].num_c,2,b2);
						if(tload[i].num_a!=0)
						TextOut(mdc,rat+tload[i].a-word_x*3,ray+tload[x1].b/2.0-word_y,b1,strlen(b1));
						TextOut(mdc,rat+tload[i].c-word_x*3,ray+tload[i].b+tload[x1].b/2.0-word_y,b2,strlen(b2));
						//TextOut(mdc[i],rat+tload[i].a,word_y/2.0,buff1,strlen(buff1));
						TextOut(mdc,rat+tload[i].c,ray+tload[i].b+tload[x1].b/2.0-word_y/2.0,buff2,strlen(buff2));
						drawtree(mdc,rat+0,MAX(ray+tload[x1].b/2.0,ray+word_y),tload[i].a,tload[i].b,tload[i].c);
						//buildtree(mdc,rat+rat+tload[i].c, ray+tload[i].b+word_y,x2);
						buildtree(mdc,rat+tload[i].a, ray/*+tload[x1].b/2.0*/,x1);
						return 0;
						break;
					}
				}
			}
			else
			{
						/*tload[i].num_a=box_Many[i-1].num/2.0-box_Many[x1-1].num/2.0;
						tload[i].num_c=box_Many[i-1].num/2.0-box_Many[x2-1].num/2.0;
						tload[i].a=MT*tload[i].num_a;
						tload[i].c=MT*tload[i].num_c;
						tload[i].b=tload[x2].b+tload[x1].b;
						tload[i].w=MAX(tload[i].a+tload[x1].w,tload[i].b+tload[x2].w);	*/
						char b1[10],b2[10];
						float2str(tload[i].num_a,2,b1);
						float2str(tload[i].num_c,2,b2);
						if(tload[i].num_a!=0)
						TextOut(mdc,rat+tload[i].a-word_x*3,ray+tload[x1].b/2.0-word_y,b1,strlen(b1));
						if(tload[i].num_c!=0)
						TextOut(mdc,rat+tload[i].c-word_x*3,ray+tload[i].b+tload[x1].b/2.0-word_y,b2,strlen(b2));
						drawtree(mdc,rat+0,ray+tload[x1].b/2.0,tload[i].a,tload[i].b,tload[i].c);
						buildtree(mdc,rat+tload[i].c, ray+tload[i].b+tload[x1].b/2.0-tload[x2].b/2.0,x2);
						buildtree(mdc,rat+tload[i].a, ray/*+tload[x1].b/2.0*/,x1);
						return 0;
					
			}
		
}
/*非加权组平均法建树初始化*/int Init_btree()
{
for(int i=1;i<i_treebox;i++)
		{
			char buff1[10],buff2[10];
			memset(buff1,'\0',10);
			memset(buff2,'\0',10);
			int m=0,n=0;
			int k=0;
			int x1=0;
			int x2=0;
			for(int j=0;box_Many[i].box_head[0][j]!='\0';j++)
			{
				/*if(box_Many[i].box_head[0][j]=='\0')
					break;*/
				if(box_Many[i].box_head[0][j]!=' ')
					buff1[m++]=box_Many[i].box_head[0][j];
				else 
				{
					
					break;
				}
			}
			for(int j=m+1;box_Many[i].box_head[0][j]!='\0';j++)
					buff2[n++]=box_Many[i].box_head[0][j];
			
			
			for(int m1=0;m1<i;m1++)
			{
				char buf[100];
				int l=0;
				memset(buf,'\0',100);
				for(int h=0;box_Many[m1].box_head[0][h]!='\0';h++)
				{
				if(box_Many[m1].box_head[0][h]!=' ')
					buf[l++]=box_Many[m1].box_head[0][h];
				}
				if(strcmp(buff1,buf)==0)
				{
					x1=m1;	
				}
				if(strcmp(buff2,buf)==0)
				{
					//MessageBox(NULL,buff1,buff2,MB_OK);
					x2=m1;
				}
			}
			if(strlen(buff1)==1&&strlen(buff2)==1)
				{
				for(int j=0;j<box_x;j++)
				{
					char buf[100];
						int l=0;
						memset(buf,'\0',100);
						for(int h=0;box_head_buff[j][h]!='\0';h++)
						{
						if(box_head_buff[j][h]!=' ')
							buf[l++]=box_head_buff[j][h];
						}
					if(strcmp(buf,buff1)==0)
					{
						
						tload[i].num_a=box_Many[i-1].num/2.0;
						tload[i].num_c=box_Many[i-1].num/2.0;
						tload[i].a=MT*tload[i].num_a;
						tload[i].c=MT*tload[i].num_c;
						tload[i].b=word_x*3;
						tload[i].w=MAX(tload[i].a+word_x,tload[i].c+word_x);
						break;
					}
				}
				
			}
/*****************************************************************error******************************/

			else if(strlen(buff1)==1&&strlen(buff2)!=1)
			{
				
				for(int j=0;j<box_x;j++)
					{
					char buf[100];
						int l=0;
						memset(buf,'\0',100);
						for(int h=0;box_head_buff[j][h]!='\0';h++)
						{
						if(box_head_buff[j][h]!=' ')
							buf[l++]=box_head_buff[j][h];
						}
					if(strcmp(buf,buff1)==0)
					{
						
						tload[i].num_a=box_Many[i-1].num/2.0;
						tload[i].num_c=(box_Many[i-1].num-box_Many[x2-1].num)/2.0;
						tload[i].a=MT*tload[i].num_a;
						tload[i].c=MT*tload[i].num_c;
						tload[i].b=word_x*3+tload[x2].b;
						tload[i].w=tload[i].b+tload[x2].w;
						
						break;
					}
				}
			}
			else if(strlen(buff2)==1&&strlen(buff1)!=1)
			{
				
				for(int j=0;j<box_x;j++)
					{
					char buf[100];
						int l=0;
						memset(buf,'\0',100);
						for(int h=0;box_head_buff[j][h]!='\0';h++)
						{
						if(box_head_buff[j][h]!=' ')
							buf[l++]=box_head_buff[j][h];
						}
					if(strcmp(buf,buff2)==0)
					
					{
						
						tload[i].num_a=(box_Many[i-1].num-box_Many[x1-1].num)/2.0;
						tload[i].num_c=box_Many[i-1].num/2.0;
						tload[i].a=MT*tload[i].num_a;
						tload[i].c=MT*tload[i].num_c;
						tload[i].b=word_x*3+tload[x1].b;
						tload[i].w=tload[i].a+tload[x1].w;
						
						break;
					}
				}
			}
			else
			{
				
						tload[i].num_a=box_Many[i-1].num/2.0-box_Many[x1-1].num/2.0;
						tload[i].num_c=box_Many[i-1].num/2.0-box_Many[x2-1].num/2.0;
						tload[i].a=MT*tload[i].num_a;
						tload[i].c=MT*tload[i].num_c;
						tload[i].b=tload[x2].b+tload[x1].b;
						tload[i].w=MAX(tload[i].a+tload[x1].w,tload[i].b+tload[x2].w);	
			}
		}
return 0;
}
/*非加权组平均法建树过程*/	int TreebuildProcess(HDC hdc,int rax,int ray)
{
		//for(int i=0;i<i_treebox-1;i++)
		//{
		//	for(int j=0;j<box_Many[i].len;j++)
		//		for(int k=0;k<box_Many[i].len;k++)
		//		{
		//			char b[10];
		//			float2str(box_Many[i].box_fnum[j][k],2,b);
		//			//_itoa_s((int)box_Many[i].box_fnum[j][k],b,10);
		//			TextOut(hdc,100+300*i+k*50,100+j*50,b,strlen(b));
		//			
		//		}
		//		char a[10];
		//		//_itoa_s((int)box_Many[i].num,a,10);
		//		float2str(box_Many[i].num,2,a);
		//		TextOut(hdc,100+300*i-20,100-20,a,strlen(a));
		//		for(int j=0;j<box_Many[i].len;j++)
		//			TextOut(hdc,100+300*i+50*j,100-20,box_Many[i].box_head[j],strlen(box_Many[i].box_head[j]));
		//}

	int ram=10;//微调
	HFONT hFont = CreateFont
                        (
                            20,0,    //高度20, 宽取0表示由系统选择最佳
                            0, 0,    //文本倾斜，与字体倾斜都为0
                            0/*FW_HEAVY*/,    //粗体
                            0,0,0,        //非斜体，无下划线，无中划线
                            GB2312_CHARSET,    //字符集
                            OUT_DEFAULT_PRECIS,        
                            CLIP_DEFAULT_PRECIS,        
                            DEFAULT_QUALITY,        //一系列的默认值
                            DEFAULT_PITCH | FF_DONTCARE,   
                           (TCHAR*)"楷体"    //字体名称
                        );
	SelectObject(hdc,hFont);
	SetTextColor(hdc,RGB(0,0,0));
	SetBkMode(hdc,TRANSPARENT);
	Init_btree();
	for(int i=0;i<i_treebox-1;i++)
		{
			for(int j=0;j<box_Many[i].len;j++)
				for(int k=j+1;k<box_Many[i].len;k++)
				{
					if(box_Many[i].box_fnum[j][k]!=-1)
					{
					char b[10];
					float2str(box_Many[i].box_fnum[j][k],1,b);
					TextOut(hdc,rax+100+(i_treebox+1)*(50+ram)*i+k*(50+ram),ray+100+j*(50+ram),b,strlen(b));
					}
					
				}
				char a[10];
				float2str(box_Many[i].num,1,a);
				TextOut(hdc,rax+100+(i_treebox+1)*(50+ram)*i-40,ray+100-40,"矩阵最小值",strlen("矩阵最小值"));
				TextOut(hdc,rax+100+(i_treebox+1)*(50+ram)*i+60,ray+100-40,a,strlen(a));
				for(int j=0;j<box_Many[i].len;j++)
					TextOut(hdc,rax+100+(i_treebox+1)*(50+ram)*i+(50+ram)*j,ray+100-20,box_Many[i].box_head[j],strlen(box_Many[i].box_head[j]));
		}
	return 0;
}

