#include<iostream>
#include<vector>
#include<fstream>
#include<ctime>
#include<iomanip>
#include<string>
using namespace std;
vector<vector<int>> getMatrix(int n)
{
    vector<vector<int>> matrix(n, vector<int>(n));
    ifstream fin;
    fin.open("DistanceMatrix.txt");
    if (!fin.is_open()) { cout << "Error ocured"; exit(1); }

    string tmp;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fin >> tmp;
            matrix[i][j] = stoi(tmp);
        }
    }
    fin.close();
    return matrix;
}
void FeromGen(int a) {
    srand(time(NULL));
    ofstream fout;
    fout.open("Smell.txt");
    for (int i = 0; i < a; i++) {
        for (int j = 0; j < a; j++) {

            if (i == j) {
                if (j < a - 1)
                    fout << -1.0 << " ";
                else
                    fout << -1.0;
                continue;
            }
            if (j < a - 1)
                fout << (float)(rand()%3+1)/10 << " ";//<< (float)((rand() % 3) + 1) / 10 << " ";
            else
                fout << (float)(rand() % 3 + 1) / 10;//<< (float)((rand() % 3) + 1) / 10;
        }
        if (i < a - 1)
            fout << endl;
    }
    fout.close();
}

void visiblGen(int a, vector<vector<int>> distances) {
    srand(time(NULL));
    ofstream fout;
    fout.open("Visibility.txt");
    for (int i = 0; i < a; i++) {
        for (int j = 0; j < a; j++) {

            if (i == j) {
                fout << -1;
                if (j < a - 1)
                    fout << " ";
                continue;
            }
            if (j < a - 1)
                fout << setprecision(2) << (float)1 / distances[i][j] << " ";//(float)((rand() % 10) + 1) / 10 << " ";
            else
                fout << setprecision(2) << (float)1 / distances[i][j]; //(float)((rand() % 10) + 1) / 10;
        }
        if (i < a - 1)
            fout << endl;
    }
    fout.close();
}
void matrixGenerator(int a)
{
	srand(time(NULL));
    ofstream fout;
    fout.open("DistanceMatrix.txt");
	vector<vector<int>> matrix(a, vector<int>(a));
    for (int i = 0; i < a; i++) {
        for (int j = 0; j < a; j++) 
        {
            if (i == j) {
                fout << -1;
                if (j != a - 1)
                    fout << ' ';
                continue;
            }

            if (matrix[j][i] == 0) {
                matrix[i][j] = (rand() % 40) + 1;
                fout << matrix[i][j];

            }
            else {

                matrix[i][j] = matrix[j][i];
                fout << matrix[i][j];
            }
            if (j != a - 1)
                fout << ' ';
        }
        fout << "\n";
    }
    fout.close();
}

