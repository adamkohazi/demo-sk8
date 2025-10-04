// gl_FragCoord - fragCoord
// gl_FragColor - fragColor
// t - iTime

//static const char* fragmentShader = \
// Time, Mr. Freeman - globally available, not passed between functions
uniform float t;

// Constants
float PI = 3.14159;

// Road parameters
float ROADWIDTH = 5.0;
float LANEWIDTH = 4.0;
float STRIPEWIDTH = 0.1;

// Visual style
float DOWNSCALE = 4.0;
float QUANTIZE = 0.2;

// Bayer matrices for dithering
float bayer2(vec2 a) { a = floor(a);return fract(dot(a, vec2(.5, a.y * .75))); }
float bayer4(vec2 a) { return bayer2(.5 * a) * .25 + bayer2(a); }
float bayer8(vec2 a) { return bayer4(.5 * a) * .25 + bayer2(a); }
//float bayer16(vec2 a) { return bayer4(.25 * a) * .0625 + bayer4(a); }
//float bayer32(vec2 a) { return bayer8(.25 * a) * .0625 + bayer4(a); }
//float bayer64(vec2 a) { return bayer8(.125 * a) * .015625 + bayer8(a); }
//float bayer128(vec2 a) { return bayer16(.125 * a) * .015625 + bayer8(a); }

// Materials
vec4 MAT_BODY        = vec4(1, 0.8, 0.1, 1);
vec4 MAT_GLASS       = vec4(0, 0 , 0, 1);
vec4 MAT_TYRE        = vec4(0.1, 0.1, 0.1, 0);
vec4 MAT_RIM         = vec4(1);
vec4 MAT_LIGHT       = vec4(5);
vec4 dummy;

// Simple noise
float noise(in vec2 p) {
    return 0.5 * (cos(p.x * 3.0) + cos(p.y * 4.0));
}

// Returns the angle of objects based on a rotation with the noise function
float angle(float delay, float parallax) {
    return parallax * noise(vec2(0.2 * (float(t > 48.) * t - delay)));
}


vec3 sky (float x) {
   //Changing background colors: see graphtoy: floor(4*fract(x/16)+clamp((x-64)/4,0,4))
   int i_scene = int(4.0 * fract(t/16.0) + clamp((t-64.0)/4.0, 0.0, 4.0));

    // Dark intro
    if(t > 16. && t < 48.)
        return vec3(0);

    // Day - default
    vec3 a = vec3(1.0, 0.8, 0.2);
    vec3 b = vec3(0.8, 0.6, 0.4);
    vec3 c = vec3(0.7, 0.7, 0.5);
    vec3 d = vec3(0.0, 0.0, 0.9);
    // Ice
    if(i_scene==1) {
        a = vec3(0.5, 0.6, 1.0);
        b = vec3(0.2, 0.2, 0.5);
        //c = vec3(0.7, 0.7, 0.5);
        //d = vec3(0.0, 0.0, 0.9);
    }
    // Desert
    if(i_scene==2) {
        a = vec3(0.5);
        b = vec3(0.5);
        c = vec3(0.5);
        d = vec3(0.0, 0.1, 0.2);
    }
    // Tricolor
    if(i_scene==3) {
        a = vec3(0.7, 0.3, 0.2);
        b = vec3(0.8, 0.5, 0.1);
        c = vec3(0.5, 0.5, 0.7);
        d = vec3(0.1, 0.1, 0.5);
    }
    // Outrun
    if(i_scene==4) {
        a = vec3(0.4, 0.0, 0.4);
        b = vec3(0.5, 0.1, 0.4);
        c = vec3(0.5, 0.7, 0.7);
        d = vec3(0.1, 0.7, 0.7);
    }
    // Green
    if(i_scene==7) {
        a = vec3(0.3, 0.4, 0.3);
        b = vec3(0.4, 0.4, 0.3);
        c = vec3(0.5, 0.5, 0.7);
        d = vec3(0.0, 0.0, 0.6);
    }
    // Mars
    if(i_scene==6) {
        a = vec3(0.0, 0.3, 0.0);
        b = vec3(0.5, 0.8, 0.2);
        c = vec3(0.5, 0.2, 0.5);
        d = vec3(0.9, 0.2, 0.0);
    }
    // Sin City
    if(i_scene==5) {
        a = vec3(0.9);
        b = vec3(1.0);
        c = vec3(0.2);
        d = vec3(0.3);
    }
    return a + b * cos(2.0 * PI * (c*x + d));
}

