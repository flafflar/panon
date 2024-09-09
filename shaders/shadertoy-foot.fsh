// vim: set ft=glsl:
layout(location = 0) in vec2 qt_TexCoord0;
layout(location = 0) out vec4 out_Color;

vec2 getCoord() {
    switch(coord_gravity) {
    case 1:
        return vec2(coord_inversion!=0?(1-qt_TexCoord0.x): qt_TexCoord0.x,1-qt_TexCoord0.y);
    case 2:
        return vec2(coord_inversion!=0?(1-qt_TexCoord0.x):qt_TexCoord0.x,qt_TexCoord0.y);
    case 3:
        return vec2(coord_inversion!=0?qt_TexCoord0.y:(1-qt_TexCoord0.y),1-qt_TexCoord0.x);
    case 4:
        return vec2(coord_inversion!=0?qt_TexCoord0.y:(1-qt_TexCoord0.y),qt_TexCoord0.x);
    }
}

void main() {
    mainImage( out_Color,floor(getCoord()*iResolution.xy) );
}
