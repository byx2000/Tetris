
// TetrisView.cpp: CTetrisView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Tetris.h"
#endif

#include "TetrisDoc.h"
#include "TetrisView.h"
#include "MainFrm.h"
#include "CHelpDlg.h"
#include "CSaveGameDlg.h"
#include "CLoadGameDlg.h"

#include <fstream>
#include <string>

#include <mmsystem.h>



#pragma comment(lib,"winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTetrisView

IMPLEMENT_DYNCREATE(CTetrisView, CView)

BEGIN_MESSAGE_MAP(CTetrisView, CView)
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_GAME_START, &CTetrisView::OnGameStart)
	ON_UPDATE_COMMAND_UI(ID_GAME_START, &CTetrisView::OnUpdateGameStart)
	ON_COMMAND(ID_GAME_PAUSE, &CTetrisView::OnGamePause)
	ON_UPDATE_COMMAND_UI(ID_GAME_PAUSE, &CTetrisView::OnUpdateGamePause)
	ON_COMMAND(ID_GAME_RESTART, &CTetrisView::OnGameRestart)
	ON_COMMAND(ID_HELP, &CTetrisView::OnHelp)
	ON_COMMAND(ID_ABOUT, &CTetrisView::OnAbout)
	ON_COMMAND(ID_GAME_SAVE, &CTetrisView::OnGameSave)
	ON_COMMAND(ID_GAME_LOAD, &CTetrisView::OnGameLoad)
	ON_WM_CLOSE()
	ON_COMMAND(ID_GAME_RECORD, &CTetrisView::OnGameRecord)
	ON_COMMAND(ID_MUSIC, &CTetrisView::OnMusic)
	ON_UPDATE_COMMAND_UI(ID_MUSIC, &CTetrisView::OnUpdateMusic)
END_MESSAGE_MAP()

//COLORREF textBkColor = RGB(240, 240, 240);
COLORREF textColor = RGB(0, 0, 0);

// CTetrisView 构造/析构

CTetrisView::CTetrisView() noexcept 
	: row(CTetrisDoc::getRow()), col(CTetrisDoc::getCol()), unit(CTetrisDoc::getUnit()), pMemDC(NULL), duration(INIT_DURATION), currentScore(0), currentLevel(0), cx(0), cy(0),
	blockImg1(TEXT("img\\1.bmp")), blockImg2(TEXT("img\\2.bmp")), blockImg3(TEXT("img\\3.bmp")), blockImg4(TEXT("img\\4.bmp")), blockImg5(TEXT("img\\5.bmp"))
{
	// TODO: 在此处添加构造代码
	toastDeep = 200;
	toastFlag = false;
	toastDeepAddFlag = true;
	toastBottom = (row + 2) * unit;

	//读取音乐标记
	std::ifstream fin("tmp\\musicFlag");
	int i;
	fin >> i;
	musicFlag = (i == 1);
}

CTetrisView::~CTetrisView()
{
	delete pMemDC;
}

BOOL CTetrisView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CTetrisView 绘图

void CTetrisView::OnDraw(CDC* pDC)
{
	CTetrisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	if (pDoc->getCurrentState() == GameController::PAUSE)
	{
		drawPauseGame(&pMemDC->GetDC());
	}
	else
	{
		drawGame(&pMemDC->GetDC());
	}
	
	if (toastFlag)
	{
		drawToast(&pMemDC->GetDC(), toastText, toastDeep, toastBottom, 1000);
	}
	BitBlt(pDC->m_hDC, 0, 0, cx, cy, pMemDC->GetDC().m_hDC, 0, 0, SRCCOPY);
}


// CTetrisView 诊断

#ifdef _DEBUG
void CTetrisView::AssertValid() const
{
	CView::AssertValid();
}

void CTetrisView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTetrisDoc* CTetrisView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTetrisDoc)));
	return (CTetrisDoc*)m_pDocument;
}
#endif //_DEBUG


// CTetrisView 消息处理程序


void CTetrisView::drawSquare(CDC* pDC, int r, int c, COLORREF color)
{
	CBrush brush(color);
	HGDIOBJ oldBrush =  pDC->SelectObject(brush);
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	HGDIOBJ oldPen = pDC->SelectObject(pen);
	pDC->Rectangle(c * unit, r * unit, (c + 1) * unit + 1, (r + 1) * unit + 1);
	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);
}


