#version 130
uniform float uMultR, uMultG, uMultB; // coefficients of each type of lighting
uniform float uKa, uKd, uKs; // coefficients of each type of lighting
uniform float uShininess; // specular exponent

in vec2 vST; // texture cords
in vec3 vN; // normal vector
in vec3 vL; // vector from point to light
in vec3 vE; // vector from point to eye
in vec4 color;
void
main( )
{
	vec3 myColor;

	float Time = 1;
	myColor.r = (color.r+2*abs(sin(1000))*fract(sin(dot(vec2(color.r*abs(sin(1000)),color.r*abs(sin(1000))),vec2(12.9898,78.233)))*43758.5453))/uMultR;//color.r;//
	myColor.g = (color.g+2*abs(sin(1000))*fract(sin(dot(vec2(color.g*abs(sin(1000)),color.g*abs(sin(1000))),vec2(12.9898,78.233)))*43758.5453))/uMultG;//color.g;//
	myColor.b = (color.b+2*abs(sin(1000))*fract(sin(dot(vec2(color.b*abs(sin(1000)),color.b*abs(sin(1000))),vec2(12.9898,78.233)))*43758.5453))/uMultB;//color.b;//

	vec3 Normal = normalize(vN);
	vec3 Light = normalize(vL);
	vec3 Eye = normalize(vE);
	vec3 ambient = uKa * myColor;
	float d = max( dot(Normal,Light), 0. ); // only do diffuse if the light can see the point
	vec3 diffuse = uKd * d * myColor;
	float s = 0.;
	if( dot(Normal,Light) > 0. ) // only do specular if the light can see the point
	{
		vec3 ref = normalize( reflect( -Light, Normal ) );
		s = pow( max( dot(Eye,ref),0. ), uShininess );
	}
	vec3 specular = uKs * s * vec3(1,1,1);
	gl_FragColor = vec4( ambient + diffuse + specular, 1);
}
