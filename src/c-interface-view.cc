/* src/c-interface.cc
 * 
 * Copyright 2010 by The University of Oxford
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 */


#if defined (USE_PYTHON)
#include "Python.h"  // before system includes to stop "POSIX_C_SOURCE" redefined problems
#endif

#include "compat/coot-sysdep.h"

#ifdef USE_GUILE
#include <cstddef> // define std::ptrdiff_t for clang
#include <libguile.h>
#endif

#include "cc-interface.hh"
#include "graphics-info.h"

#include "c-interface.h"

/*  ----------------------------------------------------------------------- */
/*                  Functions for FLEV layout callbacks                     */
/*  ----------------------------------------------------------------------- */
// orient the graphics somehow so that the interaction between
// central_residue and neighbour_residue is perpendicular to screen z.
void orient_view(int imol,
		 const coot::residue_spec_t &central_residue_spec, // ligand typically
		 const coot::residue_spec_t &neighbour_residue_spec) {

   graphics_info_t g;
   if (g.use_graphics_interface_flag) {
      if (g.is_valid_model_molecule(imol)) {
	 try { 
	    clipper::Coord_orth vec = g.molecules[imol].get_vector(central_residue_spec,
								   neighbour_residue_spec);

            // Use coot::ScreenVectors here (remove duplication)

            glm::vec4 glm_back  = g.unproject(1.0);
            glm::vec4 glm_front = g.unproject(0.0);
	    coot::Cartesian b(glm_back.x, glm_back.y, glm_back.z);
	    coot::Cartesian f(glm_front.x, glm_front.y, glm_front.z);
	    coot::Cartesian vec_cart(vec);
	    coot::Cartesian b_to_f_cart = f - b;

	    glm::vec4 glm_centre = g.unproject(0, 0, 0.5);
	    glm::vec4 glm_right  = g.unproject(1, 0, 0.5);
	    glm::vec4 glm_top    = g.unproject(0, 1, 0.5);

	    coot::Cartesian centre(glm_centre.x, glm_centre.y, glm_centre.z);
	    coot::Cartesian front(glm_front.x, glm_front.y, glm_front.z);
	    coot::Cartesian right(glm_right.x, glm_right.y, glm_right.z);
	    coot::Cartesian top(glm_top.x, glm_top.y, glm_top.z);

	    coot::Cartesian screen_x = (right - centre);
	    coot::Cartesian screen_y = (top   - centre);
	    coot::Cartesian screen_z = (front - centre);

	    screen_x.unit_vector_yourself();
	    screen_y.unit_vector_yourself();
	    screen_z.unit_vector_yourself();

	    double a_s_x = dot_product(screen_x, vec_cart);
	    double a_s_z = dot_product(screen_z, vec_cart);

	    double theta = atan2(a_s_z, a_s_x) * 0.5;

	    if (0) 
	       std::cout << "theta: " << clipper::Util::rad2d(theta) << " degrees with asx " 
			 << a_s_x << " and asz " << a_s_z << std::endl;

	    rotate_y_scene(100, 0.01 * clipper::Util::rad2d(theta));

				       
	 }
	 catch (const std::runtime_error &rte) {
	    std::cout << rte.what() << std::endl;
	 }
      }
   }
}

/*  ----------------------------------------------------------------------- */
/*                         perspective,blur,AO on/off */
/*  ----------------------------------------------------------------------- */

               // maybe these functions need their own file?

void set_use_perspective_projection(short int state) {

   graphics_info_t::perspective_projection_flag = state;
   graphics_draw();
}

int use_perspective_projection_state() {
   return graphics_info_t::perspective_projection_flag;
}

//! \brief set use ambient occlusion
void set_use_ambient_occlusion(short int state) {
   // user interface is set_use_xxx
   graphics_info_t g;
   g.set_do_ambient_occlusion(state);
   graphics_draw();
}

//! \brief query use ambient occlusion
int use_ambient_occlusion_state() {
   return graphics_info_t::shader_do_ambient_occlusion_flag;
}

//! \brief set use depth blur
void set_use_depth_blur(short int state) {
   graphics_info_t::shader_do_depth_blur_flag = state;
   graphics_draw();
}
//! \brief query use depth blur
int use_depth_blur_state() {
   return graphics_info_t::shader_do_depth_blur_flag;
}


