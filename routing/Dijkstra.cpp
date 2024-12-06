#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;

struct Edge {
  string dest;
  int cost;
};

struct Router {
  string name;
  vector<Edge> neighbors;
  unordered_map<pair<string, string>, int, pair_hash> lsdb;
  unordered_map<string, pair<string, int>> routing_table;
};

// Custom hash function for pairs of strings
struct pair_hash {
  template <class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2>& p) const {
    auto h1 = std::hash<T1>{}(p.first);
    auto h2 = std::hash<T2>{}(p.second);
    return h1 ^ h2;
  }
};

unordered_map<string, Router> topology;

// Function to read input and build the topology
void readTopology() {
  string line;
  while (getline(cin, line)) {
    if (line == "LINKSTATE") break;
    Router router;
    router.name = line;
    topology[router.name] = router;
  }

  while (getline(cin, line)) {
    if (line == "UPDATE") break;
    string src, dest;
    int cost;
    stringstream ss(line);
    ss >> src >> dest >> cost;
    topology[src].neighbors.push_back(Edge{dest, cost});
    topology[dest].neighbors.push_back(
        Edge{src, cost});  // Assuming undirected graph

    for (auto& router : topology) {
      router.second.lsdb[{src, dest}] = cost;
      router.second.lsdb[{dest, src}] = cost;
    }
  }
}

// Function to perform Dijkstra's algorithm
void dijkstra(const string& start) {
  unordered_map<string, int> dist;
  unordered_map<string, string> prev;
  for (const auto& router : topology) {
    dist[router.first] = numeric_limits<int>::max();
    prev[router.first] = "";
  }
  dist[start] = 0;

  auto cmp = [&](const string& a, const string& b) {
    return dist[a] > dist[b];
  };
  priority_queue<string, vector<string>, decltype(cmp)> pq(cmp);
  pq.push(start);

  while (!pq.empty()) {
    string u = pq.top();
    pq.pop();
    for (const auto& edge : topology[u].neighbors) {
      if (dist[u] + edge.cost < dist[edge.dest]) {
        dist[edge.dest] = dist[u] + edge.cost;
        prev[edge.dest] = u;
        pq.push(edge.dest);
      }
    }
  }

  for (auto& router : topology[start].routing_table) {
    router.second = {"", numeric_limits<int>::max()};
  }

  for (const auto& router : dist) {
    if (router.second == numeric_limits<int>::max() || router.first == start)
      continue;
    string next_hop = router.first;
    while (prev[next_hop] != start && !prev[next_hop].empty()) {
      next_hop = prev[next_hop];
    }
    topology[start].routing_table[router.first] = {next_hop, router.second};
  }
}

// Function to print the Neighbour Table, LSDB, and Routing Table
void printTables(const string& routerName) {
  // Print Neighbour Table
  cout << routerName << " Neighbour Table:\n";
  for (const auto& neighbor : topology[routerName].neighbors) {
    cout << neighbor.dest << "|" << neighbor.cost << "\n";
  }
  cout << "\n";

  // Print Link-state Database (LSDB)
  cout << routerName << " LSDB:\n";
  vector<string> lsdb;
  for (const auto& entry : topology[routerName].lsdb) {
    string link = entry.first.first < entry.first.second
                      ? entry.first.first + "|" + entry.first.second
                      : entry.first.second + "|" + entry.first.first;
    lsdb.push_back(link + "|" + to_string(entry.second));
  }
  sort(lsdb.begin(), lsdb.end());
  lsdb.erase(unique(lsdb.begin(), lsdb.end()),
             lsdb.end());  // Remove duplicate entries
  for (const auto& entry : lsdb) {
    cout << entry << "\n";
  }
  cout << "\n";

  // Print Routing Table
  cout << routerName << " Routing Table:\n";
  for (const auto& entry : topology[routerName].routing_table) {
    cout << entry.first << "|" << entry.second.first << "|"
         << entry.second.second << "\n";
  }
  cout << "\n";
}

int main() {
  readTopology();

  string line;
  while (getline(cin, line)) {
    if (line == "END") break;
    string src, dest;
    int cost;
    stringstream ss(line);
    ss >> src >> dest >> cost;
    vector<string> selected_routers;
    string router_name;
    while (ss >> router_name) {
      selected_routers.push_back(router_name);
    }

    // Update topology accordingly
    auto& neighbors_src = topology[src].neighbors;
    neighbors_src.erase(
        remove_if(neighbors_src.begin(), neighbors_src.end(),
                  [&](const Edge& edge) { return edge.dest == dest; }),
        neighbors_src.end());
    auto& neighbors_dest = topology[dest].neighbors;
    neighbors_dest.erase(
        remove_if(neighbors_dest.begin(), neighbors_dest.end(),
                  [&](const Edge& edge) { return edge.dest == src; }),
        neighbors_dest.end());

    if (cost != -1) {
      neighbors_src.push_back(Edge{dest, cost});
      neighbors_dest.push_back(Edge{src, cost});
    }

    for (auto& router : topology) {
      if (cost == -1) {
        router.second.lsdb.erase({src, dest});
        router.second.lsdb.erase({dest, src});
      } else {
        router.second.lsdb[{src, dest}] = cost;
        router.second.lsdb[{dest, src}] = cost;
      }
    }

    for (const auto& router_name : selected_routers) {
      if (topology.find(router_name) != topology.end()) {
        dijkstra(router_name);
        printTables(router_name);
      }
    }
  }

  return 0;
}
