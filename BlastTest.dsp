# Microsoft Developer Studio Project File - Name="BlastTest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=BlastTest - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BlastTest.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BlastTest.mak" CFG="BlastTest - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BlastTest - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "BlastTest - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BlastTest - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "BlastTest - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "BlastTest - Win32 Release"
# Name "BlastTest - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\_recordset.cpp
# End Source File
# Begin Source File

SOURCE=.\adodc.cpp
# End Source File
# Begin Source File

SOURCE=.\BlastTest.cpp
# End Source File
# Begin Source File

SOURCE=.\BlastTest.rc
# End Source File
# Begin Source File

SOURCE=.\BlastTestDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\commution\CalSum.cpp
# End Source File
# Begin Source File

SOURCE=.\column.cpp
# End Source File
# Begin Source File

SOURCE=.\columns.cpp
# End Source File
# Begin Source File

SOURCE=.\CRTHeight.cpp
# End Source File
# Begin Source File

SOURCE=.\datagrid.cpp
# End Source File
# Begin Source File

SOURCE=.\DebugDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DelayFun.cpp
# End Source File
# Begin Source File

SOURCE=".\commution\FATEK Protocol.cpp"
# End Source File
# Begin Source File

SOURCE=.\field.cpp
# End Source File
# Begin Source File

SOURCE=.\fields.cpp
# End Source File
# Begin Source File

SOURCE=.\font.cpp
# End Source File
# Begin Source File

SOURCE=.\HdyPress.cpp
# End Source File
# Begin Source File

SOURCE=.\picture.cpp
# End Source File
# Begin Source File

SOURCE=.\PID.cpp
# End Source File
# Begin Source File

SOURCE=.\commution\PORT.cpp
# End Source File
# Begin Source File

SOURCE=.\PrintReportDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportBillPrint.cpp
# End Source File
# Begin Source File

SOURCE=.\selbookmarks.cpp
# End Source File
# Begin Source File

SOURCE=.\SLComm.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TestConditon.cpp
# End Source File
# Begin Source File

SOURCE=.\TestRecordView.cpp
# End Source File
# Begin Source File

SOURCE=.\TestSpData.cpp
# End Source File
# Begin Source File

SOURCE=.\UserSetupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WfComm.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\_recordset.h
# End Source File
# Begin Source File

SOURCE=.\adodc.h
# End Source File
# Begin Source File

SOURCE=.\BlastTest.h
# End Source File
# Begin Source File

SOURCE=.\BlastTestDlg.h
# End Source File
# Begin Source File

SOURCE=.\commution\calSum.h
# End Source File
# Begin Source File

SOURCE=.\column.h
# End Source File
# Begin Source File

SOURCE=.\columns.h
# End Source File
# Begin Source File

SOURCE=.\CRTHeight.h
# End Source File
# Begin Source File

SOURCE=.\datagrid.h
# End Source File
# Begin Source File

SOURCE=.\DebugDlg.h
# End Source File
# Begin Source File

SOURCE=.\DelayFun.h
# End Source File
# Begin Source File

SOURCE=".\commution\FATEK Protocol.h"
# End Source File
# Begin Source File

SOURCE=.\field.h
# End Source File
# Begin Source File

SOURCE=.\fields.h
# End Source File
# Begin Source File

SOURCE=.\font.h
# End Source File
# Begin Source File

SOURCE=.\HdyPress.h
# End Source File
# Begin Source File

SOURCE=.\picture.h
# End Source File
# Begin Source File

SOURCE=.\PID.h
# End Source File
# Begin Source File

SOURCE=.\commution\PORT.h
# End Source File
# Begin Source File

SOURCE=.\PrintReportDlg.h
# End Source File
# Begin Source File

SOURCE=.\ReportBillPrint.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\selbookmarks.h
# End Source File
# Begin Source File

SOURCE=.\SLComm.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TestConditon.h
# End Source File
# Begin Source File

SOURCE=.\TestRecordView.h
# End Source File
# Begin Source File

SOURCE=.\TestSpData.h
# End Source File
# Begin Source File

