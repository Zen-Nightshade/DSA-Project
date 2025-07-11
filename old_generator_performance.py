import csv
import math
import matplotlib.pyplot as plt
import subprocess as sub


fig, ax = plt.subplots()

# Change the figure background color
fig.patch.set_facecolor('black')

# Change the axes background color
ax.set_facecolor('black')

centers=[]
test_2=[]
test_3=[]

percentages =[30,30,40]
N=1000

input_data=''
input_data += str(2)+'\n'+str(N)+'\n'+str(3)+'\n'+str(100)+'\n'

for i in range(3):
    input_data += str(percentages[i])+'\n'
input_data += 'Y\n'

compile_generator= sub.run(['gcc', 'datapoint_generator.c', '-o', 'out','-lm'])
run_generator= sub.run(['./out'], input= input_data.encode(), capture_output=True)
# buff = run_generator.stdout.decode("utf-8")
# print(buff)

compile_Naive_Tree= sub.run(['gcc', 'Naive_GHT.c', '-o', 'out','-lm'])
run_Naive_Tree= sub.run(['./out'], input= input_data.encode(), capture_output=True)
output= run_Naive_Tree.stdout.decode("utf-8")
print("Naive Algo\n")
print(output)
print("--------------------------------------------------------")
compile_VarMed_Tree= sub.run(['gcc', 'VarMed_GHT.c', '-o', 'out','-lm'])
run_VarMed_Tree= sub.run(['./out'], input= input_data.encode(), capture_output=True)
output= run_VarMed_Tree.stdout.decode("utf-8")
print("VarMed ALgo\n")
print(output)

num_lines=3
with open("test_data.csv", 'r') as file:
    reader = csv.reader(file)
    
    for i, row in enumerate(reader):
        if i < num_lines:
            for x in range(1,len(row)):
                row[x] = float(row[x])
                centers.append(row)
        if i > num_lines and i < 2*num_lines+1:
            for x in range(1,len(row)):
                row[x] = float(row[x])
                test_2.append(row)
        if i > 2*num_lines+1:
            for x in range(1,len(row)):
                row[x] = float(row[x])
                test_3.append(row)
    

    # print(centers)
data=[]
with open("cluster_data.csv", 'r') as file:
    reader = csv.reader(file)
    header = next(reader) 
    
    for row in reader:
        for x in range(1,len(row)):
            row[x] = float(row[x])
        data.append(row)
    # print(data)

Naive_pivots=[]
with open("Naive_pivots.csv","r") as file:
    reader = csv.reader(file)

    for row in reader:
        row[0] = int(row[0])
        for x in range(1,len(row)):
            row[x] = float(row[x])
        Naive_pivots.append(row)

VarMed_pivots=[]
with open("VarMed_pivots.csv","r") as file:
    reader = csv.reader(file)

    for row in reader:
        row[0] = int(row[0])
        row[1] = int(row[1])
        row[2] = float(row[2])

        VarMed_pivots.append(row)

def distance(p1, p2):
    sum2=0
    for x1,x2 in zip(p1,p2):
        if type(x1) == str:
            continue
        sum2 = (x1-x2)**2
    return math.sqrt(sum2)

def minimum(p):
    i=0
    min=p[0]
    for x in range(1,len(p)):
        if min > p[x]:
            min = p[x]
            i=x
    return i
# positive=0
# negative=0
# for point in data:
#     distances=[]
#     for center in centers:
#         distances.append(distance(point,center))
#     index = minimum(distances)
#     if index+1 == int(center[0][8]):
#         positive +=1
#     else:
#         negative +=1

# print("positive:", positive,"\nnegative:",negative,"\ntotal:",len(data))

