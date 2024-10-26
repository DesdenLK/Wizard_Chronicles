#version 330

uniform vec4 color;
uniform sampler2D tex;
uniform sampler2D tex2;
uniform vec2 alpha;

in vec2 texCoordFrag;
in in fragTilesetID;  // Input from the vertex shader
out vec4 outColor;

void main()
{
	// Discard fragment if texture sample has alpha < 0.5
	// otherwise compose the texture sample with the fragment's interpolated color
	if (fragTilesetID == 0)
        texColor = texture(tex, texCoordFrag);
    else
        texColor = texture(tex2, texCoordFrag);
	if(texColor.a < 0.5f)
		discard;
	outColor = color * texColor * alpha[0];
}

