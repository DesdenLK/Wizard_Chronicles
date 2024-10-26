#version 330

uniform mat4 projection, modelview;
uniform vec2 texCoordDispl;

in vec2 position;
in vec2 texCoord;
in int tilesetID; //should be int
out vec2 texCoordFrag;
out int fragTilesetID;  // Pass the tilesetID

void main()
{
	// Pass texture coordinates to access a given texture atlas
	texCoordFrag = texCoord + texCoordDispl;
	fragTilesetID = tilesetID;
	// Transform position from pixel coordinates to clipping coordinates
	gl_Position = projection * modelview * vec4(position, 0.0, 1.0);
}

