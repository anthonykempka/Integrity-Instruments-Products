; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CIntegrityIOCtrl
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "IntegrityIO.h"
CDK=Y

ClassCount=3
Class1=CIntegrityIOCtrl
Class2=CIntegrityIOPropPage

ResourceCount=2
LastPage=0
Resource1=IDD_ABOUTBOX_INTEGRITYIO
Class3=CAboutBox
Resource2=IDD_PROPPAGE_INTEGRITYIO

[CLS:CIntegrityIOCtrl]
Type=0
HeaderFile=IntegrityIOCtl.h
ImplementationFile=IntegrityIOCtl.cpp
Filter=W
LastObject=CIntegrityIOCtrl
BaseClass=COleControl
VirtualFilter=wWC

[CLS:CIntegrityIOPropPage]
Type=0
HeaderFile=IntegrityIOPpg.h
ImplementationFile=IntegrityIOPpg.cpp
Filter=D
BaseClass=COlePropertyPage
VirtualFilter=idWC
LastObject=CIntegrityIOPropPage

[DLG:IDD_ABOUTBOX_INTEGRITYIO]
Type=1
Class=CAboutBox
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC_DDI_WEB,static,1342308352

[DLG:IDD_PROPPAGE_INTEGRITYIO]
Type=1
Class=CIntegrityIOPropPage
ControlCount=7
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDITCOMMPORT,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDITBAUDRATE,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDITCOMMTIMEOUT,edit,1350631552
Control7=IDC_STATIC,button,1342177287

[CLS:CAboutBox]
Type=0
HeaderFile=AboutBox.h
ImplementationFile=AboutBox.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_STATIC_DDI_WEB

