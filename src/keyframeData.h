#pragma once

#include "keyframes.h"
// Keyframe data

// Key Board Positions:
/*
const BoardPosition board_idle = BoardPosition(vec3(0), vec3(0));
const BoardPosition board_pop = BoardPosition(vec3(0.2, 0.15, 0), vec3(0, 0, 0.8));
const BoardPosition board_ollie_top = BoardPosition(vec3(0.2, 0.8, 0.0), vec3(0.0, 0.0, 0.3));

const BoardPosition board_kickflip_top = BoardPosition(vec3(0.2, 0.8, 0.0), vec3(-6.0, 0.0, -0.3));
const BoardPosition board_kickflip_land = BoardPosition(vec3(0), vec3(-TWO_PI, 0, 0));

const BoardPosition board_tre_pop = BoardPosition(vec3(0.2, 0.15, 0.0), vec3(-0.4, 0.5, 0.95));
const BoardPosition board_tre_top = BoardPosition(vec3(0.2, 0.8, 0.0), vec3(-6.0, 5.0, -0.5));
const BoardPosition board_tre_land = BoardPosition(vec3(0.0, 0.0, 0.0), vec3(-TWO_PI, TWO_PI, 0.0));

// Key Body Positions
const BodyPosition body_idle = BodyPosition(
    BodyPosition()
);
*/

/*
const BodyPosition body_idle = BodyPosition(
    vec3(0.05, -0.0, 0.05), // position
    0.0, // legRTwist - rotation
    0.0, // legLTwist - rotation
    0.1, // legRAngle - abduction
    -0.05, // legLAngle - abduction
    0.3, // hipRAngle - flexion
    0.3, // hipLAngle - flexion
    0.0, // kneeRAngle
    0.0, // kneeLAngle
    -HALF_PI, // ankleRAngle
    -HALF_PI // ankleLAngle
);
const BodyPosition body_ready = BodyPosition(
    vec3(0.15, -0.0, 0.1), // position
    0.0, // legRTwist - rotation
    0.0, // legLTwist - rotation
    0.05, // legRAngle - abduction
    -0.05, // legLAngle - abduction
    0.4, // hipRAngle - flexion
    0.4, // hipLAngle - flexion
    0.0, // kneeRAngle
    0.0, // kneeLAngle
    -1.7, // ankleRAngle
    -HALF_PI // ankleLAngle
);

const BodyPosition body_pop = BodyPosition(
    vec3(0.2, 0.1, 0.05), // position
    0.0, // legRTwist - rotation
    -0.5, // legLTwist - rotation
    0.2, // legRAngle - abduction
    -0.05, // legLAngle - abduction
    0.0, // hipRAngle - flexion
    0.8, // hipLAngle - flexion
    0.0, // kneeRAngle
    0.3, // kneeLAngle
    -0.2, // ankleRAngle
    -2.3 // ankleLAngle
);

const BodyPosition body_ollie = BodyPosition(vec3(0.2, 0.4, 0.05), 0.0, -0.7, 0.2, -0.05, 0.4, 1.2, 0.0, 0.0, -1.5, -HALF_PI);
const BodyPosition body_jump = BodyPosition(vec3(0.2, 0.5, 0.05), 0.0, -0.5, 0.2, -0.05, 1.1, 1.3, 0.0, 0.0, -1.5, -1.5);
*/
/*
    // Kickflip
    BoardFrame(1.0, 1.0, board_idle),
    BoardFrame(1.1, 0.8, board_pop),
    BoardFrame(1.3, 1.7, board_kickflip_top),
    BoardFrame(1.6, 1.0, board_kickflip_land),
    BoardFrame(1.6, 1.0, board_idle), //Untwist

    // Tre flip
    BoardFrame(2.0, 1.0, board_idle),
    BoardFrame(2.1, 0.8, board_tre_pop),
    BoardFrame(2.3, 1.5, board_tre_top),
    BoardFrame(2.6, 1.0, board_tre_land),
    BoardFrame(2.6, 1.0, board_idle), //Untwist
    BoardFrame(3.0, 1.0, board_idle)
*/
// Trick 1 - Ollie
/*
const BoardMove boardOllie[3] = BoardMove[](
    BoardMove(0.1, 1.0, board_idle, board_pop),
    BoardMove(0.2, 0.8, board_pop, board_ollie_top),
    BoardMove(0.3, 1.7, board_ollie_top, board_idle)
);

const BodyMove bodyOllie[4] = BodyMove[](
    BodyMove(0.1, 1.0, body_ready, body_pop),
    BodyMove(0.1, 0.8, body_pop, body_ollie),
    BodyMove(0.1, 1.0, body_ollie, body_jump),
    BodyMove(0.3, 1.8, body_jump, body_idle)
);

/*
    BodyFrame(0.0, 1.0, body_ready),
    BodyFrame(0.1, 0.8, body_pop),
    BodyFrame(0.2, 1.0, body_ollie),
    BodyFrame(0.3, 1.8, body_jump),
    BodyFrame(0.6, 1.0, body_idle)
*/

