#pragma once

#include "keyframes.h"

// Function to load keyframes from JSON
#ifdef DEBUG
#include <fstream>
constexpr size_t MAX_KEYFRAMES = 32;

Keyframe<float> camera[MAX_KEYFRAMES];

Keyframe<float> boardEuler_x[MAX_KEYFRAMES];
Keyframe<float> boardEuler_y[MAX_KEYFRAMES];
Keyframe<float> boardEuler_z[MAX_KEYFRAMES];

Keyframe<float> boardPos_x[MAX_KEYFRAMES];
Keyframe<float> boardPos_y[MAX_KEYFRAMES];
Keyframe<float> boardPos_z[MAX_KEYFRAMES];

Keyframe<float> hip_twist[MAX_KEYFRAMES];

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
    {"camera", camera},

    {"boardEuler_x", boardEuler_x},
    {"boardEuler_y", boardEuler_y},
    {"boardEuler_z", boardEuler_z},

    {"boardPos_x", boardPos_x},
    {"boardPos_y", boardPos_y},
    {"boardPos_z", boardPos_z},

    {"hip_twist", hip_twist},

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
    float time = 0.0f;

    while (true) {
        try {
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
                time = frame["time"];
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

            // If no exceptions were thrown, return the time value from the JSON
            return j["time"];
        }
        catch (const std::exception& e) {
            // If an exception is thrown, retry with a small delay
            std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Adjust as needed
        }
    }
}


#else
    #include "../assets/keyframes/keyframe_data.h"
#endif

