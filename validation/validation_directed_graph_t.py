#  This code generates the text files used to validate the directed_graph_t
#    class of the PGL to analyze the structure of directed, unweighted and
#    simple graph.
#
#  Author:  Antoine Allard
#  WWW:     antoineallard.info
#  Date:    August 2019
#
#
#  Copyright (C) 2019 Antoine Allard
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <https://www.gnu.org/licenses/>.


import networkx as nx
import numpy as np
import sys


# Gets the name of the file containing the edgelist.
edgelist_filename = sys.argv[1]

# Loads the edgelist using networkx and makes it a simple directed graph without self-loops.
g_dir = nx.read_edgelist(edgelist_filename, create_using=nx.DiGraph())
g_dir.remove_edges_from(g_dir.selfloop_edges())
g_dir.remove_nodes_from([v for v in nx.isolates(g_dir)])

# Creates an undirected version of the graph.
g_und = g_dir.to_undirected()

# Graph properties.
with open("validation_g_prop_networkx.dat", "w") as f:
    f.write("{:>50}".format("Number of vertices:") + " ")
    f.write("{:>50}".format(g_dir.number_of_nodes()) + " \n")
    f.write("{:>50}".format("Number of edges:") + " ")
    f.write("{:>50}".format(g_dir.number_of_edges()) + " \n")
    f.write("{:>50}".format("Number of edges:") + " ")
    f.write("{:>50}".format(g_dir.number_of_edges()) + " \n")
    f.write("{:>50}".format("Density:") + " ")
    density = g_dir.number_of_edges() / (g_dir.number_of_nodes() * (g_dir.number_of_nodes() - 1))
    f.write("{:50.3f}".format(density) + " \n")
    f.write("{:>50}".format("Average in-degree:") + " ")
    f.write("{:50.3f}".format(sum([d for (n, d) in g_dir.in_degree()]) / float(g_dir.number_of_nodes())) + " \n")
    f.write("{:>50}".format("Average out-degree:") + " ")
    f.write("{:50.3f}".format(sum([d for (n, d) in g_dir.out_degree()]) / float(g_dir.number_of_nodes())) + " \n")
    f.write("{:>50}".format("Number of reciprocal edges:") + " ")
    f.write("{:>50}".format(int(nx.overall_reciprocity(g_dir)*g_dir.number_of_edges())) + " \n")
    f.write("{:>50}".format("Reciprocity (ratio definition):") + " ")
    f.write("{:50.3f}".format(nx.overall_reciprocity(g_dir)) + " \n")
    f.write("{:>50}".format("Reciprocity (statistical definition):") + " ")
    f.write("{:50.3f}".format((nx.overall_reciprocity(g_dir) - density) / (1 - density)) + " \n")
    f.write("{:>50}".format("Number of triangles:") + " ")
    f.write("{:>50}".format(int(np.sum(np.array(list(nx.algorithms.triangles(g_und).values()))) / 3)) + " \n")
    f.write("{:>50}".format("Average undirected local clustering coefficient:") + " ")
    f.write("{:50.3f}".format(nx.average_clustering(g_und)) + " \n")
    f.write("{:>50}".format("Undirected global clustering coefficient:") + " ")
    f.write("{:50.3f}".format(nx.transitivity(g_und)) + " \n")


