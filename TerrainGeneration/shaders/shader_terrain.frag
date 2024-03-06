#version 430 core

uniform vec3 lightColor;
uniform vec3 lightDir;
uniform vec3 cameraPos;
uniform sampler2D colorTexture;

in vec3 Color;
in vec3 worldPosition;
in vec3 Normal;
in vec2 Tex;
in vec3 localPosition;

out vec4 outColor;

void main()
{
    vec4 textureColor texture(colorTexture, Tex);

    vec3 vertexNormal = normalize(Normal);
    
    float ambientIntensity = 0.0f;
    vec4 AmbientColor = vec4(lightColor, 1.0f) * ambientIntensity;
    
    float diffuseIntensity = 1.0f;
    float DiffuseFactor = dot(-lightDir, vertexNormal);
    
    vec4 DiffuseColor = vec4(0, 0, 0, 0);
    vec4 SpecularColor = vec4(0, 0, 0, 0);
    
    if (DiffuseFactor > 0){
        DiffuseColor = vec4(lightColor, 1.0f) *
        diffuseIntensity *
        DiffuseFactor;
        
        vec3 PixelToCamera = normalize(cameraPos - worldPosition);
        vec3 LightReflection = normalize(reflect(lightDir, vertexNormal));
        float SpecularFactor = dot(PixelToCamera, LightReflection);
        if (SpecularFactor > 0){
            float SpecularExponent = 100;
            SpecularFactor = pow(SpecularFactor, SpecularExponent);
            SpecularColor = vec4(lightColor, 1.0f) * SpecularFactor;
        }
    }

    outColor = textureColor * clamp(AmbientColor + DiffuseColor + SpecularColor, 0, 1);
}