#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>

#define EWIDTH 6
#define EHEIGHT 6

using namespace std;

typedef struct _PathNode {
	int x, y;
	int turn_times;
	_PathNode *parent;

	_PathNode(int _x, int _y, int t) {
		x = _x; y = _y; turn_times = t; parent = NULL;
	}
} PathNode;


void
push_queue(vector<int> &map, queue<PathNode*> &q, unordered_set<int> &finded, PathNode *cur, int x, int y) {
	if (x < 0 || y < 0 || x >= EWIDTH || y >= EHEIGHT) return;
	if (finded.find(x * 1000 + y) != finded.end()) return;
	if (map[y * EHEIGHT + x] == 1) return;

	bool isTurn = cur->parent && ((cur->parent->x == cur->x && cur->x != x) || (cur->parent->y == cur->y && cur->y != y));
	int turn_times = isTurn ? cur->turn_times + 1 : cur->turn_times;

	if (turn_times > 2) return;

	PathNode *n = new PathNode(x, y, turn_times);
	n->parent = cur;
	q.push(n);
}

PathNode *
find_path(vector<int> &map, int sx, int sy, int ex, int ey) {
	PathNode *startNode = new PathNode(sx, sy, 0);
	queue<PathNode*> q;
	unordered_set<int> finded;

	q.push(startNode);

	while (!q.empty()) {
		PathNode *cur = q.front();
		q.pop();
		finded.insert(cur->x * 1000 + cur->y);

		if (cur->x == ex && cur->y == ey && cur->turn_times <= 2) {
			return cur;
		}

		push_queue(map, q, finded, cur, cur->x, cur->y - 1);
		push_queue(map, q, finded, cur, cur->x, cur->y + 1);
		push_queue(map, q, finded, cur, cur->x - 1, cur->y);
		push_queue(map, q, finded, cur, cur->x + 1, cur->y);
	}
	return NULL;
}

int main(int argc, char **argv) {
	vector<int> v = {
		1, 1, 1, 1, 0, 0,
		1, 0, 0, 0, 0, 0,
		1, 0, 1, 1, 0, 0,
		1, 0, 1, 1, 0, 0,
		1, 0, 0, 0, 0, 0,
		1, 1, 1, 1, 0, 0,
	};
	PathNode *path = find_path(v, 1, 1, 1, 4);
cout << "path:" << path << endl;
	while (path) {
		cout << path->x << "," << path->y << endl;
		PathNode *temp = path;
		path = path->parent;
		delete temp;
	}
	return 0;
}