# Vertex properties.
with open("validation_v_prop_networkx.dat", "w") as f:
    # Writes the header.
    f.write("#")
    f.write("{:>14}".format("Vertex") + " ")
    f.write("{:>15}".format("InDeg") + " ")
    f.write("{:>15}".format("OutDeg") + " ")
    f.write("{:>15}".format("DummyInt") + " ")
    f.write("{:>15}".format("DummyDbl") + " ")
    f.write("{:>15}".format("UnLocalClust") + " ")
    f.write("\n")
    # Writes the computed values.
    UnLocalClust = nx.clustering(g_und)
    for v in sorted(g_dir.nodes(), key=lambda x: (len(x), x)):
        f.write("{:>15}".format(v) + " ")
        f.write("{:>15}".format(g_dir.in_degree(v)) + " ")
        f.write("{:>15}".format(g_dir.out_degree(v)) + " ")
        f.write("{:15d}".format(np.random.randint(1000)) + " ")
        # f.write("{:>15}".format("{:15.3f}".format(100 * np.random.random()).rstrip('0').rstrip('.')) + " ")
        f.write("{:>15}".format("{:15.6e}".format(100 * np.random.random())) + " ")
        # f.write("{:>15}".format(UnLocalClust[v]) + " ")
        # f.write("{:>15}".format("{:15.9f}".format(UnLocalClust[v]).rstrip('0').rstrip('.')) + " ")
        f.write("{:>15}".format("{:15.6e}".format(UnLocalClust[v])) + " ")
        f.write("\n")


# Triangles.
triangles = sorted([sorted(x) for x in nx.enumerate_all_cliques(g_und) if len(x) == 3])
with open("validation_triangles_networkx.dat", "w") as f:
    for t in triangles:
        for v in t:
            f.write("{:>25}".format(v) + " ")
        f.write("\n")


# Triangle spectrum.
triangle_spect = {}
triangle_spect["3cycle"] = 0
triangle_spect["3nocycle"] = 0
triangle_spect["4cycle"] = 0
triangle_spect["4outward"] = 0
triangle_spect["4inward"] = 0
triangle_spect["5cycle"] = 0
triangle_spect["6cycle"] = 0
link2type = {}
link2type["111"] = "3cycle"
link2type["222"] = "3cycle"
link2type["211"] = "3nocycle"
link2type["121"] = "3nocycle"
link2type["112"] = "3nocycle"
link2type["122"] = "3nocycle"
link2type["212"] = "3nocycle"
link2type["221"] = "3nocycle"
link2type["311"] = "4cycle"
link2type["131"] = "4cycle"
link2type["113"] = "4cycle"
link2type["322"] = "4cycle"
link2type["232"] = "4cycle"
link2type["223"] = "4cycle"
link2type["123"] = "4outward"
link2type["312"] = "4outward"
link2type["231"] = "4outward"
link2type["321"] = "4inward"
link2type["132"] = "4inward"
link2type["213"] = "4inward"
link2type["133"] = "5cycle"
link2type["313"] = "5cycle"
link2type["331"] = "5cycle"
link2type["233"] = "5cycle"
link2type["323"] = "5cycle"
link2type["332"] = "5cycle"
link2type["333"] = "6cycle"
for t in triangles:
    v0 = t[0]
    v1 = t[1]
    v2 = t[2]
    edge_01 = v1 in g_dir.neighbors(v0)
    edge_10 = v0 in g_dir.neighbors(v1)
    edge_12 = v2 in g_dir.neighbors(v1)
    edge_21 = v1 in g_dir.neighbors(v2)
    edge_20 = v0 in g_dir.neighbors(v2)
    edge_02 = v2 in g_dir.neighbors(v0)
    edge_config = ""
    if edge_01 and not edge_10:
        edge_config += "1"
    if not edge_01 and edge_10:
        edge_config += "2"
    if edge_01 and edge_10:
        edge_config += "3"
    if edge_12 and not edge_21:
        edge_config += "1"
    if not edge_12 and edge_21:
        edge_config += "2"
    if edge_12 and edge_21:
        edge_config += "3"
    if edge_20 and not edge_02:
        edge_config += "1"
    if not edge_20 and edge_02:
        edge_config += "2"
    if edge_20 and edge_02:
        edge_config += "3"
    triangle_spect[link2type[edge_config]] += 1
with open("validation_triangle_spect_networkx.dat", "w") as f:
    for t in ["3cycle", "3nocycle", "4cycle", "4outward", "4inward", "5cycle", "6cycle"]:
        f.write("{:>12}".format(t) + " ")
        f.write("{:>12}".format(int(triangle_spect[t])) + " ")
        f.write("\n")
