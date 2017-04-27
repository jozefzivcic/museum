// Project for PV112

#include <iostream>
#include "PV112.h"

#define _USE_MATH_DEFINES
#include <math.h>

// Include GLEW, use static library
#define GLEW_STATIC
#include <GL/glew.h>
#pragma comment(lib, "glew32s.lib") // Link with GLEW library

// Include DevIL for image loading
#if defined(_WIN32)
#pragma comment(lib, "glew32s.lib")
// On Windows, we use Unicode dll of DevIL
// That also means we need to use wide strings
#ifndef _UNICODE
#define _UNICODE
#include <IL/il.h>
#undef _UNICODE
#else
#include <IL/il.h>
#endif
#pragma comment(lib, "DevIL.lib") // Link with DevIL library
typedef wchar_t maybewchar;
#define MAYBEWIDE(s) L##s
#else // On Linux, we need regular (not wide) strings
#include <IL/il.h>
typedef char maybewchar;
#define MAYBEWIDE(s) s
#endif

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

class LocationStorage {
private:
  GLint model_matrix_loc;
  GLint PVM_matrix_loc;
  GLint normal_matrix_loc;

  GLint material_ambient_color_loc;
  GLint material_diffuse_color_loc;
  GLint material_specular_color_loc;
  GLint material_shininess_loc;

  GLint light_position_loc;
  GLint light_diffuse_color_loc;
  GLint light_ambient_color_loc;
  GLint light_specular_color_loc;

  GLint eye_position_loc;

  GLint wood_tex_loc;
public:
  LocationStorage() {}
  ~LocationStorage() {}

  GLint getModelMatrix() const {
    return model_matrix_loc;
  }

  void setModelMatrix(const GLint& location) {
    model_matrix_loc = location;
  }

  GLint getPVMMatrix() const {
    return PVM_matrix_loc;
  }

  void setPVMMatrix(const GLint& location) {
    PVM_matrix_loc = location;
  }

  GLint getNormalMatrix() const {
    return normal_matrix_loc;
  }

  void setNormalMatrix(const GLint& location) {
    normal_matrix_loc = location;
  }

  GLint getMaterialAmbientColor() const {
    return material_ambient_color_loc;
  }

  void setMaterialAmbientColor(const GLint& location) {
    material_ambient_color_loc = location;
  }

  GLint getMaterialDiffuseColor() const {
    return material_diffuse_color_loc;
  }

  void setMaterialDiffuseColor(const GLint& location) {
    material_diffuse_color_loc = location;
  }

  GLint getMaterialSpecularColor() const {
    return material_specular_color_loc;
  }

  void setMaterialSpecularColor(const GLint& location) {
    material_specular_color_loc = location;
  }

  GLint getMaterialShininess() const {
    return material_shininess_loc;
  }

  void setMaterialShininess(const GLint& location) {
    material_shininess_loc = location;
  }

  GLint getLightPosition() const {
    return light_position_loc;
  }

  void setLightPosition(const GLint& location) {
    light_position_loc = location;
  }

  GLint getLightDiffuseColor() const {
    return light_diffuse_color_loc;
  }

  void setLightDiffuseColor(const GLint& location) {
    light_diffuse_color_loc = location;
  }

  GLint getLightAmbientColor() const {
    return light_ambient_color_loc;
  }

  void setLightAmbientColor(const GLint& location) {
    light_ambient_color_loc = location;
  }

  GLint getLightSpecularColor() const {
    return light_specular_color_loc;
  }

  void setLightSpecularColor(const GLint& location) {
    light_specular_color_loc = location;
  }

  GLint getEyePosition() const {
    return eye_position_loc;
  }

  void setEyePosition(const GLint& location) {
    eye_position_loc = location;
  }

  GLint getWoodTex() const {
    return wood_tex_loc;
  }

  void setWoodTex(const GLint& location) {
    wood_tex_loc = location;
  }
};

LocationStorage storage;



// Simple geometries that we will use in this lecture
PV112Geometry my_cube;

// Simple camera that allows us to look at the object from different views
PV112Camera my_camera;

// OpenGL texture objects
GLuint wood_tex;

// Current time of the application in seconds, for animations
float app_time_s = 0.0f;

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

