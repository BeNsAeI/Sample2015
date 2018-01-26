#version 130
uniform float uDist, uMultR, uMultG, uMultB; // coefficients of each type of lighting

in vec4 color;
void
main( )
{
	vec3 myColor;

	float Time = 1;
	myColor.r = (color.r+2*abs(sin(1000*Time*uDist))*fract(sin(dot(vec2(color.r*abs(sin(1000*Time*uDist)),color.r*abs(sin(1000*Time*uDist))),vec2(12.9898,78.233)))*43758.5453))/uMultR;
	myColor.g = (color.g+2*abs(sin(1000*Time*uDist))*fract(sin(dot(vec2(color.g*abs(sin(1000*Time*uDist)),color.g*abs(sin(1000*Time*uDist))),vec2(12.9898,78.233)))*43758.5453))/uMultG;
	myColor.b = (color.b+2*abs(sin(1000*Time*uDist))*fract(sin(dot(vec2(color.b*abs(sin(1000*Time*uDist)),color.b*abs(sin(1000*Time*uDist))),vec2(12.9898,78.233)))*43758.5453))/uMultB;

	gl_FragColor = vec4( myColor, 1. );
}
