#  This code compares the text files generated to validate the directed_graph_t
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

# Compiles the PGL file.
echo -en "\nCompiling cpp/validation_directed_graph_t.cpp..."
g++ -O3 cpp/validation_directed_graph_t.cpp -o validation_directed_graph_t
echo -e "done.\n"

edgelist_filenames=("../assets/edgelists/example01.edge" \
                    "../assets/edgelists/example03.edge"
                    "../assets/edgelists/example04.edge"
                    "../assets/edgelists/example05.edge"
                    "../assets/edgelists/example06.edge"
                    "../assets/edgelists/example07.edge"
                    "../assets/edgelists/example08.edge"
                    "../assets/edgelists/example09.edge")

echo "Validating directed_graph_t using:"
# for file in $edgelist_filenames; do
for file in "${edgelist_filenames[@]}"; do
    echo -n "   using file: "$file
    echo -n "   Extacting info using NetworkX..."
    python3 python/validation_directed_graph_t.py $file
    echo -n "done.    Extracting info using PGL..."
    ./validation_directed_graph_t $file
    echo -n "done.    Comparing outputs..."
    DIFF1=$(diff validation_g_prop_pgl.dat validation_g_prop_networkx.dat)
    DIFF2=$(diff validation_v_prop_pgl.dat validation_v_prop_networkx.dat)
    DIFF3=$(diff validation_triangles_pgl.dat validation_triangles_networkx.dat)
    DIFF4=$(diff validation_triangle_spect_pgl.dat validation_triangle_spect_networkx.dat)
    if [[ $DIFF1 = "" && $DIFF2 = "" && $DIFF3 = "" && $DIFF4 = "" ]]; then
    # if [[ diff validation_g_prop_pgl.dat validation_g_prop_networkx.dat && diff validation_v_prop_pgl.dat validation_v_prop_networkx.dat && diff validation_triangles_pgl.dat validation_triangles_networkx.dat && diff validation_triangle_spect_pgl.dat validation_triangle_spect_networkx.dat ]]; then
      echo -e "done.   "'\033[92m'"Test PASSED"'\033[0m'
    else
      echo -e "done.   "'\033[91m'"Test FAILED"'\033[0m'
    fi
    rm validation_*_pgl.dat
    rm validation_*_networkx.dat
done

echo -n "Cleaning directory..."
rm validation_directed_graph_t
echo -e "done.\n"
