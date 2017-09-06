// LogWriter.cpp : implementation file
//

#include "stdafx.h"
#include "LogWriter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogWriter
// shell api，与目录创建相关
#include "shlwapi.h" 
#pragma comment(lib, "shlwapi.lib")

CLogWriter::CLogWriter()
{
	m_strFilePath = _T("d:\\finger\\log");   // 文件夹的路径，可以是相对的，可以是绝对的。
	//   判断路径是否存在   
	if   (! PathIsDirectory(m_strFilePath))   
	{   
			if   (! CreateDirectory(m_strFilePath, NULL))   
			{   
                if (AfxMessageBox(_T("日志路径创建失败..."), MB_YESNO) == IDYES)   
					return;   
            }   
	} 
	m_strFilePath += _T("\\fingerlogin.log"); 
}

CLogWriter::~CLogWriter()
{
}


void CLogWriter::WriteString(TCHAR *ch, int nLine, TCHAR *fileName)
{
	CString str;
	str.Format(_T("%s"), ch);
	WriteString(str,nLine,fileName);
}

void CLogWriter::WriteString(CString str, int nLine, TCHAR *fileName)
{
	CString strAdd;
	CTime ti = CTime::GetCurrentTime();
	strAdd.Format(_T("%d/%2d/%2d %2d:%2d:%2d  "), ti.GetYear(), ti.GetMonth(), ti.GetDay(), ti.GetHour(), ti.GetMinute(), ti.GetSecond());
	CFile t;
	if(!t.Open(m_strFilePath, CFile::modeWrite, NULL))
	{
		t.Open(m_strFilePath, CFile::modeWrite | CFile::modeCreate, NULL);
	}
	str = strAdd + str;
	strAdd.Format(_T("  line: %d    file: %s\r\n"), nLine, fileName);
	str += strAdd;
	t.SeekToEnd();
	t.Write(str.GetBuffer(str.GetLength()), str.GetLength());
	t.Close();
}

void CLogWriter::WriteString(CString str)
{
	CString strAdd, strWirte;
	CTime ti = CTime::GetCurrentTime();
	strAdd.Format(_T("%d/%2d/%2d %2d:%2d:%2d  "), ti.GetYear(), ti.GetMonth(), ti.GetDay(), ti.GetHour(), ti.GetMinute(), ti.GetSecond());//% 2d是C语言中printf函数的输出格式说明符。使输出的int型的数值以2位的固定位宽输出。如果不足2位，则在前面补空格；如果超过2位，则按实际位数输出。
	strWirte.Format(_T("%s %-10s"), strAdd, str);
	//TRACE(_T("%s\r\n"), str);
	str = strWirte + _T("\r\n");
	CFile t;
	if(!t.Open(m_strFilePath, CFile::modeWrite, NULL))
	{
		t.Open(m_strFilePath, CFile::modeWrite | CFile::modeCreate, NULL);
	}
	t.SeekToEnd();
	t.Write(str.GetBuffer(str.GetLength()), str.GetLength());
	t.Close();
}

void CLogWriter::WriteString(TCHAR *ch)
{
	CString str;
	str.Format(_T("%s"), ch);
	WriteString(str);
}
/////////////////////////////////////////////////////////////////////////////
// CLogWriter message handlers
