#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<stdbool.h>
#include<string.h>
#include<float.h>

#define dimension 10
#define K 40
#define and &&
#define or ||
#define ll long long
typedef struct {
    double *coords;
    int cluster;
} Point;

typedef struct {
    double *coords;
    int count;
} Centroid;

double euclidean_distance(double *a, double *b, int d) {
    double sum = 0;
    for (int i = 0; i < d; i++) {
        double diff = a[i] - b[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}

void initialize_centroids(Point *points, Centroid *centroids, int n, int k, int d) {
    srand(100);
    int *chosen = calloc(n, sizeof(int));
    for (int i = 0; i < k; i++) {
        int idx;
        do {
            idx = rand() % n;
        } while (chosen[idx]);
        chosen[idx] = 1;
        for (int j = 0; j < d; j++) {
            centroids[i].coords[j] = points[idx].coords[j];
        }
    }
    free(chosen);
}

void assign_clusters(Point *points, Centroid *centroids, int n, int k, int d) {
    for (int i = 0; i < n; i++) {
        double min_dist = DBL_MAX;
        int closest = 0;
        for (int j = 0; j < k; j++) {
            double dist = euclidean_distance(points[i].coords, centroids[j].coords, d);
            if (dist < min_dist) {
                min_dist = dist;
                closest = j;
            }
        }
        points[i].cluster = closest;
    }
}

int update_centroids(Point *points, Centroid *centroids, int n, int k, int d) {
    int changed = 0;

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < d; j++)
            centroids[i].coords[j] = 0;
        centroids[i].count = 0;
    }

    for (int i = 0; i < n; i++) {
        int c = points[i].cluster;
        for (int j = 0; j < d; j++) {
            centroids[c].coords[j] += points[i].coords[j];
        }
        centroids[c].count++;
    }

    for (int i = 0; i < k; i++) {
        if (centroids[i].count == 0) continue;
        for (int j = 0; j < d; j++) {
            double new_val = centroids[i].coords[j] / centroids[i].count;
            if (fabs(new_val - centroids[i].coords[j]) > 1e-6) changed = 1;
            centroids[i].coords[j] = new_val;
        }
    }

    return changed;
}

void kmeans(Point *points, int n, int d, int k, int max_iters, double **final_centroids) {
    Centroid *centroids = malloc(k * sizeof(Centroid));
    for (int i = 0; i < k; i++) {
        centroids[i].coords = calloc(d, sizeof(double));
    }

    initialize_centroids(points, centroids, n, k, d);

    for (int iter = 0; iter < max_iters; iter++) {
        assign_clusters(points, centroids, n, k, d);
        if (!update_centroids(points, centroids, n, k, d)) {
            printf("Converged at iteration %d\n", iter);
            break;
        }
    }

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < d; j++) {
            final_centroids[i][j] = centroids[i].coords[j];
        }
    }

    for (int i = 0; i < k; i++) {
        free(centroids[i].coords);
    }
    free(centroids);
}

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

    new_node->axis=-1;
    new_node->median=0;

    new_node->split_2quality= -1;
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

    int read = fscanf(fptr, "%*[^\n]\n");
    if (read != 0) {
        printf("Error skipping the first line.\n");
        fclose(fptr);
        return false;
    }

    // Creating a data point to read and store the first point
    data_point *point= Create_point();

    read = fscanf(fptr, "%49[^,]", point->cluster);
    // printf("%s\n",point->cluster);
    if( read != 1){
        printf("cluster-1 not read\n");
        fclose(fptr);
        free(point);
        return false;
    }
    // printf("%s\n", point->cluster);

    for(int i=0; i<dimension; ++i){
        read += fscanf(fptr, ",%lf", &point->array[i]);
    }
    if (read != dimension + 1){
        for(int i=0; i<dimension-1;++i){
            printf("%f, ",point->array[i]);
        }
        printf("%f\n",point->array[dimension-1]);
        printf("ERROR-1: complete data has not been read or file format incorrect\n");
        fclose(fptr);
        free(point);
        return false;
    }

    // assigning the first point to the data chain
    root->data_chain = point;
    root->count++;   

    while(true){
        //initilaizing the read variable to measure the number of inputs read
        read=0;

        data_point * new_point = Create_point();

        //reading the cluster name
        int read = fscanf(fptr, "\n%49[^,]", new_point->cluster);
        // printf("%s\n",point->cluster);

        if (read != 1) {
            if (feof(fptr)){
                break;
            } 
            else{
                printf("cluster-11 not read\n");
                fclose(fptr);
                free(point);
                free(new_point);
                return false;
            }
        }
        read=0; // reinitializing read to measure the number of dimensions read

        for(int i=0; i<dimension; ++i){
            read += fscanf(fptr, ",%lf", &new_point->array[i]);
        }
        
        if(read != dimension){
            printf("ERROR: complete data has not been read or file format incorrect\n");
            fclose(fptr);
            free(point);
            free(new_point);
            return false;
        }

        // conneting the new point to its previous point
        point->next = new_point;
        new_point->prev = point;

        root->count++;

        point = new_point;
    }
    fclose(fptr);
    return true;
}
                                                // Hyperplane Generato :)

                                            //for sorting
 int comp (const void * elem1, const void * elem2) 
{
    double f = *((double*)elem1);
    double s = *((double*)elem2);
    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}


