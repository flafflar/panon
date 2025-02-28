// vim: set ft=glsl:
precision highp float;
precision highp int;
precision mediump sampler3D;

layout(std140, binding = 0) uniform _data {
    mat4 qt_Matrix;
    float qt_Opacity;

    vec3      iResolution;           // viewport resolution (in pixels)
    float     iTime;                 // shader playback time (in seconds)
    float     iTimeDelta;            // render time (in seconds)
    float     iBeat;                // Is this frame a beat? (provided by aubio)
    int       iFrame;                // shader playback frame
    //float     iChannelTime[4];       // channel playback time (in seconds)
    #define iChannelTime (float[4](iTime,iTime,iTime,iTime))
    vec3      iChannelResolution0; // channel resolution (in pixels)
    vec3      iChannelResolution1; // channel resolution (in pixels)
    vec3      iChannelResolution2; // channel resolution (in pixels)
    vec3      iChannelResolution3; // channel resolution (in pixels)
    #define iChannelResolution (vec3[4](iChannelResolution0,iChannelResolution1,iChannelResolution2,iChannelResolution3))
    vec4      iMouse;                // mouse pixel coords. xy: current (if MLB down), zw: click
    // sampler2D iChannel0;          // input channel. XX = 2D/Cube
    // sampler2D iChannel1;          // input channel. XX = 2D/Cube
    // sampler2D iChannel2;          // input channel. XX = 2D/Cube
    // sampler2D iChannel3;          // input channel. XX = 2D/Cube
    // vec4      iDate;                 // (year, month, day, time in seconds)
    // #define iDate vec4(0,0,0,0)
    //float     iSampleRate;           // sound sample rate (i.e., 44100)
    #define iSampleRate 44100

    int colorSpaceHSL;
    int colorSpaceHSLuv;
    int hueFrom;
    int hueTo;
    int saturation;
    int lightness;

    // gravity property: North (1), West (4), East (3), South (2)
    int coord_gravity;
    int coord_inversion;

    int iParam0;
    int iParam1;
    int iParam2;
    int iParam3;
    int iParam4;
    int iParam5;
    int iParam6;
    int iParam7;
    int iParam8;
    int iParam9;

    float fParam0;
    float fParam1;
    float fParam2;
    float fParam3;
    float fParam4;
    float fParam5;
    float fParam6;
    float fParam7;
    float fParam8;
    float fParam9;
};

layout(binding = 1) uniform sampler2D iChannel0;
layout(binding = 2) uniform sampler2D iChannel1;
layout(binding = 3) uniform sampler2D iChannel2;
layout(binding = 4) uniform sampler2D iChannel3;
