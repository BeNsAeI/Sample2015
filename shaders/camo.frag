#version 130
uniform float uAlpha,uR1,uG1,uB1,uR2,uG2,uB2; // coefficients of each type of lighting
uniform sampler3D Noise3;
in vec2 vST; // texture cords
in vec3 vMCposition;

void
main( )
{
	float uAd, uBd,uTol,uNoiseFreq,uNoiseAmp;
	uAd = 0.25;
	uBd = 0.25;
	uTol = 0;
	uNoiseFreq = 0.75;
	uNoiseAmp = 0.05;
	vec3 camoColor = vec3(uR1, uG1, uB1);
	vec3 baseColor = vec3(uR2, uG2, uB2);
	vec3 myColor = baseColor;

	float Ar = uAd/2.;
	float Br = uBd/2.;
	int numins = int( vST.s / uAd );
	int numint = int( vST.t / uBd );
	float u_c = numins *uAd + Ar ;
	float v_c = numint *uBd + Br ;
	
	vec4 nv = texture3D( Noise3, uNoiseFreq*vMCposition );
	float n = nv.r + nv.g + nv.b + nv.a;    //  1. -> 3.
	n = n - 2.;                             // -1. -> 1.

	float sc = float(numins) * uAd  +  Ar;
	float ds = vST.s - sc;                   // wrt ellipse center
	float tc = float(numint) * uBd  +  Br;
	float dt = vST.t - tc;                   // wrt ellipse center

	float oldDist = sqrt( ds*ds + dt*dt );
	float newDist = oldDist + uNoiseAmp*n;
	float scale = newDist / oldDist;        // this could be < 1., = 1., or > 1.

	ds *= scale;                            // scale by noise factor
	ds /= Ar;                               // ellipse equation

	dt *= scale;                            // scale by noise factor
	dt /= Br;                               // ellipse equation

	float d = ds*ds + dt*dt;
	float a=1.;
	if(d < 1 + uTol)
	{
		if (uAlpha==0)
			discard;
		else
			a=uAlpha;
	}
	if(d < 1 - uTol)
	{
		myColor=camoColor;
	}
	if( abs( d - 1 ) < uTol )
	{
		float t = smoothstep( 1 - uTol, 1 + uTol, d );
		myColor = vec3(0,mix( vec4( baseColor, 1. ), vec4( camoColor, 1. ), t ).g,1);
	}
	
	gl_FragColor = vec4( myColor, a );
}
