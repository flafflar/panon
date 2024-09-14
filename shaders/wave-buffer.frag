#version 440

layout(location = 0) out vec4 out_Color;
layout(location = 0) in vec2 qt_TexCoord0;

layout(binding = 1) uniform sampler2D newWave;
layout(binding = 2) uniform sampler2D waveBuffer;

layout(std140, binding = 0) uniform _data {
    mat4 qt_Matrix;
    float qt_Opacity;

    int bufferSize;
    int newWaveSize;
};

void main() {
    float x=qt_TexCoord0.x*bufferSize;
    float y=qt_TexCoord0.y*2;
    
    if(y<2){
        if(x>= bufferSize-newWaveSize ){
            out_Color=texelFetch(newWave,ivec2(x-bufferSize+newWaveSize,y),0);
        }else{
            out_Color=texelFetch(waveBuffer,ivec2(x+newWaveSize,y),0);
        }
    }
}
