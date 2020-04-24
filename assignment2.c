/*Make a program that:
counts the number of occurrences of each distinct word of a text file
records the location of the first and last occurrences of each distinct word
records the smallest, largest, and average distances between
 consecutive occurrences of the same distinct word
Your program:
must use a hash table (separate chaining),
the hash table size should grow dynamically,
each hash table entry should point to either a linked list or an ordered binary tree*/
#include<stdio.h> 
#include<stdlib.h>
#include<string.h>
#define INT_MAX 2147483647

//estrutura dos nodes da linked list
typedef struct Node{
	char word[64]; //saving the word itself
	int firstIndex; // first occurence of distinct word
	int lastindex; //last index the word was found (used to calculate distances)
	int counter; //number of times word appears
	int minDis; //minimum number of words between two equal words
	int maxDis; //maximum number of words between two equal words
	int totalDis;//total distance of between equal words
	int average; // average=total/counter
	struct Node* next; //pointer to next Node
}Node;
//
static struct Node* hashtable[1007];


//funcao hash
unsigned int HashCode(const char* str){
	unsigned int hcode;
	for(int h=0u;*str != '\0';str++){
		hcode=157u * hcode + (0xFFu & (unsigned int)*str); // arithmetic overflow may occur here (just ignore it!)
	}
	return hcode % 1007; // due to the unsigned int data type, it is guaranteed that 0<= h % s < s
}
//



//funcao para adicionar um node a linked list
struct Node* putinNode(int index,char* newWord){ //head- cabeca atual da linked list, newData- novo node(nova cabeca) a colocar na linked list, dataSize- sizeof(newData)
	//so usamos esta funcao para introduzir novos nodes (palavras novas)	

	//aloccate memory for the new node
	struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));
	if(newNode==NULL){
		fprintf(stderr,"Not enough memory!");
	}
    unsigned int i = HashCode(newWord);
	strcpy(newNode->word,newWord); //copy word
	newNode->firstIndex=index; // first occurence of word
	newNode->lastindex=index; //index the word appears to calculate distances
	newNode->counter=1; //counter for the number of times word appears initiated at 1
	newNode->maxDis=0; //max distance is 0 so the next distance is recognised and the new max
	newNode->minDis=INT_MAX; //min distance initiated with max integer so the next min is recognised as the new min
	newNode->next = NULL; //initiate pointer to next as null
	newNode->totalDis=0; //total distance starts at 0
	newNode->average=0; //total distance starts at 0 because there is no total distance yet

    if(hashtable[i]==NULL){
        hashtable[i] = newNode;
    }else{
        struct Node* lastnode = hashtable[i];
        while(lastnode->next!=NULL){
            lastnode = lastnode->next;
        }
        lastnode->next = newNode;
    }
	
	return newNode;
}
//

//funcao para atualizar uma palavra ja existente
void updateNode(int index,struct Node* node){
	node->counter++; //increment counter
	int dist=index-(node->lastindex); //this is the distance between the last equal word and this one
	node->totalDis+=dist; //add the distance
	if(dist>(node->maxDis)){ node->maxDis=dist; } //see if the distance is bigger than the last maxDis
	if(dist<(node->minDis)){ node->minDis=dist; } //see if the distance is lower than the min dist
	node->average=(node->totalDis)/(node->counter);	//calculate average distance
	node->lastindex=index; //change the index of the last time the word appeared
}
//


//funcao para confirmar se a informar ja existe nessa posicao
struct Node* findWord(const char* newWord){ //newWord-the word we are trying to see if already exist, hashtable-hashtable of nodes, htSize- hashTable size
	struct Node* DSebastiao;//starts as head of linked list
	unsigned int i = HashCode(newWord); //hash code for the new word
	DSebastiao=hashtable[i];
	while(DSebastiao != NULL && strcmp(newWord,DSebastiao->word)!=0){
		DSebastiao=DSebastiao->next; //mudamos para o next ate o encontrarmos	
	}
	return DSebastiao; //sera NULL se nao o encontrarmos
}

