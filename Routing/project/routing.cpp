//============================================================================
// Name        : Tharun Battula
// Author      : 
// Version     :
// Copyright   :
// Description : Routing in C, Ansi-style
//============================================================================
#include "max_heap.h"
#include "graph.h"
#include "algo.h"
#include <time.h>
#include <ctime>
#include <sys/time.h>

int main()
{
	/***********************************************************************************/
	/*	Generating Graph															   */
	/***********************************************************************************/
	int i = 0;
//	for ( i = 0; i < 1000; ++i)
	{
		struct timeval t1;
		gettimeofday(&t1, NULL);

		uint t = t1.tv_usec * t1.tv_sec;
//		t = 2320489724;
		srand(t);
		cout<<"Seed used=\t"<<t<<endl;

		clock_t startTime = clock();
		clock_t endTime = clock();
		clock_t clockTicksTaken;
		double timeInSeconds1;
		double timeInSeconds2;
		double timeInSeconds3;

		for ( uint iter_count = 0; iter_count < 10; ++iter_count)
		{
			vector<vertex> *pgraph = new vector<vertex> (VERTEX_COUNT) ;
			cout<<endl<<endl;
//			pgraph = psparse_graph_1;
			cout<<"/********************************************************************************/"<<endl;
			cout<<"/***************************\t\tGRAPH "<<iter_count<<"\t\t*************************/"<<endl;
			cout<<"/********************************************************************************/"<<endl;

			startTime = clock();

			if(iter_count<5)
			{
				generate_graph(pgraph, SPARSE_DEGREE);
			}
			else
			{
				generate_den_graph(pgraph, DENSE_DEGREE);
			}

			endTime = clock();
			clockTicksTaken = endTime - startTime;
			timeInSeconds1 = clockTicksTaken / (double) CLOCKS_PER_SEC;
			cout << "For Graph generation\t" << timeInSeconds1 << " seconds."<<endl;

			for (int rand_iter = 0; rand_iter < 5; ++rand_iter)
			{
				int src_id = rand()%VERTEX_COUNT;
				int term_id;
				do
				{
					term_id = rand()%VERTEX_COUNT;
				} while (term_id == src_id);

				cout<<endl<<"=========\tCASE "<<rand_iter<<"\t========="<<endl<<endl;
				cout<<"Finding max capacity path\tfrom "<<src_id<<"-------to--------"<<term_id;
				cout<<"\t, printing nodes in reverse order"<<endl;

				cout<<endl<<"ALGORITHM : Dijkstra's without Heap"<<endl;
				startTime = clock();
				int ret_value1 = 	MCP_dijktras_without_heap(pgraph,src_id,term_id);

				endTime = clock();
				clockTicksTaken = endTime - startTime;
				timeInSeconds1 = clockTicksTaken / (double) CLOCKS_PER_SEC;

				//		cout<<"without heap success"<<endl;
				if(ret_value1 <0)
				{
					cout<<"something went wrong"<<endl;
				}

				cout<<endl<<"ALGORITHM : Dijkstra's using Heap"<<endl;
				startTime = clock();
				int ret_value2 = 	MCP_dijktras_with_heap(pgraph,src_id,term_id);


				endTime = clock();
				clockTicksTaken = endTime - startTime;
				timeInSeconds2 = clockTicksTaken / (double) CLOCKS_PER_SEC;

				if(ret_value2 <0)
				{
					cout<<"something went wrong"<<endl;
				}
				//
				//		cout<<"_____________________________"<<endl<<"The max capacity WITHOUT heap = "<< ret_value1<<endl;
				//		cout<<"_____________________________"<<endl<<"The max capacity using heap = "<< ret_value2<<endl;

				//	exit(1);

				cout<<endl<<"ALGORITHM : Kruskal Max Spanning Tree "<<endl;
				startTime = clock();
				int ret_value3 = MCP_kruskal(pgraph,src_id,term_id);

				endTime = clock();
				clockTicksTaken = endTime - startTime;
				timeInSeconds3 = clockTicksTaken / (double) CLOCKS_PER_SEC;
				cout<<"Summary :"<<endl;
				cout << "Dijksra' W/out heap \t took " << timeInSeconds1 << " seconds."<<"\t&Found max capacity =\t"<<ret_value1<<endl;
				cout << "Dijksra' WITH heap \t took " << timeInSeconds2 << " seconds."<<"\t&Found max capacity =\t"<<ret_value2<<endl;
				cout << "Kruskal  MST algo \t took " << timeInSeconds3<< " seconds."<<"\t&Found max capacity =\t"<<ret_value3<<endl;

				if((ret_value1 != ret_value2) || (ret_value1 != ret_value3))
				{
					cout<<i<<" ";
					cout<<endl<<"NOT matching "<<i<<endl;
					cout<<"value 1 = "<<ret_value1<<endl;
					cout<<"value 2 = "<<ret_value2<<endl;
					cout<<"value 3 = "<<ret_value3<<endl;
				}

			}
			delete pgraph;
//			exit(0);
		}
	}
    cout<<"Reached End of Code Successfully "<<endl;

return 0;
}


