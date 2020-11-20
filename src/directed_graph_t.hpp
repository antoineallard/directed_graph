#ifndef __DIRECTED_GRAPH_T_HPP__
#define __DIRECTED_GRAPH_T_HPP__

/*
 *
 *  This file contains the complete source code of the directed_graph_t class of the PGL
 *    package to analyze the structure of directed, unweighted and simple graph. While having
 *    one single file is not the most clear and organized choice for source codes, this format has
 *    been chosen to faciliate the portability of the code.
 *
 *  Compilation example: g++ -O3 my_code.cpp
 *
 *  Author:  Antoine Allard
 *  WWW:     antoineallard.info
 *  Date:    August 2019
 *
 *  Version: 1.0
 *
 *
 *  Copyright (C) 2019 Antoine Allard
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

// Standard Template Library
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>





namespace pgl
{
  class directed_graph_t
  {
    // =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
    // =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
    // Internal storage of the graph object.
    public:
      // Name to ID conversion. Every vertex is assigned a numerical ID in [0, |V|).
      std::map<std::string, int> Name2ID;
      // Edgelist.
      std::set< std::pair<int, int> > edgelist;
      // Adjacency list.
      std::vector< std::vector< std::vector<int> > > adjacency_list;
      // Indices for the in and out degree in the adjacency list.
      static const int InDegreeIdx = 0;
      static const int OutDegreeIdx = 1;
    // =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
    // =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
    // Typedefs and accessors.
    public:
      typedef std::set< std::pair<int, int> >::iterator edgelist_iterator;
      edgelist_iterator edgelist_begin() { return edgelist.begin(); }
      edgelist_iterator edgelist_end()   { return edgelist.end();   }
    // =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
    // =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
    // Objects/functions related to outputs.
    public:
      // Custom types to indicate how to identify vertices in outputs.
      enum vID_t { vID_none, vID_num, vID_name };
      static const bool header_true = true;
      static const bool header_false = false;
      // ID to name conversion.
      std::vector< std::string > ID2Name;
      // Build the ID2Name vector.
      void build_ID2Name();
    private:
      // Default width of columns.
      static const int default_column_width = 15;
      // Available vertex properties.
      std::set< std::string > available_vertex_prop;
      // Available vertex integer properties.
      std::set< std::string > available_vertex_integer_prop;
      // Headers associated with vertex properties.
      std::map< std::string, std::string > v_prop_header;
    // =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
    // =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
    // Properties.
    public:
      // Properties of the graph.
      std::map< std::string, double> g_prop;
      // Properties of vertices.
      std::map< std::string, std::vector<double> > v_prop;
    //   // Statistics of vertices.
    //   std::map< std::string, std::map<int, double> > v_stat;
      // List of all triangles.
      std::vector< std::vector<int> > triangles;
      // Triangle spectrum histogram.
      std::map<std::string, int> triangle_spect;
    private:
      // Function verifying whether a vertex property exists.
      void is_vertex_property(std::string prop);
      // Function verifying wether a vertex property qualifies as a "integer" property.
      bool is_vertex_integer_property(std::string prop, bool exit_on_error);
    // =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
    // =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
    // Functions loading/saving edgelists and vertices/edges properties.
    public:
      // Functions to add vertices and/or edges.
      int add_vertex(std::string name_str, bool ignore_unknown_vertices = false);
      bool add_edge(std::string name1_str, std::string name2_str, bool ignore_unknown_vertices = false);
      bool add_edge(int v1, int v2);
    public:
      // Loads the graph structure from an edgelist in a file.
      void load_graph_from_edgelist_file(std::string edgelist_filename);
      // Outputing the vertices properties (the last 3 inputs can be omitted and/or put in any order).
      void save_vertices_properties(std::string filename, std::vector<std::string> props_id, vID_t vID = vID_name, int width = default_column_width, bool header = header_true);
      void save_vertices_properties(std::string filename, std::vector<std::string> props_id, vID_t vID,            bool header,                      int width = default_column_width)                { save_vertices_properties(filename, props_id, vID, width, header); };
      void save_vertices_properties(std::string filename, std::vector<std::string> props_id, bool header,          vID_t vID = vID_name,             int width = default_column_width)                { save_vertices_properties(filename, props_id, vID, width, header); };
      void save_vertices_properties(std::string filename, std::vector<std::string> props_id, bool header,          int width,                        vID_t vID = vID_name)                            { save_vertices_properties(filename, props_id, vID, width, header); };
      void save_vertices_properties(std::string filename, std::vector<std::string> props_id, int width,            vID_t vID = vID_name,             bool header = header_true)                       { save_vertices_properties(filename, props_id, vID, width, header); };
      void save_vertices_properties(std::string filename, std::vector<std::string> props_id, int width,            bool header,                      vID_t vID = vID_name)                            { save_vertices_properties(filename, props_id, vID, width, header); };
      // Loads vertices properties from a file.
      void load_vertices_properties(std::string prop_filename, int usecol, std::string prop, std::string prop_header = "", bool ignore_unknown_vertices = true);
      // Adding new vertex (integer) properties.
      void add_new_vertex_integer_property(std::string prop, std::string prop_header = "");
      void add_new_vertex_property(std::string prop, std::string prop_header = "");
    // =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
    // =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
    // Functions building secondary objects related to the graph.
    public:
      // Builds the adjacency list.
      void build_adjacency_list();
    // =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
    // =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
    // Functions extracting properties of graph/vertices/edges.
    private:
      // Function checking if directed edge already exists.
      bool is_edge(int v_source, int v_target);
    public:
      // Computes the average value of a vertex property.
      void compute_average_vertex_prop(std::string prop);
      // Extracts the in-/out-degree of vertices.
      void compute_degrees();
      // Computes the density of the graph.
      void compute_density();
      // Computes the reciprocity.
      void compute_reciprocity();
      // Compiles a list of all triangles in the graph.
      void survey_triangles(bool build_triangle_list = true, bool compute_undirected_local_clustering = true);
      // Compiles the number of every unique triangle configurations.
      void compile_triangle_spectrum();
    // =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
    // =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
    // Constructors (and related functions).
    private:
      // Function setting default values (to avoid requiring the C++11 standard).
      void initialization();
    public:
      // Empty constructor.
      directed_graph_t() { initialization(); };
      // Constructor with edgelist.
      directed_graph_t(std::string edgelist_filename) { initialization(); load_graph_from_edgelist_file(edgelist_filename); };
      // =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
      // =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
      // Convert to python.
      public:
        // Graph properties.
        int get_nb_vertices();
        int get_nb_edges();
        int get_nb_reciprocal_edges();
        double get_reciprocity();
        // // Vertex properties.
        // std::map<std::string, double> get_local_reciprocity();
  };
}










// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
void pgl::directed_graph_t::initialization()
{
  // Available vertex properties.
  available_vertex_prop.insert("in-degree");
  available_vertex_prop.insert("out-degree");
  available_vertex_prop.insert("reciprocal_degree");
  available_vertex_prop.insert("reciprocity_jaccard");
  available_vertex_prop.insert("reciprocity_ratio");
  available_vertex_prop.insert("undir_local_clust");
  // Available vertex integer properties.
  available_vertex_integer_prop.insert("in-degree");
  available_vertex_integer_prop.insert("out-degree");
  // Headers for vertex properties.
  v_prop_header["in-degree"]  = "InDeg";
  v_prop_header["out-degree"] = "OutDeg";
  v_prop_header["reciprocal_degree"] = "RecDeg";
  v_prop_header["reciprocity_jaccard"] = "ReciproJacc";
  v_prop_header["reciprocity_ratio"] = "ReciproRatio";
  v_prop_header["undir_local_clust"] = "UnLocalClust";
  // Graph properties.
  g_prop["nb_vertices"] = 0;
  g_prop["nb_edges"] = 0;
  g_prop["density"] = 0;
  // Graph properties initialized to "impossible" values to check if it has been computed.
  g_prop["nb_reciprocal_edges"] = -1;
  g_prop["reciprocity_ratio"] = -1;
  g_prop["reciprocity_stat"] = -1;
  g_prop["nb_triangles"] = -1;

}


// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// Structure containing the desired method to compared strings (put shorter ones before longer ones).
struct compare_names_directed_graph_t
{
  bool operator()(const std::pair<std::string, int>& lhs, const std::pair<std::string, int>& rhs) const
  {
    if(lhs.first.size() == rhs.first.size())
    {
      if(lhs.first == rhs.first)
      {
        return lhs.second < rhs.second;
      }
      else
      {
        return lhs.first < rhs.first;
      }
    }
    else
    {
      return lhs.first.size() < rhs.first.size();
    }
  }
};










// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// 0. INTERNAL MANAGMENT: GENERATING SECONDARY OBJECTS
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=


// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
void pgl::directed_graph_t::build_adjacency_list()
{
  // ===============================================================================================
  // Initializes relevant objects of the class.
  int nb_vertices = g_prop["nb_vertices"];
  adjacency_list.clear();
  adjacency_list.resize(nb_vertices, std::vector< std::vector<int> >(2));
  // ===============================================================================================

  // Loops over all edges.
  int v1, v2;
  std::set< std::pair<int, int> >::iterator it = edgelist.begin();
  std::set< std::pair<int, int> >::iterator end = edgelist.end();
  for(; it!=end; ++it)
  {
    // Identifies the vertices.
    v1 = it->first;
    v2 = it->second;
    // Adds the percolated edge.
    adjacency_list[v1][OutDegreeIdx].push_back(v2);
    adjacency_list[v2][InDegreeIdx].push_back(v1);
  }
}


// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
void pgl::directed_graph_t::build_ID2Name()
{
  // ===============================================================================================
  // Initializes relevant objects of the class.
  int nb_vertices = g_prop["nb_vertices"];

  ID2Name.clear();
  ID2Name.resize(nb_vertices);
  // ===============================================================================================

  // Loops over all names.
  std::map<std::string, int>::iterator it = Name2ID.begin();
  std::map<std::string, int>::iterator end = Name2ID.end();
  for(; it!=end; ++it)
  {
    ID2Name[it->second] = it->first;
  }
}










// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// 0. ADDING AND CHECKING THE EXISTENCE OF VERTICES AND EDGES
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=


// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
bool pgl::directed_graph_t::is_edge(int v_source, int v_target)
{
  return std::find(adjacency_list[v_source][OutDegreeIdx].begin(),
                   adjacency_list[v_source][OutDegreeIdx].end(),
                   v_target) != adjacency_list[v_source][OutDegreeIdx].end();
}


// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
int pgl::directed_graph_t::add_vertex(std::string name_str, bool ignore_unknown_vertices)
{
  // Numerical ID of the vertex.
  int v;
  // Is the vertex new?
  std::map< std::string, int >::iterator name_it = Name2ID.find(name_str);
  if(name_it == Name2ID.end())
  {
    if(ignore_unknown_vertices)
    {
      v = g_prop["nb_vertices"];
    }
    else
    {
      v = g_prop["nb_vertices"];
      Name2ID[name_str] = v;
      g_prop["nb_vertices"] = v + 1;
    }
  }
  // If not, fetch its already assigned numerical ID.
  else
  {
    v = name_it->second;
  }
  // Returns the numerical ID of the vertex.
  return v;
}


// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
bool pgl::directed_graph_t::add_edge(std::string name1_str, std::string name2_str, bool ignore_unknown_vertices)
{
  // Ignores self-loops.
  if(name1_str == name2_str)
  {
    return false;
  }

  // Source vertex.
  int v1 = add_vertex(name1_str, ignore_unknown_vertices);
  if(ignore_unknown_vertices)
  {
    if(v1 == g_prop["nb_vertices"])
    {
      return false;
    }
  }

  // Target vertex.
  int v2 = add_vertex(name2_str, ignore_unknown_vertices);
  if(ignore_unknown_vertices)
  {
    if(v2 == g_prop["nb_vertices"])
    {
      return false;
    }
  }

  // Adds the edge (multiedges are automatically ignored).
  bool edge_added = (edgelist.insert(std::make_pair(v1, v2))).second;
  if(edge_added)
  {
    g_prop["nb_edges"] += 1;
  }
  return edge_added;
}


// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
bool pgl::directed_graph_t::add_edge(int v1, int v2)
{
  // Ignores self-loops.
  if(v1 == v2)
  {
    return false;
  }

  // Does not add the edge if vertices do not exist.
  if(v1 >= g_prop["nb_vertices"])
  {
    return false;
  }
  if(v2 >= g_prop["nb_vertices"])
  {
    return false;
  }

  // Adds the edge (multiedges are automatically ignored).
  bool edge_added = (edgelist.insert(std::make_pair(v1, v2))).second;
  if(edge_added)
  {
    g_prop["nb_edges"] += 1;
  }
  return edge_added;
}










// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// 0. MANAGING VERTEX PROPERTIES
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=


// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
bool pgl::directed_graph_t::is_vertex_integer_property(std::string prop, bool exit_on_error)
{
  bool is_not = available_vertex_integer_prop.find(prop) == available_vertex_integer_prop.end();
  if(exit_on_error)
  {
    if(is_not)
    {
      std::cerr << "ERROR: " << prop << " is not a valid vertex integer property." << std::endl;
      std::terminate();
    }
  }
  return !is_not;
}


// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
void pgl::directed_graph_t::is_vertex_property(std::string prop)
{
  if(available_vertex_prop.find(prop) == available_vertex_prop.end())
  {
    std::cerr << "ERROR: " << prop << " is not a valid vertex property." << std::endl;
    std::terminate();
  }
}


// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
void pgl::directed_graph_t::add_new_vertex_integer_property(std::string prop, std::string prop_header)
{
  add_new_vertex_property(prop, prop_header);
  if(available_vertex_integer_prop.find(prop) != available_vertex_integer_prop.end())
  {
    std::cerr << "ERROR: Vertex integer property " << prop << " already exists." << std::endl;
    std::terminate();
  }
  available_vertex_integer_prop.insert(prop);
}


// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
void pgl::directed_graph_t::add_new_vertex_property(std::string prop, std::string prop_header)
{
  if(prop_header == "")
  {
    prop_header = prop;
  }
  if(available_vertex_prop.find(prop) != available_vertex_prop.end())
  {
    std::cerr << "ERROR: Vertex property " << prop << " already exists." << std::endl;
    std::terminate();
  }
  available_vertex_prop.insert(prop);
  int nb_vertices = g_prop["nb_vertices"];
  v_prop_header[prop] = prop_header;
  v_prop[prop] = std::vector<double>(nb_vertices, 0);
}









// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// 1. INTERNAL MANAGMENT: AVERAGE VALUES OF VERTEX PROPERTIES
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=


// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
void pgl::directed_graph_t::compute_average_vertex_prop(std::string prop)
{
  // ===============================================================================================
  // Initializes relevant objects of the class.
  int nb_vertices = g_prop["nb_vertices"];
  std::vector<double>& Vertex2Prop = v_prop[prop];
  // ===============================================================================================

  // Checks if the property has been extracted/computed already.
  if(v_prop[prop].size() != nb_vertices)
  {
    std::cerr << "ERROR: The property " << prop << " has not been extracted/computed." << std::endl;
    std::terminate();
  }

  // Computes the average degree of vertices.
  double sum_of_values = 0;
  for(int v(0); v<nb_vertices; ++v)
  {
    sum_of_values += Vertex2Prop[v];
  }

  // ===============================================================================================
  // Updates the properties of the graph.
  std::string avg_v_prop = "avg_" + prop;
  g_prop[avg_v_prop] = sum_of_values / nb_vertices;
  // ===============================================================================================
}










// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// 2. INPUT
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=


// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
void pgl::directed_graph_t::load_graph_from_edgelist_file(std::string edgelist_filename)
{
  // ===============================================================================================
  // Initializes relevant objects of the class.
  edgelist.clear();
  // ===============================================================================================

  // Stream objects.
  std::fstream edgelist_file;
  std::stringstream one_line;
  // String objects.
  std::string full_line, name1_str, name2_str;

  // Opens the stream and terminates if the operation did not succeed.
  edgelist_file.open(edgelist_filename.c_str(), std::fstream::in);
  if( !edgelist_file.is_open() )
  {
    std::cerr << "ERROR: Could not open file: " << edgelist_filename << "." << std::endl;
    std::terminate();
  }
  else
  {
    // Reads the file line by line.
    while( !edgelist_file.eof() )
    {
      // Reads a line of the file.
      std::getline(edgelist_file, full_line);
      edgelist_file >> std::ws;
      one_line.str(full_line);
      one_line >> std::ws;
      one_line >> name1_str >> std::ws;
      // Skips a line of comment.
      if(name1_str == "#")
      {
        one_line.clear();
        continue;
      }
      one_line >> name2_str >> std::ws;
      one_line.clear();

      // Adds the edge.
      add_edge(name1_str, name2_str, false);
    }
  }
  // Closes the stream.
  edgelist_file.close();
  // ===============================================================================================
  // Updates the properties of the graph.
  // g_prop["nb_edges"] = edgelist.size();
  compute_density();
  // ===============================================================================================
}


// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
void pgl::directed_graph_t::load_vertices_properties(std::string prop_filename, int usecol, std::string prop, std::string prop_header, bool ignore_unknown_vertices)
{
  // ===============================================================================================
  // Initializes relevant objects of the class.
  add_new_vertex_property(prop, prop_header);
  std::vector<double>& Vertex2Prop = v_prop[prop];
  // ===============================================================================================
  // Stream object.
  std::stringstream one_line;
  // String objects.
  std::string full_line, name_str, value_str;
  // Variable.
  int idx;
  double tmp_value, dummy;
  bool is_integer = true;
  // Opens the stream and terminates if the operation did not succeed.
  std::fstream prop_file(prop_filename.c_str(), std::fstream::in);
  if( !prop_file.is_open() )
  {
    std::cerr << "Could not open file: " << prop_filename << "." << std::endl;
    std::terminate();
  }
  // Reads the property file line by line.
  while( !prop_file.eof() )
  {
    // Reads a line of the file.
    std::getline(prop_file, full_line);
    prop_file >> std::ws;
    one_line.str(full_line);
    one_line >> std::ws;
    one_line >> name_str >> std::ws;
    // Skips lines of comment.
    // if(name_str == "#")
    if(name_str.compare(0, 1, "#") == 0)
    {
      one_line.clear();
      continue;
    }
    // Identifies the vertex and may ignore unknown vertices.
    idx = add_vertex(name_str, ignore_unknown_vertices);
    if(idx == g_prop["nb_vertices"])
    {
      one_line.clear();
      continue;
    }
    // Inputs the property.
    for(int r(1); r<usecol; ++r)
    {
      one_line >> value_str >> std::ws;
    }
    one_line >> value_str >> std::ws;
    tmp_value = std::atof(value_str.c_str());
    if(Vertex2Prop.size() == idx)
    {
      Vertex2Prop.push_back(tmp_value);
    }
    else
    {
      Vertex2Prop[idx] = tmp_value;
    }
    if( is_integer && (std::modf(tmp_value, &dummy) != 0) )
    {
      is_integer = false;
    }
    // Resets the stringstream.
    one_line.clear();
  }
  // Closes the stream.
  prop_file.close();
  // ===============================================================================================
  // Initializes relevant objects of the class.
  if(is_integer)
  {
    available_vertex_integer_prop.insert(prop);
  }
  // ===============================================================================================
}










// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// 3. OUTPUT
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=

// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
void pgl::directed_graph_t::save_vertices_properties(std::string filename, std::vector<std::string> props_id, vID_t vID, int width, bool header)
{
  // Stream objects.
  std::fstream output_file;
  // Opens the stream and terminates if the operation did not succeed.
  output_file.open(filename.c_str(), std::fstream::out);
  if( !output_file.is_open() )
  {
    std::cerr << "ERROR: Could not open file: " << filename << "." << std::endl;
    std::terminate();
  }

  // ===============================================================================================
  // Initializes relevant objects of the class.
  int nb_vertices = g_prop["nb_vertices"];
  // ===============================================================================================

  // Number of properties.
  int nb_props = props_id.size();

  // Checks whether all properties have been extracted/computed.
  for(int i(0); i<nb_props; ++i)
  {
    is_vertex_property(props_id[i]);
    if(v_prop[props_id[i]].size() != nb_vertices)
    {
      std::cerr << "ERROR: The property " << props_id[i] << " has not been extracted/computed." << std::endl;
      std::terminate();
    }
  }

  // Ensures that the ID2Name has been built. Otherwise, build it.
  if(ID2Name.size() != nb_vertices)
  {
    build_ID2Name();
  }

  // Prints the header (if required).
  if(header)
  {
    int i = 0;
    if(vID == vID_name || vID == vID_num)
    {
      output_file << "#" << std::setw(width - 1) << "Vertex" << " ";
      output_file << std::setw(width) << v_prop_header[props_id[i]] << " ";
      ++i;
    }
    else if(vID == vID_none)
    {
      output_file << "#" << std::setw(width - 1) << v_prop_header[props_id[i]] << " ";
      ++i;
    }
    else
    {
      std::cerr << "ERROR: Unknown vertex identifier type." << std::endl;
      std::terminate();
    }
    for(; i<nb_props; ++i)
    {
      output_file << std::setw(width) << v_prop_header[props_id[i]] << " ";
    }
    output_file << std::endl;
  }

  // Prints the properties.
  std::set< std::pair<std::string, int>, compare_names_directed_graph_t > ordered_names;
  for(int v(0); v<nb_vertices; ++v)
  {
    ordered_names.insert(std::make_pair(ID2Name[v], v));
  }
  std::set< std::pair<std::string, int> >::iterator it  = ordered_names.begin();
  std::set< std::pair<std::string, int> >::iterator end = ordered_names.end();
  for(int v; it!=end; ++it)
  {
    v = it->second;
    if(vID == vID_name)
    {
      output_file << std::setw(width) << ID2Name[v] << " ";
    }
    else if(vID == vID_num)
    {
      output_file << std::setw(width) << v << " ";
    }
    else if(vID == vID_none) { }
    else
    {
      std::cerr << "ERROR: Unknown vertex identifier type." << std::endl;
      std::terminate();
    }
    for(int i(0); i<nb_props; ++i)
    {
      if(is_vertex_integer_property(props_id[i], false))
      {
        output_file << std::setw(width) << std::setprecision(0) << std::fixed << v_prop[props_id[i]][v] << " ";
      }
      else
      {
        output_file << std::setw(width) << std::setprecision(6) << std::scientific << v_prop[props_id[i]][v] << " ";
      }
    }
    output_file << std::endl;
  }

  // Closes the stream.
  output_file.close();
}










// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// 4. DEGREE
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=

// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
void pgl::directed_graph_t::compute_degrees()
{
  // ===============================================================================================
  // Initializes relevant objects of the class.
  int nb_vertices = g_prop["nb_vertices"];
  v_prop["in-degree"].clear();
  v_prop["out-degree"].clear();
  std::vector<double>& Vertex2InDegree = v_prop["in-degree"];
  std::vector<double>& Vertex2OutDegree = v_prop["out-degree"];
  Vertex2InDegree.resize(nb_vertices, 0);
  Vertex2OutDegree.resize(nb_vertices, 0);
  // ===============================================================================================

  // Extracts the degrees from the adjacency list (if already generated).
  if(adjacency_list.size() == nb_vertices)
  {
    // Loops over all vertices.
    for(int v(0); v<nb_vertices; ++v)
    {
      Vertex2InDegree[v] = adjacency_list[v][InDegreeIdx].size();
      Vertex2OutDegree[v] = adjacency_list[v][OutDegreeIdx].size();
    }
  }
  // Otherwise extracts the degrees from the edgelist.
  else
  {
    // Loops over all edges.
    int v1, v2;
    std::set< std::pair<int, int> >::iterator it = edgelist.begin();
    std::set< std::pair<int, int> >::iterator end = edgelist.end();
    for(; it!=end; ++it)
    {
      // Identifies the vertices.
      v1 = it->first;
      v2 = it->second;
      // Adds the contribution to the degree of vertices.
      Vertex2OutDegree[v1] += 1;
      Vertex2InDegree[v2] += 1;
    }
  }

  // Counts the number of vertices with total degree of 0 or 1.
  int nb_vertices_deg_0 = 0;
  int nb_vertices_deg_1 = 0;
  for(int v(0); v<nb_vertices; ++v)
  {
    if(Vertex2OutDegree[v] == 0 && Vertex2InDegree[v] == 0)
    {
      nb_vertices_deg_0 += 1;
    }
    else if(Vertex2OutDegree[v] == 1 && Vertex2InDegree[v] == 0)
    {
      nb_vertices_deg_1 += 1;
    }
    else if(Vertex2OutDegree[v] == 0 && Vertex2InDegree[v] == 1)
    {
      nb_vertices_deg_1 += 1;
    }
  }

  // ===============================================================================================
  // Updates the properties of the graph.
  g_prop["nb_vertices_undir_deg_0"] = nb_vertices_deg_0;
  g_prop["nb_vertices_undir_deg_1"] = nb_vertices_deg_1;
  // ===============================================================================================
}


// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
void pgl::directed_graph_t::compute_density()
{
  g_prop["density"] = g_prop["nb_edges"] / (g_prop["nb_vertices"] * (g_prop["nb_vertices"] - 1));
}


// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
void pgl::directed_graph_t::compute_reciprocity()
{
  // ===============================================================================================
  // Initializes relevant objects of the class.
  int nb_vertices = g_prop["nb_vertices"];
  int nb_edges = g_prop["nb_edges"];

  std::vector<double>& Vertex2InDegree = v_prop["in-degree"];
  std::vector<double>& Vertex2OutDegree = v_prop["out-degree"];

  v_prop["reciprocal_degree"].clear();
  std::vector<double>& Vertex2ReciprocalDegree = v_prop["reciprocal_degree"];
  Vertex2ReciprocalDegree.resize(nb_vertices, 0);

  v_prop["reciprocity_jaccard"].clear();
  std::vector<double>& Vertex2ReciprocityJaccard = v_prop["reciprocity_jaccard"];
  Vertex2ReciprocityJaccard.resize(nb_vertices, 0);

  v_prop["reciprocity_ratio"].clear();
  std::vector<double>& Vertex2ReciprocityRatio = v_prop["reciprocity_ratio"];
  Vertex2ReciprocityRatio.resize(nb_vertices, 0);
  // ===============================================================================================

  // Ensures that the in-/out-degrees have been computed.
  if(Vertex2InDegree.size() != nb_vertices)
  {
    compute_degrees();
  }

  // Runs over the edgelist and counts (twice!) the number of reciprocal edges.
  double nb_reciprocal_edges = 0;
  edgelist_iterator it = edgelist_begin();
  edgelist_iterator end = edgelist_end();
  for(; it!=end; ++it)
  {
    if(it->first < it->second)
    {
      if(is_edge(it->second, it->first))
      {
        nb_reciprocal_edges += 2;
        Vertex2ReciprocalDegree[it->first] += 1;
        Vertex2ReciprocalDegree[it->second] += 1;
        Vertex2ReciprocityJaccard[it->first] += 1;
        Vertex2ReciprocityJaccard[it->second] += 1;
        Vertex2ReciprocityRatio[it->first] += 2;
        Vertex2ReciprocityRatio[it->second] += 2;
      }
    }
  }

  // Completes the calculation of the overlap between in- and out-neighborhoods.
  for(int v(0); v<nb_vertices; ++v)
  {
    Vertex2ReciprocityJaccard[v] /= Vertex2InDegree[v] + Vertex2OutDegree[v] - Vertex2ReciprocityJaccard[v];
    Vertex2ReciprocityRatio[v] /= Vertex2InDegree[v] + Vertex2OutDegree[v];
  }

  // ===============================================================================================
  // Updates the properties of the graph.
  compute_density();
  g_prop["nb_reciprocal_edges"] = nb_reciprocal_edges;
  g_prop["reciprocity_ratio"] = nb_reciprocal_edges / g_prop["nb_edges"];
  g_prop["reciprocity_stat"] = (g_prop["reciprocity_ratio"] - g_prop["density"]) / (1 - g_prop["density"]);
  // ===============================================================================================
}










// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// 5. TRIANGLES
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=

// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
void pgl::directed_graph_t::survey_triangles(bool build_triangle_list, bool compute_undirected_local_clustering)
{
  // ===============================================================================================
  // Initializes relevant objects of the class.
  int nb_vertices = g_prop["nb_vertices"];
  triangles.clear();

  v_prop["undir_local_clust"].clear();
  std::vector<double>& Vertex2UndirLocalClust = v_prop["undir_local_clust"];
  Vertex2UndirLocalClust.resize(nb_vertices, 0);
  // ===============================================================================================

  // Variables.
  int v2, v3, d1, d2;
  double total_nb_triads = 0;
  // Vector objects.
  std::vector<int> intersection;
  // Set objects.
  std::set<int> neighbours_v1, neighbours_v2;
  // Iterator objects.
  std::vector<int>::iterator it;
  std::set<int>::iterator it1, end1, it2, end2;
  // Newly found triangle.
  std::vector<int> triangle(3);

  // Ensures that the adjacency list has been built.
  if(adjacency_list.size() != nb_vertices)
  {
    build_adjacency_list();
  }

  // Finds all the triangles.
  for(int v1(0); v1<nb_vertices; ++v1)
  {
    // Total degree of vertex v1.
    d1 = adjacency_list[v1][InDegreeIdx].size() + adjacency_list[v1][OutDegreeIdx].size();
    // Performs the calculation only if d1>1.
    if(d1 > 1)
    {
      // Builds an ordered list of the neighbourhood of v1
      neighbours_v1.clear();
      neighbours_v1.insert(adjacency_list[v1][InDegreeIdx].begin(), adjacency_list[v1][InDegreeIdx].end());
      neighbours_v1.insert(adjacency_list[v1][OutDegreeIdx].begin(), adjacency_list[v1][OutDegreeIdx].end());
      // Loops over the neighbours of vertex v1.
      it1 = neighbours_v1.begin();
      end1 = neighbours_v1.end();
      for(; it1!=end1; ++it1)
      {
        // Identity and degree of vertex 2.
        v2 = *it1;
        if(v1 < v2)
        {
          // Total degree of vertex v2.
          d2 = adjacency_list[v2][InDegreeIdx].size() + adjacency_list[v2][OutDegreeIdx].size();
          // Performs the calculation only if d2>1 and if v2>v1 (ensures that each triangle is counted once).
          if(d2 > 1)
          {
            // Builds an ordered list of the neighbourhood of v2
            neighbours_v2.clear();
            d2 = adjacency_list[v2][InDegreeIdx].size();
            for(int n2(0); n2<d2; ++n2)
            {
              // Identifies the neighbors.
              v3 = adjacency_list[v2][InDegreeIdx][n2];
              if(v2 < v3) // Ensures that triangles will be counted only once.
              {
                neighbours_v2.insert(v3);
              }
            }
            d2 = adjacency_list[v2][OutDegreeIdx].size();
            for(int n2(0); n2<d2; ++n2)
            {
              // Identifies the neighbors.
              v3 = adjacency_list[v2][OutDegreeIdx][n2];
              if(v2 < v3) // Ensures that triangles will be counted only once.
              {
                neighbours_v2.insert(v3);
              }
            }
            // Identifies the triangles.
            d2 = neighbours_v2.size();
            intersection.clear();
            intersection.resize(std::min(d1, d2));
            it = std::set_intersection(neighbours_v1.begin(), neighbours_v1.end(), neighbours_v2.begin(), neighbours_v2.end(), intersection.begin());
            intersection.resize(it-intersection.begin());
            // Loops over the common neighbours of vertices v1 and v2.
            for(int n(0), nn(intersection.size()); n<nn; ++n)
            {
              triangle[0] = v1;
              triangle[1] = v2;
              triangle[2] = intersection[n];
              if(build_triangle_list)
              {
                // Adds the triangle to the list.
                triangles.push_back(triangle);
              }
              if(compute_undirected_local_clustering)
              {
                // Compiles the undirected local clustering coefficients (1 of 2).
                Vertex2UndirLocalClust[ triangle[0] ] += 1;
                Vertex2UndirLocalClust[ triangle[1] ] += 1;
                Vertex2UndirLocalClust[ triangle[2] ] += 1;
              }
            }
          }
        }
      }
    }
  }

  if(compute_undirected_local_clustering)
  {
    // Compiles the undirected local clustering coefficients (2 of 2).
    for(int v1(0); v1<nb_vertices; ++v1)
    {
      intersection.clear();
      d1 = adjacency_list[v1][InDegreeIdx].size() + adjacency_list[v1][OutDegreeIdx].size();
      intersection.resize(d1);
      it = std::set_union(adjacency_list[v1][InDegreeIdx].begin(), adjacency_list[v1][InDegreeIdx].end(), adjacency_list[v1][OutDegreeIdx].begin(), adjacency_list[v1][OutDegreeIdx].end(), intersection.begin());
      intersection.resize(it-intersection.begin());
      d1 = intersection.size();
      if(d1 > 1)
      {
        Vertex2UndirLocalClust[v1] /= d1 * (d1 - 1) / 2;
      }
      total_nb_triads += d1 * (d1 - 1) / 2;
    }
  }


  // ===============================================================================================
  // Updates the properties of the graph.
  g_prop["nb_triangles"] = triangles.size();
  g_prop["total_nb_triads"] = total_nb_triads;
  g_prop["undir_global_clust"] = 3 * g_prop["nb_triangles"] / g_prop["total_nb_triads"];
  // ===============================================================================================
}


// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
void pgl::directed_graph_t::compile_triangle_spectrum()
{
  // ===============================================================================================
  // Initializes relevant objects of the class.
  // int nb_vertices = g_prop["nb_vertices"];
  triangle_spect.clear();
  triangle_spect["3cycle"] = 0;
  triangle_spect["3nocycle"] = 0;
  triangle_spect["4cycle"] = 0;
  triangle_spect["4outward"] = 0;
  triangle_spect["4inward"] = 0;
  triangle_spect["5cycle"] = 0;
  triangle_spect["6cycle"] = 0;
  // ===============================================================================================

  // Ensures that the triangles have been surveyed.
  if(triangles.size() != 0)
  {
    survey_triangles();
  }

  // Builds a std::map to translate the 27 possible triangle configurations to
  //   the 7 unique possible triangle configurations.
  std::map<std::string, std::string> link2type;
  link2type["111"] = "3cycle";
  link2type["222"] = "3cycle";
  link2type["211"] = "3nocycle";
  link2type["121"] = "3nocycle";
  link2type["112"] = "3nocycle";
  link2type["122"] = "3nocycle";
  link2type["212"] = "3nocycle";
  link2type["221"] = "3nocycle";
  link2type["311"] = "4cycle";
  link2type["131"] = "4cycle";
  link2type["113"] = "4cycle";
  link2type["322"] = "4cycle";
  link2type["232"] = "4cycle";
  link2type["223"] = "4cycle";
  link2type["123"] = "4outward";
  link2type["312"] = "4outward";
  link2type["231"] = "4outward";
  link2type["321"] = "4inward";
  link2type["132"] = "4inward";
  link2type["213"] = "4inward";
  link2type["133"] = "5cycle";
  link2type["313"] = "5cycle";
  link2type["331"] = "5cycle";
  link2type["233"] = "5cycle";
  link2type["323"] = "5cycle";
  link2type["332"] = "5cycle";
  link2type["333"] = "6cycle";

  // Runs over the triangles and counts the frequency of each triangle
  //   configurations.
  std::string edge_config;
  bool edge_01, edge_10, edge_12, edge_21, edge_20, edge_02;
  for(int t(0), tt(g_prop["nb_triangles"]), v0, v1, v2; t<tt; ++t)
  {
    // Identifies the vertices.
    v0 = triangles[t][0];
    v1 = triangles[t][1];
    v2 = triangles[t][2];
    // Identifies the configuration.
    edge_config = "";
    edge_01 = is_edge(v0, v1);
    edge_10 = is_edge(v1, v0);
    edge_12 = is_edge(v1, v2);
    edge_21 = is_edge(v2, v1);
    edge_20 = is_edge(v2, v0);
    edge_02 = is_edge(v0, v2);
    if(edge_01 && !edge_10)
      edge_config += "1";
    if(!edge_01 && edge_10)
      edge_config += "2";
    if(edge_01 && edge_10)
      edge_config += "3";
    if(edge_12 && !edge_21)
      edge_config += "1";
    if(!edge_12 && edge_21)
      edge_config += "2";
    if(edge_12 && edge_21)
      edge_config += "3";
    if(edge_20 && !edge_02)
      edge_config += "1";
    if(!edge_20 && edge_02)
      edge_config += "2";
    if(edge_20 && edge_02)
      edge_config += "3";
    triangle_spect[link2type[edge_config]] += 1;
  }
}










// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// 6. ACCESSORS FOR GRAPH PROPERTIES
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=

// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
int pgl::directed_graph_t::get_nb_edges()
{
  return g_prop["nb_edges"];
}


// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
int pgl::directed_graph_t::get_nb_vertices()
{
  return g_prop["nb_vertices"];
}


// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
int pgl::directed_graph_t::get_nb_reciprocal_edges()
{
  if(g_prop["nb_reciprocal_edges"] == -1)
  {
    compute_reciprocity();
  }
  return g_prop["nb_reciprocal_edges"];
}


// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
double pgl::directed_graph_t::get_reciprocity()
{
  if(g_prop["reciprocity"] == -1)
  {
    compute_reciprocity();
  }
  return g_prop["reciprocity"];
}





// // =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// // 7. ACCESSORS FOR VERTEX PROPERTIES
// // =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
//
//
// // =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// // =~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// std::map<std::string, double> pgl::directed_graph_t::get_local_reciprocity()
// {
//   // ===============================================================================================
//   // Initializes relevant objects of the class.
//   int nb_vertices = g_prop["nb_vertices"];
//   std::vector<double>& Vertex2Prop = v_prop["reciprocity_ratio"];
//   // ===============================================================================================
//   // Ensures that the ID2Name has been built. Otherwise, build it.
//   if(ID2Name.size() != nb_vertices)
//   {
//     build_ID2Name();
//   }
//   // Extracts the property.
//   if(g_prop["reciprocity"] == -1)
//   {
//     compute_reciprocity();
//   }
//   // Builds the map from names to property.
//   std::map<std::string, double> dict;
//   for(int v(0); v<nb_vertices; ++v)
//   {
//     dict[ID2Name[v]] = Vertex2Prop[v];
//   }
//   return dict;
// }





#endif
