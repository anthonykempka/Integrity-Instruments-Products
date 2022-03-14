@echo off
del adc-dig.cod
set PATH=C:\PICTOOLS\;%PATH%
MPASM.EXE /w2 adc-dig.asm /q /eerrors.tde > tde.err
if not ERRORLEVEL 1 goto L0
echo (last command = %CMDLINE%) >> errors.tde
type tde.err >> errors.tde
goto ERROR
:L0
if exist adc-dig.cod copy adc-dig.cod symbols.tde
goto done
:error
echo ERRORS! >> errors.tde
:done
@cls
