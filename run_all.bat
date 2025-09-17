@echo off
rem run_all.bat - inicia Docker (se necessário), VcXsrv (se existir), sobe server/slaves e rebuilda+roda client

rem 1) Check if Docker is running and start if needed
echo Checking Docker status...
docker version >nul 2>&1
if %errorlevel% neq 0 (
  echo Docker not running. Starting Docker Desktop...
  start "" "C:\Program Files\Docker\Docker\Docker Desktop.exe"
  echo Waiting for Docker to start...
  timeout /t 30 /nobreak >nul
  docker version >nul 2>&1
  if %errorlevel% neq 0 (
    echo Warning: Docker may not be ready yet. Please wait and try again if needed.
  )
) else (
  echo Docker is running.
)

rem 2) start VcXsrv helper if present
if exist start_vcxsrv.bat (
  echo Starting VcXsrv via start_vcxsrv.bat
  call start_vcxsrv.bat
) else (
  echo start_vcxsrv.bat not found, skipping X server start
)

rem 3) Build & start server and slaves with docker compose
echo Bringing up master and slaves via docker compose
docker compose up --build -d

rem 4) Rebuild client image and run
echo Rebuilding client image
cd client
docker rm -f client 2>nul || echo no existing client container
docker rmi -f client 2>nul || echo no existing client image
docker build -t client .

rem Run client container. If you want GUI via X11, set DISPLAY accordingly on host
rem Use host.docker.internal to reach host's ports; add host-gateway mapping for compatibility
echo Running client container
docker run -d --name client client

echo Done. Use 'docker ps' to check running containers and 'docker logs -f client' to see client output.
