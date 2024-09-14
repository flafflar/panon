#version 440
include(shadertoy-head.fsh)
include(utils.fsh)

float rand(vec2 co) {
    return fract(sin(dot(co.xy,vec2(12.9898,78.233))) * 43758.5453);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord ) {

    float h=fragCoord.y/iResolution.y;
    bool gr=(h>.5);
    h=abs(2*h-1);
    h+=0.01;

    fragColor=vec4(0,0,0,0);
    for(int j=0; j<iParam5; j++) {
        float num=rand(fragCoord/iResolution.xy+vec2(iTime/60/60/24/10,j));
        float distanc=(2*num-1);
        float i=(2*num-1)*fParam0/sqrt(fParam1);
        float x=fragCoord.x+i;

        vec4 raw=texture(iChannel1, vec2(x/iResolution.x,0));
        float raw_max=gr?raw.g:raw.r;
        float h_target1=fParam4* raw_max*exp(-distanc*distanc/fParam1)*iResolution.y-.03*iResolution.y*fParam4;
        float h_target2=fParam4* raw_max*exp(-distanc*distanc/fParam1)*iResolution.y;

        if(h_target1<=h*iResolution.y)
            if(h*iResolution.y<=h_target2) {
            fragColor+=vec4(getRGB(5*x/iResolution.x)*fParam3, fParam3);
        }
    }
}

include(shadertoy-foot.fsh)
