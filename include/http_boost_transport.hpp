#pragma once
#include <credential_factory.hpp>
#include <curl/curl.h>
#include <format>
#include <iostream>
#include <http_transport_interface.hpp>
#include <regex>

#include <boost/beast/src.hpp>
#include <boost/beast.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio/ssl/impl/src.hpp>
// #include <boost/asio/ssl/impl/error.ipp>
#include <cstdlib>
#include <iostream>
#include <string>

namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http;   // from <boost/beast/http.hpp>
namespace net = boost::asio;    // from <boost/asio.hpp>
namespace ssl = net::ssl;       // from <boost/asio/ssl.hpp>
using tcp = net::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
using http::field;

namespace transport
{
    class HTTPTransportBoost: public HTTPTransportInterface
    {
        public:
            HTTPTransportBoost(std::string server, ushort port) 
            {
                _server = server;
                _port = port;
                // credentialstore::CredentialFactory credFactory;
                // auto auth = credFactory.GetCredentialStore(_server);
                // auto cred = auth->GetCredential().GetCredential();
                _userpsw = "";
            }
        
            virtual Response Get(std::string path, Request request)
            {
                int version = 10;
                // auto url = std::format("https://{}:{}/redfish/v1/{}", _server, _port, path);
                auto url = std::format("/redfish/v1/{}", path);
                std::cout << "HTTPTransportBoost::Get " << url << "\n";
                net::io_context ioc;
                ssl::context ctx(ssl::context::tlsv12_client);
                ctx.set_verify_mode(ssl::verify_none);
                tcp::resolver resolver(ioc);
                beast::ssl_stream<beast::tcp_stream> stream(ioc, ctx);
                if(! SSL_set_tlsext_host_name(stream.native_handle(), _server.c_str()))
                {
                    beast::error_code ec{static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()};
                    throw beast::system_error{ec};
                }
                auto const results = resolver.resolve(_server.c_str(), "2443");
                beast::get_lowest_layer(stream).connect(results);
                stream.handshake(ssl::stream_base::client);
                http::request<http::string_body> req{http::verb::get, url, version};
                req.set(http::field::host, _server.c_str());
                req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
                for (const auto& [key, value] : request.GetParams())
                {
                    req.set(key.c_str(), value.c_str());
                }
                http::write(stream, req);
                beast::flat_buffer buffer;
                http::response<http::dynamic_body> res;
                http::read(stream, buffer, res);
                // for (const auto& field : res)
                // {
                //     std::cout << "\theader " << field.name_string() << " : " << field.value()  << std::endl;
                // }

                // Gracefully close the stream
                beast::error_code ec;
                stream.shutdown(ec);
                // std::cout << res << std::endl;
                if(ec == net::error::eof)
                {
                    // Rationale:
                    // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
                    ec = {};
                }
                // if(ec)
                // {
                //     std::cout << "return error: " << ec << std::endl;
                //     throw beast::system_error{ec};
                //         return Response(0, "");
                // }
                        
                // std::cout << "return res" << std::endl;
                std::map<std::string, std::string> replyHeaders;
                
                return Response(200, boost::beast::buffers_to_string(res.body().data()));
                // return Response(0, "");
            }

            virtual Response Put(std::string path, Request request)
            {
                return Response(0, "");
            }

