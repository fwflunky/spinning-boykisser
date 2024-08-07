//
// Created by user on 07.08.2024.
//

#include "gif.h"
#include "../bk.h"

#include <EasyGifReader.h>

bool utils::gif::prepareData() {
    try {
        auto const gifReader = EasyGifReader::openMemory(bk_gif, sizeof(bk_gif));

        for (const EasyGifReader::Frame &frame : gifReader) {
            std::uint32_t const* framePixels = frame.pixels();
            std::vector<std::uint32_t> vf = {};

            for(int i = 0; i < gifReader.width() * gifReader.height(); i++){
                vf.emplace_back(framePixels[i]);
            }
            frames.emplace_back(vf);
        }
        maxFrame = (int) frames.size() - 1;
    } catch (EasyGifReader::Error) {
        return false;
    }
    return true;
}

std::vector<std::uint32_t>& utils::gif::nextFrame() {
    if(currentFrame++ >= maxFrame)
        currentFrame = 0;

    return frames.at(currentFrame);
}