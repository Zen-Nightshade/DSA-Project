# from sklearn.datasets import make_blobs
import numpy as np
import random
import numpy as np

def make_blobs(n_samples=100, n_features=2, centers=3, cluster_std=1.0, center_box=(-10.0, 10.0), random_state=None, percentages=None):
    rng = np.random.RandomState(random_state)

    # Determine center locations
    if isinstance(centers, int):
        centers_array = rng.uniform(center_box[0], center_box[1], size=(centers, n_features))
    else:
        centers_array = np.array(centers)
        centers = len(centers_array)

    # Compute number of samples per center
    if percentages is None:
        n_samples_per_center = [n_samples // centers] * centers
        for i in range(n_samples % centers):
            n_samples_per_center[i] += 1
    else:
        assert len(percentages) == centers, "Length of percentages must match number of centers"
        assert abs(sum(percentages) - 100.0) < 1e-2, "percentages must sum to 100"
        n_samples_per_center = [int(n_samples * p/100) for p in percentages]
        # Adjust rounding errors
        diff = n_samples - sum(n_samples_per_center)
        for i in range(abs(diff)):
            n_samples_per_center[i % centers] += 1 if diff > 0 else -1

    # Generate data
    X = []
    y = []

    for idx, (n, center) in enumerate(zip(n_samples_per_center, centers_array)):
        samples = rng.normal(loc=center, scale=cluster_std, size=(n, n_features))
        X.append(samples)
        y += [idx] * n

    X = np.vstack(X)
    y = np.array(y)
    return X, y



def train_data(X, Y):
    first = 0
    with open("cluster_data.csv", 'w') as file:
        for y,x in zip(Y, X):
            first +=1
            if first == 1:
                file.write("ClusterID")
                for i in range(len(x)):
                    coord = ",Coordinate_"+str(i+1)
                    file.write(coord)
    
                file.write("\n")
    
            file.write(f"CLuster-{y+1}")
            for x_coord in x:
                file.write(f",{x_coord:.4f}")
            if first != len(Y):
                file.write("\n")

def test1_data(t1_X, t1_Y):
    first = 0
    with open("test-1.csv", 'w') as test1:
        for y,x in zip(t1_Y, t1_X):
            first +=1
            if first == 1:
                    test1.write("ClusterID")
                    for i in range(len(x)):
                        coord = ",Coordinate_"+str(i+1)
                        test1.write(coord)
        
                    test1.write("\n")
                
            test1.write(f"CLuster-{y+1}")
            for x_coord in x:
                test1.write(f",{x_coord:.4f}")
            
            if first != len(t1_Y):
                test1.write("\n")

def test2_data(t2_X, t2_Y):
    first = 0
    with open("test-2.csv", 'w') as test2:
        for y,x in zip(t2_Y, t2_X):
            first +=1
            if first == 1:
                    test2.write("ClusterID")
                    for i in range(len(x)):
                        coord = ",Coordinate_"+str(i+1)
                        test2.write(coord)
        
                    test2.write("\n")
                
            test2.write(f"CLuster-{y+1}")
            for x_coord in x:
                test2.write(f",{x_coord:.4f}")
            
            if first != len(t2_Y):
                test2.write("\n")

def test3_data(t3_X, t3_Y):
    first = 0
    with open("test-3.csv", 'w') as test3:
        for y,x in zip(t3_Y, t3_X):
            first +=1
            if first == 1:
                    test3.write("ClusterID")
                    for i in range(len(x)):
                        coord = ",Coordinate_"+str(i+1)
                        test3.write(coord)
        
                    test3.write("\n")
                
            test3.write(f"CLuster-{y+1}")
            for x_coord in x:
                test3.write(f",{x_coord:.4f}")
            
            if first != len(t3_Y):
                test3.write("\n")


def Istropic_gaussian_clusters(dimension=10, n_clusters=3, train=1000, test1=100, test2=100, test3=100, seed=None, sep =10, percentages = None):
    if seed is not None:
        rng = np.random.RandomState(seed)
    else:
        rng = np.random

    centers = rng.randn(n_clusters, dimension) * sep

    X_train, Y_train = make_blobs(n_samples=train, centers=centers, cluster_std=1.0, n_features=dimension, random_state=seed, percentages = percentages)
    train_data(X_train, Y_train)

    X_test1, Y_test1 = make_blobs(n_samples=test1, centers=centers, cluster_std=0.5, n_features=dimension, random_state=seed, percentages = percentages)
    test1_data(X_test1, Y_test1)

    X_test2, Y_test2 = make_blobs(n_samples=test2, centers=centers, cluster_std=1.5, n_features=dimension, random_state=seed, percentages = percentages)
    test2_data(X_test2, Y_test2)

    X_test3, Y_test3 = make_blobs(n_samples=test3, centers=centers, cluster_std=2.0, n_features=dimension, random_state=seed, percentages = percentages)
    test3_data(X_test3, Y_test3)

    return centers


def Anisotropic_gaussian_cluster(dimension=10, n_clusters=3, std = [1.0, 0.5, 1.5, 2.0], train=1000, test1=100, test2=100, test3=100, seed=None, sep =10, percentages = None):
    
    A = np.random.randn(dimension, dimension)

    if seed is not None:
        rng = np.random.RandomState(seed)
    else:
        rng = np.random
    centers = rng.randn(n_clusters, dimension) * sep

    
    X_train, Y_train = make_blobs(n_samples=train, centers=centers, cluster_std=std[0], n_features=dimension, random_state=seed, percentages = percentages)
    X_train = X_train @ A
    train_data(X_train, Y_train)

    X_test1, Y_test1 = make_blobs(n_samples=test1, centers=centers, cluster_std=std[1], n_features=dimension, random_state=seed, percentages = percentages)
    X_test1 = X_test1 @ A
    test1_data(X_test1, Y_test1)

    X_test2, Y_test2 = make_blobs(n_samples=test2, centers=centers, cluster_std=std[2], n_features=dimension, random_state=seed, percentages = percentages)
    X_test2 = X_test2 @ A
    test2_data(X_test2, Y_test2)

    X_test3, Y_test3 = make_blobs(n_samples=test3, centers=centers, cluster_std=std[3], n_features=dimension, random_state=seed, percentages = percentages)
    X_test3 = X_test3 @ A
    test3_data(X_test3, Y_test3)

    return A, centers

def Uniform(dimensions=10, train=1000, test1=100, test2=100, test3=100, seed=100):
    if seed is not None:
        np.random.seed(seed)

    def gen_data(n_samples):
        X = np.random.rand(n_samples, dimensions)
        Y = (X.sum(axis=1) > (dimensions / 2)).astype(int)
        return X, Y

    X_train, Y_train = gen_data(train)
    train_data(X_train, Y_train)

    X_test1, Y_test1 = gen_data(test1)
    test1_data(X_test1, Y_test1)

    X_test2, Y_test2 = gen_data(test2)
    test2_data(X_test2, Y_test2)

    X_test3, Y_test3 = gen_data(test3)
    test3_data(X_test3, Y_test3)

def main():
    dimension = 10
    n_clusters = 3
    train = 1000
    test1 = 100
    test2 = 100
    test3 = 100

    #------------------------------------------ Isotropic Gaussian Cluster Generator
    Istropic_gaussian_clusters(
        dimension = dimension,
        n_clusters = n_clusters,
        train = train,
        test1 = test1,
        test2 = test2,
        test3 = test3,
        seed = 100
    )
    #------------------------------------------ Anisotropic Gaussian Cluster Generator
    # aniso = [1.0, 0.5, 1.5, 2.0]

    # Anisotropic_gaussian_cluster(
    #     dimension = dimension,
    #     n_clusters = n_clusters,
    #     std = aniso,
    #     train = train,
    #     test1 = test1,
    #     test2 =test2,
    #     test3 = test3,
    #     seed = 100
    # )

    #---------------------------------------- Uniform Data Generator
    # Uniform(
    #     dimensions = dimension,
    #     train = train,
    #     test1 = test1,
    #     test2 = test2,
    #     test3 = test3,
    #     seed = 100
    # )

    #---------------------------------------- Overlapping Cluster Generator
    # uni = [6.0, 0.5, 4.0, 10.0]

    # Anisotropic_gaussian_cluster(
    #     dimension = dimension,
    #     n_clusters = n_clusters,
    #     std = uni,
    #     train = train,
    #     test1 = test1,
    #     test2 =test2,
    #     test3 = test3,
    #     seed = 100
    # )



if __name__ == "__main__":
    main()