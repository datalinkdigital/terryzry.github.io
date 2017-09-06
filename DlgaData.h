#pragma once


// CDlgaData 对话框

class CDlgaData : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgaData)

public:
	CDlgaData(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgaData();

// 对话框数据
	enum { IDD = IDD_DLG_USE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButSave();
};
