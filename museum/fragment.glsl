#version 330

out vec4 final_color;

in vec3 VS_normal_ws;
in vec3 VS_position_ws;
in vec2 VS_tex_coord;

uniform vec3 material_ambient_color;
uniform vec3 material_diffuse_color;
uniform vec3 material_specular_color;
uniform float material_shininess;

uniform vec4 light_position;
uniform vec3 light_ambient_color;
uniform vec3 light_diffuse_color;
uniform vec3 light_specular_color;

uniform vec3 eye_position;

uniform sampler2D wall_tex;

void main()
{
    vec3 wood_color = texture(wall_tex, VS_tex_coord).rgb;

    vec3 tex_color = wood_color;
    vec3 N = normalize(VS_normal_ws);
    vec3 Eye = normalize(eye_position - VS_position_ws);

    vec3 L = normalize(light_position.xyz - VS_position_ws * light_position.w);

    vec3 H = normalize(L + Eye);

    float Idiff = max(dot(N, L), 0.0);
    float Ispec = pow(max(dot(N, H), 0.0), material_shininess) * Idiff;

    // prenasobujeme tex_color, spekularna zlozka sa neprenasobuje
    vec3 mat_ambient = material_ambient_color * tex_color;
    vec3 mat_diffuse = material_diffuse_color * tex_color;
    vec3 mat_specular = material_specular_color;

    vec3 light =
        mat_ambient * light_ambient_color +
        mat_diffuse * light_diffuse_color * Idiff +
        mat_specular * light_specular_color * Ispec;

    final_color = vec4(light, 1.0);
}
