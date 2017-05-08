// Project for PV112
#include "locationstorage.h"
#include "helpers.h"

#include <iostream>
#include "PV112.h"
#define _USE_MATH_DEFINES
#include <math.h>
// Include GLEW, use static library
#define GLEW_STATIC
#include <GL/glew.h>
#pragma comment(lib, "glew32s.lib") // Link with GLEW library
// Include FreeGLUT
#include <GL/freeglut.h>
// Include the most important GLM functions
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


using namespace std;
using namespace PV112;

// Current window size
int win_width = 1024;
int win_height = 768;

// Shader program and its uniforms
GLuint program;

LocationStorage storage;

PV112Geometry my_cube;
PV112Geometry my_rectangle;
PV112Geometry statue_of_liberty;
PV112Geometry marble_statue;
PV112Geometry cup;

// Simple camera that allows us to look at the object from different views
PV112Camera my_camera;

// OpenGL texture objects
GLuint wall_tex;
GLuint paving_tex;
GLuint mona_lisa_tex;
GLuint painting_frame_tex;
GLuint bronze_tex;
GLuint night_watch_tex;
GLuint school_of_athens_tex;
GLuint fall_of_icarus_tex;
GLuint water_lilies_tex;
GLuint wood_tex;
GLuint cup_tex;
GLuint glass_tex;
GLuint door_tex;

// Current time of the application in seconds, for animations
float app_time_s = 0.0f;

glm::vec3 size_vector = glm::vec3(20.0, 5.0, 40.0);

// Called when the user presses a key
void key_pressed(unsigned char key, int mouseX, int mouseY)
{
  switch (key)
  {
  case 27: // Escape
      exit(0);
      break;
  case 'l':
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glutPostRedisplay();
      break;
  case 'f':
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glutPostRedisplay();
      break;
  case 't':
      glutFullScreenToggle();
      break;
  case 'w':
      my_camera.move(Moving::FORWARD);
      glutPostRedisplay();
      break;
  case 's':
      my_camera.move(Moving::BACKWARD);
      glutPostRedisplay();
      break;
  case 'a':
      my_camera.move(Moving::LEFT);
      glutPostRedisplay();
      break;
  case 'd':
      my_camera.move(Moving::RIGHT);
      glutPostRedisplay();
      break;
  }
}

// Called when the user presses a mouse button
void mouse_button_changed(int button, int state, int x, int y)
{
  my_camera.OnMouseButtonChanged(button, state, x, y);
}

// Called when the user moves with the mouse (when some mouse button is pressed)
void mouse_moved(int x, int y)
{
  my_camera.OnMouseMoved(x, y);
}

void initVariables() {
  storage.setModelMatrix(glGetUniformLocation(program, "model_matrix"));
  storage.setPVMMatrix(glGetUniformLocation(program, "PVM_matrix"));
  storage.setNormalMatrix(glGetUniformLocation(program, "normal_matrix"));

  storage.setMaterialAmbientColor(glGetUniformLocation(program, "material_ambient_color"));
  storage.setMaterialDiffuseColor(glGetUniformLocation(program, "material_diffuse_color"));
  storage.setMaterialSpecularColor(glGetUniformLocation(program, "material_specular_color"));
  storage.setMaterialShininess(glGetUniformLocation(program, "material_shininess"));

  storage.setLightPosition(glGetUniformLocation(program, "light_position"));
  storage.setLightAmbientColor(glGetUniformLocation(program, "light_ambient_color"));
  storage.setLightDiffuseColor(glGetUniformLocation(program, "light_diffuse_color"));
  storage.setLightSpecularColor(glGetUniformLocation(program, "light_specular_color"));

  storage.setEyePosition(glGetUniformLocation(program, "eye_position"));

  storage.setMyTex(glGetUniformLocation(program, "my_tex"));
  storage.setTexRepeatXLocation(glGetUniformLocation(program, "tex_repeat_factor_x"));
  storage.setTexRepeatYLocation(glGetUniformLocation(program, "tex_repeat_factor_y"));
  storage.setProceduralTexType(glGetUniformLocation(program, "procedural_tex_type"));
}

