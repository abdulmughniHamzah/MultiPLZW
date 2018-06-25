//#define LetterSet 256

#include "LZW_AGST.h"


LZW_AGST::LZW_AGST(){





}

//create the proposed automated Finite state mechine from the given file of patterns
void
LZW_AGST::create(string fname){
	string line;
	ifstream file(fname);



	getline(file, line);
	np=stoi(line);
	string * patts=new string[np];
	root=new Node();
	root->setParameters(0,0,  1,0,0);
	nn=1;
	//////////////////
	getline(file, line);
	int pid=-1;
	while (getline(file, line)) {
		pid++;
        addSequence(line,pid, line.length(),true);
        //addSequence(string patt,int pid, int len,bool fp,int seqIndx)
		patts[pid]=line;


	}
	pr_n=nn;
	//cout<<"--pr_n="<<pr_n<<endl;
	for(int k=0;k<=pid;k++){
		addSuffixes(patts[k], patts[k].length(),k);
	}
	suff_pr_n=nn-pr_n-1;
	//cout<<"--suff_pr_n="<<suff_pr_n<<endl;
	makeFailureLinks();
	//following to make successiveness table
	mapTable=new int*[pr_n];
	nodesList=new Node*[pr_n];
	nodesList[0]=root;
	for(int i=0;i<pr_n;i++){
		mapTable[i]=new int[nn];
	}
	for(int i=0;i<np;i++){
		createMapTable(patts[i] ,patts[i].length());
	}
	//ust print
	/*for(int i=0;i<pr_n;i++){
		cout<<setw(4)<<nodesList[i]->ID;

		cout<<endl;
	}
	cout<<setw(4)<<"Node";
	for(int i=0;i<nn;i++){
		cout<<setw(4)<<i;
	}
	cout<<endl;
	for(int i=0;i<pr_n;i++){
		cout<<setw(4)<<i;
		for(int y=0;y<nn;y++){
			cout<<setw(4)<<mapTable[i][y];
		}
		cout<<endl;
	}*/

}
void
LZW_AGST::createMapTable( string& patt ,int plen){
	Node *tmp3,*tmp2,*tmp1=root;
	int k,i=0;
	while(i<plen){
		tmp1=tmp1->child[patt[i]];
		tmp2=root;
		k=i+1;
		nodesList[tmp1->ID]=tmp1;
		tmp3=tmp1;
		while(k<plen){
			tmp2=tmp2->child[patt[k]];
			tmp3=tmp3->child[patt[k]];
			mapTable[tmp1->ID][tmp2->ID]=tmp3->ID;
			//fill first row in mapTable symmetrically...
			//mapTable[root->ID][tmp2->ID]=tmp2->ID;


			k++;
		}
		//fill first row in mapTable symmetrically...
		mapTable[root->ID][tmp1->ID]=tmp1->ID;
		i++;
	}
}
void
LZW_AGST::addSuffixes(string patt, int len,int pid){


	for(int i=1;i<len;i++){

		addSequence(patt.substr(i,len-i),pid, len-i,false);

	}
}
void
LZW_AGST::addSequence(string patt,int pid, int len,bool prefix){

	Node *tmp=root;
	for(int i=0; i<len;i++){
		if(tmp->getChild(patt[i])==NULL){
			Node *node=new Node();
			node->setEdge(patt[i]);
			node->setID(nn++);
			node->prefix=prefix;
			node->suffix=false;
			node->finalState=false;
			tmp->setChild(node,patt[i]);

		}
		tmp=tmp->getChild(patt[i]);
		tmp->prefix=tmp->prefix||prefix;

		if(i==(len-1)){
			tmp->suffix=true;
			if(prefix){
				tmp->finalState=true;
				tmp->pattID=pid;
			}
		}


	}

}


void
LZW_AGST::makeFailureLinks(){
	root->setFailure(root);
	Queue *queue=new Queue();
	for(int i=0;i<LetterSetSize;i++){
		if(root->getChild(i)!=NULL)
			queue->enqueue(root->getChild(i));
	}

	Node *node,*failure,*parent;
	int cndCntr=0;

	node=queue->dequeue();
	while(node!=NULL){



		for(int i=0;i<LetterSetSize;i++){
			if(node->getChild(i)!=NULL)
				queue->enqueue(node->getChild(i));
		}
		parent=node->getParent();




		if(parent!=root){
			cndCntr=0;
			failure=parent->getFailure()->getChild(node->getEdge());
			if(failure!=NULL){//
				cndCntr++;
				if(failure->isPrefix())
					cndCntr++;
			}
			if(cndCntr==2){
				node->setFailure(failure);
			}else{
				cndCntr=0;
				failure=root->getChild(node->getEdge());
				if(failure!=NULL){//
					cndCntr++;
					if(failure->isPrefix())
						cndCntr++;
				}
				if(cndCntr==2){
					node->setFailure(failure);
				}else{
					node->setFailure(root);
				}
			}

		}else{
			node->setFailure(root);
		}

		//node->print();
		node=queue->dequeue();
	}


}

int
LZW_AGST::getLastVisited(string &patt,int beg,int end){

	Node* tmp=root;
	for(int i=beg;i<=end;i++){
		tmp=tmp->getChild((int)patt[i]);

	}

	return tmp->getID();
}
int *
LZW_AGST::getTraversed(string & patt, int beg,int plen){
	Node *tmp=root;

	int * arr=new int[plen];
	for (int i=beg;i<(beg+plen);i++){
		tmp=tmp->getChild(patt[i]);

		arr[i-beg]=tmp->getID();

	}
	return arr;

}
void
Queue::enqueue(Node* node){
	if(tail==NULL){
		tail=head=new QueueEntry(node,NULL,NULL);
	}else{
		tail=new QueueEntry(node,NULL,tail);
		tail->next->prev=tail;
	}
}
Node*
Queue::dequeue(){

	if(head==NULL){

		return NULL;
	}
	Node* node=head->node;

	if(tail==head){
		tail=head=NULL;
	}else{
		head=head->prev;
		head->next=NULL;




	}

	return node;
}
