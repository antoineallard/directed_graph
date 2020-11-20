## Directed graph

The `directed_graph_t` class offers many methods to analyze the structure of **directed, unweighted and simple graph without self-loops**.


The class can be imported directly and is available under the namepace `pgl`.
```c++
// Importing the class
#include "src/directed_graph_t.hpp"
```

The currently available functions are
* [Importing a graph from an edgelist file](#importing-a-graph-from-an-edgelist-file)
* [Generating one instance of a bond percolated graph](#generating-one-instance-of-a-bond-percolated-edgelist)
* [Properties of the percolated graph](#properties-of-the-percolated-edgelist)

Note that further examples on how to use `edgelist_perco_t` are also provided in a [notebook](https://github.com/antoineallard/percolation_on_edgelist/blob/main/validation/plot_validation_figures.ipynb) (see also related scripts in [`validation/`](https://github.com/antoineallard/percolation_on_edgelist/tree/main/validation)) used to validate the class.


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

# where the vertices's name will be imported as std::string and can therefore
#  e virtually anything as long as they do not include white spaces (i.e.,
#  there is not need for the vertices to be identified by contiguous
#  integers).
```
**IMPORTANT**: this class only considers simple directed graphs without self-loops. Any multiple edges or self-loops will be ignored.

```c++
// The graph is loaded at the initialization of the class
pgl::directed_graph_t g(""<path-to-edgelist-file>"");
```
