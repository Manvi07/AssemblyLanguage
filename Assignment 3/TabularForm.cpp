/* CS 201P Computer Organizartion Laboratory
 * Assignment 7
 * Topic: Memory Unit
 * Author: Manvi Gupta(B17092)
 */

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#define cache1 1024
#define cache2 2048
#define cache3 4096
#define cache4 8192

#define wordSize1 16
#define wordSize2 32

#define set 4

string s;
long long location;
ifstream addressFile;

void direct(int cmb, int wordSize);
void fourWay(int cmb, int wordSize, int temp);

long long dec(string s)
{
  long long base = 1;
  long long a = 0;
  for(int i=s.size()-1; i>=0; i--)
  {
    if(s[i]>='0' and s[i]<='9')
    {
      a += (s[i] - 48)*base;
      base *= 16;
    }
    else if(s[i]>='A' and s[i] <= 'F')
    {
      a += (s[i] - 55)*base;
      base *= 16;
    }
  }
  return a;
}

int main(int argc, char const *argv[])
{
  addressFile.open("address.txt");
  if(!addressFile.is_open())
  {
    cout << "Could not open file!\n";
    return -1;
  }

  int wordSize[] = {wordSize1, wordSize2};
  cout << "--------------------------------------------- Direct Method --------------------------------------------------\n\n";

  cout << "Wordsize" << "   \t " << cache1 << "\t\t\t" << cache2 << "\t\t\t" << cache3 << "\t\t\t" << cache4 << '\n';
  for(int i=0; i<2; i++)
    {
      cout << wordSize[i] << "\t\t";
      direct(cache1, wordSize[i]);
      direct(cache2, wordSize[i]);
      direct(cache3, wordSize[i]);
      direct(cache4, wordSize[i]);
      cout << endl;
    }

  cout << "\n-------------------------------------- Four way set associative: 16 byte -------------------------------------\n\n";
  cout << "Method" << "   \t " << cache1 << "\t\t\t" << cache2 << "\t\t\t" << cache3 << "\t\t\t" << cache4 << '\n';

  cout <<"FIFO" << "\t\t";
  fourWay(cache1, wordSize1, 0);
  fourWay(cache2, wordSize1, 0);
  fourWay(cache3, wordSize1, 0);
  fourWay(cache4, wordSize1, 0);
  cout << endl;
  cout << "LRU\t\t";
  fourWay(cache1, wordSize1, 1);
  fourWay(cache2, wordSize1, 1);
  fourWay(cache3, wordSize1, 1);
  fourWay(cache4, wordSize1, 1);
  cout << endl;

  cout << "\n-------------------------------------- Four way set associative: 32 byte -------------------------------------\n\n";
  cout << "Method" << "   \t " << cache1 << "\t\t\t" << cache2 << "\t\t\t" << cache3 << "\t\t\t" << cache4 << '\n';

  cout <<"FIFO" << "\t\t";
  fourWay(cache1, wordSize2, 0);
  fourWay(cache2, wordSize2, 0);
  fourWay(cache3, wordSize2, 0);
  fourWay(cache4, wordSize2, 0);
  cout << endl;
  cout << "LRU\t\t";
  fourWay(cache1, wordSize2, 1);
  fourWay(cache2, wordSize2, 1);
  fourWay(cache3, wordSize2, 1);
  fourWay(cache4, wordSize2, 1);
  cout << endl;
  return 0;
}

//-------------------------------------Function definitions-----------------------------------

void direct(int c, int wordSize)
{
  int cmb = c/wordSize;
  int hits = 0, miss = 0;
  long long cache[cmb][wordSize] = {0};
  addressFile.clear();
  addressFile.seekg(0, ios::beg);
  while(addressFile >> s)
  {
    location = dec(s);
    int I = (location/wordSize)%cmb;
    int J = location%wordSize;

    if(cache[I][J] == location)
    {
      hits ++;
    }
    else
    {
      long long val = location - location%wordSize;
      miss++;
      for(int j=0;j<wordSize; j++)
      {
        cache[I][j] = val+j;
      }
    }
  }
  cout << miss << "/" << miss+hits << "\t\t";
  return;
}


void fourWay(int c, int wordSize, int temp)
{
  int hits = 0, miss = 0;
  int cmb = c/wordSize;
  long long cache[cmb/set][set][wordSize];
  int counter[cmb/set][set];
  //Initialize
  for(int i=0; i<cmb/set; i++)
  {
    for(int j=0; j<set; j++)
    counter[i][j] = -1;
  }

  for(int i=0; i<cmb/set; i++)
  {
    for(int j=0; j<set; j++)
    {
      for(int k=0; k<wordSize; k++)
      cache[i][j][k] = 0;
    }
  }
  /////////////////////////////////
  bool is_full[cmb/set] = {false};
  addressFile.clear();
  addressFile.seekg(0, ios::beg);
  while(addressFile >> s)
  {
    location = dec(s);
    int I = (location/wordSize)%(cmb/set);
    int K = location%wordSize;
    int J;
    bool flag = false;
    int x=0;

    for(int j=0; j<set; j++)
    {
      if(cache[I][j][K] == location)
      {
        hits++;
        x = j;
        if(temp == 1)
        {
          //LRU
          counter[I][j] = 0;
          for(int j=0; j<set; j++)
          {
            if(x!=j)
            counter[I][j]++;
          }
        }
        flag = true;
        break;
      }
    }
    if(!flag)
    {
      if(!is_full[I])
      {
        for(int j=0; j<set; j++)
        {
          if(counter[I][j] == -1)
          {
            J = j;
            counter[I][J] ++;
            if(j==set-1)
            is_full[I] = true;
            break;
          }
          else
          counter[I][j]++;
        }
      }
      else
      {
        int max = 0;
        for(int j=0; j<set; j++)
        if(counter[I][j]>max)
        {
          max = counter[I][j];
          J = j;
        }
        counter[I][J] = 0;

        for(int j=0; j<set; j++)
        if(J != j)
        counter[I][j]++;
      }
      long long val = location - location%wordSize;
      miss++;
      for(int k=0; k<wordSize; k++)
      {
        cache[I][J][k] = val+k;
      }
    }
  }
  cout << miss << "/" << miss+hits << "\t\t";
  return;
}