def plot_bisector(pivot):
    x1 = pivot[1]
    y1 = pivot[2]
    x2 = pivot[3]
    y2 = pivot[4]

    mid_x = (x1 + x2) / 2
    mid_y = (y1 + y2) / 2

    if y1==y2:
        y_values = [mid_y, mid_y]
        x_values = [plt.xlim()[0], plt.xlim()[1]]

    elif x2 != x1:
        slope = (y2 - y1) / (x2 - x1)
        perp_slope = -1 / slope


        x_min, x_max = plt.xlim()  
        x_values = [x_min, x_max]
        
        y_values = [mid_y + perp_slope * (x - mid_x) for x in x_values]
    else:
        x_values = [mid_x, mid_x]
        y_values = [plt.ylim()[0], plt.ylim()[1]] 

        
    lat = "node-"+str(pivot[0])
    plt.plot(x_values, y_values, 'r--')
    
def plot_axis(axis):
    xis = axis[1]
    median = axis[2]

    if( axis[1] == 0 ):
        y_min, y_max = plt.ylim()  
        y_values = [y_min, y_max]
        
        x_values = [median for x in y_values]
        plt.plot(x_values, y_values, 'y-')
    if( axis[1] == 1 ):
        x_min, x_max = plt.xlim()  
        x_values = [x_min, x_max]
        
        y_values = [median for x in x_values]
        plt.plot(x_values, y_values, 'y-')



x1=[]
x2=[]
x3=[]
y1=[]
y2=[]
y3=[]
for point in data:
    if int(point[0][8]) == 1:
        x1.append(point[1])
        y1.append(point[2])
    if int(point[0][8]) == 2:
        x2.append(point[1])
        y2.append(point[2])
    if int(point[0][8]) == 3:
        x3.append(point[1])
        y3.append(point[2])

x_test21 = []
x_test22 = []
x_test23 = []

x_test31 = []
x_test32 = []
x_test33 = []

y_test21 = []
y_test22 = []
y_test23 = []

y_test31 = []
y_test32 = []
y_test33 = []

for test in test_2:
    if test[0][8]=='1':
        x_test21.append(test[1])
        y_test21.append(test[2])
    if test[0][8]=='2':
        x_test22.append(test[1])
        y_test22.append(test[2])
    if test[0][8]=='3':
        x_test23.append(test[1])
        y_test23.append(test[2])

for test in test_3:
    if test[0][8]=='1':
        x_test31.append(test[1])
        y_test31.append(test[2])
    if test[0][8]=='2':
        x_test32.append(test[1])
        y_test32.append(test[2])
    if test[0][8]=='3':
        x_test33.append(test[1])
        y_test33.append(test[2])

plt.scatter(x_test21, y_test21, color = 'yellowgreen', marker= 'o',label='Cluster-1')
plt.scatter(x_test22, y_test22, color = 'crimson', marker= 'o',label='Cluster-2')
plt.scatter(x_test23, y_test23, color = 'teal', marker= 'o',label='Cluster-3')

plt.scatter(x_test31, y_test31, color = 'yellowgreen', marker= 'o',label='Cluster-1')
plt.scatter(x_test32, y_test32, color = 'crimson', marker= 'o',label='Cluster-2')
plt.scatter(x_test33, y_test33, color = 'teal', marker= 'o',label='Cluster-3')

for pivot in Naive_pivots:
    plot_bisector(pivot)

for axis in VarMed_pivots:
    plot_axis(axis)

plt.scatter(centers[0][1],centers[0][2], color='blue', marker='o')
plt.scatter(centers[1][1],centers[1][2], color='blue', marker='o')
plt.scatter(centers[2][1],centers[2][2], color='blue', marker='o')

plt.scatter(x1, y1, color='purple', label='Cluster-1')
plt.scatter(x2, y2, color='orange', label='Cluster-2')
plt.scatter(x3, y3, color='cyan', label='Cluster-3')

plt.axhline(y=0, color='black', linewidth=0.5)
plt.axvline(x=0, color='black', linewidth=0.5)
title= "data points"
plt.title(title)
plt.legend()
plt.show()