#include "LZW_AGST.cpp"

class HistoryTreeNode{//lookupTable queue
	public:
	bool  bookmarked;//indicate whether the lookup entry is bookmarked to the historyTree or not
	int ancestorIndx;

	HistoryTreeNode(){

		bookmarked=false;
		ancestorIndx=-1;
	}
};
class LookupTable;
class HistoryTree{
	public:
		void addNode  (LookupTable* lookupTable,int  cw_indx,ushort cw);
		void setAnchor(LookupTable* lookupTable,int  cw_indx,ushort cw);
};

class LookupTableEntry{
	public:
	HistoryTreeNode anchor;
	Node      *state;
	//uchar      *sequence;
	uchar fchr;
	uchar lchr;
	int        seqSize;
	bool f ;//go_failed flag
	LookupTableEntry(){
		state=NULL;
	}
};
//the lookup table data structure difination
class LookupTable{
	public:
	LookupTableEntry *entries;
	int size;
	LookupTable(LZW_AGST* afsm, int size);
	void setEntry(/*uchar* chunk,int chnkSize,*/int size,uchar fchr,uchar lchr,int index,Node *state,bool f);
	void insertEntry(LookupTableEntry* nEntry,int indx);
	bool isGoFailed(int indx){
		return (entries[indx].f);
	}
	~LookupTable(){
		delete [] entries;
		entries=NULL;
	}
	Node * getState(int indx){
		return entries[indx].state;
	}
	/*uchar* getEntryChunk(int index){

		return entries[index]->sequence;

	}*/
	int getEntryChunkSize(int index){

		return entries[index].seqSize;

	}
	uchar getFirstLetterOf(int index1, int index2){
		if(entries[index1].state!=NULL)
			return entries[index1].fchr;
		else
			return entries[index2].fchr;
	}
	uchar getFirstLetterOf(int index){
		return entries[index].fchr;
	}
	uchar getLastLetterOf(int index){
			return entries[index].lchr;
	}
	HistoryTreeNode* getAnchor(int indx){
		return &(entries[indx].anchor);
	}
	LookupTableEntry* getEntryAt(int indx){
		return &(entries[indx]);
	}

};
/*
================================================================================
*/
