#version 150

uniform sampler2DRect image;
uniform sampler2DRect imageMask;
uniform int imgWidth;
uniform int mskXPos;
uniform int imgXPos;

in vec2 texCoordVarying;

out vec4 outputColor;

void main()
{
    // move and wrap the msk
    vec2 texCoordMsk = texCoordVarying;
    
    texCoordMsk.x += mskXPos;
    
    if (texCoordMsk.x <0){
        texCoordMsk.x +=imgWidth * (abs( int (texCoordMsk.x / imgWidth))+1);
    }
    else if(texCoordMsk.x > imgWidth){
        texCoordMsk.x -= imgWidth * abs( int (texCoordMsk.x / imgWidth));
    }
    
    // move and wrap the img
    vec2 texCoordImg = texCoordVarying;
    texCoordImg.x += imgXPos;
    
    if (texCoordImg.x <0){
        texCoordImg.x +=imgWidth * (abs( int (texCoordImg.x / imgWidth))+1);
    }
    else if(texCoordImg.x > imgWidth){
        texCoordImg.x -= imgWidth * abs( int (texCoordImg.x / imgWidth));
    }
    
    
    vec4 texel0 = texture(image, texCoordImg);
    vec4 texel1 = texture(imageMask, texCoordMsk);
    // blend together
    outputColor = vec4(texel0.rgb, texel0.a * texel1.a);
    
}