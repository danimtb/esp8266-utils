#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <string>
#include <vector>
#include <map>

#include <ESP8266WebServer.h>
#include <FS.h>
#include "ArduinoJson.h"


class WebServer
{
private:
    WebServer();              // Singleton
    WebServer(WebServer const&);              // Singleton: Don't Implement
    void operator=(WebServer const&); // Singleton: Don't implement

    std::map<String, String> m_inputFieldsContent;
    void (*m_submitCallback)(std::map<String, String>);
    String m_htmlPagePath;
    File m_htmlPage;

    void setSubmitCallback(void (*submitCallback)(std::map<String, String>));
    void setHtmlPagePath(String htmlPagePath);

    String m_jsonDataString;

public:
    static WebServer& getInstance();
    ~WebServer();

    void webServerHandleRoot();
    void webServerHandleData();
    void webServerHandleSubmit();

    std::map<String, String> getInputFieldsContent();
    void setup(String htmlPagePath, void (*submitCallback)(std::map<String, String>));
    void setData(std::vector<std::pair<String, String>> data);
    void start();
    void stop();
    void loop();
};

#endif
