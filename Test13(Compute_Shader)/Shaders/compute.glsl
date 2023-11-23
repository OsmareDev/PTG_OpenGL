#version 430  

layout( std140, binding=0 ) buffer Pos {
   vec4 Positions[ ]; 
};
layout( std140, binding=1 ) buffer Vel {
   vec4 Velocities[ ]; 
};

// define the size of the local workgroup.
layout( local_size_x = 1024, local_size_y = 1, local_size_z = 1 ) in;

// the dt could be passed according to the fps as a uniform
const float dt = 0.0005;
const float tamCube = 1.0; // The cube that encompasses the particles x,y,z = [-tamCube, tamCube]
const vec3 g = vec3( 0.0, -9.8, 0.0 );

layout(location = 1) uniform float uAmortig;


void main()
{
	// get thread id (particle)
	uint gid = gl_GlobalInvocationID.x;

	// read your position and speed from the buffers
	vec3 p = Positions[gid].xyz;
	vec3 v = Velocities[gid].xyz;

	vec3 newpos = p + v * dt + 0.5 * dt * dt * g;
	vec3 newvel = v + g * dt;
	
	//	cube
	if (newpos.x > tamCube)  { newvel = reflect(v, vec3(-1,0,0)) * (1.0-uAmortig);   newpos = vec3(tamCube,  newpos.y, newpos.z); }
	if (newpos.x < -tamCube) { newvel = reflect(v, vec3(1,0,0))  * (1.0-uAmortig);   newpos = vec3(-tamCube, newpos.y, newpos.z); }
	if (newpos.y > tamCube)  { newvel = reflect(v, vec3(0,-1,0)) * (1.0-uAmortig);   newpos = vec3(newpos.x, tamCube,  newpos.z); }
	if (newpos.y < -tamCube) { newvel = reflect(v, vec3(0,1,0))  * (1.0-uAmortig);   newpos = vec3(newpos.x, -tamCube, newpos.z); }
	if (newpos.z > tamCube)  { newvel = reflect(v, vec3(0,0,-1)) * (1.0-uAmortig);   newpos = vec3(newpos.x, newpos.y, tamCube ); }
	if (newpos.z < -tamCube) { newvel = reflect(v, vec3(0,0,1))  * (1.0-uAmortig);   newpos = vec3(newpos.x, newpos.y, -tamCube); }

	//  sphere
//	if (length(newpos) > tamCube) { newpos = normalize(newpos); newvel = reflect(v, -newpos) * (1.0-uAmortig); } 

	// Update buffer values.
	Positions[gid].xyz = newpos;
	Velocities[gid].xyz = newvel;
}