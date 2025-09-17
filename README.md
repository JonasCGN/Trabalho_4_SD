# Sistema mestre-escravo (C++17, Docker)

Este repositório contém uma implementação simples em C++ de um serviço Mestre que recebe requisições do Cliente e distribui trabalho para dois Escravos (`slave1`, `slave2`). A comunicação é feita por HTTP REST usando a biblioteca single-header `cpp-httplib`.

Estrutura
- `server/` - código do Mestre
- `slave1/`, `slave2/` - código dos Escravos
- `client/` - exemplo de cliente (CLI)
- `docker-compose.yml` - orquestra os serviços

Pré-requisitos
- Docker e Docker Compose
<!-- - (Opcional) g++/clang/MSVC se for compilar localmente -->

Subir com Docker (recomendado)

1. Build e iniciar os serviços:

```bash
docker-compose up --build
```

2. Testar o endpoint do Mestre (do host):

```bash
curl -X POST http://localhost:8080/process -d 'hello'
```

Cliente CLI (envio de arquivo .txt)

Há um cliente CLI em `client/` que envia um arquivo texto ao Mestre.

Build e executar via Docker (PowerShell):

```powershell
cd client;
docker build -t client_cli .;
docker run --rm -v "${PWD}:/data" client_cli /data/meuarquivo.txt http://host.docker.internal:8080
```

Compilar e executar localmente (opcional):

```powershell
g++ -std=c++17 client/cli.cpp -Iclient -o client_cli -pthread
./client_cli meuarquivo.txt http://localhost:8080
```

<!-- Rodando sem Docker (desenvolvimento)

```bash
g++ -std=c++17 server/main.cpp -o master -pthread
g++ -std=c++17 slave1/main.cpp -o slave1 -pthread
g++ -std=c++17 slave2/main.cpp -o slave2 -pthread

# Em terminais separados:
./slave1
./slave2
./master
``` -->

Usando interface gráfica no Windows (VcXsrv)

Se você precisa executar aplicações gráficas dentro de containers e visualizar no Windows, uma opção simples é usar o VcXsrv (X server para Windows).

- Link: https://sourceforge.net/projects/vcxsrv/

Passos rápidos (Windows + Docker Desktop)

1. Baixe e instale o VcXsrv.
2. Execute o XLaunch e escolha "Multiple windows" (ou outra opção que preferir). Use Display number 0. Para desenvolvimento local, marcar "Disable access control" simplifica a configuração (atenção: reduz segurança).
3. Permita o VcXsrv no Firewall do Windows se solicitado.

Configurar containers para usar o X server

Opção A — Docker Desktop (Windows):

Defina a variável de ambiente `DISPLAY` como `host.docker.internal:0.0`. Exemplo genérico:

```powershell
docker run --rm -e DISPLAY=host.docker.internal:0.0 -v "${PWD}:/work" my-gui-image
```

No `docker-compose.yml` você pode adicionar:

```yaml
services:
	gui-service:
		image: my-gui-image
		environment:
			- DISPLAY=host.docker.internal:0.0
```

Opção B — WSL2 (quando usar Docker via WSL2):

No WSL2, exporte o DISPLAY para o IP do Windows host:

```bash
export DISPLAY=$(grep nameserver /etc/resolv.conf | awk '{print $2}'):0.0
```

Dicas e problemas comuns
- Se `host.docker.internal` não for resolvido, adicione `--add-host=host.docker.internal:host-gateway` ao `docker run` (Docker 20.10+).
- Se a janela não abrir, verifique o Firewall do Windows e as configurações do VcXsrv.
- Para desenvolvimento local, desabilitar o controle de acesso do X server é prático, mas não seguro para ambientes públicos.

Observações do projeto
- Implementação didática: JSON construído manualmente e tratamento de erros simples.
- Para produção, recomenda-se usar `nlohmann/json`, CMake, logs e políticas de retry/timeout mais robustas.

Licença / Créditos

Projeto para fins educacionais/demonstração.
