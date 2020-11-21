## Directed graph

The `directed_graph_t` class offers many methods to analyze the structure of **simple unweighted directed graph without self-loops**.


The class can be imported directly and is available under the namepace `pgl`.
```c++
// Importing the class
#include "src/directed_graph_t.hpp"
```

The currently available functionalities are
* [Importing a graph from an edgelist file](#importing-a-graph-from-an-edgelist-file)
* [Loading vertices properties](#loading-vertices-properties)
* [Number of vertices and edges](#number-of-vertices-and-edges)
* [In-degrees and out-degrees](#in-degrees-and-out-degrees)
* [Reciprocity](#reciprocity)
* [List of triangles](#list-of-triangles)
* [Spectrum of unique triangle configurations](#spectrum-of-unique-triangle-configurations)

Note that further examples on how to use `directed_graph_t` are also provided in a notebook (see also related scripts in `validation/`) used to validate the class.


#### Importing a graph from an edgelist file

A graph can be imported from a file containing its edgelist (one edge per line). The edgelist file consists in a simple text file with the following convention

```python
# lines beginning with "#" are ignored (comments).
# note that vertices' name must be separated by at least one white space.
# there may be white space at the beginning of a line.
[name of source vertex]  [name of target vertex]  [ignores the rest]
[name of source vertex]  [name of target vertex]  [ignores the rest]
[name of source vertex]  [name of target vertex]  [ignores the rest]
# comments can be inserted between edges
[name of source vertex]  [name of target vertex]  [ignores the rest]
[name of source vertex]  [name of target vertex]  [ignores the rest]
...
```
Note that the vertices' name will be imported as `std::string` and can therefore be virtually anything as long as they do not include white spaces (i.e., there is not need for the vertices to be identified by contiguous integers).

**IMPORTANT**: this class only considers simple directed graphs without self-loops. Any multiple edges or self-loops will be ignored.

```c++
// The graph is loaded at the initialization of the class
pgl::directed_graph_t g("<path-to-edgelist-file>");
```


#### Loading vertices properties

Vertex properties can be loaded from a text file in order to be used by the code (one vertex per line). These text file must follow the following convention

```python
# lines beginning with "#" are ignored (comments).
# note that vertices' name must be separated by at least one white space.
# there may be white space at the beginning of a line.
[name_of_vertex_1]  [value_for_property1]  [value_for_property2] [value_for_property3]
[name_of_vertex_2]  [value_for_property1]  [value_for_property2] [value_for_property3]
[name_of_vertex_3]  [value_for_property1]  [value_for_property2] [value_for_property3]
# comments can be inserted between edges
[name_of_vertex_4]  [value_for_property1]  [value_for_property2] [value_for_property3]
[name_of_vertex_5]  [value_for_property1]  [value_for_property2] [value_for_property3]
...
```


```c++
// Loads a vertex property.
g.load_vertices_properties("<path-to-file>",   // name of the property file to read
                           <column_number>,    // column in the file which corresponds to the property to read (column 0 correspsonds to the names)
                           "<internal_name>",  // internal name of the property (used to access the property via v_prop["<internal_name>"])
                           "<output_name>",    // default header name to use when using save_vertices_properties() (uses "<internal_name>" if not provided)
                           <bool>)             // indicate whether a vertex found in the property file that is not already in the graph should be
                                               // added as a vertex with degree 0 (default: true).
```


#### Number of vertices and edges

```c++
// The number of vertices/edges in the graph are accessible via
int nb_vertices = g.g_prop["nb_vertices"];
int nb_edges = g.g_prop["nb_edges"];
// The density of the graph is accessible via
double density = g.g_prop["density"]
```


#### In-degrees and out-degrees

```c++
// Computes the in-/out-degree of vertices.
g.degrees();

// This function activates the two vertex property (v_prop) keywords
//   "in-degree" and "out-degree", which give access to std::vector<double>
//  objects.
std::vector<double>& Vertex2InDegree = g.v_prop["in-degree"];
std::vector<double>& Vertex2OutDegree = g.v_prop["out-degree"];
// It also activates the graph property (g_prop) keywords containing the number
//   of vertices with degree equal to 0 and degree equal to 1.
int nb_vertices_deg_0 = g_prop["nb_vertices_undir_deg_0"]
int nb_vertices_deg_1 = g_prop["nb_vertices_undir_deg_1"]


// The joint in-/out-degree distribution can be written into a text file via
std::string p[] = {"in-degree", "out-degree"};
std::vector<std::string> props(p, p+2);
g.save_vertices_properties("<output_filename>",                // name of the file to write into
                           props,                              // std::vector<string> with the keywords of the vertex properties
                           pgl::directed_graph_t::vID_name,    // indicates whether vertices should be identified or not (adds a column)
                                                               //   - directed_graph_t::vID_name (default): names in the original edgelist
                                                               //   - directed_graph_t::vID_num: contiguous integer ID
                                                               //   - directed_graph_t::vID_none: does not identify the vertices (no additional column)
                           15,                                 // column width (default_column_width = 15)
                           pgl::directed_graph_t::header_true  // indicates whether a header should be added to identify the columns
                                                               //   - header_true (default)
                                                               //   - header_false
                          );  // NOTE: The last three parameters can be omitted or provided in any order.
```


#### Reciprocity

```c++
// Computes the number of reciprocal edges and the reciprocity coefficient
//   (the latter is returned by the function).  An edge is reciprocal is an
//   edge running in the opposite direction exists. There is therefore an even
//   number of reciprocal edges. The reciprocity coefficient is defined as the
//   fraction of edges that are reciprocal.
double reciprocity = g.reciprocity();

// Calling this function activates the g_prop keywords "nb_reciprocal_edges"
//   and "reciprocity".
int nb_reciprocal_edges = g.g_prop["nb_reciprocal_edges"];
reciprocity = g.g_prop["reciprocity"];
```


#### List of triangles

```c++
// Surveys the graph and builds a list of every triangles. In directed graphs,
//   triangles are any group of 3 vertices connected by 3 edges independently
//   of their direction.
g.survey_triangles();

// Doing so activates the graph property keyword "nb_triangles"
int nb_triangles = g.g_prop["nb_triangles"];

// The list of triangles is a std::vector< std::vector<int> > with dimensions
//   nb_triangles x 3. It can be accessed via
std::vector< std::vector<int> >& list_of_triangles = g.triangles;
// Creating this list can be skipped by calling instead
g.survey_triangles(false);

// Calling survey_triangles() also computes various properties related to the
//   clustering of the undirected version of the graph. The graph properties are
int total_nb_triplets = g_prop["total_nb_triplets"];      // Number of triplets (three vertices connected by two edges)
double undir_global_clust = g_prop["undir_global_clust"]  // Global clustering coefficient
// as well as the vertex property (v_prop) keyword
std::vector<double>& Vertex2UndirLocalClust = v_prop["undir_local_clust"];
// Calculating these values can be skipped by setting the second argument out
//   survey_triangles() to false.
g.survey_triangles(<true/false>, false);
```


#### Spectrum of unique triangle configurations

```c++
// The number of each 7 unique triangle configurations is extracted via
g.triangle_spectrum();

// The histogram can be accessed via
std::map<std::string, int>& triangle_spect = g.triangle_spect;

// The 7 unique triangle configurations are
// "3cycle":   A  -> B  -> C  -> A
// "3nocycle": A  -> B  -> C <-  A
// "4cycle":   A <-> B  -> C  -> A
// "4outward": A <-> B  -> C <-  A
// "4inward":  A <-> B <-  C  -> A
// "5cycle":   A <-> B <-> C  -> A
// "6cycle":   A <-> B <-> C <-> A
// The 20 remaining possible triangles are automorphisms of these 7 configurations.
```
