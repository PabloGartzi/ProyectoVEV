#version 120

varying vec4 f_color;
varying vec2 f_texCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform float uCloudOffset; // The offset of the cloud texture

void main() {

	// The final color must be a linear combination of both
	// textures with a factor of 0.5, e.g:
	//
	vec4 color_textura = texture2D(texture0, f_texCoord);
	vec2 nuevo_f_texCoord = vec2(f_texCoord.x+uCloudOffset,f_texCoord.y);
	vec4 color_textura1 = texture2D(texture1, nuevo_f_texCoord);
	vec4 color_textura_final = 0.5 * color_textura + 0.5 * color_textura1;
	gl_FragColor = f_color * color_textura_final;

}

