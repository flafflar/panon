// vim: set ft=glsl:
layout(location = 0) in vec2 qt_TexCoord0;
layout(location = 0) out vec4 out_Color;

vec2 getCoord() {
    return qt_TexCoord0;
}

void main() {
    mainImage( out_Color,floor(getCoord()*iResolution.xy) );
}
