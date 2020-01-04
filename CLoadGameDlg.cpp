// CLoadGameDlg.cpp: 实现文件
//

#include "pch.h"
#include "Tetris.h"
#include "CLoadGameDlg.h"
#include "afxdialogex.h"
#include <vector>


// CLoadGameDlg 对话框

IMPLEMENT_DYNAMIC(CLoadGameDlg, CDialogEx)

CLoadGameDlg::CLoadGameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOAD_GAME, pParent)
	, filename(_T(""))
{

}

CLoadGameDlg::~CLoadGameDlg()
{
}

void CLoadGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO, m_combo);
	DDX_CBString(pDX, IDC_COMBO, filename);
}


BEGIN_MESSAGE_MAP(CLoadGameDlg, CDialogEx)
	ON_BN_CLICKED(IDB_OK, &CLoadGameDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLoadGameDlg 消息处理程序


void CLoadGameDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (filename.IsEmpty())
	{
		MessageBox(TEXT("请选择存档文件！"));
		return;
	}

	OnOK();
}


BOOL CLoadGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 初始化下拉列表
	
	//遍历save目录下的存档文件
	CFileFind find;
	BOOL IsFind = find.FindFile(TEXT("save\\*.*"));
	while (IsFind)
	{
		IsFind = find.FindNextFile();
		if (find.IsDots() || find.IsDirectory())
		{
			continue;
		}
		else
		{
			CString filename = find.GetFileName();
			m_combo.AddString(filename);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
