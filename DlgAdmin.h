#pragma once


// CDlgAdmin 对话框

class CDlgAdmin : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAdmin)

public:
	CDlgAdmin(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAdmin();

// 对话框数据
	enum { IDD = IDD_DLG_ADMIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// 计时
	UINT m_UINTTime;
	UINT m_CountTime1=120;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CString m_strCursorPos;
	CString m_PointScreen;
	afx_msg void OnBnClickedBtnReg();
	// 人数
	int m_iCountPeople;
	afx_msg void OnBnClickedBinCng();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCancel();
	// 等级
	int m_iRank;
	afx_msg void OnBnClickedBtnDel();
};
