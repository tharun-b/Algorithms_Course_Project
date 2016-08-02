/*
 * graph.h
 *
 *  Created on: Nov 22, 2014
 *      Author: Tharun Battula
 */

#ifndef GRAPH_H_
#define GRAPH_H_


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <iostream>
#include <iterator>
#include <time.h>
#include <vector>
#include <list>
using namespace std;


typedef struct vertex_t
{
	uint vertex_ID;
	list <edge> edges;
} vertex;

#define DENSE   0
#define VERTEX_COUNT 1000
#define MAX_EDGE_WEIGHT (1<<14)
#define DEBUG_FLAG 0
#define GRAPH_PRINT_FLAG 0

#define SPARSE_DEGREE  3
#define DENSE_DEGREE  ((int)(VERTEX_COUNT * 0.2))


#define MIN(a, b) ((a < b) ? a : b)
#define MAX(a, b) ((a > b) ? a : b)


int remove_number_at(vector<int> *pvector, uint index )
{

#if DEBUG_FLAG
	vector <int> ::iterator v_it;
	cout<<endl;
	for(v_it = pvector->begin() ; v_it != pvector->end(); v_it++)
		cout<<*v_it<<" ";
	cout<<endl;
#endif
	pvector->at(index) = pvector->back();
	pvector->pop_back();

#if DEBUG_FLAG
	for(v_it = pvector->begin() ; v_it != pvector->end(); v_it++)
		cout<<*v_it<<" ";
	cout<<endl;
#endif

	return 0;

}
int remove_number(vector<int> *pvector, int value )
{
	vector <int> ::iterator v_it;
#if DEBUG_FLAG
	cout<<endl;
	for(v_it = pvector->begin() ; v_it != pvector->end(); v_it++)
		cout<<*v_it<<" ";
	cout<<endl;
#endif
	for(v_it = pvector->begin() ; v_it != pvector->end(); v_it++)
	{
		if(*v_it == (int)value)
		{
			*v_it =pvector->back();
			pvector->pop_back();
			return 0;
		}
	}
	return -1;
#if DEBUG_FLAG
	for(v_it = pvector->begin() ; v_it != pvector->end(); v_it++)
		cout<<*v_it<<" ";
	cout<<endl;
#endif

	return 0;

}
void print_graph(vector <vertex> *pgraph)
{
	vector <vertex>::iterator iter_vert;
	list<edge>::iterator iter_edge;

	/***********************************************************************************/
	/*	Printing  graph															  	 */
	/***********************************************************************************/
	int count_edges=0;

	for(iter_vert = pgraph->begin() ; iter_vert != pgraph->end(); iter_vert++)
	{
#if 1//DEBUG_FLAG
		cout<<iter_vert->vertex_ID<<")-->["<<iter_vert->edges.size()<<"] ";;
#endif
		count_edges += iter_vert->edges.size();
		for(iter_edge = iter_vert->edges.begin() ; iter_edge != iter_vert->edges.end(); iter_edge++)
		{
#if 1// DEBUG_FLAG

			cout<<"\t("<< iter_edge->dest_ID<<','<<iter_edge->weight<<")";
#endif
		}
#if 1//DEBUG_FLAG
		cout<<endl;
#endif
	}

	cout<<"Total edges "<<(count_edges>>1)<<endl;
	cout<<"missed edges = "<< (((VERTEX_COUNT * DENSE_DEGREE)-count_edges)>>1)<<endl;

}

