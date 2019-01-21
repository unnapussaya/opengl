#version 330


layout (triangles) in;
layout (triangle_strip) out;
layout (max_vertices = 3) out;

uniform mat4 mvMatrix;
uniform mat4 norMatrix;
uniform vec4 lightPos;
uniform mat4 mvpMatrix;

out vec4 colfrag;

void main(void)
{
	int i;
	vec4 white = vec4(1.0);
	vec4 grey = vec4(0.2);
	vec4 material = vec4(0.5, 0.8, 0.8, 1.0);
	vec4 ambOut = grey * material;
	float shininess = 100.0;

	//calculate the transformed face normal
	vec4 v1 = gl_in[2].gl_Position - gl_in[1].gl_Position;
	vec4 v2 = gl_in[0].gl_Position - gl_in[1].gl_Position;
	vec3 normal = normalize(cross(v1.xyz, v2.xyz));


	vec4 pos, posnEye, normalEye, lgtVec, viewVec, halfVec, diffOut, specOut, tes_color;
	float diffTerm, specTerm;


    for (i = 0; i < gl_in.length(); i++)
    {

		pos = gl_in[i].gl_Position; 

		posnEye = mvMatrix * pos;
		normalEye = norMatrix * vec4(normal, 0);
		lgtVec = normalize(lightPos - posnEye); 
		viewVec = normalize(vec4(-posnEye.xyz, 0)); 
		halfVec = normalize(lgtVec + viewVec); 

		diffTerm = max(dot(lgtVec, normalEye), 0);
		diffOut = material * diffTerm;
		specTerm = max(dot(halfVec, normalEye), 0);
		specOut = white *  pow(specTerm, shininess);

		tes_color = ambOut + diffOut + specOut;

	  
		gl_Position = mvpMatrix*gl_in[i].gl_Position;
		colfrag=tes_color;
		EmitVertex();
		
	}
    EndPrimitive();
    
}
