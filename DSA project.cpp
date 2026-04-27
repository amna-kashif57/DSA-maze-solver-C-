#include <utility>
#include <iostream>
using namespace std;

const int SIZE = 5;

char maze[SIZE][SIZE] = {
    {'S', '0', '1', '0', '0'},
    {'1', '0', '1', '0', '1'},
    {'1', '0', '0', '0', '1'},
    {'1', '1', '1', '0', '1'},
    {'0', '0', '0', 'E', '1'}
};

bool visited[SIZE][SIZE];
int dx[] = { -1, 0, 1, 0 };
int dy[] = { 0, 1, 0, -1 };

struct StackNode {
    int x, y;
};
class Stack {
    StackNode data[SIZE * SIZE] = {};
    int top = -1;
public:
    void push(int x, int y) {
        data[++top] = { x, y };
    }
    StackNode pop() {
        return data[top--];
    }
    bool isEmpty() {
        return top == -1;
    }
};

struct QueueNode {
    int x, y;
};
class Queue {
    QueueNode data[SIZE * SIZE] = {};
    int front = 0, rear = 0;
public:
    void enqueue(int x, int y) {
        data[rear++] = { x, y };
    }
    QueueNode dequeue() {
        return data[front++];
    }
    bool isEmpty() {
        return front == rear;
    }
};

class LinkedList {
    struct ListNode {
        int x, y;
        ListNode* next;
        ListNode(int a, int b) : x(a), y(b), next(nullptr) {}
    };
    ListNode* head = nullptr, * tail = nullptr;
public:
    void addToEnd(int x, int y) {
        ListNode* newNode = new ListNode(x, y);
        if (!head) head = tail = newNode;
        else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    void display() {
        cout << "Path: ";
        ListNode* temp = head;
        while (temp) {
            cout << "(" << temp->x << "," << temp->y << ") ";
            temp = temp->next;
        }
        cout << endl;
    }
};

class TreeNode {
public:
    int x, y;
    TreeNode* parent;
    TreeNode* children[4];
    int childCount;

    TreeNode(int a, int b, TreeNode* p = nullptr) {
        x = a;
        y = b;
        parent = p;
        childCount = 0;
        for (int i = 0; i < 4; i++)
            children[i] = nullptr;
    }
};

void printMaze() {
    cout << "\nMaze:\n";
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++)
            cout << maze[i][j] << ' ';
        cout << endl;
    }
}

void findStartEnd(int& sx, int& sy, int& ex, int& ey) {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++) {
            if (maze[i][j] == 'S') {
                sx = i;
                sy = j;
            }
            if (maze[i][j] == 'E') {
                ex = i;
                ey = j;
            }
        }
}

void solveDFS(int sx, int sy, int ex, int ey) {
    cout << "\nDFS Path:\n";
    Stack stack;
    LinkedList path;

    TreeNode* root = new TreeNode(sx, sy);
    TreeNode* treeMap[SIZE][SIZE] = {};
    stack.push(sx, sy);
    visited[sx][sy] = true;
    treeMap[sx][sy] = root;

    while (!stack.isEmpty()) {
        StackNode current = stack.pop();
        int x = current.x, y = current.y;

        if (x == ex && y == ey) {
            cout << "Reached End (DFS)\n";
            TreeNode* node = treeMap[x][y];
            while (node) {
                path.addToEnd(node->x, node->y);
                node = node->parent;
            }
            path.display();
            return;
        }

        TreeNode* parentNode = treeMap[x][y];
        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i], ny = y + dy[i];
            if (nx >= 0 && ny >= 0 && nx < SIZE && ny < SIZE &&
                !visited[nx][ny] && maze[nx][ny] != '1') {
                visited[nx][ny] = true;
                stack.push(nx, ny);

                TreeNode* child = new TreeNode(nx, ny, parentNode);
                parentNode->children[parentNode->childCount++] = child;
                treeMap[nx][ny] = child;
            }
        }
    }
    cout << "No path found with DFS.\n";
}

void solveBFS(int sx, int sy, int ex, int ey) {
    cout << "\nBFS Path:\n";
    Queue q;
    bool vis[SIZE][SIZE] = {};
    pair<int, int> parent[SIZE][SIZE];
    q.enqueue(sx, sy);
    vis[sx][sy] = true;
    parent[sx][sy] = make_pair(-1, -1);
    bool found = false;

    while (!q.isEmpty()) {
        QueueNode current = q.dequeue();
        int x = current.x, y = current.y;

        if (x == ex && y == ey) {
            found = true;
            break;
        }

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i], ny = y + dy[i];
            if (nx >= 0 && ny >= 0 && nx < SIZE && ny < SIZE &&
                !vis[nx][ny] && maze[nx][ny] != '1') {
                vis[nx][ny] = true;
                parent[nx][ny] = make_pair(x, y);
                q.enqueue(nx, ny);
            }
        }
    }

    if (!found) {
        cout << "No path found with BFS.\n";
        return;
    }

    LinkedList path;
    pair<int, int> steps[SIZE * SIZE];
    int count = 0;
    int x = ex, y = ey;

    while (x != -1 && y != -1) {
        steps[count++] = make_pair(x, y);
        int px = parent[x][y].first;
        int py = parent[x][y].second;
        x = px;
        y = py;
    }

    for (int i = count - 1; i >= 0; i--) {
        path.addToEnd(steps[i].first, steps[i].second);
    }

    cout << "Reached End (BFS)\n";
    path.display();
}

int main() {
    int sx, sy, ex, ey;
    printMaze();
    findStartEnd(sx, sy, ex, ey);
    solveDFS(sx, sy, ex, ey);
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            visited[i][j] = false;
    solveBFS(sx, sy, ex, ey);
    return 0;
}