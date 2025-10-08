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
const BodyPosition body_ready = BodyPosition(vec3(0.15, -0.0, 0.1), 0.0, 0.0, 0.05, -0.05, 0.4, 0.4, 0.0, 0.0, -1.7, -HALF_PI);
const BodyPosition body_pop = BodyPosition(vec3(0.2, 0.1, 0.05), 0.0, -0.5, 0.2, -0.05, 0.0, 0.8, 0.0, 0.3, -0.2, -2.3);
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


template<typename ValueType>
void loadKeyframesFromJSON(const std::string& filename) {
    // Open file
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    // Parse JSON
    json j;
    file >> j;

    // Map from track name to fixed-length array of keyframes
    std::unordered_map<std::string, Keyframe<ValueType>[MAX_KEYFRAMES]> outTracks;

    // Initialize all arrays with sentinel keyframes (time = -1 means invalid)
    for (auto& [track, arr] : outTracks) {
        for (size_t i = 0; i < MAX_KEYFRAMES; ++i) {
            arr[i] = Keyframe<ValueType>{ -1.0f, ValueType{}, Interpolation::LINEAR };
        }
    }

    // We'll keep a temporary insertion index map per track here
    std::unordered_map<std::string, size_t> insertIndices;

    // Iterate over JSON and fill arrays
    for (const auto& frame : j["keyframes"]) {
        float time = frame["time"];

        for (const auto& node : frame["nodes"]) {
            std::string track = node["track"];
            ValueType value = node["value"];
            Interpolation mode = static_cast<Interpolation>(node["mode"].get<int>());

            size_t& index = insertIndices[track];
            if (index < MAX_KEYFRAMES) {
                // Insert keyframe
                outTracks[track][index++] = Keyframe<ValueType>{time, value, mode};
            }
            // else: ignore extra keyframes beyond max size
        }
    }

    // Update individual tracks
    memcpy(boardPos_x, outTracks["boardPos_x"], sizeof(boardPos_x));
    memcpy(boardPos_y, outTracks["boardPos_y"], sizeof(boardPos_y));
    memcpy(boardPos_z, outTracks["boardPos_z"], sizeof(boardPos_z));

    memcpy(boardEuler_x, outTracks["boardEuler_x"], sizeof(boardEuler_x));
    memcpy(boardEuler_y, outTracks["boardEuler_y"], sizeof(boardEuler_y));
    memcpy(boardEuler_z, outTracks["boardEuler_z"], sizeof(boardEuler_z));

    memcpy(legRightHip_x, outTracks["legRightHip_x"], sizeof(legRightHip_x));
    memcpy(legRightHip_y, outTracks["legRightHip_y"], sizeof(legRightHip_y));
    memcpy(legRightHip_z, outTracks["legRightHip_z"], sizeof(legRightHip_z));
    memcpy(legRightKnee, outTracks["legRightKnee"], sizeof(legRightKnee));
    memcpy(legRightAnkle, outTracks["legRightAnkle"], sizeof(legRightAnkle));

    memcpy(legLeftHip_x, outTracks["legLeftHip_x"], sizeof(legLeftHip_x));
    memcpy(legLeftHip_y, outTracks["legLeftHip_y"], sizeof(legLeftHip_y));
    memcpy(legLeftHip_z, outTracks["legLeftHip_z"], sizeof(legLeftHip_z));
    memcpy(legLeftKnee, outTracks["legLeftKnee"], sizeof(legLeftKnee));
    memcpy(legLeftAnkle, outTracks["legLeftAnkle"], sizeof(legLeftAnkle));

    memcpy(bodyHipPosition_x, outTracks["bodyHipPosition_x"], sizeof(bodyHipPosition_x));
    memcpy(bodyHipPosition_y, outTracks["bodyHipPosition_y"], sizeof(bodyHipPosition_y));
    memcpy(bodyHipPosition_z, outTracks["bodyHipPosition_z"], sizeof(bodyHipPosition_z));
    memcpy(bodyHipEuler_x, outTracks["bodyHipEuler_x"], sizeof(bodyHipEuler_x));
    memcpy(bodyHipEuler_y, outTracks["bodyHipEuler_y"], sizeof(bodyHipEuler_y));
    memcpy(bodyHipEuler_z, outTracks["bodyHipEuler_z"], sizeof(bodyHipEuler_z));
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