int return_dimension_number_of_max_variance(node *new_node){
    double sum[dimension] = {0};
    double sum_squared[dimension] = {0};
    data_point *dummy_point = new_node->data_chain;
    int total_number = 0;
    while((dummy_point ) != NULL){
        for(int j=0;j<dimension;j++){
            sum[j]+=dummy_point->array[j];
            sum_squared[j]+=(dummy_point->array[j])*(dummy_point->array[j]);
        }
        total_number++;
        dummy_point = dummy_point->next;
    }

    for(int j=0;j<dimension;j++){
        sum[j] = sum[j]/total_number;
        sum_squared[j] = sum_squared[j]/total_number;
    }

    for(int j=0;j<dimension;j++){
        sum_squared[j] = sum_squared[j]-(sum[j])*(sum[j]);
    }

    int max_variance = sum_squared[0], max_variance_dimension = 0;
    for(int j=1;j<dimension;j++){
        if(sum_squared[j]>max_variance){
            max_variance = sum_squared[j];
            max_variance_dimension = j;
        }
    }

    return max_variance_dimension;
}

double return_median_and_others(int max_variance_dimension, node *new_node){
    data_point *dummy_point = new_node->data_chain;
    data_point *dummy_point_2 = new_node->data_chain;
    int total_number = 0;
    while(dummy_point != NULL){
        total_number++;
        dummy_point = dummy_point->next;
    }
    double temp_array[total_number];
    for(int j = 0;j<total_number;j++){
        temp_array[j] = dummy_point_2->array[max_variance_dimension];
        dummy_point_2 = dummy_point_2->next;
    }
    qsort(temp_array, sizeof(temp_array)/sizeof(*temp_array),sizeof(*temp_array), comp);
    
    if(total_number %2 == 0){
        return (temp_array[total_number/2 -1] + temp_array[total_number/2])/2;
    }

    else return temp_array[total_number/2];
}

