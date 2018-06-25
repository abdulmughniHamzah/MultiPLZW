#include "LZW_AGST.cpp"
class LookupTableEntry;

class HistoryTreeNode{//lookupTable queue
	public:
	LookupTableEntry * lookupTableEntry;
	HistoryTreeNode* ancestor;

	HistoryTreeNode(LookupTableEntry * newState/*,QueueEntry * prev*/, HistoryTreeNode* ancestor){
		this->lookupTableEntry=newState;
		this->ancestor=ancestor;
	}
};
class HistoryTree{
	public:
		HistoryTreeNode*	addNode(LookupTableEntry * lookupEntryRef, HistoryTreeNode* ancestor);
};
class LookupTableEntry{
	public:
	HistoryTreeNode *anchor;//queue head
	Node      *state;
	//uchar      *sequence;
	uchar fchr;
	uchar lchr;
	int        seqSize;
	bool f ;//go_failed
	LookupTableEntry(){
		anchor=NULL;
		state=NULL;
		//sequence=NULL;
		//seqSize=0;
		//fchr=lchr='';
		//f=false;
		//chainStatus=ChainStatus::undefined;
	}
	~LookupTableEntry(){
		//delete []sequence;
	}

};
//the lookup table data structure difination
class LookupTable{
	public:
	LookupTableEntry **entry;
	int size;
	LookupTable(LZW_AGST* afsm, int size);
	void setEntry(/*uchar* chunk,int chnkSize,*/int size,uchar fchr,uchar lchr,int index,Node *state,bool f);
	void insertEntry(LookupTableEntry* nEntry,int indx);
	bool isGoFailed(int indx){
		return (entry[indx]->f);
	}
	Node * getState(int indx){
		return entry[indx]->state;
	}
	/*uchar* getEntryChunk(int index){

		return entry[index]->sequence;

	}*/
	int getEntryChunkSize(int index){

		return entry[index]->seqSize;

	}
	uchar getFirstLetterOf(int index1, int index2){
		if(entry[index1]!=NULL)
			return entry[index1]->fchr;
		else
			return entry[index2]->fchr;
	}
	uchar getFirstLetterOf(int index){
		return entry[index]->fchr;
	}
	uchar getLastLetterOf(int index){
		//if(entry[index]->sequence!=NULL)
			//return entry[index]->sequence[entry[index]->seqSize - 1];
		//else
			//return ' ';
			return entry[index]->lchr;
	}
	HistoryTreeNode* getAnchor(int indx){
		return entry[indx]->anchor;
	}
	LookupTableEntry* getEntryAt(int indx){
		return entry[indx];
	}

};
