#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define threshold 50
#define clusters 4
#define N 200

typedef struct {
    double *coordinates;
} Point;

void generate_centers(Point *centers, int dimensions) {
    for (int i = 0; i < clusters; i++) {
        centers[i].coordinates = malloc(dimensions * sizeof(double));
        for (int j = 0; j < dimensions; j++) {
            centers[i].coordinates[j] = (rand() % 100) / 10.0;  // Generate random coordinates
        }
        printf("Cluster-%d center: ", i + 1);
        for (int j = 0; j < dimensions; j++) {
            printf("%lf ", centers[i].coordinates[j]);
        }
        printf("\n");
    }
}

void generate_points(Point *centers, double *radii, int *percentages, int dimensions) {
    FILE *file = fopen("cluster_data.csv", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    // Write CSV header
    fprintf(file, "ClusterID");
    for (int i = 0; i < dimensions; i++) {
        fprintf(file, ",Coordinate_%d", i + 1);
    }
    fprintf(file, "\n");

    // Generate points based on percentage distribution
    for (int cluster_id = 0; cluster_id < clusters; cluster_id++) {
        int num_points = (N * percentages[cluster_id]) / 100;
        for (int p = 0; p < num_points; p++) {
            fprintf(file, "Cluster-%d", cluster_id + 1);

            // Generate point coordinates within the radius
            for (int d = 0; d < dimensions; d++) {
                double offset = ((rand() % 100) / 100.0) * radii[cluster_id];  // Random offset within radius
                double coordinate = centers[cluster_id].coordinates[d] + offset;
                fprintf(file, ",%lf", coordinate);
            }
            fprintf(file, "\n");
        }
    }

    fclose(file);
    printf("Data successfully written to cluster_data.csv\n");
}

int main() {
    int dimensions;
    double radii[clusters];
    int percentage_distribution[clusters];

    printf("Enter the number of dimensions for the clusters: ");
    scanf("%d", &dimensions);

    // Allocate memory for centers
    Point centers[clusters];

    // Generate and print cluster centers
    generate_centers(centers, dimensions);

    // Get radius for each cluster
    for (int i = 0; i < clusters; i++) {
        printf("Enter the radius for Cluster-%d: ", i + 1);
        scanf("%lf", &radii[i]);
    }

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

    // Generate points and write to CSV
    generate_points(centers, radii, percentage_distribution, dimensions);

    // Free memory for centers
    for (int i = 0; i < clusters; i++) {
        free(centers[i].coordinates);
    }

    return 0;
}