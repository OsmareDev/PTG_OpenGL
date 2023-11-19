#version 150

uniform sampler2D uStoneTex; 
uniform sampler2D uMossTex;
uniform samplerCube uTexMap; 
uniform int uDrawSky;

in vec2 vTexCoord;
out vec4 fFragColor;

void main()
{	
	vec4 stoneTex = texture( uStoneTex, vTexCoord );
	vec4 mossTex = texture( uMossTex, vTexCoord );
	fFragColor = vec4(mix(stoneTex.rgb, mossTex.rgb, mossTex.a), 1);
	//fFragColor = stoneTex;
}