// Function to load keyframes from JSON
#ifdef DEBUG
#include <fstream>
constexpr size_t MAX_KEYFRAMES = 255;

Keyframe<float> boardPos_x[MAX_KEYFRAMES];
Keyframe<float> boardPos_y[MAX_KEYFRAMES];
Keyframe<float> boardPos_z[MAX_KEYFRAMES];

Keyframe<float> boardEuler_x[MAX_KEYFRAMES];
Keyframe<float> boardEuler_y[MAX_KEYFRAMES];
Keyframe<float> boardEuler_z[MAX_KEYFRAMES];

Keyframe<float> legRightHip_x[MAX_KEYFRAMES];
Keyframe<float> legRightHip_y[MAX_KEYFRAMES];
Keyframe<float> legRightHip_z[MAX_KEYFRAMES];
Keyframe<float> legRightKnee[MAX_KEYFRAMES];
Keyframe<float> legRightAnkle[MAX_KEYFRAMES];

Keyframe<float> legLeftHip_x[MAX_KEYFRAMES];
Keyframe<float> legLeftHip_y[MAX_KEYFRAMES];
Keyframe<float> legLeftHip_z[MAX_KEYFRAMES];
Keyframe<float> legLeftKnee[MAX_KEYFRAMES];
Keyframe<float> legLeftAnkle[MAX_KEYFRAMES];

Keyframe<float> bodyHipPosition_x[MAX_KEYFRAMES];
Keyframe<float> bodyHipPosition_y[MAX_KEYFRAMES];
Keyframe<float> bodyHipPosition_z[MAX_KEYFRAMES];
Keyframe<float> bodyHipEuler_x[MAX_KEYFRAMES];
Keyframe<float> bodyHipEuler_y[MAX_KEYFRAMES];
Keyframe<float> bodyHipEuler_z[MAX_KEYFRAMES];

// Temporary buffer
std::unordered_map<std::string, Keyframe<float>*> trackMap = {
    {"boardPos_x", boardPos_x},
    {"boardPos_y", boardPos_y},
    {"boardPos_z", boardPos_z},
    {"boardEuler_x", boardEuler_x},
    {"boardEuler_y", boardEuler_y},
    {"boardEuler_z", boardEuler_z},
    {"legRightHip_x", legRightHip_x},
    {"legRightHip_y", legRightHip_y},
    {"legRightHip_z", legRightHip_z},
    {"legRightKnee", legRightKnee},
    {"legRightAnkle", legRightAnkle},
    {"legLeftHip_x", legLeftHip_x},
    {"legLeftHip_y", legLeftHip_y},
    {"legLeftHip_z", legLeftHip_z},
    {"legLeftKnee", legLeftKnee},
    {"legLeftAnkle", legLeftAnkle},
    {"bodyHipPosition_x", bodyHipPosition_x},
    {"bodyHipPosition_y", bodyHipPosition_y},
    {"bodyHipPosition_z", bodyHipPosition_z},
    {"bodyHipEuler_x", bodyHipEuler_x},
    {"bodyHipEuler_y", bodyHipEuler_y},
    {"bodyHipEuler_z", bodyHipEuler_z},
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

