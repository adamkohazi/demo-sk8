// Copyright (c) 2025 Adam Kohazi (derangedlines)
// Licensed under the MIT License.

float i_WIDTH = 1920.;
float i_HEIGHT = 1080.;

const float i_PI = 3.14159;

/* Parameters */
const int i_RAYMARCH_MAXITER = 1024;
const float i_RAYMARCH_MINSTEP = 0.001;
const float i_RAYMARCH_MAXDIST = 64.0;

const float i_SHADOW_MAXDIST = 32.0;
const float i_SHADOW_HARDNESS = 16.0;

// Background
const vec3 i_SUNLIGHT_DIRECTION = normalize(vec3(5, 3, -10));
const vec3 i_SUN_DIRECTION = normalize(vec3(5, 1, -10));
const vec3 i_SUN_COLOR = vec3(0.9, 0.4, 0);

const vec3 i_SKY_COLOR = vec3(0.2, 0.5, 0.9);

const float i_CLOUD_FREQ = 2.0;
const vec3 i_CLOUD_COLOR = vec3(0.3, 0.4, 0.5);

// Material IDs
const int i_MAT_ID_WATER = 0;
const int i_MAT_ID_SAND = 1;
const int i_MAT_ID_CONCRETE = 2;
const int i_MAT_ID_TRUCK = 3;
const int i_MAT_ID_PALM_TRUNK = 4;
const int i_MAT_ID_PALM_LEAF = 5;
const int i_MAT_ID_DECK = 6;
const int i_MAT_ID_WHEEL = 7;
const int i_MAT_ID_LEG = 8;
const int i_MAT_ID_SHOE = 9;

// Objects
const vec3 i_BASE_COLOR[10] = vec3[](
    vec3(0.05, 0.1, 0.2),
    vec3(0.6, 0.5, 0.4),
    vec3(0.4),
    vec3(0.3),
    vec3(0.3, 0.2, 0.1),
    vec3(0.1, 0.2, 0.1),
    vec3(0.3, 0.9, 0.3),
    vec3(1),
    vec3(0.1, 0.2, 0.4),
    vec3(0.05)
);

// Lighing
const float i_SUN_BRIGHTNESS = 4.0;
const float i_SKY_BRIGHTNESS = 0.5;
const float i_BOUNCE_BRIGHTNESS = 0.1;

// Board
const vec3 i_BOARD_CENTER = vec3(0,0.05,0);

// Body
const float i_HIP_HEIGHT = 0.81;
const float i_HIP_WIDTH = 0.2;

const float i_THIGH_LENGTH = 0.4;
const float i_SHIN_LENGTH = 0.3;
const float i_FOOT_LENGTH = 0.1;

const float i_THIGH_WIDTH = 0.04;
const float i_SHIN_WIDTH = 0.04;
const float i_FOOT_WIDTH = 0.05;

// Scenery
const float i_PALM_HEIGHT = 5.0;
const float i_PALM_SIZE = 2.0;


// Camera angles
uniform vec3 camera;
uniform vec3 target;

// Board position
uniform vec3 board_euler;
uniform vec3 board_offset;

// Body position
uniform vec3 body_offset;
uniform float body_twist;

// Body parts
// Hip Internal/External rotation:
//  -1: Fully rotated internally
//   0: Straight
//   1: Fully rotated externally
uniform float hip_rotation_r;

// Hip Flexion/Extension:
//   0: Fully extended
//   1: Fully flexed
uniform float hip_flexion_r;

// Hip Abduction:
//   0: Straight
//   1: Fully abducted
uniform float hip_abduction_r;

// Knee Flexion:
//   0: Fully extended
//   1: Fully flexed
uniform float knee_flexion_r;

// Ankle Flexion/Extension:
//  -1: Fully extended
//   0: Straight
//   1: Fully flexed
uniform float ankle_flexion_r;