//

//Hash Table:
/*A dictionary in which keys are mapped to array positions by hash functions. Having the keys of more than one item map to the same position is called a collision.
 There are many collision resolution schemes, but they may be divided into open addressing,
 chaining, and keeping one special overflow area. Perfect hashing avoids collisions, but may be time-consuming to create.*/



unsigned int insertWord(int index, char* newWord){
    unsigned int hcode = HashCode(newWord);
	struct Node* node = (struct Node*) malloc(sizeof(struct Node));
    node = findWord(newWord);
    if(node==NULL){
        putinNode(index, newWord);
    }else{
        updateNode(index, node);
    }
    return HashCode(newWord);
}

 // Copiar codigo da hash function dos slides
 //Hash function:

 //

 //https://study.com/academy/lesson/separate-chaining-concept-advantages-disadvantages.html  -  Hash table and separate chaining
 //https://www.geeksforgeeks.org/hashing-set-2-separate-chaining/ - separate chaining

 //Separate chaining:
 /*
 .Is defined as a method by which linked lists of values are built in association with each location within the hash table when a collision occurs.
 .The concept of separate chaining involves a technique in which each index key is built with a linked list.
  This means that the table's cells have linked lists governed by the same hash function.
  So, in place of the collision error the cell now contains a linked list with both Strings*/


//The following code may be used to open and close a text file.

int file_operation(char *filename){

    FILE *file;
    char word[50];
    int counter = 1;
    file = fopen(filename, "r");

    if (file == NULL)
        printf("Can't open %s for reading.\n", filename);
    else
    {
        while (fscanf(file, "%s", word) != EOF)
        {
            for(int i = 0; word[i]; i++){
				if(ispunct(word[i])){
					//for(int k = i; word)
					word[i] = '\0';
				}
 				 word[i] = tolower(word[i]);
			}
			if(word[0]=='\0' || word[0] == ' '){
				continue;
			}
            insertWord(counter,word);
            counter++;
        }
    }
    fclose(file);
	return counter;
}
//

//The following code may be used to open and close a text file.

void file_operation2(char *filename,int filesize){

    FILE *file;
    char word[64];
	char uniquewords[filesize][64];
	int wordsind = 0;
	int repeat = 0;

    file = fopen(filename, "r");

    if (file == NULL)
        printf("Can't open %s for reading.\n", filename);
    else
    {
        while (fscanf(file, "%s", word) != EOF)
        {

            for(int i = 0; word[i]; i++){
				if(ispunct(word[i])){
					word[i] = '\0';
				}
 				 word[i] = tolower(word[i]);
			}
			if(word[0]=='\0' || word[0] == ' '){
				continue;
			}
			for(int k = 0; k<wordsind; k++){
				if (strcmp(word,uniquewords[k])==0){
					repeat=1;
				}
			}
			if(repeat==0){
				for(int c = 0; c<64; c++){
					uniquewords[wordsind][c]=word[c];
				}
				wordsind++;
            	struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));
            	newNode = findWord(word);
            	int findex = newNode->firstIndex;
            	int counter = newNode->counter;
	        	int lindex = newNode->lastindex; 
	        	int minDis = newNode->minDis; 
	        	int maxDis = newNode->maxDis; 
        		int totalDis = newNode->totalDis;
        		int average = newNode->average;
            	printf("Word:%s | FirstIndex:%d | LastIndex:%d | NumOfAppearances:%d | MinDistance: %d | MaxDistance: %d | TotalDistance: %d | AverageDistance: %d\n",word, findex, lindex, counter, minDis, maxDis, totalDis, average);
            	//free(newNode);
			}
        }
    }
}

int main(int argc, char *argv[])
{   
    if (argc < 2){
		printf("Invalid Number of arguments!\nUsage: <program> <file>");
		return 1;
	}
	char * filename = argv[1];

    int filesize = file_operation(filename);
    file_operation2(filename, filesize);
    return 0;
}
