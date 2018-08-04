#include<string>
#include<cmath>
#include "svm.h"
#include "utils.hpp"
using namespace std;
SVM::SVM(){}
SVM::SVM(const string& file_name,float C, const string&  kernel, float precision)
{
    this->init(file_name, C, kernel, precision);
}
SVM::~SVM()
{
  delete [] this->alpha;
  delete [] this->E;
  delete [] this->label;
  for(int i=0;i<this->sample_n;i++)
  {
    delete[] this->sample[i];
  }
  delete[] this->sample;
}
void SVM::init(const string& file_name,float C, const string& kernel, float precision)
{ 
   load_file(file_name, &this->sample, &this->label, this->sample_n, this->field_n);
   this->C = C;
   this->alpha = new float[sample_n]();
   this->E = new float[sample_n]();
   this->kernel = kernel;
   this->b = 0;
   this->precision = precision;
   this->theta = precision;
}
void SVM::smo(int iter_num, float precision)
{
  for(int i=0;i<iter_num;i++)
  {
    int var_1 = -1;
    if(this->check_KKT(var_1))return;
    int var_2 = this->select_var2(var_1);
    this->solve_min(var_1, var_2);
    this->compute_E();
  }
}

void SVM::solve_min(int var_1, int var_2)
{
  float delta = this->compute_kernel(var_1,var_1) + this->compute_kernel(var_2,var_2) - 2*this->compute_kernel(var_1,var_2);
  float alpha_new_2=0;
  if(abs(this->E[var_1]-this->E[var_2]-delta)<this->theta)
    alpha_new_2 = this->alpha[var_2] + this->label[var_2];
  else
    alpha_new_2 = this->alpha[var_2] + (this->label[var_2]*(this->E[var_1]-this->E[var_2]))/delta;
  this->clop_alpha(var_1,var_2,alpha_new_2);
}
void SVM::clop_alpha(int var_1, int var_2, float new_alpha_2)
{
  float alpha_1_old = this->alpha[var_1];
  float alpha_2_old = this->alpha[var_2];
  float theta = this->alpha[var_1]*this->label[var_1] + this->alpha[var_2]*this->label[var_2];
  if(abs(this->label[var_1]-this->label[var_2])<this->theta)
  {
    float L = 0;
    float H = this->C;
    if(L<(this->alpha[var_1]+this->alpha[var_2]-this->C))
       L = this->alpha[var_1]+this->alpha[var_2]-this->C;   
    if(H>this->alpha[var_1]+this->alpha[var_2])
       H = this->alpha[var_1]+this->alpha[var_2];   
    if(L>new_alpha_2)
      this->alpha[var_2] = L;
    else
      this->alpha[var_2] = new_alpha_2;
    if(H<new_alpha_2)
      this->alpha[var_2] = H;
    else
      this->alpha[var_2] = new_alpha_2;
  } else
  {
    float L = 0;
    float H = this->C;
    if(L<this->alpha[var_2]-this->alpha[var_1])
       L = this->alpha[var_2]-this->alpha[var_1];   
    if(H>this->C-this->alpha[var_1]+this->alpha[var_2])
       H = this->C-this->alpha[var_1]+this->alpha[var_2];   
    if(L>new_alpha_2)
      this->alpha[var_2] = L;
    else
      this->alpha[var_2] = new_alpha_2;
    if(H<new_alpha_2)
      this->alpha[var_2] = H;
    else
      this->alpha[var_2] = new_alpha_2;
  } 
  this->alpha[var_1] = (theta-this->label[var_2]*this->alpha[var_2])*this->label[var_1];
  float b1 = -this->E[var_1]-this->label[var_1]*this->compute_kernel(var_1,var_1)*(this->alpha[var_1]-alpha_1_old)-this->label[var_2]*this->compute_kernel(var_2,var_1)*(this->alpha[var_2]-alpha_2_old) + this->b;
  float b2 = -this->E[var_2]-this->label[var_1]*this->compute_kernel(var_1,var_2)*(this->alpha[var_1]-alpha_1_old)-this->label[var_2]*this->compute_kernel(var_2,var_2)*(this->alpha[var_2]-alpha_2_old) + this->b;
  this->b = (b1+b2)/2;
}

