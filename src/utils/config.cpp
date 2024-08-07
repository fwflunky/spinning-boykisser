//
// Created by user on 07.08.2024.
//

#include "config.h"

#include <filesystem>
#include <fstream>

void utils::config::save(int const wx, int const wy, unsigned char const cm) {
    std::ofstream ss(path, std::ios::binary);
    auto const fdata = std::string("\x00\x00\x00\x00\x00\x00\x00\x00\x00", 9);

    int offset = 0;

    memcpy((void*) &fdata.data()[offset], &wx, sizeof(wx));
    offset += sizeof(wx);
    memcpy((void*) &fdata.data()[offset], &wy, sizeof(wy));
    offset += sizeof(wy);
    memcpy((void*) &fdata.data()[offset], &cm, sizeof(cm));
    offset += sizeof(cm);

    ss.write(fdata.data(), fdata.size());
    ss.close();
}

void utils::config::load() {
    if(!std::filesystem::exists(path)) {
        save(0, 0, 4);
        return;
    }

    std::ifstream ss(path, std::ios::binary);

    auto fsize = ss.tellg();
    ss.seekg( 0, std::ios::end);
    fsize = ss.tellg() - fsize;
    std::string buffer(fsize, ' ');
    ss.seekg(0, std::ios::beg);
    ss.read(buffer.data(), fsize);
    buffer.resize(buffer.size() + 32);

    int offset = 0;

    memcpy(&winX, buffer.data() + offset, sizeof(winX));
    offset += sizeof(winX);

    memcpy(&winY, buffer.data() + offset, sizeof(winY));
    offset += sizeof(winY);

    memcpy(&colorMode, buffer.data() + offset, sizeof(colorMode));
    offset += sizeof(colorMode);
}
