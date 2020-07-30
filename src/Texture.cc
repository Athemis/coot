
#include <iostream>

#include "Texture.hh"
#include "stb_image.h"

Texture::Texture(const std::string &file_name) {
   init(file_name);
}

void
Texture::init(const std::string &file_name) {

   int width, height, num_components;
   stbi_uc* image_data = stbi_load(file_name.c_str(), &width, &height, &num_components, 4);
   id = file_name;

   if (!image_data) {
      std::string s = stbi_failure_reason();
      std::cout << "Error loading image data from " << file_name << " : " << s << std::endl;
      return;
   }

   glGenTextures(1, &m_texture_handle);
   glBindTexture(GL_TEXTURE_2D, m_texture_handle);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

   stbi_image_free(image_data);
}

void
Texture::close() {

   std::cout << "INFO:: deleting texture with id: " << id << " handle " << m_texture_handle << std::endl;
   glDeleteTextures(1, &m_texture_handle);

}

Texture::~Texture() {
   // don't delete the texture here - otherwise we can't copy textures.
}

void
Texture::Bind(unsigned int unit) {

   // unit should be less than 32
   glActiveTexture(GL_TEXTURE0 + unit);
   glBindTexture(GL_TEXTURE_2D, m_texture_handle);

}
