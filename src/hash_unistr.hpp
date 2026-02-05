#ifndef HASH_UNISTR
#define HASH_UNISTR

#include <unicode/unistr.h>
#include <string>
#include <functional>

/**
* @brief Fazer o std::hash tratar icu::UnicodeString
*/
namespace std {
    template <>
    struct hash<icu::UnicodeString> {
        size_t operator()(const icu::UnicodeString& s) const {
            std::string utf8;
            s.toUTF8String(utf8); // converter para string padrao
            return std::hash<std::string>{}(utf8); // retornar hash normal de string
        }
    };
}

#endif 