void CTetrisView::drawBorder(CDC* pDC)
{
	/*Graphics g(pDC->m_hDC);
	for (int i = 0; i < col + 2; ++i)
	{
		g.DrawImage(&borderImg, i * unit, 0, unit, unit);
		g.DrawImage(&borderImg, i * unit, (row + 1) * unit, unit, unit);
	}
	for (int i = 0; i < row + 2; ++i)
	{
		g.DrawImage(&borderImg, 0, unit * (i + 1), unit, unit);
		g.DrawImage(&borderImg, (col + 1) * unit, unit * (i + 1), unit, unit);
	}*/

	CPen pen(PS_SOLID, 1, RGB(0x47, 0x59, 0x67));
	HGDIOBJ hOldPen = pDC->SelectObject(pen);

	for (int i = 1; i <= row + 1; ++i)
	{
		pDC->MoveTo(unit, i * unit);
		pDC->LineTo((col + 1) * unit, i * unit);
	}
	for (int i = 1; i <= col + 1; ++i)
	{
		pDC->MoveTo(i * unit, unit);
		pDC->LineTo(i * unit, (row + 1) * unit);
	}

	pDC->SelectObject(hOldPen);
}


void CTetrisView::drawGameMap(CDC* pDC)
{
	CTetrisDoc* pDoc = GetDocument();

	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			drawBlock(pDC, i + 1, j + 1, pDoc->getBlockType(i, j));
		}
	}
}


void CTetrisView::drawCurrentShape(CDC* pDC)
{
	CTetrisDoc* pDoc = GetDocument();
	Shape shape = pDoc->getCurrentShape();
	for (int i = 0; i < 4; ++i)
	{
		Shape::Type type = shape.getType();
		int r = shape.getBlockGlobalPosRow(i) + 1;
		int c = shape.getBlockGlobalPosCol(i) + 1;
		drawBlock(pDC, r, c, shape.getType());
	}
}


void CTetrisView::drawGame(CDC* pDC)
{
	clearClient(pDC);

	/*CFont font;
	font.CreatePointFont(500, TEXT("Consolas"));
	HGDIOBJ hOldFont = pDC->SelectObject(font);
	pDC->SetTextColor(RGB(0x4f, 0x4f, 0x4f));
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(TEXT("Tetris"), CRect(0, 0, (col + 2) * unit, (row / 2) * unit), DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	pDC->SelectObject(hOldFont);*/

	drawBorder(pDC);
	drawGameMap(pDC);
	drawShapePreview(pDC);
	drawCurrentShape(pDC);
	drawNextShape(pDC);
	drawScore(pDC);
	drawState(pDC);
	drawHelp(pDC);
}


