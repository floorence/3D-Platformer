#version 330 core

// parts of this file were adapted from
// * https://github.com/JoeyDeVries/LearnOpenGL/blob/master/src/2.lighting/6.multiple_lights/6.multiple_lights.fs
// * https://github.com/JoeyDeVries/LearnOpenGL/blob/master/src/5.advanced_lighting/3.2.1.point_shadows/3.2.1.point_shadows.fs
// * https://github.com/JoeyDeVries/LearnOpenGL/blob/master/src/5.advanced_lighting/7.bloom/7.bloom.fs

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

// These values must match those in enum class ColorSource in Shader.h!!!
const int COLOR_SOURCE_TEXTURE = 0;
const int COLOR_SOURCE_VERTEX_COLOR = 1;
const int COLOR_SOURCE_MATERIAL_COLOR = 2;

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
in vec3 color;

// These are not part of the material struct since other shaders also have these uniforms and it would be annoying
// to have these have a different uniform name
uniform int colorSource;
uniform vec3 materialColor;

uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;
uniform samplerCube depthMap;

uniform int numPointLights;
uniform vec4 tintColor;
uniform vec3 camPos;
uniform float farPlane;

bool isInShadow(vec3 fragPos, vec3 normal, vec3 lightPos) {
    // calculate bias
    vec3 lightToFrag = fragPos - lightPos;
    vec3 lightDir = normalize(lightPos - fragPos); // direction to light from frag
    float currentDepth = length(lightToFrag);
    // float depthScale = (1.0 / farPlane) * currentDepth; 
    float maxBias = 0.05; float minBias = 0.0005;
    float bias = max(maxBias * (1.0 - dot(normal, lightDir)), minBias);

    // normal offset
    vec3 biasedFragPos = fragPos + normal * bias;
    vec3 biasedLightToFrag = biasedFragPos - lightPos;
    currentDepth = length(biasedLightToFrag);

    // cube texture sampling works by having vector from middle of cube point to where you want to sample
    float closestDepth = texture(depthMap, biasedLightToFrag).r;
    closestDepth *= farPlane; // transform [0,1] back to original depth value

    // test for shadows
    bool shadow = currentDepth > closestDepth;
       
    return shadow;
}

vec3 calculatePointLight(PointLight light, vec3 texColor, vec3 specColor, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse intensity
    float diff = max(dot(normal, lightDir), 0.0);
    // specular intensity
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results with texture
    vec3 diffuse = diff * texColor;
    // handle if specular texture only has red channel but needs to be grey
    vec3 specTex = specColor;
    vec3 greySpecTex = vec3(specTex.r);
    vec3 specular = spec * greySpecTex;
    diffuse *= attenuation;
    specular *= attenuation;
    float shadow = isInShadow(fragPos, normal, light.position) ? 0.0 : 1.0;
    return light.color * shadow * (diffuse + specular);
}

// TODO
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

vec3 getColorFromSource() {
    if (colorSource == COLOR_SOURCE_TEXTURE) {
        return vec3(texture(material.diffuse, texCoord));
    } else if (colorSource == COLOR_SOURCE_MATERIAL_COLOR) {
        return materialColor;
    } else {
        return color;
    }
}

float getBrightness(vec3 color) {
    return dot(color, vec3(0.2126, 0.7152, 0.0722));
}

void main() {
	vec3 normal = normalize(normal);
	vec3 viewDirection = normalize(camPos - crntPos);
    vec3 texColor = getColorFromSource();
    vec3 specColor = (colorSource == COLOR_SOURCE_TEXTURE) ? vec3(texture(material.specular, texCoord)) : vec3(getBrightness(texColor));
    vec3 ambient = AMBIENT_LIGHT * texColor;

    //	vec3 result = calculateSpotLight(spotLight, normal, crntPos, viewDirection);
	vec3 result = vec3(0);

	for (int i = 0; i < numPointLights; i++)
        result += calculatePointLight(pointLights[i], texColor, specColor, normal, crntPos, viewDirection);    

    result += ambient;
	result = mix(result, tintColor.rgb, tintColor.a);
    
    FragColor = vec4(result, 1.0);

    // uncomment to see depthMap
    // vec3 fragToLight = crntPos - pointLights[0].position;
    // float depth = texture(depthMap, fragToLight).r;
    // FragColor = vec4(vec3(depth), 1.0);

    // check whether fragment output is higher than threshold, if so output as brightness color
    float brightness = getBrightness(FragColor.rgb);
    if (brightness > 50.0) {
		// bright bloom blur texture is NOT tone mapped in hdr_bloom.frag, must map to 0-1 here, otherwise blurred areas will be too bright and not look blurred
		float maxColorChannel = max(max(FragColor.r, FragColor.g), FragColor.b);
		vec3 mapped = FragColor.rgb / maxColorChannel;
        BrightColor = vec4(mapped, 1.0);
    } else {
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}