// Outside variables
uniform float TIME;

uniform vec3 BOARDPOS;
uniform vec3 BOARDEULER;

uniform vec3 LEGRHIP;
uniform float LEGRKNEE;
uniform float LEGRANKLE;

uniform vec3 LEGLHIP;
uniform float LEGLKNEE;
uniform float LEGLANKLE;

uniform vec3 BODYHIPEULER;
uniform vec3 BODYHIPPOS;

// Constants
const float PI = 3.14159;

/* Data types */
// Board
// Structure describing relative board position
struct BoardPosition {
    vec3 position;
    vec3 euler;
};

//Body
// Structure describing relative position of a single leg
struct LegPosition {
    // https://image1.slideserve.com/1887324/slide12-l.jpg
    vec3 hipJoint; // (x: flexion, y: abduction, z: rotation)
    float kneeAngle;
    float ankleAngle;
};

// Structure describing relative position of the lower body
struct BodyPosition {
    vec3 hipPosition;
    vec3 hipEuler;
    LegPosition rightLeg;
    LegPosition leftLeg;
};

/* SDF primitives */
//Box
float SDFBox(vec3 position, vec3 size) {
    vec3 q = abs(position) - 0.5 * size;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

// Sphere
float SDFSphere(vec3 position, float size) {
  return length(position)-size;
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
// 3D rotations
// Rotate point around arbitrary axis
// https://suricrasia.online/blog/shader-functions/
vec3 rotateA(float theta, vec3 axis, vec3 point) {
    return mix(dot(axis, point)*axis, point, cos(theta)) + cross(axis,point)*sin(theta);
}

// Rotate point around main axis
vec3 rotateX(float theta, vec3 point) {
    return rotateA(theta, vec3(1,0,0), point);
}

vec3 rotateY(float theta, vec3 point) {
    return rotateA(theta, vec3(0,1,0), point);
}

vec3 rotateZ(float theta, vec3 point) {
    return rotateA(theta, vec3(0,0,1), point);
}

// Smooth Operators
float opSmoothUnion( float d1, float d2, float k ) {
    float h = clamp( 0.5 + 0.5*(d2-d1)/k, 0.0, 1.0 );
    return mix( d2, d1, h ) - k*h*(1.0-h);
}

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

// Assemble skateboard from components
float SDFSkateboard(vec3 position, BoardPosition board) {
    vec3 centerpoint = vec3(0.0, 0.05, 0.0);
    position -= centerpoint;
    position -= board.position;
    position = rotateZ(board.euler.z, position);
    position = rotateY(board.euler.y, position); 
    position = rotateX(board.euler.x, position);
    return min(SDFDeck(position + centerpoint),
               min(SDFWheels(position + centerpoint),
                   SDFTrucks(position + centerpoint))
    );
}

// Shoe
float SDFShoe(vec3 position, vec3 startPoint, vec3 endPoint, float radius) {
  return max(
      SDFCapsule(position, startPoint, endPoint, radius),
      -dot(position-startPoint, cross(vec3(1,0,0), normalize(endPoint-startPoint)))
  );
}

float SDFLeg(vec3 position, LegPosition leg){
    const float thighLen = 0.4;
    const float shinLen = 0.3;
    const float footLen = 0.12;
    
    const float thighWidth = 0.05;
    const float shinWidth = 0.04;
    const float footWidth = 0.04;
    
    //Hip joint rotation
    position = rotateY(leg.hipJoint.z, position); //rotation
    
    //Calculate joint positions
    // Knee
    vec3 kneePos = rotateX(leg.hipJoint.x,-vec3(0,thighLen,0)); //flexion
    kneePos = rotateZ(leg.hipJoint.y, kneePos); //abduction
 
    // Ankle
    vec3 anklePos = kneePos + rotateX(-(leg.hipJoint.x + leg.kneeAngle),-vec3(0,shinLen,0));
    anklePos = rotateZ(leg.hipJoint.y/2.0, anklePos); //abduction
    
    // Toe
    vec3 toePos = anklePos + rotateX(-(leg.hipJoint.x + leg.kneeAngle + leg.ankleAngle),-vec3(0,footLen,0));
    
    // Draw joints
    //distance = min(distance, SDFSphere(position-kneePos, 0.05));
    //distance = min(distance, SDFSphere(position-anklePos, 0.05));
    //distance = min(distance, SDFSphere(position-toePos, 0.05));
    
    // Draw limbs
    // Draw with cylinders
    //distance = min(distance, SDFCylinder(rotateX(hipRAngle           ,position-hipRPos )+vec3(0,0.5*thighLen,0), 0.5*thighLen, 0.04));
    //distance = min(distance, SDFCylinder(rotateX(hipRAngle+kneeRAngle,position-kneeRPos)+vec3(0,0.5*shinLen, 0), 0.5*shinLen , 0.03));
    //distance = min(distance, SDFCylinder(rotateX(hipRAngle+kneeRAngle+ankleRAngle,position-ankleRPos)+vec3(0,0.5*footLen, 0), 0.5*footLen , 0.03));
    
    // Draw with capsules
    float distance = SDFCapsule(position, vec3(0), kneePos, thighWidth);
    distance = min(distance, SDFCapsule(position, kneePos, anklePos, shinWidth));
    distance = min(distance, SDFShoe(position, anklePos, toePos, footWidth));
    
    return distance;
}

// Body
float SDFBody(vec3 position, BodyPosition body) {
    //Relative move according to keyframe info
    position -= body.hipPosition;
    
    const float hipHeight = 0.8;
    const float hipWidth = 0.2;

    float distance = 10.0;
    
    float twistHip = 0.0;
    position = rotateY(twistHip, position);
    
    vec3 hipRPos = vec3(hipWidth/2., hipHeight, 0);
    //distance = min(distance, SDFSphere(position-hipRPos, 0.05));
    vec3 hipLPos = vec3(-hipWidth/2., hipHeight, 0);
    //distance = min(distance, SDFSphere(position-hipLPos, 0.05));
    
    return min(
        SDFLeg(position-hipRPos, body.rightLeg),
        SDFLeg(position-hipLPos, body.leftLeg)
    );
}

// Scene
float map(in vec3 position) {

    // Ground plane
    float distance = 10.0;
    distance = min(distance, dot(position, vec3(0, 1, 0)));

    // Board
    BoardPosition boardPosition = BoardPosition(
        BOARDPOS, // position
        BOARDEULER  // euler
    );
    
    LegPosition legPosR = LegPosition(
        LEGRHIP, // hipJoint (x: flexion, y: abduction, z: rotation)
        LEGRKNEE, //kneeAngle
        LEGRANKLE  // ankleAngle
    );
    
    LegPosition legPosL = LegPosition(
        LEGLHIP, // hipJoint (x: flexion, y: abduction, z: rotation)
        LEGLKNEE, //kneeAngle
        LEGLANKLE  // ankleAngle
    );

    BodyPosition bodyPosition = BodyPosition(
        BODYHIPPOS, // hipPosition
        BODYHIPEULER, // hipEuler
        legPosR,
        legPosL
    );
    
    distance = min(distance, SDFSkateboard(position, boardPosition));
    distance = min(distance, SDFBody(position, bodyPosition));
    return distance;
}

// Calculate surface normal
// https://iquilezles.org/articles/normalsSDF/
vec3 calculateNormal(vec3 p) {
    float EPS = 0.001;
    vec3 v1 = vec3(
        map(p + vec3(EPS, 0.0, 0.0)),
        map(p + vec3(0.0, EPS, 0.0)),
        map(p + vec3(0.0, 0.0, EPS)));
    vec3 v2 = vec3(
        map(p - vec3(EPS, 0.0, 0.0)),
        map(p - vec3(0.0, EPS, 0.0)),
        map(p - vec3(0.0, 0.0, EPS)));

    return normalize(v1 - v2);
}

// Ray marching: Follow a ray of light from camera origin until collision to find distance
// and material. Material is passed along using the "out" argument.
vec3 castRay(in vec3 rayOrigin, vec3 rayDirection) {
    // material
    float brightness = 1.0;

    // Start from origin
    float distance = 0.0;
    for(int i=0; i<100; i++) {
        // Find stepsize for marching (distance of closest point from current position)
        float step = map(rayOrigin + distance * rayDirection);

        // An object is hit
        if(step < 0.001) {
            return vec3(distance, 0.5, 0);
        }
            
        // March!
        distance += step;
    } 
    
    // Object not hit
    return vec3(-1, 1, 0);
}

// Calculate soft shadows
// Thanks iq!
// https://iquilezles.org/articles/rmshadows/
float softShadow( in vec3 ro, in vec3 rd, float mint, float maxt, float w )
{
    float res = 1.0;
    float ph = 1e20;
    float t = mint;
    for(int i=0; i<256 && t<maxt; i++) {
        float h = map(ro + rd*t);
        if(h<0.001)
            return 0.0;
        float y = h*h/(2.0*ph);
        float d = sqrt(h*h-y*y);
        res = min( res, d/(w*max(0.0,t-y)) );
        ph = h;
        t += h;
    }
    return res;
}

// Calculate ambient occlusions
// Stole it from here:
// https://www.shadertoy.com/view/3lsSzf
float calcOcclusion(vec3 pos, in vec3 nor)
{
	float occ = 0.0;
    float sca = 1.0;
    for(int i=0; i<5; i++) {
        float h = 0.01 + 0.11*float(i)/4.0;
        vec3 opos = pos + h*nor;
        occ += (h-map(opos))*sca;
        sca *= 0.95;
    }
    return clamp(1.0 - 2.0*occ, 0.0, 1.0);
}

void main(void)
{
    // Pixel coordinates (from -1 to 1)
    // Also downscales image to get that retro look
    vec2 p = (2.0*floor(gl_FragCoord)-vec2(1280, 720))/720.;

    // Camera position
    vec3 rayOrigin = vec3(0, 0.2, -2);
    //vec3 rayOrigin = vec3(1.0*sin(TIME), 0.2, -1.0*cos(TIME));
    vec3 cameraDirection = vec3(0,0.2,0) - rayOrigin;
    
    // Find ray direction for ray marching, based on position and direction
    vec3 i_ww = normalize(cameraDirection);
    vec3 i_uu = normalize(cross(i_ww, vec3(0.0, 1.0, 0.0)));
    vec3 i_vv = normalize(cross(i_uu, i_ww));
    vec3 rayDirection = normalize(p.x * i_uu + p.y * i_vv + 2.0 * i_ww);
    
    vec3 render = castRay(rayOrigin, rayDirection);
    float distance = render.x;
    float brightness = render.y;
    
    // Calculate lighting for valid collisions:
    // This excludes lighting the background
    if (distance > 0.0) {
        // Ray hit
        vec3 position = rayOrigin + distance * rayDirection;
        vec3 normal = calculateNormal(position);
        
        // Ambient light 1
        float lightAmbientStrength = 0.0;
         
        // Diffused light 1
        vec3 light1Direction = normalize(vec3(0.3, 1, 0.2));
        
        float light1Strength = 1.0;
        float diffusedLight1 = clamp(dot(normal, light1Strength*light1Direction),0.0, 1.0);

        // Soft shadows
        float minDistance = 0.01;
        float maxDistance = 5.0;
        float hardness = 0.5;
        float shadow1 = softShadow(position + 0.001*normal, light1Direction, minDistance, maxDistance, hardness);
        
        // Fake ambient occlusion
        //float occlusion = calcOcclusion(position, normal) * 1.0;
        
        //Mixing lights
        float linear = 0.0;
        linear += lightAmbientStrength;
        linear += diffusedLight1 * shadow1;
        //linear *= occlusion;
        
        brightness = brightness * linear;
        
        // Distance fog
        brightness += min(1.0, 0.01*distance*distance);
    }
    
    vec3 color = vec3(brightness);

    // Gamma correction
    color = pow(color, vec3(0.4545));

    // Output to screen
    gl_FragColor = vec4(color, 1.0);
}