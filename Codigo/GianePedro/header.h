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
#define DELETED_ENTRY "-11111"

// 1 , AFG , DIST , 93 , 18
//ESSE É O REGISTRO

struct rec
{
    //int key;
    char codigoLivro[6]; // chave
    char titulo[30];
    char nomeCompletoPrimeiroAutor[30];
    int anoPublicacao;
};
typedef struct rec recordNode;

//ESSE É O NÓ DA ARVORE
typedef struct bTreeNode
{
    bool isLeaf;
    int pos;
    int noOfRecs;
    int keyRecArr[2 * t - 1];   // as chaves dos registros, que são no máximo 2t-1
    int posRecArr[2 * t - 1];   // posições dos registros no arquivo data.dat, que são no máximo 2t-1
    int children[2 * t];        // posições das páginas filhas no arquivo tree.dat, que são no máximo 2t
} bTreeNode;
//typedef struct bTreeNode bTreeNode;

struct tree
{
    char tree_filename[20];
    char data_filename[20];
    FILE* tree_fp;
    FILE* data_fp;
    int root;
    int nextPos;
};
typedef struct tree bTree;

bTree *createTree(char *tree_filename, char *data_filename, bool mode);
bTreeNode* nodeInit(bTreeNode* node,bool isLeaf,bTree* tree);
void insert(bTree* tree,recordNode* record);
void delete(bTree* tree,int key);
void traverse(bTree* tree, int root);
void dispNode(bTreeNode* node);
void write_treedat(bTree* ptr_tree, bTreeNode* p, int pos);
void write_datadat(bTree* ptr_tree, bTreeNode* p, recordNode* record, int recpos);
void read_treedat(bTree* ptr_tree, bTreeNode* p, int pos);


void enterData(recordNode* registry, char codigoLivro[], char titulo[], char nomeCompleto[], int anoPublicado);
//recordNode* getData(char *filepath, int len);
recordNode * node_read(char *filepath, int a_len);
//recordNode* search(bTree* tree, int key);
recordNode* searchRecursive(bTree* tree, int key, bTreeNode* root);
bool removeFromTree(bTree* tree, int key);
bTreeNode* merge(bTree* tree, bTreeNode *node, int idx);
void borrowFromNext(bTree* tree, bTreeNode *node, int idx);
void borrowFromPrev(bTree* tree, bTreeNode *node, int idx);
void fill(bTree* tree, bTreeNode *node, int idx);
int getSucc(bTree* tree, bTreeNode *node, int idx);
int getPred(bTree* tree, bTreeNode *node, int idx);
bool removeFromNonLeaf(bTree* tree, bTreeNode *node, int idx);
void removeFromLeaf(bTreeNode *node, int idx);
bool removeNode(bTree* tree, bTreeNode* node, int k);
//int findKey(bTreeNode* node, int k);


#endif //EP_AED2_PT2_HEADER_H
