/*
 * algo.h
 *
 *  Created on: Nov 23, 2014
 *      Author: tharun
 */

#ifndef ALGO_H_
#define ALGO_H_

//#include "algo.h"

//MCP -- Max Capacity Path

#define UNSEEN 0
#define FRINGE 1
#define INTREE 2
#define ALGO_DEBUG_FLAG 0

typedef struct data_t
{
	uint status; /* 0 = unseen, 1 = fringe, 2 = seen*/
	uint capacity;
	uint dad;
	uint ID;

} data_t;
using namespace std;

int extract_max(vector <edge> *ptree)
{
	vector <edge> :: iterator it;
	if(ptree->empty()) return -1;
	uint max_capacity = ptree->begin()->weight;
	int ret_value;

	int idx = 0;
	int max_idx = 0;
//	cout<<"The tree right now  [ID, cap]"<<endl;
	for (it = ptree->begin(); it != ptree->end(); ++it)
	{
		if(max_capacity < it->weight)
		{
			max_idx = idx;
			max_capacity = it->weight;
		}
		idx++;
#if ALGO_DEBUG_FLAG
		cout<<"["<<it->dest_ID<<","<<it->weight<<"\t";
#endif
	}
//	cout<<endl;

#if DEBUG_FLAG
	cout<<"The next deque item  Idx =  "<<max_idx<<endl;
#endif

	ret_value = ptree->at(max_idx).dest_ID;
	ptree->erase(ptree->begin()+max_idx);

	return ret_value;
}
int update_key(vector <edge> *ptree, uint ID, uint new_value)
{
	vector <edge> :: iterator it;
	if(ptree->empty()) return -1;

	for (it = ptree->begin(); it != ptree->end(); ++it)
	{
		if(ID == it->dest_ID)
		{
			it->weight = new_value;
			return 0;
		}
	}
	cout<<"Something wrong!! couldn't find ID in the list"<<endl;
	return -1;
}

