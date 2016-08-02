/*
 * max_heap.h
 *
 *  Created on: Nov 16, 2014
 *      Author: Tharun Battula
 */

#ifndef MAX_HEAP_H_
#define MAX_HEAP_H_


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <iostream>
#include <iterator>
#include <time.h>
#include <vector>

#define HEAP_FLAG 0

using namespace std;

typedef struct edge_t
{
	uint src_ID ; // Not used in dijkstra's but used in kruskal
	uint dest_ID;
	uint weight;
} edge;

class Heap {
public:
    Heap();
    ~Heap();
    void insert(edge element);
    edge extract_max();
    void print();
    int size() { return heap_vector.size();};
    void delete_index(uint index);
    int update_key(uint ID, uint new_value);
    int heap_sort();
private:

    int left(int parent);
    int right(int parent);
    int parent(int child);
    void sift_up(int index);
    void sift_down(int index);
private:
    vector<edge> heap_vector;
//    vector<edge> *ptr_heap;
};

Heap::Heap()
{
}

Heap::~Heap()
{
}

void Heap::insert(edge element)
{

    heap_vector.push_back(element);
    sift_up(heap_vector.size() - 1);
#if HEAP_FLAG
	 cout<< "____ inserted Key ___"<<endl;
    this->print();
#endif
}

edge Heap::extract_max()
{

    edge edge_max = heap_vector.front();
    heap_vector.front() = heap_vector.back();
    heap_vector.pop_back();

    this->sift_down(0);
//    cout<<"siftdowncomplete"<<endl;
#if HEAP_FLAG
	 cout<< "____ extracting max ___"<<endl;
    this->print();
#endif
    return edge_max;

}

void Heap::print()
{
#if HEAP_FLAG
    vector<edge>::iterator pos = heap_vector.begin();
    cout << "[ID, capacity]"<<endl;
    while ( pos != heap_vector.end() ) {
        cout <<"["<<pos->dest_ID <<", "<<pos->weight<<"]\t";
        ++pos;
    }
    cout << endl;
#endif
}

void Heap::delete_index(uint index)
{

	heap_vector[index] = heap_vector.back();
	 heap_vector.pop_back();
	 /* corner leaf case */
	 if(index == (heap_vector.size()))
	 {
		 return;
	 }
	 /* corner root case */
	 int pt = parent(index);
	 if((pt < 0) || (heap_vector[pt].weight > heap_vector[index].weight))
	 {
		 sift_down(index);
	 }
	 else
	 {
		 sift_up(index);
	 }
	 return;
}

int Heap::update_key(uint ID, uint new_value)
{
	vector <edge> :: iterator it;
	if(heap_vector.empty()) return -1;
#if HEAP_FLAG
    this->print();
	 cout<< "____ updating Key ___"<<endl;

#endif

	for (it = heap_vector.begin(); it != heap_vector.end(); ++it)
	{
		if(ID == it->dest_ID)
		{
//			this->heapifydown((it-heap.begin()));
			this->delete_index((it-heap_vector.begin()));

			this->print();
			edge e;
		    e.dest_ID = ID;
		    e.weight = new_value;
		    this->insert(e);


			return 0;
		}
	}
#if HEAP_FLAG
    this->print();
#endif

	cout<<"Something wrong!! couldn't find ID in the list"<<endl;
	return -1;
}

/* not used */
//int Heap::heap_sort()
//{
////	vector <edge> temp;
//	vector <edge> :: iterator v_it;
//	ptr_heap = &heap_vector;
//
//	for (int i = 0; i < this->size(); ++i)
//	{
//		this->sift_down(i);
//	}
//	while(temp.size()>0)
//	{
//		*ptr_heap->sift_down(0);
//		temp.assign(temp.begin()+1,temp.end());
//	}
//
//	return EXIT_SUCCESS;
//}

void Heap::sift_up(int index)
{
#if HEAP_FLAG
    cout << "index=" << index << endl;
    cout << "parent(index)=" << parent(index) << endl;
    cout << "heap[parent(index)]=" << heap_vector[parent(index)].weight << endl;
    cout << "heap[index]=" << heap_vector.at(index).weight << endl;
#endif

    while ((index > 0 ) && ( parent(index) >= 0 ) &&
            (heap_vector[parent(index)].weight < heap_vector[index].weight))
    {
        edge tmp = heap_vector[parent(index)];
        heap_vector[parent(index)] = heap_vector[index];
        heap_vector[index] = tmp;
        index = parent(index);
    }



}

void Heap::sift_down(int index)
{

    int left = this->left(index);
    int right = this->right(index);
    int largest = index;
#if HEAP_FLAG
    cout << "index=" << index << endl;
      cout << "left(index)=" <<left << endl;
      cout << "right(index)=" << right << endl;
      cout<<"size"<<this->size()<<endl;
#endif

    if (left > 0 && left < this->size() &&
    		heap_vector.at(left).weight > heap_vector.at(largest).weight )
    	largest = left;

    if (right > 0 &&right < this->size() &&
       		heap_vector.at(right).weight > heap_vector.at(largest).weight )
       	largest = right;

    if (largest != index)
    {
    	edge tmp = heap_vector[index];
    	heap_vector[index] = heap_vector[largest];
    	heap_vector[largest] = tmp;

    	sift_down(largest);
//    	cout<<"returned"<<endl;
    }
}

int Heap::left(int parent)
{
    int i = ( parent << 1 ) + 1; // 2 * parent + 1
    return ( i < (int)heap_vector.size() ) ? i : -1;
}

int Heap::right(int parent)
{
    int i = ( parent << 1 ) + 2; // 2 * parent + 2
    return ( i < (int)heap_vector.size() ) ? i : -1;
}

int Heap::parent(int child)
{
    if (child != 0)
    {
        int i = (child - 1) >> 1;
        return i;
    }
    return -1;
}




#endif /* MAX_HEAP_H_ */
