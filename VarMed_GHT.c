#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<stdbool.h>

#define dimension 3
#define K 50
#define and &&
#define or ||
#define ll long long

                                                //defining the data types for the nodes and the data points

typedef struct data_point{

    double array[dimension];

    struct data_point * prev;
    struct data_point * next;

    char cluster[50];

} data_point;

                                                // nodes of tree

typedef struct node{

    int axis;
    double median;
    struct data_point* data_chain;

    int count;
    int equal_count;

    struct node* left_node;
    struct node* right_node;
    struct node* parent;

}node;

                                                // a function to create a node of the tree

node * Create_node(){
    node* new_node= (node*)malloc(sizeof(node));

    new_node->count=0;
    new_node->equal_count=0;

    new_node->data_chain=NULL;

    new_node->left_node=NULL;
    new_node->right_node=NULL;
    new_node->parent=NULL;

    new_node->axis=0;
    new_node->median=0;

    return new_node;
}
                                                // a function to create a new data point
data_point * Create_point(){

    data_point * new_point = (data_point*)malloc(sizeof(data_point));

    for(int i=0; i < dimension; ++i){
        new_point->array[i]=0;
    }

    for(int i=0; i<50; ++i){
        new_point->cluster[i]= '0';
    }

    new_point->next=NULL;
    new_point->prev=NULL;

    for(int i=0; i<dimension; ++i){
        new_point->array[i]=0;
    }

    return new_point;
}

                                                // Function to insert a single point into the node

bool Insert_point(node * child, data_point* point){

    if(child->data_chain == NULL){

        child->data_chain = point;
        point->next=NULL;

        return true;
    }

    point->next = child->data_chain;
    child->data_chain->prev = point;

    child->data_chain = point;

    return true;
}

                                                // Function to insert the data into the root at the begining

bool Insert_data(node * root){

    data_point *ptr = Create_point();

    //opening the file
    FILE *fptr = fopen("cluster_data.csv", "r");

    if(fptr == NULL){
        printf("ERROR occured in opening the file\n");

        return false;
    }

    char buffer[500];  // Buffer to store the first line

    // reading and ignoring the first line
    if (fgets(buffer, sizeof(buffer), fptr) == NULL){

        printf("File is empty or error reading first line.\n");

        fclose(fptr);

        return false;
    }

    // Creating a data point to read and store the first point
    data_point *point= Create_point();

    int read = fscanf(fptr, "%49[^,]", point->cluster);

        if( read != 1){
            printf("cluster-1 not read\n");

            return false;
        }

        for(int i=0; i<dimension; ++i){
            fscanf(fptr, ",%lf", &point->array[i]);
        }
    // assigning the first point to the data chain
    root->data_chain = point;
    root->count++;   

    while(!feof(fptr)){
        //initilaizing the read variable to measure the number of inputs read
        read=0;

        data_point * new_point = Create_point();

        //reading the cluster name
        int read = fscanf(fptr, "%49[^,]", new_point->cluster);

        if( read != 1){
            printf("cluster not read\n");

            return false;
        }
        read=0; // reinitializing read to measure the number of dimensions read

        for(int i=0; i<dimension; ++i){
            read += fscanf(fptr, ",%lf", &new_point->array[i]);
        }
        
        if(read != dimension){
            printf("ERROR: complete data has not been read or file format incorrect\n");

            return false;
        }

        // conneting the new point to its previous point
        point->next = new_point;
        new_point->prev = point;

        root->count++;

        point = new_point;
    }
    
    return true;
}

                                                // Hyperplane Generato :)
bool Hyperplane_Generator (node * root){

    /* write a seperate fuctions to calcualte variance, 
                                 to compare vareance along each axis and return the one with highest variance
                                 to calculate the median for a give array*/

}

bool Tree_Construct(node* root){
    
    /*thoroughly undersatnd how insert point function works and use it accordingly 
                            do check for the edge cases like the first point and the last point*/
}

data_point * Free_data_chain(data_point * point){
    
    if(point == NULL){
        printf("data chain is empty\n");

        return NULL;
    }
    while(point->next != NULL){

        point = point->next;

        free(point->prev);
        point->prev = NULL;
    }

    return NULL;
}

void Free_tree(node *root){

    if(root->left_node == NULL and root->right_node == NULL){

        root->data_chain = Free_data_chain(root->data_chain);

        free(root);

        return;
    }
    else{
        Free_tree(root->left_node);
        Free_tree(root->right_node);
    }
    free(root);

    return;
}

int Naive1=1;
int Naive2=1;
int Naive3=1;

void node_info_printer(FILE *fptr, node* root){

    fprintf(fptr, "Axis : ");
    
    if(root->axis!=0){
        
        fprintf(fptr, "x%d= %lf", root->axis, root->median);

    }
    else{
        fprintf(fptr, "NULL");
    }
    
    fprintf(fptr, "<br> count= %d <br> Equal_count= %d <br>", root->count, root->equal_count);

    return;
}

void node_printer( FILE *fptr, node* root){

    fprintf(fptr, "node-%d[",Naive1);
    node_info_printer(fptr, root);
    fprintf(fptr, "]\n");
    int x= Naive1;
    Naive1++;

    if(root->left_node != NULL){
        fprintf(fptr, "node-%d-->node-%d\n",x,Naive1);
        node_printer(fptr, root->left_node);
    }
    if(root->right_node != NULL){
        fprintf(fptr, "node-%d-->node-%d\n",x, Naive1);
        node_printer(fptr, root->right_node);
    }
    return;
}

bool Display_VarMed_Tree(node*root){

    FILE *fptr = fopen("VarMed_Tree.md","w");

    if(fptr == NULL){
        printf("ERROR: Opening the file\n");
        return false;
    }
    else{
        fprintf(fptr, "```mermaid\n");
        fprintf(fptr,"flowchart TD\n\n");
        node_printer(fptr, root);
        //pivot_printer(fptr, root);
        fprintf(fptr,"```");

        fclose(fptr);

    }
    return true;
}

int main(){
    
    node * root = Create_node();

    bool check;

    check = Insert_data(root);

    if(!check){
        return 0;
    }
    printf("root count = %d\n", root->count);

    Tree_Construct(root);

    check = Display_VarMed_Tree(root);
    if(check == 1){
        printf("printed to the file\n");
    }

    Free_tree(root);

    return 0;    
}