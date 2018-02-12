from pygraphml import Graph
from pygraphml import GraphMLParser

# data sets are from http://www.graphdrawing.org/data.html
parser = GraphMLParser()
g = parser.parse('./random-dag/g.100.1.graphml')


with open('example.lgf', 'w') as fp:
    fp.write('@nodes\nlabel\n')
    for n in g.nodes():
        fp.write(n['label'][1:] + '\n')
    fp.write('@arcs\n\t\tlabel\n')
    counter = 0
    for n in g.nodes():
        for nn in g.children(n):
            fp.write(n['label'][1:] + '\t' + nn['label'][1:] + '\t' + str(counter) + '\n')
            counter += 1
    fp.write('@end\n')
