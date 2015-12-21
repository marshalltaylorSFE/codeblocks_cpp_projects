// basic file operations
#include <iostream>
#include <fstream>

#include "bitmap_image.hpp"

class GridParams
{
public:
    GridParams( int32_t, int32_t ){};
    void setColor(uint8_t, uint8_t, uint8_t);
    int32_t xOrigin;
    int32_t yOrigin;
    int32_t lastX;
    int32_t lastY;
    int8_t red;
    int8_t green;
    int8_t blue;
private:
};

class GraphWrapper
{
public:
    GraphWrapper( void ){};
    void open( void );
    void close( void );
    GridParams makeGrid( int32_t, int32_t, int32_t, int32_t, int32_t, int32_t );
    void plotPoint( int32_t, int32_t, GridParams& );
private:
};
