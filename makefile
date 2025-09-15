all: start_visual run_server run_client_gui

start_visual:
	@start_vcxsrv.bat

run_client_gui:
	@docker rm -f client || true
	@docker rmi -f client || true
	cd client && docker build -t client .
	cd client && docker run -d --name client client

run_server:
	docker compose up --build -d