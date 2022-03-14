# Microsoft Developer Studio Project File - Name="EZTerm" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=EZTerm - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EZTerm.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EZTerm.mak" CFG="EZTerm - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EZTerm - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "EZTerm - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EZTerm - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"Release/EZTerminal.exe"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "EZTerm - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "EZTerm - Win32 Release"
# Name "EZTerm - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\comdlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Comm32.cpp
# End Source File
# Begin Source File

SOURCE=.\Connection.cpp
# End Source File
# Begin Source File

SOURCE=.\ConsoleData.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgKeyLog.cpp
# End Source File
# Begin Source File

SOURCE=.\EZTerm.cpp
# End Source File
# Begin Source File

SOURCE=.\EZTerm.rc
# End Source File
# Begin Source File

SOURCE=.\EZTermDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\EZTermView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\RtsControlDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectColor.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\terminaldlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\comdlg.h
# End Source File
# Begin Source File

SOURCE=.\Comm32.h
# End Source File
# Begin Source File

SOURCE=.\Connection.h
# End Source File
# Begin Source File

SOURCE=.\ConsoleData.h
# End Source File
# Begin Source File

SOURCE=.\defines.h
# End Source File
# Begin Source File

SOURCE=.\DlgKeyLog.h
# End Source File
# Begin Source File

SOURCE=.\EZTerm.h
# End Source File
# Begin Source File

SOURCE=.\EZTermDoc.h
# End Source File
# Begin Source File

SOURCE=.\EZTermView.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RtsControlDlg.h
# End Source File
# Begin Source File

SOURCE=.\SelectColor.h
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\terminaldlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\cursorbm.bmp
# End Source File
# Begin Source File

SOURCE=.\res\EZTerm.ico
# End Source File
# Begin Source File

SOURCE=.\res\EZTerm.rc2
# End Source File
# Begin Source File

SOURCE=.\res\EZTermDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Hlink.cur
# End Source File
# Begin Source File

SOURCE=.\Splsh16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\EZTerm.reg
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section EZTerm : {E8C25536-1257-4DA3-B100-528C3C899192}
# 	1:20:IDD_TERMINALSETTINGS:104
# 	2:16:Resource Include:resource.h
# 	2:10:ENUM: enum:enum
# 	2:15:terminaldlg.cpp:terminaldlg.cpp
# 	2:18:CLASS: terminaldlg:terminaldlg
# 	2:19:Application Include:EZTerm.h
# 	2:20:IDD_TERMINALSETTINGS:IDD_TERMINALSETTINGS
# 	2:13:terminaldlg.h:terminaldlg.h
# End Section
# Section EZTerm : {87338369-232A-44B3-B327-84228E0B3528}
# 	2:14:CLASS: CComm32:CComm32
# 	2:10:Comm32.cpp:Comm32.cpp
# 	2:19:Application Include:EZTerm.h
# 	2:8:Comm32.h:Comm32.h
# End Section
# Section EZTerm : {DD6753EF-3B02-4A18-9547-660A9DA4852F}
# 	1:16:IDD_COMMSETTINGS:102
# 	2:16:Resource Include:resource.h
# 	2:16:IDD_COMMSETTINGS:IDD_COMMSETTINGS
# 	2:13:CLASS: comdlg:comdlg
# 	2:10:comdlg.cpp:comdlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:8:comdlg.h:comdlg.h
# 	2:19:Application Include:EZTerm.h
# End Section
# Section EZTerm : {BC27E300-A4EF-4F91-AD1D-91A1534BE59D}
# 	1:14:IDD_RTSCONTROL:103
# 	2:16:Resource Include:resource.h
# 	2:20:CLASS: RtsControlDlg:RtsControlDlg
# 	2:17:RtsControlDlg.cpp:RtsControlDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:14:IDD_RTSCONTROL:IDD_RTSCONTROL
# 	2:15:RtsControlDlg.h:RtsControlDlg.h
# 	2:19:Application Include:EZTerm.h
# End Section
# Section EZTerm : {72ADFD78-2C39-11D0-9903-00A0C91BC942}
# 	1:10:IDB_SPLASH:105
# 	2:21:SplashScreenInsertKey:4.0
# End Section
