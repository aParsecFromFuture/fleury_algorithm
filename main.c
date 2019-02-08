#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct graph graph;
typedef struct edge edge;

struct graph{
	size_t v_count;
	bool *isvisited;
	bool **adj_matrix;
};

struct edge{
	edge *next;
	int vertice_1;
	int vertice_2;
	int weight;
};

graph* create_graph(int);
void set_edge(graph*,int,int,bool);
void find_bridges(graph*,int,int,int*,int*,edge**);
bool is_bridge(graph*,int,int);
void push(edge**,int,int,int);
void fleury(graph*,int);

int main(){
	graph *g = create_graph(5);
	set_edge(g,0,1,1);
	set_edge(g,0,4,1);
	set_edge(g,1,2,1);
	set_edge(g,1,3,1);
	set_edge(g,1,4,1);
	set_edge(g,2,3,1);
	set_edge(g,2,4,1);
	set_edge(g,3,4,1);
	fleury(g,2);	
}

graph* create_graph(int v){
	graph *g = malloc(sizeof(graph));
	g->v_count = v;
	
	g->isvisited = calloc(v,sizeof(bool));

	g->adj_matrix = malloc(sizeof(bool*) * v);
	for(int i = 0;i < v;i++){
		g->adj_matrix[i] = calloc(v,sizeof(bool));
	}

	return g;
}

void set_edge(graph *g,int v1,int v2,bool w){
	g->adj_matrix[v1][v2] = w;
	g->adj_matrix[v2][v1] = w;
}

void find_bridges(graph *g,int at,int parent,int *ids,int *low,edge **bridge_ptr){
	static int id = 0;
	ids[at] = low[at] = id++;
	g->isvisited[at] = 1;
	for(int to = 0;to < g->v_count;to++){
		if(g->adj_matrix[at][to]){
			if(to == parent){
				continue;
			}
			if(!g->isvisited[to]){
				find_bridges(g,to,at,ids,low,bridge_ptr);
				low[at] = (low[at] < low[to])? low[at] : low[to];
				if(ids[at] < low[to]){
					push(bridge_ptr,at,to,g->adj_matrix[at][to]);
				}
			}else{
				low[at] = (low[at] < ids[to])? low[at] : ids[to];
			}
		}
	}
}

bool is_bridge(graph *g,int v1,int v2){
	int *ids = malloc(sizeof(int) * g->v_count);
	int *low = malloc(sizeof(int) * g->v_count);
	edge *bridges = NULL;
	find_bridges(g,v1,-1,ids,low,&bridges);
	for(edge *iter = bridges;iter != NULL;iter = iter->next){
		if((iter->vertice_1 == v1 && iter->vertice_2 == v2) || (iter->vertice_2 == v1 && iter->vertice_1 == v2)){
			return 1;
		}
	}
	memset(g->isvisited,0,sizeof(bool) * g->v_count);
	return 0;
}

void push(edge **edge_ptr,int v1,int v2,int w){
	if(*edge_ptr == NULL){
		*edge_ptr = malloc(sizeof(edge));
		(*edge_ptr)->vertice_1 = v1;
		(*edge_ptr)->vertice_2 = v2;
		(*edge_ptr)->weight = w;
		(*edge_ptr)->next = NULL;
	}else{
		edge *iter = *edge_ptr;
		while(iter->next != NULL){
			iter = iter->next;
		}
		iter->next = malloc(sizeof(edge));
		iter->next->vertice_1 = v1;
		iter->next->vertice_2 = v2;
		iter->next->weight = w;
		iter->next->next = NULL;
	}
}

void fleury(graph *g,int at){
	printf("%d, ",at);
	int bridge[3] = {-1,-1,-1};
	for(int to = 0;to < g->v_count;to++){
		if(g->adj_matrix[at][to]){
			if(is_bridge(g,at,to)){
				bridge[0] = at;
				bridge[1] = to;
				bridge[2] = g->adj_matrix[at][to];
			}else{
				set_edge(g,at,to,0);
				fleury(g,to);
				return;
			}
		}
	}

	if(bridge[0] != -1){
		set_edge(g,bridge[0],bridge[1],0);
		fleury(g,bridge[1]);
	}
}
