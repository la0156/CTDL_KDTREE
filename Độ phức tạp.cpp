#include<iostream> 
#include<conio.h>
#include<Windows.h>
#include<time.h>
#include<chrono>
using namespace std;

const int DIM = 2;

struct Point
{
    int point[DIM];
};
struct Node
{
    int point[DIM];
    Node* left, * right;
};

struct Node* newNode(int arr[])
{
    struct Node* temp = new Node;

    for (int i = 0; i < DIM; i++)
        temp->point[i] = arr[i];

    temp->left = temp->right = nullptr;
    return temp;
}
Node* insertTree(Node* root, int point[], unsigned cd)
{
    if (root == nullptr)
        return newNode(point);

    if (point[cd] < (root->point[cd]))
        root->left = insertTree(root->left, point, (cd + 1) % DIM);
    else
        root->right = insertTree(root->right, point, (cd + 1) % DIM);

    return root;
}


Node* insert(Node* root, int point[])
{
    return insertTree(root, point, 0);
}




// Tim gia tri nho nhat theo x va y
Node* minNode(Node* x, Node* y, Node* z, int dimension)
{
    Node* res = x;
    if (y != nullptr && y->point[dimension] < res->point[dimension])
        res = y;
    if (z != nullptr && z->point[dimension] < res->point[dimension])
        res = z;
    return res;
}

Node* findMin(Node* root, int dimension, unsigned cd)
{

    if (root == nullptr)
        return nullptr;

    if (cd == dimension)
    {
        if (root->left == nullptr)
            return root;
        return findMin(root->left, dimension, (cd + 1) % 2);
    }

    return minNode(root, findMin(root->left, dimension, (cd + 1) % 2), findMin(root->right, dimension, (cd + 1) % 2), dimension);
}

Node* find(Node* root, int dimension)
{
    return findMin(root, dimension, 0);
}


//Find point
bool arePointsSame(int point1[], int point2[])
{
    for (int i = 0; i < DIM; ++i)
        if (point1[i] != point2[i])
            return false;
    return true;
}

bool findpoint(Node* root, int point[], int cd)
{
    if (root == nullptr)
        return false;
    if (arePointsSame(root->point, point))
        return true;
    if (point[cd] < root->point[cd])
        findpoint(root->left, point, (cd + 1) % DIM);
    else findpoint(root->right, point, (cd + 1) % DIM);
}
bool find(Node* root, int point[])
{
    return findpoint(root, point, 0);
}


//Delete
void copyPoint(int p1[], int p2[])
{
    for (int i = 0; i < DIM; i++)
        p1[i] = p2[i];
}
Node* deleteNode(Node* root, int point[], int cd)
{
    if (root == nullptr)
        return nullptr;

    if (arePointsSame(root->point, point))
    {
        if (root->right != nullptr)
        {
            Node* min = find(root->right, cd);
            copyPoint(root->point, min->point);
            root->right = deleteNode(root->right, min->point, (cd + 1) % DIM);
        }
        else if (root->left != nullptr)
        {
            Node* min = find(root->left, cd);
            copyPoint(root->point, min->point);
            root->right = deleteNode(root->left, min->point, (cd + 1) % DIM);
            root->left = nullptr;
        }
        else
        {
            delete root->point;
            return nullptr;
        }
    }
    else if (point[cd] < root->point[cd])
        root->left = deleteNode(root->left, point, (cd + 1) % DIM);
    else
        root->right = deleteNode(root->right, point, (cd + 1) % DIM);
    return root;
}
Node* deleteN(Node* root, int point[])
{
    return deleteNode(root, point, 0);
}

