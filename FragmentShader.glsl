#version 330 core

out vec4 FragColor;
in  vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform mat3 Nmat;
uniform vec3 camPos;
float specularStrength = 0.5;

struct Material {
vec3 ambient;
vec3 diffuse;
vec3 specular;
float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

in vec3 oNormal;
void main()
{
	vec3 Normal = Nmat * normalize(oNormal);
	vec3 lightDir = normalize(light.position - FragPos);
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(camPos - FragPos);

   	vec3 ambient = light.ambient * material.ambient;
	
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);


	// specular
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0),
	material.shininess);
	vec3 specular = light.specular * (spec * material.specular);



	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}
