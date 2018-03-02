#version 410 core
out vec4 FragColor;

uniform sampler2D Tex1;
uniform mat4 slice;
uniform float uNoiseAmp;
uniform float uKa;
uniform float uKd;
uniform float uKs;
uniform float shineness;
bool uFlat;

flat in vec3 Nf;
     in vec3 Ns;
flat in vec3 Lf;
     in vec3 Ls;
flat in vec3 Ef;
     in vec3 Es;

in vec2 TexCoord;
in vec4 vMC;
vec4 darkwood = vec4(0.8,0.5,0.1,1.0);
vec4 lightwood = vec4(1.0,0.75,0.25,1.0);


void main()
{
    uFlat = false;
    vec3 Normal;
    vec3 Light;
    vec3 Eye;

    if(uFlat)
    {
        Normal = normalize(Nf);
        Light = normalize(Lf);
        Eye = normalize(Ef);
    }
    else
    {
        Normal = normalize(Ns);
        Light = normalize(Ls);
        Eye = normalize(Es);
    }

    if (!gl_FrontFacing) // is the fragment part of a front face?
    {
        Normal = -Normal;
    }


    vec4 cyl = slice*vec4(TexCoord.st,0.0,1.0);
    float dist = length(cyl.xz);
    vec4 noise = texture(Tex1,TexCoord);

    float shift = noise.r + noise.g + noise.b + noise.a;
    float colorshift = (noise.a - 0.5)/2.0;
    shift *= uNoiseAmp;
    dist+=shift;

    float t = 1.0 - abs(fract(dist)*2.0-1.0);
    t = smoothstep(0.2,0.5,t);
    vec4 color = mix(darkwood+colorshift,lightwood+colorshift,t);
    vec4 ambient = uKa * color;

    float d = max(dot(Normal,Light),0.);
    vec4 diffuse = uKd*d*color;

    float s =0;
    if(dot(Normal,Light)>0.0)
    {
        vec3 ref = normalize(2.0*Normal*dot(Normal,Light)-Light);
        s = pow(max(dot(Eye,ref),0.0),shineness);
    }
    vec4 specularColor = vec4(1.0);
    vec4 specular = uKs*s*specularColor;
    FragColor = vec4(ambient.rgb+diffuse.rgb+specular.rgb,1.0);
}

