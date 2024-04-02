#pragma once

#include <tuple>
#include <credential_store_interface.hpp>

namespace credentialstore
{
    class StaticCredentialStore: public CredentialStoreInterface
    {
        public:
            StaticCredentialStore() {}
            Credential GetCredential() 
            {   
                std::vector<std::string> creds = {"root", "0penBmc"};
                return Credential(CredentialType::UsernamePassword, creds);
                // return std::make_tuple<std::string,std::string>("root", "0penBmc");
            }
    };
}