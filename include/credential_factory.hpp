#pragma once

#include <memory>
#include <tuple>
#include <credential_store_interface.hpp>
#include <static_credential.hpp>

namespace credentialstore
{
    class CredentialFactory
    {
        public:
            CredentialFactory() {}
            std::shared_ptr<CredentialStoreInterface> GetCredentialStore(std::string server) 
            {
                return std::make_shared<StaticCredentialStore>();
            }
    };
}