
// TetrisDoc.cpp: CTetrisDoc 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Tetris.h"
#endif

#include "TetrisDoc.h"

#include <propkey.h>

#include <fstream>
#include <iostream>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTetrisDoc

IMPLEMENT_DYNCREATE(CTetrisDoc, CDocument)

BEGIN_MESSAGE_MAP(CTetrisDoc, CDocument)
END_MESSAGE_MAP()

//游戏地图行数和列数
int CTetrisDoc::row = 30;
int CTetrisDoc::col = 20;
//方格尺寸
int CTetrisDoc::unit = 27;

// CTetrisDoc 构造/析构

CTetrisDoc::CTetrisDoc() noexcept :  game(row, col)
{
	// 计算单位方格大小
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	unit = screenHeight * 3 / 4 / row;
}

CTetrisDoc::~CTetrisDoc()
{
}

BOOL CTetrisDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CTetrisDoc 序列化

void CTetrisDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO:  在此添加存储代码
	}
	else
	{
		// TODO:  在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CTetrisDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CTetrisDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CTetrisDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CTetrisDoc 诊断

#ifdef _DEBUG
void CTetrisDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTetrisDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

int CTetrisDoc::getRow()
{
	return row;
}

int CTetrisDoc::getCol()
{
	return col;
}

int CTetrisDoc::getUnit()
{
	return unit;
}
// CTetrisDoc 命令


GameMap::BlockState CTetrisDoc::getBlockState(int r, int c)
{
	return game.getBlockState(r, c);
}


Shape CTetrisDoc::getCurrentShape()
{
	return game.getCurrentShape();
}


void CTetrisDoc::start()
{
	game.start();
}


void CTetrisDoc::pause()
{
	game.pause();
}


GameController::GameState CTetrisDoc::getCurrentState()
{
	return game.getCurrentState();
}


int CTetrisDoc::run()
{
	return game.run();
}


void CTetrisDoc::left()
{
	game.left();
}


void CTetrisDoc::right()
{
	game.right();
}


void CTetrisDoc::rotate()
{
	game.rotate();
}


void CTetrisDoc::reset()
{
	game = GameController(row, col);
}


void CTetrisDoc::fall()
{
	game.fall();
}


Shape CTetrisDoc::getCurrentShapePreview()
{
	return game.getCurrentShapePreview();
}


Shape CTetrisDoc::getNextShape()
{
	return game.getNextShape();
}


int CTetrisDoc::getScore()
{
	return game.getScore();
}

int CTetrisDoc::getLevel()
{
	return game.getLevel();
}


/*COLORREF CTetrisDoc::getBlockColor(int r, int c)
{
	return game.getBlockColor(r, c);
}*/


Shape::Type CTetrisDoc::getBlockType(int r, int c)
{
	return game.getBlockType(r, c);
}

//保存游戏
bool CTetrisDoc::saveGame(const char* filename)
{
	using namespace std;
	ofstream out(filename, ios::out | ios::binary);
	if (!out)
	{
		return false;
	}

	out.write((char*)&game, sizeof(game));

	return true;
}

//读取游戏
bool CTetrisDoc::loadGame(const char* filename)
{
	using namespace std;

	ifstream in(filename, ios::in | ios::binary);
	if (!in)
	{
		return false;
	}

	GameController backup = game;
	in.read((char*)&game, sizeof(game));

	//数据有效性检测
	if (game.getRow() != 30 || game.getCol() != 20)
	{
		game = backup;
		return false;
	}

	return true;
}

//保存游戏记录
bool CTetrisDoc::saveGameRecord()
{
	using namespace std;
	ofstream out("rec\\record.rd");
	if (!out)
	{
		return false;
	}

	int data = game.getScore() ^ 159;
	out.write((char*)&data, sizeof(data));
	return true;
}

//读取游戏记录
int CTetrisDoc::loadGameRecord()
{
	using namespace std;
	ifstream in("rec\\record.rd");
	if (!in)
	{
		return 0;
	}
	int data = 0 ^ 159;
	in.read((char*)& data, sizeof(data));
	return data ^ 159;
}


int CTetrisDoc::updateMap()
{
	return game.updateMap();
}


void CTetrisDoc::setScore(int score)
{
	game.setScore(score);
}


void CTetrisDoc::setLevel(int level)
{
	game.setLevel(level);
}