int generate_graph(vector <vertex> *pgraph, uint degree)
{
	vector <vertex>::iterator iter_vert;
	vector <int> v_vector(VERTEX_COUNT);
	vector <int>:: iterator v_it;

	list<edge>::iterator iter_edge;

	int count = 0;
	for(iter_vert = pgraph->begin() ; iter_vert != pgraph->end(); iter_vert++)
	{
		iter_vert->vertex_ID = count;
		//		cout<<c<<iter_vert->vertex_ID<<endl;
		count++;
	}

#if 0//DEBUG_FLAG
	for(v_it = v_vector.begin() ; v_it != v_vector.end(); v_it++)
		cout<<*v_it<<endl;
#endif

	count = 0;
	//	for(iter_vert = pgraph->begin() ; iter_vert != pgraph->end(); iter_vert++)
	{
		edge E;
		vertex *p_dest;
		uint ID = 0;
		uint weight;

		iter_vert = pgraph->begin();
#if DEBUG_FLAG
		cout<<"For node----------------  "<<iter_vert->vertex_ID<<endl;
		//		iter_vert->edges;
#endif
		v_vector.clear();
		for (int i=0; i<VERTEX_COUNT; i++) v_vector.push_back(i);
		while (v_vector.size() > 1)
		{
			int randn = (((long long)rand() * v_vector.size())>>31);
			ID = v_vector.at(randn);

#if DEBUG_FLAG
			cout<<randn<<"-->For loop of destn = "<<ID<<endl;
			cout<<"size of RAND array is "<<v_vector.size()<<endl;
#endif
			p_dest = &pgraph->at(ID);

			/* check if destination edge has 6 nodes already */
			/* check if destination is not same as source 	*/
			if((p_dest->edges.size() >= 1))
			{
#if DEBUG_FLAG
				cout<<"destination degree reached "<<p_dest->edges.size()<<" for ID, "<<ID<<endl;
#endif

				remove_number_at(&v_vector,randn);
				continue;

			}
			if((ID == iter_vert->vertex_ID))
			{
#if DEBUG_FLAG

				cout<<"ID == iter_vert->vertex_ID"<<ID<<endl;
#endif
				remove_number_at(&v_vector,randn);

				continue;

			}
			else
			{
				int number  = rand();
				weight = (number - ((number>>14)<<14))+1; /* as max weight is set to 2^14 */

				E.dest_ID = ID;
				E.weight = weight;
#if DEBUG_FLAG
				cout<<E.dest_ID<<" ";
#endif
				iter_vert->edges.push_back(E); /* updated current list */

				E.dest_ID = iter_vert->vertex_ID;
#if DEBUG_FLAG
				cout<<E.weight <<"  "<<iter_vert->vertex_ID<<endl;
				cout<<"next verex__________ "<<ID<<endl;

#endif
				p_dest->edges.push_back(E);
				iter_vert = pgraph->begin()+ ID;
				count++;

			}

		}

		E.dest_ID = 0;
		E.weight = (rand()% MAX_EDGE_WEIGHT+1);

#if DEBUG_FLAG
		cout<<E.dest_ID<<" ";
#endif
		iter_vert->edges.push_back(E); /* updated current list */
#if DEBUG_FLAG
		cout<<E.dest_ID<<" ";
#endif

		E.dest_ID = iter_vert->vertex_ID;
#if DEBUG_FLAG
		cout<<E.weight <<"  "<<iter_vert->vertex_ID<<endl;
#endif
		iter_vert = pgraph->begin();
		iter_vert->edges.push_back(E); /* updated current list */
		count++;


#if DEBUG_FLAG

		cout<<" done for __________________ "<<iter_vert->vertex_ID<<endl;
#endif
		v_vector.clear();
	}



#if DEBUG_FLAG
	cout<<endl;
	print_graph(pgraph);
#endif
	/**********************************************************************************************/
	/**********************************************************************************************/
	/**********************************************************************************************/
	/**********************************************************************************************/

	//	exit(1);
	for(iter_vert = pgraph->begin() ; iter_vert != pgraph->end(); iter_vert++)
	{
		edge E;
		vertex *p_dest;
		uint ID = 0;
		uint weight;
#if DEBUG_FLAG
		cout<<"For node----------------  "<<iter_vert->vertex_ID<<endl;
		//		iter_vert->edges;
#endif
		v_vector.clear();
		for (int i=0; i<VERTEX_COUNT; i++) v_vector.push_back(i);
		for(iter_edge = iter_vert->edges.begin(); iter_edge != iter_vert->edges.end(); iter_edge++)
		{
			if(remove_number(&v_vector,iter_edge->dest_ID)<0)
				{
				cout<<"removing non existent edge ============================="<<endl;
				}
		}
		while (iter_vert->edges.size()< degree)
		{
			//				bool duplicate_found = 0;
			//				int randn = ((double)rand()/((double)RAND_MAX+1))* v_vector.size();

			int randn = (((long long)rand() * v_vector.size())>>31) ;

			if(v_vector.empty())
			{
				//				print_graph(pgraph);
				break;
			}
			ID = v_vector.at(randn);

#if DEBUG_FLAG
			cout<<randn<<"-->For loop of destn = "<<ID<<endl;
			cout<<"size of RAND array is "<<v_vector.size()<<endl;
#endif
			p_dest = &pgraph->at(ID);

			/* check if destination edge has 6 nodes already */
			/* check if destination is not same as source 	*/
			if((p_dest->edges.size() >= degree))
			{
#if DEBUG_FLAG
				cout<<"destination degree reached "<<p_dest->edges.size()<<" for ID, "<<ID<<endl;
#endif

				remove_number_at(&v_vector,randn);
				continue;

			}
			if((ID == iter_vert->vertex_ID))
			{
#if DEBUG_FLAG

				cout<<"ID == iter_vert->vertex_ID"<<ID<<endl;
#endif
				remove_number_at(&v_vector,randn);

				continue;

			}
			else
			{
				int number = rand();
				weight = (number - ((number>>14)<<14))+1; /* as max weight is set to 2^14 */
				//					weight = (rand()% MAX_EDGE_WEIGHT+1);

				E.dest_ID = ID;
				E.weight = weight;
#if DEBUG_FLAG
				cout<<E.dest_ID<<" ";
#endif
				iter_vert->edges.push_back(E); /* updated current list */

				E.dest_ID = iter_vert->vertex_ID;
#if DEBUG_FLAG
				cout<<E.weight <<"  "<<iter_vert->vertex_ID<<endl;
#endif
				p_dest->edges.push_back(E);
				count++;

			}

		}
#if DEBUG_FLAG

		cout<<" done for __________________ "<<iter_vert->vertex_ID<<endl;
#endif
		v_vector.clear();
	}



#if  GRAPH_PRINT_FLAG
	cout<<endl;

	print_graph(pgraph);
#endif
	cout<< "TOTAL number of edges = \t"<<count<<endl;
	return 0;
}
#if 1

