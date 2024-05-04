#include "Config.hpp"

#include <iostream>
#include <sstream>
#include <utility>
#include <format>

Config::Config(std::string filename) : filename(std::move(filename)), _ready(false)
{
        this->file = std::ifstream(this->filename, std::ios::in);
        if (!this->file.is_open()) {
                std::cerr << "Could not open config file \"" << this->filename << "\" ! " << std::endl;
        }

        parse();
}

#define IS_ALLOWED_IDENTIFIER(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
#define IS_ALLOWED_VALUE(c) (IS_ALLOWED_IDENTIFIER(c) || (c >= '0' && c <= '9'))

enum parser_mode : uint8_t {
        MODE_KEY,
        MODE_VALUE
};

bool Config::parse()
{
        this->_ready = false;

        std::stringstream ss;
        ss << this->file.rdbuf();

        std::string line;

        auto mode = parser_mode::MODE_KEY;

        std::string key;
        std::string value;

        auto write_iden = [&](char c) {
                std::string s{c};
                std::string *target;
                switch (mode) {
                        case parser_mode::MODE_KEY:
                                target = &key;
                                break;
                        case parser_mode::MODE_VALUE:
                                target = &value;
                                break;
                }
                target->append(s);
        };

        size_t lineNo = 0;

        while (std::getline(ss, line, '\n')) {
                lineNo++;

                if (line.empty())
                        continue;

                mode = parser_mode::MODE_KEY;

                key.clear();
                value.clear();

                for (size_t i = 0; i < line.length(); i++) {
                        char c = line[i];

                        if ((mode == parser_mode::MODE_KEY && IS_ALLOWED_IDENTIFIER(c)) ||
                            (mode == parser_mode::MODE_VALUE && IS_ALLOWED_VALUE(c))) {
                                write_iden(c);
                                continue;
                        }

                        if (c == '=') {
                                if (mode == parser_mode::MODE_VALUE) {
                                        std::cerr
                                                << "Syntax error: The assignment operator '=' must not appear twice in a single config line. Error on line "
                                                << lineNo << std::endl;
                                        return false;
                                }
                                mode = parser_mode::MODE_VALUE;
                                continue;
                        }

                        std::cerr << "Syntax error: Unknown token ('" << c
                                  << "') encountered while parsing configuration line " << lineNo << std::endl;
                        return false;
                }

                if (key.empty()) {
                        std::cerr << "Syntax error: The key must not be empty. Error on line " << lineNo << std::endl;
                        return false;
                }

                if (value.empty()) {
                        std::cerr << "Syntax error: The value must not be empty. Error on line " << lineNo << std::endl;
                        return false;
                }

                if (this->config.count(key)) {
                        std::cerr << "Error: Attempting to redefine an existing key \"" << key << "\" on line "
                                  << lineNo << std::endl;
                        return false;
                }

                this->config[key] = value;
        }

        this->_ready = true;
        return true;
}

std::string Config::operator[](const std::string &key)
{
        if (!this->config.count(key)) {
                throw invalid_key_exception(key);
        }
        return this->config[key];
}

bool Config::is_ready()
{
        return this->_ready;
}

std::vector<std::string> Config::keys()
{
        std::vector<std::string> keys;

        for (auto iter = this->config.begin(); iter != this->config.end(); ++ iter)
                keys.push_back(iter->first);

        return keys;
}

#undef IS_ALLOWED_VALUE
#undef IS_ALLOWED_IDENTIFIER

invalid_key_exception::invalid_key_exception(std::string key) : key(std::move(key)), message(std::format(
        "Invalid configuration key requested: {}", key))
{
}

const char *invalid_key_exception::what() const noexcept
{
        return this->message.c_str();
}
