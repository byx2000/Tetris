
// Tetris.cpp: 定义应用程序的类行为。
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Tetris.h"
#include "MainFrm.h"

#include "TetrisDoc.h"
#include "TetrisView.h"

#include <mmsystem.h>

#pragma comment(lib,"winmm.lib")

#include <gdiplus.h>
#pragma comment( lib, "gdiplus.lib" )

#include "vikey/Vikey.h"

using namespace Gdiplus;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTetrisApp

BEGIN_MESSAGE_MAP(CTetrisApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CTetrisApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CTetrisApp 构造

CTetrisApp::CTetrisApp() noexcept
{
	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Tetris.AppID.NoVersion"));

	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	mciSendString(TEXT("open audio\\bgm.mp3 alias MySong"), NULL, 0, NULL);
}

// 唯一的 CTetrisApp 对象

CTetrisApp theApp;


// CTetrisApp 初始化

BOOL CTetrisApp::InitInstance()
{
	//加密狗验证
	/*DWORD dwCount;
	DWORD dwRetCode = VikeyFind(&dwCount);
	if (dwRetCode)
	{
		MessageBox(NULL, TEXT("没有发现加密狗！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		exit(0);
	}

	DWORD hid;
	DWORD ret = VikeyGetHID(0, &hid);
	if (hid != 0x11e356ce)
	{
		MessageBox(NULL, TEXT("加密狗验证失败！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		exit(0);
	}*/

	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	//初始化gdi+
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要 AfxInitRichEdit2()
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)


	// 注册应用程序的文档模板。  文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CTetrisDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CTetrisView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 启用“DDE 执行”
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// 调度在命令行中指定的命令。  如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	//调整窗口大小
	int row = CTetrisDoc::getRow();
	int col = CTetrisDoc::getCol();
	int unit = CTetrisDoc::getUnit();
	RECT rc;
	rc.left = 100;
	rc.right = rc.left + (col + 2) * unit + 6 * unit;
	rc.top = 100;
	rc.bottom = rc.top + (row + 2) * unit + 1;
	::AdjustWindowRectEx(&rc, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, TRUE, 0);
	m_pMainWnd->MoveWindow(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);

	//窗口居中显示
	HWND hwnd = m_pMainWnd->m_hWnd;
	RECT rcWindow;
	GetWindowRect(hwnd, &rcWindow);
	int xWindow = rcWindow.right - rcWindow.left;
	int yWindow = rcWindow.bottom - rcWindow.top;
	int xScreen = GetSystemMetrics(SM_CXSCREEN);
	int yScreen = GetSystemMetrics(SM_CYSCREEN);
	MoveWindow(hwnd, (xScreen - xWindow) / 2, (yScreen - yWindow) / 2, xWindow, yWindow, TRUE);

	//设置窗口标题
	m_pMainWnd->SetWindowText(TEXT("Tetris-暂停"));

	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	// 启用拖/放
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}

// CTetrisApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框



CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CTetrisApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CTetrisApp 消息处理程序





int CTetrisApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	GdiplusShutdown(m_gdiplusToken);

	return CWinApp::ExitInstance();
}
