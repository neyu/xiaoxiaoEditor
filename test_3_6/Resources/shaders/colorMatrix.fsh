#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec3 brightnessColor;

void main()
{
    gl_FragColor = texture2D(CC_Texture0, v_texCoord);
    gl_FragColor.rgb += brightnessColor*gl_FragColor.a;
}