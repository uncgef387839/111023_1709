#version 450


layout(location = 0) out vec4 outColor;

layout( push_constant ) uniform constants {
    vec3 color;
} PushConstants;

void main() {
    outColor = vec4(PushConstants.color, 1.0);
}
