#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

#define dimension 3
#define K 50
#define and &&
#define or ||
#define ll long long

typedef struct data_point{

    double array[dimension];

    struct data_point * prev;
    struct data_point * next;

    char cluster[50];

} data_point;

                                                // nodes of tree

typedef struct node{

    struct data_point* pivot_1;
    struct data_point* pivot_2;
    struct data_point* data_chain;

    int count;
    int equal_count;

    struct node* left_node;
    struct node* right_node;
    struct node* parent;

} node;


node * Create_node(){
    node* new_node= (node*)malloc(sizeof(node));

    new_node->count=0;
    new_node->equal_count=0;

    new_node->data_chain=NULL;

    new_node->left_node=NULL;
    new_node->right_node=NULL;
    new_node->parent=NULL;

    new_node->pivot_1=NULL;
    new_node->pivot_2=NULL;

    return new_node;
}

data_point * Create_point(){

    data_point * new_point = (data_point*)malloc(sizeof(data_point));

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

int Naive1=1;
int Naive2=1;
int Naive3=1;


void node_info_printer(FILE *fptr, node* root){

    fprintf(fptr, "p%d : ", Naive2);
    
    if(root->pivot_1!=NULL){
        for(int i=0; i< dimension-1; ++i){
            fprintf(fptr, "%.2lf, ", root->pivot_1->array[i]);
        }
        fprintf(fptr, "%.2lf", root->pivot_1->array[dimension-1]);

    }
    else{
        fprintf(fptr, "NULL");
    }
    
    fprintf(fptr, "<br> count= %d <br> Equal_count= %d <br>", root->count, root->equal_count);
    Naive2++;

    fprintf(fptr, "p%d : ", Naive2);

    if(root->pivot_2 != NULL){

        for(int i=0; i< dimension-1; ++i){
            fprintf(fptr, "%.2lf, ", root->pivot_2->array[i]);
        }
        fprintf(fptr, "%.2lf", root->pivot_2->array[dimension-1]);
    }
    else{
        fprintf(fptr, "NULL");
    }
    Naive2++;

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
//----------------------------------pivot printer is not required anymore
// void pivot_printer(FILE *fptr, node * root){

//     fprintf(fptr, "p%d :", Naive3);

//     for(int i=0; i< dimension-1; ++i){
//         fprintf(fptr, "%.2lf, ", root->pivot_1->array[i]);
//     }
//     fprintf(fptr, "%.2lf\n", root->pivot_1->array[dimension-1]);

//     Naive3++;

//     fprintf(fptr, "p%d : ", Naive3);

//     for(int i=0; i< dimension-1; ++i){
//         fprintf(fptr, "%.2lf, ", root->pivot_2->array[i]);
//     }
//     fprintf(fptr, "%.2lf\n", root->pivot_2->array[dimension-1]); 

//     if(root->left_node != NULL){
//         pivot_printer(fptr, root->left_node);
//     }
//     if(root->right_node != NULL){
//         pivot_printer(fptr, root->right_node);
//     }
// }

void Display_Naive_Tree(node*root){

    FILE *fptr = fopen("Tree.md","w");

    if(fptr == NULL){
        printf("ERROR: Opening the file\n");
    }
    else{
        fprintf(fptr, "```mermaid\n");
        fprintf(fptr,"flowchart\n\n");
        node_printer(fptr, root);
        //pivot_printer(fptr, root);
        fprintf(fptr,"```");

        fclose(fptr);

    }
    return;
}

int main(){

    node * root = Create_node();
    root->count=5;
    root->equal_count=0;

    data_point * ptr1 = Create_point();
    data_point * ptr2 = Create_point();

    ptr1->array[0]=1;
    ptr1->array[1]=2;
    ptr1->array[2]=3;

    ptr2->array[0]=4;
    ptr2->array[1]=5;
    ptr2->array[2]=6;

    root->pivot_1=ptr1;
    root->pivot_2=ptr2;

    root->left_node=Create_node();
    root->right_node= Create_node();
    root->left_node->count=7;
    root->right_node->count=8;

    Display_Naive_Tree(root);

    free(ptr1);
    free(ptr2);
    free(root->left_node);
    free(root->right_node);
    free(root);  

    return 0;
}