#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<stdbool.h>
#include<string.h>

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

    struct data_point* pivot_1;
    struct data_point* pivot_2;
    struct data_point* data_chain;

    int count;
    int equal_count;

    struct node* left_node;
    struct node* right_node;
    struct node* parent;
    double split_2quality;
    double split_3quality;

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

    new_node->split_2quality=-1;
    new_node->split_3quality= -1;

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

                                                // Defining the function find the Eucledean distance

double Distance(data_point * p1, data_point * p2){
    
    double sum=0;

    for(int i=0; i< dimension; ++i){

        sum += (p1->array[i]-p2->array[i])*(p1->array[i]- p2->array[i]);
    }
    return sqrt(sum);
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
    
    srand(time(NULL));

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

data_point* unit_vector(data_point * p1, data_point * p2){
    data_point * unit_point = Create_point();
    double sum2=0;

    for(int i=0; i< dimension; ++i){
        unit_point->array[i]= p2->array[i] - p1->array[i];
        sum2 += unit_point->array[i] * unit_point->array[i];
    }

    for(int i=0; i< dimension; ++i){
        unit_point->array[i] /= sqrt(sum2);
    }
    return unit_point;
}

double hyperplane(data_point * unit_point, data_point* mid_point, data_point* point){
    data_point* new_point= Create_point();

    for(int i=0; i< dimension; ++i){
        new_point->array[i] = point->array[i] - mid_point->array[i];
    }

    double projection=0;
    for(int i=0; i< dimension; ++i){
        projection += new_point->array[i] * unit_point->array[i];
    }
    free(new_point);
    return projection;
}
double absolute( double x){
    if(x>=0){
        return x;
    }
    else{
        return -x;
    }
}

                                                // Function to construct the Tree in a recurcive manner

bool Tree_Construct(node* root){

    if(root == NULL){
        printf("ERROR: NULL root given to Tree Constructor\n");
        return false;
    }

    int check;
    // calling the hyperplane generator on the root
    check = Hyperplane_Generator(root);

    node* left_child = Create_node();
    node* right_child = Create_node();

    // creating a pointer to the data points to iterate through the data chain
    data_point * point = root->data_chain;

    int loop =root->count;
    double lsum1=0;
    double lsum2=0;
    double rsum1=0;
    double rsum2=0;

    data_point *unit_point = unit_vector(root->pivot_1, root->pivot_2);
    data_point * mid_point = Create_point();
    for(int i=0; i<dimension; ++i){
        mid_point->array[i]= (root->pivot_1->array[i] + root->pivot_2->array[i])/2;
        // printf("%f, ", unit_point->array[i]);
    }
    // printf("\n");

    for (int i = 0; i < loop; ++i){
        if(point->next != NULL){
            // point is closer to pivot1
            if(Distance(point, root->pivot_1) > Distance(point, root->pivot_2)){ 
                
                point = point->next;
                Insert_point(left_child, point->prev);
                left_child->count++;

                double x = hyperplane(unit_point, mid_point, point -> prev);
                x = absolute(x);
                lsum1 += x;
                lsum2 += x*x;
                
            }
            // point is closer to pivot2
            else if(Distance(point, root->pivot_1) < Distance(point, root->pivot_2)){ 
                
                point = point->next;
                Insert_point(right_child, point->prev);
                right_child->count++;

                double x = hyperplane(unit_point, mid_point, point -> prev);
                x = absolute(x);
                rsum1 += x;
                rsum2 += x*x;                
            }
            // point is equiv distant from pivot1 and pivot2
            else if(Distance(point, root->pivot_1) == Distance(point, root->pivot_2)){ 

                root->equal_count++;

                if((root->equal_count)%2){
                    point = point->next;
                    Insert_point(left_child, point->prev);
                    left_child->count++;
                    
                    double x = hyperplane(unit_point, mid_point, point -> prev);
                    x = absolute(x);
                    lsum1 += x;
                    lsum2 += x*x;
                }
                else{
                    point = point->next;
                    Insert_point(right_child, point->prev);
                    right_child->count++;

                    double x = hyperplane(unit_point, mid_point, point -> prev);
                    x = absolute(x);
                    rsum1 += x;
                    rsum2 += x*x;  
                    
                }
            }
            
        }
    }
    // last point in the data chian

    // point is closer to pivot1
        if(Distance(point, root->pivot_1) > Distance(point, root->pivot_2)){ 
            
            Insert_point(left_child, point);
            left_child->count++;

            double x = hyperplane(unit_point, mid_point, point -> prev);
            x = absolute(x);
            lsum1 += x;
            lsum2 += x*x;
            
        }
        // point is closer to pivot2
        else if(Distance(point, root->pivot_1) < Distance(point, root->pivot_2)){ 
            
            Insert_point(right_child, point);
            right_child->count++;

            double x = hyperplane(unit_point, mid_point, point -> prev);
            x = absolute(x);
            rsum1 += x;
            rsum2 += x*x;  
            
        }
        // point is equiv distant from pivot1 and pivot2
        else if(Distance(point, root->pivot_1) == Distance(point, root->pivot_2)){ 

            root->equal_count++;

            if((root->equal_count)%2){

                Insert_point(left_child, point);
                left_child->count++;

                double x = hyperplane(unit_point, mid_point, point -> prev);
                x = absolute(x);
                lsum1 += x;
                lsum2 += x*x;
                
            }
            else{
                Insert_point(right_child, point);
                right_child->count++;

                double x = hyperplane(unit_point, mid_point, point -> prev);
                x = absolute(x);
                rsum1 += x;
                rsum2 += x*x;  
            }
        }

        free(unit_point);

    root->left_node = left_child;
    left_child->parent = root;
    double lavg = lsum1/left_child->count;
    double lvar = lsum2/left_child->count - lavg*lavg;
    double lstd = sqrt(lvar);
    // printf("lavg = %f, lstd= %f\n", lavg, lstd);
    left_child->split_2quality = lavg - 2*lstd;
    left_child->split_3quality = lavg - 3*lstd;

    root->right_node = right_child;
    right_child->parent = root;
    double ravg = rsum1/right_child->count;
    double rvar = rsum2/right_child->count - ravg*ravg;
    double rstd = sqrt(rvar);
    // printf("ravg = %f, rstd= %f\n", ravg, rstd);
    right_child->split_2quality= ravg - 2*rstd;
    right_child->split_3quality= ravg - 3*rstd;
    

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
            printf("ERROR: check is 0 on right child\n");
        }
    }
    
    return true;
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
    
    fprintf(fptr, "<br> count= %d <br> Equal_count= %d <br> Split 2Quality = %f <br> Split 3Quality = %f <br>", root->count, root->equal_count, root->split_2quality, root->split_3quality);
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

