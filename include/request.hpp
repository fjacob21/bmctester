#include <map>

namespace transport
{
    class Request
    {
        public:
            Request(std::string data="", std::map<std::string, std::string> params={})
            {
                _data = data;
                _params = params;
            }

            std::string GetData()
            {
                return _data;
            }

            std::map<std::string, std::string> GetParams()
            {
                return _params;
            }
        
        private:
            std::string _data;
            std::map<std::string, std::string> _params;
    };
}