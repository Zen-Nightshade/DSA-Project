# DSA-Project

### 1. **Naive GHT Splitting**

**Split Criterion:**
* Picks two random points and splits along hyperplane bisecting the two points.

**Pros:**
* Extremely fast construction and simple to implement.
* Performs reasonably well when the number of clusters is high, as the chance of picking two points from the same cluster decreases, which can lead to better separation.

**Cons:**
* Completely unaware of the underlying data distribution.

---

### 2. **VarMed (Variance-Median) GHT Splitting**

**Split Criterion:**
* Compute the **variance across all dimensions**.
* Selects the **dimension with the highest variance**.
* Split at the **median** value of that dimension.

**Intuition:**
* High variance = more spread = more informative.
* Median ensures roughly equal partition sizes.

**Pros:**
* Performs better especially when the number of clusters is low.
* Smallest possible tree size

**Cons:**
* Still axis-aligned (not optimal for diagonally structured data).
* Worst than Naive, when the number of clusters is high.

---

### 3. **k-means GHT Splitting**

**Split Criterion:**
* Run **k-means (k=2)** on current node’s data points.
* Obtain 2 centroids: `c1`, `c2`.
* Constructs a hyperplane **perpendicular to the line joining c1 and c2**, passing through their midpoint.

**Pros:**
* Captures data shape and distribution.
* Produces oblique Hyperplane (non axis-aligned) — better fit for general data.

**Cons:**
* More expensive to compute than axis-aligned splits.
* Computation cost rises as the number of dimensions increases.

---

### 4. **D1-kmeans GHT Splitting**
A hybrid between VarMed and k-means GHT.

**Split Criterion:**
* First find the dimension of highest variance 
* Run **1D k-means (k=2)** along this dimension and get 2 centroids (`c1`, `c2`).
* Split at the midpoint between c1 and c2.

**Pros:**
* Faster than k-means GHT.
* A bit smaller tree size.
* A trade off between computational cost and adaptability to data in high dimensions.

**Cons:**
* Less flexible than full k-means GHT as it restricts splitting to one dimension.

---

# Summary of GHT Splitting Strategies

| **Method**    | **Split Type**                  | **Data Awareness**     | **Hyperplane Type**      | **Computation Cost** | **Best When**                                             |
| ------------- | ------------------------------- | ---------------------- | ------------------------ | -------------------- | --------------------------------------------------------- |
| **Naive**     | Random pair of points           |  No                   | Oblique (random)         |  Very Low           | Large number of clusters; simplicity preferred            |
| **VarMed**    | Highest variance + median       |  Partial (variance)   | Axis-aligned             |  Low                | Low number of clusters; need balanced splits              |
| **k-means**   | k-means (k=2) centroids         |  Full (distribution) | Oblique (centroid-based) |  High               | Few clusters; complex distribution shapes                 |
| **D1-kmeans** | k-means on highest variance dim |  Balanced             | Axis-aligned (1D)        |  Moderate          | Mid-point between performance and cost in high dimensions |
