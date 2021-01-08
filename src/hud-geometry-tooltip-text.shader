
#shader vertex

#version 330 core

layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 texCoord;

out vec2 texCoord_transfer;

uniform vec2 position;
uniform vec2 scales;

// This shader is for textures

void main() {

   // Note to self: the text of the tooltip needs to go over the
   // background of the tooltip

   vec2 scaled = vertex * scales;
   gl_Position = vec4(scaled + position, -1.0, 1.0);
   texCoord_transfer = texCoord;
}


#shader fragment

#version 330 core

uniform sampler2D text;
uniform vec4 text_colour; // only use the first 3 of course.

in vec2 texCoord_transfer;

out vec4 outputColor;

void main() {

   vec4 sampled = texture(text, texCoord_transfer);
   sampled = vec4(text_colour.r, text_colour.g, text_colour.b, sampled.r);

   outputColor = sampled;
}