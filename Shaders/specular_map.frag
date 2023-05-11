#version 120

uniform int active_lights_n; // Number of active lights (< MG_MAX_LIGHT)
uniform vec3 scene_ambient; // Scene ambient light

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

uniform sampler2D texture0;
uniform sampler2D specmap;    // specular map

varying vec3 f_position;      // camera space
varying vec3 f_viewDirection; // camera space
varying vec3 f_normal;        // camera space
varying vec2 f_texCoord;

float lambertFactor (in vec3 N, in vec3 L){
	float NoL = dot(N, L);
	NoL = max(NoL, 0.0);
	return NoL;
}

void main() {
	
	vec3 L; 
	vec4 posEye4;
	vec4 normalEye4;
	vec3 difuso = vec3(0.0);
	vec3 especular = vec3(0.0); 



	float aconst, alin, aquad, fdist, aux, distancia= 0;
	vec4 v4;

	posEye4 = vec4(f_position,1.0);

	normalEye4 = vec4(normalize(f_normal),1.0);

	vec3 normalEye = normalize(normalEye4.xyz);

	v4 = vec4(normalize(f_viewDirection),1.0);
	vec3 V = normalize(v4.xyz);
	vec3 R;

	for(int i=0; i<active_lights_n; i++){
		if (theLights[i].position.w == 0.0){			//DIRECCIONAL
			L= -theLights[i].position.xyz;
			L= normalize(L);
					
			difuso += lambertFactor(normalEye,L)*theMaterial.diffuse*theLights[i].diffuse; //DIFUSO

			R=(2*dot(normalEye,L))*normalEye-L; //ESPECULAR
			R= normalize(R);
			aux = dot(R,V);
			if(aux > 0.0){
			especular = especular + lambertFactor(normalEye,L)*pow(aux, theMaterial.shininess)*theMaterial.specular*theLights[i].specular;
			}

		}
		else{											//SPOTLIGHT O POSICIONAL
			L= theLights[i].position.xyz - posEye4.xyz;
			distancia = length(L);
			L= normalize(L);
			
			//ATENUACION
			aconst = theLights[i].attenuation[0]; 		
			alin = theLights[i].attenuation[1];
			aquad = theLights[i].attenuation[2];
			if(aconst + alin*(distancia) + aquad*(distancia*distancia) > 0.0){
				fdist = 1.0 /(aconst + alin*(distancia) + aquad*(distancia*distancia));
			}
			else{
				fdist = 1.0;
			}

			if (theLights[i].cosCutOff > 0.0){			//SPOTLIGHT
				vec3 D = normalize(theLights[i].spotDir);

				float spot_cos = max(0.0, dot(-L, D)); // spot_cos no puede ser negativo
				float spot_factor = 0.0;
				if (spot_cos >= theLights[i].cosCutOff) { // está dentro?
					spot_factor = pow(spot_cos, theLights[i].exponent);
				}

				if (spot_factor > 0.0) {
					difuso += lambertFactor(normalEye,L)*theMaterial.diffuse*theLights[i].diffuse*fdist*spot_factor; //DIFUSO

					R=(2*dot(normalEye,L))*normalEye-L; //ESPECULAR
					R= normalize(R);
					aux = dot(R,V);
					if(aux > 0.0){
						if (theLights[i].cosCutOff > 0.0){
							especular = especular + lambertFactor(normalEye,L)*pow(aux, theMaterial.shininess)*theLights[i].specular*fdist*spot_factor;
						}
					}
				}
			}
			else{									//POSICIONAL
				difuso += lambertFactor(normalEye,L)*theMaterial.diffuse*theLights[i].diffuse*fdist; //DIFUSO

				R=(2*dot(normalEye,L))*normalEye-L; //ESPECULAR
				R= normalize(R);
				aux = dot(R,V);
				if(aux > 0.0){
					if (theLights[i].cosCutOff > 0.0){
						especular = especular + lambertFactor(normalEye,L)*pow(aux, theMaterial.shininess)*theLights[i].specular*fdist;
					}
				}
			}
		}
	}

	vec4 color_textura = texture2D(texture0, f_texCoord);

	//specmap es de donde sacamos (hablando un poco en plata) como brillara la parte metálica de la caja.
	vec4 spec_map = texture2D(specmap, f_texCoord);
	

	gl_FragColor = vec4(scene_ambient + difuso, 1.0)*color_textura + vec4(especular, 1.0)*spec_map;
}
