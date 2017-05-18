#ifndef LOCATION_H
#define LOCATION_H

#include <GL/glew.h>

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

  GLint wall_tex_loc;
  GLint paving_tex_loc;
  GLint tex_repeat_x_loc;
  GLint tex_repeat_y_loc;

  GLint tex_procedural_type;

  GLint spotLight_position;
  GLint spotLight_direction;
  GLint spotLight_ambient;
  GLint spotLight_diffuse;
  GLint spotLight_specular;
  GLint spotLight_constant;
  GLint spotLight_linear;
  GLint spotLight_quadratic;
  GLint spotLight_cutOff;
  GLint spotLight_outerCutOff;

public:
  LocationStorage();
  ~LocationStorage();

  GLint getModelMatrix() const;

  void setModelMatrix(const GLint& location);

  GLint getPVMMatrix() const;

  void setPVMMatrix(const GLint& location);

  GLint getNormalMatrix() const;

  void setNormalMatrix(const GLint& location);

  GLint getMaterialAmbientColor() const;

  void setMaterialAmbientColor(const GLint& location);

  GLint getMaterialDiffuseColor() const;

  void setMaterialDiffuseColor(const GLint& location);

  GLint getMaterialSpecularColor() const;

  void setMaterialSpecularColor(const GLint& location);

  GLint getMaterialShininess() const;

  void setMaterialShininess(const GLint& location);

  GLint getLightPosition() const;

  void setLightPosition(const GLint& location);

  GLint getLightDiffuseColor() const;

  void setLightDiffuseColor(const GLint& location);

  GLint getLightAmbientColor() const;

  void setLightAmbientColor(const GLint& location);

  GLint getLightSpecularColor() const;

  void setLightSpecularColor(const GLint& location);

  GLint getEyePosition() const;

  void setEyePosition(const GLint& location);

  GLint getMyTex() const;

  void setMyTex(const GLint& location);

  GLint getTexRepeatXLocation() const;

  void setTexRepeatXLocation(const GLint& location);

  GLint getTexRepeatYLocation() const;

  void setTexRepeatYLocation(const GLint& location);

  GLint getProceduralTexType() const;

  void setProceduralTexType(const GLint& location);

// spotLight
  GLint getSpotlightPositionLocation() const;

  void setSpotlightPositionLocation(const GLint& location);

  GLint getSpotlightDirectionLocation() const;

  void setSpotlightDirectionLocation(const GLint& location);

  GLint getSpotlightAmbientLocation() const;

  void setSpotlightAmbientLocation(const GLint& location);

  GLint getSpotlightDiffuseLocation() const;

  void setSpotlightDiffuseLocation(const GLint& location);

  GLint getSpotlightSpecularLocation() const;

  void setSpotlightSpecularLocation(const GLint& location);

  GLint getSpotlightConstantLocation() const;

  void setSpotlightConstantLocation(const GLint& location);

  GLint getSpotlightLinearLocation() const;

  void setSpotlightLinearLocation(const GLint& location);

  GLint getSpotlightQuadraticLocation() const;

  void setSpotlightQuadraticLocation(const GLint& location);

  GLint getSpotlightCutOffLocation() const;

  void setSpotlightCutOffLocation(const GLint& location);

  GLint getSpotlightOuterCutOffLocation() const;

  void setSpotlightOuterCutOffLocation(const GLint& location);
};
#endif
