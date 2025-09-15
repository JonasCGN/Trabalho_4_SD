FROM gcc:14.3.0-trixie

WORKDIR /app

# Instala dependências do sistema para compilar aplicações Qt5
# pkg-config é usado para retornar as flags necessárias para compilação
RUN apt-get update && apt-get install -y --no-install-recommends \
    qtbase5-dev \
    pkg-config \
    # X11 & XCB runtime libraries commonly required by Qt xcb plugin
    libx11-6 \
    libxrender1 \
    libxrandr2 \
    libxfixes3 \
    libxi6 \
    libxcursor1 \
    libxcomposite1 \
    libxcb1 \
    libxcb-icccm4 \
    libxcb-image0 \
    libxcb-keysyms1 \
    libxcb-util1 \
    libxcb-xkb1 \
    libxcb-xinerama0 \
    libxcb-xinput0 \
    libxcb-render0 \
    libxcb-render-util0 \
    libxcb-shm0 \
    libxkbcommon-x11-0 \
    dbus-x11 \
    libglib2.0-0 \
    && rm -rf /var/lib/apt/lists/*

COPY . .

RUN curl -sSL https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h -o httplib.h

# Compila todos os arquivos .cpp em objetos com -fPIC (colocando -fPIC depois das cflags do pkg-config
# para garantir que qualquer -fPIE seja sobrescrito), e depois linka sem PIE.
RUN set -euo pipefail \
    && for src in *.cpp; do \
        echo "Compiling $src"; \
        g++ -std=c++17 -O2 $(pkg-config --cflags Qt5Widgets Qt5Network) -fPIC -c "$src" -o "${src%.cpp}.o"; \
    done \
    && echo "Linking GUI..." \
    && g++ -std=c++17 -O2 -fno-pie -o main *.o $(pkg-config --libs Qt5Widgets Qt5Network) -lpthread \
    && if [ -f cli.cpp ]; then \
        echo "Compiling CLI..."; \
        g++ -std=c++17 -O2 -fno-pie -I. cli.cpp httplib.h -o client_cli -lpthread; \
    else \
        echo "No cli.cpp - skipping CLI build"; \
    fi

# Observação: executar GUIs dentro de containers requer encaminhar o display (X11/Wayland)
# Para testes de compilação/integração isso é suficiente; execução pode precisar de ajustes.

COPY docker-entrypoint.sh /usr/local/bin/docker-entrypoint.sh

RUN chmod +x /usr/local/bin/docker-entrypoint.sh

ENV MASTER_URL=http://host.docker.internal:8080/process

ENV DISPLAY=host.docker.internal:0.0 

ENV QT_X11_NO_MITSHM=1

CMD ["./main"]