#version 330 core

out vec4 FragColor;

in vec3 crntPos;
in vec3 normal;
in vec2 texCoord;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

uniform vec4 color;
uniform float colorIntensity; // 0 to 1
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

void main() {
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.5f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	vec4 preTintColor = (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor;
	vec3 tintedColor = mix(preTintColor.rgb, color.rgb, colorIntensity);
    
    FragColor = vec4(tintedColor, preTintColor.a);
}