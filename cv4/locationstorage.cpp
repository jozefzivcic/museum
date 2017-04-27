#include "locationstorage.h"

LocationStorage::LocationStorage() {}
LocationStorage::~LocationStorage() {}

GLint LocationStorage::getModelMatrix() const {
  return model_matrix_loc;
}

void LocationStorage::setModelMatrix(const GLint& location) {
  model_matrix_loc = location;
}

GLint LocationStorage::getPVMMatrix() const {
  return PVM_matrix_loc;
}

void LocationStorage::setPVMMatrix(const GLint& location) {
  PVM_matrix_loc = location;
}

GLint LocationStorage::getNormalMatrix() const {
  return normal_matrix_loc;
}

void LocationStorage::setNormalMatrix(const GLint& location) {
  normal_matrix_loc = location;
}

GLint LocationStorage::getMaterialAmbientColor() const {
  return material_ambient_color_loc;
}

void LocationStorage::setMaterialAmbientColor(const GLint& location) {
  material_ambient_color_loc = location;
}

GLint LocationStorage::getMaterialDiffuseColor() const {
  return material_diffuse_color_loc;
}

void LocationStorage::setMaterialDiffuseColor(const GLint& location) {
  material_diffuse_color_loc = location;
}

GLint LocationStorage::getMaterialSpecularColor() const {
  return material_specular_color_loc;
}

void LocationStorage::setMaterialSpecularColor(const GLint& location) {
  material_specular_color_loc = location;
}

GLint LocationStorage::getMaterialShininess() const {
  return material_shininess_loc;
}

void LocationStorage::setMaterialShininess(const GLint& location) {
  material_shininess_loc = location;
}

GLint LocationStorage::getLightPosition() const {
  return light_position_loc;
}

void LocationStorage::setLightPosition(const GLint& location) {
  light_position_loc = location;
}

GLint LocationStorage::getLightDiffuseColor() const {
  return light_diffuse_color_loc;
}

void LocationStorage::setLightDiffuseColor(const GLint& location) {
  light_diffuse_color_loc = location;
}

GLint LocationStorage::getLightAmbientColor() const {
  return light_ambient_color_loc;
}

void LocationStorage::setLightAmbientColor(const GLint& location) {
  light_ambient_color_loc = location;
}

GLint LocationStorage::getLightSpecularColor() const {
  return light_specular_color_loc;
}

void LocationStorage::setLightSpecularColor(const GLint& location) {
  light_specular_color_loc = location;
}

GLint LocationStorage::getEyePosition() const {
  return eye_position_loc;
}

void LocationStorage::setEyePosition(const GLint& location) {
  eye_position_loc = location;
}

GLint LocationStorage::getWoodTex() const {
  return wood_tex_loc;
}

void LocationStorage::setWoodTex(const GLint& location) {
  wood_tex_loc = location;
}
