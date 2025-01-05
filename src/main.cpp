#include <nlohmann/json.hpp>
#include <iostream>
// #include <http_boost_transport.hpp>
#include <credential_factory.hpp>
#include <http_libcurl_transport.hpp>
#include <redfish_protocol.hpp>
// #include <redfish_authentification.hpp>
#include <format>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

int main(int /*argc*/, char** /*argv*/) {
    std::cout << "bmctester" << "\n";
    // credentialstore::CredentialFactory credFactory;
    //             auto auth = credFactory.GetCredentialStore(_server);
    //             auto cred = auth->GetCredential().GetCredential();
    //             _userpsw = std::format("{}:{}", cred[0], cred[1]);
  nlohmann::json jsonValue;
  // transport::HTTPTransportLibcurl httpTransport("localhost", 2443);
  // transport::HTTPTransportBoost httpTransport("localhost", 2443);
  // std::shared_ptr<transport::HTTPTransportBoost> httpTransport = std::make_shared<transport::HTTPTransportBoost>("localhost", 2443);
  // std::shared_ptr<transport::HTTPTransportLibcurl> httpTransport = std::make_shared<transport::HTTPTransportLibcurl>("localhost", 2443);
  // auto protocol = protocol::RedfishProtocol(httpTransport);
  // protocol.setBasicAuth("root", "0penBmc");
  // protocol.setSessionAuth("root", "0penBmc");
  // std::shared_ptr<transport::HTTPTransportBoost> httpTransport = std::make_shared<transport::HTTPTransportBoost>("169.254.0.1", 443);
  std::shared_ptr<transport::HTTPTransportLibcurl> httpTransport = std::make_shared<transport::HTTPTransportLibcurl>("169.254.0.1");
  auto redfish_protocol = std::make_shared<protocol::RedfishProtocol>(httpTransport);
  redfish_protocol->setBasicAuth("root", "calvin");
  std::shared_ptr<protocol::ProtocolInterface> protocol = redfish_protocol;
  // protocol.setSessionAuth("root", "calvin");
  
  // std::cout << "Get root" << protocol->getRoot() << "\n";
  // std::cout << "Get Managers" << protocol->getManagers() << "\n";
  // std::cout << "Get Systems" << protocol->getSystems() << "\n";
  std::cout << "Get getBmcVersion: " << protocol->GetBmcVersion() << "\n";
  

  return 0;
}

