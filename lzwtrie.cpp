#include<iostream>
//#define LetterSet 256
using namespace std;
#include "lzwtrie.h"
int size(char *arr){
	int cnt=0;
	for(;arr[cnt]!='\0';cnt++);
	return cnt;
}
LZWTrieNode*
LZWTrie::search(char* path){
	int len=size(path);
	LZWTrieNode* crnt=root;
	int i=0;
	while(crnt!=NULL&&i<len){
		crnt=crnt->children[path[i]];
			if(crnt!=NULL)
				i++;
			else
			return NULL;
	}
	return crnt;
	
}
void 
LZWTrie::establish(){
	if(root==NULL)
		root=new LZWTrieNode();
		
	for(int i=0;i<LetterSetSize;i++){
			
		uchar lett=(uchar)i;
		
		root->children[lett]=new LZWTrieNode();
		root->children[lett]->edge=lett;
		root->children[lett]->codeword=lett;
	}
}
void 
LZWTrie::addNewChild(LZWTrieNode* parent,uchar chr,int codeword){
	parent->children[chr]=new LZWTrieNode();
	parent->children[chr]->edge=chr;
	parent->children[chr]->codeword=codeword;
}
void
LZWTrie::insert(char* strArray, int strt,int dicIndx){
	/*
	if(root==NULL)
		root=new LZWTrieNode();
		
	LZWTrieNode* crnt=root;
	int i=strt;
	while(crnt->children[strArray[i]]!=NULL){
		crnt=crnt->children[strArray[i]];
		i++;
	}
	crnt->children[strArray[i]]=new LZWTrieNode();
	crnt->children[strArray[i]]->codeword=dicIndx;
	crnt->children[strArray[i]]->edge=strArray[i];
	
		*/
	
	
}


