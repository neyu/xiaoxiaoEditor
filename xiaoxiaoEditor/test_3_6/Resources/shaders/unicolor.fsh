#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform float unicolorMultiplier;
uniform float unicolorAdd;
uniform vec4 colorMultiplier;

void main(void)
{
    vec4 c = texture2D(CC_Texture0, v_texCoord);
    c.rgb = (vec3(0.2126*c.r + 0.7152*c.g + 0.0722*c.b)*unicolorMultiplier+unicolorAdd)*colorMultiplier.xyz*colorMultiplier.w;
    c.w = c.w*colorMultiplier.w;
    gl_FragColor = c;
//    gl_FragColor.rgb = vec3(0.2126*c.r + 0.7152*c.g + 0.0722*c.b);
//    gl_FragColor.w = c.w;
}