int generate_den_graph(vector <vertex> *pgraph, uint degree)
{
	vector <vertex>::iterator iter_vert;
	vector <int> v_vector(VERTEX_COUNT);
	vector <int>:: iterator v_it;

	list<edge>::iterator iter_edge;

	int count = 0;
	for(iter_vert = pgraph->begin() ; iter_vert != pgraph->end(); iter_vert++)
	{
		iter_vert->vertex_ID = count;
		//		cout<<c<<iter_vert->vertex_ID<<endl;
		count++;
	}

	v_vector.clear();
	for (int i=0; i<VERTEX_COUNT; i++) v_vector.push_back(i);
	//	srand(time(NULL));

#if 0//DEBUG_FLAG
	for(v_it = v_vector.begin() ; v_it != v_vector.end(); v_it++)
		cout<<*v_it<<endl;
#endif

	count = 0;
	//	for(iter_vert = pgraph->begin() ; iter_vert != pgraph->end(); iter_vert++)
	{
		edge E;
		vertex *p_dest;
		uint ID = 0;
		uint weight;

		iter_vert = pgraph->begin();
#if DEBUG_FLAG
		cout<<"For node----------------  "<<iter_vert->vertex_ID<<endl;
		//		iter_vert->edges;
#endif
		v_vector.clear();
		for (int i=0; i<VERTEX_COUNT; i++) v_vector.push_back(i);
		while (v_vector.size() > 1)
		{
			int randn = (((long long)rand() * v_vector.size())>>31);
			ID = v_vector.at(randn);

#if DEBUG_FLAG
			cout<<randn<<"-->For loop of destn = "<<ID<<endl;
			cout<<"size of RAND array is "<<v_vector.size()<<endl;
#endif
			p_dest = &pgraph->at(ID);

			/* check if destination edge has 6 nodes already */
			/* check if destination is not same as source 	*/
			if((p_dest->edges.size() >= 1))
			{
#if DEBUG_FLAG
				cout<<"destination degree reached "<<p_dest->edges.size()<<" for ID, "<<ID<<endl;
#endif

				remove_number_at(&v_vector,randn);
				continue;

			}
			if((ID == iter_vert->vertex_ID))
			{
#if DEBUG_FLAG

				cout<<"ID == iter_vert->vertex_ID"<<ID<<endl;
#endif
				remove_number_at(&v_vector,randn);

				continue;

			}
			else
			{

				int number  = rand();
				weight = (number - ((number>>14)<<14))+1; /* as max weight is set to 2^14 */

				E.dest_ID = ID;
				E.weight = weight;
				remove_number_at(&v_vector,randn);

#if DEBUG_FLAG
				cout<<E.dest_ID<<" ";
#endif
				iter_vert->edges.push_back(E); /* updated current list */

				E.dest_ID = iter_vert->vertex_ID;
#if DEBUG_FLAG
				cout<<E.weight <<"  "<<iter_vert->vertex_ID<<endl;
				cout<<"next verex__________ "<<ID<<endl;

#endif
				p_dest->edges.push_back(E);
				iter_vert = pgraph->begin()+ ID;
				count++;
			}

		}

		E.dest_ID = 0;
		E.weight = (rand()% MAX_EDGE_WEIGHT+1);

#if DEBUG_FLAG
		cout<<E.dest_ID<<" ";
#endif
		iter_vert->edges.push_back(E); /* updated current list */
#if DEBUG_FLAG
		cout<<E.dest_ID<<" ";
#endif

		E.dest_ID = iter_vert->vertex_ID;
#if DEBUG_FLAG
		cout<<E.weight <<"  "<<iter_vert->vertex_ID<<endl;
#endif
		iter_vert = pgraph->begin();
		iter_vert->edges.push_back(E); /* updated current list */

		count++;

#if DEBUG_FLAG

		cout<<" done for __________________ "<<iter_vert->vertex_ID<<endl;
#endif
		v_vector.clear();
	}



#if DEBUG_FLAG
	cout<<endl;
	print_graph(pgraph);
#endif
	/**********************************************************************************************/
	/**********************************************************************************************/
	/**********************************************************************************************/
	/**********************************************************************************************/
	for(iter_vert = pgraph->begin() ; iter_vert != pgraph->end(); iter_vert++)
	{
		edge E;
		vertex *p_dest;
		uint ID = 0;
		uint weight;
#if DEBUG_FLAG
		cout<<"For node----------------  "<<iter_vert->vertex_ID<<endl;
		//		iter_vert->edges;
#endif
		v_vector.clear();
		for (int i=0; i<VERTEX_COUNT; i++) v_vector.push_back(i);
		for(iter_edge = iter_vert->edges.begin(); iter_edge != iter_vert->edges.end(); iter_edge++)
		{
			if(remove_number(&v_vector,iter_edge->dest_ID)<0)
			{
				cout<<"removing non existent edge ============================="<<endl;
			}
		}
		while (iter_vert->edges.size()< degree)
		{
			//				bool duplicate_found = 0;
			//				int randn = ((double)rand()/((double)RAND_MAX+1))* v_vector.size();

			int randn = (((long long)rand() * v_vector.size())>>31) ;

			if(v_vector.empty())
			{
				//				print_graph(pgraph);
				break;
			}
			ID = v_vector.at(randn);

#if DEBUG_FLAG
			cout<<randn<<"-->For loop of destn = "<<ID<<endl;
			cout<<"size of RAND array is "<<v_vector.size()<<endl;
#endif
			p_dest = &pgraph->at(ID);

			if((ID == iter_vert->vertex_ID))
			{
#if DEBUG_FLAG

				cout<<"ID == iter_vert->vertex_ID"<<ID<<endl;
#endif
				remove_number_at(&v_vector,randn);
				continue;
			}
			else
			{
				int number = rand();
				weight = (number - ((number>>14)<<14))+1; /* as max weight is set to 2^14 */
				//					weight = (rand()% MAX_EDGE_WEIGHT+1);

				remove_number_at(&v_vector,randn);
				E.dest_ID = ID;
				E.weight = weight;
#if DEBUG_FLAG
				cout<<E.dest_ID<<" ";
#endif
				iter_vert->edges.push_back(E); /* updated current list */

				E.dest_ID = iter_vert->vertex_ID;
#if DEBUG_FLAG
				cout<<E.weight <<"  "<<iter_vert->vertex_ID<<endl;
#endif
				p_dest->edges.push_back(E);
				count++;

			}
		}
#if DEBUG_FLAG

		cout<<" done for __________________ "<<iter_vert->vertex_ID<<endl;
#endif
		v_vector.clear();
	}



#if GRAPH_PRINT_FLAG
	cout<<endl;

	print_graph(pgraph);
#endif
	cout<< "TOTAL number of edges = \t"<<count<<endl;
	return 0;
}


