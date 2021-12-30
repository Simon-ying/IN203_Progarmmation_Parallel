#include <string>
#include <iostream>  
#include <fstream>
#include <sstream>
using namespace std;
#define MAX_JOUR 500

int main(int argc, char** argv)  
{
    string filename;
    if (argc > 1)
    {
        filename = argv[1];
    }
    else filename = "Temps.dat";
    ifstream infile(filename);  

    string temp;
    if (!infile.is_open())
    {  
        cout << "can not open the file \n" << endl;
        return -1;
    }
    getline(infile, temp);
    cout << temp << endl;
    double sum = 0.0;
    int iter = 0;
    while(getline(infile,temp))  
    {
        iter++;
        sum += stod(temp);
        if (iter >= MAX_JOUR)
        {
            break;
        }
    }
    cout << "Mean Time : " << sum/iter << endl;
    infile.close();
    return 0;  
}