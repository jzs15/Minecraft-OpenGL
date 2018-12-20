#version 330 core
out vec4 FragColor;

in vec3 TexCoords;
in float TexType;
uniform float timeValue;
uniform samplerCube skybox;

bool checkType(int v)
{
	float bias = 0.001;
	return TexType >= v - bias && TexType <= v + bias;
}

void main()
{
	vec3 TexCoords3d = TexCoords;
	if(checkType(1))
		TexCoords3d = vec3(-timeValue, TexCoords.y * 0.5, TexCoords.z);
	else if(checkType(2))
		TexCoords3d = vec3(TexCoords.x, TexCoords.y * 0.5, timeValue);
	else if(checkType(3))
		TexCoords3d = vec3(TexCoords.x, TexCoords.y * 0.5, -timeValue);
	else if(checkType(4))
		TexCoords3d = vec3(timeValue, TexCoords.y * 0.5, TexCoords.z);
	else if(checkType(5)) // 1
		TexCoords3d = vec3(timeValue, TexCoords.y,  - TexCoords.z * 0.5 - 1);
	else if(checkType(6)) // 3
		TexCoords3d = vec3(timeValue, TexCoords.y,  TexCoords.x * 0.5 - 1);
	else if(checkType(7)) // 4
		TexCoords3d = vec3(timeValue, TexCoords.y,  TexCoords.z * 0.5 - 1);
	else if(checkType(8)) // 2
		TexCoords3d = vec3(timeValue, TexCoords.y,  - TexCoords.x * 0.5 - 1);

    FragColor = texture(skybox, TexCoords3d);
	//FragColor = texture(skybox, TexCoords);;
}