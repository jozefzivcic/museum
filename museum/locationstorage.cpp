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

GLint LocationStorage::getMyTex() const {
  return wall_tex_loc;
}

void LocationStorage::setMyTex(const GLint& location) {
  wall_tex_loc = location;
}

GLint LocationStorage::getTexRepeatXLocation() const {
  return tex_repeat_x_loc;
}

void LocationStorage::setTexRepeatXLocation(const GLint& location) {
  tex_repeat_x_loc = location;
}

GLint LocationStorage::getTexRepeatYLocation() const {
  return tex_repeat_y_loc;
}

void LocationStorage::setTexRepeatYLocation(const GLint& location) {
  tex_repeat_y_loc = location;
}

GLint LocationStorage::getProceduralTexType() const {
  return tex_procedural_type;
}

void LocationStorage::setProceduralTexType(const GLint& location) {
  tex_procedural_type = location;
}

// spotlight
GLint LocationStorage::getSpotlightPositionLocation() const {
  return spotLight_position;
}

void LocationStorage::setSpotlightPositionLocation(const GLint& location) {
  spotLight_position = location;
}

GLint LocationStorage::getSpotlightDirectionLocation() const {
  return spotLight_direction;
}

void LocationStorage::setSpotlightDirectionLocation(const GLint& location) {
  spotLight_direction = location;
}

GLint LocationStorage::getSpotlightAmbientLocation() const {
  return spotLight_ambient;
}

void LocationStorage::setSpotlightAmbientLocation(const GLint& location) {
  spotLight_ambient = location;
}

GLint LocationStorage::getSpotlightDiffuseLocation() const {
  return spotLight_diffuse;
}

void LocationStorage::setSpotlightDiffuseLocation(const GLint& location) {
  spotLight_diffuse = location;
}

GLint LocationStorage::getSpotlightSpecularLocation() const {
  return spotLight_specular;
}

void LocationStorage::setSpotlightSpecularLocation(const GLint& location) {
  spotLight_specular = location;
}

GLint LocationStorage::getSpotlightConstantLocation() const {
  return spotLight_constant;
}

void LocationStorage::setSpotlightConstantLocation(const GLint& location) {
  spotLight_constant = location;
}

GLint LocationStorage::getSpotlightLinearLocation() const {
  return spotLight_linear;
}

void LocationStorage::setSpotlightLinearLocation(const GLint& location) {
  spotLight_linear = location;
}

GLint LocationStorage::getSpotlightQuadraticLocation() const {
  return spotLight_quadratic;
}

void LocationStorage::setSpotlightQuadraticLocation(const GLint& location) {
  spotLight_quadratic = location;
}

GLint LocationStorage::getSpotlightCutOffLocation() const {
  return spotLight_cutOff;
}

void LocationStorage::setSpotlightCutOffLocation(const GLint& location) {
  spotLight_cutOff = location;
}

GLint LocationStorage::getSpotlightOuterCutOffLocation() const {
  return spotLight_outerCutOff;
}

void LocationStorage::setSpotlightOuterCutOffLocation(const GLint& location) {
  spotLight_outerCutOff = location;
}
