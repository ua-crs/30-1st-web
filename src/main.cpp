/*
 *  Project 30-1st-web - main.cpp
 *      Primera pagina de Web
 *      Solo un saludo sobre la pantalla
 */

#include <Arduino.h>

#ifdef ESP32

#include <WiFi.h>
#include <WebServer.h>

#define ServerObject    WebServer
#define HelloMsg        "Hello from ESP32 !"

#else

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define ServerObject    ESP8266WebServer
#define HelloMsg        "Hello from ESP8266 !"
#endif

#include "wifi_ruts.h"

ServerObject server(80);    // Create a webserver object that listens for HTTP request on port 80 

/*
 *  handleRoot:
 *      Function to manage requirements to root page
 */

static void
handleRoot( void )
{
    Serial.println( __FUNCTION__ );
    server.send(200, "text/plain", HelloMsg );  // Send HTTP status 200 (Ok) and send some text to the browser/client
}

/*
 *  handleNotFound:
 *      Function to manage requirements to non existent pages
 */

static void
handleNotFound( void )
{
    Serial.println( __FUNCTION__ );

//  Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
    server.send(404, "text/plain", "404: Not found");
}

void
setup( void )
{
    Serial.begin(SERIAL_BAUD);
    delay(10);
    Serial.println();

    connect_wifi(MY_SSID, MY_PASS);
/*
 *  Register functions on page requirements (only one page active in this example)
 */

// Call the 'handleRoot' function when a client requests URI "/"
    server.on("/", handleRoot);
// When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
    server.onNotFound( handleNotFound);

    server.begin(); // Actually start the server
    Serial.println("HTTP server started");
}

void
loop(void)
{
    server.handleClient(); // Listen for HTTP requests from clients
}
