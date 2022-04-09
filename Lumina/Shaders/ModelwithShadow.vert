# version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 model;

layout (std140) uniform Matrices {
    mat4 view;
    mat4 projection;
};

uniform mat4 LightSpaceTransform;

out VS_OUT {
    vec3 normal;
    vec3 viewspace_fragPos;
    vec4 lightspace_fragPos;
    vec2 texCoords;
    mat4 view;
} vs_out;

void main() {
    vs_out.normal = mat3(transpose(inverse(model * view))) * aNormal;
    vs_out.viewspace_fragPos = vec3(view * model * vec4(aPosition, 1.0));
    vs_out.lightspace_fragPos = LightSpaceTransform * vec4(vec3(model * vec4(aPosition, 1.0)), 1.0);
    vs_out.texCoords = aTexCoords;
    vs_out.view = view;

    gl_Position = projection * vec4(vs_out.viewspace_fragPos, 1.0);
}