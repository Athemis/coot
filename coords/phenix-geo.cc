
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "utils/coot-utils.hh"
#include "phenix-geo.hh"

coot::atom_spec_t
coot::phenix_geo_bonds::parse_line_for_atom_spec(const std::string &l) const {

   atom_spec_t atom_spec;
   // std::cout << "---" << l << std::endl;
   std::string atom_name_1(l.substr(10,4));
   std::string chain_id(l.substr(19,1));
   unsigned int post_chain_id_char_idx = 20;

   // Fix multi-char chain-ids here with a while() {}
		     
   unsigned int llen = l.length();
   unsigned int residue_number_str_idx = post_chain_id_char_idx + 1; // start
   std::string residue_number_str;
   while (residue_number_str_idx < llen) {
      char c = l[residue_number_str_idx];
      if (c >= '0' && c <= '9') { 
	 residue_number_str += c;
      } else {
	 if (residue_number_str.length())
	    break;
      }
      residue_number_str_idx++;
   }
   try {
      int res_no = util::string_to_int(residue_number_str);
      atom_spec = atom_spec_t(chain_id, res_no, "", atom_name_1, "");
   }
   catch (const std::runtime_error &rte) {
      // parse fail. Heyho.
   }
   return atom_spec;
   
}

coot::phenix_geo_bonds::phenix_geo_bonds(const std::string &file_name) {

   if (file_exists(file_name)) {

      std::ifstream f(file_name.c_str());

      if (f) {
	 std::vector<std::string> lines;
	 std::string line;
	 while (std::getline(f, line)) { 
	    lines.push_back(line);
	 }
	 if (lines.size() > 0) {
	    unsigned int line_idx = 0;
	    while (line_idx < lines.size()) {
	       const std::string &l = lines[line_idx];
	       if (l.length() > 22) {
		  if (l.substr(0,4) == "bond") {
		     atom_spec_t atom_spec_1 = parse_line_for_atom_spec(l);
		     if (! atom_spec_1.empty()) {
			const std::string &l2 = lines[line_idx+1];
			atom_spec_t atom_spec_2 = parse_line_for_atom_spec(l2);
			if (! atom_spec_2.empty()) {
			   const std::string &lg = lines[line_idx+3];
			   std::vector<std::string> bits = util::split_string_no_blanks(lg);
			   if (bits.size() == 6) {
			      try {
				 std::vector<double> v(6,-1);
				 for (unsigned int i=0; i<6; i++) { 
				    v[i] = util::string_to_float(bits[i]);
				 }
				 phenix_geo_bond gb(atom_spec_1, atom_spec_2);
				 gb.set_geom(v[0], v[2], v[2], v[3], v[4], v[5]);
				 bonds.push_back(gb);
			      }
			      catch(const std::runtime_error &rte) {
				 // number parse fail.
				 std::cout << "number parse fail " << lg << std::endl;
			      } 
			   } 
			} 
		     }
		  }
	       }
	       line_idx++; // next
	    }
	 } 
      }
   }
   std::cout << "found " << bonds.size() << " bonds" << std::endl;
}

// 	    