void CTetrisView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CTetrisDoc* pDoc = GetDocument();

	// 键盘消息处理
	switch (nChar)
	{
	case VK_SPACE: //空格键：暂停、开始
		if (pDoc->getCurrentState() == GameController::PAUSE)
		{
			startGame();
		}
		else
		{
			pauseGame();
			//showPauseToast();
		}
		break;
	case VK_LEFT: //当前形状左移
		pDoc->left();
		Invalidate(FALSE);
		break;
	case VK_RIGHT: //当前形状右移
		pDoc->right();
		Invalidate(FALSE);
		break;
	case VK_UP: //旋转当前形状
		pDoc->rotate();
		Invalidate(FALSE);
		break;
	case VK_DOWN: //快速放置当前形状
		{
			pDoc->fall();
			
			/*int cLine = pDoc->updateMap();
			//int cLine = pDoc->run();
			if (cLine > 0)
			{
				showScoreToast(cLine);
				if (cLine <= 2)
				{
					PlaySound((LPCTSTR)IDR_ELIMINATE1, NULL, SND_RESOURCE | SND_ASYNC);
				}
				else
				{
					PlaySound((LPCTSTR)IDR_ELIMINATE2, NULL, SND_RESOURCE | SND_ASYNC);
				}
			}*/
			Invalidate(FALSE);
		}
		break;
	case VK_CONTROL: //Ctrl键加速
		pDoc->run();
		Invalidate(FALSE);
		break;
	/*case VK_RETURN: //作弊码
		pDoc->setLevel(pDoc->getLevel() + 1);
		pDoc->setScore(pDoc->getLevel() * 150);
		KillTimer(ID_GAMETIMER);
		duration = getDuration(pDoc->getLevel());
		SetTimer(ID_GAMETIMER, duration, NULL);
		break;*/
	default:
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CTetrisView::OnTimer(UINT_PTR nIDEvent)
{
	// 计时器消息

	//游戏主计时器
	if (nIDEvent == ID_GAMETIMER)
	{
		CTetrisDoc* pDoc = GetDocument();

		//游戏结束
		if (pDoc->getCurrentState() == GameController::GAMEOVER)
		{
			pauseGame();
			MessageBox(TEXT("游戏结束！"), TEXT("GameOver"), MB_OK | MB_ICONINFORMATION);
			updateRecord();
			restartGame();
		}
		//游戏未结束
		else
		{
			//运行游戏
			int cLine = pDoc->run();

			//若消除行数大于0行，则弹出加分提示
			if (cLine > 0)
			{
				//显示通知
				showScoreToast(cLine);
				//播放消除音乐
				if (musicFlag)
				{
					if (cLine <= 2)
					{
						PlaySound((LPCTSTR)IDR_ELIMINATE1, NULL, SND_RESOURCE | SND_ASYNC);
					}
					else
					{
						PlaySound((LPCTSTR)IDR_ELIMINATE2, NULL, SND_RESOURCE | SND_ASYNC);
					}
				}
			}

			//判断是否升级
			int level = pDoc->getLevel();
			if (level > currentLevel)
			{
				//弹出提示
				showLevelToast();

				//更新当前等级和速度
				currentLevel = level;
				duration = getDuration(currentLevel);
				endGameTimer();
				startGameTimer();
				updateTitle();
			}

		}

		Invalidate(FALSE);
	}
	//Toast动画绘制计时器
	else if (nIDEvent == ID_TOASTTIMER)
	{
		//static bool flag = true;
		/*if (toastDeepAddFlag)
		{
			toastDeep = max(0, toastDeep - 255 / (300 / 17));
		}
		else
		{
			toastDeep = min(200, toastDeep + 255 / (300 / 17));
		}
		
		toastBottom -= ((row + 2) * unit) / (600 / 17);
		Invalidate(FALSE);
		if (toastDeep == 0)
		{
			toastDeepAddFlag = false;
		}
		else if (toastDeep == 200 && !toastDeepAddFlag)
		{
			toastFlag = false;
			toastDeepAddFlag = true;
			KillTimer(ID_TOASTTIMER);
		}*/

		toastDeep = max(150, toastDeep - (255 - 150) / (500 / 17));
		toastBottom -= ((row + 2) * unit) / (500 / 17);
		Invalidate(FALSE);
		if (toastDeep == 150)
		{
			toastFlag = false;
			KillTimer(ID_TOASTTIMER);
		}
	}

	CView::OnTimer(nIDEvent);
}


void CTetrisView::clearClient(CDC* pDC)
{
	CRect rect(0, 0, (col + 2) * unit, (row + 2) * unit);
	//rect = CRect(unit, unit, (col + 1) * unit, (row + 1) * unit);
	//CRect rect;
	//CBrush brush(RGB(0xe4, 0xdc, 0xc9));
	CBrush brush;
	brush.CreateSolidBrush(RGB(0x2f, 0x3f, 0x4f));
	pDC->FillRect(&rect, &brush);

	rect = CRect((col + 2) * unit, 0, (col + 8) * unit, (row + 8) * unit);
	brush.DeleteObject();
	brush.CreateSolidBrush(RGB(200, 200, 200));
	pDC->FillRect(&rect, &brush);

	CFont font;
	font.CreatePointFont(unit * 40, TEXT("consolas"));

	LOGFONT lf;
	font.GetLogFont(&lf);
	lf.lfEscapement = -60;
	lf.lfUnderline = true;
	lf.lfWeight = FW_BOLD;
	font.DeleteObject();
	font.CreateFontIndirect(&lf);

	HGDIOBJ hOldFont = pDC->SelectObject(font);
	pDC->SetTextColor(RGB(0x10, 0x20, 0x30));
	pDC->SetBkMode(TRANSPARENT);
	GetClientRect(&rect);
	pDC->DrawText(TEXT("Tetris"), CRect(0, 0, (col + 2) * unit, (row + 2) * unit / 2), DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	pDC->SelectObject(hOldFont);
}


void CTetrisView::OnSize(UINT nType, int cx, int cy)
{
	//获取客户区尺寸
	this->cx = cx;
	this->cy = cy;

	if (cx > 0 && cy > 0)
	{
		//创建内存DC
		delete pMemDC;
		CClientDC dc(this);
		pMemDC = new CMemDC(dc, this);
		drawPauseGame(&pMemDC->GetDC());
	}

	CView::OnSize(nType, cx, cy);
}


void CTetrisView::clearHdc(HDC hdc, int cxClient, int cyClient)
{
	HBRUSH hBrush = ::CreateSolidBrush(RGB(255, 255, 255));
	::SelectObject(hdc, hBrush);
	RECT rect = { 0, 0, cxClient, cyClient };
	::FillRect(hdc, &rect, hBrush);
	::DeleteObject(hBrush);
}


BOOL CTetrisView::OnEraseBkgnd(CDC* pDC)
{
	// 不擦除背景
	return TRUE;
}


void CTetrisView::drawShapePreview(CDC* pDC)
{
	CTetrisDoc* pDoc = GetDocument();
	Shape shape = pDoc->getCurrentShapePreview();
	CBrush brush(HS_BDIAGONAL, RGB(255, 255, 255));
	HGDIOBJ hOldBrush = pDC->SelectObject(brush);
	CPen pen(PS_SOLID, 1, RGB(255, 255, 255));
	HGDIOBJ hOldPen = pDC->SelectObject(pen);
	for (int i = 0; i < 4; ++i)
	{
		pDC->Rectangle((shape.getBlockGlobalPosCol(i) + 1) * unit + 1, (shape.getBlockGlobalPosRow(i) + 1) * unit + 1,
			(shape.getBlockGlobalPosCol(i) + 2) * unit, (shape.getBlockGlobalPosRow(i) + 2) * unit);
	}

	pDC->SelectObject(hOldBrush);
	pDC->SelectObject(hOldPen);
}


void CTetrisView::OnGameStart()
{
	// 开始游戏
	startGame();
}


void CTetrisView::startGameTimer()
{
	SetTimer(ID_GAMETIMER, duration, NULL);
}


void CTetrisView::endGameTimer()
{
	KillTimer(ID_GAMETIMER);
}


void CTetrisView::startGame()
{
	// 开始游戏
	CTetrisDoc* pDoc = GetDocument();
	pDoc->start();

	currentScore = pDoc->getScore();
	currentLevel = pDoc->getLevel();
	duration = getDuration(currentLevel);
	

	startGameTimer();

	Invalidate(FALSE);
	pDoc->run();

	showLevelToast();

	//设置标题
	updateTitle();

	//播放背景音乐
	if (musicFlag)
	{
		playBGM();
	}
}


void CTetrisView::pauseGame()
{
	// 暂停游戏
	CTetrisDoc* pDoc = GetDocument();
	pDoc->pause();
	endGameTimer();
	((CMainFrame*)AfxGetMainWnd())->SetWindowTextW(TEXT("Tetris-暂停"));
	Invalidate(FALSE);

	KillTimer(ID_TOASTTIMER);
	toastFlag = false;

	//暂停背景音乐
	if (musicFlag)
	{
		pauseBGM();
	}
}


void CTetrisView::OnUpdateGameStart(CCmdUI* pCmdUI)
{
	CTetrisDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->getCurrentState() != GameController::RUNNING);
}


void CTetrisView::OnGamePause()
{
	pauseGame();
}


void CTetrisView::OnUpdateGamePause(CCmdUI* pCmdUI)
{
	CTetrisDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->getCurrentState() != GameController::PAUSE);
}


