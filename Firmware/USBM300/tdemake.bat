@echo off
set PATH=e:\pictools
MPASM.EXE  usbm300.asm /q /eusbm300.err > tde.err
if ERRORLEVEL 1 goto ERROR
goto done
:error
if exist tde.err type tde.err >>usbm300.err
echo (last command = %CMDLINE%) >>usbm300.err
echo ERRORS! >>usbm300.err
:done
@cls
