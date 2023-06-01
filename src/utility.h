#pragma once

#include <string>

namespace utility {

unsigned char random_char();
std::string generate_hex(const unsigned int len);
std::string guid();

} // namespace utility