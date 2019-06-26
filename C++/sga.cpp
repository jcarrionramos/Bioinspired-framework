//Yerko Ortiz
//2019
//C++17 
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <random>
#include <fstream>
#include <string>

using namespace std;

typedef vector<int> vctr;
typedef pair<int, vctr> individual;
typedef vector<individual> pop;

const int POP_SIZE = 100; // Tamaño de la poblacion
const double MUT_RATE = 0.1; // Rate de mutaciones
const int CROSS_RATE = 1; // Rate de crossover
const int GEN_SIZE = 4; // Tamaño del gen(target)
const int MAX_IT = 100; // Numero de iteraciones
const int N_EXPERIMENTS = 10; // Numero de csv's a generar
const int TARGET[] {1,1,0,0}; // Combinacion buscada
/*
  0 -> no quiero que exista proteina en el target[i]
  1 -> quiero que exista una proteina en el target[i]
  2 -> soy indiferente si hay o no hay proteina en el target[i]
*/
const string PATH_OUTPUT = "/Users/jcarrion/Downloads/SGA/v1/Mutation/10";
/*  
    Path de escritura(Especificar solamente la carpeta)
    Los archivos se generan y guardan de manera automatica
*/


void print_population(pop &population){
    for(int i(0); i < POP_SIZE; ++i){
        cout<<"Gen "<<i + 1<<": ";
        for(int j(0); j < GEN_SIZE; ++j){
            cout<<population[i].second[j];
            //cout<<(j == GEN_SIZE - 1? "] ": ", ");
            cout<<' ';
        }
        cout<<"Valor: "<<population[i].first<<'\n';
    }
}

int fitness_evaluation(vctr &v){
    int c(0);
    for(int i(0); i < GEN_SIZE; ++i){
        if(TARGET[i] == 2) continue;
        if(v[i] != TARGET[i]) c++;
    }
    return c;
}

int rnd_int(int lo, int hi){
    random_device rd; mt19937 e(rd());
    static uniform_int_distribution<> dis(lo, hi);
    return dis(e);
}

float rnd_float(float lo, float hi){
    random_device rd; mt19937 e(rd());
    static uniform_real_distribution<> dis(lo, hi);
    return dis(e);
}

void mutate_gen(vctr &v){
    int index(rnd_int(0, GEN_SIZE - 1));
    if (v[index] == 1) v[index] = 0;
    else v[index] = 1;
    //~ es el simbolo del operador logico NOT.
}

void crossover(vctr &p1, vctr &p2, vctr &offspring){
    for(int i(0); i < GEN_SIZE; ++i){
        float dice = rnd_float(0, 1);
        if (dice < 0.5) offspring[i] = p1[i];
        else offspring[i] = p2[i];
    }
}

string new_generation(pop &population, int it){
    pop auxpop;
    int best_fraction = (10 * POP_SIZE) / 100;
    for(int i(0); i < best_fraction; ++i){
        auxpop.push_back(population[i]);
    }
    string s = "";
    int c(0);
    int fittest_individual = (CROSS_RATE * POP_SIZE) / 100;
    int cross_fraction = 90 * POP_SIZE / 100;
    for(int i(0); i < cross_fraction; ++i){
        int index1 = rnd_int(0, fittest_individual);
        int index2 = rnd_int(0, fittest_individual);
        vctr offspring(GEN_SIZE, 0);
        crossover(population[index1].second, population[index2].second, offspring);
        float dice = rnd_float(0,1);
        if (dice < MUT_RATE){
            mutate_gen(offspring);
        }
        int f_value = fitness_evaluation(offspring);
        if(f_value == 0) c++;
        auxpop.push_back(make_pair(f_value, offspring));
    }
    population = auxpop;
    return to_string(it) + ", " + to_string(c) + '\n';
}

int main(){    
    for(int k(0); k < N_EXPERIMENTS; ++k){ 
        fstream file;
        string s_index = to_string(k + 1);
        file.open(PATH_OUTPUT + "/simu" + s_index + ".csv", fstream::out);
        pop population(POP_SIZE);
        int c(0);
        for(int i(0); i < POP_SIZE; ++i){
            vctr v(GEN_SIZE);
            v[rnd_int(0, GEN_SIZE - 1)] = 1;
            int f_value = fitness_evaluation(v);
            if (f_value == 0) c++;
            population[i] = make_pair(f_value, v);
        }

        string s = s_index + ", " + (to_string(c)) + "\n";
        file<<s;
        
        for(int i(0); i < MAX_IT; ++i){
            sort(population.begin(), population.end());
            file<<new_generation(population, i + 2);
        } 
        sort(population.begin(), population.end());  
        file.close();
    }
    return 0;
}