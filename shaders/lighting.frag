#version 330 compatibility
uniform float uKa, uKd, uKs; // coefficients of each type of lighting
uniform float uShininess; // specular exponent
uniform sampler2D ImageUnit;

/*
in vec2 vST; // texture cords
in vec3 vN; // normal vector
in vec3 vL; // vector from point to light
in vec3 vE; // vector from point to eye
in vec4 vColor;
*/
in vec2 gST; // texture cords
in vec3 gN; // normal vector
in vec3 gL; // vector from point to light
in vec3 gE; // vector from point to eye
in vec4 gColor;

layout(location = 0) out vec3 color;

void
main( )
{
	vec3 myColor;
	myColor.r = gColor.r*3/4;
	myColor.g = gColor.g;
	myColor.b = gColor.b;

	vec3 Normal = normalize(gN);
	vec3 Light = normalize(gL);
	vec3 Eye = normalize(gE);
	vec3 ambient = uKa * myColor;
	float d = max( dot(Normal,Light), 0. ); // only do diffuse if the light can see the point
	vec3 diffuse = uKd * d * myColor;
	float s = 0.;
	if( dot(Normal,Light) > 0. ) // only do specular if the light can see the point
	{
		vec3 ref = normalize( reflect( -Light, Normal ) );
		s = pow( max( dot(Eye,ref),0. ), uShininess );
	}
	vec3 specular = uKs * s * vec3(1,1,1);//uSpecularColor;
	vec3 shaded = ambient + diffuse + specular;
	
	//Toon shading:
	float uMagTol=1;
	float uQuantize=200;
	// Store the buffer to a texture unit
	
	
	gl_FragColor = vec4( shaded, 1 );
}