int MCP_dijktras_without_heap(vector <vertex> *pgraph,uint src_ID,uint terminal_ID)
{
	vector <data_t> status_vector(VERTEX_COUNT);

	vector <data_t>:: iterator v_it;
	vector <data_t>:: iterator tree_it;

	vector <edge> tree;
	edge edge_min;


	vector <vertex>::iterator iter_vert;
	list<edge>::iterator iter_edge;

	/********************** 1st step **********************/
	int count = 0;
//	int max_capacity = INT_MAX;
	uint dad_capacity = 0;

//	cout<<"status vector initialization "<<endl;
	for(v_it = status_vector.begin(); v_it != status_vector.end(); v_it++)
	{
		v_it->ID = count;
		v_it->capacity = INT_MAX;
		v_it->dad 	= -1;
		v_it->status = 0;
		count++;
#if ALGO_DEBUG_FLAG
		cout<<"["<<v_it->ID
				<<","<<v_it->status
				<<","<<v_it->capacity
				<<","<<v_it->dad
				<<"]\t"<<endl;
#endif
	}
//	cout<<endl;

	/********************** 2nd step **********************/

	status_vector.at(src_ID).status = INTREE;
	status_vector.at(src_ID).capacity = INT_MAX;
	status_vector.at(src_ID).dad = src_ID;

//	iter_vert = pgraph->begin();
	iter_vert = pgraph->begin() + src_ID;
#if ALGO_DEBUG_FLAG
	cout<<"For source edges "<<endl;
	cout<<"[ status , capacity , dad] "<<endl;

#endif
	/********************** 3rd step **********************/
	for(iter_edge = iter_vert->edges.begin() ; iter_edge != iter_vert->edges.end(); iter_edge++)
	{
		int dest_ID = iter_edge->dest_ID;
		status_vector.at(dest_ID).status = FRINGE;
		status_vector.at(dest_ID).capacity = iter_edge->weight;
		status_vector.at(dest_ID).dad = iter_vert->vertex_ID;

		edge_min.dest_ID = dest_ID;
		edge_min.weight = iter_edge->weight;
		tree.push_back(edge_min);
#if ALGO_DEBUG_FLAG
		cout<<"["<<dest_ID
				 <<"\t"<<status_vector.at(dest_ID).status
					<<","<<status_vector.at(dest_ID).capacity
					<<","<<status_vector.at(dest_ID).dad
					<<"]\t"<<endl;
#endif
	}

//	cout<<endl;

	/********************** 4th step **********************/
	while(!tree.empty()  )
	{
		uint v_ID;
		v_ID = extract_max(&tree);

		if(v_ID == terminal_ID)
			break;

		iter_vert = pgraph->begin() + v_ID;

		dad_capacity= status_vector.at(v_ID).capacity;

		for(iter_edge = iter_vert->edges.begin() ; iter_edge != iter_vert->edges.end(); iter_edge++)
		{
			int dest_ID = iter_edge->dest_ID;

			if(status_vector.at(dest_ID).status == UNSEEN)
			{
				status_vector.at(dest_ID).status = FRINGE;
				status_vector.at(dest_ID).capacity = min(dad_capacity,iter_edge->weight);
				status_vector.at(dest_ID).dad = v_ID;

				edge_min.dest_ID = dest_ID;
				edge_min.weight = status_vector.at(dest_ID).capacity;
				tree.push_back(edge_min);
			}
			else if((status_vector.at(dest_ID).status == FRINGE) &&
					(status_vector.at(dest_ID).capacity < min(dad_capacity,iter_edge->weight)))
			{
				status_vector.at(dest_ID).capacity = min(dad_capacity,iter_edge->weight);
				status_vector.at(dest_ID).dad = v_ID;

				/* change the capacity value, in the tree*/
				update_key(&tree,dest_ID,status_vector.at(dest_ID).capacity);

			}
		}
#if ALGO_DEBUG_FLAG
		cout<<"status prints"<<endl;
		for(v_it = status_vector.begin(); v_it != status_vector.end(); v_it++)
		{
			cout<<"["<<v_it->ID
					<<"\t"<<v_it->status
					<<","<<v_it->capacity
					<<","<<v_it->dad
					<<"]\t"<<endl;

		}
		cout<<endl;
#endif
		status_vector.at(v_ID).status = INTREE;
	}

	/* traversing the dad array */
	uint var_ID = terminal_ID;
	int ct =0;

	while(var_ID != src_ID)
	{
		cout<<var_ID<<", ";
		var_ID = status_vector.at(var_ID).dad;
		ct++;
		if(ct > VERTEX_COUNT-1)
		{
			cout<<"path not formed properly"<<endl;
			 break;
		}
	}
	cout<<var_ID<<endl;
	return status_vector.at(terminal_ID).capacity;
}


