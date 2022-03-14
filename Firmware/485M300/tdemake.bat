@echo off
set PATH=e:\pictools
MPASM.EXE  485m300.asm /aINHX8M /q > tde.err
if ERRORLEVEL 1 goto ERROR
goto done
:error
if exist tde.err type tde.err >>485m300.err
echo (last command = %CMDLINE%) >>485m300.err
echo ERRORS! >>485m300.err
:done
@cls
