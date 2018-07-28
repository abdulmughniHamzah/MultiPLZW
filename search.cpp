#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <string.h>
#include <stdlib.h>
#include <cmath>
#include "lookupTable.cpp"
using namespace std;
/*
|******************************************************|
|***************Constants******************************|
|******************************************************|
*/
const int BUFSIZE =100;
/*
|******************************************************|
|***************global variables***********************|
|******************************************************|
*/
LookupTable *lookupTable;
Node				*state;
Node 				*newState;
Node 				*failureDest;
LZW_AGST 				*agst;
HistoryTree	*historyTree;
LookupTableEntry *lastEntry;
Node* q;
long lettersCntr;
long cwCntr;
ushort cw,nxt_cw;
int size;
uchar lchr,fchr;
int cw_indx;
int decSize;
string patFile="test-data/patterns.txt";
FILE *fin =  fopen("test-data/compressed.txt", "rb");  // Open the binary trace file
ushort buff[BUFSIZE];
int items,i;
int flag=true;
/*
|*********************************************************************************************|
|***************Functions*********************************************************************|
|*********************************************************************************************|
*/
void goTo(ushort cw){
	newState=lookupTable->entries[cw].state->child[lookupTable->entries[cw].lchr];
	//return newState;
}
void failure(ushort cw){
	Node* tmpState=state->getFailure();
	newState=tmpState->getChild(lookupTable->getLastLetterOf(cw));
	while(newState==NULL&&tmpState!=agst->root){
		tmpState=tmpState->getFailure();
		newState=tmpState->getChild(lookupTable->getLastLetterOf(cw));
	}
	if(newState==NULL){
		newState=agst->root;
	}
}
void updateMatchingHistory( ushort cw , Node* newState /*,lastEntry->f*/ ){
	if(newState->finalState||(newState->suffix && ! lastEntry->f )){
		historyTree->addNode(lookupTable,cw_indx,cw);
	}else{
		historyTree->setAnchor(lookupTable,cw_indx,cw);
	}
}
//
Node* next(bool f,Node* Q, Node* newState){
	if(f){
		return agst->nodesList[agst->mapTable[agst->root->ID][newState->ID]];
	}
	if(Q==agst->root){
		return agst->nodesList[agst->mapTable[Q->ID][newState->ID]];
	}
	int z=agst->mapTable[Q->ID][newState->ID];
	if(z!=0){
		return agst->nodesList[agst->mapTable[Q->ID][newState->ID]];
	}
	return next(f,Q->getFailure(),newState);
}
void output( int k/*cw_indx*/){
	int VID;

	int V=k;
	if(!(lookupTable->entries[k].anchor.bookmarked))
		V=lookupTable->entries[k].anchor.ancestorIndx;

	while(V>-1){//root

		if(lookupTable->entries[V].f){
			//report a matching
		//	cout<<"pattern "<<setw(2)<<lookupTable->entries[V].state->pattID<< " occurs at: "<< setw(6)<<cwCntr<<" in the compressed text and at: "<<setw(12)<<lettersCntr+lookupTable->entries[V].seqSize<<" in the original text\n";
		}else{
			//next
			VID=lookupTable->entries[V].state->getID();
			Node *t=q;
			while((agst->mapTable[t->ID][VID]==0)&&(t!=agst->root)){
				t=t->failureDest;
			}
			//end next
			if(agst->nodesList[agst->mapTable[t->ID][VID]]->finalState){
				//report a matching
			//	cout<<"pattern "<<setw(2)<<agst->nodesList[agst->mapTable[t->ID][VID]]->pattID<< " occurs at: "<< setw(6)<<cwCntr<<" in the compressed text and at: "<<setw(12)<<lettersCntr+lookupTable->entries[V].seqSize<<" in the original text\n";
			}
		}
		V=lookupTable->entries[V].anchor.ancestorIndx;
	}
}
void updateLookupTable(int cw_indx){
	lookupTable->insertEntry( lastEntry, cw_indx);

}
void searchMultiPLZW(){

	newState=lookupTable->getState(cw);
	q=agst->nodesList[agst->mapTable[agst->root->ID][newState->ID]];
	lettersCntr+=lookupTable->entries[cw].seqSize;// only counter

	bool fileHitEnd=false;

	while(!fileHitEnd){

		for(int i=0;i<items;i++){

			cw=buff[i];
			if(i+1<items){
				nxt_cw=buff[i+1];
			}else{
				items=fread(buff,2 ,BUFSIZE , fin);
				if(!(items>0)){
					fileHitEnd=true;
				}
				nxt_cw=buff[0];
			}

			cwCntr++;

			lastEntry=&(lookupTable->entries[cw_indx]);//new LookupTableEntry();
			size=lookupTable->getEntryChunkSize(cw);
			lastEntry->fchr=lookupTable->getFirstLetterOf(cw);
			lastEntry->lchr=lookupTable->getFirstLetterOf(nxt_cw,cw);
			lastEntry->seqSize=size+1;
			lastEntry->f=lookupTable->entries[cw].f;//lookupTable->isGoFailed(cw);
			
			state=lookupTable->entries[cw].state;
			goTo(cw);
			if(newState==NULL){
				lastEntry->f=true;
				failure(cw);
			}
			lastEntry->state=newState;
			updateMatchingHistory(cw,newState);
			updateLookupTable(cw_indx);
			output(cw_indx);
			q=next(lastEntry->f,q,newState);
			cw_indx++;
			lettersCntr+=lastEntry->seqSize;
			// re-initialize lookup table
			if(cw_indx>=(int)pow(2,16)){// (int)pow(2,16)
				cw_indx=LETTER_SET_SIZE;
				delete  lookupTable;
				lookupTable=new LookupTable(agst,decSize);
			}

		}
	}

}
/*
|*********************************************************************************************|
|***************Main*********************************************************************|
|*********************************************************************************************|
*/

int main () {

	  cw_indx=LETTER_SET_SIZE;//256
		decSize=(int)pow(2,16);//dictionarry size
		lettersCntr=0;// just a counter
		cwCntr=0;//just anoher counter
		agst=new LZW_AGST();//initiate the LZW-AGST
		historyTree=new HistoryTree();//Initialize the history-tree
		agst->create(patFile);//create LZW-AGST
		lookupTable=new LookupTable(agst,decSize); //creat a lookupe table of 2^16 entry

		fseek(fin, 0, SEEK_SET);
		items=fread(buff,2 ,BUFSIZE , fin);


//search multiple patterns in the compressed data directly using MultiPLZW.
	searchMultiPLZW();

	fclose(fin);
	return 0;
}

