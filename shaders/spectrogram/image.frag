#version 440
include(shadertoy-head.fsh)
include(utils.fsh)

void mainImage( out vec4 fragColor, in vec2 fragCoord ) {
    fragColor= texture(iChannel2,fragCoord/iResolution.xy);
    fragColor.a=fParam1+ max(fragColor.r,fragColor.g);
}

include(shadertoy-foot.fsh)
