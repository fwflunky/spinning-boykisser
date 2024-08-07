//
// Created by user on 07.08.2024.
//

#ifndef GIF_H
#define GIF_H
#include <vector>


namespace utils {
    class gif {
    public:
        static bool prepareData();
        static std::vector<std::uint32_t>& nextFrame();
    private:
        static inline std::vector<std::vector<std::uint32_t>> frames;
        static inline int currentFrame = 0;
        static inline int maxFrame = 0;
    };
}


#endif //GIF_H
