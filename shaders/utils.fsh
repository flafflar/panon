// vim: set ft=glsl:
include(hsluv-glsl.fsh)

vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec3 getRGB(float x) {
    if(colorSpaceHSL != 0) {
        return hsv2rgb(vec3(x*(hueTo-hueFrom)/360.0+hueFrom/360.0,saturation/100.0,lightness/100.0));
    } else if(colorSpaceHSLuv != 0) {
        return hsluvToRgb(vec3(x*(hueTo-hueFrom)+hueFrom,saturation,lightness));
    }
}
