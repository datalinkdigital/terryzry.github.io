#pragma once


// CDlgUser 对话框

class CDlgUser : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgUser)

public:
	CDlgUser(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgUser();

// 对话框数据
	enum { IDD = IDD_DIG_USER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButRegfinger();
	UINT m_CountTime1 ;
	UINT m_UINTTime;
	CString m_PointScreen;
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