class AntAlg
{
    double Lmin;
    int countVert;
    vector<vector<int>> matrixDist;
    vector<vector<double>> smell;
    vector<vector<double>> visible;
    int alpha;
    int beta;
    double p;
    int ants;
    int crazy_ants;
    bool CheckVer(int a, vector<int> checked)
    {
        for (int n = 0; n < checked.size(); n++)
        {
            if (a == checked[n])
            {
                return false;
            }
        }
        return true;
    }
    vector<vector<double>> getMatrixVisible(int n) {
        vector<vector<double>> matrix(n, vector<double>(n));
        ifstream fin;
        fin.open("Visibility.txt");
        if (!fin.is_open()) { cout << "Error ocured"; exit(1); }

        string tmp;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                fin >> tmp;
                matrix[i][j] = stod(tmp);
            }
        }
        fin.close();
        return matrix;
    }
    vector<vector<double>> getMatrixSmell(int n) {
        vector<vector<double>> matrix(n, vector<double>(n));
        ifstream fin;
        fin.open("Smell.txt");
        if (!fin.is_open()) { cout << "Error ocured"; exit(1); }

        string tmp;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                fin >> tmp;
                matrix[i][j] = stod(tmp);
            }
        }
        fin.close();
        return matrix;
    }
    vector<vector<int>> getMatrix(int n)
    {
        vector<vector<int>> matrix(n, vector<int>(n));
        ifstream fin;
        fin.open("DistanceMatrix.txt");
        if (!fin.is_open()) { cout << "Error ocured"; exit(1); }

        string tmp;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                fin >> tmp;
                matrix[i][j] = stoi(tmp);
            }
        }
        fin.close();
        return matrix;
    }
    bool Check(int a, vector<int> checked)
    {
        for (int n = 0; n < checked.size(); n++)
        {
            if (a == checked[n])
            {
                return false;
            }
        }
        return true;
    }
    int GreedyAlg(int a, vector<vector<int>> matrix)
    {
        int Lmin = 0;
        vector<int> checked;
        int i = 0;
        while (checked.size() < a)
        {
            if (checked.size() + 1 == a)
            {
                Lmin += matrix[i][0];
                break;
            }
            int min = 41;
            int count = 0;
            checked.push_back(i);
            for (int j = 0; j < a; j++)
            {
                if (min > matrix[i][j] && Check(j, checked))
                {
                    min = matrix[i][j];
                    count = j;
                }
            }
            i = count;
            Lmin += min;
        }
        return Lmin;
    }
