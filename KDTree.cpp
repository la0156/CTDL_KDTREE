#include<iostream> 
#include<conio.h>
#include<Windows.h>
using namespace std;

const int DIM = 2;
// tao K-D tree
void gotoxy(int x, int y)
{
    static HANDLE h = NULL;
    if (!h)
        h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD c = { x, y };
    SetConsoleCursorPosition(h, c);
}
struct Point
{
    int point[DIM];
};
struct Node
{
    int point[DIM];
    Node* left, * right;
};

struct Node* newNode(int point[])
{
    struct Node* temp = new Node;

    for (int i = 0; i < DIM; i++)
        temp->point[i] = point[i];

    temp->left = temp->right = NULL;
    return temp;
}

// Them diem vao cay
Node* insertTree(Node* root, int point[], unsigned cd)
{
    if (root == NULL)
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
    Node* min = x;
    if (y != NULL && y->point[dimension] < min->point[dimension])
        min = y;
    if (z != NULL && z->point[dimension] < min->point[dimension])
        min = z;
    return min;
}

Node* findMin(Node* root, int dimension, unsigned cd)
{

    if (root == NULL)
        return NULL;
    
    if (cd == dimension)
    {
        if (root->left == NULL)
            return root;
        else
            return findMin(root->left, dimension, (cd + 1) % DIM);
    }

    return minNode(root, findMin(root->left, dimension, (cd + 1) % DIM), findMin(root->right, dimension, (cd + 1) % DIM), dimension);
}

Node* find(Node* root, int dimension)
{
    return findMin(root, dimension, 0);
}

//duyet cay k-dtree
void NLR(Node* root, int x, int y, int d) {
    int i;
    if (root != nullptr)
    {
        gotoxy(x, y);
        cout << "(" << root->point[0] << "," << root->point[1] << ")";
        if (root->left != nullptr) {
            NLR(root->left, x - d / 2, y + 3, d / 2);
        }
        if (root->right != nullptr) {
            NLR(root->right, x + d / 2, y + 3, d / 2);
        }
    }
}
void Hienthi(Node* root)
{
    NLR(root, 50, 2, 50);
}

//Find point
bool arePointsSame(int point1[], int point2[])
{
    for (int i = 0; i < DIM; ++i)
        if (point1[i] != point2[i])
            return false;
    return true;
}

bool searchpoint(Node* root, int point[], int cd)
{
    if (root == NULL)
        return false;
    if (arePointsSame(root->point, point))
        return true;
    if (point[cd] < root->point[cd])
        searchpoint(root->left, point, (cd + 1) % DIM);
    else searchpoint(root->right, point, (cd + 1) % DIM);
}
bool search(Node* root, int point[])
{
    return searchpoint(root, point, 0);
}


