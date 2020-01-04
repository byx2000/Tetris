#pragma once


// CSaveGameDlg 对话框

class CSaveGameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSaveGameDlg)

public:
	CSaveGameDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSaveGameDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SAVE_GAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString filename;
};
