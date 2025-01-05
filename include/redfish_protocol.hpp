#pragma once

#include <memory>
#include <protocol.hpp>
#include <http_transport_interface.hpp>

namespace protocol
{
    enum class RedfishAuthentificationType {
        None,
        Basic,
        Session,
        Certificate,
    };

    class RedfishProtocol: public ProtocolInterface
    {
        public:
            RedfishProtocol(std::shared_ptr<transport::HTTPTransportInterface> transport)
            {
                _transport = transport;
            }

            virtual ~RedfishProtocol()
            {
                if (_authType==RedfishAuthentificationType::Session && _sessionId != "")
                    logoutSession();
            }

            void setBasicAuth(std::string username, std::string password)
            {
                _authType = RedfishAuthentificationType::Basic;
                // _username = username;
                // _password = password;
                _transport->SetBasicAuth(username, password);
            }

            void setSessionAuth(std::string username, std::string password)
            {
                _authType = RedfishAuthentificationType::Session;
                // _username = username;
                // _password = password;
                loginSession(username, password);
            }

            void loginSession(std::string username, std::string password)
            {
                nlohmann::json loginJson;
                loginJson["UserName"] = username;
                loginJson["Password"] = password;
                std::cout << "session login" << "\n";
                std::map<std::string, std::string> headers = {{"content-type", "application/json"}}; 
                auto loginResponse = _transport->Post("SessionService/Sessions", transport::Request(loginJson.dump(), headers));
                std::cout << "session reply " << loginResponse.GetData() << "\n";
                nlohmann::json loginData = nlohmann::json::parse(loginResponse.GetData());
                for (const auto& [key, value] : loginResponse.GetParams())
                {
                    std::cout << key << ":<==" << value << "==>\n";
                }
                _sessionToken = loginResponse.GetParams()["X-Auth-Token"];
                _sessionId = std::string(loginData["/Id"_json_pointer]);
                _defaultHeaders["X-Auth-Token"] = _sessionToken;
            }

            void logoutSession()
            {
                std::string url = std::format("SessionService/Sessions/{}", _sessionId);
                auto logoutResponse = _transport->Delete(url, transport::Request("", _defaultHeaders));
            }

            void setCertAuth()
            {
                _authType = RedfishAuthentificationType::Certificate;
            }

            std::string getRoot()
            {
                auto response = _transport->Get("", transport::Request("", _defaultHeaders));
                auto buffer = response.GetData();
                return buffer;
            }

            std::string getManagers()
            {
                auto response = _transport->Get("Managers", transport::Request("", _defaultHeaders));
                auto buffer = response.GetData();
                return buffer;
            }

            std::string getSystems()
            {
                auto response = _transport->Get("Systems", transport::Request("", _defaultHeaders));
                auto buffer = response.GetData();
                return buffer;
            }

            std::string getChassis()
            {
                auto response = _transport->Get("Chassis", transport::Request("", _defaultHeaders));
                auto buffer = response.GetData();
                return buffer;
            }

            virtual std::string GetBmcVersion()
            {
                auto response = _transport->Get("Managers/iDRAC.Embedded.1", transport::Request("", _defaultHeaders));
                auto buffer = response.GetData();
                nlohmann::json managerData = nlohmann::json::parse(buffer);

                return managerData["FirmwareVersion"];
            }
        
        private:
            RedfishAuthentificationType _authType = RedfishAuthentificationType::None;
            std::shared_ptr<transport::HTTPTransportInterface> _transport;
            std::map<std::string, std::string> _defaultHeaders;
            std::string _sessionToken = "";
            std::string _sessionId = "";
            // std::string _username = "";
            // std::string _password = "";
    };
}