int MCP_dijktras_with_heap(vector <vertex> *pgraph,uint src_ID,uint terminal_ID)
{
	vector <data_t> status_vector(VERTEX_COUNT);
	vector <data_t>:: iterator v_it;

    Heap* s_heap = new Heap();

//	vector <edge> heap_vector;
	edge edge_tmp;

//	edge_tmp.src_ID = 0;

	vector <vertex>::iterator iter_vert;
	list<edge>::iterator iter_edge;

	/********************** 1st step **********************/
	int count = 0;
//	int max_capacity = INT_MAX;
	uint dad_capacity = 0;

//	cout<<"status vector initialization "<<endl;
	for(v_it = status_vector.begin(); v_it != status_vector.end(); v_it++)
	{
		v_it->ID = count;
		v_it->capacity = INT_MAX;
		v_it->dad 	= -1;
		v_it->status = 0;
		count++;
#if ALGO_DEBUG_FLAG
		cout<<"["<<v_it->ID
				<<","<<v_it->status
				<<","<<v_it->capacity
				<<","<<v_it->dad
				<<"]\t"<<endl;
#endif

	}
//	cout<<endl;

	/********************** 2nd step **********************/

	status_vector.at(src_ID).status = INTREE;
	status_vector.at(src_ID).capacity = INT_MAX;
	status_vector.at(src_ID).dad = src_ID;

//	iter_vert = pgraph->begin();
	iter_vert = pgraph->begin() + src_ID;
#if ALGO_DEBUG_FLAG
	cout<<"For source edges "<<endl;
	cout<<"[ status , capacity , dad] "<<endl;
#endif
	/********************** 3rd step **********************/
	for(iter_edge = iter_vert->edges.begin() ; iter_edge != iter_vert->edges.end(); iter_edge++)
	{
		int dest_ID = iter_edge->dest_ID;
		status_vector.at(dest_ID).status = FRINGE;
		status_vector.at(dest_ID).capacity = iter_edge->weight;
		status_vector.at(dest_ID).dad = iter_vert->vertex_ID;

		edge_tmp.dest_ID = dest_ID;
		edge_tmp.weight = iter_edge->weight;
		s_heap->insert(edge_tmp);
#if ALGO_DEBUG_FLAG
		cout<<"["<<dest_ID
				 <<"\t"<<status_vector.at(dest_ID).status
					<<","<<status_vector.at(dest_ID).capacity
					<<","<<status_vector.at(dest_ID).dad
					<<"]\t"<<endl;
#endif
	}

//	cout<<endl;

	/********************** 4th step **********************/
	while(s_heap->size())
	{
		uint v_ID;
//		v_ID = extract_max(&heap_vector);
		edge max_edge = s_heap->extract_max();
		v_ID = max_edge.dest_ID;
//		cout<<"_________VID =  "<<v_ID<<endl;

		if(v_ID == terminal_ID)
			break;

		iter_vert = pgraph->begin() + v_ID;

		dad_capacity= status_vector.at(v_ID).capacity;

		for(iter_edge = iter_vert->edges.begin() ; iter_edge != iter_vert->edges.end(); iter_edge++)
		{
			int dest_ID = iter_edge->dest_ID;

			if(status_vector.at(dest_ID).status == UNSEEN)
			{
				status_vector.at(dest_ID).status = FRINGE;
				status_vector.at(dest_ID).capacity = min(dad_capacity,iter_edge->weight);
				status_vector.at(dest_ID).dad = v_ID;

				edge_tmp.dest_ID = dest_ID;
				edge_tmp.weight = status_vector.at(dest_ID).capacity;
//				heap_vector.push_back(edge_tmp);
				s_heap->insert(edge_tmp);

			}
			else if((status_vector.at(dest_ID).status == FRINGE) &&
					(status_vector.at(dest_ID).capacity < min(dad_capacity,iter_edge->weight)))
			{
				status_vector.at(dest_ID).capacity = min(dad_capacity,iter_edge->weight);
				status_vector.at(dest_ID).dad = v_ID;

				/* change the capacity value, in the tree*/
				s_heap->update_key(dest_ID,status_vector.at(dest_ID).capacity);

			}
		}

#if ALGO_DEBUG_FLAG
		cout<<"status prints"<<endl;
		for(v_it = status_vector.begin(); v_it != status_vector.end(); v_it++)
		{
			cout<<"["<<v_it->ID
					<<"\t"<<v_it->status
					<<","<<v_it->capacity
					<<","<<v_it->dad
					<<"]\t"<<endl;

		}
		cout<<endl;
#endif

		status_vector.at(v_ID).status = INTREE;
	}

	/* traversing the dad array */
		uint var_ID = terminal_ID;
		int ct =0;
		while(var_ID != src_ID)
		{
			cout<<var_ID<<", ";
			var_ID = status_vector.at(var_ID).dad;
			ct++;
			if(ct > VERTEX_COUNT-1)
			{
				cout<<"path not formed properly"<<endl;
				break;
			}
		}
		cout<<var_ID<<endl;

	delete s_heap;
	return status_vector.at(terminal_ID).capacity;
}

