; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CEZTermView
LastTemplate=CColorDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "EZTerm.h"
LastPage=0

ClassCount=7
Class1=CEZTermApp
Class2=CEZTermDoc
Class3=CEZTermView
Class4=CMainFrame

ResourceCount=6
Resource1=IDD_RTSCONTROL
Class5=CAboutDlg
Resource2=IDD_ABOUTBOX
Resource3=IDD_TERMINALSETTINGS
Resource4=IDD_COMMSETTINGS
Resource5=IDR_MAINFRAME
Class6=CDlgKeyLog
Class7=CSplashWnd
Resource6=IDD_DLGKBDDISPLAY

[CLS:CEZTermApp]
Type=0
HeaderFile=EZTerm.h
ImplementationFile=EZTerm.cpp
Filter=N
LastObject=CEZTermApp

[CLS:CEZTermDoc]
Type=0
HeaderFile=EZTermDoc.h
ImplementationFile=EZTermDoc.cpp
Filter=N
LastObject=ID_LOGGING_STARTLOGGING
BaseClass=CDocument
VirtualFilter=DC

[CLS:CEZTermView]
Type=0
HeaderFile=EZTermView.h
ImplementationFile=EZTermView.cpp
Filter=C
BaseClass=CScrollView
VirtualFilter=VWC
LastObject=ID_SETTINGS_KEYBOARDLOG


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=CMainFrame




[CLS:CAboutDlg]
Type=0
HeaderFile=EZTerm.cpp
ImplementationFile=EZTerm.cpp
Filter=D
LastObject=CAboutDlg
BaseClass=CDialog
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=7
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC_DDI_WEB,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT1,edit,1352730628

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_MRU_FILE1
Command6=ID_APP_EXIT
Command7=ID_SETTINGS_RTSDTR
Command8=ID_SETTINGS_COMMPORT
Command9=ID_SETTINGS_TERMINALEMULATION
Command10=ID_SETTINGS_COLORS_TRANSMIT
Command11=ID_SETTINGS_COLORS_RECIEVE
Command12=ID_SETTINGS_KEYBOARDLOG
Command13=ID_SETTINGS_HEXMODE
Command14=ID_LOGGING_SETFILENAME
Command15=ID_LOGGING_STARTLOGGING
Command16=ID_LOGGING_STOPLOGGING
Command17=ID_VIEW_TOOLBAR
Command18=ID_VIEW_STATUS_BAR
Command19=ID_APP_ABOUT
CommandCount=19

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_SETTINGS_RTSDTR
Command5=ID_SETTINGS_COMMPORT
Command6=ID_SETTINGS_TERMINALEMULATION
Command7=ID_SETTINGS_KEYBOARDLOG
Command8=ID_SETTINGS_HEXMODE
Command9=ID_APP_ABOUT
CommandCount=9

[DLG:IDD_COMMSETTINGS]
Type=1
Class=?
ControlCount=12
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_COMPORT,combobox,1344339971
Control4=IDC_BAUDRATE,combobox,1344339971
Control5=IDC_DATABITS,combobox,1344339971
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_PARITY,combobox,1344339971
Control12=IDC_STOPBITS,combobox,1344339971

[DLG:IDD_RTSCONTROL]
Type=1
Class=?
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_RTSON,button,1342242819
Control4=IDC_DTRON,button,1342242819

[DLG:IDD_TERMINALSETTINGS]
Type=1
Class=?
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_APPEND_LF_CHECK,button,1342242819
Control4=IDC_LCOAL_ECHO_CHECK,button,1342242819
Control5=IDC_HEX_MODE_CHECK,button,1342242819

[DLG:IDD_DLGKBDDISPLAY]
Type=1
Class=CDlgKeyLog
ControlCount=1
Control1=IDC_EDIT_KEYLOG,edit,1350631552

[CLS:CDlgKeyLog]
Type=0
HeaderFile=DlgKeyLog.h
ImplementationFile=DlgKeyLog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDlgKeyLog

[CLS:CSplashWnd]
Type=0
HeaderFile=Splash.h
ImplementationFile=Splash.cpp
BaseClass=CWnd
LastObject=CSplashWnd

