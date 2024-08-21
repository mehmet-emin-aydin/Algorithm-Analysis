#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Graph Node representing a node in the graph
typedef struct Node {
    int id;
    struct Edge *edges; // Linked list of edges connected to this vertex
} Node;

// Graph Edge representing a connection between two vertices
typedef struct Edge {
    struct Node *dest; // Destination Node of the edge
    struct Edge *next; // Next edge in the linked list
    double centrality; // Centrality value for betweenness centrality
} Edge;

// Graph structure representing the entire graph
typedef struct {
    Node *nodes; // Array of vertices (nodes) in the graph
    int nodeCount; // Number of vertices in the graph
} Graph;

// Community structure representing a group of vertices in a community
typedef struct {
    int *nodes; // Array of vertices in the community
    int size;    // Number of vertices in the community
} Community;

// Function declarations

// Function declarations
/**
 * @brief Initializes the graph with a specified number of nodes.
 *
 * @param graph Pointer to the Graph structure to be initialized.
 * @param nodeCount Number of nodes (vertices) in the graph.
 */
void initGraph(Graph *graph, int nodeCount);

/**
 * @brief Reads graph structure from a file and adds edges to the graph.
 *
 * @param graph Pointer to the Graph structure to which edges will be added.
 * @param filename Name of the file containing the graph structure.
 */
void loadGraphFromFile(Graph *graph, const char *filename);
/**
 * @brief Adds an edge between two specified nodes in the graph.
 *
 * @param graph Pointer to the Graph structure to which the edge will be added.
 * @param sourceId ID of the source node for the edge.
 * @param destId ID of the destination node for the edge.
 */
void addEdgeToGraph(Graph *graph, int sourceId, int destId);

/**
 * @brief Prints the adjacency list representation of the graph.
 *
 * @param graph Pointer to the Graph structure to be printed.
 */
void displayGraph(Graph *graph);

/**
 * @brief Identifies communities in the graph using BFS.
 *
 * @param graph Pointer to the Graph structure in which communities will be identified.
 * @param communities Array of Community structures to store the identified communities.
 *
 * @return Number of identified communities.
 */
int findCommunities(Graph *graph, Community *communities);

/**
 * @brief Performs Breadth-First Search (BFS) starting from a specified node.
 *
 * @param graph Pointer to the Graph structure in which BFS will be performed.
 * @param startNode ID of the starting node for BFS.
 * @param visited Array indicating whether a node has been visited.
 * @param community Pointer to the Community structure to store the BFS result.
 */
void runBreadthFirstSearch(Graph *graph, int startNode, int *visited, Community *community);


/**
 * @brief Computes betweenness centrality for edges in the graph.
 *
 * @param graph Pointer to the Graph structure for which centrality will be computed.
 */
void computeCentrality(Graph *graph);

/**
 * @brief Removes a specific edge from a vertex in the graph.
 *
 * @param node Pointer to the Node structure representing the vertex.
 * @param destId ID of the destination node for the edge to be removed.
 */
void removeEdgeFromNode(Node *node, int destId);

/**
 * @brief Removes edges with the highest centrality value from the graph.
 *
 * @param graph Pointer to the Graph structure from which edges will be removed.
 */
void deleteHighestEdges(Graph *graph);


/**
 * @brief Main function for the program.
 *
 * Manages the overall execution flow of the community detection algorithm.
 *
 * @return Exit status.
 */
