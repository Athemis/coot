

#ifndef C_INTERFACE_LIGANDS_SWIG_HH
#define C_INTERFACE_LIGANDS_SWIG_HH

#include "probe-clash-score.hh"
#include "ligand-check.hh"

// We don't need to SWIG this one...
std::pair<mmdb::Residue *, int>
new_molecule_sans_biggest_ligand(int imol);


#ifdef USE_GUILE
// Create a new molecule, which is a copy of this molecule without the
// biggest hetgroup.
// 
// return a list of the new molecule number and the spec of the removed residue 
// (or scheme false).
// 
SCM new_molecule_sans_biggest_ligand_scm(int imol);
void gui_ligand_metrics_scm(SCM ligand_spec, SCM ligand_metrics, double percentile_limit);

#endif

#ifdef USE_PYTHON
PyObject *new_molecule_sans_biggest_ligand_py(int imol);
void gui_ligand_metrics_py(PyObject *ligand_spec, PyObject *ligand_metrics, double percentile_limit);
#endif


coot::probe_clash_score_t
probe_clash_score(const std::string &dots_file_name);


#ifdef USE_GUILE
bool
residues_torsions_match_scm(int imol_1, SCM res_1,
			    int imol_2, SCM res_2,
			    float tolerance); // in degrees
#endif // USE_GUILE

#ifdef USE_PYTHON
bool
residues_torsions_match_py(int imol_1, PyObject *res_1,
			   int imol_2, PyObject *res_2,
			   float tolerance); // in degrees
#endif // USE_PYTHON

#ifdef USE_GUILE
double kolmogorov_smirnov_scm(SCM l1, SCM l2);
#endif

#ifdef USE_GUILE
SCM kullback_liebler_scm(SCM l1, SCM l2);
#endif

#ifdef USE_PYTHON
double kolmogorov_smirnov_py(PyObject *l1, PyObject *l2);
#endif

#ifdef USE_PYTHON
PyObject *kullback_liebler_py(PyObject *l1, PyObject *l2);
#endif

// Returning void ATM.  We shoud return an interesting object at some
// stage. Perhaps a coot::geometry_distortion_info_container_t?
//
void
print_residue_distortions(int imol, std::string chain_id, int res_no, std::string ins_code);
void
display_residue_distortions(int imol, std::string chain_id, int res_no, std::string ins_code);

void display_residue_hydrogen_bond_atom_status_using_dictionary(int imol, std::string chain_id, int res_no,
								std::string ins_code);
void
write_dictionary_from_residue(int imol, std::string chain_id, int res_no, std::string ins_code, std::string cif_file_name);
void 
add_dictionary_from_residue(int imol, std::string chain_id, int res_no, std::string ins_code);

void
invert_chiral_centre(int imol, std::string chain_id, int res_no, std::string ins_code, std::string atom_name);

// Read cif_dict_in, match the atoms there-in to those of the dictionary of reference_comp_id.
// Write a new dictionary to cif_dict_out.
// 
// Return 1 if was successful in doing the atom matching and writing the cif file.
int
match_residue_and_dictionary(int imol, std::string chain_id, int res_no, std::string ins_code,
			     std::string cif_dict_in,
			     std::string cif_dict_out,
			     std::string cif_dict_comp_id,
			     std::string reference_comp_id);


#endif // C_INTERFACE_LIGANDS_SWIG_HH
