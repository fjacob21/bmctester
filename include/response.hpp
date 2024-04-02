namespace transport
{
    class Response
    {
        public:
            Response(int result, std::string data, std::map<std::string, std::string> params={})
            {
                _result = result;
                _data = data;
                _params = params;
            }

            int GetResult()
            {
                return _result;
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
            int _result = 0;
            std::string _data;
            std::map<std::string, std::string> _params;
    };
}