/*
 * g++ -o ExportDataConnection ExportDataConnection.cpp
 */

#include <string>
#include <iomanip>
#include <boost/unordered_map.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>

template <class Name, class Inport, class Outport>
class node_writer {

public:
  
  node_writer(Name name, Inport inport, Outport outport) :
    name_(name), inport_(inport), outport_(outport) {}

  template <class Vertex>
  void operator() (std::ostream& out, const Vertex& v) const {
    
    out << "[shape=record, label=\" { ";
    for (int i = 0; i < inport_[v].size(); i++) {
      out << "<in" << i + 1 << "> " << inport_[v][i];
      if (i != inport_[v].size() - 1)
        out << " | ";
    }
    out << "} | " << name_[v] << " | { ";
    for (int i = 0; i < outport_[v].size(); i++) {
      out << "<out" << i + 1 << "> " << outport_[v][i];
      if (i != outport_[v].size() - 1)
        out << " | ";
    }
    out << " } \"]";
  }
  
private:

  Name name_;
  Inport inport_;
  Outport outport_;
};

template <class Name, class Inport, class Outport>
inline node_writer<Name, Inport, Outport> make_node_writer(Name name, Inport inport, Outport outport) {
  return node_writer<Name, Inport, Outport>(name, inport, outport);
}

template <class From, class To>
class edge_writer {

public:

  edge_writer(From from, To to) :
    from_(from), to_(to) {}

  template <class Edge>
  void operator() (std::ostream& out, const Edge& e) const {
    out << "[tailport=" << from_[e] << ", headport=" << to_[e] << "]";
  }

private:

  From from_;
  To to_;
};

template <class From, class To>
inline edge_writer<From, To> make_edge_writer(From from, To to) {
  return edge_writer<From, To>(from, to);
}

struct Plugin {
  std::string name;
  std::vector<std::string> inport;
  std::vector<std::string> outport;
};

struct Connection {
  std::string from;
  std::string to;
};

struct HMC {
  std::string name;
};

typedef boost::adjacency_list<boost::listS, boost::vecS, boost::bidirectionalS, Plugin, Connection, HMC> Map;

int main(void) {

  Map map;

  map[boost::graph_bundle].name = "HMC";

  std::string filename = "connection";
  
  std::ofstream file((filename + std::string(".dot")).c_str());
  
  Map::vertex_descriptor v1 = add_vertex(map);
  map[v1].name = "foot-motion-generator";
  map[v1].inport.push_back("foot-in1");
  map[v1].inport.push_back("foot-in2");
  map[v1].outport.push_back("foot-out1");
  map[v1].outport.push_back("foot-out2");
  map[v1].outport.push_back("foot-out3");

  Map::vertex_descriptor v2 = add_vertex(map);
  map[v2].name = "force-distribution";
  map[v2].inport.push_back("force-in1");
  map[v2].inport.push_back("force-in2");
  map[v2].outport.push_back("force-out1");
  map[v2].outport.push_back("force-out2");

  bool inserted = false;
  
  Map::edge_descriptor e;
  boost::tie(e, inserted) = add_edge(v1, v2, map);
  map[e].from = "out1";
  map[e].to = "in2";
  
  boost::write_graphviz(file, map, make_node_writer(boost::get(&Plugin::name, map), boost::get(&Plugin::inport, map), boost::get(&Plugin::outport, map)), make_edge_writer(boost::get(&Connection::from, map), boost::get(&Connection::to, map)));
  
  return 0;
}
