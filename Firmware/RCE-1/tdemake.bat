@echo off
set PATH=e:\pictools
MPASM.EXE /w2 rce-1.asm /q /erce-1.err > tde.err
if ERRORLEVEL 1 goto ERROR
goto done
:error
if exist tde.err type tde.err >>rce-1.err
echo (last command = %CMDLINE%) >>rce-1.err
echo ERRORS! >>rce-1.err
:done
@cls
