#include "header.h"

bool quit = false;



recordNode * node_read(char *filepath, int a_len){
    recordNode *reg_array = malloc(sizeof(recordNode)*a_len);


    char delim = ',';

    char codigoLivro[6];
    char titulo[30];
    char nomeCompleto[30];
    int anoPublicado;

    int num_reg = 0;
    FILE* file_reg = fopen(filepath, "r"); //TODO: o que significa "%[^,]" ???
    while(num_reg < a_len && fscanf(file_reg, "%d,%[^,],%[^,],%d,%d", codigoLivro, titulo, nomeCompleto, &anoPublicado)) {
        enterData(&reg_array[num_reg], codigoLivro, titulo, nomeCompleto, anoPublicado);
        num_reg++;
    }
}

void enterData(recordNode* registry, char codigoLivro[], char titulo[], char nomeCompleto[], int anoPublicado){
    strcpy(registry->codigoLivro, codigoLivro);
    strcpy(registry->codigoLivro, codigoLivro);
    strcpy(registry->titulo, titulo);
    strcpy(registry->nomeCompletoPrimeiroAutor, nomeCompleto);
    registry->anoPublicacao = anoPublicado;

    return;
}
bTree* createTree(char* fileName,bool mode)
{
	bTree* tree = malloc(sizeof(bTree));
    
	if(!mode) //new file
    {
        //Criar arquivo
        strcpy(tree->fileName,fileName);
        tree->fp = fopen(fileName,"w");
        fclose(tree->fp);

        tree->root = 0;
        tree->nextPos = 0;
    } else {
        FILE *oldFile = fopen("tree.dat", "r");
        fread(tree, sizeof(bTree), 1, oldFile);
        fclose(oldFile);
    }

    tree->fp = fopen(fileName, "r+");
    return tree;
}

bTreeNode* nodeInit(bTreeNode* node,bool isLeaf,bTree* tree)
{
    node->isLeaf = isLeaf;
    node->noOfRecs=0;
    node->pos = tree->nextPos;
    (tree->nextPos)++;
    int i;
    for (int i = 0; i < 2*t; ++i)
    {
        node->children[i] = -1;
    }
    return node;
}
//------------------------------E/S-------------------------------------------
//TODO: verificar como implementar essas duas funções a seguir, agora com o arquivo de indice.
void write_treedat(bTree* ptr_tree, bTreeNode* p, int pos) {// pos = -1; use nextPos {
    if(pos == -1) {
        pos = ptr_tree->nextPos++;
    }

    fseek(ptr_tree->fp, pos * sizeof(bTreeNode), 0);
    fwrite(p, sizeof(bTreeNode), 1, ptr_tree->fp);

}

void read_treedat(bTree* ptr_tree, bTreeNode* p, int pos) {
    fseek(ptr_tree->fp, pos * sizeof(bTreeNode), SEEK_SET);
    fread(p, sizeof(bTreeNode), 1, ptr_tree->fp);
}

//----------------------------------ALGORITMOS----------------------------------
//TODO: adaptar os metodos que lidam com a estrutura de dados para trabalhar com a estrutura de dados
void splitChild(bTree* tree, bTreeNode* x, int i, bTreeNode* y)
{
    bTreeNode* z = malloc(sizeof(bTreeNode));
    nodeInit(z,y->isLeaf,tree);
    z->noOfRecs = t-1;

    int j;
    for(j=0;j<t-1;j++)
    {
        z->keyRecArr[j] = y->keyRecArr[j+t];
    }

    if(!y->isLeaf)
    {
        for(j=0;j<t;j++)
        {
            z->children[j] = y->children[j+t];
            y->children[j+t] = -1; //RESPOSTA:
        }
    }
    y->noOfRecs = t-1;

    for(j=(x->noOfRecs); j >= i+1;j--)
    {
        x->children[j+1] = x->children[j];
    }
    //linking z
    x->children[i+1] = z->pos;

    for(j=(x->noOfRecs) - 1; j >= i;j--)
    {
        x->keyRecArr[j+1] = x->keyRecArr[j];
    }
    x->keyRecArr[i] = y->keyRecArr[t-1];
    x->noOfRecs++;

    write_treedat(tree, x, x->pos);
    write_treedat(tree, y, y->pos);
    write_treedat(tree, z, z->pos);
    free(z);
}

