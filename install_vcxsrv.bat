@echo off
echo === Instalador VcXsrv para Docker GUI ===
echo.

REM Verificar se está rodando como admin
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo Este script precisa ser executado como Administrador.
    echo Clique com o botao direito no arquivo e selecione "Executar como administrador"
    pause
    exit /b 1
)

echo Verificando se VcXsrv ja esta instalado...
reg query "HKLM\Software\Microsoft\Windows\CurrentVersion\Uninstall" /s /f "VcXsrv" >nul 2>&1
if %errorLevel% equ 0 (
    echo VcXsrv ja esta instalado!
    goto :create_scripts
)

echo Baixando VcXsrv...
powershell -Command "& {Invoke-WebRequest -Uri 'https://sourceforge.net/projects/vcxsrv/files/vcxsrv/1.20.14.0/vcxsrv-64.1.20.14.0.installer.exe/download' -OutFile '%TEMP%\vcxsrv-installer.exe'}"

echo Instalando VcXsrv...
"%TEMP%\vcxsrv-installer.exe" /S

echo Limpando arquivos temporarios...
del "%TEMP%\vcxsrv-installer.exe" 2>nul

:create_scripts
echo Criando scripts de configuracao...

echo @echo off > start_vcxsrv.bat
echo echo Iniciando VcXsrv... >> start_vcxsrv.bat
echo "C:\Program Files\VcXsrv\vcxsrv.exe" :0 -multiwindow -clipboard -wgl -ac >> start_vcxsrv.bat
echo echo VcXsrv iniciado! Agora voce pode executar suas aplicacoes Docker GUI. >> start_vcxsrv.bat

echo.
echo === Instalacao concluida! ===
echo Script criado: start_vcxsrv.bat
echo.
echo Para usar:
echo 1. Execute: start_vcxsrv.bat
echo 2. Execute: make run_client_gui
echo.
pause