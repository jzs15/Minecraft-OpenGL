#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float aType;

out vec3 TexCoords;
out float TexType;

uniform mat4 mvp;

void main()
{
    TexCoords = aPos;
	TexType = aType;
    vec4 pos = mvp * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}  