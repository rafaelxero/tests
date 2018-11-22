/*
 * g++ -o ExportDataConnection ExportDataConnection.cpp
 */

#include <string>
#include <iomanip>
#include <boost/unordered_map.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>

template <class Name>
class node_writer {

public:
  
  node_writer(Name name) : name_(name) {}

  template <class Vertex>
  void operator() (std::ostream& out, const Vertex& v) const {
    out << "[shape=record, label=\"" << name_[v] << "\"]";
  }
  
private:

  Name name_;
};

template <class Name>
inline node_writer<Name> make_node_writer(Name name) {
  return node_writer<Name>(name);
}

struct Plugin {
  std::string name;
  std::vector<std::string> inport;
  std::vector<std::string> outport;
};

struct Connection {
  double weight;
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
  
  Map::vertex_descriptor v = add_vertex(map);
  map[v].name = "force-distribution";

  boost::write_graphviz(file, map, make_node_writer(boost::get(&Plugin::name, map)));
  
  return 0;
}
