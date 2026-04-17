#ifndef BOR_PIPELINE_HPP
#define BOR_PIPELINE_HPP

#include <string>
#include <vector>

namespace bor
{
    class BoRPipeline
    {
    public:
        BoRPipeline(const std::string& vertFilepath, const std::string& fragFilepath);

    private:
        static std::vector<char> readFile(const std::string& filepath);
        void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath);
    };
};

#endif