bool Display_Naive_Tree(node*root){

    FILE *fptr = fopen("Naive_Tree.md","w");

    if(fptr == NULL){
        printf("ERROR: Opening the file\n");
        return false;
    }
    else{
        fprintf(fptr, "```mermaid\n");
        fprintf(fptr,"flowchart\n\n");
        node_printer(fptr, root);
        //pivot_printer(fptr, root);
        fprintf(fptr,"```");

        fclose(fptr);

    }
    return true;
}

int max(int h1, int h2){
    if(h1 >= h2){
        return h1;
    }
    else
    return h2;
}
int count_nodes=0;
int count_leaf=0;
int sum2_leaf=0;
double split2_sum1=0;
double split3_sum1=0;
double split2_sum2=0;
double split3_sum2=0;

int height_of_tree(node * root){
    if(root == NULL){
        return 0;
    }
    
    int h=1, h1=0, h2=0;

    if(root->left_node != NULL){
        h1 = height_of_tree(root->left_node);
        count_nodes++;
        double s2=root->split_2quality;
        double s3= root->split_3quality;
        split2_sum1 += s2;
        split2_sum2 += s2*s2;

        split3_sum1 += s3;
        split3_sum2 += s3*s3;
    }

    if(root-> right_node != NULL){
        h2 = height_of_tree(root->right_node);
        count_nodes++;
        double s2=root->split_2quality;
        double s3= root->split_3quality;
        split2_sum1 += s2;
        split2_sum2 += s2*s2;

        split3_sum1 += s3;
        split3_sum2 += s3*s3;
    }
    if(root->left_node == NULL && root->right_node == NULL){
        count_leaf++;
        sum2_leaf += (root->count)*(root->count);
    }

    h += max(h1, h2);

    return h;
}
int sum_balance=0;
int balance_factor(node * root){
    
    if(root == NULL){
        return 0;
    }
    else{
        return abs(height_of_tree(root->left_node) - height_of_tree(root->right_node));
    }
}
int sum_of_balance_factor(node * root){
    if(root == NULL){
        return 0;
    }
    int bf = balance_factor(root);

    int suml = sum_of_balance_factor(root->left_node);
    int sumr = sum_of_balance_factor(root->right_node);

    return bf+sumr+suml;
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

    int h = height_of_tree(root);
    printf("Height = %d\n", h-1);

    printf("no. of nodes = %d\n", count_nodes+1);
    printf("no. of leaf nodes = %d\n", count_leaf);

    double average_leaf_size = (double)root->count/count_leaf;
    double variance_leaf = (double)(sum2_leaf)/count_leaf - (average_leaf_size)*(average_leaf_size);
    float std_dev_leaf = sqrt(variance_leaf);
    printf("Average leaf size = %f\n",average_leaf_size);
    printf("Stndard deviation of leaf size = %f\n", std_dev_leaf);

    double avg_split2 = split2_sum1/count_nodes;
    double var_split2 = split2_sum2/count_nodes - avg_split2*avg_split2;
    double std_split2 = sqrt(var_split2);
    printf("Average Split-2 quality= %f \nand Std_dev = %f\n", avg_split2, std_split2);

    double avg_split3 = split3_sum1/count_nodes;
    double var_split3 = split3_sum2/count_nodes - avg_split3*avg_split3;
    double std_split3 = sqrt(var_split3);
    printf("Average Split-3 quality= %f \nand Std_dev = %f\n", avg_split3, std_split3);
    
    
    check = Display_Naive_Tree(root);
    if(check == 1){
        printf("printed to the file\n");
    }

    Free_tree(root);

    return 0;    
}