//Delete
void copyPoint(int p1[], int p2[])
{
    for (int i = 0; i < DIM; i++)
        p1[i] = p2[i];
}
Node* deleteNode(Node* root, int point[], int cd)
{
    if (root == NULL)
        return NULL;

    if (arePointsSame(root->point, point))
    {
        if (root->right != NULL)
        {
            Node* min = find(root->right, cd);
            copyPoint(root->point, min->point);
            root->right = deleteNode(root->right, min->point, (cd + 1) % DIM);
        }
        else if (root->left != NULL)
        {
            Node* min = find(root->left, cd);
            copyPoint(root->point, min->point);
            root->right = deleteNode(root->left, min->point, (cd + 1) % DIM);
            root->left = NULL;
        }
        else
        {
            delete root->point;
            return NULL;
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
double distance(int a[], int b[])
{
    int dim = DIM;
    double t = 0;
    while (dim > 0)
    {
        t += (a[dim - 1] - b[dim - 1]) * (a[dim - 1] - b[dim - 1]);
        dim = dim--;
    }
    return sqrt(t);
}
Point* nearest(Node* root, Point* point, int cd, Point* best)
{
    if (root == nullptr)
        return best;
    struct Point* next_best = new Point;
    Node* next_branch = nullptr;
    double distance_best = 0;
    if (best == nullptr || (distance(point->point, best->point) > distance(point->point, root->point)))
    {
        copyPoint(next_best->point, root->point);
    }
    else next_best = best;
    if (point->point[cd] < root->point[cd])
    {
        next_branch = root->left;
        best = nearest(next_branch, point, (cd + 1) % DIM, next_best);
        if (distance(point->point, best->point) > abs(point->point[cd] - root->point[cd]))
            return nearest(root->right, point, (cd + 1) % DIM, best);
        else return best;
    }
    else
    {
        next_branch = root->right;
        best = nearest(next_branch, point, (cd + 1) % DIM, next_best);
        if (distance(point->point, best->point) > abs(point->point[cd] - root->point[cd]))
        {
            return nearest(root->left, point, (cd + 1) % DIM, best);
        }
        else return best;
    }
}


int main()
{
    struct Node* root = nullptr;
    struct Node* team = nullptr;
    struct Point* best = nullptr;
    int n;
    int points[DIM] ;
    cout << "Enter the number of points: ";
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        gotoxy(0, 1);
        cout << "Enter data for the tree: ";
        for (int k = 0; k < DIM; k++)
        {
            cin >> points[k];
        }
        root = insert(root, points);
        system("cls");
        Hienthi(root);
    }
    system("cls");
    Hienthi(root);

    //Find min 
    cout << endl << endl << endl << endl;
    //Find x
    gotoxy(1, 15);
    team = find(root, 0);
    cout << "Min (x-dimension): " << "(" << team->point[0] << " , " << team->point[1] << ")" << endl;
    //Find y
    gotoxy(1, 16);
    team = find(root, 1);
    cout << "Min (y-dimension): " << "(" << team->point[0] << " , " << team->point[1] << ")" << endl;
    _getch();
    cout << endl;

    int step = 5;
    do
    {
        cout << "Select the step you want to take: " << endl;
        cout << "1. Insert point" << endl;
        cout << "2. Search point" << endl;
        cout << "3. Delete point" << endl;
        cout << "4. Search point nearest neighbour " << endl;
        cout << "5. Exit" << endl;
        cout << "Step: ";
        cin >> step;
        //Insert point
        if (step == 1)
        {
            int point1s[DIM];
            cout << "Insert point: ";
            for (int k = 0; k < DIM; k++)
            {
                cin >> point1s[k];
            }
            root = insert(root, point1s);
            system("cls");
            gotoxy(0, 0);
            cout << "Tree after inserting: ";
            Hienthi(root);
            /*cout << endl << endl << endl << endl;*/
            //Find x
            //gotoxy(1, 15);
            gotoxy(0, 15);
            team = find(root, 0);
            cout << "Min (x-dimension): " << "(" << team->point[0] << " , " << team->point[1] << ")" << endl;
            //Find y
            //gotoxy(1, 16);
            team = find(root, 1);
            cout << "Min (y-dimension): " << "(" << team->point[0] << " , " << team->point[1] << ")" << endl;
            cout << endl;

        }
        //Search point
        else if (step == 2)
        {
            int point1s[DIM];
            cout << "Search point: ";
            cin >> point1s[0] >> point1s[1];
            if (search(root, point1s))
                cout << "Found" << endl;
            else cout << "No found" << endl;
        }
        // Delete point
        else if (step == 3)
        {
            int point1[DIM];
            cout << "Point to delete: ";
            for (int k = 0; k < DIM; k++)
            {
                cin >> point1[k];
            }
            root = deleteN(root, point1);
            system("cls");
            gotoxy(0, 0);
            cout << "Tree after deleting: ";
            Hienthi(root);
            cout << endl;
            cout << endl;
            _getch();
        }
        // Nearest Neighbour
        else if (step == 4)
        {
            int point2s[DIM];
            cout << "Searching nearest neighbor: ";
            cin >> point2s[0] >> point2s[1];
            struct Point* temp = new Point;
            for (int k = 0; k < DIM; k++)
            {
                temp->point[k] = point2s[k];
            }

            best = nearest(root, temp, 0, best);
            cout << "Nearest Neighbor: " << "(" << best->point[0] << " , " << best->point[1] << ")" << endl;
            _getch();
        }
    } while (step != 5);
    return 0;
}