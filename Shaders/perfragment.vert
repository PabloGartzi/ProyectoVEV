#version 120

attribute vec3 v_position;
attribute vec3 v_normal;
attribute vec2 v_texCoord;

uniform int active_lights_n; // Number of active lights (< MG_MAX_LIGHT)

uniform mat4 modelToCameraMatrix;
uniform mat4 cameraToClipMatrix;
uniform mat4 modelToWorldMatrix;
uniform mat4 modelToClipMatrix;

varying vec3 f_position;      // camera space
varying vec3 f_viewDirection; // camera space
varying vec3 f_normal;        // camera space
varying vec2 f_texCoord;

void main() {
	f_position = vec3(modelToCameraMatrix * vec4(v_position, 1.0));
	f_normal = vec3(modelToCameraMatrix * vec4(v_normal, 0.0));
	f_viewDirection = -f_position;
	f_texCoord = v_texCoord;
	gl_Position = modelToClipMatrix * vec4(v_position, 1.0);

}
