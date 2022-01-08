#ifndef FILTER_H
#define FILTER_H
 
#include "handler.h"

class FilterBase 
{
    public:
        FilterBase(HERTZ freq) { RC = 1.0 / ( 2 * PI * double(freq)); dT = 1.0 / 44100.0; };
        virtual double filter(double input) = 0;
    protected:
        double RC,dT;
};


/* 
    From wikipedia,
    function lowpass(real[0..n] x, real dt, real RC)
        var real[0..n] y
        var real α := dt / (RC + dt)
        y[0] := α * x[0]
        for i from 1 to n
            y[i] := α * x[i] + (1-α) * y[i-1]
        return y

    https://en.wikipedia.org/wiki/Low-pass_filter
*/
class LowPassFilter : FilterBase
{
    public:
        LowPassFilter(HERTZ freq) : FilterBase(freq) { a = dT / (dT + RC); };
        virtual double filter(double input) final { prevY = prevY + a * (input - prevX); prevX = input; return prevY; };
    private:
        double a;
        double prevX = 0;
        double prevY = 0;
};


/*
    From wikipedia,
    function highpass(real[1..n] x, real dt, real RC)
        var real[1..n] y
        var real α := RC / (RC + dt)
        y[1] := x[1]
        for i from 2 to n
            y[i] := α × y[i−1] + α × (x[i] − x[i−1])
        return y

    https://en.wikipedia.org/wiki/High-pass_filter
*/
class HighPassFilter : FilterBase
{
    public:
        HighPassFilter(HERTZ freq) : FilterBase(freq) { a = RC / (dT + RC); };
        virtual double filter(double input) final { prevY = a * (prevY + input - prevX); prevX = input; return prevY; };
    private:
        double a;
        double prevX = 0;
        double prevY = 0;
};



#endif