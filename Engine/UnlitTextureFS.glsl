#version 460 core

struct PointLight
{
	vec4 lightColor;
	
	float ambientStrength;

	float shininess;
	float specularStrength;
	float intensity;

	float attenuation_const;
	float attenuation_linear;
	float attenuation_quadratic;
};

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform PointLight pointLight;

uniform sampler2D tex_diffuse1;
uniform sampler2D tex_diffuse2;
uniform sampler2D tex_specular1;
uniform sampler2D tex_specular2;
uniform sampler2D tex_normal1;
uniform sampler2D tex_normal2;

in vec3 vertexPos;
in vec3 normal;
in vec2 texCoord;
out vec4 color;

//vec3 normal;

vec4 CalculatePointLight(PointLight light, vec3 viewPos, vec3 lightPos, vec3 fragmentPos)
{
	vec4 diffuseTexelColor = texture(tex_diffuse1, texCoord);//vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec4 ambientColor = light.lightColor * light.ambientStrength * diffuseTexelColor;

	float lightDistance = length(lightPos - fragmentPos);

	float attenuation =
		1.0f / (lightDistance * lightDistance * light.attenuation_quadratic + lightDistance * light.attenuation_linear + light.attenuation_const);

	vec3 lightDirection = normalize(lightPos - fragmentPos);
	float diffuseStrength = max(dot(lightDirection, normal), 0.0f);
	vec4 diffuseColor = light.lightColor * diffuseStrength * attenuation * diffuseTexelColor;

	vec3 reflection = reflect(-lightDirection, normal);
	vec3 viewDirection = normalize(viewPos - fragmentPos);
	float specularStrength = pow(max(dot(viewDirection,  reflection), 0.0f), light.shininess);
	vec4 specularColor = light.lightColor * light.specularStrength * specularStrength * /*vec4(1.0f, 1.0f, 1.0f, 1.0f)*/texture(tex_specular1, texCoord);

	return (ambientColor + diffuseColor + specularColor);
}

void main()
{
	//normal = texture(tex_normal1, texCoord).rgb;
	//normal = normalize(normal * 2.0f - 1.0f);
	color =  CalculatePointLight(pointLight, viewPos, lightPos, vertexPos);
}