class pixelGL
{
    //Use this as a namespace for objects
public:
    pixelGL( void )
    {
    }
private:
};

class genericObject
{
public:
    genericObject( void ) : layer(0), xOffset(0), yOffset(0), xScale(0), yScale(0){}
    void setLayer( int32_t var )
    {
        layer = var;
    }
private:
    float layer;
    float xOffset;
    float yOffset;
    float xScale;
    float yScale;

};

class point
{
public:
    point( void );
    point( int32_t xVar, int32_t yVar )
    {
        x = xVar;
        y = yVar;
    }
    void setPoint( int32_t xVar, int32_t yVar )
    {
        x = xVar;
        y = yVar;
    }
private:
    int32_t x;
    int32_t y;
};

class box: public genericObject
{
public:
    box( int32_t x1Var, int32_t y1Var, int32_t x2Var, int32_t y2Var )
    {
        p1.setPoint(x1Var, y1Var);
        p2.setPoint(x2Var, y2Var);
    }
    void setPoints( int32_t x1Var, int32_t y1Var, int32_t x2Var, int32_t y2Var )
    {
        p1.setPoint(x1Var, y1Var);
        p2.setPoint(x2Var, y2Var);
    }
private:
    point p1;
    point p2;

};

class line: public genericObject
{
public:
    line( int32_t x1Var, int32_t y1Var, int32_t x2Var, int32_t y2Var )
    {
        p1.setPoint(x1Var, y1Var);
        p2.setPoint(x2Var, y2Var);
    }
    void setPoints( int32_t x1Var, int32_t y1Var, int32_t x2Var, int32_t y2Var )
    {
        p1.setPoint(x1Var, y1Var);
        p2.setPoint(x2Var, y2Var);
    }
private:
    point p1;
    point p2;

};
