#define PI 3.14159

/* Parameters */
#define RAYMARCH_MAXITER 1024
#define RAYMARCH_MINSTEP 0.001
#define RAYMARCH_MAXDIST 64.0

#define SHADOW_MAXDIST 32.0
#define SHADOW_HARDNESS 16.0

#define OCCLUSION_STEPS 5

// Background
#define SUNLIGHT_DIRECTION normalize(vec3(5, 3, -10))
#define SUN_DIRECTION normalize(vec3(5, 1, -10))
#define SUN_COLOR normalize(vec3(8, 4, 0))

#define SKY_COLOR normalize(vec3(1, 3, 5))
#define AMBIENT_COLOR normalize(vec3(2, 1, 0))

#define CLOUD_FREQ 2.0
#define CLOUD_COLOR vec3(0.3, 0.4, 0.5)

// Material IDs
#define MAT_ID_DEFAULT 0
#define MAT_ID_WATER 1
#define MAT_ID_SAND 2
#define MAT_ID_CONCRETE 3
#define MAT_ID_PALM_TRUNK 4
#define MAT_ID_PALM_LEAF 5
#define MAT_ID_DECK 6
#define MAT_ID_WHEEL 7
#define MAT_ID_TRUCK 8
#define MAT_ID_LEG 9
#define MAT_ID_SHOE 10

// Objects
#define WATER_COLOR vec3(0.05, 0.1, 0.2)
#define SAND_COLOR vec3(0.5)
#define CONCRETE_LIGHT vec3(0.6)
#define CONCRETE_DARK vec3(0.4)
#define PALM_TRUNK vec3(0.3, 0.2, 0.1)
#define PALM_LEAF vec3(0.1, 0.2, 0.1)
#define COLOR_DECK vec3(1, 0, 0)
#define COLOR_WHEEL vec3(1)
#define COLOR_TRUCK vec3(0.3)
#define COLOR_LEG vec3(0.1,0.2,0.4)
#define COLOR_SHOE vec3(0.05)

// Lighing
#define SUN_BRIGHTNESS 4.0
#define SKY_BRIGHTNESS 0.5
#define BOUNCE_BRIGHTNESS 0.2

uniform float scroll;

uniform vec3 camera;
uniform vec3 target;

uniform vec3 board_euler;
uniform vec3 board_offset;

uniform float body_twist;
uniform vec3 body_offset;

uniform float hip_rotation_r;
uniform float hip_flexion_r;
uniform float hip_abduction_r;
uniform float knee_flexion_r;
uniform float ankle_flexion_r;

uniform float hip_rotation_l;
uniform float hip_flexion_l;
uniform float hip_abduction_l;
uniform float knee_flexion_l;
uniform float ankle_flexion_l;

/* Generic functions */
// Convert to polar coords from cartesian coords
vec3 polarCoords(vec3 cartesian) {
    return vec3(
        length(cartesian), //r
        atan(cartesian.y, cartesian.x), //theta
        acos(cartesian.z / length(cartesian))); //pi
}


/* Hash and Noise */
// Hash: 1 out, 2 in
// Copyright (c)2014 David Hoskins.
// https://www.shadertoy.com/view/4djSRW
float hash12(vec2 p)
{
	vec3 p3  = fract(vec3(p.xyx) * .1031);
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.x + p3.y) * p3.z);
}

// 2D Value noise by iq:
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
vec3 rotateX(float theta, vec3 point) {
    return rotateA(point, vec3(1,0,0), theta);
}

vec3 rotateY(float theta, vec3 point) {
    return rotateA(point, vec3(0,1,0), theta);
}

vec3 rotateZ(float theta, vec3 point) {
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
    position = rotateZ(-0.3, position);
    
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
    return max(SDFCylinder(rotateX(-PI/2.0, position)-vec3(0.2, 0.09, -0.03),0.01,0.02)-0.01,
               -SDFCylinder(rotateX(-PI/2.0, position)-vec3(0.2, 0.00, -0.03),0.12,0.01));
}