// TODO replace with rotation around arbitrary axis
vec3 rotateX(float theta, vec3 v) {
    return vec3(v.x, v.y * cos(theta) - v.z * sin(theta), v.y * sin(theta) + v.z * cos(theta));
}

vec3 rotateY(float theta, vec3 v) {
    return vec3(v.x * cos(theta) + v.z * sin(theta), v.y, -v.x * sin(theta) + v.z * cos(theta));
}

vec3 rotateZ(float theta, vec3 v) {
    return vec3(v.x * cos(theta) - v.y * sin(theta), v.x * sin(theta) + v.y * cos(theta), v.z);
}

vec3 mirrorZ(vec3 v) {
    return vec3(v.xy, abs(v.z));
}

float SDFBox(vec3 position, vec3 center, vec3 size) {
    vec3 q = abs(position - center) - 0.5 * size;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

// Horizontal cylinder pair (2.4 apart)
float SDFCylinder(vec3 p, float h, float r)
{
    p.x = abs(p.x) - 0.5 * 2.4;
    vec2 d = abs(vec2(length(p.yx), p.z)) - vec2(r, 0.5 * h);
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0));
}

// SDF operands
float opSmoothUnion(float d1, float d2, float k) {
    float h = clamp(0.5 + 0.5 * (d2 - d1) / k, 0.0, 1.0);
    return mix(d2, d1, h) - k * h * (1.0 - h);
}

float opSmoothSubtraction(float d1, float d2, float k) {
    float h = clamp(0.5 - 0.5 * (d2 + d1) / k, 0.0, 1.0);
    return mix(d2, -d1, h) + k * h * (1.0 - h);
}

// Draw road
vec4 getRoadColor(vec3 rayPosition) {
    float center = 1.5 - angle(0.0, 0.006 * rayPosition.z * rayPosition.z - 2.0);
    float lane = cos(rayPosition.z + 32.0 * max(0, t-48.));

    // Ground
    vec3 groundColor = sky(0.3);
    if(lane>0)
        groundColor -= 0.1;

    // Road 
    if (abs(rayPosition.x - center) < ROADWIDTH) {
        // Light
       groundColor = vec3(0.1);
        // Dark
       if(lane>0) {
            //Road only dark during intro
            if(t > 32.)
                groundColor += 0.1; 
            // Lane stripes
            if(abs(rayPosition.x - center) < STRIPEWIDTH)
                groundColor += 1.0;
       }
        // Edge stripes
        if(abs(abs(rayPosition.x - center) - LANEWIDTH) < STRIPEWIDTH)
            groundColor += 1.0;
   }
    // Blend to background
    groundColor = mix(groundColor, sky(0.6), length(rayPosition) / 70.0);

    // Road is grey during dark scenes
    if (t > 16. && t < 48.)
        groundColor = vec3(0.05 * abs(atan(1.0/length(rayPosition))));
   
    // Outrun
    if (t > 80. && t < 96.)
        groundColor = 9.0/length(rayPosition)*sky(0.3) * float(cos((rayPosition.z) + 32.0 * t)>0.99 || cos(rayPosition.x-center)>0.99);
       
   return vec4(groundColor, 0);
}

