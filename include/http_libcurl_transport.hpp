#pragma once
#include <credential_factory.hpp>
#include <curl/curl.h>
#include <format>
#include <iostream>
#include <http_transport_interface.hpp>
#include <regex>

namespace transport
{
    class HTTPTransportLibcurl: public HTTPTransportInterface
    {
        public:
            static char errorBuffer[CURL_ERROR_SIZE];
            static std::string buffer;
            static std::map<std::string, std::string> replyHeaders;

            static size_t header_callback(char *data, size_t size,
                                    size_t nitems, std::map<std::string, std::string> *userdata);

            static int writer(char *data, size_t size, size_t nmemb,
                        std::string *writerData);
        
            HTTPTransportLibcurl(std::string server, ushort port=443) 
            {
                _server = server;
                _port = port;
                curl_global_init(CURL_GLOBAL_DEFAULT);
                // credentialstore::CredentialFactory credFactory;
                // auto auth = credFactory.GetCredentialStore(_server);
                // auto cred = auth->GetCredential().GetCredential();
                
                _userpsw = "";
            }
        
            ~HTTPTransportLibcurl()
            {
                curl_global_cleanup();
            }

            virtual Response Get(std::string path, Request request)
            {
                CURLcode res;
                memset(errorBuffer, 0, CURL_ERROR_SIZE);
                buffer = "";
                CURL *curl = GetBaseCurl(path, request);
                if(curl) {
                    res = curl_easy_perform(curl);
                    /* always cleanup */
                    curl_easy_cleanup(curl);
                    curl_slist_free_all(headers);
                    headers = NULL;
                    return Response(200, buffer);
                }
                return Response(0, "");
            }

            virtual Response Put(std::string path, Request request)
            {
                CURLcode res;
                memset(errorBuffer, 0, CURL_ERROR_SIZE);
                buffer = "";
                CURL *curl = GetBaseCurl(path, request);
                if(curl) {
                    curl_easy_setopt(curl, CURLOPT_PUT, 1L);
                    res = curl_easy_perform(curl);
                    curl_easy_cleanup(curl);
                    curl_slist_free_all(headers);
                    headers = NULL;
                    return Response(200, buffer, replyHeaders);
                }
                return Response(0, "");
            }

            virtual Response Post(std::string path, Request request)
            {
                CURLcode res;
                memset(errorBuffer, 0, CURL_ERROR_SIZE);
                buffer = "";
                CURL *curl = GetBaseCurl(path, request);
                if(curl) {
                    curl_easy_setopt(curl, CURLOPT_POST, 1L);
                    auto data = request.GetData();
                    std::cout << data << "\n";
                    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
                    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, -1L);
                    res = curl_easy_perform(curl);
                    curl_easy_cleanup(curl);
                    curl_slist_free_all(headers);
                    headers = NULL;
                    return Response(200, buffer, replyHeaders);
                }
                return Response(0, "");
            }
            
            virtual Response Delete(std::string path, Request request)
            {
                CURLcode res;
                memset(errorBuffer, 0, CURL_ERROR_SIZE);
                buffer = "";
                CURL *curl = GetBaseCurl(path, request);
                if(curl) {
                    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
                    res = curl_easy_perform(curl);
                    curl_easy_cleanup(curl);
                    curl_slist_free_all(headers);
                    headers = NULL;
                    return Response(200, buffer, replyHeaders);
                }
                return Response(0, "");
            }

            virtual void SetBasicAuth(std::string username, std::string password)
            {
                _userpsw = std::format("{}:{}", username, password);
            }
        
        protected:

            CURL* GetBaseCurl(std::string path, Request request)
            {
                CURL *curl;
                CURLcode res;
                curl = curl_easy_init();
                if(curl) {
                    replyHeaders.clear();
                    memset(HTTPTransportLibcurl::errorBuffer, 0, CURL_ERROR_SIZE);
                    buffer = "";
                    auto url = std::format("https://{}:{}/redfish/v1/{}", _server, _port, path);
                    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
                    curl_easy_setopt(curl, CURLOPT_SSLENGINE_DEFAULT, 1L);
                    curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_TRY);
                    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYSTATUS, 0L);
                    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
                    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
                    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, HTTPTransportLibcurl::errorBuffer);
                    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HTTPTransportLibcurl::writer);
                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &HTTPTransportLibcurl::buffer);
                    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, HTTPTransportLibcurl::header_callback);
                    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &HTTPTransportLibcurl::replyHeaders);
                    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_ANY);
                    if (_userpsw != "")
                        curl_easy_setopt(curl, CURLOPT_USERPWD, _userpsw.c_str());
                    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
                    
                    for (const auto& [key, value] : request.GetParams())
                    {
                        std::string headerString = std::format("{}: {}", key, value);
                        headers = curl_slist_append(headers, headerString.c_str());
                    }
                    
                    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
                }
                return curl;
            }

        private:
            std::string _server;
            ushort _port;
            std::string _userpsw;
            struct curl_slist *headers = NULL;
    };

    char HTTPTransportLibcurl::errorBuffer[CURL_ERROR_SIZE];
    std::string HTTPTransportLibcurl::buffer;
    std::map<std::string, std::string> HTTPTransportLibcurl::replyHeaders;

    size_t HTTPTransportLibcurl::header_callback(char *data, size_t size,
                            size_t nitems, std::map<std::string, std::string> *userdata)
    {
        /* received header is nitems * size long in 'buffer' NOT ZERO TERMINATED */
        /* 'userdata' is set with CURLOPT_HEADERDATA */
        std::string header;
        header.append(data, (size*nitems)-2);
        
        std::size_t found = header.find_first_of(":");
        if(found!=std::string::npos)
        {
            auto key = header.substr(0, found);
            auto value = header.substr(found+2);
            //std::cout << "key: " << key << " value: " << value << "\n" ;
            (*userdata)[key] = value;
        }
        

        return nitems * size;
    }

    int HTTPTransportLibcurl::writer(char *data, size_t size, size_t nmemb,
                std::string *writerData)
    {
        if(writerData == NULL)
            return 0;
        
        writerData->append(data, size*nmemb);
        
        return size * nmemb;
    }
}