bool Hyperplane_Generator (node * root){

    // If root is NULL
    if(root == NULL){
        printf("ERROR: NULL node input for hyperplane generator\n");
        return false;
    }
    int max_var = return_dimension_number_of_max_variance(root);
    data_point * ptr = root->data_chain;
    int n = root->count;
    Point *points = malloc(n * sizeof(Point));

    for (int i = 0; i < n; i++) {
        points[i].coords = malloc(sizeof(double));
        
        points[i].coords[0] = ptr->array[max_var];
        
        ptr = ptr->next;
    }
    double **centroids = malloc(2 * sizeof(double *));
    for (int i = 0; i < 2; i++)
        centroids[i] = malloc( sizeof(double));

    kmeans(points, n, 1, 2, 100, centroids);
    
    
    double p1 = centroids[0][0];
    double p2 = centroids[1][0];
    
    root->axis = max_var;
    root->median = (p1+p2)/2;

    for (int i = 0; i < n; i++)
        free(points[i].coords);
    free(points);

    for (int i = 0; i < 2; i++)
        free(centroids[i]);
    free(centroids);
    return true;
}
double absolute( double x){
    if(x>=0){
        return x;
    }
    else{
        return -x;
    }
}
double Distance(data_point * p1, data_point * p2){
    
    double sum=0;

    for(int i=0; i< dimension; ++i){

        sum += (p1->array[i]-p2->array[i])*(p1->array[i]- p2->array[i]);
    }
    return sqrt(sum);
}

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

    int dim= root->axis;
    double median = root->median;

    for (int i = 0; i < loop; ++i){
        if(point->next != NULL){
            // point is closer to pivot1
            if(point->array[dim] < median){ 
                
                point = point->next;
                Insert_point(left_child, point->prev);
                left_child->count++;

                double x = point->array[dim] - median;
                x = absolute(x);
                lsum1 += x;
                lsum2 += x*x;
                
            }
            // point is closer to pivot2
            else if(point->array[dim] > median){ 
                
                point = point->next;
                Insert_point(right_child, point->prev);
                right_child->count++;

                double x = point->array[dim] - median;
                x = absolute(x);
                rsum1 += x;
                rsum2 += x*x;                
            }
            // point is equiv distant from pivot1 and pivot2
            else if(point->array[dim] == median){ 

                root->equal_count++;

                if((root->equal_count)%2){
                    point = point->next;
                    Insert_point(left_child, point->prev);
                    left_child->count++;
                    
                    double x = point->array[dim] -median;
                    x = absolute(x);
                    lsum1 += x;
                    lsum2 += x*x;
                }
                else{
                    point = point->next;
                    Insert_point(right_child, point->prev);
                    right_child->count++;

                    double x = point->array[dim] - median;
                    x = absolute(x);
                    rsum1 += x;
                    rsum2 += x*x;  
                    
                }
            } 
        }
    }
    // last point in the data chian

    // point is closer to pivot1
        if(point->array[dim] < median){ 
            
            Insert_point(left_child, point);
            left_child->count++;

            double x = point->array[dim] - median;
            x = absolute(x);
            lsum1 += x;
            lsum2 += x*x;
            
        }
        // point is closer to pivot2
        else if(point->array[dim] > median){ 
            
            Insert_point(right_child, point);
            right_child->count++;

            double x = point->array[dim] < median;
            x = absolute(x);
            rsum1 += x;
            rsum2 += x*x;  
            
        }
        // point is equiv distant from pivot1 and pivot2
        else if(point->array[dim] == median){ 

            root->equal_count++;

            if((root->equal_count)%2){

                Insert_point(left_child, point);
                left_child->count++;

                double x = point->array[dim] - median;
                x = absolute(x);
                lsum1 += x;
                lsum2 += x*x;
                
            }
            else{
                Insert_point(right_child, point);
                right_child->count++;

                double x = point->array[dim] - median;
                x = absolute(x);
                rsum1 += x;
                rsum2 += x*x;  
            }
        }

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

int D1_kmeans1=1;
int D1_kmeans2=1;
int D1_kmeans3=1;

void node_info_printer(FILE *fptr, node* root){

    fprintf(fptr, "Axis : ");
    // printf("axis: %d, median: %.2f\n",root->axis, root->median);
    if(root->axis!=-1){
        
        fprintf(fptr, "x%d= %lf", root->axis+1, root->median);

    }
    else{
        fprintf(fptr, "NULL");
    }
    
    fprintf(fptr, "<br> count= %d <br> Equal_count= %d <br> Split 2Quality = %f <br> Split 3Quality = %f <br>", root->count, root->equal_count, root->split_2quality, root->split_3quality);

    return;
}

void node_printer( FILE *fptr, node* root){

    fprintf(fptr, "node-%d[",D1_kmeans1);
    node_info_printer(fptr, root);
    fprintf(fptr, "]\n");
    int x= D1_kmeans1;
    D1_kmeans1++;

    if(root->left_node != NULL){
        fprintf(fptr, "node-%d-->node-%d\n",x,D1_kmeans1);
        node_printer(fptr, root->left_node);
    }
    if(root->right_node != NULL){
        fprintf(fptr, "node-%d-->node-%d\n",x, D1_kmeans1);
        node_printer(fptr, root->right_node);
    }
    return;
}