public:
    AntAlg(int a)
    {
        countVert = a;
        alpha = 4;
        beta = 2;
        p = 0.3;
        ants = 35;
        crazy_ants = 10;
        matrixDist = getMatrix(a);
        visible = getMatrixVisible(a);
        smell = getMatrixSmell(a);
        Lmin = GreedyAlg(a, getMatrix(a));
    }
    void FindSolution()
    {
        srand(time(NULL));
        for (int i = 0; i < 1000; i++)
        {


            vector<vector<int>> matrOfRoad(this->ants + this->crazy_ants, vector<int>(this->countVert));
            vector<double> matrOfLk;
            for (int i = 0; i < ants; i++)
            {
                int Lk = 0;
                double pij;
                //int start_position = rand() % 250;
                int start_position = rand() % 6;
                //  cout << "Start pos = " << start_position << endl;
                vector<int> checkedVert;
                int sumOfRoad = 0;
                int itr = 1;
                matrOfRoad[i][0] = start_position;
                while (checkedVert.size() < this->countVert)
                {
                    if (checkedVert.size() + 1 == this->countVert)
                    {
                        // matr[start_position][0] = 1;
                        Lk += this->matrixDist[start_position][0];

                        break;
                    }

                    checkedVert.push_back(start_position);
                    double max = 1;
                    // vector<double> next_peak;
                    int nextVert = 0;
                    double sum = 0;
                    for (int j = 0; j < this->countVert; j++)
                    {
                        if (CheckVer(j, checkedVert))
                        {
                            sum += pow(this->smell[start_position][j], this->alpha) * pow(this->visible[start_position][j], this->beta);
                        }
                    }
                    double next_vertex_chance = (double)(rand() % 1000 + 1) / 1000;
                    for (int j = 0; j < this->countVert; j++)
                    {
                        if (CheckVer(j, checkedVert))
                        {
                            pij = (pow(this->smell[start_position][j], this->alpha) * pow(this->visible[start_position][j], this->beta)) / sum;
                            if (max > abs(next_vertex_chance - pij))
                            {
                                max = pij;
                                sumOfRoad = this->matrixDist[start_position][j];
                                nextVert = j;
                            }
                        }
                    }
                    Lk += sumOfRoad;
                    matrOfRoad[i][itr] = nextVert;
                    ++itr;
                    start_position = nextVert;
                }
                matrOfLk.push_back(Lk);
            }
            for (int i = this->ants; i < this->crazy_ants + this->ants; i++)
            {
                int Lk = 0;
                double pij;
                //int start_position = rand() % 250;
                int start_position = rand() % 6;
                //  cout << "Start pos = " << start_position << endl;
                vector<int> checkedVert;
                int sumOfRoad = 0;
                int itr = 1;
                matrOfRoad[i][0] = start_position;
                while (checkedVert.size() < this->countVert)
                {
                    if (checkedVert.size() + 1 == this->countVert)
                    {
                        // matr[start_position][0] = 1;
                        Lk += this->matrixDist[start_position][0];

                        break;
                    }

                    checkedVert.push_back(start_position);
                    int nextVert = 0;
                    int next_vertex_chance = rand() % 6;
                    for (int j = 0; j < 1000; j++)
                    {
                        next_vertex_chance = rand() % 6;
                        if (CheckVer(next_vertex_chance, checkedVert))
                        {
                            sumOfRoad = this->matrixDist[start_position][next_vertex_chance];
                            nextVert = next_vertex_chance;
                        }
                    }
                    Lk += sumOfRoad;
                    matrOfRoad[i][itr] = nextVert;
                    ++itr;
                    start_position = nextVert;
                }
                matrOfLk.push_back(Lk);
            }
          /*  cout << "Matr of road \n";
            for (int i = 0; i < this->ants + this->crazy_ants; i++)
            {
                for (int j = 0; j < 6; j++)
                {
                    cout << matrOfRoad[i][j] << " ";
                }
                cout << endl;
            }*/
           /* cout << "MatrixOfLk \n";
            for (int i = 0; i < matrOfLk.size(); i++)
            {
                cout << matrOfLk[i] << " ";
            }*/
            /*cout << "\nSmell before\n";
            for (int i = 0; i < smell.size(); i++)
            {
                for (int j = 0; j < smell.size(); j++)
                {
                    cout << smell[i][j] << " ";
                }
                cout << endl;
            }*/
            for (int i = 0; i < smell.size(); i++)
            {
                for (int j = 0; j < smell.size(); j++)
                {
                    if (i != j)
                        smell[i][j] = (1 - this->p) * smell[i][j];
                }
            }
           /* cout << "\nSmell after1\n";
            for (int i = 0; i < smell.size(); i++)
            {
                for (int j = 0; j < smell.size(); j++)
                {
                    cout << smell[i][j] << " ";
                }
                cout << endl;
            }*/
            for (int i = 0; i < this->ants + this->crazy_ants; i++)
            {
                for (int j = 0; j < 5; j++)
                {
                    smell[matrOfRoad[i][j]][matrOfRoad[i][j + 1]] += this->Lmin / matrOfLk[i];
                }
                smell[matrOfRoad[i][5]][matrOfRoad[i][0]] += this->Lmin / matrOfLk[i];
            }
           
        }
        cout << "Smell after2\n";
        for (int i = 0; i < smell.size(); i++)
        {
            for (int j = 0; j < smell.size(); j++)
            {
                cout << smell[i][j] << " ";
            }
            cout << endl;
        }
    }
   
    void PrintSolution()
    {

    }
};
int main()
{
    int a = 6;
    AntAlg ant(a);
    ant.FindSolution();
   // matrixGenerator(a);
  // FeromGen(a);
  // visiblGen(a, getMatrix(a));
   // cout << "Lmin= " << GreedyAlg(a, getMatrix(a));
   // vector<vector<double>> smell;
    //vector<vector<double>> visible;
    //visible = getMatrixVisible(a);
   // smell = getMatrixSmell(a);
   // cout << endl;
   /* for (int i = 0; i < smell.size(); i++)
    {
        for (int j = 0; j < smell.size(); j++)
        {
            cout << smell[i][j] << " ";
        }
        cout << endl;
    }*/
   /* for (int i = 0; i < visible.size(); i++)
    {
        for (int j = 0; j < visible.size(); j++)
        {
            cout << visible[i][j] << " ";
        }
        cout << endl;
    }*/
}