// CFirstDlg.cpp: 实现文件
//

#include "pch.h"
#include "Tetris.h"
#include "CFirstDlg.h"
#include "afxdialogex.h"


// CFirstDlg 对话框

IMPLEMENT_DYNAMIC(CFirstDlg, CDialogEx)

CFirstDlg::CFirstDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FIRST, pParent)
{

}

CFirstDlg::~CFirstDlg()
{
}

void CFirstDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDS_CAPTION, m_caption);
	DDX_Control(pDX, IDS_TEXT, m_text);
}


BEGIN_MESSAGE_MAP(CFirstDlg, CDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CFirstDlg 消息处理程序


BOOL CFirstDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	COLORREF colorrrefRGB = RGB(0, 0, 0);
	::SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, ::GetWindowLongPtr(GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
	this->SetLayeredWindowAttributes(colorrrefRGB, (255 * 70) / 100, LWA_ALPHA);

	m_captionFont.CreatePointFont(180, TEXT("微软雅黑"));
	m_caption.SetFont(&m_captionFont);

	m_textFont.CreatePointFont(120, TEXT("微软雅黑"));
	m_text.SetFont(&m_textFont);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CFirstDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	SendMessage(WM_CLOSE);

	CDialogEx::OnLButtonDown(nFlags, point);
}


HBRUSH CFirstDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	//if (pWnd->GetDlgCtrlID() == IDC_STATIC1)
	{
		pDC->SetTextColor(RGB(255, 251, 240));
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(RGB(0, 0, 0));
		//return (HBRUSH)GetStockObject(NULL_BRUSH);
		return CreateSolidBrush(RGB(0, 0, 0));
	}
	//return hbr;
}
