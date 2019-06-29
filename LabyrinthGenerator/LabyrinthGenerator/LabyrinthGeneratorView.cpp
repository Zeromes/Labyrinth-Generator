
// LabyrinthGeneratorView.cpp: CLabyrinthGeneratorView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "LabyrinthGenerator.h"
#endif

#include "LabyrinthGeneratorDoc.h"
#include "LabyrinthGeneratorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define test(data) CString str;str.Format(_T("%d"), data);MessageBox(str)
int difficulty = 20;		//难度需为2的倍数
// CLabyrinthGeneratorView

IMPLEMENT_DYNCREATE(CLabyrinthGeneratorView, CView)

BEGIN_MESSAGE_MAP(CLabyrinthGeneratorView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_32771, &CLabyrinthGeneratorView::OnGenerate)
	ON_COMMAND(ID_32773, &CLabyrinthGeneratorView::OnSolve)
END_MESSAGE_MAP()

// CLabyrinthGeneratorView 构造/析构

CLabyrinthGeneratorView::CLabyrinthGeneratorView() noexcept
{
	// TODO: 在此处添加构造代码

}

CLabyrinthGeneratorView::~CLabyrinthGeneratorView()
{
}

BOOL CLabyrinthGeneratorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CLabyrinthGeneratorView 绘图

void CLabyrinthGeneratorView::OnDraw(CDC* /*pDC*/)
{
	CLabyrinthGeneratorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CLabyrinthGeneratorView 打印

BOOL CLabyrinthGeneratorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CLabyrinthGeneratorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CLabyrinthGeneratorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CLabyrinthGeneratorView 诊断

#ifdef _DEBUG
void CLabyrinthGeneratorView::AssertValid() const
{
	CView::AssertValid();
}

void CLabyrinthGeneratorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLabyrinthGeneratorDoc* CLabyrinthGeneratorView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLabyrinthGeneratorDoc)));
	return (CLabyrinthGeneratorDoc*)m_pDocument;
}
#endif //_DEBUG


// CLabyrinthGeneratorView 消息处理程序