void CTetrisView::restartGame()
{
	// 重新开始
	CTetrisDoc* pDoc = GetDocument();
	pDoc->reset();
	currentScore = 0;
	currentLevel = 1;
	endGameTimer();
	duration = INIT_DURATION;
	((CMainFrame*)AfxGetMainWnd())->SetWindowTextW(TEXT("Tetris-暂停"));

	//从头播放背景音乐
	if (musicFlag)
	{
		replayBGM();
	}
}


void CTetrisView::OnGameRestart()
{
	pauseGame();
	int ret = MessageBox(TEXT("是否放弃当前游戏进度？"), TEXT("提示"), MB_OKCANCEL | MB_ICONINFORMATION);
	if (ret == IDCANCEL)
	{
		return;
	}

	restartGame();
	Invalidate(FALSE);
}


void CTetrisView::drawNextShape(CDC* pDC)
{
	//绘制文字
	CFont font;
	font.CreatePointFont(unit * 7, TEXT("微软雅黑"));
	setFontBold(font);
	HGDIOBJ hOldFont = pDC->SelectObject(font);
	pDC->SetTextColor(textColor);
	pDC->SetBkMode(TRANSPARENT);
	CRect rect((col + 2) * unit, unit, (col + 8) * unit, 2 * unit);
	int height = pDC->DrawText(TEXT("下一个"), rect, DT_CALCRECT | DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	rect = CRect((col + 2) * unit, unit, (col + 8) * unit, height + unit);
	pDC->DrawText(TEXT("下一个"), rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	pDC->SelectObject(hOldFont);

	//绘制外方框
	int dr = height / unit + 1;
	pDC->SelectObject(GetStockObject(WHITE_BRUSH));
	CBrush brush(RGB(0x2f, 0x3f, 0x4f)); 
	HGDIOBJ hOldBrush = pDC->SelectObject(brush);
	pDC->Rectangle((col + 3) * unit - 1, (dr + 1) * unit - 1, (col + 7) * unit + 2, (dr + 1) * unit + 4 * unit + 2);
	pDC->SelectObject(hOldBrush);

	//绘制形状
	CTetrisDoc* pDoc = GetDocument();
	Shape shape = pDoc->getNextShape();
	int width = shape.getMaxCol() - shape.getMinRow();


	int dc;
	if (width == 1 || width == 2)
	{
		dc = 1;
	}
	else
	{
		dc = 0;
	}
	for (int i = 0; i < 4; ++i)
	{
		drawBlock(pDC, shape.getBlockPosRow(i) + dr + 1, shape.getBlockPosCol(i) + col + 2 + dc, shape.getType());
	}

	CPen pen(PS_SOLID, 1, RGB(100, 100, 100));
	pDC->SelectObject(pen);
	pDC->MoveTo((col + 2) * unit, 8 * unit);
	pDC->LineTo((col + 8)* unit, 8 * unit);

	pDC->SelectObject(hOldFont);
}


void CTetrisView::drawScore(CDC* pDC)
{
	CString str;
	CTetrisDoc* pDoc = GetDocument();
	CFont font;
	font.CreatePointFont(unit * 7, TEXT("微软雅黑"));
	setFontBold(font);
	HGDIOBJ hOldFont = pDC->SelectObject(font);
	pDC->SetTextColor(textColor);
	pDC->SetBkMode(TRANSPARENT);
	CRect rect((col + 2) * unit, 6 * unit, (col + 8) * unit, 7 * unit);
	int height = pDC->DrawText(TEXT("得分"), &rect, DT_CALCRECT | DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	rect = CRect((col + 2) * unit, 8.5 * unit, (col + 8) * unit, 8.5 * unit + height);
	pDC->DrawText(TEXT("得分"), rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	font.DeleteObject();
	font.CreatePointFont(unit * 8, TEXT("微软雅黑"));
	rect = CRect((col + 2) * unit, 16 * unit + height, (col + 8) * unit, 17 * unit + height);
	str.Format(TEXT("%d"), pDoc->getScore());
	pDC->SelectObject(font);
	pDC->SetTextColor(RGB(0x79, 0x55, 0x48));
	int height2 = pDC->DrawText(str, rect, DT_CALCRECT | DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	rect = CRect((col + 2) * unit, 8.5 * unit + height, (col + 8) * unit, 8.5 * unit + height + height2);
	pDC->DrawText(str, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	pDC->SetTextColor(RGB(0, 0, 0));

	font.CreatePointFont(unit * 7, TEXT("微软雅黑"));
	setFontBold(font);
	pDC->SelectObject(font);
	pDC->SetTextColor(textColor);
	rect = CRect((col + 2) * unit, 8.5 * unit + height + height2 + unit, (col + 8) * unit, 8.5 * unit + 2 * height + height2 + unit);
	pDC->DrawText(TEXT("等级"), rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	font.DeleteObject();
	font.CreatePointFont(unit * 8, TEXT("微软雅黑"));
	str.Format(TEXT("%d"), pDoc->getLevel());
	pDC->SelectObject(font);
	pDC->SetTextColor(RGB(0x79, 0x55, 0x48));
	rect = CRect((col + 2) * unit, 8.5 * unit + 2 * height + height2 + unit, (col + 8) * unit, 8.5 * unit + 2 * height + 2 * height2 + unit);
	pDC->DrawText(str, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	pDC->SetTextColor(RGB(0, 0, 0));

	CPen pen(PS_SOLID, 1, RGB(100, 100, 100));
	HGDIOBJ hOldPen = pDC->SelectObject(pen);
	pDC->MoveTo((col + 2) * unit, 15 * unit);
	pDC->LineTo((col + 8) * unit, 15 * unit);

	pDC->SelectObject(hOldPen);
	pDC->SelectObject(hOldFont);
}


void CTetrisView::setFontBold(CFont& font)
{
	LOGFONT lf;
	font.GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	font.DeleteObject();
	font.CreateFontIndirect(&lf);
}


void CTetrisView::updateTitle()
{
	CString title;
	//title.Format(TEXT("Tetris-运行中    当前速度：%d"), duration);
	title.Format(TEXT("Tetris-运行中"));
	((CMainFrame*)AfxGetMainWnd())->SetWindowTextW(title);
}


void CTetrisView::drawHelp(CDC* pDC)
{
	CFont font;
	font.CreatePointFont(unit * 7, TEXT("微软雅黑"));
	setFontBold(font);
	HGDIOBJ hOldFont =  pDC->SelectObject(font);
	CRect rect((col + 2) * unit, 25 * unit, (col + 8) * unit, 26 * unit);
	int height = pDC->DrawText(TEXT("操作提示"), &rect, DT_CALCRECT | DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	rect = CRect((col + 2) * unit, 24 * unit, (col + 8) * unit, 24 * unit + height);
	pDC->DrawText(TEXT("操作提示"), rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	font.DeleteObject();
	font.CreatePointFont(unit * 4, TEXT("微软雅黑"));
	pDC->SelectObject(font);
	rect = CRect((col + 2) * unit, 26 * unit + height, (col + 8) * unit, 27 * unit + height);
	int height2 = pDC->DrawText(TEXT("左右键移动形状"), &rect, DT_CALCRECT | DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	rect = CRect((col + 2) * unit, 25 * unit + height, (col + 8) * unit, 25 * unit + height + height2);
	pDC->DrawText(TEXT("左右键移动形状"), rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	rect = CRect((col + 2) * unit, 26 * unit + height + height2, (col + 8) * unit, 27 * unit + height + height2);
	int height3 = pDC->DrawText(TEXT("↑键旋转形状"), &rect, DT_CALCRECT | DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	rect = CRect((col + 2) * unit, 25 * unit + height + height2, (col + 8) * unit, 25 * unit + height + height2 + height3);
	pDC->DrawText(TEXT("↑键旋转形状"), rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	rect = CRect((col + 2) * unit, 26 * unit + height + height2 + height3, (col + 8) * unit, 27 * unit + height + height2 + height3);
	int height4 = pDC->DrawText(TEXT("↓键快速放置"), &rect, DT_CALCRECT | DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	rect = CRect((col + 2) * unit, 25 * unit + height + height2 + height3, (col + 8) * unit, 25 * unit + height + height2 + height3 + height4);
	pDC->DrawText(TEXT("↓键快速放置"), rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	rect = CRect((col + 2) * unit, 26 * unit + height + height2 + height3, (col + 8) * unit, 27 * unit + height + height2 + height3);
	int height5 = pDC->DrawText(TEXT("Ctrl键加速下落"), &rect, DT_CALCRECT | DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	rect = CRect((col + 2) * unit, 25 * unit + height + height2 + height3 + height4, (col + 8) * unit, 25 * unit + height + height2 + height3 + height4 + height5);
	pDC->DrawText(TEXT("Ctrl键加速下落"), rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	pDC->SelectObject(hOldFont);
}


void CTetrisView::drawState(CDC* pDC)
{
	CFont font;
	font.CreatePointFont(unit * 7, TEXT("微软雅黑"));
	setFontBold(font);
	pDC->SelectObject(font);
	CRect rect((col + 2) * unit, 25 * unit, (col + 8) * unit, 26 * unit);
	int height = pDC->DrawText(TEXT("游戏状态"), &rect, DT_CALCRECT | DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	rect = CRect((col + 2) * unit, 16 * unit, (col + 8) * unit, 16 * unit + height);
	pDC->DrawText(TEXT("游戏状态"), rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	CTetrisDoc* pDoc = GetDocument();
	int height1 = 0;
	if (pDoc->getCurrentState() == GameController::RUNNING)
	{
		font.DeleteObject();
		font.CreatePointFont(unit * 8, TEXT("微软雅黑"));
		pDC->SelectObject(font);
		pDC->SetTextColor(RGB(0, 150, 0));
		CRect rect((col + 2) * unit, 25 * unit, (col + 8) * unit, 26 * unit);
		height1 = pDC->DrawText(TEXT("运行中"), &rect, DT_CALCRECT | DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		rect = CRect((col + 2) * unit, 17 * unit + height, (col + 8) * unit, 17 * unit + height + height1);
		pDC->DrawText(TEXT("运行中"), rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}
	else
	{
		font.DeleteObject();
		font.CreatePointFont(unit * 8, TEXT("微软雅黑"));
		pDC->SelectObject(font);
		pDC->SetTextColor(RGB(220, 0, 0));
		CRect rect((col + 2) * unit, 25 * unit, (col + 8) * unit, 26 * unit);
		height1 = pDC->DrawText(TEXT("暂停"), &rect, DT_CALCRECT | DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		rect = CRect((col + 2) * unit, 17 * unit + height, (col + 8) * unit, 17 * unit + height + height1);
		pDC->DrawText(TEXT("暂停"), rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}

	font.DeleteObject();
	font.CreatePointFont(unit * 4, TEXT("微软雅黑"));
	setFontBold(font);
	pDC->SelectObject(font);
	pDC->SetTextColor(RGB(0, 0, 255));
	rect = CRect((col + 2) * unit, 26 * unit, (col + 8) * unit, 27 * unit);
	int height2 = pDC->DrawText(TEXT("按空格键开始\n/暂停游戏"), &rect, DT_CALCRECT | DT_CENTER);
	rect = CRect((col + 2) * unit, 18 * unit + height + height1, (col + 8) * unit, 18 * unit + height + height1 + height2);
	pDC->DrawText(TEXT("按空格键开始/\n暂停游戏"), rect, DT_CENTER);

	pDC->SetTextColor(RGB(0, 0, 0));

	CPen pen(PS_SOLID, 1, RGB(100, 100, 100));
	pDC->SelectObject(pen);
	pDC->MoveTo((col + 2) * unit, 23 * unit);
	pDC->LineTo((col + 8) * unit, 23 * unit);
}


void CTetrisView::drawPauseGame(CDC* pDC)
{
	using namespace Gdiplus;

	clearClient(pDC);
	drawBorder(pDC);
	drawGameMap(pDC);
	drawShapePreview(pDC);
	drawCurrentShape(pDC);
	drawNextShape(pDC);
	drawScore(pDC);
	drawState(pDC);
	drawHelp(pDC);

	Graphics g(pDC->m_hDC);
	SolidBrush brush(Color(100, 0, 0, 0));
	g.FillRectangle(&brush, 0, 0, (col + 2) * unit, (row + 2) * unit);

	CFont font;
	font.CreatePointFont(500, TEXT("微软雅黑"));
	HGDIOBJ hOldFont = pDC->SelectObject(font);
	pDC->SetTextColor(RGB(255, 255, 255));
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(TEXT("暂停"), CRect(0, 0, (col + 2) * unit, (row + 2) * unit), DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	pDC->SelectObject(hOldFont);
}


void CTetrisView::OnHelp()
{
	// 弹出帮助对话框
	CTetrisDoc* pDoc = GetDocument();
	bool flag = false;
	if (pDoc->getCurrentState() == GameController::RUNNING)
	{
		pauseGame();
		flag = true;
	}
	CHelpDlg dlg;
	dlg.DoModal();
	//CFirstDlg dlg;
	//dlg.DoModal();

	if (flag)
	{
		startGame();
	}
}


void CTetrisView::OnAbout()
{
	// 弹出关于对话框
	CTetrisDoc* pDoc = GetDocument();
	bool flag = false;
	if (pDoc->getCurrentState() == GameController::RUNNING)
	{
		pauseGame();
		flag = true;
	}
	CAboutDlg dlg;
	dlg.DoModal();
	if (flag)
	{
		startGame();
	}
}


void CTetrisView::OnGameSave()
{
	//保存游戏
	pauseGame();

	CSaveGameDlg dlg;
	int ret = dlg.DoModal();
	//确认保存游戏
	if (ret == IDOK)
	{
		CString filename;
		filename.Format(TEXT("save\\%s"), dlg.filename.GetBuffer());
		std::string str = CT2A(filename.GetBuffer());
		CTetrisDoc* pDoc = GetDocument();
		if (pDoc->saveGame(str.c_str()))
		{
			MessageBox(TEXT("保存成功！"));
		}
		else
		{
			MessageBox(TEXT("保存失败！"));
		}
	}
}


void CTetrisView::OnGameLoad()
{
	//载入游戏
	pauseGame();

	int ret = MessageBox(TEXT("是否放弃当前游戏进度？"), TEXT("提示"), MB_OKCANCEL | MB_ICONINFORMATION);
	if (ret == IDCANCEL)
	{
		return;
	}

	CLoadGameDlg dlg;
	ret = dlg.DoModal();
	//确认载入游戏
	if (ret == IDOK)
	{
		CString filename;
		filename.Format(TEXT("save\\%s"), dlg.filename.GetBuffer());
		std::string str = CT2A(filename.GetBuffer());
		CTetrisDoc* pDoc = GetDocument();
		if (pDoc->loadGame(str.c_str()))
		{
			MessageBox(TEXT("载入成功！"));
		}
		else
		{
			MessageBox(TEXT("载入失败！"));
		}
	}
	
	Invalidate(FALSE);
}


int CTetrisView::getDuration(int level)
{
	return max(50, INIT_DURATION - 30 * level);
}


//查看游戏记录
void CTetrisView::OnGameRecord()
{
	CTetrisDoc* pDoc = GetDocument();
	int record = pDoc->loadGameRecord();
	CString str;
	str.Format(TEXT("历史最高得分：%d"), record);
	MessageBox(str, TEXT("游戏记录"), MB_OK);
}


//更新游戏记录
void CTetrisView::updateRecord()
{
	CTetrisDoc* pDoc = GetDocument();
	int record = pDoc->loadGameRecord();
	int score = pDoc->getScore();
	if (score > record)
	{
		pDoc->saveGameRecord();
		CString str;
		str.Format(TEXT("本次得分：%d\n恭喜您创造了新纪录！"), pDoc->getScore());
		MessageBox(str, TEXT("新纪录"), MB_OK | MB_ICONINFORMATION);
	}
}


void CTetrisView::drawToast(CDC* pDC, const TCHAR* text, int deep, int bottom, int msec)
{
	CRect rect(0, 0, (col + 2) * unit, bottom);
	CFont font;
	font.CreatePointFont(500, TEXT("微软雅黑"));
	HGDIOBJ hOldFont = pDC->SelectObject(font);
	pDC->SetTextColor(RGB(deep, deep, deep));
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(text, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	pDC->SelectObject(hOldFont);
}


void CTetrisView::showToast(CString text)
{
	KillTimer(ID_TOASTTIMER);
	toastFlag = true;
	toastDeepAddFlag = true;
	toastText = text;
	toastDeep = 255;
	toastBottom = (row + 2) * unit;
	SetTimer(ID_TOASTTIMER, 17, NULL);
}


void CTetrisView::showLevelToast()
{
	CTetrisDoc* pDoc = GetDocument();
	CString str;
	str.Format(TEXT("Level %d"), pDoc->getLevel());
	showToast(str);
}


void CTetrisView::showPauseToast()
{
	showToast(TEXT("Pause"));
}


void CTetrisView::showScoreToast(int cLine)
{
	if (cLine == 1)
	{
		showToast(TEXT("+10"));
	}
	else if (cLine == 2)
	{
		showToast(TEXT("+30"));
	}
	else if (cLine == 3)
	{
		showToast(TEXT("+60"));
	}
	else
	{
		showToast(TEXT("+100"));
	}
}


//音效开启与关闭
void CTetrisView::OnMusic()
{
	musicFlag = !musicFlag;
	if (musicFlag)
	{
		playBGM();
	}
	else
	{
		replayBGM();
	}
}


void CTetrisView::OnUpdateMusic(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(musicFlag);
}


//播放背景音乐
void CTetrisView::playBGM()
{
	mciSendString(TEXT("play MySong repeat"), NULL, 0, NULL);
}


//暂停背景音乐
void CTetrisView::pauseBGM()
{
	mciSendString(TEXT("pause MySong"), NULL, 0, NULL);
}


//重新播放背景音乐
void CTetrisView::replayBGM()
{
	mciSendString(TEXT("close MySong"), NULL, 0, NULL);
	mciSendString(TEXT("open audio\\bgm.mp3 alias MySong"), NULL, 0, NULL);
}


void CTetrisView::drawBlock(CDC* pDC, int r, int c, Shape::Type type)
{
	if (type == Shape::Empty)
	{
		return;
	}

	
	Graphics g(pDC->m_hDC);
	if (type == Shape::L || type == Shape::RL)
	{
		g.DrawImage(&blockImg1, c * unit + 1, r * unit + 1, unit - 1, unit - 1);
	}
	else if (type == Shape::Z || type == Shape::RZ)
	{
		g.DrawImage(&blockImg2, c * unit + 1, r * unit + 1, unit - 1, unit - 1);
	}
	else if (type == Shape::Square)
	{
		g.DrawImage(&blockImg3, c * unit + 1, r * unit + 1, unit - 1, unit - 1);
	}
	else if (type == Shape::T)
	{
		g.DrawImage(&blockImg4, c * unit + 1, r * unit + 1, unit - 1, unit - 1);
	}
	else
	{
		g.DrawImage(&blockImg5, c * unit + 1, r * unit + 1, unit - 1, unit - 1);
	}
}


bool CTetrisView::getMusicFlag()
{
	return musicFlag;
}
