FROM gcc:14.3.0-trixie

WORKDIR /app

RUN apt-get update && apt-get install -y --no-install-recommends curl && rm -rf /var/lib/apt/lists/*

COPY main.cpp .

RUN curl -sSL https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h -o httplib.h

RUN g++ -std=c++17 -O2 -I. main.cpp -o slave2 -pthread

CMD ["./slave2"]