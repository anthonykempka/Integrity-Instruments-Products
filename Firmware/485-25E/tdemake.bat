@echo off
del 485e.cod
set PATH=E:\PICTOOLS\;%PATH%
MPASM.EXE  485e.asm /q /eerrors.tde > tde.err
if not ERRORLEVEL 1 goto L0
echo (last command = %CMDLINE%) >> errors.tde
type tde.err >> errors.tde
goto ERROR
:L0
if exist 485e.cod copy 485e.cod symbols.tde
goto done
:error
echo ERRORS! >> errors.tde
:done
@cls
