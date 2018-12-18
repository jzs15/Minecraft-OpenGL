varying vec4 texcoord;
uniform sampler2D texture;

const vec4 fogcolor = vec4(0.6, 0.8, 1.0, 1.0);
const float fogdensity = .00003;

float get_u(float w)
{
	while(w >= 8.0)
		w -= 8.0;
	return w;
}

float get_y(float y)
{
	while(y >= 1.0)
		y -= 1.0;
	while(y < 0.0)
		y += 1.0;
	return 1 - y;
}

float get_z(float z)
{
	while(z >= 1.0)
		z -= 1.0;
	while(z < 0.0)
		z += 1.0;
	return z - 1;
}

void main(void) {
	int texW = int(texcoord.w + 0.0001);
	if (texW == 63)
	{
		gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
		return;
	}
	
	vec2 coord2d;
	float intensity;
	float v = int(texW / 8.0);

	// If the texture index is negative, it is a top or bottom face, otherwise a side face
	// Side faces are less bright than top faces, simulating a sun at noon
	if(texcoord.w < 0.0) {
		coord2d = vec2((fract(texcoord.x) + get_u(texcoord.w)) / 8.0, (get_z(texcoord.z) + v) / 8.0);
		intensity = 1.0;
	} else {
		coord2d = vec2((fract(texcoord.x + texcoord.z) + get_u(texcoord.w)) / 8.0,  (get_y(texcoord.y) + v) / 8.0);
		intensity = 0.85;
	}
	
	vec4 color = texture2D(texture, coord2d);

	// Very cheap "transparency": don't draw pixels with a low alpha value
	if(color.a < 0.4)
		discard;

	// Attenuate sides of blocks
	color.xyz *= intensity;

	// Calculate strength of fog
	float z = gl_FragCoord.z / gl_FragCoord.w;
	float fog = clamp(exp(-fogdensity * z * z), 0.2, 1.0);

	// Final color is a mix of the actual color and the fog color
	gl_FragColor = mix(fogcolor, color, fog);
}
