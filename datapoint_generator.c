#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<time.h>

#define min_treshold 10
#define threshold 50
int clusters=4;
int N=1000;

typedef struct {
    double *coordinates;
} Point;


// function to find the square root of sum of square of all the coordinates, which will be used in calculation of direction cosine
double Direction_cosine(double *c, int dimension){
    
    double square_sum=0;

    for(int i=0; i< dimension; ++i){

        square_sum += (c[i])*(c[i]);
    }
    return sqrt(square_sum);
}

// function to calculate the distance between two points
double distance( double * p1, double *p2, int dimension){
    double square_sum=0;

    for(int i=0; i< dimension; ++i){

        square_sum += (p1[i]- p2[i])*(p1[i]-p2[i]);
    }
    return sqrt(square_sum);
}

int minimum(double * array){
    double min=array[0];
    int pos=0;
    for(int i=1; i< clusters; ++i){
        if(min > array[i]){
            min = array[i];
            pos =i;
        }
    }
    return pos;
}

//function to priint all the distances
void Radii_finalizer (Point * centers, int dimension, double* cluster_radii, int *percentages){
    // double *cluster_radii= malloc(clusters*sizeof(double));

    for(int i=0; i< clusters; ++i){

        double cluster_dist[clusters];
        printf("\n-------------------cluster %d-------------------\n", i+1);

        for(int j=0; j<clusters; ++j ){

            if(i==j){
                cluster_dist[i]= 9999;
                continue;
            }
            
            if(centers[i].coordinates!=NULL && centers[j].coordinates!= NULL){
                cluster_dist[j] = distance(centers[i].coordinates, centers[j].coordinates, dimension);
                printf("d(%d,%d)= %f\n", i+1,j+1,cluster_dist[j]);
            }
            else{
                printf("Error in i= %d and j=%d\n",i,j);
            }
        }
        for(int i=0; i<clusters; ++i){
           printf("percentages-%d = %d\n", i+1, percentages[i]);
        }
        int min= minimum(cluster_dist);
        int x= percentages[i];
        int y= percentages[min];
        cluster_radii[i]= (cluster_dist[min])/2;
        printf("clster-%d : radius = %lf with %% = %d\n", i, cluster_radii[i], percentages[i]);

    }
    return;
}

void generate_centers(Point *centers, int dimensions){

    // allocating memory for the data center
    double * data_center = malloc(dimensions * sizeof(double));

    // randomly genreating the data center
    printf("data center: ");
    for (int k = 0; k < dimensions; k++){
        data_center[k]= rand() % 100;
        printf("%f, ", data_center[k]);
    }
    printf("\n");

    //loop to generate center for each cluster
    for (int i = 0; i < clusters; i++) {

        // allocating memory for the centers of a clusters
        centers[i].coordinates = malloc(dimensions * sizeof(double));

        // allocating the memory for offset
        double * offset = malloc(dimensions * sizeof(double));       

        for(int j = 0; j < dimensions; j++){
            offset[j] = ((rand() % 200) - 100) / 10.0;  // Generate random offset for the centers
        }

        // getting the divider to generate direction cosine
        double divider = Direction_cosine(offset, dimensions);

        double r= rand()%(threshold - min_treshold) + min_treshold; // generating the distance of the center from the data center
        
        for(int j = 0; j < dimensions; j++){
            offset[j] = (offset[j] / divider)*r;  // making the random offset direction cosines and then multiplying it by radius 
        }

        for(int j=0; j< dimensions; j++){
            centers[i].coordinates[j]= data_center[j] + offset[j]; // calculating the center coordinates
        }
        // pirnting the cluster centers
        printf("Cluster-%d center: ", i + 1);
        for (int j = 0; j < dimensions-1; j++) {
            printf("%lf, ", centers[i].coordinates[j]);
        }
        printf("%lf\n", centers[i].coordinates[dimensions-1]);

    }
    //printing the distances between the centers
    // Distance_printer(centers,dimensions);

    return;
}

