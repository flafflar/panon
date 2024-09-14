#version 440
include(shadertoy-head.fsh)
include(utils.fsh)

void mainImage( out vec4 fragColor, in vec2 fragCoord ) {
    int pixel_x= int( fragCoord.x);
    int pixel_y= int( fragCoord.y);

    float h=fragCoord.y/iResolution.y;


    fragColor=vec4(0,0,0,0);
    if(pixel_x%(iParam0+iParam1)<iParam0) {
        float x=pixel_x/(iParam0+iParam1) /1.0/iResolution.x*(iParam0+iParam1) ;
        vec3 rgb=getRGB(x);

        vec4 sample1= texture(iChannel1, vec2(x,0)) ;
        float max_=sample1.g*.5+sample1.r*.5;
        if(h<=max_)
            fragColor=vec4(rgb*1.,1.);

        vec4 sample2= texture(iChannel2,vec2(x,0));
        int max_2=int(iResolution.y*(sample2.g+sample2.r)/2);
        if((max_2-1)<pixel_y && pixel_y <max_2+1)
            fragColor=vec4(rgb*1.,1.);
    }
}

include(shadertoy-foot.fsh)