// Loads a texture from file and calls glTexImage2D to se its data.
// Returns true on success or false on failure.
// NOTE 1a) Describe
bool LoadAndSetTexture(const maybewchar *filename, GLenum target)
{
  // Create IL image
  ILuint IL_tex;
  ilGenImages(1, &IL_tex);

  ilBindImage(IL_tex);

  // Solve upside down textures
  ilEnable(IL_ORIGIN_SET);
  ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

  // Load IL image
  ILboolean success = ilLoadImage(filename);
  if (!success)
  {
    ilBindImage(0);
    ilDeleteImages(1, &IL_tex);
    cerr << "Couldn't load texture: " << filename << endl;
    return false;
  }

  // Get IL image parameters
  int img_width = ilGetInteger(IL_IMAGE_WIDTH);
  int img_height = ilGetInteger(IL_IMAGE_HEIGHT);
  int img_format = ilGetInteger(IL_IMAGE_FORMAT);
  int img_type = ilGetInteger(IL_IMAGE_TYPE);

  // Choose internal format, format, and type for glTexImage2D
  GLint internal_format = 0;
  GLenum format = 0;
  GLenum type = img_type; // IL constants matches GL constants
  switch (img_format)
  {
  case IL_RGB:  internal_format = GL_RGB;  format = GL_RGB;  break;
  case IL_RGBA: internal_format = GL_RGBA; format = GL_RGBA; break;
  case IL_BGR:  internal_format = GL_RGB;  format = GL_BGR;  break;
  case IL_BGRA: internal_format = GL_RGBA; format = GL_BGRA; break;
  case IL_COLOR_INDEX:
  case IL_ALPHA:
  case IL_LUMINANCE:
  case IL_LUMINANCE_ALPHA:
      // Unsupported format
      ilBindImage(0);
      ilDeleteImages(1, &IL_tex);
      cerr << "Texture " << filename << " has unsupported format\n";
      return false;
  }

  // Set the data to OpenGL (assumes texture object is already bound)
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(target, 0, internal_format, img_width, img_height, 0, format,
          type, ilGetData());

  // Unset and delete IL texture
  ilBindImage(0);
  ilDeleteImages(1, &IL_tex);

  return true;
}

GLuint CreateAndLoadTexture(const maybewchar *filename)
{
  // Create OpenGL texture object
  GLuint tex_obj;
  glGenTextures(1, &tex_obj);
  glBindTexture(GL_TEXTURE_2D, tex_obj);

  // Load the data into OpenGL texture object
  if (!LoadAndSetTexture(filename, GL_TEXTURE_2D))
  {
      glBindTexture(GL_TEXTURE_2D, 0);
      glDeleteTextures(1, &tex_obj);
      return 0;
  }
  glBindTexture(GL_TEXTURE_2D, 0);

  return tex_obj;
}


// Initializes OpenGL stuff
void init()
{
  glClearColor(0.3f, 0.4f, 0.3f, 0.0f);
  glClearDepth(1.0);
  glEnable(GL_DEPTH_TEST);

  // Create shader program
  program = CreateAndLinkProgram("vertex.glsl", "fragment.glsl");
  if (0 == program)
      WaitForEnterAndExit();

  // Get attribute and uniform locations
  int position_loc = glGetAttribLocation(program, "position");
  int normal_loc = glGetAttribLocation(program, "normal");
  int tex_coord_loc = glGetAttribLocation(program, "tex_coord");

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

  storage.setWoodTex(glGetUniformLocation(program, "wood_tex"));

  // Create geometries
  my_cube = CreateCube(position_loc, normal_loc, tex_coord_loc);
  // Wood texture

  wood_tex = CreateAndLoadTexture(MAYBEWIDE("wood.jpg"));
  glBindTexture(GL_TEXTURE_2D, wood_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}

// Called when the window needs to be rerendered
void render()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 projection_matrix, view_matrix, model_matrix, PVM_matrix;
  glm::mat3 normal_matrix;

  projection_matrix = glm::perspective(glm::radians(45.0f),
        float(win_width) / float(win_height), 0.1f, 100.0f);
  view_matrix = glm::lookAt(my_camera.GetEyePosition(),
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

  // Light position, with a simple animation
  glm::vec4 light_pos =  glm::vec4(0.0f, 10.0f, 0.0f, 1.0f);

  glUseProgram(program);

  glUniform3fv(storage.getEyePosition(), 1, glm::value_ptr(my_camera.GetEyePosition()));

  glUniform4fv(storage.getLightPosition(), 1, glm::value_ptr(light_pos));
  glUniform3f(storage.getLightAmbientColor(), 0.3f, 0.3f, 0.3f);
  glUniform3f(storage.getLightDiffuseColor(), 1.0f, 1.0f, 1.0f);
  glUniform3f(storage.getLightSpecularColor(), 1.0f, 1.0f, 1.0f);

  // Cube
  glBindVertexArray(my_cube.VAO);

  model_matrix = glm::mat4(1.0f);

  //zavolame aktivnu texturovaciu jednotku
  glActiveTexture(GL_TEXTURE0);
  // zmenime podla textury
  glBindTexture(GL_TEXTURE_2D, wood_tex);
  //tento sampler bude pracovat s jednotkou 0
  glUniform1i(storage.getWoodTex(), 0);


  glUniform3f(storage.getMaterialAmbientColor(), 1.0f, 1.0f, 1.0f);
  glUniform3f(storage.getMaterialDiffuseColor(), 1.0f, 1.0f, 1.0f);
  glUniform3f(storage.getMaterialSpecularColor(), 1.0f, 1.0f, 1.0f);
  glUniform1f(storage.getMaterialShininess(), 40.0f);

  PVM_matrix = projection_matrix * view_matrix * model_matrix;
  normal_matrix = glm::inverse(glm::transpose(glm::mat3(model_matrix)));
  glUniformMatrix4fv(storage.getModelMatrix(), 1, GL_FALSE, glm::value_ptr(model_matrix));
  glUniformMatrix4fv(storage.getPVMMatrix(), 1, GL_FALSE, glm::value_ptr(PVM_matrix));
  glUniformMatrix3fv(storage.getNormalMatrix(), 1, GL_FALSE, glm::value_ptr(normal_matrix));

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