int main() {
    Graph socialGraph;
    int i, j, k, t;
    // Variables for tracking stability and small community conditions
    int prevCount = 0;
    int stableCount = 0;
    bool underMin = false;
    int finished = 0;
	Community *communities;
    int currentCount;
    Edge *temp;
    
    
    
    // Initialize the graph with 26 nodes (assuming A-Z)
    initGraph(&socialGraph, 26);

    // User input for stable iterations (k) and minimum community size (t)
    printf("Enter k (number of stable iterations): ");
    scanf("%d", &k);
    printf("Enter t (minimum number of members in a community): ");
    scanf("%d", &t);

    // Read graph structure from a file
    loadGraphFromFile(&socialGraph, "graph.txt");

    // Print the initial state of the graph
    displayGraph(&socialGraph);

    // Allocate memory for community information
    communities = (Community *)malloc(socialGraph.nodeCount * sizeof(Community));
    if (communities == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }


    // Iteratively identify communities and update graph
    while (!underMin && !finished) {
        currentCount = findCommunities(&socialGraph, communities);

        // Print communities
        for (i = 0; i < currentCount; i++) {
            printf("Community %d: ", i);
            for (j = 0; j < communities[i].size; j++) {
                printf("%d ", communities[i].nodes[j]);
            }
            printf("\n");
        }

        // Check for stability in the number of communities
        if (currentCount == prevCount) {
            stableCount++;
        } else {
            stableCount = 0;
        }
        prevCount = currentCount;

        // Check for stability condition (stable for k iterations)
        if (stableCount >= k) {
            printf("Reason for stopping: Community count did not change during %d iterations\n", k);
            finished = 1;
        }

        // Perform iterations if stability condition is not met
        if (!finished) {
            computeCentrality(&socialGraph);
            displayGraph(&socialGraph);
            deleteHighestEdges(&socialGraph);
            displayGraph(&socialGraph);

            // Check for small communities (fewer than t members)
            for (i = 0; i < currentCount; i++) {
                if (communities[i].size < t) {
                    underMin = true;
                }
            }

            // finished condition if a small community is found
            if (underMin) {
                printf("Reason for stopping: A community has less than %d members.\n", t);
            }
        }
    }

    // Free allocated memory
    for (i = 0; i < socialGraph.nodeCount; i++) {
        temp = socialGraph.nodes[i].edges;
        while (socialGraph.nodes[i].edges) {
            temp = temp->next;
            free(socialGraph.nodes[i].edges);
            socialGraph.nodes[i].edges = temp;
        }
        free(communities[i].nodes);
    }

    free(socialGraph.nodes);
    free(communities);

    return 0;
}

