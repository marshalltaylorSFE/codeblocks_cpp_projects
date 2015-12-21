
// basic file operations
#include <iostream>
#include <fstream>

#include "bitmap_image.hpp"
#include "graphWrapper.h"

bitmap_image image(1024,768);
image_drawer draw(image);

void GridParams::setColor( uint8_t redVar, uint8_t greenVar, uint8_t blueVar)
{
    red = redVar;
    green = greenVar;
    blue = blueVar;
}

void GraphWrapper::open( void )
{
    image.set_region(3,3,1018,762,bitmap_image::red_plane,255);
    image.set_region(3,3,1018,762,bitmap_image::blue_plane,255);
    image.set_region(3,3,1018,762,bitmap_image::green_plane,255);

}

void GraphWrapper::close( void )
{
    image.save_image("newfile.bmp");

}

GridParams GraphWrapper::makeGrid( int32_t x1Var, int32_t y1Var, int32_t x2Var, int32_t y2Var, int32_t xTickVar, int32_t yTickVar )
{
    GridParams returnObj(0,0);

    returnObj.xOrigin = x1Var;
    returnObj.yOrigin = y2Var;
    returnObj.lastX = 0;
    returnObj.lastY = 0;

    //Draw a grid in the space
    //Grid prep
    draw.pen_color(200,200,200);
    draw.pen_width(1);

    //draw outline
    draw.vertical_line_segment(y1Var,y2Var,x1Var);
    draw.vertical_line_segment(y1Var,y2Var,x2Var);

    //draw horiztonal
    int grid = returnObj.yOrigin;
    while( grid > y1Var )
    {
        draw.horiztonal_line_segment(x1Var,x2Var,grid);
        grid = grid - yTickVar;
    }
    draw.horiztonal_line_segment(x1Var,x2Var,y1Var);

    //draw vertical
    grid = x2Var;
    while( grid > returnObj.xOrigin )
    {
        draw.vertical_line_segment(y1Var,y2Var,grid);
        grid = grid - xTickVar;
    }
    draw.vertical_line_segment(y1Var,y2Var,x2Var);


    return returnObj;
}

void GraphWrapper::plotPoint( int32_t x1Var, int32_t y1Var, GridParams& gPVar )
{
    draw.pen_width(2);
    draw.pen_color(gPVar.red, gPVar.green, gPVar.blue);
    draw.line_segment(gPVar.xOrigin + gPVar.lastX, gPVar.yOrigin - gPVar.lastY, gPVar.xOrigin + x1Var, gPVar.yOrigin - y1Var);
    gPVar.lastX = x1Var;
    gPVar.lastY = y1Var;
}
