

#ifndef GM_INFO_T_HH
#define GM_INFO_T_HH

// is this a molecular mesh generator?

#include <vector>
#include <chrono>
#include <string>
#include <map>
#include <mmdb2/mmdb_manager.h>

#include "generic-vertex.hh"
#include "g_triangle.hh"


class molecular_mesh_generator_t {
   std::vector<glm::vec3> generate_spline_points(std::vector<glm::vec3> &control_points, unsigned int n=20);
   void smooth_vertices(std::vector<s_generic_vertex> *v_p, unsigned int idx_begin, unsigned int idx_end);
   float get_torsion_angle(const std::vector<glm::vec3> &cis_pep_quad) const;
   std::vector<std::vector<std::pair<mmdb::Residue *, glm::vec3> > > make_CA_fragments(mmdb::Manager *mol) const;
   std::pair<std::vector<s_generic_vertex>, std::vector<g_triangle> >
   dodec_at_position(float scale, const glm::vec3 &position) const;
   void add_to_mesh(std::pair<std::vector<s_generic_vertex>, std::vector<g_triangle> > *vp, // update vp
                    const std::pair<std::vector<s_generic_vertex>, std::vector<g_triangle> > &vp_new) const;
   void add_to_mesh(std::pair<std::vector<s_generic_vertex>, std::vector<g_triangle> > *vp, // update vp
                    const std::vector<s_generic_vertex> &gv, std::vector<g_triangle> &tris) const;

   std::pair<std::vector<s_generic_vertex>, std::vector<g_triangle> >
   molecular_representation_instance_to_mesh(std::shared_ptr<MolecularRepresentationInstance> mol_rep_inst);
   int get_max_resno_for_polymer(mmdb::Chain *chain_p) const;
   std::vector<std::pair<std::string, std::string> > selection_colours;

public:
   molecular_mesh_generator_t() {
      bool do_worm = false;
      t_previous = std::chrono::high_resolution_clock::now();
      t_start = t_previous;
      if (do_worm) {
      } else {
         fill_atom_positions();
         update_mats_and_colours();
      }
   }
   std::chrono::time_point<std::chrono::high_resolution_clock> t_previous;
   std::chrono::time_point<std::chrono::high_resolution_clock> t_start;
   std::vector<glm::vec3> atom_positions;
   std::vector<glm::mat4> bond_instance_matrices;
   std::vector<glm::vec4> bond_instance_colours;
   std::vector<float> original_bond_lengths;
   std::vector<glm::mat4> atom_instance_matrices;
   std::vector<glm::vec4> atom_instance_colours;
   // the atoms that contribute to the ith GM restraint
   std::map<unsigned int, std::tuple<unsigned int, unsigned int, float, bool> > bond_atom_pair_indices;
   std::vector<std::tuple<unsigned int, unsigned, float> > atom_atom_pair_indices;
   void fill_atom_positions();
   void update_mats_and_colours();
   void move_the_atoms_and_update_the_instancing_matrices();
   std::pair<std::vector<s_generic_vertex>, std::vector<g_triangle> > get_cis_peptides(const std::string &pdb_file_name);
   std::pair<std::vector<s_generic_vertex>, std::vector<g_triangle> > get_test_cis_peptides();
   std::pair<std::vector<s_generic_vertex>, std::vector<g_triangle> > make_cis_peptide_geom(const std::vector<glm::vec3> &cis_pep_quad);
   std::pair<std::vector<s_generic_vertex>, std::vector<g_triangle> > get_test_twisted_trans_peptides();
   std::pair<std::vector<s_generic_vertex>, std::vector<g_triangle> > make_twisted_trans_peptide_geom(const std::vector<glm::vec3> &cis_pep_quad);
   std::pair<std::vector<s_generic_vertex>, std::vector<g_triangle> > get_worm_mesh(std::string pdb_file_name);
   std::pair<std::vector<s_generic_vertex>, std::vector<g_triangle> > get_test_molecular_triangles_mesh(mmdb::Manager *mol);

   std::pair<std::vector<s_generic_vertex>, std::vector<g_triangle> >
   get_molecular_triangles_mesh(mmdb::Manager *mol,
                                const std::string &selection_string, // mmdb-format
                                const std::string &colour_scheme,
                                const std::string &style);
   std::pair<std::vector<s_generic_vertex>, std::vector<g_triangle> >
   get_molecular_triangles_mesh(mmdb::Manager *mol,
                                mmdb::Chain *chain_p,
                                const std::string &colour_scheme,
                                const std::string &style);
   void add_selection_and_colour(const std::string &sel, const std::string &col);
};

#endif // GM_INFO_T

