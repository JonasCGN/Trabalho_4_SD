@echo off
REM Verifica se vcxsrv.exe já está em execução
tasklist /FI "IMAGENAME eq vcxsrv.exe" | find /I "vcxsrv.exe" >nul
if %ERRORLEVEL%==0 (
    echo VcXsrv ja esta em execucao. Saindo.
) else (
    echo Iniciando VcXsrv...
    start "" "C:\Program Files\VcXsrv\vcxsrv.exe" :0 -multiwindow -clipboard -wgl -ac
    echo VcXsrv iniciado!
)
