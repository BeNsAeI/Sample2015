#version 130
out vec2 vST; // texture coords
out vec3 vMCposition;

void
main( )
{
	vec4 model = gl_Vertex;
	vST = gl_MultiTexCoord0.st;
	vec4 ECposition = gl_ModelViewMatrix * gl_Vertex;
	vMCposition  = gl_Vertex.xyz;
	gl_Position = gl_ModelViewProjectionMatrix * vec4( vMCposition, 1. );
}
