#version 440
include(shadertoy-head.fsh)

#define decay 0.003

void mainImage(out vec4 fragColor, in vec2 fragCoord) {
    float x=fragCoord.x/iResolution.x;
    vec4 newdata= texture(iChannel1, vec2(x,0));
    vec4 olddata= texture(iChannel2,vec2(x,0));
    fragColor=max(newdata,olddata-decay);
}

include(shadertoy-foot-buffer.fsh)
