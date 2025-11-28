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

// Keyframes are floats, with the last 4 bits of the mantissa being the interpolation type
#define STEP 0
#define LINEAR 1
#define QUADRATIC_IN 2
#define QUADRATIC_OUT 3
#define SMOOTHSTEP 4

// Timestamps are either loaded or defined
#ifdef DEBUG
extern float timestamps[];
#else
extern const float timestamps[];
#endif

// Interpolation function template
template<size_t N>
float findValue(float time, const float(&keys)[N]) {
    // Find previous and next keyframes
    uint8_t i;
    for (i = 1; (i < N) && (time >= timestamps[i]); i++);

    float t = (time - timestamps[i - 1]) / (timestamps[i] - timestamps[i - 1]);

    float a = keys[i - 1];
    float b = keys[i];

    uint8_t mode = *((unsigned int*)&b) & 0xF; // Extract last 4 bits

    // Interpolate
    return
        mode == STEP ? INTERP_STEP(a, b, t) :
        mode == LINEAR ? INTERP_LINEAR(a, b, t) :
        mode == QUADRATIC_IN ? INTERP_QUADRATIC_IN(a, b, t) :
        mode == QUADRATIC_OUT ? INTERP_QUADRATIC_OUT(a, b, t) :
        INTERP_SMOOTHSTEP(a, b, t);

    // Fallback to last keyframe
    return keys[N - 1];
}

/*
Notes:
- keyframe array needs to start with a keyframe with time<=0.0
- keyframes need to be sorted
*/
