// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__4D71A147_33FC_4D4F_9173_3F33D6353F1F__INCLUDED_)
#define AFX_STDAFX_H__4D71A147_33FC_4D4F_9173_3F33D6353F1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <SDKDDKVer.h>

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
//wf
//Enable MFC project to support ATL
#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>
#include <afxcontrolbars.h>
//wf end

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__4D71A147_33FC_4D4F_9173_3F33D6353F1F__INCLUDED_)
