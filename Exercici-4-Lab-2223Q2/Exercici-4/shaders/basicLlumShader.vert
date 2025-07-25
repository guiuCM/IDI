#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3  matamb;
in vec3  matdiff;
in vec3  matspec;
in float matshin;

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;

out vec3  fmatamb;
out vec3  fmatdiff;
out vec3  fmatspec;
out float fmatshin;
out vec3  fvertex;
out vec3  fnormal;

//nou codi


mat4 vertexSCO;

uniform vec3 colFocus;


void main()
{	
    // Passem les dades al fragment shader
    fmatamb  = matamb;
    fmatdiff = matdiff;
    fmatspec = matspec;
    fmatshin = matshin;

    mat3 NormalMatrix = inverse(transpose(mat3(View*TG)));
    //normalSCO = (NM * normal);
    //vertexSCO = View * TG * vec4 (vertex, 1.0);
    

    fvertex = (View * TG * vec4 (vertex, 1.0)).xyz;
    fnormal = (NormalMatrix * normal);


    gl_Position = Proj * View * TG * vec4 (vertex, 1.0);
}
