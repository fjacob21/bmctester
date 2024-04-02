#pragma once

#include <credential.hpp>
#include <tuple>

namespace credentialstore
{
    

    class CredentialStoreInterface
    {
        public:
            CredentialStoreInterface(){}
            virtual ~CredentialStoreInterface(){}
            virtual Credential GetCredential() = 0;
    };
}