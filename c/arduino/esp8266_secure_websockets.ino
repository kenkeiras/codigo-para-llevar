// See: https://codigoparallevar.com/2019/secure-websockets-esp8266-arduino/
#define WIFI_SSID "SET_HERE_YOUR_WIFI_NETWORK_NAME"
#define WIFI_PASSWORD "SET_HERE_YOUR_WIFI_PASSWORD"

// WebSocket configuration
#include <WebSocketsClient.h>

// WiFi configuration
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

// WiFi handler
ESP8266WiFiMulti WiFiMulti;

// WebSocket object
WebSocketsClient webSocket;

// TLS certificate to enable secure websockets
const char ENDPOINT_CA_CERT[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIEkjCCA3qgAwIBAgIQCgFBQgAAAVOFc2oLheynCDANBgkqhkiG9w0BAQsFADA/
MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT
DkRTVCBSb290IENBIFgzMB4XDTE2MDMxNzE2NDA0NloXDTIxMDMxNzE2NDA0Nlow
SjELMAkGA1UEBhMCVVMxFjAUBgNVBAoTDUxldCdzIEVuY3J5cHQxIzAhBgNVBAMT
GkxldCdzIEVuY3J5cHQgQXV0aG9yaXR5IFgzMIIBIjANBgkqhkiG9w0BAQEFAAOC
AQ8AMIIBCgKCAQEAnNMM8FrlLke3cl03g7NoYzDq1zUmGSXhvb418XCSL7e4S0EF
q6meNQhY7LEqxGiHC6PjdeTm86dicbp5gWAf15Gan/PQeGdxyGkOlZHP/uaZ6WA8
SMx+yk13EiSdRxta67nsHjcAHJyse6cF6s5K671B5TaYucv9bTyWaN8jKkKQDIZ0
Z8h/pZq4UmEUEz9l6YKHy9v6Dlb2honzhT+Xhq+w3Brvaw2VFn3EK6BlspkENnWA
a6xK8xuQSXgvopZPKiAlKQTGdMDQMc2PMTiVFrqoM7hD8bEfwzB/onkxEz0tNvjj
/PIzark5McWvxI0NHWQWM6r6hCm21AvA2H3DkwIDAQABo4IBfTCCAXkwEgYDVR0T
AQH/BAgwBgEB/wIBADAOBgNVHQ8BAf8EBAMCAYYwfwYIKwYBBQUHAQEEczBxMDIG
CCsGAQUFBzABhiZodHRwOi8vaXNyZy50cnVzdGlkLm9jc3AuaWRlbnRydXN0LmNv
bTA7BggrBgEFBQcwAoYvaHR0cDovL2FwcHMuaWRlbnRydXN0LmNvbS9yb290cy9k
c3Ryb290Y2F4My5wN2MwHwYDVR0jBBgwFoAUxKexpHsscfrb4UuQdf/EFWCFiRAw
VAYDVR0gBE0wSzAIBgZngQwBAgEwPwYLKwYBBAGC3xMBAQEwMDAuBggrBgEFBQcC
ARYiaHR0cDovL2Nwcy5yb290LXgxLmxldHNlbmNyeXB0Lm9yZzA8BgNVHR8ENTAz
MDGgL6AthitodHRwOi8vY3JsLmlkZW50cnVzdC5jb20vRFNUUk9PVENBWDNDUkwu
Y3JsMB0GA1UdDgQWBBSoSmpjBH3duubRObemRWXv86jsoTANBgkqhkiG9w0BAQsF
AAOCAQEA3TPXEfNjWDjdGBX7CVW+dla5cEilaUcne8IkCJLxWh9KEik3JHRRHGJo
uM2VcGfl96S8TihRzZvoroed6ti6WqEBmtzw3Wodatg+VyOeph4EYpr/1wXKtx8/
wApIvJSwtmVi4MFU5aMqrSDE6ea73Mj2tcMyo5jMd6jmeWUHK8so/joWUoHOUgwu
X4Po1QYz+3dszkDqMp4fklxBwXRsW10KXzPMTZ+sOPAveyxindmjkW8lGy+QsRlG
PfZ+G6Z6h7mjem0Y+iWlkYcV4PIWL1iwBi8saCbGS5jN2p8M+X+Q7UNKEkROb3N6
KOqkqm57TH2H3eDJAkSnh6/DNFu0Qg==
-----END CERTIFICATE-----
)EOF";


void setup() {
    // Configure serial output
    Serial.begin(115200);

    // Activate debug output. All help is welcome in this phase.
    Serial.setDebugOutput(true);

    // Connect wifi
    DEBUG_WEBSOCKETS("Connecting");

    // Configure WiFi name and password
    WiFiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

    // Wait for the connection to be established
    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(500);
        DEBUG_WEBSOCKETS(".");
    }

    // Show connection data    
    Serial.println();
    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());

    // Establish websocket connection
    webSocket.beginSslWithCA("echo.websocket.org", 443, "/", ENDPOINT_CA_CERT);

    // Setup connection handler
    webSocket.onEvent(webSocketEventHandler);
}

void webSocketEventHandler(WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
    case WStype_DISCONNECTED:
        Serial.printf("[WSc] Disconnected\n");
        break;
    case WStype_CONNECTED: // When WebSocket does connect, send a message
        Serial.printf("[WSc] Connected to url: %s\n",  payload);
        webSocket.sendTXT("Hi there!\n");    
        break;

    case WStype_TEXT:  // When a message is received, print it
        Serial.printf("[WSc] get text: %s\n", payload);
        break;

    case WStype_BIN:
        Serial.printf("[WSc] get binary length: %u\n", length);
        break;

    case WStype_ERROR:
        // Error
        Serial.printf("[WSc] get error length: %u\n", length);
        break;

    default:
        Serial.printf("[WSc] Unknown transmission: %i. Probably can be ignored.", type);
    }
}

void loop() {
    // On every chance, let the WebSocket update
    webSocket.loop();
}
