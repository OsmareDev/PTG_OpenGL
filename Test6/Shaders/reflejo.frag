#version 150

uniform sampler2D uStoneTex; 
uniform sampler2D uMossTex;
uniform samplerCube uTexMap; 
uniform int uDrawSky;

in vec2 vTexCoord;
in vec3 vTexCoordReflect;
out vec4 fFragColor;

void main()
{	
	fFragColor = texture( uTexMap, vTexCoordReflect );
}
