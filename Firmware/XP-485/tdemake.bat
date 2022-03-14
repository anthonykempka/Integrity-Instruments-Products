@echo off
set PATH=e:\pictools
MPASM.EXE  xp-485.asm /aINHX8M /q > tde.err
if ERRORLEVEL 1 goto ERROR
goto done
:error
if exist tde.err type tde.err >>xp-485.err
echo (last command = %CMDLINE%) >>xp-485.err
echo ERRORS! >>xp-485.err
:done
@cls
