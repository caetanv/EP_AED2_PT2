//
// Created by gregorio on 17/07/22.
//

#ifndef EP_AED2_PT2_HEADER_H
#define EP_AED2_PT2_HEADER_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define t 3

// 1 , AFG , DIST , 93 , 18
struct rec
{
    int key;
    // rest
    char codigoLivro[6]; // chave
    char titulo[30];
    char nomeCompletoPrimeiroAutor[30];
    int anoPublicacao;
};
typedef struct rec recordNode;

struct bTreeNode
{
    bool isLeaf;
    int pos;
    int noOfRecs;
    int keyRecArr[2 * t - 1]; //as chaves dos registros, que são no máximo 2t-1
    int posRecArr[2 * t - 1]; //posições dos registros no arquivo data.dat, que são no máximo 2t-1
    int children[2 * t]; //posições das páginas filhas no arquivo tree.dat, que são no máximo 2t
}
typedef struct bTreeNode bTreeNode;

struct tree
{
    char fileName[20];
    FILE* fp;
    int root;
    int nextPos;
};
typedef struct tree bTree;

bTree* createTree(char* fileName,bool mode);
bTreeNode* nodeInit(bTreeNode* node,bool isLeaf,bTree* tree);
void insert(bTree* tree,recordNode* record);
void delete(bTree* tree,int key);
void traverse(bTree* tree, int root);
void dispNode(bTreeNode* node);
void writeFile(bTree* ptr_tree, bTreeNode* p, int pos);
void readFile(bTree* ptr_tree, bTreeNode* p, int pos);


void enterData(recordNode* record, int id_num, char country[], char Grate[], int Score, int Rate);
recordNode* getData(char *filepath, int len);
recordNode* search(bTree* tree, int key);
recordNode* searchRecursive(bTree* tree, int key, bTreeNode* root);
bool removeFromTree(bTree* tree, int key);
bTreeNode* merge(bTree* tree, bTreeNode *node, int idx);
void borrowFromNext(bTree* tree, bTreeNode *node, int idx);
void borrowFromPrev(bTree* tree, bTreeNode *node, int idx);
void fill(bTree* tree, bTreeNode *node, int idx);
recordNode *getSucc(bTree* tree, bTreeNode *node, int idx);
recordNode *getPred(bTree* tree, bTreeNode *node, int idx);
void removeFromNonLeaf(bTree* tree, bTreeNode *node, int idx);
void removeFromLeaf (bTree* tree, bTreeNode *node, int idx);
void removeNode(bTree* tree, bTreeNode* node, int k);
int findKey(bTreeNode* node, int k);


#endif //EP_AED2_PT2_HEADER_H