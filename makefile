run_client_gui:
	@echo "Building image and starting client_container with DISPLAY forwarded (host.docker.internal:0.0)"
	cd client && docker build -t client .

	-@docker rm -f client_container
	cd client && docker run -d --name client_container -e DISPLAY=host.docker.internal:0.0 -e QT_X11_NO_MITSHM=1 -e MASTER_URL="http://host.docker.internal:8080/process" client

run_server:
	docker compose up --build -d