//#prag

int find_root(vector <int> *pdad_array, int vertex_id)
{
//	cout<<"ID "<<vertex_id<<endl;
	if(pdad_array->at(vertex_id) == vertex_id)
	{
//		cout <<" the root"<<vertex_id<<endl;
		return vertex_id;
	}
	else
	{
		pdad_array->at(vertex_id) = find_root(pdad_array,pdad_array->at(vertex_id));
		return (pdad_array->at(vertex_id));
	}
}

void Union(vector <int> *pdad_array, vector <int> *prank_array, int root1, int root2)
		{

		    // Attach smaller rank tree under root of high rank tree
		    // (Union by Rank)
		    if (prank_array->at(root1) < prank_array->at(root2))
		        pdad_array->at(root1) = root2;
		    else if (prank_array->at(root1) > prank_array->at(root2))
		        pdad_array->at(root2) = root1;

		    // If ranks are same, then make one as root and increment
		    // its rank by one
		    else
		    {
		        pdad_array->at(root2) = root1;
		        prank_array->at(root1)++;
		    }

#if ALGO_DEBUG_FLAG
		    cout<<"arrays"<<endl;
		    uint  i;
		    for (i	 = 0; i < pdad_array->size(); ++i) {

		    	cout<< i <<", ";
		    }
		    cout<<endl;
		    for (i	 = 0; i < pdad_array->size(); ++i) {

		    	cout<< pdad_array->at(i) <<", ";
		    }
		    cout<<endl;
		    for (i	 = 0; i < prank_array->size(); ++i) {

		    	cout<< prank_array->at(i) <<", ";
		    }
		    cout<<endl;
#endif
		}

int DFSUtil(vector <vertex> *pgraph, int v,int final_dest_ID, bool visited[], uint min)
{
    // Mark the current node as visited and print it
    visited[v] = true;
#if ALGO_DEBUG_FLAG
    cout << v << " ";
#endif
    int ret_valu = -1;
    // Recur for all the vertices adjacent to this vertex
    if(v == final_dest_ID)
    {
//    	cout<<"reached final dest __________________________________-"<<final_dest_ID<<endl;
//    	cout<<"min capacity "<<min<<endl;
    	for (int i = 0; i < VERTEX_COUNT; ++i)
    	{
    	    visited[i] = true;
		}
    	return min;
    }
#if ALGO_DEBUG_FLAG

	cout<<"current id final id current min -->\t";
    cout<<v<<"\t"<<final_dest_ID<<"\t"<<min<<endl;
#endif
    vector <vertex>::iterator iter_vert;
	list<edge>::iterator iter_edge;

	iter_vert = pgraph->begin() + v;
	for(iter_edge = iter_vert->edges.begin() ; iter_edge != iter_vert->edges.end(); iter_edge++)
	{
		int max_vert_id = INT_MAX;
#if ALGO_DEBUG_FLAG
		cout<<"des src weight "<< iter_edge->dest_ID<<","<<iter_edge->src_ID<<","<<iter_edge->weight<<endl;
#endif
		if(!visited[iter_edge->dest_ID])
		{
			int dfs_val = DFSUtil(pgraph,iter_edge->dest_ID, final_dest_ID,visited, MIN(iter_edge->weight,min));
			if(ret_valu < dfs_val)
			{
				ret_valu = dfs_val; /* taking maximum */
				max_vert_id = iter_edge->dest_ID;/* storing maximum ID*/
				cout<<max_vert_id<<", ";

			}
		}
	}
	return ret_valu;
}

// DFS traversal of the vertices reachable from v. It uses recursive DFSUtil()
int DFS(vector <vertex> *pgraph, int src_ID,int dest_ID)
{
    // Mark all the vertices as not visited
    bool visited[VERTEX_COUNT];
    for(int i = 0; i < VERTEX_COUNT; i++)
        visited[i] = false;

    // Call the recursive helper function to print DFS traversal
    return DFSUtil(pgraph,src_ID, dest_ID,visited , INT_MAX);
}


