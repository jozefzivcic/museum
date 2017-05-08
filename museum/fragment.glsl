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

uniform sampler2D my_tex;

uniform int procedural_tex_type;

// This following two functions permute and snoise are taken from:
// https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83#simplex-noise
// Simplex 2D noise
//
vec3 permute(vec3 x) { return mod(((x*34.0)+1.0)*x, 289.0); }

float snoise(vec2 v){
  const vec4 C = vec4(0.211324865405187, 0.366025403784439,
           -0.577350269189626, 0.024390243902439);
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);
  vec2 i1;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;
  i = mod(i, 289.0);
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
  + i.x + vec3(0.0, i1.x, 1.0 ));
  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy),
    dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;
  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}


// inspired by http://www.tinysg.de/techGuides/tg1_proceduralMarble.html
float turbulence(vec2 point, int frequency)
{
  float val = 0.0;
  float freq = 1.0;
  for (int i=0; i<frequency; i++) {
     val += abs(snoise(point * freq) / freq);
     freq *= 2.07;
  }
  return val;
}

vec3 marble_tex_color(float x)
{
  vec3 color;
  x = 0.5 * (x + 1.0);
  x = sqrt(x);
  x = sqrt(x);
  x = sqrt(x);
  color = vec3(0.2 + 0.75 * x);
  color.b *= 0.95;
  return color;
}

vec3 get_marble_color() {
  const float amplitude = 10.0;
  const int roughness = 8;

  float color = VS_tex_coord.x * 6.0;
  color += amplitude * turbulence(VS_tex_coord, roughness);
  return marble_tex_color(sin(color));
}

void main()
{
    vec4 tex_color_alpha = texture(my_tex, VS_tex_coord.xy);
    vec3 tex_color = tex_color_alpha.rgb;
    float alpha = tex_color_alpha.a;

    if (procedural_tex_type == 1) {
      tex_color = get_marble_color();
      alpha = 1.0;
    }

    if (alpha == 0.0)
      discard;

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

    final_color = vec4(light, alpha);
}
