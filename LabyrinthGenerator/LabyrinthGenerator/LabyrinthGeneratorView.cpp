
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


void CLabyrinthGeneratorView::OnGenerate()
{
	// TODO: 在此添加命令处理程序代码
	CRect CR;
	GetClientRect(CR);
	CR.DeflateRect(100, 100);
	CDC* pDC = GetDC();
	if (CR.Width() >= CR.Height())
	{
		int lenth = CR.CenterPoint().y - CR.TopLeft().y;
		int rx1 = CR.CenterPoint().x - lenth;
		int rx2 = CR.CenterPoint().x + lenth;
		CR.SetRect(rx1, CR.TopLeft().y, rx2, CR.BottomRight().y);
	}
	pDC->Rectangle(CR);
	
}


void CLabyrinthGeneratorView::OnSolve()
{
	// TODO: 在此添加命令处理程序代码
}
