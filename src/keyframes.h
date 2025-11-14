#pragma once


#ifdef DEBUG
#include "nlohmann/json.hpp"
#include <unordered_map>
#include <vector>
#include <stdexcept>

using json = nlohmann::json;
#endif


// Compact type definition for 8-bit ints
typedef unsigned char uint8_t;

// Interpolation types
#define INTERP_STEP(a,b,t)          (a)
#define INTERP_LINEAR(a,b,t)        ((a) + (t) * ((b) - (a)))
#define INTERP_QUADRATIC_IN(a,b,t)  ((a) + (t) * (t) * ((b) - (a)))
#define INTERP_QUADRATIC_OUT(a,b,t) ((a) + (2.f * (t) - (t) * (t)) * ((b) - (a)))
#define INTERP_SMOOTHSTEP(a,b,t)    ((a) + (t) * (t) * (3.f - 2.f * (t)) * ((b) - (a)))
#define INTERP_CUBIC_IN(a,b,t)      ((a) + (t) * (t) * (t) * ((b) - (a)))
#define INTERP_CUBIC_OUT(a,b,t)     ((a) + (1.f - (1.f - (t)) * (1.f - (t)) * (1.f - (t))) * ((b) - (a)))
// Visualization:
// https://graphtoy.com/?f1(x,t)=floor(x)&v1=true&f2(x,t)=floor(x)%20+%20fract(x)%20*%20(floor(x+1)-floor(x))&v2=true&f3(x,t)=floor(x)%20+%20%20fract(x)%20*%20fract(x)%20*%20(floor(x+1)-floor(x))&v3=true&f4(x,t)=floor(x)%20+%20(fract(x)%20*%20fract(x)%20*%20(3-2*fract(x)))%20*%20(floor(x+1)-floor(x))&v4=true&f5(x,t)=floor(x)%20+%20fract(x)%20*%20fract(x)%20*%20fract(x)%20*%20(floor(x+1)-floor(x))&v5=true&f6(x,t)=(floor(x)%20+%20(3*fract(x)%20-%203*fract(x)%20*%20fract(x)%20+%20fract(x)%20*%20fract(x)%20*%20fract(x))%20*%20(floor(x+1)-floor(x)))&v6=true&grid=1&coords=0,0,1.962095889918702


// Interpolation types as small constants
enum Interpolation : uint8_t {
    STEP = 0,
    LINEAR = 1,
    QUADRATIC_IN = 2,
    QUADRATIC_OUT = 3,
    SMOOTHSTEP = 4
};

// Anatomy of a keyframe
template<typename ValueType>
struct Keyframe {
    float time;
    ValueType value;
    Interpolation mode;
};


// Interpolation function template
#pragma optimize("", off)
template<typename ValueType, size_t N>
ValueType findValue(float time, const Keyframe<ValueType>(&keys)[N]) {
    // Find previous and next keyframes
    size_t i = 1;
    while ((i < N) && (time > keys[i].time))
        i++;
    float a = (float)keys[i - 1].value;
    float b = (float)keys[i].value;
    float t = (time - keys[i - 1].time) / (keys[i].time - keys[i - 1].time);
    // Interpolate
    return
        keys[i].mode == STEP ? INTERP_STEP(a, b, t) :
        keys[i].mode == LINEAR ? INTERP_LINEAR(a, b, t) :
        keys[i].mode == QUADRATIC_IN ? INTERP_QUADRATIC_IN(a, b, t) :
        keys[i].mode == QUADRATIC_OUT ? INTERP_QUADRATIC_OUT(a, b, t) :
        INTERP_SMOOTHSTEP(a, b, t);

    // Fallback to last keyframe
    return (float)keys[N - 1].value;
}
#pragma optimize("", on)

/*
Notes:
- keyframe array needs to start with a keyframe with time<=0.0
- keyframes need to be sorted
*/
