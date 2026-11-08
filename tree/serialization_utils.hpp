#pragma once

#include "bst_exceptions.hpp"

#include <cctype>
#include <cmath>
#include <iomanip>
#include <istream>
#include <limits>
#include <ostream>
#include <string>
#include <type_traits>

// После значения разрешены только пробельный символ или конец строки
inline void checkTokenEnd(std::istream& is) {
    if (is.eof()) return;

    const int ch = is.peek();

    if (ch != std::char_traits<char>::eof() &&
        !std::isspace(static_cast<unsigned char>(ch))) {
        throw ParseException("Unexpected characters after value");
    }
}

// После законченной структуры разрешены только пробельные символы
inline void checkInputEnd(std::istream& is) {
    is >> std::ws;

    if (is.peek() != std::char_traits<char>::eof()) {
        throw ParseException("Unexpected trailing data");
    }
}

template <typename T>
void writeValue(std::ostream& os, const T& value) {
    if constexpr (std::is_floating_point_v<T>) {
        if (!std::isfinite(value)) {
            throw ParseException(
                "Non-finite numbers are not supported"
            );
        }

        os << std::setprecision(
            std::numeric_limits<T>::max_digits10
        );
    }

    if constexpr (std::is_same_v<T, std::string>) {
        os << std::quoted(value);
    } else {
        os << value;
    }

    if (!os) {
        throw ParseException("Failed to write value");
    }
}

template <typename T>
T readValue(std::istream& is) {
    T value{};
    is >> std::ws;

    if constexpr (std::is_same_v<T, std::string>) {
        if (is.peek() != '"' ||
            !(is >> std::quoted(value))) {
            throw ParseException("Expected a quoted string");
        }
    } else {
        if (!(is >> value)) {
            throw ParseException("Invalid value");
        }
    }

    if constexpr (std::is_floating_point_v<T>) {
        if (!std::isfinite(value)) {
            throw ParseException(
                "Non-finite numbers are not supported"
            );
        }
    }

    checkTokenEnd(is);
    return value;
}
