; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CADC_LogDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ADC_Log.h"

ClassCount=5
Class1=CADC_LogApp
Class2=CADC_LogDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_ADC_LOG_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_COMM_SETUP
Class4=CComSetup
Resource4=IDD_ABOUTBOX
Class5=CConfiguration
Resource5=IDD_CONFIG_DIALOG

[CLS:CADC_LogApp]
Type=0
HeaderFile=ADC_Log.h
ImplementationFile=ADC_Log.cpp
Filter=N

[CLS:CADC_LogDlg]
Type=0
HeaderFile=ADC_LogDlg.h
ImplementationFile=ADC_LogDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_CONFIG

[CLS:CAboutDlg]
Type=0
HeaderFile=ADC_LogDlg.h
ImplementationFile=ADC_LogDlg.cpp
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

[DLG:IDD_ADC_LOG_DIALOG]
Type=1
Class=CADC_LogDlg
ControlCount=14
Control1=IDCANCEL,button,1342242816
Control2=IDC_BEGIN,button,1342242816
Control3=IDC_HALT,button,1342242816
Control4=IDC_CONFIG,button,1342242816
Control5=IDC_COMM_SETUP,button,1342242816
Control6=IDC_FILENAME,edit,1350631552
Control7=IDC_BROWSE,button,1342242816
Control8=IDC_STATIC,button,1342177287
Control9=IDC_STATIC,static,1342308352
Control10=IDC_LAST_COMMAND,edit,1350566016
Control11=IDC_STATIC,static,1342308352
Control12=IDC_LAST_RESPONSE,edit,1350566016
Control13=IDC_SAMPLE_COUNT,edit,1350566016
Control14=IDC_STATIC,static,1342308352

[DLG:IDD_COMM_SETUP]
Type=1
Class=CComSetup
ControlCount=11
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_COMPORT,combobox,1344340290
Control4=IDC_BAUD_RATE,combobox,1344340034
Control5=IDC_RS485_Radio,button,1342308361
Control6=IDC_RS232_Radio,button,1342177289
Control7=IDC_MODULE_ADDRESS,edit,1350639744
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,button,1342177287
Control11=IDC_STATIC,static,1342308352

[CLS:CComSetup]
Type=0
HeaderFile=ComSetup.h
ImplementationFile=ComSetup.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CComSetup

[DLG:IDD_CONFIG_DIALOG]
Type=1
Class=CConfiguration
ControlCount=34
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_SAMPLE1,combobox,1344340034
Control4=IDC_UNI_SAMPLE1,button,1342242819
Control5=IDC_UNI_SAMPLE2,button,1342242819
Control6=IDC_UNI_SAMPLE3,button,1342242819
Control7=IDC_UNI_SAMPLE4,button,1342242819
Control8=IDC_UNI_SAMPLE5,button,1342242819
Control9=IDC_UNI_SAMPLE6,button,1342242819
Control10=IDC_UNI_SAMPLE7,button,1342242819
Control11=IDC_UNI_SAMPLE8,button,1342242819
Control12=IDC_LOG_INTERVAL,edit,1350639744
Control13=IDC_VREF,combobox,1344340034
Control14=IDC_FILTER,edit,1350639744
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,static,1342308352
Control22=IDC_STATIC,static,1342308352
Control23=IDC_STATIC,static,1342308352
Control24=IDC_STATIC,static,1342308352
Control25=IDC_STATIC,static,1342308352
Control26=IDC_STATIC,static,1342308352
Control27=IDC_OFFSET,button,1342242816
Control28=IDC_SAMPLE2,combobox,1344340034
Control29=IDC_SAMPLE3,combobox,1344340034
Control30=IDC_SAMPLE4,combobox,1344340034
Control31=IDC_SAMPLE5,combobox,1344340034
Control32=IDC_SAMPLE6,combobox,1344340034
Control33=IDC_SAMPLE7,combobox,1344340034
Control34=IDC_SAMPLE8,combobox,1344340034

[CLS:CConfiguration]
Type=0
HeaderFile=Configuration.h
ImplementationFile=Configuration.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_FILTER

