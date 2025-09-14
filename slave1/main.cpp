// Escravo 1: expõe /health e /letras
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <cwctype>
#include <httplib.h>

int main(){
    // Ajuste de locale para classificação de caracteres wide
    std::setlocale(LC_CTYPE, "en_US.UTF-8");

    httplib::Server svr;
    svr.Get("/health", [](const httplib::Request&, httplib::Response& res){
        res.set_content("OK", "text/plain");
    });

    // Recebe texto no body e retorna a contagem de letras (conta letras Unicode)
    svr.Post("/letras", [](const httplib::Request& req, httplib::Response& res){
        std::string in = req.body;
        size_t count = 0;

        try {
            std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
            std::wstring w = conv.from_bytes(in);
            for (wchar_t wc : w) {
                if (iswalpha(wc)) ++count;
            }
        } catch (...) {
            // fallback simples: contar apenas ascii letters
            for (unsigned char c : in) if (std::isalpha(c)) ++count;
        }

        std::string out = "{\"letras\": " + std::to_string(count) + "}";
        res.set_content(out, "application/json");
    });

    std::cout << "Slave1 listening on 0.0.0.0:8081 (endpoints: /health, /letras)\n";
    svr.listen("0.0.0.0", 8081);
    return 0;
}