float SVM::compute_v(int index, int var_1, int var_2)
{
  float sum = this->alpha[var_1]*this->label[var_1]*this->compute_kernel(index,var_1) + this->alpha[var_2]*this->label[var_2]*this->compute_kernel(index,var_2) + this->b;
  return this->compute_gxi(index) - sum;
}

float SVM::compute_kernel(int index_1, int index_2)
{
  float sum=0;
  if(this->kernel=="none" || this->kernel=="None" || this->kernel=="NONE")
  {
    for(int i=0;i<this->field_n;i++)
    {
      sum+=this->sample[index_1][i]*this->sample[index_2][i];
    }
  }
  return sum;
}
float SVM::compute_kernel_feature(float* index_1, float* index_2)
{
  float sum=0;
  if(this->kernel=="none" || this->kernel=="None" || this->kernel=="NONE")
  {
    for(int i=0;i<this->field_n;i++)
    {
      sum+=index_1[i]*index_2[i];
    }
  }
  return sum;
}
void SVM::compute_E()
{
  for(int i=0;i<this->sample_n;i++)
  {
    this->E[i] = this->compute_gxi(i)-this->label[i];
  }
}

int SVM::select_var2(int index)
{
  if(this->E[index]>=0)
     return this->min_E(index);
  else
     return this->max_E(index);
}

int SVM::min_E(int id)
{
  int index = 0;
  float min = this->E[0];
  for(int i=1;i<this->sample_n;i++)
  {
      if(min>this->E[i] && id!=i)
      {
          min = this->E[i];
          index = i;
      }
  }
  return index;
}

int SVM::max_E(int id)
{
  int index = 0;
  float max = this->E[0];
  for(int i=1;i<this->sample_n;i++)
  {
      if(max<this->E[i] && id!=i)
      {
          max = this->E[i];
          index = i;
      }
  }
  return index;
}

bool SVM::check_KKT(int& index_2)
{
  int index_1 = -1;
  int index_3 = -1;
  for(int i=0;i<this->sample_n;i++)
  {
    // KKT 2 sastifier
    float gxi = this->compute_gxi(i)*this->alpha[i];
    
    if(this->alpha[i]>0 && this->alpha[i]<this->C)
    {
      if(abs(gxi-1.0)>theta)
      {
         index_2 = i;
         return false;
      }
    }
    if(abs(this->alpha[i])<this->theta)
    {
      if(gxi<1.0)
      {
         index_1 = i;
      }
    }
    if(abs(this->alpha[i]-this->C)<this->theta)
    {
      if((gxi)>1.0)
      {
         index_3 = i;
      }
    }
    
  }
  if(index_1>-1)
  {
    index_2 = index_1;
    return false;
  }
  if(index_3>-1)
  {
    index_2 = index_3;
    return false;
  }
  return true;
}

float SVM::compute_gxi(int index)
{
  float sum=0;
  for(int i=0;i<this->sample_n;i++)
  {
     sum += this->alpha[i]*this->label[i]*this->compute_kernel(index,i);
  }
  return sum+this->b;
}

void SVM::save_model(const string& path)
{
}

void SVM::load_model(const string& path)
{
}

float SVM::predict(float* feature)
{
  float sum = 0;
  for(int i=0;i<this->sample_n;i++)
  {
    sum += this->alpha[i]*this->label[i]*this->compute_kernel_feature(feature,this->sample[i]);
  }
  for(int i=0;i<this->sample_n;i++)
  {
    cout<< this->E[i]<<"\t"<<this->alpha[i]<<endl;
  }
  cout<<sum+this->b<<endl;
  return sum+this->b;
}