// Countach
float map(in vec3 position, out vec4 material) {
    float d = 96.0;
    float d_mat = 96.0;

    // Rotate by 90
    position = rotateY(PI/2.0, position); //TODO rotate car model

   if(t > 16.) { //Only show car after 16s
        // Windshield
        d = min(d, SDFBox(position, vec3(0.5, 0.85, 0), vec3(1.5,0.35,1.84))); //roof
        if (abs(d - d_mat)>.001)
           material = MAT_BODY;
        d_mat = d;
    
        d = opSmoothSubtraction(SDFBox(rotateX(0.9,mirrorZ(position)), vec3(0, 0, 1.6), vec3(3,1,1)), d, 0.03); //side window angle
        d = opSmoothSubtraction(SDFBox(rotateZ(0.4,rotateY(0.3,mirrorZ(position))), vec3(0.7, 1.9, 0.0), vec3(1.5)), d, 0.05); //windshield front curve
        if (abs(d - d_mat)>.001)
           material = MAT_GLASS;
        d_mat = d;
    
        // Windshield Beams
        d = opSmoothUnion(SDFBox(rotateY(0.5,rotateZ(0.4,mirrorZ(position))), vec3(0.6, 0.57, 0.4), vec3(1,1,0.01)), d, 0.04); //windshield beam
    
        // Main Body
        d = min(d, SDFBox(position, vec3(-0.34, 0.39, 0.0), vec3(3.17, 0.65, 1.84))); //main box
        d = min(d, SDFBox(rotateZ(0.4,position), vec3(1.34, 0.89, 0.0),vec3(0.95, 0.5, 1.84))); //hood
        d = opSmoothSubtraction(SDFBox(rotateY(-0.3,mirrorZ(position)), vec3(1.45, 0.5, 1.8),vec3(1)), d, 0.1); //make hood wedge shaped
        d = min(d, SDFBox(rotateZ(-0.18, position), vec3(-0.91, 0.9, 0), vec3(1.7, 0.32, 1.84))); //hatch
    
        // Hatch cuts
        d = opSmoothSubtraction(SDFBox(rotateY(-0.15,rotateZ(-0.12,mirrorZ(position))), vec3(-1.1, 0.94, 0), vec3(1.7, 0.15, 0.6)), d, 0.05); //hatch cut
        d = opSmoothSubtraction(SDFBox(rotateX(0.9,mirrorZ(position)), vec3(0, 0, 1.63), vec3(3,1,1)), d, 0.03); //shaping hatch to side windows
        d = opSmoothSubtraction(SDFBox(rotateY(0.15, rotateX(1.3,mirrorZ(position))), vec3(-1, -0.4, 1.65), vec3(2,1,1)), d, 0.03); //hatch side cut
        d = opSmoothUnion(SDFBox(rotateY(0.1, rotateX(1.2,mirrorZ(position))), vec3(-0.5, -0.35, 1), vec3(0.38))-0.02, d, 0.00); //air inlet bumps
        d = opSmoothSubtraction(SDFBox(rotateX(0.2, rotateZ(0.5,mirrorZ(position))), vec3(-1.6, -1.2, 0.35), vec3(1)), d, 0.05); //back skirt corner cut
    
        // Spoiler alert
        d = min(d, SDFBox(rotateY(0.1,mirrorZ(position)), vec3(-1.6, 0.95, 0.0), vec3(0.35, 0.03,2.2))); //spoiler
        d = opSmoothUnion(SDFBox(mirrorZ(position), vec3(-1.6, 0.8, 0.55), vec3(0.2,0.22,0.01)), d, 0.05); //spoiler bar
    
        // Wheel flares
        d = opSmoothUnion(d, SDFCylinder(mirrorZ(position)-vec3(-0.1, 0.3, 0.9), 0.1, 0.35), 0.1);
        // Ground clearence
        d = max(d, -SDFBox(position, vec3(0), vec3(5,0.3,2))); 
    
        if (abs(d - d_mat)>.001)
           material = MAT_BODY;
        d_mat = d;
        // Black parts
        d = max(d, -SDFCylinder(mirrorZ(position)-vec3(-0.1, 0.3, (1.9-0.35)/2.0), 0.6, 0.35)); //wheel hole
        d = min(d, SDFCylinder(mirrorZ(position)-vec3(-0.1, 0.3, (1.9-0.35)/2.0), 0.35, 0.3)); //wheels
        if (abs(d - d_mat)>.001)
           material = MAT_TYRE;
        d_mat = d;
    
        // Rims
        d = max(d, -SDFCylinder(mirrorZ(position)-vec3(-0.1, 0.3, 1), 0.2, 0.22)); //rim inset hole
        if (abs(d - d_mat)>.001)
           material = MAT_RIM;
        d_mat = d;
    
        // Front lights
        d = min(d, SDFBox(rotateZ(0.4, mirrorZ(position)), vec3(1.7, 1.1, 0.5),vec3(0.1, 0.1, 0.3)));
        if (abs(d - d_mat)>.001)
           material = MAT_LIGHT;
        d_mat = d;
   }

    //Road - plane
    return min(d, dot(position, vec3(0, 1, 0)));
}