            virtual Response Post(std::string path, Request request)
            {
                int version = 10;
                // auto url = std::format("https://{}:{}/redfish/v1/{}", _server, _port, path);
                auto url = std::format("/redfish/v1/{}", path);
                std::cout << "HTTPTransportBoost::Post " << url << "\n";
                net::io_context ioc;
                ssl::context ctx(ssl::context::tlsv12_client);
                ctx.set_verify_mode(ssl::verify_none);
                tcp::resolver resolver(ioc);
                beast::ssl_stream<beast::tcp_stream> stream(ioc, ctx);
                if(! SSL_set_tlsext_host_name(stream.native_handle(), _server.c_str()))
                {
                    beast::error_code ec{static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()};
                    throw beast::system_error{ec};
                }
                auto const results = resolver.resolve(_server.c_str(), "2443");
                beast::get_lowest_layer(stream).connect(results);
                stream.handshake(ssl::stream_base::client);
                http::request<http::string_body> req{http::verb::post, url, version};
                for (const auto& [key, value] : request.GetParams())
                {
                    req.set(key.c_str(), value.c_str());
                }
                req.set(http::field::host, _server.c_str());
                req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
                req.set(http::field::content_type, "application/json");
                auto data = request.GetData();
                // std::cout << "POST data: " << data << "\n";
                req.body() = data.c_str();
                req.prepare_payload();
                http::write(stream, req);
                beast::flat_buffer buffer;
                http::response<http::dynamic_body> res;
                http::read(stream, buffer, res);
                std::map<std::string, std::string> replyHeaders;
                for (const auto& field : res)
                {
                    replyHeaders[field.name_string()] = field.value();
                    // std::cout << "\theader " << field.name_string() << " : " << field.value()  << std::endl;
                }

                // Gracefully close the stream
                beast::error_code ec;
                stream.shutdown(ec);
                // std::cout << res << std::endl;
                if(ec == net::error::eof)
                {
                    // Rationale:
                    // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
                    ec = {};
                }
                // if(ec)
                // {
                //     std::cout << "return error: " << ec << std::endl;
                //     throw beast::system_error{ec};
                //         return Response(0, "");
                // }
                        
                // std::cout << "return res" << std::endl;
                return Response(200, boost::beast::buffers_to_string(res.body().data()), replyHeaders);
                // return Response(0, "");
            }
            
            virtual Response Delete(std::string path, Request request)
            {
                int version = 10;
                // auto url = std::format("https://{}:{}/redfish/v1/{}", _server, _port, path);
                auto url = std::format("/redfish/v1/{}", path);
                std::cout << "HTTPTransportBoost::Delete " << url << "\n";
                net::io_context ioc;
                ssl::context ctx(ssl::context::tlsv12_client);
                ctx.set_verify_mode(ssl::verify_none);
                tcp::resolver resolver(ioc);
                beast::ssl_stream<beast::tcp_stream> stream(ioc, ctx);
                if(! SSL_set_tlsext_host_name(stream.native_handle(), _server.c_str()))
                {
                    beast::error_code ec{static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()};
                    throw beast::system_error{ec};
                }
                auto const results = resolver.resolve(_server.c_str(), "2443");
                beast::get_lowest_layer(stream).connect(results);
                stream.handshake(ssl::stream_base::client);
                http::request<http::string_body> req{http::verb::delete_, url, version};
                for (const auto& [key, value] : request.GetParams())
                {
                    req.set(key.c_str(), value.c_str());
                }
                req.set(http::field::host, _server.c_str());
                req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
                req.set(http::field::content_type, "application/json");
                auto data = request.GetData();
                // std::cout << data << "\n";
                req.body() = data.c_str();
                req.prepare_payload();
                http::write(stream, req);
                beast::flat_buffer buffer;
                http::response<http::dynamic_body> res;
                http::read(stream, buffer, res);
                std::map<std::string, std::string> replyHeaders;
                for (const auto& field : res)
                {
                    replyHeaders[field.name_string()] = field.value();
                    // std::cout << "\theader " << field.name_string() << " : " << field.value()  << std::endl;
                }

                // Gracefully close the stream
                beast::error_code ec;
                stream.shutdown(ec);
                // std::cout << res << std::endl;
                if(ec == net::error::eof)
                {
                    // Rationale:
                    // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
                    ec = {};
                }
                // if(ec)
                // {
                //     std::cout << "return error: " << ec << std::endl;
                //     throw beast::system_error{ec};
                //         return Response(0, "");
                // }
                        
                // std::cout << "return res" << std::endl;
                return Response(200, boost::beast::buffers_to_string(res.body().data()), replyHeaders);
                // return Response(0, "");
            }

            virtual void SetBasicAuth(std::string username, std::string password)
            {
                _userpsw = std::format("{}:{}", username, password);
            }
        private:
            std::string _server;
            ushort _port;
            std::string _userpsw;
    };
}