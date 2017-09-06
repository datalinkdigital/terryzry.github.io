#pragma once


// CPassDlg 对话框

class CPassDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPassDlg)

public:
	CPassDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPassDlg();

// 对话框数据
	enum { IDD = IDD_DLG_PASS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedFinger();
	CString m_strRankPass;
};