// Trucks of a skateboard
float SDFTrucks(vec3 position) {
    position.x = abs(position.x);
    float distance = SDFCylinder(rotateX(-PI/2.0, position)-vec3(0.2, 0.00, -0.03),0.1,0.0)-0.01;
    distance = opSmoothUnion(distance, SDFCylinder(rotateZ(-0.3, position)-vec3(0.18, 0.00, 0.00), 0.003, 0.02), 0.05);
    distance = max(distance, -SDFCylinder(rotateZ(-0.3, position)-vec3(0.18, -0.11, 0.00), 0.1, 0.02));
    distance = min(distance, SDFCylinder(rotateZ(-0.3, position)-vec3(0.18, -0.0, 0.00), 0.02, 0.0)-0.01);
    distance = opSmoothUnion(distance, SDFCylinder(rotateZ(0.3, position)-vec3(0.2, 0.13, 0.00), 0.01, 0.0), 0.05);
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
    materialID = MAT_ID_WATER;
    
    // Sand
    if(position.z > -20.0+3.0*sin(0.1*position.x))
        materialID = MAT_ID_SAND;
    
    // Platform
    if(position.z > -2.0) {
        vec3 tiled = moving - vec3(1,0,1)*round(moving/vec3(1,0,1));
        distance = SDFBox(tiled-vec3(0,-0.01,0), vec3(0.98,0,0.98))-0.01;
        
        materialID = MAT_ID_CONCRETE;
    }
    
    float material_distance = distance;
    
    // Skateboard
    const vec3 centerpoint = vec3(0,0.05,0);
    vec3 board_position = position-centerpoint;
    
    board_position -= board_offset;
    board_position = rotateZ(board_euler.z, board_position);
    board_position = rotateY(board_euler.y, board_position); 
    board_position = rotateX(board_euler.x, board_position);
    
    // Deck
    distance = min(distance, SDFDeck(board_position + centerpoint));
    // Update material
    if (abs(distance-material_distance) > RAYMARCH_MINSTEP)
        materialID = MAT_ID_DECK;
    material_distance = distance;
    
    // Wheels
    distance = min(distance, SDFWheels(board_position + centerpoint));
    // Update material
    if (abs(distance-material_distance) > RAYMARCH_MINSTEP)
        materialID = MAT_ID_WHEEL;
    material_distance = distance;
    
    // Trucks
    distance = min(distance, SDFTrucks(board_position + centerpoint));
    // Update material
    if (abs(distance-material_distance) > RAYMARCH_MINSTEP)
        materialID = MAT_ID_TRUCK;
    material_distance = distance;

    // Relative move according to keyframe info
    const float hipHeight = 0.81;
    const float hipWidth = 0.2;
    
    const float thighLen = 0.4;
    const float shinLen = 0.3;
    const float footLen = 0.1;
    
    const float thighWidth = 0.04;
    const float shinWidth = 0.04;
    const float footWidth = 0.05;
    vec3 body_position = position - body_offset;
    
    // Twist hip
    body_position = rotateY(2.0 * PI * (body_twist+0.5), body_position);
    
    vec3 leg_r_position = body_position-vec3(0.5*hipWidth, hipHeight, 0);
    vec3 leg_l_position = body_position-vec3(-0.5*hipWidth, hipHeight, 0);
    
    // 1. Hip rotation
    leg_r_position = rotateY(0.25 * PI * hip_rotation_r, leg_r_position);
    leg_l_position = rotateY(-0.25 * PI * hip_rotation_l, leg_l_position);
    
    //Calculate joint positions
    // Knee
    vec3 knee_right_point = rotateX(140.0 / 360.0 * 2.0 * PI * hip_flexion_r,-vec3(0,thighLen,0)); // 2. Flexion
    knee_right_point = rotateZ(0.25 * PI * hip_abduction_r, knee_right_point); // 3. Abduction
    
    vec3 knee_left_point = rotateX(140.0 / 360.0 * 2.0 * PI * hip_flexion_l,-vec3(0,thighLen,0)); // 2. Flexion
    knee_left_point = rotateZ(-0.25 * PI * hip_abduction_l, knee_left_point); // 3. Abduction
 
    // Ankle
    vec3 ankle_right_point = knee_right_point + rotateX(140.0 / 360.0 * 2.0 * PI * hip_flexion_r - 140.0 / 360.0 * 2.0 * PI * knee_flexion_r,-vec3(0,shinLen,0));
    ankle_right_point = rotateZ(0.25 * PI * hip_abduction_r, ankle_right_point - knee_right_point) + knee_right_point; //flexion
    
    vec3 ankle_left_point = knee_left_point + rotateX(140.0 / 360.0 * 2.0 * PI * hip_flexion_l - 140.0 / 360.0 * 2.0 * PI * knee_flexion_l,-vec3(0,shinLen,0));
    ankle_left_point = rotateZ(-0.25 * PI * hip_abduction_l, ankle_left_point - knee_left_point) + knee_left_point; //flexion
    
    // Toe
    vec3 toe_right_point = ankle_right_point + rotateX(140.0 / 360.0 * 2.0 * PI * hip_flexion_r - 140.0 / 360.0 * 2.0 * PI * knee_flexion_r + 0.25 * PI * ankle_flexion_r +  0.5 * PI,-vec3(0,footLen,0));
    toe_right_point = rotateZ(0.25 * PI * hip_abduction_r, toe_right_point - ankle_right_point) + ankle_right_point; //flexion
    
    vec3 toe_left_point = ankle_left_point + rotateX(140.0 / 360.0 * 2.0 * PI * hip_flexion_l - 140.0 / 360.0 * 2.0 * PI * knee_flexion_l + 0.25 * PI * ankle_flexion_l +  0.5 * PI,-vec3(0,footLen,0));
    toe_left_point = rotateZ(-0.25 * PI * hip_abduction_l, toe_left_point - ankle_left_point) + ankle_left_point; //flexion
    
    // Draw thighs
    float leg_right = SDFCapsule(leg_r_position, vec3(0), knee_right_point, thighWidth);
    float leg_left = SDFCapsule(leg_l_position, vec3(0), knee_left_point, thighWidth);
    
    // Draw shins
    leg_right = opSmoothUnion(leg_right, SDFCapsule(leg_r_position, knee_right_point, ankle_right_point, shinWidth), shinWidth); 
    leg_left = opSmoothUnion(leg_left, SDFCapsule(leg_l_position, knee_left_point, ankle_left_point, shinWidth), shinWidth);
    
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
    if (abs(distance-material_distance) > RAYMARCH_MINSTEP)
        materialID = MAT_ID_LEG;
    material_distance = distance;
    
    
    // Shoe
    distance = min(distance, max(
        SDFCapsule(leg_r_position, 1.1*ankle_right_point-0.1*toe_right_point, toe_right_point, footWidth), // extend heel back a bit
        -dot(
            leg_r_position-ankle_right_point,
            normalize(cross(cross(ankle_right_point-knee_right_point, toe_right_point-ankle_right_point), toe_right_point-ankle_right_point)) // Cut sole flat
            )
        )-0.01
    );
    
    distance = min(distance, max(
        SDFCapsule(leg_l_position, 1.1*ankle_left_point-0.1*toe_left_point, toe_left_point, footWidth), // extend heel back a bit
        -dot(
            leg_l_position-ankle_left_point,
            normalize(cross(cross(ankle_left_point-knee_left_point, toe_left_point-ankle_left_point), toe_left_point-ankle_left_point)) // Cut sole flat
            )
        )-0.01
    );
    
    // Update material
    if (abs(distance-material_distance) > RAYMARCH_MINSTEP)
        materialID = MAT_ID_SHOE;
    material_distance = distance;
    
    // Repeating boardwalk
    position = moving;
    position.x -= 20.0*round((position.x)/20.0);
    
    // Ledges
    distance = min(distance, SDFBox(position-vec3(0,0,-1.5), vec3(3, 1, 0.5)));
    
    // Update material
    if (abs(distance-material_distance) > RAYMARCH_MINSTEP)
        materialID = MAT_ID_CONCRETE;
    material_distance = distance;

    // Palm tree - trunk
    vec3 location = position-vec3(0,-1,-12);
    distance = min(distance, length(location+vec3(sin(2.0*location.y/15.0), -clamp(location.y, 0.0, 15.0), 0)) - 0.3);
    
    // Update material
    if (abs(distance-material_distance) > RAYMARCH_MINSTEP)
        materialID = MAT_ID_PALM_TRUNK;
    material_distance = distance;
    
    // Palm tree - leaves
    //Make leaves curve downward and position at top of trunk
    location.y -= 15.0+sin(length(location.xz));
    
    vec3 polar = polarCoords(location);
    float leaves = polar.x-max(0.1,4.0*cos(5.0*polar.y)*cos(5.0*polar.z))-0.5; //Leaves
    
    distance = min(distance, 0.1*leaves); // Smaller stepsize due to inaccuracies
    
    // Update material
    if (abs(distance-material_distance) > RAYMARCH_MINSTEP)
        materialID = MAT_ID_PALM_LEAF;
    material_distance = distance;
    
    // Combine SDFs:
    return distance;
}

