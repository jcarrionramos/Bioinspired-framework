#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

typedef vector<pair<int,int>> vecii;

void FindAndReplace(string &strTemp, string f, string r){
    for (size_t p = strTemp.find( f ); p != string::npos; p = strTemp.find( f, p ) ){
        strTemp.replace( p, f.length(),  r);
    }
}

string NotExpressed(vecii p){
    string expression;
    for (int i = 0; i < p.size(); i++){
        if(p[i].first == 2) continue;

        if (p[i].first == 0){
            expression.push_back('"');
            expression.push_back('p');
            expression.push_back((i+1)+'0');
            expression.push_back('"');
            expression.push_back(',');
        }
    }
    expression.pop_back();
    return expression;
}

string Expressed(vecii p){
    string expression;
    for (int i = 0; i < p.size(); i++){
        if(p[i].first == 2) continue;

        if (p[i].first == 1){
            expression.push_back('"');
            expression.push_back('p');
            expression.push_back((i+1)+'0');
            expression.push_back('"');
            expression.push_back(',');
        }
    }
    expression.pop_back();
    return expression;
}

string Operons(vecii &p){
    ifstream filein("./sga/genes.txt");
    string operons;
    string strTemp;

    for(int i = 0; i < p.size(); i++){
        while(getline(filein, strTemp)){
            FindAndReplace(strTemp,"XYZ", to_string(i+1));
            strTemp += "\n";
            operons += strTemp;
        }
        operons += "\n";
        filein.clear();
        filein.seekg(0, filein.beg);
    }

    return operons;
}

string Plasmids(vecii &p){
    ifstream filein("./sga/plasmids.txt");
    string plasmids;
    string strTemp;

    for(int i = 0; i < p.size(); i++){
        while(getline(filein, strTemp)){
            FindAndReplace(strTemp,"XYZ", to_string(i+1));
            plasmids += strTemp;
        }
        filein.clear();
        filein.seekg(0, filein.beg);
        plasmids += ",\n    ";
    }
    plasmids.resize(plasmids.size()-6);
    return plasmids;
}

string Conjugations(vecii &p, string c){
    ifstream filein("./sga/conjugations.txt");
    string conjugation;
    string strTemp;

    for(int i = 0; i < p.size(); i++){
        while(getline(filein, strTemp)){
            FindAndReplace(strTemp,"XYZ", to_string(i+1));
            FindAndReplace(strTemp,"CROSSOVER", c);
            strTemp += "\n";
            conjugation += strTemp;
        }
        filein.clear();
        filein.seekg(0, filein.beg);
    }

    return conjugation;
}


string AmountBacteria(vecii &p){
    ifstream filein("./sga/bacteria.txt");
    string bacteria;
    string strTemp;
    int amount = 0;

    for(int i = 0; i < p.size(); i++){
        while(getline(filein, strTemp)){
            FindAndReplace(strTemp,"XYZ", to_string(i+1));
            FindAndReplace(strTemp,"AAA", to_string(p[i].second));
            strTemp += "\n";
            bacteria += strTemp;
        }
        amount += p[i].second;
        filein.clear();
        filein.seekg(0, filein.beg);
        bacteria += "    ";
    }

    string total = "\n    c_ecolis(XYZ, 0, 0, 100, {\"pFitness\"}, program p());";

    FindAndReplace(total, "XYZ", to_string(amount));

    bacteria += total;

    return bacteria;
}

void MakeTemplate(ofstream &fout, vecii &p, string m, string c, string pop_max){
    ifstream filein("./sga/template.txt");

    string noExpression = NotExpressed(p);
    string expression = Expressed(p);
    string operons = Operons(p);
    string plasmids = Plasmids(p);
    string conjugations = Conjugations(p, c);
    string bacteria = AmountBacteria(p);

    string strTemp;
    while(getline(filein, strTemp)) {
        FindAndReplace(strTemp, "POP_MAX", pop_max);
        FindAndReplace(strTemp, "MUTATION", m);
        FindAndReplace(strTemp, "NOT_EXPRESSED", noExpression);
        FindAndReplace(strTemp, "EXPRESSED", expression);
        FindAndReplace(strTemp, "//ADD_THE_OPERONS", operons);
        FindAndReplace(strTemp, "//ADD_PLASMIDS", plasmids);
        FindAndReplace(strTemp, "//ADD_THE_CONJUGATIONS", conjugations);
        FindAndReplace(strTemp, "//ADD_FITNESS_FUCTION", bacteria);
        fout << strTemp;
    }
}

int main() {

    /* READING PARAMETERS */
    int size;
    cout << "SIMPLE GENETIC ALGORITHM GENERATOR\n\nPlease enter the number of proteins needed for the simulation: ";
    cin >> size;

    cout << "\nPlease say for each protein, if it has to be expressed (Y/y/1) or not (N/n/0) and the initial amount\n";
    vecii proteins(size);
    string answer;
    for (int i = 0; i < size; i++) {
        cout << "Protein " << i + 1 << ": ";
        cin >> answer;
        cin >> proteins[i].second;
        
        if (answer == "Y" || answer == "y" || answer == "1") proteins[i].first = 1;
        else if (answer == "N" || answer == "n" || answer == "0") proteins[i].first = 0;
        else proteins[i].first = 2;
    }

    string mutation, crossover;
    cout << "\nNow please type the percentage Now please type the percentage of Mutation and Crossover respectively:\n";
    cout << "Mutation: "; cin >> mutation;
    cout << "Crossover: "; cin >> crossover;

    // mutation /= 100;
    // crossover /= 100;

    string POP_MAX;
    cout <<"\nFinally, please enter the population max of the simulation: ";
    cin >> POP_MAX;
    /* PARAMETERS FINISHED */

    /* STAR MAKING THE gro FILE */
    ofstream output;
    output.open ("output.gro");

    MakeTemplate(output, proteins, mutation, crossover, POP_MAX);

    /* GRO FILE FINISHED */
    output.close();
}
