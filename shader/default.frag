#version 330 core

// this file was adapted from https://github.com/JoeyDeVries/LearnOpenGL/blob/master/src/2.lighting/6.multiple_lights/6.multiple_lights.fs

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct PointLight {
    vec3 position;
    vec3 color;
    
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;

    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
};

#define MAX_POINT_LIGHTS 100
#define AMBIENT_LIGHT 0.2

in vec3 crntPos;
in vec3 normal;
in vec2 texCoord;

uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

uniform int numPointLights;
uniform vec3 tintColor;
uniform float tintIntensity; // 0 to 1
uniform vec3 camPos;

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse intensity
    float diff = max(dot(normal, lightDir), 0.0);
    // specular intensity
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    //float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
	float attenuation = 1.0;
    // combine results with texture
    vec3 ambient = AMBIENT_LIGHT * vec3(texture(material.diffuse, texCoord));
    vec3 diffuse = diff * vec3(texture(material.diffuse, texCoord));
    vec3 specular = spec * vec3(texture(material.specular, texCoord));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return light.color * (ambient + diffuse + specular);
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse intensity
    float diff = max(dot(normal, lightDir), 0.0);
    // specular intensity
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results with texture
    vec3 ambient = AMBIENT_LIGHT * vec3(texture(material.diffuse, texCoord));
    vec3 diffuse = diff * vec3(texture(material.diffuse, texCoord));
    vec3 specular = spec * vec3(texture(material.specular, texCoord));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return light.color * (ambient + diffuse + specular);
}

void main() {
	vec3 normal = normalize(normal);
	vec3 viewDirection = normalize(camPos - crntPos);

//	vec3 result = calculateSpotLight(spotLight, normal, crntPos, viewDirection);
	vec3 result = vec3(0);

	for (int i = 0; i < numPointLights; i++)
        result += calculatePointLight(pointLights[i], normal, crntPos, viewDirection);    

	result = mix(result, tintColor, tintIntensity);
    
    FragColor = vec4(result, 1.0);
}