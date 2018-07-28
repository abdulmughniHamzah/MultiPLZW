
//LZW dictionary trie
#define   LETTER_SET_SIZE 256
typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;
struct DecEntry{
	public:
	uchar* chunk;
	int size;

	DecEntry(){
		chunk=NULL;
		size=0;
	}
	~DecEntry(){
		delete chunk;
	}
};
struct LZWTrieNode{
	public:
	int codeword;// index of the corresponding entry in the dictionary
	uchar* chunk;
	uchar edge;
	LZWTrieNode *children[LETTER_SET_SIZE];
	LZWTrieNode(){

		codeword=-1;
		edge='\0';
		chunk=NULL;

		for(int i=0;i<LETTER_SET_SIZE;i++){
			uchar lett=(uchar)i;
			children[lett]=NULL;

		}
	}
	~LZWTrieNode(){
		delete [] chunk;

		for(int i=0;i<LETTER_SET_SIZE;i++){
			if(children[i]!=NULL)
				delete children[i];

		}

	}


};
class Dictionary{
	public:
	int size;
	DecEntry* entry;
	Dictionary(int size){
		entry=new DecEntry[size];
		for(int i=0;i<LETTER_SET_SIZE;i++){
			setEntry(new uchar[1]{uchar(i)},1,uchar(i));
		}
	}
	uchar getFirstLetterOf(unsigned int index1,unsigned int index2){
		if(entry[index1].chunk!=NULL)
			return entry[index1].chunk[0];
		else
			return entry[index2].chunk[0];
	}
	uchar* getEntryChunk(unsigned int index){
		if(entry[index].chunk!=NULL)
			return entry[index].chunk;
		else
			return NULL;
	}
	int getEntryChunkSize(unsigned int index){

		return entry[index].size;

	}
	void setEntry(uchar* chunk,int chnkSize, int index){
		entry[index].chunk=chunk;
		entry[index].size=chnkSize;
	}
	~Dictionary(){
		delete [] entry;
	}

};
class LZWTrie{



	public:

	LZWTrieNode* root;
    void establish();
	inline LZWTrieNode* getChild(LZWTrieNode* parent,uchar chr){return parent->children[chr];}
	void addNewChild(LZWTrieNode* parent,uchar chr,int codeword);
	LZWTrie(){root=new LZWTrieNode();}
	void insert(char* strArray, int strt,int dicIndx);
	LZWTrieNode* search(char* path);
	~LZWTrie(){
		delete root;
	}
};
