#pragma once

//#include <CString>

//using namespace ATL;

inline CString GetInstallPath(void)
{
	TCHAR FileName[MAX_PATH];
	GetModuleFileName(NULL, FileName, MAX_PATH);
	::CharLower( FileName );
	CString strFileName( FileName );
	int Index = strFileName.Find( _T("samples") );
	CString Path = strFileName.Mid(0, Index);
	//zhj modify V1.05   return Path;
	return _T("C:\\BlastTest\\");
};


inline CString GetReportTemplatePath(void)
{
	return GetInstallPath() + CString(_T("Samples\\reports\\"));
}

inline CString GetReportDataPath(void)
{
	return GetInstallPath() + CString(_T("Samples\\Data"));
}

inline CString GetReportDataPathFile(void)
{
	//return GetInstallPath() + CString(_T("Samples\\Data\\NorthWind.mdb"));
	return GetInstallPath() + CString(_T("Samples\\Data\\±¨∆∆ºÏ≤‚œµÕ≥.mdb"));

}

inline CString GetDatabaseConnectionString(void)
{
	return CString(_T("Provider=Microsoft.Jet.OLEDB.4.0;User ID=Admin;Data Source="))
		+ GetReportDataPathFile() + _T(";");
}