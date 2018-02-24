#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec3 brightness;

void main(void)
{
    gl_FragColor = v_fragmentColor*texture2D(CC_Texture0, v_texCoord);
    gl_FragColor.rgb = vec3(gl_FragColor.r*0.2126+gl_FragColor.g*0.7152+gl_FragColor.b*0.0722)+brightness*gl_FragColor.a;
}