// Calculate surface normal
vec3 calculateNormal(vec3 p) {
    float EPS = 0.001;
    vec3 v1 = vec3(
        map(p + vec3(EPS, 0.0, 0.0), dummy),
        map(p + vec3(0.0, EPS, 0.0), dummy),
        map(p + vec3(0.0, 0.0, EPS), dummy));
    vec3 v2 = vec3(
        map(p - vec3(EPS, 0.0, 0.0), dummy),
        map(p - vec3(0.0, EPS, 0.0), dummy),
        map(p - vec3(0.0, 0.0, EPS), dummy));

    return normalize(v1 - v2);
}

// Draw Background
vec4 getBackgroundColor(vec3 position) {  
    // from 0 to 1 from horizont to top of sky
    float theta = position.y/length(position);

    // from -pi to pi around the perimeter
    float fi = sign(position.z) * acos(position.x/length(position.xz))
        // Background moving with turns
        + angle(3.0, 0.5);

    // Sky
    vec3 color = sky(sqrt(theta));

    //Sun
    if(length(vec2(fi-1.3, theta-0.1)) < 0.1)
        color += sky(0.0);

    //parallax
    fi += angle(3.0, 0.1);

    //Skyline
    for (float dist = 4.0; dist > 0.9; dist /= 2.0) {

        //Buildings
        float skyline = 0.1 * (noise(vec2(floor(20.0*fi*dist + dist)))
                               -0.2 * dist
                               +sin(2.0*fi));

        //Mountains
        if (fract(t/8.)<0.5)
           skyline = 0.4*abs(fract(3.0*fi+dist+0.5)-0.5)*noise(vec2(floor(3.0*fi+dist)))
                   + 0.2*abs(fract(10.0*fi+dist-0.5)-0.5)*noise(vec2(floor(10.0*fi+dist)))
                   + 0.1;

        //Coloring
        if (theta < skyline)
            color = sky(0.3+0.4/dist-0.7*theta);

        //Outrun Coloring       
        if (t > 80. && t < 96.)
            if(theta < skyline-0.01)
                color *= float(fract(99.0*theta*(skyline-0.5))<0.1);
    }
    
    return vec4(color, 0);
}


// Ray marching
float castRay(in vec3 rayOrigin, vec3 rayDirection, out vec4 material) {
    float distance = 0.0;
    while(1) {
        //Find next step
        float step = map(rayOrigin + distance * rayDirection, material);

        //Close to object
        if(step < 0.001) {
            if ((rayOrigin + distance * rayDirection).y<0.01)
                material = getRoadColor(rayOrigin + distance * rayDirection);
            return distance;
        }
            
        //March!
        distance += step;

        //Max render distance
        if(length(rayOrigin + distance * rayDirection)>96.0) {
            material = getBackgroundColor(rayOrigin + distance * rayDirection);
            return -1.0;
        }
    } 
}

// Calculate soft shadows
float softShadow(in vec3 rayOrigin, vec3 rayDirection)
{
    float res = 1.0;
    float distance = 0.0; //Min distance

    while (distance < 16.0) //Max distance
    {
        float step = map(rayOrigin + distance * rayDirection, dummy);
        if(step < 0.001)
            return 0.0;
        res = min(res, 4.0 * step/distance ); //hardness
        distance += step;
    }
    return res;
}

// Calculate ambient occlusions
float calcOcclusion(vec3 pos, in vec3 nor)
{
	float occ = 0.0;
    float sca = 1.0;
    for(int i=0; i<5; i++)
    {
        float h = 0.01 + 0.11*float(i)/4.0;
        vec3 opos = pos + h*nor;
        occ += (h-map(opos, dummy))*sca;
        sca *= 0.95;
    }
    return clamp(1.0 - 2.0*occ, 0.0, 1.0);
}

