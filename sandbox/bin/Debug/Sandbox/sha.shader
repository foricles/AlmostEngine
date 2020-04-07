#_alm_vertex_#
#version 350

vec2 positions[3] = vec2[](
    vec2(0.0f, -0.5f),
    vec2(0.5f, 0.5f),
    vec2(-0.5f, 0.5f)
);

void main() {
    gl_Position = vec4(positions[gl_VertexIndex], 0.0f, 1.0f);
}


#_alm_fragment_#
#version 350

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}