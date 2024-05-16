#include <libxml/parser.h>

struct Point {
    long time_ts;
    double lat;
    double lon;
    double ele;
};

struct Point *createPointArray(int size);

struct Point *parserGpx(const char *filename, int *numPoints);

time_t seconds_since_fit_epoch();