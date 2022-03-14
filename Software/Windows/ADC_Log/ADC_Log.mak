# Microsoft Developer Studio Generated NMAKE File, Based on ADC_Log.dsp
!IF "$(CFG)" == ""
CFG=ADC_Log - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ADC_Log - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ADC_Log - Win32 Release" && "$(CFG)" !=\
 "ADC_Log - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ADC_Log.mak" CFG="ADC_Log - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ADC_Log - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ADC_Log - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ADC_Log - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ADC_Log.exe"

!ELSE 

ALL : "$(OUTDIR)\ADC_Log.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\ADC_Log.obj"
	-@erase "$(INTDIR)\ADC_Log.pch"
	-@erase "$(INTDIR)\ADC_Log.res"
	-@erase "$(INTDIR)\ADC_LogDlg.obj"
	-@erase "$(INTDIR)\Comm32.obj"
	-@erase "$(INTDIR)\ComSetup.obj"
	-@erase "$(INTDIR)\Configuration.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\ADC_Log.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\ADC_Log.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 
CPP_OBJS=.\Release/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\ADC_Log.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ADC_Log.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\ADC_Log.pdb" /machine:I386 /out:"$(OUTDIR)\ADC_Log.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ADC_Log.obj" \
	"$(INTDIR)\ADC_Log.res" \
	"$(INTDIR)\ADC_LogDlg.obj" \
	"$(INTDIR)\Comm32.obj" \
	"$(INTDIR)\ComSetup.obj" \
	"$(INTDIR)\Configuration.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\ADC_Log.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ADC_Log - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ADC_Log.exe"

!ELSE 

ALL : "$(OUTDIR)\ADC_Log.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\ADC_Log.obj"
	-@erase "$(INTDIR)\ADC_Log.pch"
	-@erase "$(INTDIR)\ADC_Log.res"
	-@erase "$(INTDIR)\ADC_LogDlg.obj"
	-@erase "$(INTDIR)\Comm32.obj"
	-@erase "$(INTDIR)\ComSetup.obj"
	-@erase "$(INTDIR)\Configuration.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\ADC_Log.exe"
	-@erase "$(OUTDIR)\ADC_Log.ilk"
	-@erase "$(OUTDIR)\ADC_Log.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\ADC_Log.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\ADC_Log.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ADC_Log.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)\ADC_Log.pdb" /debug /machine:I386 /out:"$(OUTDIR)\ADC_Log.exe"\
 /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\ADC_Log.obj" \
	"$(INTDIR)\ADC_Log.res" \
	"$(INTDIR)\ADC_LogDlg.obj" \
	"$(INTDIR)\Comm32.obj" \
	"$(INTDIR)\ComSetup.obj" \
	"$(INTDIR)\Configuration.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\ADC_Log.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "ADC_Log - Win32 Release" || "$(CFG)" ==\
 "ADC_Log - Win32 Debug"
SOURCE=.\ADC_Log.cpp

!IF  "$(CFG)" == "ADC_Log - Win32 Release"

DEP_CPP_ADC_L=\
	".\ADC_Log.h"\
	".\ADC_LogDlg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ADC_Log.obj" : $(SOURCE) $(DEP_CPP_ADC_L) "$(INTDIR)"\
 "$(INTDIR)\ADC_Log.pch"


!ELSEIF  "$(CFG)" == "ADC_Log - Win32 Debug"

DEP_CPP_ADC_L=\
	".\ADC_Log.h"\
	".\ADC_LogDlg.h"\
	

"$(INTDIR)\ADC_Log.obj" : $(SOURCE) $(DEP_CPP_ADC_L) "$(INTDIR)"\
 "$(INTDIR)\ADC_Log.pch"


!ENDIF 

SOURCE=.\ADC_Log.rc
DEP_RSC_ADC_LO=\
	".\res\ADC_Log.ico"\
	".\res\ADC_Log.rc2"\
	

"$(INTDIR)\ADC_Log.res" : $(SOURCE) $(DEP_RSC_ADC_LO) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\ADC_LogDlg.cpp

!IF  "$(CFG)" == "ADC_Log - Win32 Release"

DEP_CPP_ADC_LOG=\
	".\ADC_Log.h"\
	".\ADC_LogDlg.h"\
	".\Comm32.h"\
	".\ComSetup.h"\
	".\Configuration.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ADC_LogDlg.obj" : $(SOURCE) $(DEP_CPP_ADC_LOG) "$(INTDIR)"\
 "$(INTDIR)\ADC_Log.pch"


!ELSEIF  "$(CFG)" == "ADC_Log - Win32 Debug"

DEP_CPP_ADC_LOG=\
	".\ADC_Log.h"\
	".\ADC_LogDlg.h"\
	".\Comm32.h"\
	".\ComSetup.h"\
	".\Configuration.h"\
	

"$(INTDIR)\ADC_LogDlg.obj" : $(SOURCE) $(DEP_CPP_ADC_LOG) "$(INTDIR)"\
 "$(INTDIR)\ADC_Log.pch"


!ENDIF 

SOURCE=.\Comm32.cpp

!IF  "$(CFG)" == "ADC_Log - Win32 Release"

DEP_CPP_COMM3=\
	".\StdAfx.h"\
	

"$(INTDIR)\Comm32.obj" : $(SOURCE) $(DEP_CPP_COMM3) "$(INTDIR)"\
 "$(INTDIR)\ADC_Log.pch"


!ELSEIF  "$(CFG)" == "ADC_Log - Win32 Debug"


"$(INTDIR)\Comm32.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ADC_Log.pch"


!ENDIF 

SOURCE=.\ComSetup.cpp

!IF  "$(CFG)" == "ADC_Log - Win32 Release"

DEP_CPP_COMSE=\
	".\ADC_Log.h"\
	".\ComSetup.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ComSetup.obj" : $(SOURCE) $(DEP_CPP_COMSE) "$(INTDIR)"\
 "$(INTDIR)\ADC_Log.pch"


!ELSEIF  "$(CFG)" == "ADC_Log - Win32 Debug"

DEP_CPP_COMSE=\
	".\ADC_Log.h"\
	".\ComSetup.h"\
	

"$(INTDIR)\ComSetup.obj" : $(SOURCE) $(DEP_CPP_COMSE) "$(INTDIR)"\
 "$(INTDIR)\ADC_Log.pch"


!ENDIF 

SOURCE=.\Configuration.cpp

!IF  "$(CFG)" == "ADC_Log - Win32 Release"

DEP_CPP_CONFI=\
	".\ADC_Log.h"\
	".\Configuration.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Configuration.obj" : $(SOURCE) $(DEP_CPP_CONFI) "$(INTDIR)"\
 "$(INTDIR)\ADC_Log.pch"


!ELSEIF  "$(CFG)" == "ADC_Log - Win32 Debug"

DEP_CPP_CONFI=\
	".\ADC_Log.h"\
	".\Configuration.h"\
	

"$(INTDIR)\Configuration.obj" : $(SOURCE) $(DEP_CPP_CONFI) "$(INTDIR)"\
 "$(INTDIR)\ADC_Log.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "ADC_Log - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\ADC_Log.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ADC_Log.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ADC_Log - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)\ADC_Log.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ADC_Log.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