uniform float hip_rotation_l;
uniform float hip_flexion_l;
uniform float hip_abduction_l;
uniform float knee_flexion_l;
uniform float ankle_flexion_l;

// Scroll scenery
uniform float scroll;

// Convert to polar coords from cartesian coords
vec3 polarCoords(vec3 cartesian) {
    return vec3(
        length(cartesian), //r
        atan(cartesian.y, cartesian.x), //theta
        acos(cartesian.z / length(cartesian))); //pi
}


// Hash: 1 out, 2 in
// Copyright (c)2014 David Hoskins.
// https://www.shadertoy.com/view/4djSRW
float hash12(vec2 p) {
	vec3 p3  = fract(vec3(p.xyx) * .1031);
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.x + p3.y) * p3.z);
}

// 2D Value noise - by iq
// https://www.shadertoy.com/view/lsf3WH
float noise( in vec2 p ) {
    vec2 i = floor(p);
    vec2 f = fract(p);
	
    // cubic interpolant
    vec2 u = f*f*(3.0-2.0*f);

    return mix(mix(hash12(i + vec2(0.0,0.0)), 
                   hash12(i + vec2(1.0,0.0)), u.x),
               mix(hash12(i + vec2(0.0,1.0)), 
                   hash12(i + vec2(1.0,1.0)), u.x), u.y);
}

// Fractal brownian motion - multiple octaves of noise
float fbm(vec2 p) {
    return(
        0.5 * noise(p) +
        0.25 * noise(p*=2.02) +
        0.125 * noise(p*=2.02) +
        0.0625 * noise(p*=2.02)
    )/0.9375;
}

/* SDF primitives */
// Sphere
float SDFSphere(vec3 position, float radius) {
    return length(position)-radius;
}