void insertNonFull(bTree* tree,bTreeNode* x,recordNode* record)
{
    int i = (x->noOfRecs)-1;
    if(x->isLeaf == true)
    {
        while((i>=0) && (record->key < x->keyRecArr[i]))
        {
            x->keyRecArr[i+1] = x->keyRecArr[i];
            i--;
        }
        x->keyRecArr[i+1] = record;
        (x->noOfRecs)++;

        writeFile(tree, x, x->pos);
    }
    else
    {
        while((i>=0) && (record->key < x->keyRecArr[i]))
        {
            i=i-1;
        }
        bTreeNode* childAtPosi = malloc(sizeof(bTreeNode));
        readFile(tree, childAtPosi, x->children[i+1]);

        if(childAtPosi->noOfRecs == (2*t-1))
        {
            splitChild(tree,x,i+1,childAtPosi);
            if( x->keyRecArr[i+1] < record->key){
                i++;
            }
        }

        readFile(tree, childAtPosi, x->children[i+1]);
        insertNonFull(tree,childAtPosi,record);

        free(childAtPosi);
    }
}

/*
recordNode* getData(char *filepath, int len) {
    
    recordNode *recordArr = malloc(sizeof(recordNode)*len);
    char delim = ','; char line[256];
    int file_no = 0;
    int i,key,score,rate;
    char country[255];
    char grate[255];
    
    FILE *inpFile = fopen(filepath, "r");
    
    while(file_no < len && fscanf(inpFile, "%d,%[^,],%[^,],%d,%d", &key, country, grate, &score, &rate)) {
        enterData(&recordArr[file_no], key, country, grate, score, rate);
        file_no++;
    }

    return recordArr;
}



void insert(bTree* tree,recordNode* record)
{
	if(tree->nextPos == 0) //empty tree, first element.
	{
		tree->root = tree->nextPos;

		bTreeNode* firstNode = malloc(sizeof(bTreeNode));
		nodeInit(firstNode,true,tree);
		firstNode->keyRecArr[0] = record;
		(firstNode->noOfRecs)++;

        writeFile(tree, firstNode, firstNode->pos);

		free(firstNode);
		return;
	}
	else
	{
		bTreeNode* rootCopy = malloc(sizeof(bTreeNode));
        readFile(tree, rootCopy, tree->root);

		if(rootCopy->noOfRecs == (2*t-1))
		{
			bTreeNode* newRoot = malloc(sizeof(bTreeNode));
			nodeInit(newRoot,false,tree);
			newRoot->children[0] = tree->root;

			splitChild(tree,newRoot,0,rootCopy);

			int i=0;
			if(newRoot->keyRecArr[0]->key < record->key){
				i++;
			}
			
			bTreeNode* childAtPosi = malloc(sizeof(bTreeNode));
            readFile(tree, childAtPosi, newRoot->children[i]);
			insertNonFull(tree,childAtPosi,record);

			tree->root = newRoot->pos;
            
            //writeFile(tree, rootCopy, rootCopy->pos); //RESPOSTA:

			free(newRoot);
            free(childAtPosi);
		}
		else
		{
			insertNonFull(tree,rootCopy,record);
		}
		free(rootCopy);
	}
}

void traverse(bTree* tree, int root) {
    
    if(-1 == root) {    
        return;
    }

    bTreeNode *toPrint = malloc(sizeof(bTreeNode));
    readFile(tree, toPrint, root);
    dispNode(toPrint);

    for(int i = 0; i < 2*t; i++) {
        traverse(tree, toPrint->children[i]);
    }

    free(toPrint);
}

void dispNode(bTreeNode* node)
{
	printf("Position in node:%d\n",node->pos );
    printf("Number of Records:%d\n",node->noOfRecs );
	printf("Is leaf?:%d\n",node->isLeaf );
	printf("Keys:\n");
	for(int i = 0; i < node->noOfRecs; i++)
	{
		printf("%d ", node->keyRecArr[i]->key);
	}
	printf("\n");
	printf("Links:\n");
	for (int i = 0; i < 2*t; ++i)
	{
		printf("%d ",node->children[i] );
	}
	printf("\n");
    printf("\n");
}

int searchRecursive(bTree* tree, int key, bTreeNode* root) {
    int i = 0;
    
    while(i < root->noOfRecs && key > root->keyRecArr[i])
        i++;
    
    
    if(i < root->noOfRecs && key == root->keyRecArr[i])
        return root->keyRecArr[i];
    
    
    else if(root->isLeaf) {
        return NULL;
    }
    else {
        bTreeNode* childAtPosi = malloc(sizeof(bTreeNode));
        readFile(tree, childAtPosi, root->children[i]);

        recordNode* found = searchRecursive(tree, key, childAtPosi);
        free(childAtPosi);
        return found;
    }
}

recordNode* search(bTree* tree, int key) {
    
    bTreeNode* root = malloc(sizeof(bTreeNode));
    readFile(tree, root, tree->root);

    recordNode* result = searchRecursive(tree, key, root);
    free(root);
    return result;
     
}

int findKey(bTreeNode* node, int k) {
    int idx=0;
    while (idx < node->noOfRecs && node->keyRecArr[idx]->key < k)
        ++idx;
    return idx;
}

// A function to remove the idx-th key from this node - which is a leaf node
void removeFromLeaf (bTree* tree, bTreeNode *node, int idx) {
    // Move all the keys after the idx-th pos one place backward 
    for (int i=idx+1; i<node->noOfRecs; ++i){
	    node->keyRecArr[i-1] = node->keyRecArr[i];
    }
    // Reduce the count of keys
    node->noOfRecs--;
}
 
// A function to remove the idx-th key from this node - which is a non-leaf node
void removeFromNonLeaf(bTree* tree, bTreeNode *node, int idx) {
 
    int k = node->keyRecArr[idx]->key;
    
    bTreeNode *child = malloc(sizeof(bTreeNode));
    bTreeNode *sibling = malloc(sizeof(bTreeNode));
    
    readFile(tree, child, node->children[idx]);
    readFile(tree, sibling, node->children[idx+1]); // SOLUÇÃO: children[idx+1] ao inves de children[idx]
 
    // If the child that precedes k (C[idx]) has atleast t keys,
    // find the predecessor 'pred' of k in the subtree rooted at
    // C[idx]. Replace k by pred. Recursively delete pred
    // in C[idx]
    
    if (child->noOfRecs >= t) {
        recordNode* pred = getPred(tree, node, idx);
        node->keyRecArr[idx] = pred;
        removeNode(tree, child, pred->key); //SOLUÇÃO: pred->key em vez de pred
    }
 
    // If the child C[idx] has less that t keys, examine C[idx+1].
    // If C[idx+1] has atleast t keys, find the successor 'succ' of k in
    // the subtree rooted at C[idx+1]
    // Replace k by succ
    // Recursively delete succ in C[idx+1]
    else if  (sibling->noOfRecs >= t)
    {
        recordNode* succ = getSucc(tree, node, idx);
        node->keyRecArr[idx] = succ;
        removeNode(tree, sibling, succ->key); //RESPOSTA: succ->key ao invés de succ
    }
 
    // If both C[idx] and C[idx+1] has less that t keys,merge k and all of C[idx+1]
    // into C[idx]
    // Now C[idx] contains 2t-1 keys
    // Free C[idx+1] and recursively delete k from C[idx]
    else {
        child = merge(tree, node, idx); //RESPOSTA:
        removeNode(tree, child, k);
	    return; //RESPOSTA:
    }
    
    writeFile(tree, child, child->pos);
    writeFile(tree, sibling, sibling->pos);

    free(child);
    free(sibling);
}


void removeNode(bTree* tree, bTreeNode* node, int k) {

    int idx = findKey(node, k);
    // The key to be removed is present in this node
    if (idx < node->noOfRecs && node->keyRecArr[idx]->key == k) {
        // If the node is a leaf node - removeFromLeaf is called
        // Otherwise, removeFromNonLeaf function is called
        if (node->isLeaf){
	        removeFromLeaf(tree, node, idx);
        } else {
            removeFromNonLeaf(tree, node, idx);
        }
        
	    writeFile(tree, node, node->pos);
    }
    else {
       
        // If this node is a leaf node, then the key is not present in tree
        if (node->isLeaf) {
		    return false;
       	}
 
        // The key to be removed is present in the sub-tree rooted with this node
        // The flag indicates whether the key is present in the sub-tree rooted
        // with the last child of this node
        bool flag = idx==node->noOfRecs;
 
        // If the child where the key is supposed to exist has less that t keys,
        // we fill that child

        bTreeNode *childAtPosi = malloc(sizeof(bTreeNode));
        readFile(tree, childAtPosi, node->children[idx]);

        if (childAtPosi->noOfRecs < t) {
            fill(tree, node, idx);
            //RESPOSTA: child pode ter sido atualizada
            readFile(tree, childAtPosi, node->children[idx]);
        }

        // If the last child has been merged, it must have merged with the previous
        // child and so we recurse on the (idx-1)th child. Else, we recurse on the
        // (idx)th child which now has atleast t keys
        if (flag && idx > node->noOfRecs) {
            bTreeNode *sibling = malloc(sizeof(bTreeNode));
            readFile(tree, sibling, node->children[idx-1]);
            removeNode(tree, sibling, k);

            writeFile(tree, sibling, sibling->pos);
            free(sibling);
        }
        else {
            removeNode(tree, childAtPosi, k);
        }
        
        writeFile(tree, childAtPosi, childAtPosi->pos);
        free(childAtPosi);
    }
}


// A function to get predecessor of keys[idx]
recordNode* getPred(bTree* tree, bTreeNode *node, int idx) {
    // Keep moving to the right most node until we reach a leaf
    bTreeNode *curr = malloc(sizeof(bTreeNode));
    readFile(tree, curr, node->children[idx]);

    while (!curr->isLeaf){
        readFile(tree, curr, curr->children[curr->noOfRecs]);
    }
        
    // Return the last key of the leaf
    recordNode* result = curr->keyRecArr[curr->noOfRecs-1];
    free(curr);
    return result;
}
 
recordNode* getSucc(bTree* tree, bTreeNode *node, int idx) {
 
    // Keep moving the left most node starting from C[idx+1] until we reach a leaf
    bTreeNode *curr = malloc(sizeof(bTreeNode));
    readFile(tree, curr, node->children[idx+1]); //RESPOSTA: children[idx+1]
    while (!curr->isLeaf){
        readFile(tree, curr, curr->children[0]);
    }
 
    // Return the first key of the leaf
    recordNode* result = curr->keyRecArr[0];
    free(curr);
    return result;
}
 
// A function to fill child C[idx] which has less than t-1 keys
void fill(bTree* tree, bTreeNode *node, int idx) {
    bTreeNode *cPrev = malloc(sizeof(bTreeNode));
    bTreeNode *cSucc = malloc(sizeof(bTreeNode));

    readFile(tree, cPrev, node->children[idx-1]);
    readFile(tree, cSucc, node->children[idx+1]);
    
    // If the previous child(C[idx-1]) has more than t-1 keys, borrow a key
    // from that child
    if (idx!=0 && cPrev->noOfRecs>=t) {
        borrowFromPrev(tree, node, idx);
    }
 
    // If the next child(C[idx+1]) has more than t-1 keys, borrow a key
    // from that child
    else if (idx!=node->noOfRecs && cSucc->noOfRecs>=t) {
        borrowFromNext(tree, node, idx);
    }
 
    // Merge C[idx] with its sibling
    // If C[idx] is the last child, merge it with with its previous sibling
    // Otherwise merge it with its next sibling
    else {
        if (idx != node->noOfRecs)
            merge(tree, node, idx);
        else
            merge(tree, node, idx-1);
    }

    free(cPrev);
    free(cSucc);

    return;
}
 
// A function to borrow a key from C[idx-1] and insert it
// into C[idx]
void borrowFromPrev(bTree* tree, bTreeNode *node, int idx) {
    bTreeNode *child = malloc(sizeof(bTreeNode));
    bTreeNode *sibling = malloc(sizeof(bTreeNode));
    
    readFile(tree, child, node->children[idx]);
    readFile(tree, sibling, node->children[idx-1]);
    
    // The last key from C[idx-1] goes up to the parent and key[idx-1]
    // from parent is inserted as the first key in C[idx]. Thus, the  loses
    // sibling one key and child gains one key
 
    // Moving all key in C[idx] one step ahead
    for (int i=child->noOfRecs-1; i>=0; --i)
        child->keyRecArr[i+1] = child->keyRecArr[i];
 
    // If C[idx] is not a leaf, move all its child pointers one step ahead
    if (!child->isLeaf) {
        for(int i=child->noOfRecs; i>=0; --i)
            child->children[i+1] = child->children[i];
    }
 
    // Setting child's first key equal to keys[idx-1] from the current node
    child->keyRecArr[0] = node->keyRecArr[idx-1];
 
    // Moving sibling's last child as C[idx]'s first child
    if (!node->isLeaf) {
        child->children[0] = sibling->children[sibling->noOfRecs];
        sibling->children[sibling->noOfRecs] = -1; //RESPOSTA:
    }
 
    // Moving the key from the sibling to the parent
    // This reduces the number of keys in the sibling
    node->keyRecArr[idx-1] = sibling->keyRecArr[sibling->noOfRecs-1];
 
    child->noOfRecs += 1;
    sibling->noOfRecs -= 1;
    
    writeFile(tree, node, node->pos);
    writeFile(tree, child, child->pos);
    writeFile(tree, sibling, sibling->pos);
    
    free(child);
    free(sibling);

    return;
}
 
// A function to borrow a key from the C[idx+1] and place
// it in C[idx]
void borrowFromNext(bTree* tree, bTreeNode *node, int idx) {
 
    bTreeNode *child = malloc(sizeof(bTreeNode));
    bTreeNode *sibling = malloc(sizeof(bTreeNode));
    
    readFile(tree, child, node->children[idx]);
    readFile(tree, sibling, node->children[idx+1]);
    
    // keys[idx] is inserted as the last key in C[idx]
    child->keyRecArr[(child->noOfRecs)] = node->keyRecArr[idx];
 
    // Sibling's first child is inserted as the last child
    // into C[idx]
    if (!(child->isLeaf))
        child->children[(child->noOfRecs)+1] = sibling->children[0];
 
    //The first key from sibling is inserted into keys[idx]
    node->keyRecArr[idx] = sibling->keyRecArr[0];
 
    // Moving all keys in sibling one step behind
    for (int i=1; i<sibling->noOfRecs; ++i)
        sibling->keyRecArr[i-1] = sibling->keyRecArr[i];
 
    // Moving the child pointers one step behind
    if (!sibling->isLeaf) {
        for(int i=1; i<=sibling->noOfRecs; ++i)
            sibling->children[i-1] = sibling->children[i];

        sibling->children[sibling->noOfRecs] = -1; //RESPOSTA:
    }
 
    // Increasing and decreasing the key count of C[idx] and C[idx+1]
    // respectively
    child->noOfRecs += 1;
    sibling->noOfRecs -= 1;
    
    writeFile(tree, node, node->pos);
    write_treedat(tree, child, child->pos);
    writeFile(tree, sibling, sibling->pos);

    free(child);
    free(sibling);

    return;
}
 
// A function to merge C[idx] with C[idx+1]
// C[idx+1] is freed after merging
bTreeNode* merge(bTree* tree, bTreeNode *node, int idx) {
    // RESOLUCAO: retorna o filho atualizado

    bTreeNode *child = malloc(sizeof(bTreeNode));
    bTreeNode *sibling = malloc(sizeof(bTreeNode));
    
    readFile(tree, child, node->children[idx]);
    read_treedat(tree, sibling, node->children[idx+1]);
    
    // Pulling a key from the current node and inserting it into (t-1)th
    // position of C[idx]
    child->keyRecArr[t-1] = node->keyRecArr[idx];
 
    // Copying the keys from C[idx+1] to C[idx] at the end
    for (int i=0; i<sibling->noOfRecs; ++i)
        child->keyRecArr[i+t] = sibling->keyRecArr[i];
 
    // Copying the child pointers from C[idx+1] to C[idx]
    if (!child->isLeaf) {
        for(int i=0; i<=sibling->noOfRecs; ++i)
            child->children[i+t] = sibling->children[i];
    }

    // Moving all keys after idx in the current node one step before -
    // to fill the gap created by moving keys[idx] to C[idx]
    for (int i=idx+1; i<node->noOfRecs; ++i)
        node->keyRecArr[i-1] = node->keyRecArr[i];
 
    // Moving the child pointers after (idx+1) in the current node one
    // step before
    for (int i=idx+2; i<=node->noOfRecs; ++i) 
        node->children[i-1] = node->children[i];
    node->children[node->noOfRecs] = -1; //RESPOSTA:
    // Updating the key count of child and the current node
    child->noOfRecs += sibling->noOfRecs+1;
    node->noOfRecs--;
 
    //RESPOSTA: se a raiz foi mergeada, agora teremos uma raiz "fantasma"
    if(node->noOfRecs == 0) {
        //Precisamos corrigir isso fazendo com que ela aponte para a raiz verdadeira
        tree->root = node->children[0];
    }

    // Freeing the memory occupied by sibling
    writeFile(tree, node, node->pos);
    writeFile(tree, child, child->pos);
    writeFile(tree, sibling, sibling->pos);

    free(sibling);

    return child;
}

bool removeFromTree(bTree* tree, int key) {
    bTreeNode *root = malloc(sizeof(bTreeNode));
    readFile(tree, root, tree->root);

    bool found = search(tree, key);
    if(found);
        removeNode(tree, root, key); 

    free(root);
    return found;
}

void hardPrint(bTree* tree) {
    bTreeNode* lido = (bTreeNode*) malloc(sizeof(bTreeNode));
    for(int i = 0; i < tree->nextPos; i++) {
        fseek(tree->fp, i * sizeof(bTreeNode), SEEK_SET);
        fread(lido, sizeof(bTreeNode), 1, tree->fp);

        if(lido->isLeaf <= 1)
            dispNode(lido);
        else
            printf("ERRO: isLeaf = %i\n\n", lido->isLeaf);
    }

    free(lido);
}

void doublePrint(bTree* tree) {
    printf("=================");
    printf("\nTraverse\n");
    traverse(tree, tree->root);

    printf("=================");
    printf("\nHard print\n");
    hardPrint(tree);
} */