bool Display_D1_kmeans_Tree(node*root){

    FILE *fptr = fopen("D1_kmeans_Tree.md","w");

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

char* determine_cluster(node* leaf_node) {
    
    if (!leaf_node || !leaf_node->data_chain) {
        return NULL;
    }
    
    data_point* current_point = leaf_node->data_chain;
    char* common_cluster = current_point->cluster;
    int max_count = 0;
    
    while (current_point) {
        int count = 0;
        data_point* point = leaf_node->data_chain;
        while (point) {
            if (strcmp(current_point->cluster,point->cluster)==0) {
                count++;
            }
            point = point->next;
        }
        if (count > max_count) {
            max_count = count;
            common_cluster = current_point->cluster;
        }
        current_point = current_point->next;
    }
    return common_cluster;
}

void distinct_clusters(data_point *data_chain, char ***list_ptr, int *counter_ptr) {
    
    *list_ptr = (char **)malloc(sizeof(char *));
    *counter_ptr = 1;
    (*list_ptr)[0] = strdup(data_chain->cluster);
    
    
    data_point *ptr = data_chain->next;
    while (ptr) {
        int found = 0;
        for (int j = 0; j < *counter_ptr; ++j) {
            if (strcmp((*list_ptr)[j], ptr->cluster) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            (*counter_ptr)++;
            *list_ptr = realloc(*list_ptr, (*counter_ptr) * sizeof(char *));
            (*list_ptr)[(*counter_ptr) - 1] = strdup(ptr->cluster);
            
        }
        ptr = ptr->next;
    }
}

char * weighted_vote_determine_cluster(node * leaf_node, data_point * new_point){
    double factors [leaf_node->count];
    
    int size = leaf_node->count;
    
    char **list = NULL;
    int counter = 0;
    distinct_clusters(leaf_node->data_chain, &list, &counter);
    // double sum_num=0;
    // double sum_den=0;
    
    data_point * ptr = leaf_node->data_chain;
    int i=0;
    
    for (ptr = leaf_node->data_chain; ptr != NULL && i < size; ptr = ptr->next, ++i) {
        double distance = Distance(ptr, new_point);
        if (distance < 0) {
            printf("Something went wrong in the new Cluster determiner\n");
        }
        factors[i] = 100 / (distance + 1);
    }
    
    double weighted_vote[counter];
    
    for(int i =0; i<counter; ++i){
        weighted_vote[i] = 0;
        ptr = leaf_node->data_chain;
        for(int j=0; j< leaf_node->count; ++j){
            if(ptr != NULL){
                if(strcmp(list[i], ptr->cluster) ==0 ){
                    weighted_vote[i] += factors[j];
                }
                ptr = ptr->next;
            }
        }
    }
    double max_votes = weighted_vote[0];
    char * most_freq_cluster = list[0];
    for(int i =0; i<counter; ++i){
        if(weighted_vote[i] > max_votes){
            max_votes = weighted_vote[i];
            most_freq_cluster = list[i];
        }
    }
    char *result = strdup(most_freq_cluster);
    
    for (int i = 0; i < counter; ++i) {
        free(list[i]);
    }
    free(list);
    return result;
    
}


int Insert_new_point(node* root, data_point* new_point) {
    
    if (!root || !new_point) {
        return false;
    }
    node* current_node = root;
    current_node-> count++;
    
    while(current_node->left_node && current_node->right_node) {
        int axis = current_node->axis;
        
        if(new_point->array[axis] < current_node->median){
            current_node= current_node->left_node;
        }
        else if(new_point->array[axis] > current_node->median){
            current_node = current_node->right_node;
        }
        else{
            current_node->equal_count++;
            if(current_node->equal_count%2){
                current_node = current_node->left_node;
            }
            else{
                current_node = current_node->right_node;
            }
        }
    }
    
    int return_value=0;
    char* assigned_cluster = determine_cluster(current_node);
    char* new_assigned_cluster = weighted_vote_determine_cluster(current_node, new_point);
    // printf("cluster = %s and assigned = %s\n", new_point->cluster, assigned_cluster);
    
    if(!Insert_point(current_node, new_point)){
        return false;
    }
    current_node->count++;
    
    assigned_cluster = new_assigned_cluster;
    
    if(assigned_cluster != NULL){
        if(!(strcmp(new_point->cluster, assigned_cluster)==0)){
            if(current_node->count > 2 * K){
                int read= Tree_Construct(current_node);
                if(read!=1){
                    printf("construction went wrong at 1\n");
                }
            }
            // printf("cluster = %s and assigned = %s, new assigned = %s\n", new_point->cluster, assigned_cluster, new_assigned_cluster);
            // printf("Assign= %s, found= %s \n",assigned_cluster, new_point->cluster);
            return 2;
        }
    }
    
    if(current_node->count > 2 * K){
        int read= Tree_Construct(current_node);
        if(read!=1){
            printf("construction went wrong at 1\n");
        }
    }
    return true;
}

int jojo=1;
void print_pivot(node * root, FILE* file){
    if(root == NULL || root->left_node==NULL){
        return;
    }
    if(root->left_node!=NULL){
        fprintf(file, "%d,",jojo);
        jojo++;
        fprintf(file, "%d,%f\n",root->axis,root->median);

        print_pivot(root->left_node, file);
        print_pivot(root->right_node,file);

    }
}
double Test(node* root, FILE * file){
    
    if(file == NULL){
        printf("ERROR occured in opening the file\n");
        
        return false;
    }
    int counter =0;
    int positive =0;
    int negative =0;
    
    for(int i=0;!feof(file);++i){
        int read =0;
        data_point * new_point = Create_point();
        if(i==0){
            read = fscanf(file, "%*[^\n]");
            if (read != 0) {
                printf("Error skipping the first line.\n");
                fclose(file);
                return -1;
            }
            continue;
        }
        else{
            read = fscanf(file, "\n%49[^,]", new_point->cluster);
        }
        if( read != 1){
            printf("Test cluster not read\n");
            
            return false;
        }
        
        read=0; // reinitializing read to measure the number of dimensions read
        
        for(int i=0; i<dimension; ++i){
            read += fscanf(file, ",%lf", &new_point->array[i]);
        }
        
        if(read != dimension){
            printf("ERROR: complete data has not been read or file format incorrect and read = %d\n", read);
            
            free(new_point);
            return false;
        }
        read =0;
        read = Insert_new_point(root, new_point);
        if(read == 1){
            positive++;
            counter++;
        }
        else if(read == 2){
    
            negative++;
            counter++;
        }
        else{
            printf("Error occured\n");
        }
    }
    
    double accuracy = ((double) positive/counter)*100;
    return accuracy;
}



bool is_not_empty(FILE * file){
    fseek(file, 0, SEEK_END);
    long size = ftell(file);

    if (size == 0) {
        fclose(file);
        return false;
    }
    rewind(file);
    return true;
}

int main(){
    double total_build_time = 0; 
    
    double total_test_time[3] = {0};
    int test_runs[3] = {0}; 
    
    
    bool check;
    int n=1;
    int h=0;
    double avg_average_leaf_size=0;
    double avg_std_dev_leaf =0;
    double avg_avg_split2 =0;
    double avg_std_split2 =0;
    double avg_avg_split3 =0;
    double avg_std_split3=0;
    double avg_aim[3]={0,0,0};
    int count=0;
    int num_nodes =0;
    int num_leaves=0;

    for(int i=0; i<n;++i){
        
        clock_t build_start, build_end;
        double cpu_time_used;

        clock_t test_start[3], test_end[3];
        double test_time[3] = {0};
        
        build_start = clock();
        node * root = Create_node();

        check = Insert_data(root);
        if(!check){
            return 0;
        }
        Tree_Construct(root);
        build_end = clock();
        double build_time = ((double)(build_end - build_start)) / CLOCKS_PER_SEC;
        total_build_time += build_time;

        int test[3] = {0};
        double accuracy[3] = {0};

        FILE * test1 = fopen("test-1.csv", "r");
        if(is_not_empty(test1)){
            test_start[0] = clock();
            accuracy[0] = Test(root, test1);
            test_end[0] = clock();
            test_time[0] = ((double)(test_end[0] - test_start[0])) / CLOCKS_PER_SEC;
        }
        test[0] = is_not_empty(test1);
        fclose(test1);

        FILE * test2 = fopen("test-2.csv", "r");
        if(is_not_empty(test2)){
            test_start[1] = clock();
            accuracy[1] = Test(root, test2);
            test_end[1] = clock();
            test_time[1] = ((double)(test_end[1] - test_start[1])) / CLOCKS_PER_SEC;
        }
        test[1] = is_not_empty(test2);
        fclose(test2);


        FILE * test3 = fopen("test-3.csv", "r");
        if(is_not_empty(test3)){
           test_start[2] = clock();
           accuracy[2] = Test(root, test3);
           test_end[2] = clock();
           test_time[2] = ((double)(test_end[2] - test_start[2])) / CLOCKS_PER_SEC;
        }
        test[2] = is_not_empty(test3);
        fclose(test3);

        if (test[0]) {
            total_test_time[0] += test_time[0];
            test_runs[0]++;
        }
        if (test[1]) {
            total_test_time[1] += test_time[1];
            test_runs[1]++;
        }
        if (test[2]) {
            total_test_time[2] += test_time[2];
            test_runs[2]++;
        }

        
        count += root->count;
        for(int a=0; a<3; ++a){
            avg_aim[a] += accuracy[a];//-----------------------------------------------------
        }

        h += height_of_tree(root);

        double average_leaf_size = (double)root->count/count_leaf;
        double variance_leaf = (double)(sum2_leaf)/count_leaf - (average_leaf_size)*(average_leaf_size);
        double std_dev_leaf = sqrt(variance_leaf);

        avg_average_leaf_size += average_leaf_size;
        avg_std_dev_leaf += std_dev_leaf;

        double avg_split2 = split2_sum1/count_nodes;
        double var_split2 = split2_sum2/count_nodes - avg_split2*avg_split2;
        double std_split2 = sqrt(var_split2);

        avg_avg_split2 += avg_split2;
        avg_std_split2 += std_split2;

        double avg_split3 = split3_sum1/count_nodes;
        double var_split3 = split3_sum2/count_nodes - avg_split3*avg_split3;
        double std_split3 = sqrt(var_split3);

        avg_avg_split3 += avg_split3;
        avg_std_split3 += std_split3;

        num_nodes += count_nodes+1;
        num_leaves += count_leaf;

        FILE * file = fopen("D1_kmeans_pivots.csv","w");
        if(file != NULL){
            print_pivot(root, file);
            fclose(file);
        }
        else{
            printf("ERROR opening the file pivots.csv\n");
        }
        check = Display_D1_kmeans_Tree(root);
        Free_tree(root);
        count_nodes=0;
        count_leaf=0;
        sum2_leaf=0;
        split2_sum1=0;
        split3_sum1=0;
        split2_sum2=0;
        split3_sum2=0;
    }
    for(int a=0; a<3; ++a){

        avg_aim[a] /= n; //-------------------------------------------
    }
    num_leaves /=n;
    num_nodes /=n;
    count /= n;
    h /=n;
    avg_average_leaf_size /=n;
    avg_std_dev_leaf /=n;
    avg_avg_split2 /= n;
    avg_std_split2 /= n;
    avg_avg_split3 /= n;
    avg_std_split3 /= n;

    printf("root count = %d\n", count);
    printf("Height = %d\n", h-1);   
    printf("no. of nodes = %d\n\n", num_nodes+1);
    printf("no. of leaf nodes = %d\n", num_leaves);
    printf("Average leaf size = %f\n",avg_average_leaf_size);

    printf("Stndard deviation of leaf size = %f\n\n", avg_std_dev_leaf);
    printf("Average Split-2 quality= %f \nand Std_dev = %f\n\n", avg_avg_split2, avg_std_split2);
    printf("Average Split-3 quality= %f \nand Std_dev = %f\n\n", avg_avg_split3, avg_std_split3);
    for(int i=0; i<3;++i){
        printf("Test-%d data accuracy = %f\n", i+1, avg_aim[i]); //-------------------------------------------
    }
    if(check == 1){
        printf("printed to the file\n");
    }
    if (n!=1){
        printf("\nAverage execution times over %d run(s):\n", n);
    }
    printf("\nBuild time: %f seconds\n", total_build_time / n);

    for (int i = 0; i < 3; ++i) {
        if (test_runs[i]) {
            printf("Test-%d time: %f seconds \n", i + 1, total_test_time[i] / test_runs[i]);
        } else {
            printf("Test-%d was skipped (empty file).%d\n", i + 1, test_runs[i]);
        }
    }

    return 0;    
}