#pragma once

#include <cstdint>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <locale>
#include <cmath>
#include <cctype>
#include <utility>

struct Student {
    uint64_t id{0};
    std::string firstName;
    std::string lastName;
    int birthYear{2000};
    double gpa{0.0};

    Student() = default;
    Student(uint64_t sId, std::string first, std::string last, int year, double avgScore)
        : id(sId), firstName(std::move(first)), lastName(std::move(last)), birthYear(year), gpa(avgScore) {}

    bool operator<(const Student& other) const noexcept { return id < other.id; }
    bool operator>(const Student& other) const noexcept { return other < *this; }
    bool operator<=(const Student& other) const noexcept { return !(other < *this); }
    bool operator>=(const Student& other) const noexcept { return !(*this < other); }

    bool operator==(const Student& other) const noexcept {
        return id == other.id &&
               firstName == other.firstName &&
               lastName == other.lastName &&
               birthYear == other.birthYear &&
               gpa == other.gpa;
    }

    bool operator!=(const Student& other) const noexcept { return !(*this == other); }

    friend std::ostream& operator<<(std::ostream& os, const Student& s) {
        if (!std::isfinite(s.gpa)) {
            os.setstate(std::ios::failbit);
            return os;
        }

        std::ostringstream record;
        record.imbue(std::locale::classic());

        record << s.id << ':'
            << std::quoted(s.firstName) << ':'
            << std::quoted(s.lastName) << ':'
            << s.birthYear << ':'
            << std::setprecision(std::numeric_limits<double>::max_digits10)
            << s.gpa;
        return os << record.str();
    }

    friend std::istream& operator>>(std::istream& is, Student& s) {
        Student parsed;

        const auto readColon = [&is]() {
            char ch;
            return static_cast<bool>(is.get(ch)) && ch == ':';
        };

        is >> std::ws;
        if (is.peek() == '-' ||
            !(is >> parsed.id) ||
            !readColon() ||
            is.peek() != '"' ||
            !(is >> std::quoted(parsed.firstName)) ||
            !readColon() ||
            is.peek() != '"' ||
            !(is >> std::quoted(parsed.lastName)) ||
            !readColon() ||
            !(is >> parsed.birthYear) ||
            !readColon() ||
            !(is >> parsed.gpa) ||
            !std::isfinite(parsed.gpa)) {
            is.setstate(std::ios::failbit);
            return is;
        }

        const int ch = is.eof() ? std::char_traits<char>::eof() : is.peek();

        if (ch != std::char_traits<char>::eof() && !std::isspace(static_cast<unsigned char>(ch))) {
            is.setstate(std::ios::failbit);
            return is;
        }

        s = std::move(parsed);
        return is;
    }
};