void CLabyrinthGeneratorView::OnGenerate()//生成迷宫
{
	// TODO: 在此添加命令处理程序代码

	CDC* pDC = GetDC();						//设备环境

	//把画笔选入环境
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen eraser(PS_SOLID, 1, RGB(255, 255, 255));
	CPen* oldPen = pDC->SelectObject(&pen);

	//将迷宫的绘制区域放到area
	CRect area;				//区域
	GetClientRect(area);
	area.DeflateRect(50, 50);
	int width;
	if (area.Width() >= area.Height())
	{
		width = area.CenterPoint().y - area.TopLeft().y;	//实为1/2宽度
		while (width % (difficulty/2) != 0)
		{
			width++;
		}
		int rx1 = area.CenterPoint().x - width;
		int rx2 = area.CenterPoint().x + width;
		int ry1 = area.CenterPoint().y - width;
		int ry2 = area.CenterPoint().y + width;
		area.SetRect(rx1, ry1, rx2, ry2);
	}
	else
	{
		width = area.CenterPoint().x - area.TopLeft().x;	//实为1/2宽度
		while (width % (difficulty / 2) != 0)
		{
			width++;
		}
		int rx1 = area.CenterPoint().x - width;
		int rx2 = area.CenterPoint().x + width;
		int ry1 = area.CenterPoint().y - width;
		int ry2 = area.CenterPoint().y + width;
		area.SetRect(rx1, ry1, rx2, ry2);
	}
	pDC->Rectangle(area);					//绘制区域
	
	width *= 2;			//实际宽度

	int slen = width / difficulty;	//获取单线长度

	//绘制入口和出口
	pDC->SelectObject(&eraser);
	pDC->MoveTo(area.TopLeft());
	pDC->LineTo(pDC->GetCurrentPosition().x, pDC->GetCurrentPosition().y + slen);	//入口
	pDC->MoveTo(area.BottomRight().x - 1, area.BottomRight().y - 1);
	pDC->LineTo(pDC->GetCurrentPosition().x, pDC->GetCurrentPosition().y - slen);	//出口

	//随机绘制竖线和横线
	pDC->SelectObject(&pen);
	int* Vline = new int[difficulty];	//用于保存竖线的位置
	int* Hline = new int[difficulty];	//用于保存横线的位置
	int* ConnectionIndexA = new int[difficulty];		//用于保存旧的连通区的编号
	int* ConnectionIndexB = new int[difficulty];		//用于保存新的连通区的编号
	int MaxIndex = 0;	//记录当前使用的最大连通区编号
	//初始化上边框线
	for (int i = 0; i < difficulty; i++)
	{
		Hline[i] = 1;
	}
	//绘制迷宫
	for (int i = 0; i < difficulty - 1; i++)
	{
		//初始化竖线记录数组
		for (int j = 0; j < difficulty; j++)
		{
			Vline[j] = 0;
		}
		Vline[difficulty - 1] = 1;	//右边框
		//绘制竖线
		bool kong = !Hline[0];	//记录是否是两个连续空挡
		bool VIsDrawn = FALSE;	//记录是否画过竖线
		for (int j = 1; j < difficulty; j++)
		{
			if (Hline[j] == 0)	//如果已经画到空挡
			{
				if (kong)		//如果是两个连续空格
				{
					//随机画竖线
					srand((int)time(0) * rand());
					pDC->MoveTo(area.TopLeft().x + (j * slen), area.TopLeft().y + (i * slen));	//将画笔移到起始位置
					if (rand() < 10000)
					{
						pDC->LineTo(pDC->GetCurrentPosition().x, pDC->GetCurrentPosition().y + slen);	//画竖线
						Vline[j - 1] = 1;
						VIsDrawn = FALSE;
					}
					else
					{
						Vline[j - 1] = 0;
						VIsDrawn = FALSE;
					}

				}
				else if (!kong && VIsDrawn)	//如果之前已经画过竖线且不是两个连续空格
				{
					//随机画竖线
					srand((int)time(0) * rand());
					pDC->MoveTo(area.TopLeft().x + (j * slen), area.TopLeft().y + (i * slen));	//将画笔移到起始位置
					if (rand() < 10000)
					{
						pDC->LineTo(pDC->GetCurrentPosition().x, pDC->GetCurrentPosition().y + slen);	//画竖线
						Vline[j - 1] = 1;
						VIsDrawn = FALSE;
					}
					else
					{
						Vline[j - 1] = 0;
						VIsDrawn = FALSE;
					}
				}
				else    //没画过竖线
				{
					//那就画一条
					pDC->MoveTo(area.TopLeft().x + (j * slen), area.TopLeft().y + (i * slen));	//将画笔移到起始位置
					pDC->LineTo(pDC->GetCurrentPosition().x, pDC->GetCurrentPosition().y + slen);	//画竖线
					Vline[j - 1] = 1;
					VIsDrawn = FALSE;
				}
				kong = TRUE;
			}
			else    //还没画到空挡
			{
				//随性
				srand((int)time(0) * rand());
				pDC->MoveTo(area.TopLeft().x + (j * slen), area.TopLeft().y + (i * slen));	//将画笔移到起始位置
				if (rand() < 10000)
				{
					pDC->LineTo(pDC->GetCurrentPosition().x, pDC->GetCurrentPosition().y + slen);	//画竖线
					Vline[j - 1] = 1;
					VIsDrawn = TRUE;
				}
				else
				{
					Vline[j - 1] = 0;
				}
				kong = FALSE;
			}
		}
		//计算连通区
		//先对新区赋值
		int before = 0;									//检查区起始编号
		for (int j = 0; j < difficulty; j++)			//逐个竖线检查
		{
			if (Vline[j] == 1)							//如果遇到竖线
			{
				bool found = FALSE;						//检查到出口的标志
				for (int k = before; k <= j; k++)		//逐个检查检查区的横线
				{
					if (Hline[k] == 0)					//如果遇到出口
					{
						found = TRUE;
						break;
					}
				}
				if (found == FALSE)						//如果没有遇到出口则是新区
				{
					//新建该片区域的连通区编号
					for (int l = before; l <= j; l++)
					{
						ConnectionIndexB[l] = MaxIndex;
					}
					MaxIndex++;
				}
				before = j + 1;		//设置检查区起始位置到下一个横线
			}
		}
		//处理旧连通区编号（只对有2个及以上出口的检查区做操作）
		before = 0;
		for (int j = 0; j < difficulty; j++)			//逐个竖线检查
		{
			if (Vline[j] == 1)							//如果遇到竖线
			{
				for (int k = before; k <= j; k++)		//逐个检查检查区的横线
				{
					int ind1, ind2;							//暂存区号
					int c = 0;								//记录检查区中出口的个数
					if (Hline[k] == 0)						//如果遇到出口
					{
						c++;
						if (c == 1)							//如果是第一个出口
						{
							ind1 = ConnectionIndexA[k];		//保存区号到ind1
						}
						else if (c == 2)					//如果是第个出口
						{
							ind2 = ConnectionIndexA[k];		//保存区号到ind2
						}
						else if (c > 2)						//如果是第3或以上出口
						{
							ind1 = ind2;
							ind2 = ConnectionIndexA[k];		//ind1，ind2后移
						}
						if (c >= 2)							//如果检查到了第二或以上个出口
						{
							
							if (ind1 <= ind2)				//如果前面出口的区号比后面出口区号的值小
							{
								//将旧区号数组中小的换大的
								for (int l = 0; l < difficulty; l++)
								{
									if (ConnectionIndexA[l] == ind2)
									{
										ConnectionIndexA[l] = ind1;
									}
								}
							}
							else                             //如果前面出口的区号比后面出口区号的值大
							{
								//将旧区号数组中小的换大的
								for (int l = 0; l < difficulty; l++)
								{
									if (ConnectionIndexA[l] == ind1)
									{
										ConnectionIndexA[l] = ind2;
									}
								}
							}
						}
					}
				}
				before = j + 1;		//设置检查区起始位置到下一个横线
			}
		}
		//对新连通区编号数组进行赋值
		before = 0;									//检查区起始编号
		for (int j = 0; j < difficulty; j++)			//逐个竖线检查
		{
			if (Vline[j] == 1)							//如果遇到竖线
			{
				for (int k = before; k <= j; k++)		//逐个检查检查区的横线
				{
					if (Hline[k] == 0)					//如果遇到出口
					{
						//将该片区域的连通区编号赋为出口处的连通区编号
						for (int l = before; l <= j; l++)
						{
							ConnectionIndexB[l] = ConnectionIndexA[k];
						}
						break;
					}
				}
				before = j + 1;		//设置检查区起始位置到下一个横线
			}
		}

		//将计算好的连通区编号放到旧连通区编号
		for (int j = 0; j < difficulty; j++)
		{
			ConnectionIndexA[j] = ConnectionIndexB[j];
		}
		//找出每一个连通区的最终位置
		int* EndOfConnection = new int[MaxIndex];	//用于保存对应连通区的最终位置，若为-1则该区已被合并
		for (int j = 0; j < MaxIndex; j++)			//挨个连通区编号录入
		{
			bool found = FALSE;
			for (int k = 0; k < difficulty; k++)	//挨个横线检索
			{
				if (ConnectionIndexA[k] == j)		//如果找到对应连通区
				{
					EndOfConnection[j] = k;			//更新最终位置
					found = TRUE;
				}
			}
			if (found == FALSE)						//如果没能找到对应连通区
			{
				EndOfConnection[j] = -1;				//则该区已被合并
			}
		}
		//绘制横线
		bool HIsFull = TRUE;		//记录之前是否一直没有空挡
		bool HIsEmpty = FALSE;		//记录上一条是否未划线
		for (int j = 0; j < difficulty; j++)
		{
			bool END = FALSE;	//是否到达最终位置标志
			int num;			//最终位置区号
			for (int k = 0; k < MaxIndex; k++)	//挨个连通区号检查
			{
				if (EndOfConnection[k] == j)	//如果有连通区的最终位置与当前要画的横线位置相同
				{
					END = TRUE;
					num = k;					//记录下该连通区区号
					break;
				}
			}
			if (END)		//如果到了最终位置
			{
				for (int k = 0; k < j; k++)			//挨个横线检查
				{
					if (ConnectionIndexA[k] == num)		//如果该横线对应的连通区号与当前检查的连通区相等
					{
						if (Hline[k] == 0)				//如果这里是空位
						{
							HIsFull = FALSE;
							break;
						}
					}
				}
				if (!HIsFull)		//如果前面已经有空位
				{
					//随机画横线
					if (HIsEmpty)
					{
						pDC->MoveTo(area.TopLeft().x + (j * slen), area.TopLeft().y + ((i + 1) * slen));	//将画笔移到起始位置
						pDC->LineTo(pDC->GetCurrentPosition().x + slen, pDC->GetCurrentPosition().y);	//画横线
						Hline[j] = 1;
						HIsEmpty = FALSE;
					}
					else
					{
						srand((int)time(0) * rand());
						pDC->MoveTo(area.TopLeft().x + (j * slen), area.TopLeft().y + ((i + 1) * slen));	//将画笔移到起始位置
						if (rand() <50)
						{
							pDC->LineTo(pDC->GetCurrentPosition().x + slen, pDC->GetCurrentPosition().y);	//画横线
							Hline[j] = 1;
							HIsEmpty = FALSE;
						}
						else
						{
							Hline[j] = 0;
							HIsEmpty = TRUE;
						}
					}
				}
				else    //前面没有空位或是只有这一个空
				{
					Hline[j] = 0;
				}
			}
			else    //如果没有到最终位置
			{
				//随机画横线
				if (HIsEmpty)
				{
					pDC->MoveTo(area.TopLeft().x + (j * slen), area.TopLeft().y + ((i + 1) * slen));	//将画笔移到起始位置
					pDC->LineTo(pDC->GetCurrentPosition().x + slen, pDC->GetCurrentPosition().y);	//画横线
					Hline[j] = 1;
					HIsEmpty = FALSE;
				}
				else
				{
					srand((int)time(0) * rand());
					pDC->MoveTo(area.TopLeft().x + (j * slen), area.TopLeft().y + ((i + 1) * slen));	//将画笔移到起始位置
					if (rand() <50)
					{
						pDC->LineTo(pDC->GetCurrentPosition().x + slen, pDC->GetCurrentPosition().y);	//画横线
						Hline[j] = 1;
						HIsEmpty = FALSE;
					}
					else
					{
						Hline[j] = 0;
						HIsEmpty = TRUE;
					}
				}
			}
			HIsFull = TRUE;
		}
	}

	//最后一行
	int first;
	int firstout;
	int secondout;
	//先获得第一个出口的位置
	for (int j = 0; j < difficulty; j++)
	{
		if (Hline[j] == 0)
		{
			first = j;
			break;
		}
	}
	firstout = first;
	bool found;
	while (firstout < difficulty)
	{
		//获得第二个出口的位置
		found = FALSE;
		for (int j = firstout + 1; j < difficulty; j++)
		{
			if (Hline[j] == 0)
			{
				secondout = j;
				found = TRUE;
				break;
			}
		}
		if (found)
		{
			if (ConnectionIndexA[firstout] == ConnectionIndexA[secondout])
			{
				if (secondout == firstout + 1)
				{
					pDC->MoveTo(area.TopLeft().x + ((first + 1) * slen), area.TopLeft().y + ((difficulty - 1) * slen));	//将画笔移到随机位置
					pDC->LineTo(pDC->GetCurrentPosition().x, pDC->GetCurrentPosition().y + slen);							//画竖线
				}
				else
				{
					srand((int)time(0) * rand());
					int random = rand() % (secondout - 1 - firstout) + firstout;
					pDC->MoveTo(area.TopLeft().x + ((random + 1) * slen), area.TopLeft().y + ((difficulty - 1) * slen));	//将画笔移到随机位置
					pDC->LineTo(pDC->GetCurrentPosition().x, pDC->GetCurrentPosition().y + slen);							//画竖线
				}
			}
			firstout = secondout;
		}
		else
		{
			break;
		}
	}

	//绘制完毕，恢复环境
	pDC->SelectObject(oldPen);
}


void CLabyrinthGeneratorView::OnSolve()//求解迷宫
{
	// TODO: 在此添加命令处理程序代码

	//老鼠跟墙走？
}