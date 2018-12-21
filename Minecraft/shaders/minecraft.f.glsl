#version 330 core
in vec4 texcoord;
in float textype;
uniform sampler2D blockTexture;
uniform samplerCube depthMap;
uniform float timeValue;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float far_plane;

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

vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float ShadowCalculation(vec3 fragPos)
{
    vec3 fragToLight = fragPos - lightPos;
    float currentDepth = length(fragToLight);
    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
    return shadow;
}

void main(void) {
	int texW = int(texcoord.w + 0.0001);
	if (texW == 63)
	{
		gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
		return;
	}
	
	vec3 lightColor =  vec3(1 - abs(timeValue) * 0.7);
	
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
	
	vec4 color = texture2D(blockTexture, coord2d);

	// Very cheap "transparency": don't draw pixels with a low alpha value
	if(color.a < 0.4)
		discard;

	// Attenuate sides of blocks
	color.xyz *= intensity;

	// Calculate strength of fog
	float z = gl_FragCoord.z / gl_FragCoord.w;
	float fog = clamp(exp(-fogdensity * z * z), 0.2, 1.0);
	color = mix(fogcolor, color, fog);
	// Final color is a mix of the actual color and the fog color
	gl_FragColor = vec4(color.rgb * lightColor, color.a);
}