void init()
{
  glClearColor(0.3f, 0.4f, 0.3f, 0.0f);
  glClearDepth(1.0);
  glEnable(GL_DEPTH_TEST);

  // Create shader program
  program = CreateAndLinkProgram("vertex.glsl", "fragment.glsl");
  if (0 == program)
      WaitForEnterAndExit();

  initVariables();

  int position_loc = glGetAttribLocation(program, "position");
  int normal_loc = glGetAttribLocation(program, "normal");
  int tex_coord_loc = glGetAttribLocation(program, "tex_coord");

  my_cube = CreateCube(position_loc, normal_loc, tex_coord_loc);
  my_rectangle = CreateRectangle(position_loc, normal_loc, tex_coord_loc);
  statue_of_liberty = LoadOBJ("./obj_files/statue_of_liberty.obj", position_loc, normal_loc, tex_coord_loc);
  marble_statue = LoadOBJ("./obj_files/marble_statue.obj", position_loc, normal_loc, tex_coord_loc);
  cup = LoadOBJ("./obj_files/cup.obj", position_loc, normal_loc, tex_coord_loc);

  wall_tex = CreateAndLoadTexture(MAYBEWIDE("./textures/wall.jpg"));
  paving_tex = CreateAndLoadTexture(MAYBEWIDE("./textures/paving.jpg"));
  mona_lisa_tex = CreateAndLoadTexture(MAYBEWIDE("./textures/mona_lisa.jpg"));
  painting_frame_tex = CreateAndLoadTexture(MAYBEWIDE("./textures/painting_frame.png"));
  bronze_tex = CreateAndLoadTexture(MAYBEWIDE("./textures/bronze.jpg"));
  night_watch_tex = CreateAndLoadTexture(MAYBEWIDE("./textures/night_watch_rembrandt.jpg"));
  school_of_athens_tex = CreateAndLoadTexture(MAYBEWIDE("./textures/school_of_athens_raphael.jpg"));
  fall_of_icarus_tex = CreateAndLoadTexture(MAYBEWIDE("./textures/fall_of_icarus.jpg"));
  water_lilies_tex = CreateAndLoadTexture(MAYBEWIDE("./textures/water_lilies.jpg"));
  wood_tex = CreateAndLoadTexture(MAYBEWIDE("./textures/wood.jpg"));
  cup_tex = CreateAndLoadTexture(MAYBEWIDE("./textures/cup_tex.jpg"));
  glass_tex = CreateAndLoadTexture(MAYBEWIDE("./textures/glass2.png"));
  door_tex = CreateAndLoadTexture(MAYBEWIDE("./textures/door.jpg"));

  glBindTexture(GL_TEXTURE_2D, wall_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  glBindTexture(GL_TEXTURE_2D, paving_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  glBindTexture(GL_TEXTURE_2D, mona_lisa_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  glBindTexture(GL_TEXTURE_2D, painting_frame_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  glBindTexture(GL_TEXTURE_2D, bronze_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  glBindTexture(GL_TEXTURE_2D, night_watch_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  glBindTexture(GL_TEXTURE_2D, school_of_athens_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  glBindTexture(GL_TEXTURE_2D, fall_of_icarus_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  glBindTexture(GL_TEXTURE_2D, water_lilies_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  glBindTexture(GL_TEXTURE_2D, wood_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  glBindTexture(GL_TEXTURE_2D, cup_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  glBindTexture(GL_TEXTURE_2D, glass_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  glBindTexture(GL_TEXTURE_2D, door_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void sendDataToShaders(const glm::mat4& PV_matrix, const glm::mat4& model_matrix,
const float& tex_repeat_x = 1.0, const float& tex_repeat_y = 1.0,
const int& procedural_tex_type = 0) {
  glm::mat4 PVM_matrix = PV_matrix * model_matrix;
  glm::mat3 normal_matrix = getNormalMatrix(model_matrix);
  glUniformMatrix4fv(storage.getModelMatrix(), 1, GL_FALSE, glm::value_ptr(model_matrix));
  glUniformMatrix4fv(storage.getPVMMatrix(), 1, GL_FALSE, glm::value_ptr(PVM_matrix));
  glUniformMatrix3fv(storage.getNormalMatrix(), 1, GL_FALSE, glm::value_ptr(normal_matrix));
  glUniform1f(storage.getTexRepeatXLocation(), tex_repeat_x);
  glUniform1f(storage.getTexRepeatYLocation(), tex_repeat_y);
  glUniform1i(storage.getProceduralTexType(), procedural_tex_type);
}

void renderRectangle(const glm::mat4& PV_matrix, const glm::mat4& model_matrix,
  float tex_repeat_factor_x, float tex_repeat_factor_y) {
  sendDataToShaders(PV_matrix, model_matrix, tex_repeat_factor_x, tex_repeat_factor_y);
  DrawGeometry(my_rectangle);
}

void renderRoom(const glm::mat4& PV_matrix) {
  glBindVertexArray(my_rectangle.VAO);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, wall_tex);
  glUniform1i(storage.getMyTex(), 0);
  //  glEnable(GL_CULL_FACE);
  //  glCullFace(GL_BACK);

  //left wall
  glm::mat4 model_matrix;
  model_matrix = glm::mat4(1.0f);
  model_matrix = glm::rotate(model_matrix, (float)glm::radians(90.0), glm::vec3(0.0,1.0,0.0));
  model_matrix = glm::translate(model_matrix, glm::vec3(0.0, size_vector.y, -size_vector.x / 2.0));
  model_matrix = glm::scale(model_matrix, glm::vec3(size_vector.x, size_vector.y, 0));
  renderRectangle(PV_matrix, model_matrix, 2.0, 2.0);

  // right wall
  model_matrix = glm::mat4(1.0f);
  model_matrix = glm::rotate(model_matrix, (float)glm::radians(-90.0), glm::vec3(0.0,1.0,0.0));
  model_matrix = glm::translate(model_matrix, glm::vec3(0.0, size_vector.y, -size_vector.x / 2.0));
  model_matrix = glm::scale(model_matrix, glm::vec3(size_vector.x, size_vector.y,0));
  renderRectangle(PV_matrix, model_matrix,  2.0, 2.0);

  // back wall
  model_matrix = glm::mat4(1.0f);
  model_matrix = glm::translate(model_matrix, glm::vec3(0.0, size_vector.y, -size_vector.z / 2.0));
  model_matrix = glm::scale(model_matrix, glm::vec3(size_vector.x / 2.0, size_vector.y,0));
  renderRectangle(PV_matrix, model_matrix, 2.0, 2.0);

  // front wall
  model_matrix = glm::mat4(1.0f);
  model_matrix = glm::rotate(model_matrix, (float)glm::radians(180.0), glm::vec3(0.0,1.0,0.0));
  model_matrix = glm::translate(model_matrix, glm::vec3(0.0, size_vector.y, -size_vector.z / 2.0));
  model_matrix = glm::scale(model_matrix, glm::vec3(size_vector.x / 2.0, size_vector.y,0));
  renderRectangle(PV_matrix, model_matrix, 2.0, 2.0);

  // bottom paving
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, paving_tex);
  glUniform1i(storage.getMyTex(), 0);

  model_matrix = glm::mat4(1.0f);
  model_matrix = glm::rotate(model_matrix, (float)glm::radians(-90.0), glm::vec3(1.0,0.0,0.0));
  model_matrix = glm::translate(model_matrix, glm::vec3(0.0, 0.0, 0.0));
  model_matrix = glm::scale(model_matrix, glm::vec3(size_vector.x / 2.0, size_vector.z / 2.0,0));
  float ratio = size_vector.z / size_vector.x;
  int repeat = 5;
  renderRectangle(PV_matrix, model_matrix, repeat, repeat * ratio);

  //door_tex
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, door_tex);
  glUniform1i(storage.getMyTex(), 0);

  float factor = 2.064891847;
  model_matrix = glm::mat4(1.0f);
  model_matrix = glm::translate(model_matrix, glm::vec3(0.0, 4.0, size_vector.z / 2.0 -0.1));
  model_matrix = glm::rotate(model_matrix, static_cast<float>(glm::radians(180.0)), glm::vec3(0.0, 1.0, 0.0));
  model_matrix = glm::scale(model_matrix, glm::vec3(2.0, 2.0 * factor,0));
  renderRectangle(PV_matrix, model_matrix, 1.0, 1.0);
  //glBindVertexArray(0);
}

void renderLight() {
  glm::vec4 light_pos =  glm::vec4(0.0f, 10.0f, 0.0f, 1.0f);
  glUniform4fv(storage.getLightPosition(), 1, glm::value_ptr(light_pos));
  glUniform3f(storage.getLightAmbientColor(), 0.3f, 0.3f, 0.3f);
  glUniform3f(storage.getLightDiffuseColor(), 1.0f, 1.0f, 1.0f);
  glUniform3f(storage.getLightSpecularColor(), 1.0f, 1.0f, 1.0f);

  glUniform3f(storage.getMaterialAmbientColor(), 1.0f, 1.0f, 1.0f);
  glUniform3f(storage.getMaterialDiffuseColor(), 1.0f, 1.0f, 1.0f);
  glUniform3f(storage.getMaterialSpecularColor(), 1.0f, 1.0f, 1.0f);
  glUniform1f(storage.getMaterialShininess(), 40.0f);
}

void renderPictures(const glm::mat4& PV_matrix) {
  const float spaceBetweenPaintings = size_vector.z / 5.0;
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, mona_lisa_tex);
  glUniform1i(storage.getMyTex(), 0);

  float ratio = 4.0/3.0;
  float x_size = 1.8;

  // mona lisa
  glm::mat4 model_matrix = glm::mat4(1.0f);
  model_matrix = glm::translate(model_matrix, glm::vec3(0.0, size_vector.y, -size_vector.z / 2.0 + 0.1));
  model_matrix = glm::scale(model_matrix, glm::vec3(x_size, x_size * ratio,0));
  renderRectangle(PV_matrix, model_matrix, 1.0, 1.0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, painting_frame_tex);
  glUniform1i(storage.getMyTex(), 0);

  float factor = 1.2;
  model_matrix = glm::mat4(1.0f);
  model_matrix = glm::rotate(model_matrix, static_cast<float>(glm::radians(90.0)), glm::vec3(0.0,0.0,1.0));
  model_matrix = glm::translate(model_matrix, glm::vec3(size_vector.y, 0.0, -size_vector.z / 2.0 + 0.2));
  model_matrix = glm::scale(model_matrix, glm::vec3(x_size * ratio * factor, x_size * factor,0));
  renderRectangle(PV_matrix, model_matrix, 1.0, 1.0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, night_watch_tex);
  glUniform1i(storage.getMyTex(), 0);

  //night_watch
  ratio = 1.20251938;
  x_size = 2.6;
  model_matrix = glm::mat4(1.0f);
  model_matrix = glm::rotate(model_matrix, static_cast<float>(glm::radians(-90.0)), glm::vec3(0.0, 1.0, 0.0));
  model_matrix = glm::translate(model_matrix, glm::vec3(-size_vector.z / 2.0 + x_size * 2.0 , size_vector.y, -size_vector.x / 2.0 + 0.1));
  model_matrix = glm::scale(model_matrix, glm::vec3(x_size, x_size / ratio,0));
  renderRectangle(PV_matrix, model_matrix, 1.0, 1.0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, painting_frame_tex);
  glUniform1i(storage.getMyTex(), 0);

  factor = 1.3;
  model_matrix = glm::translate(model_matrix, glm::vec3(0.0, 0.0, 0.2));
  model_matrix = glm::scale(model_matrix, glm::vec3(factor, factor*1.14 ,0.0));
  renderRectangle(PV_matrix, model_matrix, 1.0, 1.0);

  //school_of_athens
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, school_of_athens_tex);
  glUniform1i(storage.getMyTex(), 0);

  ratio = 1.287605295;
  x_size = 2.6;
  model_matrix = glm::mat4(1.0f);
  model_matrix = glm::rotate(model_matrix, static_cast<float>(glm::radians(-90.0)), glm::vec3(0.0, 1.0, 0.0));
  model_matrix = glm::translate(model_matrix, glm::vec3(-size_vector.z / 2.0 + spaceBetweenPaintings * 2 , size_vector.y, -size_vector.x / 2.0 + 0.1));
  model_matrix = glm::scale(model_matrix, glm::vec3(x_size, x_size / ratio,0));
  renderRectangle(PV_matrix, model_matrix, 1.0, 1.0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, painting_frame_tex);
  glUniform1i(storage.getMyTex(), 0);

  factor = 1.3;
  model_matrix = glm::translate(model_matrix, glm::vec3(0.0, 0.0, 0.2));
  model_matrix = glm::scale(model_matrix, glm::vec3(factor, factor*1.14 ,0.0));
  renderRectangle(PV_matrix, model_matrix, 1.0, 1.0);

  //fall_of_icarus
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fall_of_icarus_tex);
  glUniform1i(storage.getMyTex(), 0);

  ratio = 1.516425756;
  x_size = 2.7;
  model_matrix = glm::mat4(1.0f);
  model_matrix = glm::rotate(model_matrix, static_cast<float>(glm::radians(-90.0)), glm::vec3(0.0, 1.0, 0.0));
  model_matrix = glm::translate(model_matrix, glm::vec3(-size_vector.z / 2.0 + spaceBetweenPaintings * 3.2 , size_vector.y, -size_vector.x / 2.0 + 0.1));
  model_matrix = glm::scale(model_matrix, glm::vec3(x_size, x_size / ratio,0));
  renderRectangle(PV_matrix, model_matrix, 1.0, 1.0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, painting_frame_tex);
  glUniform1i(storage.getMyTex(), 0);

  factor = 1.3;
  model_matrix = glm::translate(model_matrix, glm::vec3(0.0, 0.0, 0.2));
  model_matrix = glm::scale(model_matrix, glm::vec3(factor, factor*1.14 ,0.0));
  renderRectangle(PV_matrix, model_matrix, 1.0, 1.0);

  //water_lilies
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, water_lilies_tex);
  glUniform1i(storage.getMyTex(), 0);

  ratio = 1.508503401;
  x_size = 2.7;
  model_matrix = glm::mat4(1.0f);
  model_matrix = glm::rotate(model_matrix, static_cast<float>(glm::radians(-90.0)), glm::vec3(0.0, 1.0, 0.0));
  model_matrix = glm::translate(model_matrix, glm::vec3(-size_vector.z / 2.0 + spaceBetweenPaintings * 4.35 , size_vector.y, -size_vector.x / 2.0 + 0.1));
  model_matrix = glm::scale(model_matrix, glm::vec3(x_size, x_size / ratio,0));
  renderRectangle(PV_matrix, model_matrix, 1.0, 1.0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, painting_frame_tex);
  glUniform1i(storage.getMyTex(), 0);

  factor = 1.3;
  model_matrix = glm::translate(model_matrix, glm::vec3(0.0, 0.0, 0.2));
  model_matrix = glm::scale(model_matrix, glm::vec3(factor, factor*1.14 ,0.0));
  renderRectangle(PV_matrix, model_matrix, 1.0, 1.0);
}

void renderStatues(const glm::mat4& PV_matrix) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, bronze_tex);
  glUniform1i(storage.getMyTex(), 0);

  glBindVertexArray(statue_of_liberty.VAO);
  glm::mat4 model_matrix = glm::mat4(1.0f);
  model_matrix = glm::translate(model_matrix, glm::vec3(-size_vector.x / 2.0 + 2.0, 0.0, -size_vector.z / 2.0 + 2.0));
  model_matrix = glm::scale(model_matrix, glm::vec3(5.0, 5.0, 5.0));
  model_matrix = glm::rotate(model_matrix, static_cast<float>(glm::radians(45.0)), glm::vec3(0.0, 1.0, 0.0));
  sendDataToShaders(PV_matrix, model_matrix);
  DrawGeometry(statue_of_liberty);

  // busta
  float distance = size_vector.z / 5;
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, wood_tex);
  glUniform1i(storage.getMyTex(), 0);

  glBindVertexArray(my_cube.VAO);
  model_matrix = glm::mat4(1.0f);
  model_matrix = glm::translate(model_matrix, glm::vec3(-size_vector.x / 2.0 + 2.0, 1.0, -size_vector.z / 2.0 + 2.0 + distance));
  model_matrix = glm::scale(model_matrix, glm::vec3(0.9, 1.5, 0.9));
  sendDataToShaders(PV_matrix, model_matrix, 1.0, 1.0, 0);
  DrawGeometry(my_cube);

  glBindVertexArray(marble_statue.VAO);
  model_matrix = glm::mat4(1.0f);
  model_matrix = glm::rotate(model_matrix, static_cast<float>(glm::radians(90.0)), glm::vec3(0.0, 1.0, 0.0));
  model_matrix = glm::translate(model_matrix, glm::vec3(size_vector.z / 2.0 - 2.0 - distance , 2.4, -size_vector.x / 2.0 + 2.0));
  sendDataToShaders(PV_matrix, model_matrix, 1.0, 1.0, 1);
  DrawGeometry(marble_statue);

  // golden cup
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, cup_tex);
  glUniform1i(storage.getMyTex(), 0);

  glBindVertexArray(cup.VAO);
  model_matrix = glm::mat4(1.0f);
  model_matrix = glm::translate(model_matrix, glm::vec3(0.0, 1.0, 0.0));
  model_matrix = glm::translate(model_matrix, glm::vec3(-size_vector.x / 2.0 + 2.0, 1.3, -size_vector.z / 2.0 + 2.0 + distance * 2));
  model_matrix = glm::rotate(model_matrix, app_time_s / 3.0f, glm::vec3(0.0, 1.0, 0.0));
  sendDataToShaders(PV_matrix, model_matrix, 1.0, 1.0, 0);
  DrawGeometry(cup);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, wood_tex);
  glUniform1i(storage.getMyTex(), 0);

  glBindVertexArray(my_cube.VAO);
  model_matrix = glm::mat4(1.0f);
  model_matrix = glm::translate(model_matrix, glm::vec3(-size_vector.x / 2.0 + 2.0, 1.2, -size_vector.z / 2.0 + 2.0 + 2 * distance));
  model_matrix = glm::scale(model_matrix, glm::vec3(1.4, 1.2, 1.4));
  sendDataToShaders(PV_matrix, model_matrix, 1.0, 1.0, 0);
  DrawGeometry(my_cube);

  glBindVertexArray(my_cube.VAO);
  model_matrix = glm::mat4(1.0f);
  model_matrix = glm::translate(model_matrix, glm::vec3(-size_vector.x / 2.0 + 2.0, 6.7, -size_vector.z / 2.0 + 2.0 + 2 * distance));
  model_matrix = glm::scale(model_matrix, glm::vec3(1.4, 0.3, 1.4));
  sendDataToShaders(PV_matrix, model_matrix, 1.0, 1.0, 0);
  DrawGeometry(my_cube);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, glass_tex);
  glUniform1i(storage.getMyTex(), 0);

  // glass around cup - uses alpha blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  model_matrix = glm::mat4(1.0f);
  model_matrix = glm::translate(model_matrix, glm::vec3(-size_vector.x / 2.0 + 2.0, 4.4, -size_vector.z / 2.0 + 2.0 + 2 * distance));
  model_matrix = glm::scale(model_matrix, glm::vec3(1.4, 2.0, 1.4));
  sendDataToShaders(PV_matrix, model_matrix, 1.0, 1.0, 0);
  DrawGeometry(my_cube);
  glDisable(GL_BLEND);
  //glBindVertexArray(0);
}

void render()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(program);

  glm::mat4 projection_matrix, view_matrix, model_matrix, PVM_matrix;
  glm::mat3 normal_matrix;

  projection_matrix = glm::perspective(glm::radians(50.0f),
        float(win_width) / float(win_height), 0.1f, 100.0f);

  view_matrix = glm::lookAt(my_camera.getPosition(),
        my_camera.GetEyePosition(), glm::vec3(0.0f, 1.0f, 0.0f));

  glUniform3fv(storage.getEyePosition(), 1, glm::value_ptr(my_camera.getPosition()));

  glm::mat4 PV_matrix = projection_matrix * view_matrix;

  renderLight();
  renderRoom(PV_matrix);
  renderPictures(PV_matrix);
  renderStatues(PV_matrix);

  // Cube
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, wood_tex);
  glUniform1i(storage.getMyTex(), 0);

  glBindVertexArray(my_cube.VAO);
  model_matrix = glm::mat4(1.0f);
  sendDataToShaders(PV_matrix, model_matrix);
  DrawGeometry(my_cube);

  glBindVertexArray(0);
  glUseProgram(0);

  glutSwapBuffers();
}

