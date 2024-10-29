#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<stdbool.h>

#define dimension 10
#define K 50
#define and &&
#define or ||
#define ll long long

                                                //defining the data types for the nodes and the data points

typedef struct data_point{

    double array[dimension];

    struct data_point * prev;
    struct data_point * next;

    struct node * cluster;

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

    new_node->pivot_1=NULL;
    new_node->pivot_2=NULL;

    return new_node;
}

                                                // Defining the function find the Eucledean distance

double Distance(data_point * p1, data_point * p2){
    
    double sum;

    for(int i=0; i< dimension; ++i){

        sum += (p1->array[i]-p2->array[i])*(p1->array[i]- p2->array[i]);
    }
    return sqrt(sum);
}

                                                // Function to insert a single point into the node

bool Insert_point(node * child, data_point* point){

    if(child->data_chain == NULL){

        child->data_chain = point;

        point->cluster = child;

        return true;
    }

    point->next = child->data_chain;
    child->data_chain->prev = point;

    child->data_chain = point;

    point->cluster = child;

    return true;
}

                                                // Function to insert the data into the root at the begining

bool Insert_data(node * root){

    /* we will be taking the input from a file and insert it into the root */

    /* We might split this Insrt_data dunction into further parts*/
}

                                                // Hyperplane Generato :)

bool Hyperplane_Generator (node * root){
    
    // If root is NULL
    if(root == NULL){
        printf("ERROR: NULL node input for hyperplane generator\n");
        return false;
    }

    int rand1;
    int rand2;
    
    rand1 = rand()%(root->count);
    // rand2 should not be equal to rand1
    do{ 
        rand2 = rand()%(root->count);
    }
    while (rand2 == rand1);

    // creating a pointer to the data points to iterate through the data chain
    data_point * point = root->data_chain;
    
    // finding the rand1 th position in the data chain
    for(int i=0; i< root->count; ++i){
        if(i == rand1){
            root->pivot_1 = point;
            break;
        }
        
        if(point == NULL){
            printf("ERROR: Hyperplane_Generator-1 in the loop= %d\n", i);
            printf("count = %d\n", root->count);
        }
        else{
            point = point->next;
        }
    }

    point = root->data_chain;
    
    // made a seperate loop for rand2 hoping that it might help in debugging, will merge them in future
    // finding the rand2 th position in the data chain
    for(int i=0; i< root->count; ++i){
        if(i == rand2){
            root->pivot_2 = point;
            break;
        }
        
        if(point == NULL){
            printf("ERROR: Hyperplane_Generator-2 in the loop= %d\n", i);
            printf("count = %d\n", root->count);
        }
        else{
            point = point->next;
        }
    }

    return true;
}

                                                // Function to construct the Tree in a recurcive manner

bool Tree_Construct(node* root){

    if(root == NULL){
        printf("ERROR: NULL root given to Tree Construct\n");
        return false;
    }

    int check;
    // calling the hyperplane generator on the root
    check = Hyperplane_Generator(root);

    node* left_child = Create_node();
    node* right_child = Create_node();

    // creating a pointer to the data points to iterate through the data chain
    data_point * point=root->data_chain;

    for (size_t i = 0; i < root->count; ++i){
        
        // point is closer to pivot1
        if(Distance(point, root->pivot_1) > Distance(point, root->pivot_2)){ 
            
            Insert_point(left_child, point);
            left_child->count++;
        }
        // point is closer to pivot2
        else if(Distance(point, root->pivot_1) < Distance(point, root->pivot_2)){ 
            
            Insert_point(right_child, point);
            right_child->count++;
        }
        // point is equiv distant from pivot1 and pivot2
        else if(Distance(point, root->pivot_1) == Distance(point, root->pivot_2)){ 

            root->equal_count++;

            if((root->equal_count)%2){

                Insert_point(left_child, point);
                left_child->count++;
            }
            else{
                Insert_point(right_child, point);
                right_child->count++;
            }
        }
    }

    // calling tree construct on left child
    if(left_child->count > 2*K){
        check = Tree_Construct(left_child);
        if(check == 0){
            printf("ERROR: check is 0 on left child\n");
        }
    }
    // calling tree child on right child
    if(right_child->count > 2*K){
        
        check = Tree_Construct(right_child);

        if(check == 0){
            printf("ERROR: check 0 on right child\n");
        }
    }
    
    return true;
}

int main(){
    
    node * root = Create_node();

    Insert_data(root);

    Tree_Construct(root);

    return;    
}