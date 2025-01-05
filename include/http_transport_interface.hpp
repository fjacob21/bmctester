#pragma once

#include <request.hpp>
#include <response.hpp>

namespace transport
{
    class HTTPTransportInterface
    {
        public:
            HTTPTransportInterface(){}
            virtual ~HTTPTransportInterface(){}
            virtual Response Get(std::string path, Request request) = 0;
            virtual Response Put(std::string path, Request request) = 0;
            virtual Response Post(std::string path, Request request) = 0;
            virtual Response Delete(std::string path, Request request) = 0;
            virtual void SetBasicAuth(std::string username, std::string password) = 0;
    };
}