// Box
float SDFBox(vec3 position, vec3 size) {
    vec3 q = abs(position) - 0.5 * size;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

// Capsule
float SDFCapsule(vec3 position, vec3 startPoint, vec3 endPoint, float radius) {
    vec3 pa = position - startPoint;
    vec3 ba = endPoint - startPoint;
    float h = clamp(dot(pa,ba)/dot(ba,ba), 2.*radius, 1.-2.*radius);
    return length(pa - ba*h) - radius;
}

// Cylinder
float SDFCylinder(vec3 position, float height, float radius) {
    vec2 d = abs(vec2(length(position.xz),position.y)) - vec2(radius,height);
    return min(max(d.x,d.y),0.0) + length(max(d,0.0));
}


/* SDF transformations */
// Rotate point around arbitrary axis
// https://suricrasia.online/blog/shader-functions/
vec3 rotateA(vec3 point, vec3 axis, float theta) {
    return mix(dot(axis, point)*axis, point, cos(theta)) + sin(theta)*cross(axis,point);
}

// Rotate point around main axis
vec3 i_rotateX(float theta, vec3 point) {
    return rotateA(point, vec3(1,0,0), theta);
}

vec3 i_rotateY(float theta, vec3 point) {
    return rotateA(point, vec3(0,1,0), theta);
}

vec3 i_rotateZ(float theta, vec3 point) {
    return rotateA(point, vec3(0,0,1), theta);
}

/* SDF operators */
float opSmoothUnion(float d1, float d2, float k) {
    float h = clamp( 0.5 + 0.5*(d2-d1)/k, 0.0, 1.0 );
    return mix( d2, d1, h ) - k*h*(1.0-h);
}


/* Scene Assets */
// Deck of a skateboard
float SDFDeck(vec3 position) {
    position.x -= clamp(position.x, -0.25, 0.25);
    position.x = abs(position.x);
    position = i_rotateZ(-0.3, position);
    
    // 2D SDF - Fully rounded rectangle
    float len = 0.08 / 2.0;
    float radius = 0.1;
    float d_2d = length(vec2(max(abs(position.x)-len, 0.0), position.z))-radius;
    float d = length(vec2(position.y-0.08, max(d_2d, 0.0)))-0.002;
    return d;
}

// Wheels of a skateboard
float SDFWheels(vec3 position) {
    position.xz = abs(position.xz);    
    return max(SDFCylinder(i_rotateX(-i_PI/2.0, position)-vec3(0.2, 0.09, -0.03),0.01,0.02)-0.01,
               -SDFCylinder(i_rotateX(-i_PI/2.0, position)-vec3(0.2, 0.00, -0.03),0.12,0.01));
}

// Trucks of a skateboard
float SDFTrucks(vec3 position) {
    position.x = abs(position.x);
    float distance = SDFCylinder(i_rotateX(-i_PI/2.0, position)-vec3(0.2, 0.00, -0.03),0.1,0.0)-0.01;
    distance = opSmoothUnion(distance, SDFCylinder(i_rotateZ(-0.3, position)-vec3(0.18, 0.00, 0.00), 0.003, 0.02), 0.05);
    distance = max(distance, -SDFCylinder(i_rotateZ(-0.3, position)-vec3(0.18, -0.11, 0.00), 0.1, 0.02));
    distance = min(distance, SDFCylinder(i_rotateZ(-0.3, position)-vec3(0.18, -0.0, 0.00), 0.02, 0.0)-0.01);
    distance = opSmoothUnion(distance, SDFCylinder(i_rotateZ(0.3, position)-vec3(0.2, 0.13, 0.00), 0.01, 0.0), 0.05);
    distance = min(distance, SDFBox(position-vec3(0.19, 0.075, 0.00),vec3(0.1, 0.01, 0.06)));
    return distance;
}


/* Main Scene */
// This function defines the scene by returning the distance of the nearest point from given
// position. Also sets the material ID for that point, via the "out" argument.
float map(in vec3 position, out int materialID) {
    vec3 moving = position-vec3(-scroll,0,0);
    
    // Water
    float distance = position.y + 1.0;
    materialID = i_MAT_ID_WATER;
    
    // Sand
    if(position.z > -20.0+3.0*sin(0.1*position.x))
        materialID = i_MAT_ID_SAND;
    
    // Platform
    if(position.z > -1.5) {
        vec3 tiled = moving - vec3(1,0,1)*round(moving/vec3(1,0,1));
        distance = SDFBox(tiled-vec3(0,-0.01,0), vec3(0.98,0,0.98))-0.01;
        
        materialID = i_MAT_ID_CONCRETE;
    }
    
    float material_distance = distance;
    
    // Skateboard
    vec3 board_position = position-i_BOARD_CENTER;
    
    board_position -= board_offset;
    board_position = i_rotateZ(board_euler.z, board_position);
    board_position = i_rotateY(board_euler.y, board_position); 
    board_position = i_rotateX(board_euler.x, board_position);
    
    // Deck
    distance = min(distance, SDFDeck(board_position + i_BOARD_CENTER));
    // Update material
    if (abs(distance-material_distance) > i_RAYMARCH_MINSTEP)
        materialID = i_MAT_ID_DECK;
    material_distance = distance;
    
    // Wheels
    distance = min(distance, SDFWheels(board_position + i_BOARD_CENTER));
    // Update material
    if (abs(distance-material_distance) > i_RAYMARCH_MINSTEP)
        materialID = i_MAT_ID_WHEEL;
    material_distance = distance;
    
    // Trucks
    distance = min(distance, SDFTrucks(board_position + i_BOARD_CENTER));
    // Update material
    if (abs(distance-material_distance) > i_RAYMARCH_MINSTEP)
        materialID = i_MAT_ID_TRUCK;
    material_distance = distance;

    // Relative move according to keyframe info
    vec3 body_position = position - body_offset;
    
    // Twist hip
    body_position = i_rotateY(2.0 * i_PI * (body_twist+0.5), body_position);
    
    vec3 leg_r_position = body_position-vec3(0.5*i_HIP_WIDTH, i_HIP_HEIGHT, 0);
    vec3 leg_l_position = body_position-vec3(-0.5*i_HIP_WIDTH, i_HIP_HEIGHT, 0);
    
    // 1. Hip rotation
    leg_r_position = i_rotateY(0.25 * i_PI * hip_rotation_r, leg_r_position);
    leg_l_position = i_rotateY(-0.25 * i_PI * hip_rotation_l, leg_l_position);
    
    //Calculate joint positions
    // Knee
    vec3 knee_right_point = i_rotateX(140.0 / 360.0 * 2.0 * i_PI * hip_flexion_r,-vec3(0,i_THIGH_LENGTH,0)); // 2. Flexion
    knee_right_point = i_rotateZ(0.25 * i_PI * hip_abduction_r, knee_right_point); // 3. Abduction
    
    vec3 knee_left_point = i_rotateX(140.0 / 360.0 * 2.0 * i_PI * hip_flexion_l,-vec3(0,i_THIGH_LENGTH,0)); // 2. Flexion
    knee_left_point = i_rotateZ(-0.25 * i_PI * hip_abduction_l, knee_left_point); // 3. Abduction
 
    // Ankle
    vec3 ankle_right_point = knee_right_point + i_rotateX(140.0 / 360.0 * 2.0 * i_PI * hip_flexion_r - 140.0 / 360.0 * 2.0 * i_PI * knee_flexion_r,-vec3(0,i_SHIN_LENGTH,0));
    ankle_right_point = i_rotateZ(0.25 * i_PI * hip_abduction_r, ankle_right_point - knee_right_point) + knee_right_point; //flexion
    
    vec3 ankle_left_point = knee_left_point + i_rotateX(140.0 / 360.0 * 2.0 * i_PI * hip_flexion_l - 140.0 / 360.0 * 2.0 * i_PI * knee_flexion_l,-vec3(0,i_SHIN_LENGTH,0));
    ankle_left_point = i_rotateZ(-0.25 * i_PI * hip_abduction_l, ankle_left_point - knee_left_point) + knee_left_point; //flexion
    
    // Toe
    vec3 toe_right_point = ankle_right_point + i_rotateX(140.0 / 360.0 * 2.0 * i_PI * hip_flexion_r - 140.0 / 360.0 * 2.0 * i_PI * knee_flexion_r + 0.25 * i_PI * ankle_flexion_r +  0.5 * i_PI,-vec3(0,i_FOOT_LENGTH,0));
    toe_right_point = i_rotateZ(0.25 * i_PI * hip_abduction_r, toe_right_point - ankle_right_point) + ankle_right_point; //flexion
    
    vec3 toe_left_point = ankle_left_point + i_rotateX(140.0 / 360.0 * 2.0 * i_PI * hip_flexion_l - 140.0 / 360.0 * 2.0 * i_PI * knee_flexion_l + 0.25 * i_PI * ankle_flexion_l +  0.5 * i_PI,-vec3(0,i_FOOT_LENGTH,0));
    toe_left_point = i_rotateZ(-0.25 * i_PI * hip_abduction_l, toe_left_point - ankle_left_point) + ankle_left_point; //flexion
    
    // Draw thighs
    float leg_right = SDFCapsule(leg_r_position, vec3(0), knee_right_point, i_THIGH_WIDTH);
    float leg_left = SDFCapsule(leg_l_position, vec3(0), knee_left_point, i_THIGH_WIDTH);
    
    // Draw shins
    leg_right = opSmoothUnion(leg_right, SDFCapsule(leg_r_position, knee_right_point, ankle_right_point, i_SHIN_WIDTH), i_SHIN_WIDTH); 
    leg_left = opSmoothUnion(leg_left, SDFCapsule(leg_l_position, knee_left_point, ankle_left_point, i_SHIN_WIDTH), i_SHIN_WIDTH);
    
    // Add wrinkles
    leg_right -= 0.04 * fbm(5.0 * leg_r_position.yz);
    leg_left -= 0.04 * fbm(5.0 * leg_l_position.yz);
    
    // Cut to length
    leg_right = max(leg_right, dot(leg_r_position - ankle_right_point, normalize(ankle_right_point - knee_right_point)) + 0.05); 
    leg_left  = max(leg_left , dot(leg_l_position - ankle_left_point , normalize(ankle_left_point  - knee_left_point )) + 0.05);
    
    // Combine
    distance = min(distance, leg_right);
    distance = min(distance, leg_left);
    
    // Update material
    if (abs(distance-material_distance) > i_RAYMARCH_MINSTEP)
        materialID = i_MAT_ID_LEG;
    material_distance = distance;
    
    
    // Shoe
    distance = min(distance, max(
        SDFCapsule(leg_r_position, 1.1*ankle_right_point-0.1*toe_right_point, toe_right_point, i_FOOT_WIDTH), // extend heel back a bit
        -dot(
            leg_r_position-ankle_right_point,
            normalize(cross(cross(ankle_right_point-knee_right_point, toe_right_point-ankle_right_point), toe_right_point-ankle_right_point)) // Cut sole flat
            )
        )-0.01
    );
    
    distance = min(distance, max(
        SDFCapsule(leg_l_position, 1.1*ankle_left_point-0.1*toe_left_point, toe_left_point, i_FOOT_WIDTH), // extend heel back a bit
        -dot(
            leg_l_position-ankle_left_point,
            normalize(cross(cross(ankle_left_point-knee_left_point, toe_left_point-ankle_left_point), toe_left_point-ankle_left_point)) // Cut sole flat
            )
        )-0.01
    );
    
    // Update material
    if (abs(distance-material_distance) > i_RAYMARCH_MINSTEP)
        materialID = i_MAT_ID_SHOE;
    material_distance = distance;
    
    // Repeating boardwalk
    position = moving;
    position.x -= 20.0*round((position.x)/20.0);
    
    // Ledges
    distance = min(distance, SDFBox(position-vec3(0,0,-1.5), vec3(3, 1, 0.5)));
    
    // Update material
    if (abs(distance-material_distance) > i_RAYMARCH_MINSTEP)
        materialID = i_MAT_ID_CONCRETE;
    material_distance = distance;

    // Palm tree - trunk
    vec3 location = position-vec3(0,-1,-12);
    distance = min(distance, length(location+vec3(sin(2.0*location.y/i_PALM_HEIGHT), -clamp(location.y, 0.0, i_PALM_HEIGHT), 0)) - 0.2);
    
    // Update material
    if (abs(distance-material_distance) > i_RAYMARCH_MINSTEP)
        materialID = i_MAT_ID_PALM_TRUNK;
    material_distance = distance;
    
    // Palm tree - leaves
    // Make leaves curve downward and position at top of trunk
    location.x += 1.0;
    location.y -= i_PALM_HEIGHT+sin(length(location.xz));
    
    vec3 polar = polarCoords(location);
    float leaves = polar.x-max(0.1,i_PALM_SIZE*cos(5.0*polar.y)*cos(5.0*polar.z))-0.5;
    
    distance = min(distance, 0.1*leaves);
    
    // Update material
    if (abs(distance-material_distance) > i_RAYMARCH_MINSTEP)
        materialID = i_MAT_ID_PALM_LEAF;
    
    // Combine SDFs:
    return distance;
}

vec3 drawBackground(vec3 direction) {
    // Sky gradient
    float gradient = 0.5 * pow(1.0-direction.y, 4.0);
    vec3 color = mix(i_SKY_COLOR, i_SUN_COLOR, gradient);
    
    // Sun
    float sunAngle = max(dot(direction, i_SUN_DIRECTION), 0.0);
    color += 0.5 * i_SUN_COLOR * pow(sunAngle, 50.0);
    color += 5.0 * i_SUN_COLOR * pow(sunAngle, 1000.0);
    
    // Clouds
    float clouds = fbm((i_CLOUD_FREQ/direction.y)*direction.xz);
    vec3 cloudColor = mix(i_CLOUD_COLOR, 3.0*i_SUN_COLOR, pow(sunAngle, 3.0));
    color = mix(color, cloudColor, 0.5*smoothstep(0.4, 1.0, clouds));
    
    // Mountains
    if (direction.y < 0.3 * fbm(5.0*direction.xz) - 0.1)
        color = mix(color, vec3(0.3), 0.5+5.0*direction.y);
    
    return color;
}

/* Ray marching */
// Follow a ray of light from camera origin until collision to find distance
// and material. Material ID is passed along using the "out" argument.
float castRay(in vec3 rayOrigin, vec3 rayDirection, out int materialID) {
    // Start from origin
    float distance = 0.0;
    for(int i=0; i<i_RAYMARCH_MAXITER; i++) {
        // Find stepsize for marching (distance of closest point from current position)
        float step = map(rayOrigin + distance * rayDirection, materialID);

        // An object is hit
        if(step < i_RAYMARCH_MINSTEP)
            return distance;
            
        // March!
        distance += step;

        // If ray doesn't hit any objects and maximum render distance is reached,
        // draw the background
        if(length(rayOrigin + distance * rayDirection) > i_RAYMARCH_MAXDIST)
            break;
    }
    // Return a negative distance, so background is not affected by lighting.
    return -1.0;
}

int dummy; // Dummy material variable

// Calculate surface normal - by iq
// https://iquilezles.org/articles/normalsSDF/
vec3 calculateNormal(vec3 p) {
    vec3 v1 = vec3(
        map(p + vec3(i_RAYMARCH_MINSTEP, 0, 0), dummy),
        map(p + vec3(0, i_RAYMARCH_MINSTEP, 0), dummy),
        map(p + vec3(0, 0, i_RAYMARCH_MINSTEP), dummy));
    vec3 v2 = vec3(
        map(p - vec3(i_RAYMARCH_MINSTEP, 0, 0), dummy),
        map(p - vec3(0, i_RAYMARCH_MINSTEP, 0), dummy),
        map(p - vec3(0, 0, i_RAYMARCH_MINSTEP), dummy));

    return normalize(v1 - v2);
}

// Calculate soft shadows
// https://iquilezles.org/articles/rmshadows/
float softShadow(in vec3 rayOrigin, vec3 rayDirection) {
    float distance, step, shadow = 1.0;
    for(distance = 0.0; distance < i_SHADOW_MAXDIST; distance += step) {
        step = map(rayOrigin + distance * rayDirection, dummy);
        if(step < i_RAYMARCH_MINSTEP)
            return 0.0;
        shadow = min(shadow, i_SHADOW_HARDNESS * step/distance);
    }
    return shadow;
}

void addBumps(vec3 position, inout vec3 normal, float size, float strength) {
    // Sample the reference shape value at the current position
    float baseSample = fbm(size * position.xz);

    // Compute gradient by sampling noise in horizontal directions
    vec3 gradient = strength * vec3(
        fbm(size * vec2(position.x + 0.001, position.z)) - baseSample,
        0,
        fbm(size * vec2(position.x, position.z + 0.001)) - baseSample
    ) / 0.001;

    // Displace normal by the gradient)
    normal = normalize(normal - gradient);
}


// Entry Point
void main() {
    // Pixel coordinates (from -1 to 1)
    vec2 uv = (2.0*floor(gl_FragCoord)-vec2(i_WIDTH, i_HEIGHT))/i_WIDTH;
    
    // Fisheye
    uv *= (1.0 + 0.5 * pow(0.5 * length(uv), 2.0));
    
    // Camera lens
    float zoom = 1.5;
    
    // Calculating ray angles
    vec3 ww = normalize(vec3(target - camera));
    vec3 uu = normalize(cross(ww, vec3(0,1,0)));
    vec3 vv = normalize(cross(uu, ww));
    vec3 rayDirection = normalize(ww*zoom + uv.x*uu + uv.y*vv);
    
    // Render Scene (single render pass)
    // Perform ray marhcing:
    // This finds the material and distance for the current pixel.
    int materialID;
    float distance = castRay(camera, rayDirection, materialID);
    
    // This will be the final color of pixel
    vec3 color;
    
    // Ray hit
    if(distance > 0.0) {
        // Surface parameters
        vec3 position = camera + distance * rayDirection;
        vec3 normal = calculateNormal(position);
        
        // Object base color
        color = i_BASE_COLOR[materialID];
        
        // Material
        // Water
        if(materialID == i_MAT_ID_WATER) {
            // Reflectivity (Fresnel)
            float reflectivity = pow(1.0 - max(dot(normal, -rayDirection), 0.0), 5.0); //5.0 is magic number
            
            // Displacement vector from noise (simulating waves)
            addBumps(position - vec3(-scroll,0,scroll), normal, 0.1, 0.5);
            
            // Reflect sky and background
            vec3 reflection = drawBackground(reflect(rayDirection, normal));
            
            // If not reflected, show water color
            color += reflection * reflectivity;
            
            // Add specular reflection for sun
            color += i_SUN_COLOR * pow(max(dot(rayDirection, i_SUN_DIRECTION), 0.0), 5.0); 
        }
        // Lights don't affect water
        else {
            // Special surfaces
            // Sand
            if (materialID == i_MAT_ID_SAND) {
                addBumps(position-vec3(-scroll,0,0), normal, 1.0, 0.1);
            }
            
            // Concrete
            if (materialID == i_MAT_ID_CONCRETE) {
                color += 0.2*fbm(5.0*(position.xy)-vec2(-scroll,0));
                addBumps(position-vec3(-scroll,0,0), normal, 10.0, 0.005);
            }
            
            // Soft shadows
            float shadow = softShadow(position + i_RAYMARCH_MINSTEP * normal, i_SUNLIGHT_DIRECTION);

            // Direct light from Sun
            vec3 sunLight = i_SUN_BRIGHTNESS * i_SUN_COLOR * max(dot(normal, i_SUNLIGHT_DIRECTION), 0.0);

            // Diffused light from Sky
            vec3 skyLight = i_SKY_BRIGHTNESS * i_SKY_COLOR * (0.5+0.5*normal.y);
            
            // Bounce light from ground
            vec3 bounceLight = i_BOUNCE_BRIGHTNESS * i_SUN_COLOR * (0.5-0.5*normal.y);
            
            // Specular using Blinn-Phong
            vec3 half1 = normalize(i_SUN_DIRECTION - rayDirection);
            vec3 specular = i_SUN_BRIGHTNESS * i_SUN_COLOR * pow(max(dot(half1, normal), 0.0), 16.0) //16.0 sets radius
                // Fresnel effect
                * pow(1.0 - max(dot(half1, i_SUNLIGHT_DIRECTION), 0.0), 5.0); //5.0 is magic number

            // Combine lights and shadows
            color *= (shadow*sunLight + skyLight + bounceLight);
        
            // Shiny surfaces
            if(materialID == i_MAT_ID_CONCRETE)
                color += color * specular * shadow;
            
            if(materialID == i_MAT_ID_TRUCK)
                color += 10.0 * specular * shadow;
        }
    }
    
    // If nothing was hit - background
    else
        color = drawBackground(rayDirection);
    
    // Gamma correction
    color = pow(color, vec3(0.45));

    // Output to screen
    gl_FragColor = vec4(color, 1.0);
}