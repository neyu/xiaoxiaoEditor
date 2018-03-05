#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec4 colorMultiplier;
uniform vec4 colorOffset;

void main(void)
{
    vec4 c = texture2D(CC_Texture0, v_texCoord);
    gl_FragColor = c*colorMultiplier+colorOffset;
    gl_FragColor.rgb *= gl_FragColor.a;
//    gl_FragColor.a = c.a*colorMultiplier.a*v_fragmentColor.a+colorOffset.a;
//    gl_FragColor.rgb = c.rgb*colorMultiplier.rgb*v_fragmentColor.rgb+colorOffset.rgb*gl_FragColor.a;
}