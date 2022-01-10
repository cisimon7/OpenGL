#shader vertex
#version 330
uniform mat4 u_MVP;
layout (location = 0) in vec3 vCol;
layout (location = 1) in vec2 vPos;
out vec3 vColor;
void main()
{
    gl_Position = u_MVP * vec4(vPos, 0.0, 1.0);
    vColor = vCol;
}

#shader fragment
#version 330
in vec3 vColor;
out vec4 color;
void main()
{
    color = vec4(vColor, 1.0);
}