#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	// 0.2 is the mix ratio. It is 80% of first texture and 20% of second texture
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}