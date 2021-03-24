#version 450 core

const int NONE = 0;
const int DIRECTIONAL_LIGHT = 1;
const int POINT_LIGHT = 2;
const int SPOT_LIGHT = 3;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float shininess; //Shininess is the power the specular light is raised to
};

struct Light
{
    int type;

    vec3 position;      // Point,Spot light
    vec3 direction;     // Directional,Spot light
    float cutOff;       // Spot light
    float outerCutOff;  // Spot light

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // Point,Spot light attenuation
    float constant;
    float linear;
    float quadratic;
};

in vec3 Normal;
in vec3 FragPos;
in vec3 FragPosModel;
in vec3 ourColor;

//uniform Material_Basic material;
uniform vec3 viewPos;

out vec4 FragColor;

vec3 CalcDirLight(Light light, Material material);
vec3 CalcPointLight(Light light, Material material);
vec3 CalcSpotLight(Light light, Material material);
vec3 Ambient(Light light, Material material);
vec3 Diffuse(Light light, Material material);
vec3 Specular(Light light, Material material);
float Attenuation(Light light);
float Intensity(Light light);

void main()
{
    Material material;
    Light dirLight;
    Light spotLight;
    Light pointLights[3];
    
    // Material
    {
        material.ambient = vec3(0.0f, 0.03f, 0.1f);
        //material.diffuse = vec3(1.0f, 0.5f, 0.31f);
        material.diffuse = ourColor;
        material.specular = vec3(0.5f, 0.5f, 0.5f);
        material.shininess = 64;
    }

    // Lights
    {
        // Directional light
        dirLight.type = DIRECTIONAL_LIGHT;
        dirLight.direction = vec3(-1.0f, 0.0f, -1.0f);
        dirLight.ambient = vec3(0.05f, 0.05f, 0.05f);
        dirLight.diffuse = vec3(0.4f, 0.4f, 0.4f);
        dirLight.specular = vec3(0.5f, 0.5f, 0.5f);

        // Point light 1
        pointLights[0].type = POINT_LIGHT;
        pointLights[0].position = vec3(5.0f, 5.0f, 5.0f);
        pointLights[0].ambient = vec3(0.05f, 0.05f, 0.05f);
        pointLights[0].diffuse = vec3(1.0f, 1.0f, 1.0f);
        pointLights[0].specular = vec3(1.0f, 1.0f, 1.0f);
        pointLights[0].constant = 1.0;
        pointLights[0].linear = .09;
        pointLights[0].quadratic = .032;

        // Point light 2
        pointLights[1].type = POINT_LIGHT;
        pointLights[1].position = vec3(-5.0f, -5.0f, -5.0f);
        pointLights[1].ambient = vec3(0.05f, 0.05f, 0.05f);
        pointLights[1].diffuse = vec3(1.0f, 1.0f, 1.0f);
        pointLights[1].specular = vec3(1.0f, 1.0f, 1.0f);
        pointLights[1].constant = 1.0;
        pointLights[1].linear = .09;
        pointLights[1].quadratic = .032;

        // Point light 3
        pointLights[2].type = POINT_LIGHT;
        pointLights[2].position = vec3(0.0f, 1.0f, -3.0f);
        pointLights[2].ambient = vec3(0.05f, 0.05f, 0.05f);
        pointLights[2].diffuse = vec3(1.0f, 1.0f, 1.0f);
        pointLights[2].specular = vec3(1.0f, 1.0f, 1.0f);
        pointLights[2].constant = 1.0;
        pointLights[2].linear = .09;
        pointLights[2].quadratic = .032;

        // Spot light
    }

    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // phase 1: directional lighting
    vec3 result = vec3(0);
    //vec3 result = CalcDirLight(dirLight, material);
    // phase 2: point lights
    for(int i = 0; i < 3; i++)
        result += CalcPointLight(pointLights[i], material);    
    // phase 3: spot light
    //result += CalcSpotLight(spotLight, material);
    //vec3 result = CalcSpotLight(spotLight, material);    
    
    //FragColor = vec4(vec3(1), 1.0);
    FragColor = vec4(result, 1.0);
    //FragColor = vec4(vec3(abs(norm.x), abs(norm.y), abs(norm.z)), 1.0);
}


vec3 CalcDirLight(Light light, Material material)
{
    return Ambient(light, material) + Diffuse(light, material) + Specular(light, material);
}

vec3 CalcPointLight(Light light, Material material)
{
    return (Ambient(light, material) + Diffuse(light, material) + Specular(light, material)) * Attenuation(light);
}

vec3 CalcSpotLight(Light light, Material material)
{
    return (Ambient(light, material) + (Diffuse(light, material) + Specular(light, material)) * Attenuation(light)) * Intensity(light);
}

vec3 Ambient(Light light, Material material)
{
    //The ambient color is the color where the light does not directly hit the object.
    //You can think of it as an underlying tone throughout the object. Or the light coming from the scene/the sky (not the sun).
    vec3 ambient = light.ambient * material.ambient;

    return ambient;
}

vec3 Diffuse(Light light, Material material)
{
    //We calculate the light direction, and make sure the normal is normalized.
    vec3 norm = normalize(Normal);
    vec3 lightDir;

    if (light.type == DIRECTIONAL_LIGHT)
        lightDir = normalize(-light.direction);
    else
        lightDir = normalize(light.position - FragPos);

    // Handles two-sided shading
    //if (dot(lightDir,norm) < 0)
        //norm = -norm;

    //Diffuse is the part of the light that gives the most, it is the color of the object where it is hit by light.
    float diff = max(dot(norm, lightDir), 0.0); //We make sure the value is non negative with the max function.
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    return diffuse;
}

vec3 Specular(Light light, Material material)
{
    //We calculate the light direction, and make sure the normal is normalized.
    vec3 norm = normalize(Normal);
    vec3 lightDir;

    if (light.type == DIRECTIONAL_LIGHT)
        lightDir = normalize(-light.direction);
    else
        lightDir = normalize(light.position - FragPos);
        
    // Handles two-sided shading
    //if (dot(lightDir,norm) < 0)
        //norm = -norm;

    //The specular light is the light that shines from the object, like light hitting metal.
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //vec3 specular = light.specular * (spec * material.specular);
    //vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    // only show specular on the front side
    vec3 specular = ((dot(lightDir,norm) < 0) ? vec3(1) : light.specular) * spec;
    
    return specular;
}

float Attenuation(Light light)
{
    //The attenuation is the term we use when talking about how dim the light gets over distance
    //float distance = 1;
    float dist = length(light.position - FragPos);
    //This formula is the so called attenuation formula used to calculate the attenuation
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    return attenuation;
}

float Intensity(Light light)
{
    vec3 lightDir = normalize(light.position - FragPos);

    //This is how we calculate the spotlight, for a more in depth explanation of how this works. Check out the web tutorials.
    float theta     = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); //The intensity, is the lights intensity on a given fragment,
                                                                                //this is used to make the smooth border.

    return intensity;
}