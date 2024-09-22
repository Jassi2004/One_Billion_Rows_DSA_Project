#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>    // For std::min, std::max, std::sort
#include <iomanip>      // For std::setprecision
#include <stdexcept>    // For std::runtime_error
#include <utility>      // For std::exchange
#include <cstdint>      // For uint64_t

struct Record {
    uint64_t cnt;
    double sum;
    float min;
    float max;
};

using DB = std::unordered_map<std::string, Record>;

DB process_input(std::istream &in) {
    DB db;

    std::string station;
    std::string value;

    while (std::getline(in, station, ';') && std::getline(in, value, '\n')) {
        float fp_value = std::stof(value);
        auto [it, inserted] = db.try_emplace(station, Record{1, fp_value, fp_value, fp_value});
        if (!inserted) {
            it->second.min = std::min(it->second.min, fp_value);
            it->second.max = std::max(it->second.max, fp_value);
            it->second.sum += fp_value;
            ++it->second.cnt;
        }
    }

    return db;
}

void format_output(std::ostream &out, const DB &db) {
    std::vector<std::string> names(db.size());
    std::transform(db.begin(), db.end(), names.begin(), [](const auto &pair) {
        return pair.first;
    });
    std::sort(names.begin(), names.end());

    std::string delim = "";
    out << std::setiosflags(out.fixed | out.showpoint) << std::setprecision(1);
  
    out << "{";
    for (const auto &k : names) {
        const auto &record = db.at(k);
        out << std::exchange(delim, ", ") << k << "="
            << record.min << "/" << (record.sum / static_cast<double>(record.cnt)) << "/" << record.max;
    }
    out << "}\n";
}

int main() {
    std::ifstream ifile("./InputFile/measurements.txt", std::ifstream::in);
    if (!ifile.is_open()) {
        throw std::runtime_error("Failed to open the input file.");
    }

    auto db = process_input(ifile);
    format_output(std::cout, db);
}
