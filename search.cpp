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
FILE *fin =  fopen("test-data/compressed.txt", "rb");  //("cdata.txt", "rb"); Open the binary trace file
ushort buff[BUFSIZE];
int items,i;
int flag=true;
/*
|******************************************************|
|***************Functions******************************|
|******************************************************|
*/
void goTo(ushort cw){
	newState=lookupTable->entry[cw]->state->child[lookupTable->entry[cw]->lchr];
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
	//	ancestor=lookupTable->getAnchor(cw);//
	HistoryTreeNode* ancestor=lookupTable->entry[cw]->anchor;
	if(newState->finalState||(newState->suffix && ! lastEntry->f )){
		lastEntry->anchor=historyTree->addNode(lastEntry,ancestor);
	}else{
		lastEntry->anchor = ancestor;
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
	HistoryTreeNode* V= lookupTable->entry[k]->anchor;
	while(V!=NULL){//root
		if(V->lookupTableEntry->f){
			//report a matching
			cout<<"pattern "<<setw(2)<<V->lookupTableEntry->state->pattID<< " occurs at: "<< setw(6)<<cwCntr<<" in the compressed text and at: "<<setw(12)<<lettersCntr+V->lookupTableEntry->seqSize<<" in the original text\n";
		}else{
			//next
			VID=V->lookupTableEntry->state->getID();
			Node *t=q;
			while((agst->mapTable[t->ID][VID]==0)&&(t!=agst->root)){
				t=t->failureDest;
			}
			//end next
			if(agst->nodesList[agst->mapTable[t->ID][VID]]->finalState){
				//report a matching
				cout<<"pattern "<<setw(2)<<agst->nodesList[agst->mapTable[t->ID][VID]]->pattID<< " occurs at: "<< setw(6)<<cwCntr<<" in the compressed text and at: "<<setw(12)<<lettersCntr+V->lookupTableEntry->seqSize<<" in the original text\n";
			}
		}
		V=V->ancestor;
	}
}
void updateLookupTable(int cw_indx){
	lookupTable->insertEntry( lastEntry, cw_indx);

}
// void searchMultiPLZW(){
//   //Initialize − LookupTable()
//   LookupTable *lookupTable=new LookupTable(agst,decSize); //creat a lookupTablee table of 2^16 entry
//
//   //Initialize − HistoryTree()
//   //lstState ← AGST.root
//   int i = 0;
//   int n ← compressedLength;//∣Γ∣;
//   while (i < n) { //▷ % Γ: denotes LZW-compressed data%
//     int ω_i = compressed[i];   //  ▷ % ω_i : denotes codeword i in Γ%
//     int ω_iPlus1 = compressed[i];  //  ▷ % ω_(i+1) : denotes codeword i+1 in Γ%
//     int k =i + alphabetSize;//∣Σ∣
//     newState = goto(ω_i );
//     f1 = 0;
//     if (newState == NULL) {
//       newState = failure(ω_i );
//       f1 = 1;
//     }
//     f = (lookupTable[ω_i ].f || f1 );
//     anchor = updateMatchingHistory(k, ω_i , newState, f );
//     updateLookupTable(k, ω_i , ω_iPlus1 , newState, anchor, f );
//     output(lstState, k);
//     lstState = next(lstState, newState);
//     i=i+1;
//   }
// }
/*
|******************************************************|
|***************Main***********************************|
|******************************************************|
*/
int main () {

	cw_indx=LetterSetSize;//256
	decSize=(int)pow(2,16);
	lettersCntr=0;
	cwCntr=0;
	agst=new LZW_AGST();
	historyTree=new HistoryTree();
	agst->create("test-data/patterns.txt");
	lookupTable=new LookupTable(agst,decSize); //creat a lookupe table of 2^16 entry

	fseek(fin, 0, SEEK_SET);
	items=fread(buff,2 ,BUFSIZE , fin);
	cw=buff[0];
	newState=lookupTable->getState(cw);
	q=agst->nodesList[agst->mapTable[agst->root->ID][newState->ID]];
	lettersCntr+=lookupTable->entry[cw]->seqSize;// only counter
	cwCntr=0;//only counter
	i=1;

	while(items>0){
		for(;i<items;i++){
			cwCntr++;
			//reuse the same dictionarry instead of creating another one
			if(lookupTable->getEntryAt(cw_indx)!=NULL){

				delete lookupTable->entry[cw_indx];
				lookupTable->entry[cw_indx]=NULL;
			}
			lastEntry=new LookupTableEntry();
			size=lookupTable->getEntryChunkSize(cw);
			lastEntry->fchr=lookupTable->getFirstLetterOf(cw);
			lastEntry->lchr=lookupTable->getFirstLetterOf(buff[i],cw);
			lastEntry->seqSize=size+1;
			lastEntry->f=lookupTable->entry[cw]->f;//lookupTable->isGoFailed(cw);
			//lastEntry->seqSize=lookup->entry[cw]->seqSize+1;
			state=lookupTable->entry[cw]->state;
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
			cw=buff[i];
			lettersCntr+=lastEntry->seqSize;
			// reset the facilities if lookupTable table limit has been reached
			if(cw_indx>=(int)pow(2,16)){// (int)pow(2,16)
				cw_indx=LetterSetSize;
			}
		}
		i=0;
		items=fread(buff,2 ,BUFSIZE , fin);
	}
	////////////////////
	//reuse the same dictionarry instead of creating another one
	if(lookupTable->getEntryAt(cw_indx)!=NULL){
		delete lookupTable->entry[cw_indx];
		lookupTable->entry[cw_indx]=NULL;
	}
	lastEntry=new LookupTableEntry();
	size=lookupTable->getEntryChunkSize(cw);
	lastEntry->fchr=lookupTable->getFirstLetterOf(cw);
	lastEntry->lchr=lookupTable->getFirstLetterOf(buff[i],cw);
	lastEntry->seqSize=size+1;
	lastEntry->f=lookupTable->isGoFailed(cw);
	state= lookupTable->getState(cw);
	goTo(cw);
	if(newState==NULL){
		lastEntry->f=true;
		failure(cw);
	}
	lastEntry->state=newState;
	updateMatchingHistory(cw,newState);
	updateLookupTable(cw_indx);
	output( cw_indx);
	fclose(fin);
	return 0;
}
