#version 330 core

in vec3 FragPos;        // Позиция фрагмента в мировом пространстве
in vec3 Normal;         // Нормаль фрагмента

in vec4 a_color;

out vec4 f_color;

// Структура для хранения параметров света
struct Light {
    vec3 position;  
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

uniform Light light;
uniform vec3 viewPos;

// Материал объекта
struct Material {
    vec3 ambient;       // Окружающая составляющая освещения
    vec3 diffuse;       // Диффузная составляющая освещения
    vec3 specular;      // Зеркальная составляющая освещения
    float shininess;    // Коэффициент блеска
};

uniform Material material;  // Параметры материала объекта

void main() {
    // // ambient
    // vec3 ambient = light.ambient * material.ambient;
    
    // // diffuse 
    // vec3 norm = normalize(Normal);
    // vec3 lightDir = normalize(light.position - FragPos);
    // float diff = max(dot(norm, lightDir), 0.0);
    // vec3 diffuse = light.diffuse * diff * material.diffuse;  
    
    // // specular
    // vec3 viewDir = normalize(viewPos - FragPos);
    // vec3 reflectDir = reflect(-lightDir, norm);  
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // vec3 specular = light.specular * spec * material.specular;
    
    // spotlight (soft edges)
    // float theta = dot(lightDir, normalize(-light.direction)); 
    // float epsilon = (light.cutOff - light.outerCutOff);
    // float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // diffuse  *= intensity;
    // specular *= intensity;
    
    // // attenuation
    // float distance    = length(light.position - FragPos);
    // float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // ambient  *= attenuation; 
    // diffuse   *= attenuation;
    // specular *= attenuation;   
        
    // vec3 result = (ambient + diffuse + specular) * a_color.rgb;
    // f_color = vec4(result, 1.0);

     // Расчет направления света
    // vec3 lightDir = normalize(light.position - FragPos);
    // vec3 norm = normalize(Normal);
    // // vec3 dir = reflect(-lightDir, norm);
    
    // // Расчет интенсивности диффузного освещения
    // float diff = max(dot(norm, lightDir), 0.0);
    
    // // Учет цвета света и диффузной составляющей
    // vec3 result = vec3(1.0f) * diff * a_color.rgb;
    
    // Установка цвета фрагмента с учетом освещенности
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    lightDir *= 0.1f;
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(2.0f);
    vec3 result = (diffuse) * a_color.rgb;
    f_color = vec4(result, 1.0f);
}