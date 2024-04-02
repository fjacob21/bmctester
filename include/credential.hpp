#pragma once
#include <credential.hpp>
#include <tuple>

namespace credentialstore
{
    enum class CredentialType {
        UsernamePassword,
        Certificate
    };

    class Credential
    {
        public:
            Credential(CredentialType type, std::vector<std::string> credential)
            {
                _type = type;
                _credential = credential;
            }

            CredentialType GetType()
            {
                return _type;
            }

            std::vector<std::string> GetCredential()
            {
                return _credential;
            }
        
        private:
            CredentialType _type;
            std::vector<std::string> _credential;
    };
}