SOURCE=.\UserSetupDlg.h
# End Source File
# Begin Source File

SOURCE=.\WfComm.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=".\res\Blast Bk.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\BlastTest.ico
# End Source File
# Begin Source File

SOURCE=.\res\BlastTest.rc2
# End Source File
# Begin Source File

SOURCE=".\res\爆炸指示灯.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\查看记录按钮.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\打开报表.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\放锤-灰.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\平台上升按钮.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\平台下降按钮.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\启动键盘.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\实验记录.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\实验记录按钮.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\实验设置.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\实验设置按钮.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\试样数据设置背景.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\手动平台.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\停止上升按钮.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\停止下降按钮.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\退出按钮.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\未报指示.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\选中标志.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\样品数据按钮.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\用户设置.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\用户设置背景.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\载入按钮.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\载入确定按钮.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\重锤释放.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\装药按钮.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\装药完成按钮.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\自动平台.bmp"
# End Source File
# End Group
# Begin Group "ADO"

# PROP Default_Filter ".c .h"
# Begin Source File

SOURCE=.\ADO\ADO.cpp
# End Source File
# Begin Source File

SOURCE=.\ADO\ADO.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=".\说明.txt"
# End Source File
# End Target
# End Project
# Section BlastTest : {47A738F1-7FAF-11D0-B148-00A0C922E820}
# 	2:5:Class:CAdodc
# 	2:10:HeaderFile:adodc.h
# 	2:8:ImplFile:adodc.cpp
# End Section
# Section BlastTest : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font.h
# 	2:8:ImplFile:font.cpp
# End Section
# Section BlastTest : {CDE57A52-8B86-11D0-B3C6-00A0C90AEA82}
# 	2:5:Class:CSelBookmarks
# 	2:10:HeaderFile:selbookmarks.h
# 	2:8:ImplFile:selbookmarks.cpp
# End Section
# Section BlastTest : {7BF80981-BF32-101A-8BBB-00AA00300CAB}
# 	2:5:Class:CPicture
# 	2:10:HeaderFile:picture.h
# 	2:8:ImplFile:picture.cpp
# End Section
# Section BlastTest : {67397AA3-7FB1-11D0-B148-00A0C922E820}
# 	2:21:DefaultSinkHeaderFile:adodc.h
# 	2:16:DefaultSinkClass:CAdodc
# End Section
# Section BlastTest : {0000054F-0000-0010-8000-00AA006D2EA4}
# 	2:5:Class:C_Recordset
# 	2:10:HeaderFile:_recordset.h
# 	2:8:ImplFile:_recordset.cpp
# End Section
# Section BlastTest : {CDE57A41-8B86-11D0-B3C6-00A0C90AEA82}
# 	2:5:Class:CDataGrid
# 	2:10:HeaderFile:datagrid.h
# 	2:8:ImplFile:datagrid.cpp
# End Section
# Section BlastTest : {CDE57A50-8B86-11D0-B3C6-00A0C90AEA82}
# 	2:5:Class:CColumns
# 	2:10:HeaderFile:columns.h
# 	2:8:ImplFile:columns.cpp
# End Section
# Section BlastTest : {0000054D-0000-0010-8000-00AA006D2EA4}
# 	2:5:Class:CFields
# 	2:10:HeaderFile:fields.h
# 	2:8:ImplFile:fields.cpp
# End Section
# Section BlastTest : {0000054C-0000-0010-8000-00AA006D2EA4}
# 	2:5:Class:CField
# 	2:10:HeaderFile:field.h
# 	2:8:ImplFile:field.cpp
# End Section
# Section BlastTest : {CDE57A43-8B86-11D0-B3C6-00A0C90AEA82}
# 	2:21:DefaultSinkHeaderFile:datagrid.h
# 	2:16:DefaultSinkClass:CDataGrid
# End Section
# Section BlastTest : {CDE57A4F-8B86-11D0-B3C6-00A0C90AEA82}
# 	2:5:Class:CColumn
# 	2:10:HeaderFile:column.h
# 	2:8:ImplFile:column.cpp
# End Section
