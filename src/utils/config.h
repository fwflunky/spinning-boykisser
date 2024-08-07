//
// Created by user on 07.08.2024.
//

#ifndef CONFIG_H
#define CONFIG_H
#include <string>


namespace utils {
    class config {
    public:
        static void load();
        static void save(int wx, int wy, unsigned char cm);

        static int getWinX() {
            return winX;
        };
        static int getWinY() {
            return winY;
        };
        static int getColorMode() {
            return colorMode;
        };
    private:
        static inline unsigned char colorMode = 4;
        static inline int winX = 0;
        static inline int winY = 0;

        static inline std::string const path = "spincat.conf";
    };
}


#endif //CONFIG_H
