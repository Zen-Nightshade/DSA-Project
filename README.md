# DSA-Project

## Generalized Hyperplane Tree (GHT)

A **GHT** (Generalized Hyperplane Tree) is a tree-based spatial partitioning data structure used primarily for efficient nearest neighbor search in high-dimensional spaces. It extends traditional structures like kd-trees and binary space partitioning (BSP) trees, providing better performance and flexibility in high-dimensional settings where classic approaches often struggle.

GHTs recursively partition a dataset by splitting it into two subsets at each node, using a hyperplane as the decision boundary. The way this hyperplane is chosen greatly affects the tree's efficiency and adaptability to the data.

---

## Implemented GHT Splitting Strategies

This project implements and compares several strategies for choosing the splitting hyperplane in a GHT:

### 1. **Naive GHT Splitting**

- **Split Criterion:** Picks two random points and splits along the hyperplane bisecting these points.
- **Pros:**  
  - Extremely fast and simple to implement.
  - Performs well when there are many clusters, as random pairs are less likely to come from the same cluster.
- **Cons:**  
  - Ignores underlying data distribution.

---

### 2. **VarMed (Variance-Median) GHT Splitting**

- **Split Criterion:**  
  - Compute variance for each dimension.
  - Choose the dimension with the highest variance.
  - Split at the median value in this dimension.
- **Pros:**  
  - Better for fewer clusters.
  - Produces smaller trees.
- **Cons:**  
  - Axis-aligned splits may not capture complex data structures.
  - Can perform worse than Naive with many clusters.

---

### 3. **k-means GHT Splitting**

- **Split Criterion:**  
  - Run k-means (k=2) clustering on the node’s points.
  - Obtain two centroids (`c1`, `c2`).
  - Split using a hyperplane perpendicular to the line joining `c1` and `c2`, passing through their midpoint.
- **Pros:**  
  - Captures data shape and distribution.
  - Allows oblique (non-axis-aligned) splits, adapting better to general data.
- **Cons:**  
  - Higher computational cost, especially in high dimensions.

---

### 4. **D1-kmeans GHT Splitting**

- **Split Criterion:**  
  - Find the dimension with the highest variance.
  - Run 1D k-means (k=2) along this dimension to get two centroids.
  - Split at the midpoint between these centroids.
- **Pros:**  
  - Faster than full k-means GHT.
  - Smaller trees than pure axis-aligned splits.
  - Balances speed and adaptability.
- **Cons:**  
  - Less flexible than full k-means GHT; splits are still axis-aligned.

---

## Summary Table

| **Method**    | **Split Type**                  | **Data Awareness**     | **Hyperplane Type**      | **Computation Cost** | **Best When**                                             |
| ------------- | ------------------------------- | ---------------------- | ------------------------ | -------------------- | --------------------------------------------------------- |
| **Naive**     | Random pair of points           |  No                   | Oblique (random)         |  Very Low           | Large number of clusters; simplicity preferred            |
| **VarMed**    | Highest variance + median       |  Partial (variance)   | Axis-aligned             |  Low                | Low number of clusters; need balanced splits              |
| **k-means**   | k-means (k=2) centroids         |  Full (distribution) | Oblique (centroid-based) |  High               | Few clusters; complex distribution shapes                 |
| **D1-kmeans** | k-means on highest variance dim |  Balanced             | Axis-aligned (1D)        |  Moderate           | Trade-off between performance and cost in high dimensions |

---

## References
The following research papers were referred to, while studying and understanding the theory behind Generalized Hyperplane Trees (GHT) and their application in nearest neighbor search:

1. Uhlmann, J. K. (1991). *Satisfying general proximity / similarity queries with metric trees*. *Information Processing Letters*, 40(4), 175–179. [ScienceDirect](https://www.sciencedirect.com/science/article/abs/pii/002001909190074R)

2. Devroye, L., King, J., & McDiarmid, C. (2011). *Random Hyperplane Search Trees*. *SIAM Journal on Computing*, 40(2), 416–430. [PDF](https://www.cs.mcgill.ca/~jking/papers/hyperplane_search_trees.pdf)

---