int generate_dense_graph(vector <vertex> *pgraph, uint degree)
{
	vector <vertex>::iterator iter_vert;
	vector <int>:: iterator v_it;

	list<edge>::iterator iter_edge;

	int count = 0;
	for(iter_vert = pgraph->begin() ; iter_vert != pgraph->end(); iter_vert++)
	{
		iter_vert->vertex_ID = count;
		//		cout<<c<<iter_vert->vertex_ID<<endl;
		count++;
	}

	//	v_vector.clear();
	//	for (int i=0; i<VERTEX_COUNT; i++) v_vector.push_back(i);
	//	srand(time(NULL));

#if DEBUG_FLAG
	for(v_it = v_vector.begin() ; v_it != v_vector.end(); v_it++)
		cout<<*v_it<<endl;
#endif

	/**********************************************************************************************/
	/**********************************************************************************************/
	/**********************************************************************************************/
	/**********************************************************************************************/

	count = 0;
	for(iter_vert = pgraph->begin() ; iter_vert != pgraph->end(); iter_vert++)
	{
		edge E;
		vertex *p_dest;
		uint ID = 0;
		uint weight;
#if DEBUG_FLAG
		cout<<"For node----------------  "<<iter_vert->vertex_ID<<endl;
		//		iter_vert->edges;
#endif
		//		v_vector.clear();
		//		for (int i=0; i<VERTEX_COUNT; i++) v_vector.push_back(i);
		while (iter_vert->edges.size()< degree)
		{
			bool duplicate_found = 0;
			ID = (((long long)rand() * VERTEX_COUNT)>>31) ;

			//			int randn = ((double)rand()/((double)RAND_MAX+1))* v_vector.size();
			//
			//			if(v_vector.empty())
			//			{
			////								print_graph(pgraph);
			//				break;
			//			}
			//			ID = v_vector.at(randn);

#if DEBUG_FLAG
			cout<<randn<<"-->For loop of destn = "<<ID<<endl;
			cout<<"size of RAND array is "<<v_vector.size()<<endl;
#endif
			p_dest = &pgraph->at(ID);

			/* check if destination edge has 6 nodes already */
			/* check if destination is not same as source 	*/
			if((p_dest->edges.size() >= degree))
			{
#if DEBUG_FLAG
				cout<<"destination degree reached "<<p_dest->edges.size()<<" for ID, "<<ID<<endl;
#endif

				//				remove_number_at(&v_vector,randn);
				continue;

			}
			if((ID == iter_vert->vertex_ID))
			{
#if DEBUG_FLAG

				cout<<"ID == iter_vert->vertex_ID"<<ID<<endl;
#endif
				//				remove_number_at(&v_vector,randn);
				continue;

			}

			/* to check if the edge to that node already exists */
		for(iter_edge = iter_vert->edges.begin(); iter_edge != iter_vert->edges.end(); iter_edge++)
			{
				if(ID == iter_edge->dest_ID)
				{
#if DEBUG_FLAG
					cout<<"found duplicate--> "<<ID<<endl;
#endif
					duplicate_found = 1;
					break; /* only this for loop */
				}
			}

			if(duplicate_found)
			{
#if DEBUG_FLAG
				cout<<"Skipping for "<<ID<<endl;
#endif
				//				remove_number_at(&v_vector,randn);
				continue;
			}
			else
			{
				int number = rand();
				weight = (number - ((number>>14)<<14))+1; /* as max weight is set to 2^14 */

				//				weight = (rand()% MAX_EDGE_WEIGHT+1);

				E.dest_ID = ID;
				E.weight = weight;
#if DEBUG_FLAG
				cout<<E.dest_ID<<" ";
#endif
				iter_vert->edges.push_back(E); /* updated current list */

				E.dest_ID = iter_vert->vertex_ID;
#if DEBUG_FLAG
				cout<<E.weight <<"  "<<iter_vert->vertex_ID<<endl;
#endif
				p_dest->edges.push_back(E);
			}

		}
#if DEBUG_FLAG

		cout<<" done for __________________ "<<iter_vert->vertex_ID<<endl;
#endif
		//		v_vector.clear();
	}



	//	cout<<endl;

	//		print_graph(pgraph);
	return 0;
}
#endif

#endif /* GRAPH_H_ */
