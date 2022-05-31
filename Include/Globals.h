#ifndef _H_GLOBALS
#define _H_GLOBALS

#include <string>

#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)
#define SRC std::string(std::getenv("ASS2_PATH"))
#define DUMP_LOC SRC+"/Out/"

// Font paths
#define FONT_ROBOTO SRC+"Res/Roboto-Regular.ttf"

#define PI 3.1415926535
#define MAX_LOD 200
#define ONE_MB 1000000

#define FPS 100.0
#define INTERVAL_MAX 1.0/FPS

#define SCREEN_W 1920
#define SCREEN_H 1080

extern float delta;
#endif