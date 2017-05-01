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

// Simple camera that allows us to look at the object from different views
PV112Camera my_camera;

// OpenGL texture objects
GLuint wall_tex;
GLuint paving_tex;

// Current time of the application in seconds, for animations
float app_time_s = 0.0f;

glm::vec3 size_vector = glm::vec3(20.0, 5, 40.0);

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
  storage.setTexRepeatLocation(glGetUniformLocation(program, "tex_repeat_factor"));
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

  wall_tex = CreateAndLoadTexture(MAYBEWIDE("./textures/wall.jpg"));
  paving_tex = CreateAndLoadTexture(MAYBEWIDE("./textures/paving.jpg"));

  glBindTexture(GL_TEXTURE_2D, wall_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  glBindTexture(GL_TEXTURE_2D, paving_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void renderRectangle(const glm::mat4& PV_matrix, const glm::mat4& model_matrix,
  float tex_repeat_factor) {
  glm::mat4 PVM_matrix = PV_matrix * model_matrix;
  glm::mat3 normal_matrix = getNormalMatrix(model_matrix);
  glUniformMatrix4fv(storage.getModelMatrix(), 1, GL_FALSE, glm::value_ptr(model_matrix));
  glUniformMatrix4fv(storage.getPVMMatrix(), 1, GL_FALSE, glm::value_ptr(PVM_matrix));
  glUniformMatrix3fv(storage.getNormalMatrix(), 1, GL_FALSE, glm::value_ptr(normal_matrix));
  glUniform1f(storage.getTexRepeatLocation(), tex_repeat_factor);
  DrawGeometry(my_rectangle);
}

void renderRoom(const glm::mat4& projection_matrix, const glm::mat4& view_matrix,
const glm::vec3& scale) {
  glBindVertexArray(my_rectangle.VAO);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, wall_tex);
  glUniform1i(storage.getMyTex(), 0);
  glm::mat4 PV_matrix = projection_matrix * view_matrix;

  //  glEnable(GL_CULL_FACE);
  //  glCullFace(GL_BACK);

  //left wall
  glm::mat4 model_matrix;
  model_matrix = glm::mat4(1.0f);
  model_matrix = glm::rotate(model_matrix, (float)glm::radians(90.0), glm::vec3(0.0,1.0,0.0));
  model_matrix = glm::translate(model_matrix, glm::vec3(0.0, scale.y, -scale.x / 2.0));
  model_matrix = glm::scale(model_matrix, glm::vec3(scale.x, scale.y, 0));
  renderRectangle(PV_matrix, model_matrix, 5.0);

  // right wall
  model_matrix = glm::mat4(1.0f);
  model_matrix = glm::rotate(model_matrix, (float)glm::radians(-90.0), glm::vec3(0.0,1.0,0.0));
  model_matrix = glm::translate(model_matrix, glm::vec3(0.0, scale.y, -scale.x / 2.0));
  model_matrix = glm::scale(model_matrix, glm::vec3(scale.x, scale.y,0));
  renderRectangle(PV_matrix, model_matrix, 5.0);

  // back wall
  model_matrix = glm::mat4(1.0f);
  model_matrix = glm::translate(model_matrix, glm::vec3(0.0, scale.y, -scale.z / 2.0));
  model_matrix = glm::scale(model_matrix, glm::vec3(scale.x / 2.0, scale.y,0));
  renderRectangle(PV_matrix, model_matrix, 5.0);

  // front wall
  model_matrix = glm::mat4(1.0f);
  model_matrix = glm::rotate(model_matrix, (float)glm::radians(180.0), glm::vec3(0.0,1.0,0.0));
  model_matrix = glm::translate(model_matrix, glm::vec3(0.0, scale.y, -scale.z / 2.0));
  model_matrix = glm::scale(model_matrix, glm::vec3(scale.x / 2.0, scale.y,0));
  renderRectangle(PV_matrix, model_matrix, 5.0);

  // bottom paving
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, paving_tex);
  glUniform1i(storage.getMyTex(), 0);

  model_matrix = glm::mat4(1.0f);
  model_matrix = glm::rotate(model_matrix, (float)glm::radians(-90.0), glm::vec3(1.0,0.0,0.0));
  model_matrix = glm::translate(model_matrix, glm::vec3(0.0, 0.0, 0.0));
  model_matrix = glm::scale(model_matrix, glm::vec3(scale.x / 2.0, scale.z / 2.0,0));
  renderRectangle(PV_matrix, model_matrix, 7.0);
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

void render()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(program);

  glm::mat4 projection_matrix, view_matrix, model_matrix, PVM_matrix;
  glm::mat3 normal_matrix;

  projection_matrix = glm::perspective(glm::radians(45.0f),
        float(win_width) / float(win_height), 0.1f, 100.0f);
  view_matrix = glm::lookAt(my_camera.GetEyePosition(),
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

  glUniform3fv(storage.getEyePosition(), 1, glm::value_ptr(my_camera.GetEyePosition()));

  renderLight();
  renderRoom(projection_matrix, view_matrix, size_vector);
  // Cube
  glBindVertexArray(my_cube.VAO);
  model_matrix = glm::mat4(1.0f);

  PVM_matrix = projection_matrix * view_matrix * model_matrix;
  normal_matrix = getNormalMatrix(model_matrix);
  glUniformMatrix4fv(storage.getModelMatrix(), 1, GL_FALSE, glm::value_ptr(model_matrix));
  glUniformMatrix4fv(storage.getPVMMatrix(), 1, GL_FALSE, glm::value_ptr(PVM_matrix));
  glUniformMatrix3fv(storage.getNormalMatrix(), 1, GL_FALSE, glm::value_ptr(normal_matrix));
  glUniform1f(storage.getTexRepeatLocation(), 1.0);
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
