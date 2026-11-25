#include "APIClient.hpp"

#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>

using json = nlohmann::json;

APIClient::APIClient(std::string url, long timeoutSeg)
    : endpointUrl(std::move(url)),
      timeoutSegundos(timeoutSeg) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

size_t APIClient::escribirRespuestaCallback(
    void* contenido,
    size_t tamano,
    size_t cantidad,
    void* usuarioData
) {
    size_t totalBytes = tamano * cantidad;
    auto* buffer = static_cast<std::string*>(usuarioData);
    buffer->append(static_cast<char*>(contenido), totalBytes);
    return totalBytes;
}

bool APIClient::ejecutarPeticion(
    const std::string& metodo,
    const std::string& cuerpoJson,
    std::string& respuestaSalida
) const {
    ultimoError.clear();
    respuestaSalida.clear();

    CURL* curl = curl_easy_init();
    if (!curl) {
        ultimoError = "No se pudo inicializar libcurl.";
        return false;
    }

    struct curl_slist* cabeceras = nullptr;
    cabeceras = curl_slist_append(cabeceras, "Content-Type: application/json");
    cabeceras = curl_slist_append(cabeceras, "Accept: application/json");
    cabeceras = curl_slist_append(cabeceras, "User-Agent: EscapeDelLaboratorio/1.0");

    curl_easy_setopt(curl, CURLOPT_URL, endpointUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, cabeceras);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, escribirRespuestaCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &respuestaSalida);

    // Configuracion de red robusta para Windows / MSYS2
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, timeoutSegundos);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeoutSegundos);
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
    curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    if (metodo == "POST") {
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, cuerpoJson.c_str());
    }

    CURLcode res = curl_easy_perform(curl);
    long codigoHttp = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &codigoHttp);

    curl_slist_free_all(cabeceras);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        ultimoError = std::string("Error de red: ") + curl_easy_strerror(res);
        std::cerr << "[APIClient] " << ultimoError << "\n";
        return false;
    }

    if (codigoHttp < 200 || codigoHttp >= 300) {
        ultimoError = "HTTP error " + std::to_string(codigoHttp);
        std::cerr << "[APIClient] " << ultimoError << "\n";
        return false;
    }

    return true;
}

bool APIClient::enviarTiempo(
    const std::string& jugador,
    double tiempoSegundos
) {
    // Redondear a 2 decimales
    double tiempoRedondeado = std::round(tiempoSegundos * 100.0) / 100.0;

    json payload = {
        {"jugador", jugador.empty() ? "Cientifico" : jugador},
        {"tiempo", tiempoRedondeado}
    };

    std::string respuesta;
    return ejecutarPeticion("POST", payload.dump(), respuesta);
}

std::vector<RegistroRanking> APIClient::obtenerTop5() const {
    std::string respuesta;
    std::vector<RegistroRanking> ranking;

    if (!ejecutarPeticion("GET", "", respuesta)) {
        return ranking;
    }

    try {
        json datos = json::parse(respuesta);
        if (!datos.is_array()) {
            ultimoError = "Formato JSON inesperado.";
            return ranking;
        }

        for (const auto& item : datos) {
            if (!item.contains("tiempo") || !item["tiempo"].is_number()) {
                continue;
            }
            RegistroRanking reg;
            reg.jugador = item.value("jugador", "Cientifico");
            reg.tiempo = item["tiempo"].get<double>();
            if (reg.tiempo > 0.0) {
                ranking.push_back(reg);
            }
        }

        // Ordenar de MENOR tiempo a MAYOR tiempo
        std::sort(ranking.begin(), ranking.end(),
            [](const RegistroRanking& a, const RegistroRanking& b) {
                return a.tiempo < b.tiempo;
            });

        if (ranking.size() > 5) {
            ranking.resize(5);
        }
    } catch (const std::exception& e) {
        ultimoError = std::string("Error al leer JSON: ") + e.what();
        std::cerr << "[APIClient] " << ultimoError << "\n";
        ranking.clear();
    }

    return ranking;
}

bool APIClient::huboError() const {
    return !ultimoError.empty();
}

std::string APIClient::getUltimoError() const {
    return ultimoError;
}