int MCP_kruskal(vector <vertex> *pgraph,uint src_ID,uint terminal_ID)
{
/* Sorting Edges First */
    Heap* edge_heap = new Heap();
	vector <edge>:: iterator edge_it;

	vector <int> dad_array(VERTEX_COUNT);
	vector <int> rank_array(VERTEX_COUNT);

	vector <int>::iterator dad_ind,rank_ind;

	vector <vertex>::iterator iter_vert;
		list<edge>::iterator iter_edge;

		for(iter_vert = pgraph->begin() ; iter_vert != pgraph->end(); iter_vert++)
		{
			/* to check if the edge to that node already exists */
			for(iter_edge = iter_vert->edges.begin(); iter_edge != iter_vert->edges.end(); iter_edge++)
			{
				if(iter_edge->dest_ID >= iter_vert->vertex_ID)
				{
					iter_edge->src_ID = iter_vert->vertex_ID;
					edge_heap->insert(*iter_edge);
				}
			}
		}
#if ALGO_DEBUG_FLAG

		cout<< "edges_list count "<<edge_heap->size()<<endl;
#endif

		/* data is already heapified as per the above routine */
//		cout<<endl;
		for (int i = 0; i < (VERTEX_COUNT); ++i)
		{
			dad_array.at(i) = i;
			rank_array.at(i) = 0;
//			cout<<dad_array.at(i)<<","<<rank_array.at(i)<<'\t';
		}
//		cout <<endl;
/* KRUSKAL */
		{
			int count = 0;
			edge edge_reverse;
			edge edge_forwd;
			vector <vertex> MST_graph(VERTEX_COUNT);

			for(iter_vert = MST_graph.begin(); iter_vert != MST_graph.end(); iter_vert++)
			{
				iter_vert->vertex_ID = count;
//				cout<<iter_vert->vertex_ID<<endl;
				count++;
			}


			count = 0;
			while(edge_heap->size())
			{
//				cout<<count<<endl;
				edge_forwd = edge_heap->extract_max();
#if ALGO_DEBUG_FLAG
				cout<<"extracted max complete = "<<edge_forwd.weight<<endl;

				cout<<edge_forwd.src_ID<<endl;
				cout<<edge_forwd.dest_ID<<endl;
#endif

				int root_1 = find_root(&dad_array,edge_forwd.src_ID);
				int root_2 = find_root(&dad_array,edge_forwd.dest_ID);
#if ALGO_DEBUG_FLAG
				cout<<root_1 <<"<-- root1"<<endl;
				cout<<root_2 <<"<-- root2"<<endl;
#endif

				if(root_1 != root_2)
				{
					Union(&dad_array,&rank_array,root_1,root_2);

					MST_graph.at(edge_forwd.src_ID).edges.push_back(edge_forwd);
					edge_reverse.dest_ID = edge_forwd.src_ID;
					edge_reverse.weight = edge_forwd.weight;
					edge_reverse.src_ID = edge_forwd.dest_ID;
					MST_graph.at(edge_forwd.dest_ID).edges.push_back(edge_reverse);

//					print_graph(&MST_graph);
					count++;
				}
#if ALGO_DEBUG_FLAG
				else
				{

					cout <<"cycle formed with edge "<< edge_forwd.src_ID<<"<--"
							<<edge_forwd.weight<<"-->"<<edge_forwd.dest_ID<<endl;
				}
#endif
			}

//			cout<<"MST built = "<<count<<endl;
#if ALGO_DEBUG_FLAG
			print_graph(&MST_graph);
#endif
			int ret_value = DFS(&MST_graph, src_ID, terminal_ID);
			if(ret_value<0)
			{
				cout<<"Node not found in DFS"<<endl;
				return -1;
			}
			else
			{
				cout<<src_ID<<"."<<endl;
				return ret_value;
			}

		}


}



#endif /* ALGO_H_ */
