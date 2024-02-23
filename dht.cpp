#include <string>
#include <sstream>
#include <algorithm>
#include <queue>

#include "dht.h"

DHT::DHT() : version(0) {}

DHT::DHT(string dht) {
	vector<string> lines;
	istringstream stream(dht);
	string temp;
	while (getline(stream, temp, '\n')) lines.push_back(temp);

	version = stoi(lines[0]);

	int i;
	for (i = 2; lines[i] != "-"; i++) {
		vector<string> words;
		istringstream stream(lines[i]);
		string temp;
		while (getline(stream, temp, ' ')) words.push_back(temp);

		DHTNode node = DHTNode(stoi(words[0]), stoi(words[1]), words[2]);
		nodes.push_back(node);
	}

	for (i++; i < lines.size(); i++) {
		vector<string> words;
		istringstream stream(lines[i]);
		string temp;
		while (getline(stream, temp, ' ')) words.push_back(temp);

		DHTNode* a = getNodeFromId(stoi(words[0]));
		DHTNode* b = getNodeFromId(stoi(words[0]));

		DHTConnection connection = DHTConnection(a, b);
		connections.push_back(connection);
		
		a->connections.push_back(&connections[connections.size() - 1]);
		b->connections.push_back(&connections[connections.size() - 1]);

	}
}

string DHT::toString() {
	string dht = "";
	
	dht += to_string(version) + "\n-\n";

	for (DHTNode node : nodes) {
		dht += to_string(node.id) + " " + to_string(node.level) + " " + node.ip + "\n";
	}
	dht += "-\n";

	for (DHTConnection connection : connections) {
		dht += to_string(connection.a->id) + " " + to_string(connection.b->id) + "\n";
	}

	return dht;
}

bool DHT::addNode(DHTNode node) {
	for (int i = 0; i < nodes.size(); i++) {
		if (node == nodes[i]) {
			return false;
		}
	}

	nodes.push_back(node);
	return true;
}

bool DHT::addConnection(DHTConnection connection) {
	for (int i = 0; i < connections.size(); i++) {
		if (connection == connections[i]) {
			return false;
		}
	}

	connections.push_back(connection);

	connection.a->connections.push_back(&connections[connections.size() - 1]);
	connection.b->connections.push_back(&connections[connections.size() - 1]);

	calculateLevels();
	return true;
}

bool DHT::deleteNode(DHTNode node) {
	for (int i = 0; i < nodes.size(); i++) {
		if (node == nodes[i]) {
			for (DHTConnection* connection : nodes[i].connections) {
				deleteConnection(*connection);
			}

			nodes.erase(nodes.begin() + i);

			return true;
		}
	}

	return false;
}

bool DHT::deleteConnection(DHTConnection connection) {
	for (int i = 0; i < connections.size(); i++) {
		if (connection == connections[i]) {
			vector<DHTConnection*>* a_connections = &(connections[i].a->connections);
			vector<DHTConnection*>* b_connections = &(connections[i].b->connections);

			a_connections->erase(remove(a_connections->begin(), a_connections->end(), &connections[i]), a_connections->end());
			b_connections->erase(remove(b_connections->begin(), b_connections->end(), &connections[i]), b_connections->end());

			connections.erase(connections.begin() + i);

			calculateLevels();
			return true;
		}
	}

	return false;
}

DHTNode* DHT::getNodeFromId(int id) {
	for (DHTNode node : nodes) {
		if (node.id == id) {
			return &node;
		}
	}
	
	DHTNode empty = DHTNode();
	empty.id = -1;

	return &empty;
}

int DHT::getFreeId() {
	vector<int> ids;
	for (DHTNode node : nodes) {
		ids.push_back(node.id);
	}

	sort(ids.begin(), ids.end());

	int available = 0;
	for (int id : ids) {
		if (id == available) {
			available++;
		}
		else if (id > available) {
			return available;
		}
	}
}

void DHT::calculateLevels() {
	for (DHTNode node : nodes) {
		node.level = -1;
	}

	queue<DHTNode*> current_queue;
	queue<DHTNode*> next_queue;
	int level = 0;
	current_queue.push(&nodes[0]);

	while (!current_queue.empty()) {
		DHTNode* current_node = current_queue.front();
		current_queue.pop();

		current_node->level = level;

		for (DHTConnection* connection : current_node->connections) {
			if (connection->a->level == -1) next_queue.push(connection->a);
			if (connection->b->level == -1) next_queue.push(connection->b);
		}

		if (current_queue.empty()) {
			level++;

			while (!next_queue.empty()) {
				current_queue.push(next_queue.front());
				next_queue.pop();
			}
		}
	}
}

DHTNode::DHTNode() {};

DHTNode::DHTNode(int id, int level, string ip) : id(id), level(level), ip(ip) {};

bool DHTNode::operator ==(DHTNode const& node) {
	return id == node.id;
}

DHTConnection::DHTConnection() {};

DHTConnection::DHTConnection(DHTNode* a, DHTNode* b) : a(a), b(b) {};

bool DHTConnection::operator ==(DHTConnection const& connection) {
	return (a == connection.a && b == connection.b) || (a == connection.b && b == connection.a);
}