void generate_points(Point *centers, double *radii, int *percentages, int dimensions) {

    srand(time(NULL));

    //opening the file
    FILE *file = fopen("cluster_data.csv", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    // Writing into CSV header
    fprintf(file, "ClusterID");
    for (int i = 0; i < dimensions; i++) {
        fprintf(file, ",Coordinate_%d", i + 1);
    }
    fprintf(file, "\n");

    // Generate points based on percentage distribution
    for (int cluster_id = 0; cluster_id < clusters; cluster_id++){

        // calculating the number of points in the corresponding cluster
        int num_points = (N * percentages[cluster_id]) / 100;

        for (int p = 0; p < num_points; p++) {
            fprintf(file, "Cluster-%d", cluster_id + 1);

            // Generate point coordinates within the radius
            for (int d = 0; d < dimensions; d++){

                double offset = ((rand() % 100) / 100.0) * radii[cluster_id];  // Random offset within radius
                double coordinate = centers[cluster_id].coordinates[d] + offset;
                fprintf(file, ",%lf", coordinate);
            }
            if( p < num_points-1){
                fprintf(file, "\n");
            }
            
        }

        if(cluster_id < clusters-1){
            fprintf(file, "\n");
        }
    }

    fclose(file);
    printf("Data successfully written to cluster_data.csv\n");
}

int Generate_test_data(Point * centers, double * radii,int dimensions, int test){
    FILE *fptr = fopen("test_data.csv", "w");
    if (fptr == NULL) {
        printf("Error opening file!\n");
        return 0;
    }
    fprintf(fptr, "\n");
    int counter=0;
    for(int i=0; i< clusters; ++i){
        fprintf(fptr, "Cluster-%d", i + 1);

        // Generate point coordinates within the radius
        for (int d = 0; d < dimensions; d++){
            double coordinate = centers[i].coordinates[d];
            fprintf(fptr, ",%lf", coordinate);
        }
        fprintf(fptr,"\n");
        counter++;
    }
    fprintf(fptr, "sattar");
    for(int i=0; i<clusters; ++i){
        fprintf(fptr, ",%f",(double)9999);
    }
    fprintf(fptr, "\n");
//-------------------------------------------------------------------------//
    for(int i=0; i< clusters; ++i){
        for(int j=i+1; j<clusters; ++j){
            double ri = radii[i];
            double rj = radii[j];
            double coordinate[dimensions];
            for(int k=0; k<dimensions; ++k ){
                coordinate[k] = (rj*centers[i].coordinates[k] + ri*centers[j].coordinates[k])/(rj+ri);
            }
            double center_distances[dimensions];
            for(int k=0; k<clusters; ++k){
                center_distances[k]= distance(coordinate, centers[k].coordinates, dimensions);
            }
            int cluster_id= minimum(center_distances);
            fprintf(fptr, "Cluster-%d", cluster_id + 1);

            for (int d = 0; d < dimensions; d++){
                fprintf(fptr, ",%lf", coordinate[d]);
            }
            fprintf(fptr,"\n");
            counter++;
        }
    }
    fprintf(fptr, "sattar");
    for(int i=0; i<clusters; ++i){
        fprintf(fptr, ",%f", (double)9999);
    }
    fprintf(fptr, "\n");
//--------------------------------------------------------------------------//

    double offset[dimensions];

    for(int j = 0; j < dimensions; j++){
        offset[j] = ((rand() % 200) - 100) / 10.0;  // Generate random offset for the centers
    }

    // getting the divider to generate direction cosine
    double divider = Direction_cosine(offset, dimensions);
    
    for(int j = 0; j < dimensions; j++){
        offset[j] = (offset[j] / divider);  // making the random offset direction cosines and then multiplying it by radius 
    }

    for(int i=0; i<clusters; ++i){
        for(int j=1; j<=test; ++j){
            fprintf(fptr, "Cluster-%d", i+1);
            for(int k=0; k <dimensions; ++k){
                double coordinates;
                coordinates= centers[i].coordinates[k] + (offset[k]*radii[i]*j)/test;
                fprintf(fptr, ",%f", coordinates);
            }
            if(!(i==clusters-1 && j==test )){
                    fprintf(fptr, "\n");
                }
            counter++;
        }
    }
    return counter;
}



int main() {
    srand(time(NULL));
    int dimensions;
    double radii[clusters];
    int percentage_distribution[clusters];
    int test;

    printf("Enter the number of dimensions for the clusters: ");
    scanf("%d", &dimensions);
    printf("Enter the number of points: ");
    scanf("%d", &N);
    printf("Enter the number of clusters: ");
    scanf("%d", &clusters);
    printf("Enter the no. of test points: ");
    scanf("%d", &test);

    
    printf("clusters= %d\n", clusters);
    // Allocate memory for centers
    Point centers[clusters];

    // Generate and print cluster centers
    generate_centers(centers, dimensions);
    // Get radius for each cluster
    // for (int i = 0; i < clusters; i++) {
    //     printf("Enter the radius for Cluster-%d: ", i + 1);
    //     scanf("%lf", &radii[i]);
    // }

    // Get percentage distribution for each cluster
    int total_percentage = 0;
    for (int i = 0; i < clusters; i++) {
        printf("Enter the percentage of points for Cluster-%d: ", i + 1);
        scanf("%d", &percentage_distribution[i]);
        total_percentage += percentage_distribution[i];
    }

    if (total_percentage != 100) {
        printf("Error: Total percentage must add up to 100.\n");
        return 1;
    }
    Radii_finalizer(centers,dimensions,radii, percentage_distribution);
    char input;
    printf("dimension = %d\n", dimensions);
    printf("Do you want to generate the data and write it to the CSV file?, if yes enter 'y': ");
    scanf(" %c", &input);
    if(input == 'y' || input == 'Y'){
        // Generate points and write to CSV
        generate_points(centers, radii, percentage_distribution, dimensions);
        int counter=0;
        counter = Generate_test_data(centers, radii, dimensions, test);
        printf("counter = %d\n", counter);
    }
    
    // Free memory for centers
    for (int i = 0; i < clusters; i++) {
        free(centers[i].coordinates);
    }

    return 0;
}