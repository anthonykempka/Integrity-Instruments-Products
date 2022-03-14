@echo off
set PATH=e:\pictools
MPASM.EXE  232m200.asm /aINHX8M /q > tde.err
if ERRORLEVEL 1 goto ERROR
goto done
:error
if exist tde.err type tde.err >>232m200.err
echo (last command = %CMDLINE%) >>232m200.err
echo ERRORS! >>232m200.err
:done
@cls
