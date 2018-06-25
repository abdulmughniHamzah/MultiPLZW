#include "lookupTable.h"
/*void
LookupTable::addToFSQueue(Node *node){
		if(fsQueue==NULL){
			fsQueue=new QueueEntry(node,NULL, NULL);
		}else{
			fsQueue=new QueueEntry(node,NULL, fsQueue);
		}
}
*/
LookupTable::LookupTable(LZW_AGST* afsm, int size){
		entry=new LookupTableEntry*[size];
		this->size=size;
		for(int i=0;i<LetterSetSize;i++){

				setEntry(0, uchar(i),uchar(i),uchar(i),afsm->root,false);
		}
}
HistoryTreeNode*
HistoryTree::addNode(LookupTableEntry * lookupEntryRef, HistoryTreeNode* ancestor){
	return new HistoryTreeNode(lookupEntryRef,ancestor);

}
void
LookupTable::setEntry(int size, uchar fchr, uchar lchr,int index,Node *state,bool f){
	entry[index]=new LookupTableEntry();
	//entry[index]->sequence=chunk;
	entry[index]->fchr=fchr;
	entry[index]->lchr=lchr;
	entry[index]->seqSize=size;
	entry[index]->state=state;
	entry[index]->f=f;

}
void
LookupTable::insertEntry(LookupTableEntry* nEntry,int indx){
	/*if(indx>=size){
		cout<<"error in table size: indx>=size "<<endl;
		exit(0);
	}*/
	entry[indx]=nEntry;

}
