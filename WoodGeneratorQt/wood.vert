#version 410 core

layout (location = 0) in vec4 uPos;
layout (location = 1) in vec2 ST;

vec3 normal = vec3(0.0,0.0,1.0);

//uniform float uLightX,uLightY,uLightZ;
uniform mat4 normalMatrix;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;

out vec4 vMC;
flat out vec3 Nf;
     out vec3 Ns;
flat out vec3 Lf;
     out vec3 Ls;
flat out vec3 Ef;
     out vec3 Es;

vec3 eyeLightPosition = vec3(0.0,0.0,2.0);

out vec2 TexCoord;

void main()
{

    vMC = uPos;
    Nf = normalize(mat3(normalMatrix) * normal);
    Ns = Nf;
    vec4 ECPosition = View*Model * uPos;
    Lf = eyeLightPosition - ECPosition.xyz;
    Ls = Lf;
    Ef = vec3(0.0,0.0,0.0)-ECPosition.xyz;
    Es = Ef;


    TexCoord = ST;
    gl_Position = Proj*View*Model*uPos;
}
