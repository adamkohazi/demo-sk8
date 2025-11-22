#pragma once

#include "keyframes.h"

// Function to load keyframes from JSON
#ifdef DEBUG
#include <fstream>
constexpr size_t MAX_KEYFRAMES = 255;

Keyframe<float> boardEuler_x[MAX_KEYFRAMES];
Keyframe<float> boardEuler_y[MAX_KEYFRAMES];
Keyframe<float> boardEuler_z[MAX_KEYFRAMES];

Keyframe<float> boardPos_x[MAX_KEYFRAMES];
Keyframe<float> boardPos_y[MAX_KEYFRAMES];
Keyframe<float> boardPos_z[MAX_KEYFRAMES];

Keyframe<float> bodyHipEuler_x[MAX_KEYFRAMES];
Keyframe<float> bodyHipEuler_y[MAX_KEYFRAMES];
Keyframe<float> bodyHipEuler_z[MAX_KEYFRAMES];

Keyframe<float> bodyHipPosition_x[MAX_KEYFRAMES];
Keyframe<float> bodyHipPosition_y[MAX_KEYFRAMES];
Keyframe<float> bodyHipPosition_z[MAX_KEYFRAMES];

Keyframe<float> hip_rotation_r[MAX_KEYFRAMES];
Keyframe<float> hip_flexion_r[MAX_KEYFRAMES];
Keyframe<float> hip_abduction_r[MAX_KEYFRAMES];

Keyframe<float> knee_flexion_r[MAX_KEYFRAMES];
Keyframe<float> ankle_flexion_r[MAX_KEYFRAMES];

Keyframe<float> hip_rotation_l[MAX_KEYFRAMES];
Keyframe<float> hip_flexion_l[MAX_KEYFRAMES];
Keyframe<float> hip_abduction_l[MAX_KEYFRAMES];

Keyframe<float> knee_flexion_l[MAX_KEYFRAMES];
Keyframe<float> ankle_flexion_l[MAX_KEYFRAMES];

// Temporary buffer
std::unordered_map<std::string, Keyframe<float>*> trackMap = {
    {"boardEuler_x", boardEuler_x},
    {"boardEuler_y", boardEuler_y},
    {"boardEuler_z", boardEuler_z},

    {"boardPos_x", boardPos_x},
    {"boardPos_y", boardPos_y},
    {"boardPos_z", boardPos_z},

    {"bodyHipEuler_x", bodyHipEuler_x},
    {"bodyHipEuler_y", bodyHipEuler_y},
    {"bodyHipEuler_z", bodyHipEuler_z},

    {"bodyHipPosition_x", bodyHipPosition_x},
    {"bodyHipPosition_y", bodyHipPosition_y},
    {"bodyHipPosition_z", bodyHipPosition_z},
    
    {"hip_rotation_r", hip_rotation_r},
    {"hip_flexion_r", hip_flexion_r},
    {"hip_abduction_r", hip_abduction_r},

    {"knee_flexion_r", knee_flexion_r},
    {"ankle_flexion_r", ankle_flexion_r},

    {"hip_rotation_l", hip_rotation_l},
    {"hip_flexion_l", hip_flexion_l},
    {"hip_abduction_l", hip_abduction_l},

    {"knee_flexion_l", knee_flexion_l},
    {"ankle_flexion_l", ankle_flexion_l}
};

template<typename ValueType>
float loadKeyframesFromJSON(const std::string& filename) {
    // Read file
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    // Parse JSON
    json j;
    file >> j;

    // Map from track to next insertion index
    std::unordered_map<std::string, size_t> insertIndices;

    for (const auto& frame : j["keyframes"]) {
        float time = frame["time"];
        for (const auto& node : frame["nodes"]) {
            std::string track = node["track"];
            ValueType value = node["value"];
            Interpolation mode = static_cast<Interpolation>(node["mode"].get<int>());

            // Find destination array pointer
            auto it = trackMap.find(track);
            if (it == trackMap.end()) {
                // Unknown track, skip or warn
                continue;
            }

            size_t index = insertIndices[track]++;
            if (index >= MAX_KEYFRAMES) {
                // Too many keyframes, skip extras
                continue;
            }

            // Directly write into the array
            it->second[index] = Keyframe<ValueType>{ time, value, mode };
        }
    }

    return j["time"];
}

#else
constexpr Keyframe<float> boardPos_x[] = {
    { 0.0f, 0.0f, LINEAR }
};

constexpr Keyframe<float> boardPos_y[] = {
    { 0.0f, 0.0f, LINEAR }
};

constexpr Keyframe<float> boardPos_z[] = {
    { 0.0f, 0.0f, LINEAR }
};

constexpr Keyframe<float> boardEuler_x[] = {
    { 0.0f, 0.0f, LINEAR }
};

constexpr Keyframe<float> boardEuler_y[] = {
    { 0.0f, 0.0f, LINEAR }
};

constexpr Keyframe<float> boardEuler_z[] = {
    { 0.0f, 0.0f, LINEAR }
};

constexpr Keyframe<float> legRightHip_x[] = {
    { 0.0f, 0.0f, LINEAR }
};

constexpr Keyframe<float> legRightHip_y[] = {
    { 0.0f, 0.0f, LINEAR }
};

constexpr Keyframe<float> legRightHip_z[] = {
    { 0.0f, 0.0f, LINEAR }
};

constexpr Keyframe<float> legRightKnee[] = {
    { 0.0f, 0.0f, LINEAR }
};

constexpr Keyframe<float> legRightAnkle[] = {
    { 0.0f, 0.0f, LINEAR }
};

constexpr Keyframe<float> legLeftHip_x[] = {
    { 0.0f, 0.0f, LINEAR }
};

constexpr Keyframe<float> legLeftHip_y[] = {
    { 0.0f, 0.0f, LINEAR }
};

constexpr Keyframe<float> legLeftHip_z[] = {
    { 0.0f, 0.0f, LINEAR }
};

constexpr Keyframe<float> legLeftKnee[] = {
    { 0.0f, 0.0f, LINEAR }
};

constexpr Keyframe<float> legLeftAnkle[] = {
    { 0.0f, 0.0f, LINEAR }
};

constexpr Keyframe<float> bodyHipPosition_x[] = {
    { 0.0f, 0.0f, LINEAR }
};

constexpr Keyframe<float> bodyHipPosition_y[] = {
    { 0.0f, 0.1f, LINEAR }
};

constexpr Keyframe<float> bodyHipPosition_z[] = {
    { 0.0f, 0.0f, LINEAR }
};

constexpr Keyframe<float> bodyHipEuler_x[] = {
    { 0.0f, 0.0f, LINEAR }
};

constexpr Keyframe<float> bodyHipEuler_y[] = {
    { 0.0f, 0.0f, LINEAR }
};

constexpr Keyframe<float> bodyHipEuler_z[] = {
    { 0.0f, 0.0f, LINEAR }
};
#endif

