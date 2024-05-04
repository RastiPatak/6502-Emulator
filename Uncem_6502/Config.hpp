#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <fstream>
#include <map>

class Config {
    private:
        std::ifstream file;
        std::map<std::string, std::string> config;
        bool _ready;
        std::string filename;
    public:
        explicit Config(std::string filename);

        std::string operator[](const std::string &);

        bool is_ready();

        std::vector<std::string> keys();

    private:
        bool parse();
};

class invalid_key_exception : std::exception {
    private:
        std::string key;
        std::string message;
    public:
        explicit invalid_key_exception(std::string key);

        [[nodiscard]] const char *what() const noexcept override;
};


#endif