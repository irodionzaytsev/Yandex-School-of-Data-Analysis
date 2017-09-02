#include <iostream>
#include <memory>

void destroy(FILE* file_desc) {
    fclose(file_desc);
}

std::unique_ptr<FILE, void(*)(FILE*)> open_unique(const std::string& filename, const std::string& mode) {
    return std::unique_ptr<FILE, void(*)(FILE*)>(
            fopen(filename.c_str(), mode.c_str()),
            [](FILE* file_desc){ destroy(file_desc); });
};

std::shared_ptr<FILE> open_shared(const std::string& filename, const std::string& mode) {
    return std::shared_ptr<FILE>(
            fopen(filename.c_str(), mode.c_str()),
            [](FILE* file_desc){ destroy(file_desc); });
}
