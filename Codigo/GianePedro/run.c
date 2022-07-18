#include "header.h"
int main(int argc, char const *argv[])
{

	if(argc==1)
	{
		printf("Please enter one of the options below:\n");
		printf("Commands:\n");
		printf("./run [OPTIONS]\n");
		printf("-b [KEY] -->for building a tree.\n");
		printf("-s [KEY] -->for searching a [KEY].\n");
		printf("-d [KEY] -->for deleting a [KEY].\n");
		printf("Exiting now\n");
		exit(0);
	}
	
	int len,i;
    printf("How many records do you want to build from dataset?");
	scanf("%d",&len);

    recordNode *records = node_read("data/dataset.csv", len);
    bTree* tree = createTree("tree.dat", NULL, false);
    
    for(i=0;i<len;i++)
    {
    	insert(tree,&records[i]);
    }

    if(!strcmp(argv[1],"-d"))
    {
	    int key;
	    sscanf(argv[2],"%d",&key);
	    bool res = removeFromTree(tree,key);
    	if(res)
    	{
    		printf("Successfull Deletion.\n");
    	}
    	else
    	{
    		printf("Deletion not successful.\n");
    	}
    }

        traverse(tree, tree->root);

	if(!strcmp(argv[1],"-s"))
    {
	    // printf("Time Taken to build tree: %f seconds\n",timeToBuild);
	    int key;
	    sscanf(argv[2],"%d",&key);
	    recordNode* res = search(tree,key);

		if(res != NULL) {
			printf("key\tcountry\tgrate\tscore\trate\n");
			printf("%s\t",res->codigoLivro );
			printf("%s\t",res->codigoLivro );
			printf("%s\t",res->titulo );
			printf("%s\t",res->nomeCompletoPrimeiroAutor );
			printf("%d\n",res->anoPublicacao );

			free(res);
		} else
			printf("Record not found!");
    }

	free(records);
	free(tree);
}	
