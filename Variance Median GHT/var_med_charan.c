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
