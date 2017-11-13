#version 330 core


noperspective out vec2 texCoord;

void main()
{
	const vec4 FullScreenQuad[4] = vec4[4]
	(
		vec4(-1, -1, -1,  1), // Bottom Left
		vec4( 1, -1, -1,  1), // Bottom Right
		vec4(-1,  1, -1,  1), // Top left
		vec4( 1,  1, -1,  1)  // Top Right
	);

  const vec2 UVs[4] = vec2[4]
	(
		vec2(0,0), vec2(1,0), vec2(0,1), vec2(1,1)
	);
	
	texCoord = UVs[gl_VertexID];
	gl_Position = FullScreenQuad[gl_VertexID];
}