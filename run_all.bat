@echo off
rem run_all.bat - inicia VcXsrv (se existir), sobe server/slaves e rebuilda+roda client

rem 1) start VcXsrv helper if present
if exist start_vcxsrv.bat (
  echo Starting VcXsrv via start_vcxsrv.bat
  call start_vcxsrv.bat
) else (
  echo start_vcxsrv.bat not found, skipping X server start
)

rem 2) Build & start server and slaves with docker compose
echo Bringing up master and slaves via docker compose
docker compose up --build -d

rem 3) Rebuild client image and run
echo Rebuilding client image
cd client
docker rm -f client 2>nul || echo no existing client container
docker rmi -f client 2>nul || echo no existing client image
docker build -t client .

rem Run client container. If you want GUI via X11, set DISPLAY accordingly on host
rem Use host.docker.internal to reach host's ports; add host-gateway mapping for compatibility
echo Running client container
docker run -d --name client --add-host=host.docker.internal:host-gateway -e DISPLAY=host.docker.internal:0.0 -e QT_X11_NO_MITSHM=1 -e MASTER_URL="http://host.docker.internal:8080/process" client

echo Done. Use 'docker ps' to check running containers and 'docker logs -f client' to see client output.