//Diem gan nhat
float distance(int a[], int b[], int& step)
{
    int dim = DIM;
    step++;
    float t = 0;
    step++;
    while (dim > 0)
    {
        t += (a[dim - 1] - b[dim - 1]) * (a[dim - 1] - b[dim - 1]);
        dim = dim--;
        step += 3;
    }
    return sqrt(t);
}
Point* nearest(Node* root, Point* point, int cd, Point* best)
{
    if (root == nullptr)
    {
        return best;
    }
    struct Point* next_best = new Point;
    Node* next_branch = nullptr;
    float distance_best = 0;
    if (best == nullptr || (distance(point->point, best->point) > distance(point->point, root->point)))
    {
        next_best->point[0] = root->point[0];
        next_best->point[1] = root->point[1];
    }
    else {
        next_best = best;
    }
    if (point->point[cd] < root->point[cd])
    {
        next_branch = root->left;
        best = nearest(next_branch, point, (cd + 1) % DIM, next_best);
        if (distance(point->point, best->point) > abs(point->point[cd] - root->point[cd]))
        {
            return nearest(root->right, point, (cd + 1) % DIM, best);
        }
        else {
            return best;
        }
    }
    else
    {
        next_branch = root->right;
        best = nearest(next_branch, point, (cd + 1) % DIM, next_best);
        if (distance(point->point, best->point) > abs(point->point[cd] - root->point[cd]))
        {
            return nearest(root->left, point, (cd + 1) % DIM, best);
        }
        else {
            return best;
        }
    }
}
void printStatic(double arr[], int N)
{
    double min, max;
    double sum = 0;
    double mean;
    min = arr[0];
    for (int i = 0; i < N; i++)
        if (arr[i] < min)
            min = arr[i];
    max = arr[0];
    for (int i = 0; i < N; i++)
        if (arr[i] > max)
            max = arr[i];
    for (int i = 0; i < N; i++)
        sum += arr[i];
    mean = (double)sum / N;
    std::cout << "min:" << min << "\nmax:" << max << "\nmean:" << mean << std::endl;
}
void insertTreeComplexity(int num_simulation, int n)
{
    struct Node* root = nullptr;
    srand(time(NULL));
    double* TIME;
    int N = num_simulation;
    TIME = new double[num_simulation];
    int step;

    int points[DIM];

    for (int i = 0; i < N; i++)
    {
        auto start = chrono::high_resolution_clock::now();
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < DIM; k++)
            {
                points[k] = rand();
            }
            root = insert(root, points);

        }

        auto end = chrono::high_resolution_clock::now();
        auto time = chrono::duration<double>(end - start);
        TIME[i] = time.count();
        
    }
    printStatic(TIME, num_simulation);
}
void findminComplexity(int num_simulation, int n)
{
    struct Node* team = nullptr;
    struct Node* root = nullptr;
    srand(time(NULL));
    double* TIME;
    TIME = new double[num_simulation];
    int step;

    int points[DIM];

    for (int i = 0; i < num_simulation; i++)
    {
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < DIM; k++)
            {
                points[k] = rand();
            }
            root = insert(root, points);
        }
        auto start = chrono::high_resolution_clock::now();
        team = find(root, 0);
        auto end = chrono::high_resolution_clock::now();
        auto time = chrono::duration<double>(end - start);

        TIME[i] = time.count();
        
    }
    printStatic(TIME, num_simulation);

}
void deleteComplexity(int num_simulation, int n)
{
    struct Point* best = nullptr;
    struct Node* root = nullptr;
    srand(time(NULL));
    double* TIME;
    TIME = new double[num_simulation];
    int step;
    int point1[DIM];
    int points[DIM] ;

    for (int i = 0; i < num_simulation; i++)
    {
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < DIM; k++)
            {
                points[k] = rand();
            }
            root = insert(root, points);
        }
        for (int k = 0; k < DIM; k++)
        {
            point1[k] = rand();
        }

        auto start = chrono::high_resolution_clock::now();
        root = deleteN(root, point1);
        auto end = chrono::high_resolution_clock::now();
        auto time = chrono::duration<double>(end - start);

        TIME[i] = time.count();
        
    }
    printStatic(TIME, num_simulation);

}

void nearestComplexity(int num_simulation, int n)
{
    struct Point* best = nullptr;
    struct Node* root = nullptr;
    srand(time(NULL));
    double* TIME;
    int N = num_simulation;
    TIME = new double[num_simulation];
    int point1[DIM];
    for (int k = 0; k < DIM; k++)
    {
        point1[k] = rand();
    }
    struct Point* temp = new Point;
    for (int k = 0; k < DIM; k++)
    {
       temp->point[k] = point1[k];
    }

    int points[] = { 0, 0 };

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < DIM; k++)
            {
                points[k] = rand();
            }
            root = insert(root, points);

        }
        auto start = chrono::high_resolution_clock::now();
        best = nearest(root, temp, 0, best);
        auto end = chrono::high_resolution_clock::now();
        auto time = chrono::duration<double>(end - start);

        TIME[i] = time.count();   
    }
    printStatic(TIME, num_simulation);
}




int main()
{
    //insertTreeComplexity(50, 1000);
    //findminComplexity(50, 1000);
    //deleteComplexity(50, 1000);
    nearestComplexity(50, 1000);
    
    return 0;
}