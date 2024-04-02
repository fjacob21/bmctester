#include <nlohmann/json.hpp>
#include <iostream>
#include <http_libcurl_transport.hpp>
#include <format>
// #include <boost/format.hpp>
// using boost::format;

int main(int /*argc*/, char** /*argv*/) {
  nlohmann::json jsonValue;
  transport::HTTPTransportLibcurl httpTransport("localhost", 2443);

  nlohmann::json loginJson;
  loginJson["UserName"] = "root";
  loginJson["Password"] = "0penBmc";
  std::map<std::string, std::string> headers = {{"content-type", "application/json"}}; 
  auto loginResponse = httpTransport.Post("SessionService/Sessions", transport::Request(loginJson.dump(), headers));
  std::cout << loginResponse.GetData() << "\n";
  nlohmann::json loginData = nlohmann::json::parse(loginResponse.GetData());
  for (const auto& [key, value] : loginResponse.GetParams())
  {
    std::cout << key << ":<==" << value << "==>\n";
  }
  auto sessionToken = loginResponse.GetParams()["X-Auth-Token"];
  auto sessionId = std::string(loginData["/Id"_json_pointer]);

  std::map<std::string, std::string> getHeaders = {{"X-Auth-Token", sessionToken}}; 
  auto response = httpTransport.Get("Systems", transport::Request("", getHeaders));
  auto buffer = response.GetData();
  printf("Redfish:%s\n", buffer.c_str());
  nlohmann::json data = nlohmann::json::parse(buffer);
  auto members = data["/Members"_json_pointer];
  for (auto it = members.begin(); it != members.end(); ++it) {
    // std::cout << it.key() << " : " << it.value() << "\n";
    std::cout << *it << "\n";
    for (nlohmann::json::iterator lt = (*it).begin(); lt != (*it).end(); ++lt) {
      std::cout << "\t" << lt.key() << " : " << lt.value() << "\n";
    }
  }
  std::cout << data["/Members"_json_pointer] << "\n";

  std::string url = std::format("SessionService/Sessions/{}", sessionId);
  auto logoutResponse = httpTransport.Delete(url, transport::Request("", getHeaders));

  return 0;
}