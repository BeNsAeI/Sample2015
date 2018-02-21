#version 330 compatibility
uniform float uKa, uKd, uKs,uAd,uBd, uTol,uNoiseFreq,uNoiseAmp,uAlpha,uTime; // coefficients of each type of lighting
uniform float uShininess; // specular exponent
uniform sampler3D Noise3;

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
in vec3 gMCposition;
layout(location = 0) out vec3 color;

void
main( )
{
	vec3 camoColor = vec3(gColor.r/8,gColor.g/4,gColor.b/4);
	//vec3 camoColor = vec3(0,0,0);
	vec3 baseColor = vec3(gColor.r*3/4,gColor.g,gColor.b);
	//vec3 baseColor = vec3(1,0,0);
	
	vec3 myColor = baseColor;
	
	float Ar = uAd/2.;
	float Br = uBd/2.;
	int numins = int( gST.s / uAd );
	int numint = int( gST.t / uBd );
	float u_c = numins *uAd + Ar ;
	float v_c = numint *uBd + Br ;
	
	vec4 nv = texture3D( Noise3, uNoiseFreq*gMCposition );
	float n = nv.r + nv.g + nv.b + nv.a;    //  1. -> 3.
	n = n - 2.;
	
	float sc = float(numins) * uAd  +  Ar;
	float ds = gST.s - sc;                   // wrt ellipse center
	float tc = float(numint) * uBd  +  Br;
	float dt = gST.t - tc;                   // wrt ellipse center

	float oldDist = sqrt( ds*ds + dt*dt );
	float newDist = oldDist + uNoiseAmp*n;
	float scale = newDist / oldDist;        // this could be < 1., = 1., or > 1.

	ds *= scale;                            // scale by noise factor
	ds /= Ar;                               // ellipse equation

	dt *= scale;                            // scale by noise factor
	dt /= Br;                               // ellipse equation

	float dist = ds*ds + dt*dt;
	float a=1.;
	float rand = gST.s*abs(sin(1000*uTime*1))*fract(sin(dot(vec2(gST.t*abs(sin(1000*uTime*1)),gST.s*abs(sin(1000*uTime*1))),vec2(12.9898,78.233)))*43758.5453);
	if(dist < 1 - uTol * 2 * rand)
	{
		if (uAlpha==0)
			discard;
		else
			a=uAlpha;
	}
	if(dist < 1 - uTol)
	{
		myColor=camoColor;
	}
	if( abs( dist - 1 ) < uTol )
	{
		float t = smoothstep( 1 - uTol, 1 + uTol, dist );
		myColor = vec3(1,mix( vec4( baseColor, 1. ), vec4( camoColor, 1. ), t ).g,0);
	}
	
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