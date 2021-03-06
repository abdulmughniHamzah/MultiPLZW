
//LZW dictionary trie
#include <iostream>
#include <iomanip>      // std::setw
using namespace std;
#include <string.h>
#include <stdio.h>
#include<cmath>
#include <fstream>
#define   LETTER_SET_SIZE 256
typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;
struct Node{
	public:
	Node *child[LETTER_SET_SIZE];
	Node* parent;
	uchar edge;
	Node *failureDest;
	int pattID;
	int ID;// index of the corresponding entry in the dictionary
	bool prefix; // 1 if full  pattern
	bool suffix;// final state in a fullpattern
	bool finalState;
	void print(){
		//cout<<setw(10)<<"ID: "<<ID<<endl;

		//cout<<setw(10)<<"parent: "<<parent->ID<<endl;
		//cout<<setw(10)<<"edge: "<<edge<<endl;
		//cout<<setw(10)<<"final-state: "<<finalState<<endl;
		//cout<<setw(10)<<"suffix?: "<<suffix<<endl;
		//cout<<setw(10)<<"Prefix?: "<<prefix<<endl;
		//cout<<setw(10)<<"failureDest: "<<failureDest->getID()<<endl;
		//cout<<setw(10)<<"pattIDsSize: "<<pattIDsSize<<endl;
		/*cout<<setw(10)<<"pattIDs: ";
		for(int p=0;p<pattIDsSize;p++){
			cout<<setw(4)<<pattIDs[p];
		}
		cout<<endl;*/
	}
	bool isPrefix(){return prefix;}
	void setParameters(int id,uchar edge1, bool the_prefix,bool the_suffix,bool isFinalState){
		ID=id;
		edge=edge1;
		prefix=the_prefix;
		suffix=the_suffix;
		finalState=isFinalState;

	}

	void setParent(Node * node){parent=node;}
	Node* getParent(){return parent;}
	void setFailure(Node* theNode){failureDest=theNode;}
	Node* getFailure(){return failureDest;}
	void setEdge(uchar theEdge){edge=theEdge;}
	uchar getEdge(){return edge;}
	void setID(int id){ID=id;}
	int getID(){return ID;}
	void setPrefixFlag(bool flg){prefix=flg;}
	bool getPrefixFlag(){return prefix;}

	Node* getChild(int indx){

		return child[indx];
	}
	Node* getChild(int indx,bool go_failed){

		if(!go_failed){

			return child[indx];
		}else{

			if(child[indx]!=NULL){
				if((child[indx]->prefix)==true)
					return child[indx];
				else
					return NULL;
			}else
				return NULL;

		}

	}
	void setChild(Node* theNode,int index){
		child[index]=theNode;
		theNode->parent=this;
	}
	//////////////////////////////////////////////////////////////
	Node(){//number of patterns
		suffix=0;
		//memset(pList,-1,sizeof (*pList)*nofp);
	}
~Node(){
	cout<<"~NODE"<<endl;
}


};

class LZW_AGST{



	public:
	//bool** snippet;
	int** mapTable;
	Node* root;
	Node **nodesList;
	int nn;//number of the last added node(work as a counter);
	int np;//number of patterns
	int pr_n;//number of prefixes' nodes
	int suff_pr_n;//number of suffixes' prefixes' nodes
	LZW_AGST();
	void create(string fname);
	void addSequence(string patt,int pid, int len,bool prefix);
	void addSuffixes(string patt, int len,int pid);
	int getLastVisited(string& patt,int beg,int end);
	int* getTraversed(string& patt, int beg,int plen);
	void makeFailureLinks();
	void createMapTable( string& patt ,int plen);

};
class QueueEntry{
	public:
	Node * node;
	QueueEntry* next;
	QueueEntry* prev;

	QueueEntry(Node * node,QueueEntry * prev, QueueEntry* next){
		this->node=node;
		this->next=next;
		this->prev=prev;
	}

	~QueueEntry(){
	node=NULL;
	next=NULL;
	prev=NULL;
	}
};
class Queue{
	public:
	QueueEntry* head;
	QueueEntry* tail;

	Queue(){
		head=tail=NULL;
	}
	void enqueue(Node* node);
	Node *dequeue();

};
