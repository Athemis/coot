
#ifndef MESH_HH
#define MESH_HH

#include "generic-vertex.hh"
#include "g_triangle.hh"

#include "lights-info.hh"
#include "Shader.hh"
#include "Material.hh"
#include "Particle.hh"


class Mesh {
   void setup_debugging_instancing_buffers(); // or buffers, when we add rotation
   Material material;
   void setup_instanced_balls( Shader *shader_p, const Material &material_in);
   void setup_instanced_dodecs(Shader *shader_p, const Material &material_in);
   void setup_buffers();
   // rts rotation, translation & scale
   void setup_matrix_and_colour_instancing_buffers(const std::vector<glm::mat4> &mats, const std::vector<glm::vec4> &colours);
   int n_instances;
   bool normals_are_setup;
   GLuint normals_vao;
   GLuint normals_buffer_id;
   GLuint normals_colour_buffer_id;
   void init();

public:
   GLuint vao;
   GLuint buffer_id;
   GLuint inst_rts_buffer_id;
   GLuint inst_model_translation_buffer_id;
   GLuint inst_colour_buffer_id;
   GLuint index_buffer_id;
   bool this_mesh_is_closed;
   bool draw_this_mesh;
   bool is_instanced;
   bool is_instanced_with_rts_matrix;
   bool use_blending;
   std::vector<s_generic_vertex> vertices;
   std::vector<g_triangle> triangle_vertex_indices;
   Shader shader_for_draw_normals;
   std::string name;

   Mesh() { init(); }
   // import from somewhere else
   Mesh(const std::pair<std::vector<s_generic_vertex>, std::vector<g_triangle> > &indexed_vertices);
   Mesh(const std::string &name_in) : name(name_in) {
      init();
   }

   void debug() const;
   void clear() {
      // delete some gl buffers here
      is_instanced = false;
      is_instanced_with_rts_matrix = false;
      vertices.clear();
      triangle_vertex_indices.clear();
      use_blending = false;
      normals_are_setup = false;
   }
   void close();
   void set_draw_mesh_state(bool state) { draw_this_mesh = state; }
   void set_name(const std::string &n) { name = n; }
   void import(const std::pair<std::vector<s_generic_vertex>, std::vector<g_triangle> > &indexed_vertices);
   void import(const std::vector<s_generic_vertex> &gv, const std::vector<g_triangle> &indexed_vertices);
   void setup(Shader *shader_p, const Material &material_in);
   // can be considered as "draw_self()"
   void draw(Shader *shader,
             const glm::mat4 &mvp,
             const glm::mat4 &view_rotation_matrix,
             const std::map<unsigned int, lights_info_t> &lights,
             const glm::vec3 &eye_position, // eye position in view space (not molecule space)
             const glm::vec4 &background_colour,
             bool do_depth_fog);
   void draw_particles(Shader *shader_p, const glm::mat4 &mvp);
   void draw_normals(const glm::mat4 &mvp, float normal_scaling); // debugging

   // testing/example functions
   void setup_rama_balls(Shader *shader_p, const Material &material_in); // call fill_rama_balls() and setup_buffers()
   void setup_simple_triangles(Shader *shader_p, const Material &material_in);
   // setup_simple_triangles() calls fill_with_simple_triangles_vertices()
   void fill_with_simple_triangles_vertices();
   void fill_with_direction_triangles();
   void setup_instanced_debugging_objects(Shader *shader_p, const Material &material_in);

   // this is an import function, name it so.
   void setup_instanced_cylinders(Shader *shader_p,
                                  const Material &material_in,
                                  const std::vector<glm::mat4> &mats,
                                  const std::vector<glm::vec4> &colours);
   // this is an import function, name it so.
   void setup_instanced_octahemispheres(Shader *shader_p,
                                        const Material &material_in,
                                        const std::vector<glm::mat4> &mats,
                                        const std::vector<glm::vec4> &colours);
   void setup_camera_facing_outline();
   void setup_camera_facing_quad();
   void setup_camera_facing_hex();
   void setup_camera_facing_polygon(unsigned int n_sides = 8);

   // when the position, orientation and colour change:
   void update_instancing_buffer_data(const std::vector<glm::mat4> &mats,
                                      const std::vector<glm::vec4> &colours);
   // when the positions change:
   void update_instancing_buffer_data(const std::vector<glm::mat4> &mats);

   // void setup_instancing_buffers(const particle_container_t &particles);
   void setup_instancing_buffers(unsigned int n_particles); // setup the buffer, don't add data
   void update_instancing_buffer_data(const particle_container_t &particles);
   
   void fill_rama_balls(); // make up some balls
   void add_one_ball(float scale, const glm::vec3 &centre);
   void add_one_origin_ball();
   void add_one_origin_cylinder(unsigned int nslices=8, unsigned int n_stacks=2);
   void add_one_origin_octasphere(unsigned int num_subdivisions=1); // default matches 8 sided cylinder
   void add_one_origin_octahemisphere(unsigned int num_subdivisions=1); // default matches 8 sided cylinder
   void add_one_origin_dodec();
   void fill_one_dodec();
   void flatten_triangles(); // needs implementation (will generate new vertices).
   void smooth_triangles();  // needs implementation.
   bool is_closed() const { return this_mesh_is_closed; }

};

#endif
