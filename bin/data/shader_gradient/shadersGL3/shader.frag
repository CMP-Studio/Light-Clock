#version 150

uniform int begFadeStart;
uniform int begFadeEnd;
uniform int endFadeStart;
uniform int endFadeEnd;

uniform int width;

out vec4 outputColor;

void main()
{
    float texCoord = gl_FragCoord.x;
    
    float a = 0.0;
    if ((texCoord > begFadeStart) && (texCoord < begFadeEnd) ){
        a = (texCoord - begFadeStart) / (begFadeEnd - begFadeStart);
    }
    else if ((texCoord > begFadeEnd ) && (texCoord < endFadeStart)){
        a = 1 ;
    }
    else if ((texCoord > endFadeStart ) && (texCoord < endFadeEnd)){
        a = 1 - ((texCoord - endFadeStart) / (endFadeEnd - endFadeStart));
    }
    
    outputColor = vec4(1.0,1.0,1.0, a);
}