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

// Материал объекта
struct Material {
    vec3 ambient;       // Окружающая составляющая освещения
    vec3 diffuse;       // Диффузная составляющая освещения
    vec3 specular;      // Зеркальная составляющая освещения
    float shininess;    // Коэффициент блеска
};

uniform Light light;
uniform vec3 viewPos;
uniform Material material;  // Параметры материала объекта



void main() {
    f_color = a_color;
    // vec3 norm = normalize(Normal);
    // vec3 lightDir = normalize(light.position - FragPos);

    // // ambient
    // vec3 ambient = light.ambient * material.ambient;

    // // diffuse
    // float diff = max(dot(norm, lightDir), 0.0);
    // vec3 diffuse = light.diffuse * diff * material.diffuse;

    // // specular
    // vec3 viewDir = normalize(viewPos - FragPos);
    // vec3 reflectDir = reflect(-lightDir, norm);  
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // vec3 specular = light.specular * spec * material.specular;

    // // итог
    // vec3 result = (ambient + diffuse + specular) * a_color.rgb;
    // f_color = a_color; //vec4(result, 0.3);
}