/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name:
 * Email:
 */

#include <stdlib.h>

#include "pq.h"
#include "dynarray.h"

/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 */
struct pq{
  struct dynarray * arr;
};

struct node{
  void* val;
  int priority;
};

/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {
    struct pq* p=malloc(sizeof(struct pq));
    p->arr=dynarray_create();
    return p;
}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue.  That is the responsibility of the caller.
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
  //free(pq->arr);
  dynarray_free(pq->arr);
  free(pq);
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
  //return (pq->arr==NULL);
  return (dynarray_size(pq->arr)==0);
}


/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 */
void pq_insert(struct pq* pq, void* value, int priority) {
  struct node * n=malloc(sizeof(struct node)); //DON'T FORGET TO FREE n!!!!
  int index= dynarray_size(pq->arr); //index to insert into
  n->val=value;
  n->priority=priority;
  dynarray_insert(pq->arr, -1, n); //insert into last index

  heapify(pq->arr, index); 

}

void heapify(struct dynarray* arr, int index){
  if(index!=0) {  
    struct node* newer=dynarray_get(arr, index); //node of newest insert
    int newpri= newer->priority; //priority of new node
    if(index % 2 == 0 ){  //means it's a right child
      
      struct node* switcher=dynarray_get(arr, (index-2)/2); //node of parent
      int switchpri=switcher->priority; //priority of parent
      if(newpri<switchpri){
        swap(dynarray_get(arr, index), dynarray_get(arr, (index-2)/2) );
        index=(index-2)/2; //reset index of sifter
        heapify(arr, index); //continue to heapify
      }
    }
    else if(index % 2 == 1 ){ //means it's a left child
     
      struct node* switcher=dynarray_get(arr, (index-1)/2);
      int switchpri=switcher->priority;
      if(newpri<switchpri){
        swap(dynarray_get(arr, index), dynarray_get(arr, (index-1)/2) );
        index=(index-1)/2;
        heapify(arr, index);
      }
    }
  }


}



void swap(struct node * newest, struct node * switcher){
  struct node temp = *switcher;
  *switcher=*newest;
  *newest=temp;
}

/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_first(struct pq* pq) {
  struct node *n=dynarray_get(pq->arr, 0);
  void * value= n->val;
  return value;
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) {
    struct node *n=dynarray_get(pq->arr, 0);
    int p= n->priority;
  
  return p;
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_remove_first(struct pq* pq) {
  if(dynarray_size(pq->arr)>0){
    int lastidx=dynarray_size(pq->arr) -1; //get index of node to switch with min
    struct node * first = dynarray_get(pq->arr, 0); //get first node
    void * val= first->val; //get value carried by first node
    swap(dynarray_get(pq->arr, 0), dynarray_get(pq->arr, lastidx)); //swap first with last
    free(dynarray_get(pq->arr, lastidx)); //free the node
    dynarray_remove(pq->arr, lastidx);  //delete the last (highest pri)
     //heapifydown 
     if(dynarray_size(pq->arr)>0)
      heapifydown(pq->arr, 0);

    return val;
  }
  


  
  
}


void heapifydown(struct dynarray * arr, int index){
  int leftchildidx, rightchildidx, minidx; 
  struct node* sifter=dynarray_get(arr, index); 
  leftchildidx=(index*2)+1;   //index of left child (if it exists)
  rightchildidx=(index*2)+2; //index of right child (if it exists)
  if(rightchildidx >= dynarray_size(arr)){   //check if there is a right child
    if(leftchildidx >= dynarray_size(arr)){ //check if there is a left child
      return; //since there's neither, just quit
    }
    else{ // there's not right child, but there's a left child
      minidx=leftchildidx; //make the index to compare to the left child index
    }
  }
  
  else{ //there's both a left and right child
    struct node* leftchild=dynarray_get(arr, (index*2)+1); 
    struct node* rightchild=dynarray_get(arr, (index*2)+2);

    if(leftchild->priority <= rightchild->priority){ //compare left and right child priorities
      minidx=leftchildidx; //left pri is smaller, make it the index to compare to
    }
    else{ //right index pri is smaller
      minidx=rightchildidx;  //make it the index to compare to
    }
  }
  struct node* minchild = dynarray_get(arr, minidx); //
  if(sifter->priority > minchild->priority){ //compare pri of parent and child nodes
    swap(dynarray_get(arr, index), dynarray_get(arr, minidx)); //swap if parent is bigger than child pri
    index=minidx; //set index of node to siftdown to the index it swapped with
    heapifydown(arr, index); //continue heapifying
  }
  
}