// Called when the window changes its size
void reshape(int width, int height)
{
    win_width = width;
    win_height = height;

    // Set the area into which we render
    glViewport(0, 0, win_width, win_height);
}

// Callback function to be called when we make an error in OpenGL
void GLAPIENTRY simple_debug_callback(GLenum source, GLenum type, GLuint id,
        GLenum severity, GLsizei length, const char* message, const void* userParam)
{
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        cout << message << endl; // Put the breakpoint here
        return;
    default:
        return;
    }
}

// Simple timer function for animations
void timer(int)
{
    app_time_s += 0.020f;
    glutTimerFunc(20, timer, 0);
    glutPostRedisplay();
}

int main(int argc, char ** argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    // Set OpenGL Context parameters
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_DEBUG);

    // Initialize window
    glutInitWindowSize(win_width, win_height);
    glutCreateWindow("PV112 Museum");

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    // Initialize DevIL library
    ilInit();

    // Set the debug callback
    SetDebugCallback(simple_debug_callback);

    // Initialize our OpenGL stuff
    init();

    // Register callbacks
    glutDisplayFunc(render);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(key_pressed);
    glutTimerFunc(20, timer, 0);
    glutMouseFunc(mouse_button_changed);
    glutMotionFunc(mouse_moved);

    // Run the main loop
    glutMainLoop();

    return 0;
}