vec3 drawBackground(vec3 direction) {
    // Sky gradient
    float gradient = 0.5 * pow(1.0-direction.y, 4.0);
    vec3 color = mix(SKY_COLOR, AMBIENT_COLOR, gradient);
    
    // Sun
    float sunAngle = clamp(dot(direction, SUN_DIRECTION), 0.0, 1.0);
    color += 0.5 * SUN_COLOR * pow(sunAngle, 50.0);
    color += 5.0 * SUN_COLOR * pow(sunAngle, 1000.0);
    

    // Clouds
    if (direction.y > 0.0) {
        float clouds = fbm((CLOUD_FREQ/direction.y)*direction.xz);
        vec3 cloudColor = mix(CLOUD_COLOR, 3.0*SUN_COLOR, pow(sunAngle, 3.0));
        color = mix(color, cloudColor, 0.5*smoothstep(0.4, 1.0, clouds));
    }
    
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
    for(int i=0; i<RAYMARCH_MAXITER; i++) {
        // Find stepsize for marching (distance of closest point from current position)
        float step = map(rayOrigin + distance * rayDirection, materialID);

        // An object is hit
        if(step < RAYMARCH_MINSTEP)
            return distance;
            
        // March!
        distance += step;

        // If ray doesn't hit any objects and maximum render distance is reached,
        // draw the background
        if(length(rayOrigin + distance * rayDirection) > RAYMARCH_MAXDIST)
            break;
    }
    // Return a negative distance, so background is not affected by lighting.
    return -1.0;
}

