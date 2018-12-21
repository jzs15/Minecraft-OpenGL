varying vec2 texcoord;
uniform sampler2D texture;


void main(void) {
	if(texcoord.y > 1.0)
	{
		gl_FragColor = vec4(1.0);
		return;
	}
	vec4 color = texture2D(texture, texcoord);
	if(color.a < 0.4)
		discard;
	gl_FragColor = color;
}
