// Escravo 2: expõe /health e /numeros
#include <iostream>
#include <string>
#include <cctype>
#include <httplib.h>

int main(){
    httplib::Server svr;
    svr.Get("/health", [](const httplib::Request&, httplib::Response& res){
        res.set_content("OK", "text/plain");
    });

    // Recebe texto e retorna a contagem de dígitos
    svr.Post("/numeros", [](const httplib::Request& req, httplib::Response& res){
        std::string in = req.body;
        size_t count = 0;
        for (char c : in) if (std::isdigit(static_cast<unsigned char>(c))) ++count;
        std::string out = "{\"numeros\": " + std::to_string(count) + "}";
        res.set_content(out, "application/json");
    });

    std::cout << "Slave2 listening on 0.0.0.0:8082 (endpoints: /health, /numeros)\n";
    svr.listen("0.0.0.0", 8082);
    return 0;
}