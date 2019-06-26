#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <fstream>
using namespace std;
typedef vector<int> vctr;

int pop_size = 500;
//vctr pop_experiment = {10,50,100,500,1000,2000,5000};
vctr population(pop_size);
int max_it = 50;
int gen_size = 7;
int n_key = 2;
int rnd_int(int lo, int hi){
    random_device rd;
    mt19937 e(rd());
    static uniform_int_distribution<> dis(lo, hi);
    return dis(e);
}
float rnd_float(float lo, float hi){
    random_device rd;
    mt19937 e(rd());
    static uniform_real_distribution<> dis(lo, hi);
    return dis(e);
}
void rnd_start(){
  for(int i = 0; i < pop_size; ++i){
    population[i] = rnd_int(0, (1ul<<4) - 1);
  }
}
void print_vctr(vctr &v){
  for(int i = 0; i < v.size(); ++i){
    cout<<v[i]<<(i != v.size() - 1 ? ' ':'\n');
  }
}
int fitness_function(int x, int key){
  int value_key = (1ul<< key);
  if (x == value_key)
    return 2;
  else if ((x & value_key) != value_key)
    return 0;
  else if(x == 0)
    return 3;
  else
    return 1;
}
int aux_fitness_function(int x, int key1, int key2){//caso busqueda 2 genes
  int value_key = (1ul<<key1) | (1ul<<key2);
  int aux1 = fitness_function(x, key1);
  int aux2 = fitness_function(x, key2);
  if(x == value_key)
    return 2;
  else if (x == 0)
    return 3;
  else if(aux1 == 0 and aux2 == 0)
    return 0;
  else
    return 1;
}
void crossover(int &x, int &y){
//  if(x == (1ul<<n_key) or y == (1ul<<n_key)){
//      float dice = rnd_float(0,1);
//      if(dice > 0.3){
//        return;
//      }
//  }
  int aux_x = x;
  int aux_y = y;
  for(int i = 0; i < gen_size/2; ++i){
      int a = 1ul<<i;
      if ((x & a) == 0 and (y & a) != 0){
        aux_x |= a;
      }else if((x & a) != 0 and (y & a) == 0){
        aux_x &= (~a);
      }
  }
  for(int i = gen_size/2; i < gen_size; ++i){
      int a = 1ul<<i;
      if ((x & a) != 0 and (y & a) == 0){
        aux_y |= a;
      }else if((x & a) == 0 and (y & a) != 0){
        aux_y &= (~a);
      }
  }
  x = aux_x;
  y = aux_y;
}
int rnd_element(vctr &in){
  vctr out;
  size_t nelems = 1;
  sample(in.begin(), in.end(), std::back_inserter(out),
              nelems, mt19937{random_device{}()});
  return out[0];
}
void evaluation(vctr &count){
  for(int i = 0; i < pop_size; ++i){
    //cout<<fitness_function(population[i])<<(i != pop_size - 1 ? ' ':'\n');
    //int c = fitness_function(population[i], n_key);
    int c = aux_fitness_function(population[i], 2, 1);
    ++count[c];
  }
}
void selection(int key1, int key2, vctr &count){
  //cout<<"POBLACION\n";
  print_vctr(population);
  //EVALUACION
  //cout<<"PUNTAJES: \n";
  //vctr count(3);
  //MATING POOL
  int value_key = (1ul<<key1)|(1ul<<key2);
  vctr mating_pool;
  for(int i = 0; i < count[2]; ++i){
    float dice = rnd_float(0,1);
    if(dice > 0.5)
      //mating_pool.push_back(1ul<<key);
      mating_pool.push_back(value_key);
    else
      mating_pool.push_back((value_key)|rnd_int(0, (1ul<<4) - 1));
  }
  for(int i = 0; i < count[1]; ++i){
    float dice = rnd_float(0,1);
    if(dice > 0.5)
      mating_pool.push_back((value_key)|rnd_int(0, (1ul<<4) - 1));
    else{
      //mating_pool.push_back(rnd_int(0, (1ul<<4) - 1));
      mating_pool.push_back(rnd_element(population));
    }
  }
  for(int i = 0; i < count[0]; ++i){
    float dice = rnd_float(0,1);
    if(dice > 0.5)
      mating_pool.push_back(rnd_element(population));
    else
      mating_pool.push_back((rnd_element(population)|rnd_int(0, (1ul<<4) - 1)));
  }
  for(int i = 0; i < count[3]; ++i){
    float dice = rnd_float(0,1);
    if(dice > 0.5)
      mating_pool.push_back(rnd_element(population));
    else
      mating_pool.push_back((rnd_element(population)|rnd_int(0, (1ul<<4) - 1)));
  }
  population = mating_pool;

  for(int i = 0; i < pop_size/2;++i){
    crossover(population[i], population[pop_size/2 + i]);
  }
}
int sga(float c_point, vctr &count, int key1, int key2){
  //cout<<"Key : "<< (1ul<<n_key)<<'\n';
  for(int i = 0; i < max_it; ++i){
    evaluation(count);
    float media = (2*count[2] + count[1]) / 2;
    //cout<<"PUNTAJE = "<<media<<endl;
    if(media > pop_size * c_point){
      //cout<<"CONVERGE\n";
      return i;
    }
    selection(key1, key2, count);
    count = (i != max_it - 1? vctr(4): count);
  }
  return max_it - 1;
}
int main(){
  ofstream myfile;
  myfile.open ("data_sga.csv");
  float c_point = 0.5;
  for(int i = 0; i < 20; ++i){
    rnd_start();
    vctr count(4);
    int it = sga(c_point, count, 2, 1);
    myfile<<count[2]<<", "<<count[1]<<", "<<count[0]<<", "<<count[3]<<", "<<it<<", ";
    myfile<<n_key<<", "<<pop_size<<", "<<max_it<<", "<<c_point<<", "<<gen_size<<'\n';
    cout<<"converge a "<<it<<" iteraciones\n";
  }

  myfile.close();
  return 0;
}
