#pragma once

#include <memory>
#include <protocol.hpp>
#include <http_transport_interface.hpp>

namespace protocol
{
    class RedfishAuthentification
    {
        public:
            RedfishAuthentification(std::shared_ptr<transport::RedfishProtocol> transport)
            {
                _transport = transport;
            }

            virtual ~RedfishAuthentification(){}

            virtual Response Get(std::string path, Request request)
            {
                return _transport->Get(path, request);
            }

            virtual Response Put(std::string path, Request request)
            {
                return _transport->Put(path, request);
            }

            virtual Response Post(std::string path, Request request)
            {
                return _transport->Post(path, request);
            }

            virtual Response Delete(std::string path, Request request)
            {
                return _transport->Delete(path, request);
            }

        private:
            std::shared_ptr<transport::HTTPTransportInterface> _transport;
    };

    class RedfishBasicAuthentification: public RedfishAuthentification
    {
        public:
            RedfishBasicAuthentification(std::shared_ptr<transport::HTTPTransportInterface> transport, std::string username, std::string password) :RedfishAuthentification(transport)
            {
                _transport->SetBasicAuth(username, password);
            }

            virtual ~RedfishBasicAuthentification()
            {

            }
    };

}