/* Calculations for rendering */
// Calculate surface normal
// https://iquilezles.org/articles/normalsSDF/
vec3 calculateNormal(vec3 p) {
    int dummy;
    vec3 v1 = vec3(
        map(p + vec3(RAYMARCH_MINSTEP, 0, 0), dummy),
        map(p + vec3(0, RAYMARCH_MINSTEP, 0), dummy),
        map(p + vec3(0, 0, RAYMARCH_MINSTEP), dummy));
    vec3 v2 = vec3(
        map(p - vec3(RAYMARCH_MINSTEP, 0, 0), dummy),
        map(p - vec3(0, RAYMARCH_MINSTEP, 0), dummy),
        map(p - vec3(0, 0, RAYMARCH_MINSTEP), dummy));

    return normalize(v1 - v2);
}

// Calculate soft shadows
// Thanks iq!
// https://iquilezles.org/articles/rmshadows/
float softShadow(in vec3 rayOrigin, vec3 rayDirection)
{
    float res = 1.0;
    float distance = 0.0;   // Min distance
    int dummy;
    while (distance < SHADOW_MAXDIST) // Max distance
    {
        float step = map(rayOrigin + distance * rayDirection, dummy);
        if(step < RAYMARCH_MINSTEP)
            return 0.0;
        res = min(res, SHADOW_HARDNESS * step/distance);
        distance += step;
    }
    return res;
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


/* Entry Point */
void main(void) {
    // Pixel coordinates (from -1 to 1)
    vec2 uv = (2.0*floor(gl_FragCoord)-vec2(1280, 720))/720.;
    
    // Fisheye
    uv *= (1.0 + 0.5 * pow(0.5 * length(uv), 2.0));
    
    // Camera lens
    float zoom = 2.0;
    
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
        
        // Material
        // Water
        if(materialID == MAT_ID_WATER) {
            // Reflectivity (Fresnel)
            float reflectivity = pow(1.0 - clamp(dot(normal, -rayDirection), 0.0, 1.0), 5.0); //5.0 is magic number
            
            // Displacement vector from noise (simulating waves)
            addBumps(position - vec3(-scroll,0,scroll), normal, 0.1, 0.5);
            
            // Reflect sky and background
            vec3 reflection = drawBackground(reflect(rayDirection, normal));
            
            // If not reflected, show water color
            color = mix(WATER_COLOR, reflection, reflectivity);
            
            // Add specular reflection for sun
            color += SUN_COLOR * pow(clamp(dot(rayDirection, SUN_DIRECTION), 0.0, 1.0), 5.0); 
        }
        // Lights don't affect water
        else {
            // Sand
            if (materialID == MAT_ID_SAND) {
                color = SAND_COLOR;
                addBumps(position, normal, 1.0, 0.1);
            }
            
            // Concrete
            if (materialID == MAT_ID_CONCRETE) {
                color = mix(CONCRETE_DARK, CONCRETE_LIGHT, fbm(5.0*(position.xy)-vec2(-scroll,0)));
                addBumps(position-vec3(-scroll,0,0), normal, 10.0, 0.005);
            }
            
            // Palm trunk
            if (materialID == MAT_ID_PALM_TRUNK)
                color = PALM_TRUNK;
            
            // Palm leaf
            if (materialID == MAT_ID_PALM_LEAF)
                color = PALM_LEAF;
            
            // Deck
            if (materialID == MAT_ID_DECK)
                color = COLOR_DECK;
            
            // Wheels
            if (materialID == MAT_ID_WHEEL)
                color = COLOR_WHEEL;
            
            // Trucks
            if (materialID == MAT_ID_TRUCK)
                color = COLOR_TRUCK;
            
            // Legs
            if (materialID == MAT_ID_LEG)
                color = COLOR_LEG;
            
            // Shoes
            if (materialID == MAT_ID_SHOE)
                color = COLOR_SHOE;
            
            // Soft shadows
            float shadow = clamp(softShadow(position + RAYMARCH_MINSTEP * normal, SUNLIGHT_DIRECTION), 0.0, 1.0);

            // Direct light from Sun
            vec3 sunLight = SUN_BRIGHTNESS * SUN_COLOR * clamp(dot(normal, SUNLIGHT_DIRECTION), 0.0, 1.0);

            // Diffused light from Sky
            vec3 skyLight = SKY_BRIGHTNESS * SKY_COLOR * (0.5+0.5*normal.y);
            
            // Bounce light from sunny surfaces
            vec3 bounceLight = BOUNCE_BRIGHTNESS * AMBIENT_COLOR * dot(normal, SUN_DIRECTION*(vec3(-1,0,-1)));
            
            // Specular using Blinn-Phong
            vec3 half1 = normalize(SUN_DIRECTION - rayDirection);
            vec3 specular = SUN_BRIGHTNESS * SUN_COLOR * pow(clamp(dot(half1, normal), 0.0, 1.0), 16.0) //16.0 sets radius
                // Fresnel effect
                * pow(1.0 - clamp(dot(half1, SUNLIGHT_DIRECTION), 0.0, 1.0), 5.0); //5.0 is magic number

            // Combine lights and shadows
            color *= (shadow*sunLight + skyLight + bounceLight);
        
            // Shiny surfaces
            if(materialID == MAT_ID_CONCRETE)
                color += color * specular * shadow;
            
            if(materialID == MAT_ID_TRUCK)
                color += specular * shadow;
        }
        
    }
    
    // If nothing was hit - background
    else
        color = drawBackground(rayDirection);
    
    // Gamma correction
    color = pow(color, vec3(0.4545));

    // Output to screen
    gl_FragColor = vec4(color, 1.0);
}