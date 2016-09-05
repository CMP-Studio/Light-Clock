#version 150

const vec3 W = vec3( 0.2125, 0.7154, 0.0721 );

uniform sampler2DRect tex0;

uniform float brightness; //0 - 1
uniform float contrast;
uniform float saturation;

in vec2 texCoordVarying;

out vec4 outputColor;

void main()
{
  vec4 texel0 = texture(tex0, texCoordVarying);
  
  vec3 irgb = texel0.rgb;
  
  vec3 black = vec3( 0.0, 0.0, 0.0 );
  vec3 gray = vec3( 0.5, 0.5, 0.5 );
  float luminance = dot( irgb, W );
  vec3 target = vec3( luminance, luminance, luminance );
  
  vec3 brightnessAdjusted = mix( black, irgb, brightness );
  vec3 contrastAdjusted = mix( gray, brightnessAdjusted, contrast );
  vec3 saturationAdjusted = mix( target, contrastAdjusted, saturation );
  
  outputColor = vec4((saturationAdjusted), texel0.a);
}