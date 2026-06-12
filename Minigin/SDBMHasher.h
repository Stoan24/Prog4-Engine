#pragma once

//Template Meta Programming (compile time recursion)
//Right-Left

//template <int length> struct sdbm_hash
//{
//	consteval static unsigned int _calculate(const char* const text, unsigned int& value) {
//		const unsigned int character = sdbm_hash<length - 1>::_calculate(text, value);
//		value = character + (value << 6) + (value << 16) - value;
//		return text[length - 1];
//	}
//	consteval static unsigned int calculate(const char* const text) {
//		unsigned int value = 0;
//		const auto character = _calculate(text, value);
//		return character + (value << 6) + (value << 16) - value;
//	}
//};
//template <> struct sdbm_hash<1> {
//	consteval static int _calculate(const char* const text, unsigned int&) { return text[0]; }
//};
//template <size_t N> constexpr unsigned int make_sdbm_hash(const char(&text)[N]) {
//	return sdbm_hash<N - 1>::calculate(text);
//};


//Can run at compile time or runtime
//Could eventually use user input for strings
//Left-Right
//Will return a different hash result

#include <string_view>

constexpr unsigned int make_sdbm_hash(std::string_view text) 
{
    unsigned int hash = 0;
    for (char c : text) 
    {
        hash = (unsigned int)c + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}