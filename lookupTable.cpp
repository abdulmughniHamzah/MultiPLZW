#include "lookupTable.h"

LookupTable::LookupTable(LZW_AGST* afsm, int tableSize){

		this->entries=new LookupTableEntry[tableSize];

		size=tableSize;
		for(int i=0;i<LETTER_SET_SIZE;i++){

				setEntry(0, uchar(i),uchar(i),uchar(i),afsm->root,false);

		}

}
void
HistoryTree::addNode(LookupTable* lookupTable,int  cw_indx,ushort cw){
	if(lookupTable->entries[cw].anchor.bookmarked)
		lookupTable->entries[cw_indx].anchor.ancestorIndx=cw;
	else
		lookupTable->entries[cw_indx].anchor.ancestorIndx=lookupTable->entries[cw].anchor.ancestorIndx;
	lookupTable->entries[cw_indx].anchor.bookmarked=true;
}
void
HistoryTree::setAnchor(LookupTable* lookupTable,int  cw_indx,ushort cw){
	if(lookupTable->entries[cw].anchor.bookmarked)
		lookupTable->entries[cw_indx].anchor.ancestorIndx=cw;
	else
		lookupTable->entries[cw_indx].anchor.ancestorIndx=lookupTable->entries[cw].anchor.ancestorIndx;
	lookupTable->entries[cw_indx].anchor.bookmarked=false;
}
void
LookupTable::setEntry(int size, uchar fchr, uchar lchr,int index,Node *state,bool f){

	//entry[index]->sequence=chunk;
	entries[index].fchr=fchr;
	entries[index].lchr=lchr;
	entries[index].seqSize=size;
	entries[index].state=state;
	entries[index].f=f;
	//entries[index].anchor.ancestorIndx=-1;
	//entries[index].anchor.bookmarked=false;
}
void
LookupTable::insertEntry(LookupTableEntry* nEntry,int indx){
	/*if(indx>=size){
		cout<<"error in table size: indx>=size "<<endl;
		exit(0);
	}*/
	//entries[indx]=*nEntry;

}
