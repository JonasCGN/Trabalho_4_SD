// master: recebe requisição do cliente, consulta /health dos escravos, envia /process a cada um em paralelo
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <httplib.h>

using namespace std::chrono_literals;

int main() {
    httplib::Server svr;

    // Endpoints configuráveis via variáveis de ambiente
    const char* s1_host = std::getenv("SLAVE1_HOST");
    const char* s2_host = std::getenv("SLAVE2_HOST");
    std::string slave1 = s1_host ? s1_host : "http://slave1:8081";
    std::string slave2 = s2_host ? s2_host : "http://slave2:8082";

    svr.Post("/process", [&](const httplib::Request& req, httplib::Response& res){
        std::string payload = req.body;

        std::string resp1 = "";
        std::string resp2 = "";
        std::mutex mu;

        auto call_slave = [&](const std::string& base_url, std::string& out){
            try {
                // base_url expected like http://host:port or host:port
                std::string url = base_url;
                if (url.rfind("http://", 0) == 0) url = url.substr(7);
                auto pos = url.find(':');
                std::string host = url.substr(0, pos);
                int port = 80;
                if (pos != std::string::npos) port = std::stoi(url.substr(pos+1));

                httplib::Client cli(host.c_str(), port);
                cli.set_connection_timeout(5); // seconds

                // check health
                if (auto hres = cli.Get("/health"); hres && hres->status == 200) {
                    // decide endpoint depending on which slave (host/port)
                    std::string endpoint = (port == 8081) ? std::string("/letras") : std::string("/numeros");
                    if (auto pres = cli.Post(endpoint.c_str(), payload, "text/plain"); pres && pres->status == 200) {
                        std::lock_guard<std::mutex> lk(mu);
                        out = pres->body;
                    } else {
                        std::lock_guard<std::mutex> lk(mu);
                        out = std::string("{\"error\":\"process failed\"}");
                    }
                } else {
                    std::lock_guard<std::mutex> lk(mu);
                    out = std::string("{\"error\":\"unhealthy\"}");
                }
            } catch (const std::exception& e) {
                std::lock_guard<std::mutex> lk(mu);
                out = std::string("{\"error\":\"") + e.what() + "\"}";
            }
        };

        std::thread t1(call_slave, slave1, std::ref(resp1));
        std::thread t2(call_slave, slave2, std::ref(resp2));
        t1.join();
        t2.join();

        // Escape helper for safe embedding in JSON string
        auto escape = [](const std::string &s){
            std::string o; o.reserve(s.size());
            for(char c: s){
                if (c == '\\') o += "\\\\";
                else if (c == '"') o += "\\\"";
                else o += c;
            }
            return o;
        };

        // Combine responses into a JSON object (built manually, but escape inner strings)
        std::string combined = std::string("{\"slave1\": ") + resp1 + ", \"slave2\": " + resp2 + ", \"combined\": \"" + escape(resp1) + "|" + escape(resp2) + "\"}";
        res.set_content(combined, "application/json");
    });

    svr.Get("/health", [&](const httplib::Request&, httplib::Response& res){
        res.set_content("OK", "text/plain");
    });

    std::cout << "Master listening on 0.0.0.0:8080\n";
    svr.listen("0.0.0.0", 8080);
    return 0;
}
