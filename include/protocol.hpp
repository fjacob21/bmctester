#pragma once

namespace protocol
{
    class ProtocolInterface
    {
        public:
            ProtocolInterface(){}
            virtual ~ProtocolInterface(){}
            virtual std::string GetBmcVersion() = 0;
    };
}