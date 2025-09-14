FROM gcc:14.3.0-trixie

WORKDIR /app

RUN apt-get update && apt-get install -y --no-install-recommends curl locales && rm -rf /var/lib/apt/lists/*

# Generate UTF-8 locale
RUN sed -i '/en_US.UTF-8/s/^# //g' /etc/locale.gen && locale-gen en_US.UTF-8
ENV LANG en_US.UTF-8
ENV LANGUAGE en_US:en
ENV LC_ALL en_US.UTF-8

COPY main.cpp .

RUN curl -sSL https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h -o httplib.h

RUN g++ -std=c++17 -O2 -I. main.cpp -o slave1 -pthread

CMD ["./slave1"]
