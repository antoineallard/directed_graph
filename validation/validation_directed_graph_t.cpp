/*
 *
 *  This code generates the text files used to validate the directed_graph_t
 *    class of the PGL to analyze the structure of directed, unweighted and
 *    simple graph.
 *
 *  Compilation: g++ -O3 validation_directed_graph_t.cpp
 *
 *  Author:  Antoine Allard
 *  WWW:     antoineallard.info
 *  Date:    August 2020
 *
 *
 *  Copyright (C) 2020 Antoine Allard
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */


 // Standard template library
 #include <iomanip>
 #include <fstream>
 #include <string>
 // Portable graph library
 #include "../src/directed_graph_t.hpp"


 int main(int argc, char const *argv[])
 {
  // Name of the file containing the edgelist.
  std::string edgelist_filename = argv[1];

  // Creates the empty graph object.
  pgl::directed_graph_t g;

  // Loads the Dummy integer vertex property.
  g.load_vertices_properties("validation_v_prop_networkx.dat", 3, "DummyInt", "DummyInt", false);

  // Loads the edgelist.
  g.load_graph_from_edgelist_file(edgelist_filename);

  // Loads the Dummy double vertex property.
  g.load_vertices_properties("validation_v_prop_networkx.dat", 4, "DummyDbl");

  // Computes the degrees.
  g.compute_degrees();

  // Computes the average in-/out-degree.
  g.compute_average_vertex_prop("in-degree");
  g.compute_average_vertex_prop("out-degree");

  // Surveys the triangles and computes the undirected local clustering coefficients.
  g.survey_triangles(true, true);

  // Computes the average undirected clustering coefficient.
  g.compute_average_vertex_prop("undir_local_clust");

  // Compiles the triangle spectrum.
  g.compile_triangle_spectrum();

  // Computes the reciprocity.
  g.compute_reciprocity();


  // Writes the graph properties into a text file.
  std::ofstream g_prop_file("validation_g_prop_pgl.dat");
  std::ios_base::fmtflags g_prop_file_flags( g_prop_file.flags() );
  g_prop_file << std::setw(50) << "Number of vertices:" << " ";
  g_prop_file << std::setw(50) << g.g_prop["nb_vertices"] << " " << std::endl;
  g_prop_file << std::setw(50) << "Number of edges:" << " ";
  g_prop_file << std::setw(50) << g.g_prop["nb_edges"] << " " << std::endl;
  g_prop_file << std::setw(50) << "Number of edges:" << " ";
  g_prop_file << std::setw(50) << g.edgelist.size() << " " << std::endl;
  g_prop_file << std::setw(50) << "Density:" << " ";
  g_prop_file << std::setw(50) << std::fixed << std::setprecision(3) << g.g_prop["density"] << " " << std::endl;
  g_prop_file << std::setw(50) << "Average in-degree:" << " ";
  g_prop_file << std::setw(50) << std::fixed << std::setprecision(3) << g.g_prop["avg_in-degree"] << " " << std::endl;
  g_prop_file << std::setw(50) << "Average out-degree:" << " ";
  g_prop_file << std::setw(50) << std::fixed << std::setprecision(3) << g.g_prop["avg_out-degree"] << " " << std::endl;
  g_prop_file.flags( g_prop_file_flags );
  g_prop_file << std::setw(50) << "Number of reciprocal edges:" << " ";
  g_prop_file << std::setw(50) << g.g_prop["nb_reciprocal_edges"] << " " << std::endl;
  g_prop_file << std::setw(50) << "Reciprocity (ratio definition):" << " ";
  g_prop_file << std::setw(50) << std::fixed << std::setprecision(3) << g.g_prop["reciprocity_ratio"] << " " << std::endl;
  g_prop_file << std::setw(50) << "Reciprocity (statistical definition):" << " ";
  g_prop_file << std::setw(50) << std::fixed << std::setprecision(3) << g.g_prop["reciprocity_stat"] << " " << std::endl;
  g_prop_file.unsetf(std::ios_base::floatfield);
  g_prop_file << std::setw(50) << "Number of triangles:" << " ";
  g_prop_file << std::setw(50) << std::fixed << std::setprecision(0) << g.g_prop["nb_triangles"] << " " << std::endl;
  g_prop_file << std::setw(50) << "Average undirected local clustering coefficient:" << " ";
  g_prop_file << std::setw(50) << std::fixed << std::setprecision(3) << g.g_prop["avg_undir_local_clust"] << " " << std::endl;
  g_prop_file << std::setw(50) << "Undirected global clustering coefficient:" << " ";
  g_prop_file << std::setw(50) << std::fixed << std::setprecision(3) << g.g_prop["undir_global_clust"] << " " << std::endl;
  g_prop_file.close();


  // Writes the vertex properties into a text file.
  std::string p[] = {"in-degree", "out-degree", "DummyInt", "DummyDbl", "undir_local_clust"};
  std::vector<std::string> props(p, p+5);
  g.save_vertices_properties("validation_v_prop_pgl.dat", props);


  // Lists the triangles into a text file.
  std::set<std::string> triangles_vertices;
  std::set<std::string>::iterator triangles_vertices_it, triangles_vertices_end;
  std::set< std::set<std::string> > triangles;
  std::set< std::set<std::string> >::iterator triangles_it, triangles_end;
  for(int t(0), tt(g.g_prop["nb_triangles"]), v0, v1, v2; t<tt; ++t)
  {
    // Identifies the vertices and sorts them.
    triangles_vertices.clear();
    triangles_vertices.insert(g.ID2Name[g.triangles[t][0]]);
    triangles_vertices.insert(g.ID2Name[g.triangles[t][1]]);
    triangles_vertices.insert(g.ID2Name[g.triangles[t][2]]);
    triangles.insert(triangles_vertices);
  }
  // Writes the triangles into the file.
  std::ofstream triangles_file("validation_triangles_pgl.dat");
  triangles_it = triangles.begin();
  triangles_end = triangles.end();
  for(; triangles_it!=triangles_end; ++triangles_it)
  {
    triangles_vertices_it = triangles_it->begin();
    triangles_vertices_end = triangles_it->end();
    for(; triangles_vertices_it!=triangles_vertices_end; ++triangles_vertices_it)
    {
      triangles_file << std::setw(25) << *triangles_vertices_it << " ";
    }
    triangles_file << std::endl;
  }
  triangles_file.close();


  // Writes the triangle spectrum into a text file.
  std::ofstream triangle_spect_file("validation_triangle_spect_pgl.dat");
  triangle_spect_file << std::setw(12) << "3cycle" << " ";
  triangle_spect_file << std::setw(12) << g.triangle_spect["3cycle"] << " ";
  triangle_spect_file << std::endl;
  triangle_spect_file << std::setw(12) << "3nocycle" << " ";
  triangle_spect_file << std::setw(12) << g.triangle_spect["3nocycle"] << " ";
  triangle_spect_file << std::endl;
  triangle_spect_file << std::setw(12) << "4cycle" << " ";
  triangle_spect_file << std::setw(12) << g.triangle_spect["4cycle"] << " ";
  triangle_spect_file << std::endl;
  triangle_spect_file << std::setw(12) << "4outward" << " ";
  triangle_spect_file << std::setw(12) << g.triangle_spect["4outward"] << " ";
  triangle_spect_file << std::endl;
  triangle_spect_file << std::setw(12) << "4inward" << " ";
  triangle_spect_file << std::setw(12) << g.triangle_spect["4inward"] << " ";
  triangle_spect_file << std::endl;
  triangle_spect_file << std::setw(12) << "5cycle" << " ";
  triangle_spect_file << std::setw(12) << g.triangle_spect["5cycle"] << " ";
  triangle_spect_file << std::endl;
  triangle_spect_file << std::setw(12) << "6cycle" << " ";
  triangle_spect_file << std::setw(12) << g.triangle_spect["6cycle"] << " ";
  triangle_spect_file << std::endl;
  triangle_spect_file.close();




  return 0;
 }
