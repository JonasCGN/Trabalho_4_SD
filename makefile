start_visual:
	@start_vcxsrv.bat

run_client_gui:
	@docker rm -f client || true
	@docker rmi -f client || true
	cd client && docker build -t client .
	cd client && docker run -d --name client -e DISPLAY=host.docker.internal:0.0 -e QT_X11_NO_MITSHM=1 -e MASTER_URL="http://host.docker.internal:8080/process" client

run_server:
	docker compose up --build -d