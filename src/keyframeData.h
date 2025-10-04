#pragma once

#include "keyframes.h"

// Keyframe data
constexpr Keyframe<float> brightnessKeys[] = {
    { 0.0f, 0.0f, LINEAR },
    { 1.0f, 1.0f, QUADRATIC },
    { 2.0f, 0.5f, STEP }
};