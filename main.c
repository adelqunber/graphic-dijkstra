#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "raylib.h"

#define MAX 100
#define WIDTH 1000
#define HEIGHT 700

typedef struct Node {
    int dest;
    int weight;
    struct Node* next;
} Node;

typedef struct {
    Node* head[MAX];
    int numVertices;
} Graph;

// ===== GRAPH =====
Node* createNode(int dest, int weight) {
    Node* n = malloc(sizeof(Node));
    n->dest = dest;
    n->weight = weight;
    n->next = NULL;
    return n;
}

Graph* createGraph(int v) {
    Graph* g = malloc(sizeof(Graph));
    g->numVertices = v;
    for (int i = 0; i < v; i++) g->head[i] = NULL;
    return g;
}

void addEdge(Graph* g, int src, int dest, int w) {
    Node* n = createNode(dest, w);
    n->next = g->head[src];
    g->head[src] = n;
}

// ===== DIJKSTRA =====
int minDistance(int dist[], int vis[], int V) {
    int min = INT_MAX, idx = -1;
    for (int i = 0; i < V; i++) {
        if (!vis[i] && dist[i] < min) {
            min = dist[i];
            idx = i;
        }
    }
    return idx;
}

void dijkstra(Graph* g, int src, int dest, int parent[]) {
    int V = g->numVertices;
    int dist[MAX], vis[MAX];

    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        vis[i] = 0;
        parent[i] = -1;
    }

    dist[src] = 0;

    for (int i = 0; i < V - 1; i++) {
        int u = minDistance(dist, vis, V);
        if (u == -1) break;
        vis[u] = 1;

        Node* temp = g->head[u];
        while (temp) {
            int v = temp->dest;
            if (!vis[v] && dist[u] + temp->weight < dist[v]) {
                dist[v] = dist[u] + temp->weight;
                parent[v] = u;
            }
            temp = temp->next;
        }
    }
}

// ===== PATH CHECK =====
int isPathEdge(int parent[], int u, int v) {
    return parent[v] == u;
}

// ===== DRAW ARROW =====
void DrawArrow(Vector2 start, Vector2 end, Color color) {
    DrawLineEx(start, end, 2, color);

    float angle = atan2f(end.y - start.y, end.x - start.x);

    float arrowSize = 10;

    Vector2 left = {
        end.x - arrowSize * cosf(angle - 0.3f),
        end.y - arrowSize * sinf(angle - 0.3f)
    };

    Vector2 right = {
        end.x - arrowSize * cosf(angle + 0.3f),
        end.y - arrowSize * sinf(angle + 0.3f)
    };

    DrawTriangle(end, left, right, color);
}

// ===== MAIN =====
int main() {
    FILE* f = fopen("ex2.txt", "r");
    if (!f) {
        printf("File error\n");
        return 1;
    }

    int N, M;
    fscanf(f, "%d %d", &N, &M);

    Graph* g = createGraph(N);

    for (int i = 0; i < M; i++) {
        int s, d, w;
        fscanf(f, "%d %d %d", &s, &d, &w);
        if (w < 0) {
            printf("Negative weight error\n");
            return 1;
        }
        addEdge(g, s, d, w);
    }

    int src, dest;
    fscanf(f, "%d %d", &src, &dest);
    fclose(f);

    int parent[MAX];
    dijkstra(g, src, dest, parent);

    // ===== NODE POSITIONS (circle layout) =====
    Vector2 pos[MAX];
    float radius = 250;
    Vector2 center = {WIDTH/2, HEIGHT/2};

    for (int i = 0; i < N; i++) {
        float angle = 2 * PI * i / N;
        pos[i].x = center.x + radius * cosf(angle);
        pos[i].y = center.y + radius * sinf(angle);
    }

    // ===== RAYLIB WINDOW =====
    InitWindow(WIDTH, HEIGHT, "Graph Visualization");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // DRAW EDGES
        for (int u = 0; u < N; u++) {
            Node* temp = g->head[u];
            while (temp) {
                int v = temp->dest;

                Color color = isPathEdge(parent, u, v) ? RED : GRAY;

                DrawArrow(pos[u], pos[v], color);

                // weight text
                Vector2 mid = {
                    (pos[u].x + pos[v].x)/2,
                    (pos[u].y + pos[v].y)/2
                };

                char wStr[10];
                sprintf(wStr, "%d", temp->weight);
                DrawText(wStr, mid.x, mid.y, 20, BLACK);

                temp = temp->next;
            }
        }

        // DRAW NODES
        for (int i = 0; i < N; i++) {
            DrawCircleV(pos[i], 20, SKYBLUE);

            char id[10];
            sprintf(id, "%d", i);
            DrawText(id, pos[i].x - 5, pos[i].y - 5, 20, BLACK);
        }

        DrawText("Red = shortest path", 20, 20, 20, RED);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}