#version 330 core
in vec4 texcoord;
in float textype;
in vec3 FragPos;
in float Normal;
uniform sampler2D blockTexture;
uniform samplerCube depthMap;
uniform float timeValue;

uniform vec3 viewPos;

uniform float far_plane;

uniform float lightNum;
uniform vec3 pointLights[1000];
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

bool checkType(int v)
{
	float bias = 0.001;
	return Normal >= v - bias && Normal <= v + bias;
}

vec3 getNormal()
{
	if(checkType(1))
		return vec3(-1.0, 0.0, 0.0);
	else if(checkType(2))
		return vec3(1.0, 0.0, 0.0);
	else if(checkType(3))
		return vec3(0.0, -1.0, 0.0);
	else if(checkType(4))
		return vec3(0.0, 1.0, 0.0);
	else if(checkType(5))
		return vec3(0.0, 0.0, -1.0);
	else if(checkType(6))
		return vec3(0.0, 0.0, 1.0);
	 vec3(0.0, 0.0, 0.0);
}


float CalcPointLight(vec3 light, vec3 normal)
{
	float distance = length(light - FragPos);
	vec3 lightDir = normalize(light - FragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	float ambient = 0.7f;
    float diffuse = 0.2f * diff;
	float attenuation = 1.0 / (1.0 + 0.32 * (distance * distance));
    return (ambient + diffuse) * attenuation;
}

vec3 getColor()
{
	float result = 0.0;
	vec3 norm = getNormal();
	for(int i = 0; i < lightNum; i++)
		result += CalcPointLight(pointLights[i], norm);
	return vec3(min(1.0, result));
}


void main(void) {
	int texW = int(texcoord.w + 0.0001);
	if (texW == 63)
	{
		gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
		return;
	}
	
	vec3 lightColor =  vec3(1 - abs(timeValue) * 0.8);
	
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
	vec3 nColor = getColor();
	float z = gl_FragCoord.z / gl_FragCoord.w;
	float fog = clamp(exp(-fogdensity * z * z), 0.2, 1.0);
	color = mix(fogcolor, color, fog);
	color.rgb *= max(lightColor, nColor);
	// Final color is a mix of the actual color and the fog color
	gl_FragColor = vec4(color.rgb, color.a);
}