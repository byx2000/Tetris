#pragma once


// CLoadGameDlg 对话框

class CLoadGameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoadGameDlg)

public:
	CLoadGameDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CLoadGameDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOAD_GAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CComboBox m_combo;
	virtual BOOL OnInitDialog();
	CString filename;
};
