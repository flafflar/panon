#version 440
layout(location = 0) in vec2 qt_TexCoord0;
layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform buf {
  mat4 qt_Matrix;
  float qt_Opacity;

  vec3 iResolution;
};

layout(binding = 1) uniform sampler2D iChannel0;
layout(binding = 2) uniform sampler2D iChannel1;

void main() {
  vec2 uv = qt_TexCoord0;
  int x = int(qt_TexCoord0.x * iResolution.x);
  if (x % 7 < 2) {
    fragColor = vec4(0);
    return;
  }

  x = x - (x % 7);

  vec4 sampl = texture(iChannel1, vec2(x / iResolution.x, 0));
  float amplitude = sampl.z;

  float top = amplitude - 1/iResolution.y;
  float bottom = amplitude + 1/iResolution.y;
  
  if (uv.y >= 1.0 - 1.0/iResolution.y) {
    fragColor = vec4(1);
  } else if (uv.y >= 1.0 - amplitude) {
    fragColor = vec4(1);
  } else {
    fragColor = vec4(0);
  }
}

