#pragma once

typedef unsigned char uint8_t;

// Interpolation types as small constants
enum Interpolation : uint8_t {
    STEP = 0,
    LINEAR = 1,
    QUADRATIC = 2
};

// Anatomy of a keyframe
template<typename ValueType>
struct Keyframe {
    float time;
    ValueType value;
    Interpolation mode;
};

// Interpolation function template
template<typename ValueType, uint8_t N>
constexpr ValueType findValue(const Keyframe<ValueType>(&keys)[N], float time) {
    // Find previous and next keyframes
    for (uint8_t i = 1; i < N && time < keys[i].time; ++i) {
        float a = (float)keys[i - 1].value;
        float b = (float)keys[i].value;
        float t = (time - keys[i - 1].time) / (keys[i].time - keys[i - 1].time);
        // Interpolate
        return keys[i].mode == STEP   ? a :
               keys[i].mode == LINEAR ? a + t * (b - a) :
                                        a + t * t * (b - a);
    }
    // Fallback to last keyframe
    return (float)keys[N - 1].value;
}

/*
Notes:
- keyframe array needs to start with a keyframe with time<=0.0
- keyframes need to be sorted
*/