/* Implementation to EP AED 2 _ pt 2 */


/* Le as operacoes inseridas no console */
/*void readOp(){
	char* line = fgets(stdin);
	char* op = strtok(line, " ");
	switch(op[0]){
		case "I":
			insertElement(op[1],op[2],op[3],op[4]);
		break;
		case "R":
			deleteElement(op[1]);
		break;
		case "B":
			searchElement(op[1]);
		break;
		case "P1":
			printIndices();
		break;
		case "P2":
			printConteudoIndices();
		break;
		case "P3":
			printData();
		break;
		case "F":
			ForceQuit();
		break;
		default:
			printf("Error: Invalid operator!");
		break;
	}
}


/* Metodos das Operacoes */
/*void insertElement(int cod, char* tit, char* aut, int ano){
	// Faz a insercao na arvore
	insert();
	// Faz a insercao no arquivo
}

void deleteElement(int cod){
	// busca o indice do item
	removeFromTree();
	// Insere # no indice deletado
}

int searchElement(int cod){
	// busca elemento
	searchRecursive();
	// retorna indice
}

void printIndices(){
	// percore indices e vai printando
	traverse();
}

void printConteudoIndices(){
	// percorre indices e printa conteudo 
	traverse();
}

void printData(){
	// percorre o arquivo e printa os dados
	traverse();
}

void ForceQuit(){
	// sai do looping
	quit = true;
}*/