void main(void)
//void mainImage( out vec4 gl_FragColor, vec2 gl_FragCoord )
{
    //Pixel coordinates (from -1 to 1)
    //Downscales if set
    vec2 p = (2.0 * floor(gl_FragCoord / DOWNSCALE) * DOWNSCALE - vec2(1280, 720)) / 720.;

    int i_camera = int(4.*fract(t/16.))+2*int(t>64.);

    // Camera position
    // Helicopter - top view: default
    vec3 rayOrigin = vec3(-4.0 * sin(0.1 * t), 8.0, -4.0 * cos(0.1 * t));
    vec3 cameraDirection = vec3(0.0, 0.5, 0.0);

    // Wheel view
    if(i_camera==0) {
       rayOrigin = vec3(-2.0 + sin(0.1 * t), 0.4, -1.25);
       cameraDirection = vec3(0.0, 0.4, 1.25);
    }
    // Rotating car view
    if(i_camera==2) {
       rayOrigin.y = 1.5;
       //cameraDirection = vec3(0.0, 0.5, 0.0);
    }
    // Staring back down the road / mirror view
    if(i_camera==3) {
       rayOrigin = vec3(sin(0.5 * t)-1.0, 0.5, 5.0);
       cameraDirection.z = -16.0;
    }
    // Driveby view
    if(i_camera==5) {
       rayOrigin = vec3(-3.0, 1.5, -96.0 * (fract(t/4.)-0.5));
       //cameraDirection = vec3(0.0, 0.5, 0.0);
    }
    // Racing view
    if(i_camera==4 || t>64. && t<80.) {
       float i_fi = angle(0.5, -0.3);
       rayOrigin = vec3(-5.0 * sin(i_fi), 1.5, -5.0 * cos(i_fi));
       cameraDirection.z = 16.0;
    }

   vec3 i_ww = normalize(cameraDirection - rayOrigin);
   vec3 i_uu = normalize(cross(i_ww, vec3(0.0, 1.0, 0.0)));
   vec3 i_vv = normalize(cross(i_uu, i_ww));
   vec3 rayDirection = normalize(p.x * i_uu + p.y * i_vv + 2.0 * i_ww);

   vec4 material;
   float distance = castRay(rayOrigin, rayDirection, material);

   if (distance > 0.0) {
        // Light parameters
        float lightAmbientStrength = 0.1;
        float light1Strength = 0.9;
        vec3 light1Direction = normalize(vec3(sin(angle(3.0, 0.7) + 0.2), 1, cos(angle(3.0, 0.7) + 0.2)));
        vec3 light1Color = sky(0.3);
        vec3 lightAmbientColor = sky(0.4);

        // Ray hit
        vec3 position = rayOrigin + distance * rayDirection;
        vec3 normal = calculateNormal(position);

        // Flashing rotating light during darkness
       if(t>16. && t<48.) {
           light1Strength = float(t>32.)-sin(t)*sin(t);
           light1Color = vec3(1);
           light1Direction = normalize(vec3(sin(1.1 * (t-0.2)), 0.6, cos(1.1 * (t-0.2))));
       }  
       float i_diffusedLight1 = clamp(dot(normal, light1Strength * light1Direction), 0.0, 1.0);

        // Specular using Blinn-Phong
       vec3 i_half1 = normalize(light1Direction - rayDirection);
       float i_specularLight1 = pow(clamp(dot(i_half1, normal), 0.0, 1.0), 128.0) //32.0 sets radius
            // Fresnel effect
            * mix(pow(1.0 - clamp(dot(i_half1, light1Direction), 0.0, 1.0), 5.0), 1.0, material.w); //5.0 is magic number

        // Soft shadows
       float i_shadow1 = softShadow(position + 0.001 * normal, light1Direction);

        // Fake ambient occlusion
       float i_occlusion = calcOcclusion(position, normal);

        //Mixing lights
       material.xyz *= (lightAmbientColor * lightAmbientStrength + light1Color * i_diffusedLight1 * i_shadow1) * i_occlusion
            + 4.0 * light1Color * i_specularLight1 * i_shadow1;
   }

     //Gamma correction
    material = pow(material, vec4(0.4545));

     // Dithering with Bayer
    float i_bayer = bayer8(gl_FragCoord / DOWNSCALE) - 0.5;
    material += i_bayer * 0.2;

     // Quantizing palette
    
    material = floor((material + QUANTIZE / 2.)/QUANTIZE)*QUANTIZE;

     // Metallica - Fade to black
    material *= clamp((112.0-t)/4.0,0.0,1.0);

     // Output to screen
    gl_FragColor = vec4(material.xyz, 1.0);
}