//! \brief set use fog
void set_use_fog(short int state) {
   graphics_info_t::shader_do_depth_fog_flag = state;
   graphics_draw();
}

//! \brief query use fog
int use_fog_state() {
   return graphics_info_t::shader_do_depth_fog_flag;
}

void set_use_outline(short int state) {
   graphics_info_t g;
   g.shader_do_outline_flag = state;
   graphics_draw();
}

int use_outline_state() {
   graphics_info_t g;
   return g.shader_do_outline_flag;
}

void set_map_shininess(int imol, float shininess) {
   if (is_valid_map_molecule(imol)) {
      graphics_info_t::molecules[imol].shader_shininess = shininess;
      graphics_draw();
   }
}

void set_map_specular_strength(int imol, float specular_strength) {
   if (is_valid_map_molecule(imol)) {
      graphics_info_t::molecules[imol].shader_specular_strength = specular_strength;
      graphics_draw();
   }
}

void set_draw_normals(short int state) {

   graphics_info_t::draw_normals_flag = state;
   graphics_draw();

}

int  draw_normals_state() {
   return graphics_info_t::draw_normals_flag;
}


void set_draw_mesh(int imol, int mesh_index, short int state) {
   if (is_valid_map_molecule(imol) || is_valid_model_molecule(imol)) {
      int size = graphics_info_t::molecules[imol].meshes.size();
      if (mesh_index >= 0 && mesh_index < size) {
         graphics_info_t::molecules[imol].meshes[mesh_index].draw_this_mesh = state;
      }
   }
}

int draw_mesh_state(int imol, int mesh_index) {
   if (is_valid_map_molecule(imol) || is_valid_model_molecule(imol)) {
      int size = graphics_info_t::molecules[imol].meshes.size();
      if (mesh_index >= 0 && mesh_index < size) {
         return graphics_info_t::molecules[imol].meshes[mesh_index].draw_this_mesh;
      }
   }
   return -1;
}

void set_map_material_specular(int imol, float specular_strength, float shininess) {

   if (is_valid_map_molecule(imol)) {
      molecule_class_info_t &m = graphics_info_t::molecules[imol];
      m.material_for_maps.specular_strength = specular_strength;
      m.material_for_maps.shininess         = shininess;
      graphics_draw();
   }

}

void set_model_material_specular(int imol, float specular_strength, float shininess) {

   if (is_valid_model_molecule(imol)) {
      molecule_class_info_t &m = graphics_info_t::molecules[imol];
      m.material_for_models.specular_strength = specular_strength;
      m.material_for_models.shininess         = shininess;
      graphics_draw();
   }
}

   

/*  ----------------------------------------------------------------------- */
/*                         single-model view */
/*  ----------------------------------------------------------------------- */
/*! \name single-model view */
/* \{ */
/*! \brief put molecule number imol to display only model number imodel */
void single_model_view_model_number(int imol, int imodel) {

   if (is_valid_model_molecule(imol)) {
      graphics_info_t g;
      g.molecules[imol].single_model_view_model_number(imodel);
      graphics_draw();
      std::string s = "Model number ";
      s += coot::util::int_to_string(imodel);
      add_status_bar_text(s.c_str());
   } 
}

/*! \brief the current model number being displayed */
int single_model_view_this_model_number(int imol) {
   int r = 0;
   if (is_valid_model_molecule(imol)) {
      graphics_info_t g;
      r = g.molecules[imol].single_model_view_this_model_number();
      std::string s = "Model number ";
      s += coot::util::int_to_string(r);
      add_status_bar_text(s.c_str());
      graphics_draw();
   }
   return r;
}

/*! \brief the next model number to be displayed */
int single_model_view_next_model_number(int imol) {
   int r = 0;
   if (is_valid_model_molecule(imol)) {
      graphics_info_t g;
      r = g.molecules[imol].single_model_view_next_model_number();
      std::string s = "Model number ";
      s += coot::util::int_to_string(r);
      add_status_bar_text(s.c_str());
      graphics_draw();
   }
   return r;
}

/*! \brief the previous model number to be displayed */
int single_model_view_prev_model_number(int imol) {
   int r = 0;
   if (is_valid_model_molecule(imol)) {
      graphics_info_t g;
      r = g.molecules[imol].single_model_view_prev_model_number();
      std::string s = "Model number ";
      s += coot::util::int_to_string(r);
      add_status_bar_text(s.c_str());
      graphics_draw();
   }
   return r;
}

