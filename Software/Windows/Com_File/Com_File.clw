; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCom_FileDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Com_File.h"

ClassCount=4
Class1=CCom_FileApp
Class2=CCom_FileDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_COM_FILE_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Class4=CCommSetup
Resource4=IDD_COMM_SETUP

[CLS:CCom_FileApp]
Type=0
HeaderFile=Com_File.h
ImplementationFile=Com_File.cpp
Filter=N

[CLS:CCom_FileDlg]
Type=0
HeaderFile=Com_FileDlg.h
ImplementationFile=Com_FileDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_RCV_BYTES

[CLS:CAboutDlg]
Type=0
HeaderFile=Com_FileDlg.h
ImplementationFile=Com_FileDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_COM_FILE_DIALOG]
Type=1
Class=CCom_FileDlg
ControlCount=13
Control1=IDC_BEGIN,button,1342242816
Control2=IDC_HALT,button,1342242816
Control3=IDC_SEND_FILE,button,1342242816
Control4=IDC_COMM_SETUP,button,1342242816
Control5=IDCANCEL,button,1342242816
Control6=IDC_XMT_FILENAME,edit,1350631552
Control7=IDC_XMT_BROWSE,button,1342242816
Control8=IDC_RCV_FILENAME,edit,1350631552
Control9=IDC_RCV_BROWSE,button,1342242816
Control10=IDC_STATIC,button,1342177287
Control11=IDC_STATIC,button,1342177287
Control12=IDC_RCV_BYTES,edit,1350566016
Control13=IDC_STATIC,static,1342308352

[DLG:IDD_COMM_SETUP]
Type=1
Class=CCommSetup
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_COMPORT,combobox,1344340290
Control4=IDC_BAUD_RATE,combobox,1344340034
Control5=IDC_INTERVAL,edit,1350639744
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352

[CLS:CCommSetup]
Type=0
HeaderFile=CommSetup.h
ImplementationFile=CommSetup.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_COMPORT
VirtualFilter=dWC

