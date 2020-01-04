// CSaveGameDlg.cpp: 实现文件
//

#include "pch.h"
#include "Tetris.h"
#include "CSaveGameDlg.h"
#include "afxdialogex.h"


// CSaveGameDlg 对话框

IMPLEMENT_DYNAMIC(CSaveGameDlg, CDialogEx)

CSaveGameDlg::CSaveGameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SAVE_GAME, pParent)
	, filename(_T(""))
{

}

CSaveGameDlg::~CSaveGameDlg()
{
}

void CSaveGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDE_FILENAME, filename);
}


BEGIN_MESSAGE_MAP(CSaveGameDlg, CDialogEx)
	ON_BN_CLICKED(IDB_OK, &CSaveGameDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSaveGameDlg 消息处理程序


void CSaveGameDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (filename.IsEmpty())
	{
		MessageBox(TEXT("请输入文件名！"));
		return;
	}

	OnOK();
}