// Initialize the graph with a specified number of nodes
void initGraph(Graph *graph, int nodeCount) {
    int i;
    graph->nodeCount = nodeCount;
    graph->nodes = (Node *)malloc(nodeCount * sizeof(Node));
    if (graph->nodes == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // Initialize nodes
    for (i = 0; i < nodeCount; i++) {
        graph->nodes[i].id = i;
        graph->nodes[i].edges = NULL;
    }
}

// read graph from file and add edges
void loadGraphFromFile(Graph *graph, const char *filename) {
	char line[256];
    int nodeCount = 0;
    int source,dest;
    FILE *file = fopen(filename, "r"); // Update the file name and mode
    if (file == NULL) {
        fprintf(stderr, "Error opening the file.\n");
        exit(EXIT_FAILURE);
    }
    // Read each line from the file
    while (fgets(line, sizeof(line), file)) {
        // Find ":" character and get the source
        char *token = strtok(line, ":");
        source = atoi(token);
        // Read destinations using "," and ";" characters
        token = strtok(NULL, " ,;\n");
        while (token != NULL) {
            dest = atoi(token);
			addEdgeToGraph(graph, source, dest);

            // Get the next destination
            token = strtok(NULL, " ,;\n");
        }
    }

    fclose(file);
}

// Add an edge to the graph between two specified nodes
void addEdgeToGraph(Graph *graph, int sourceId, int destId) {
    // Create a new edge from source to destination
    Edge *newEdge = (Edge *)malloc(sizeof(Edge));
    if (newEdge == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    newEdge->centrality = 0;
    newEdge->dest = &graph->nodes[destId];
    newEdge->next = graph->nodes[sourceId].edges;
    graph->nodes[sourceId].edges = newEdge;
}

// Print the adjacency list representation of the graph
void displayGraph(Graph *graph) {
    int i = 0;
    while (i < graph->nodeCount) {
        if (graph->nodes[i].edges != NULL) {
            printf("%d: ", i);
            Edge *tmp = graph->nodes[i].edges;
            while (tmp != NULL) {
                printf("%d(%.4f) ", tmp->dest->id, tmp->centrality);
                tmp = tmp->next;
            }
            printf("\n");
        }
        i++;
    }
}


// Identify communities in the graph using BFS
int findCommunities(Graph *graph, Community *communities) {
    int *visited = (int *)calloc(graph->nodeCount, sizeof(int));
    int currentCount = 0;
    int i;

    for (i = 0; i < graph->nodeCount; i++) {
        if (graph->nodes[i].edges != NULL && !visited[i]) {
            communities[currentCount].nodes = (int *)malloc(graph->nodeCount * sizeof(int));
            communities[currentCount].size = 0;

            // Perform BFS from unvisited node
            runBreadthFirstSearch(graph, i, visited, &communities[currentCount]);

            currentCount++;
        }
    }

    free(visited);
    return currentCount;
}


// Perform Breadth-First Search starting from a specified node
void runBreadthFirstSearch(Graph *graph, int startNode, int *visited, Community *community) {
    int *queue = (int*)malloc((graph->nodeCount)*sizeof(int));
    int front = 0, end = 0;
    Edge *e;
	int neighbor;
    queue[end++] = startNode;
    visited[startNode] = 1;

    while (front < end) {
        int currentNode = queue[front++];

        community->nodes[community->size++] = currentNode;

        for (e = graph->nodes[currentNode].edges; e != NULL; e = e->next) {
            neighbor = e->dest->id;
            if (!visited[neighbor]) {
                visited[neighbor] = 1;
                queue[end++] = neighbor;
            }
        }
    }

}


// compute betweenness centrality for edges in the graph
void computeCentrality(Graph *graph) {
    int i, j,queueStart, queueEnd,current,neighbor;
    Edge *e;
    int *visited;
    int *shortestPaths;
    double *dependency;
    int *queue;
    // Reset all centralities at the start
    for (j = 0; j < graph->nodeCount; j++) {
        for (e = graph->nodes[j].edges; e != NULL; e = e->next) {
            e->centrality = 0;
        }
    }

    // compute centrality
    for (i = 0; i < graph->nodeCount; i++) {
        // Allocate the arrays
        visited = (int *)malloc(graph->nodeCount * sizeof(int));
        shortestPaths = (int *)malloc(graph->nodeCount * sizeof(int));
        dependency = (double *)malloc(graph->nodeCount * sizeof(double));
        if (visited == NULL || shortestPaths == NULL || dependency == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }

        // Reset arrays
        for (j = 0; j < graph->nodeCount; j++) {
            visited[j] = 0;
            shortestPaths[j] = 0;
            dependency[j] = 0.0;
        }

        // Initialize BFS queue
        queue = (int *)malloc(graph->nodeCount * sizeof(int));
        if (queue == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }

        queueStart = 0;
		queueEnd = 0;
        queue[queueEnd++] = i;
        visited[i] = 1;
        shortestPaths[i] = 1;

        // BFS
        while (queueStart < queueEnd) {
            current = queue[queueStart++];
            for (e = graph->nodes[current].edges; e!= NULL; e = e->next) {
                neighbor = e->dest->id;
                if (visited[neighbor] == 0) {
                    visited[neighbor] = visited[current] + 1;
                    queue[queueEnd++] = neighbor;
                }
                if (visited[neighbor] == visited[current] + 1) {
                    shortestPaths[neighbor] += shortestPaths[current];
                }
            }
        }

        // compute dependency
        for (j = queueEnd - 1; j >= 0; j--) {
            current = queue[j];
            double addition = 1.0 + dependency[current];
            for (e = graph->nodes[current].edges; e != NULL; e = e->next) {
                if (visited[e->dest->id] == visited[current] - 1) {
                    double percentage = (double)shortestPaths[e->dest->id] / shortestPaths[current];
                    dependency[e->dest->id] += percentage * addition;
                    e->centrality += percentage * addition;
                }
            }
        }

        free(visited);
        free(queue);
        free(shortestPaths);
        free(dependency);
    }
}

// Delete edges with the highest centrality value from the graph
void deleteHighestEdges(Graph *graph) {
    double maxCentrality = 0.0;
    int i,destId;
	Edge *current;
    // Find the highest centrality edges
    for (i = 0; i < graph->nodeCount; i++) {
        current = graph->nodes[i].edges;
        while (current != NULL) {
            if (current->centrality > maxCentrality) {
                maxCentrality = current->centrality;
            }
            current = current->next;
        }
    }

    //Delete the highest centrality edges
    for (i = 0; i < graph->nodeCount; i++) {
        current = graph->nodes[i].edges;
        while (current != NULL) {
            if (current->centrality == maxCentrality) {
                destId = current->dest->id;
                removeEdgeFromNode(&graph->nodes[i], destId);
                removeEdgeFromNode(&graph->nodes[destId], i);
                printf("Deleted edge: %d - %d\n", i, destId);
                current = graph->nodes[i].edges;
            } else {
                current = current->next;
            }
        }
    }
}



// Delete the edge with given infos from a node
void removeEdgeFromNode(Node *node, int destId) {
    Edge *current = node->edges;
    Edge *previous = NULL;

    while (current != NULL) {
        if (current->dest->id == destId) {
            if (previous == NULL) {
                node->edges = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            break;
        }
        previous = current;
        current = current->next;
    }
}







