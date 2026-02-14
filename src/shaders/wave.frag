#version 440
layout(location = 0) in vec2 qt_TexCoord0;
layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform buf {
  mat4 qt_Matrix;
  float qt_Opacity;

  vec3 iResolution;
};

layout(binding = 1) uniform sampler2D iChannel0;

void main() {
  vec2 uv = qt_TexCoord0;
  vec4 sampl = texture(iChannel0, vec2(uv.x, 0));
  float amplitude = (sampl.x + sampl.y) / 2.0;

  float top = amplitude - 1/iResolution.y;
  float bottom = amplitude + 1/iResolution.y;
  
  if (uv.y >= top && uv.y <= bottom) {
    fragColor = vec4(1);
  } else {
    fragColor = vec4(0);
  }
}
