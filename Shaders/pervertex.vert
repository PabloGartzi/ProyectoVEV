#version 120

uniform mat4 modelToCameraMatrix;
uniform mat4 cameraToClipMatrix;
uniform mat4 modelToWorldMatrix;
uniform mat4 modelToClipMatrix;

uniform int active_lights_n; // Number of active lights (< MG_MAX_LIGHT)
uniform vec3 scene_ambient;  // rgb

uniform struct light_t {
	vec4 position;    // Camera space
	vec3 diffuse;     // rgb
	vec3 specular;    // rgb
	vec3 attenuation; // (constant, lineal, quadratic)
	vec3 spotDir;     // Camera space
	float cosCutOff;  // cutOff cosine
	float exponent;
} theLights[4];     // MG_MAX_LIGHTS

uniform struct material_t {
	vec3  diffuse;
	vec3  specular;
	float alpha;
	float shininess;
} theMaterial;

attribute vec3 v_position; // Model space
attribute vec3 v_normal;   // Model space
attribute vec2 v_texCoord;

varying vec4 f_color;
varying vec2 f_texCoord;



//Calcular el color
float lambertFactor (in vec3 N, in vec3 L){
//Multiplica el vector normal de la cara con el vector de la luza para obtener la irradiancia sobre la cara
	float NoL = dot(N, L);
	NoL = max(NoL, 0.0);
	return NoL;
}
/*
Otra forma de hacerlo
void lamberFactor (in vec3 N, in vec3 L, out float Nol){
	NoL  max(dot(N, L), 0.0);
}
*/
/*
void sumaPonderada(in vec3 v1, in vec3 v2, in float factor, inout vec3 suma){
	suma += (v1+v2)*factor
}
*/


void main() {
	
	vec3 L; 
	vec4 posEye4;
	vec4 normalEye4;
	vec3 difuso = vec3(0.0);
	vec3 especular = vec3(0.0); 

	vec4 v4;

	//Pasar la posicion del vertice del sistema de coordenadas del modelo al sistema de coordenadas de la camara
	posEye4 = modelToCameraMatrix * vec4(v_position, 1.0);

	//Pasar vector normal de la cara del sistema de coordenadas del objeto al sistema de coordenadas de la camara
	normalEye4 = modelToCameraMatrix * vec4(v_normal, 0.0);

	vec3 normalEye = normalize(normalEye4.xyz);

	v4 = (0,0,0,1) - posEye4;
	vec3 V = normalize(v4.xyz);
	vec3 R;
	float aux;

	for(int i=0; i<active_lights_n; i++){
		if (theLights[i].position.w == 0.0){
			//Direccional
			L= normalize(-theLights[i].position.xyz);
			R=2*(normalEye*L)*normalEye-L;
			R= normalize(R);
			aux = dot(R,V);

			if(aux > 0.0){
			especular = especular + pow(aux, theLights[i].exponent);
			}

		} else{
			//Posicional o spotlight
			L= normalize(theLights[i].position.xyz - posEye4.xyz);
		}
		
		difuso += lambertFactor(normalEye,L)*theMaterial.diffuse*theLights[i].diffuse;
		
	}
	
	f_color = vec4(scene_ambient + difuso+especular*difuso, 1.0);
		
	f_texCoord = v_texCoord;



	gl_Position = modelToClipMatrix * vec4(v_position, 1);
}
