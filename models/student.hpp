#pragma once

#include <cstdint>
#include <string>
#include <iostream>
#include <sstream>

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
        os << s.id << ":" << s.firstName << ":" << s.lastName << ":" << s.birthYear << ":" << s.gpa;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Student& s) {
        std::string token;
        if (!(is >> token)) return is;
        if (token == "#") return is;

        std::stringstream ss(token);
        std::string part;
        if (std::getline(ss, part, ':')) s.id = std::stoull(part);
        if (std::getline(ss, part, ':')) s.firstName = part;
        if (std::getline(ss, part, ':')) s.lastName = part;
        if (std::getline(ss, part, ':')) s.birthYear = std::stoi(part);
        if (std::getline(ss, part, ':')) s.gpa = std::stod(part);

        return is;
    }
};
