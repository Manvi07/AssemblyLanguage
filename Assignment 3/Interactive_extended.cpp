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

string s;
long long location;
ifstream addressFile;

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

void direct(int cmb, int wordSize);
void fourWay(int cmb, int wordSize);


int main(int argc, char const *argv[])
{

  addressFile.open("address.txt");
  if(!addressFile.is_open())
  {
    cout << "Could not open file!\n";
    return -1;
  }

  int hits = 0, cmb = 0, wordSize = 0, x = 0;
  while(x == 0)
  {
    cout << "Choose wordSize (16, 32): ";
    cin >> x;
    if(x == wordSize1 or x == wordSize2)
    wordSize = x;
    else
    {
      x = 0;
      cout << "!!Wrong choice!!\n";
    }
  }
  int cmb1 = cache1/wordSize;
  int cmb2 = cache2/wordSize;
  int cmb3 = cache3/wordSize;
  int cmb4 = cache4/wordSize;

  while(cmb ==0)
  {
    cout << "Choose cache size (1024, 2048, 4096, 8192): ";
    cin >> x;
    if(x == cache1)
      cmb = cmb1;
    else if(x == cache2)
      cmb = cmb2;
    else if(x == cache3)
      cmb = cmb3;
    else if(x == cache4)
      cmb = cmb4;
    else cout << "!!Wrong choice!!\n";
  }
  x = 0;
  while(!x)
  {
    cout << "Press \n[1] for Direct mapping\n[2] for four-way set associative mapping \n";
    cin >> x;
    if(x == 1 or x == 2)
    break;
    else
    {
      x = 0;
      cout << "!!Wrong choice!!\n";
    }
  }

  switch (x)
  {
    case 1:
    {
      direct(cmb, wordSize);
      break;
    }
    case 2:
    {
      fourWay(cmb, wordSize);
      break;
    }
  }
  return 0;
}

//-------------------------------------Function definitions-----------------------------------

void direct(int cmb, int wordSize)
{
  int hits = 0;
  long long cache[cmb][wordSize] = {0};
  while(addressFile >> s)
  {

    location = dec(s);
    int I = (location/wordSize)%cmb;
    int J = location%wordSize;

    if(cache[I][J] == location)
    {
      hits ++;
      cout << s << endl;
    }
    else
    {
      long long val = location - location%wordSize;
      for(int j=0;j<wordSize; j++)
      {
        cache[I][j] = val+j;
      }
    }
  }
  cout << "No. of hits: " << hits << endl;
  cout << "Enter the element you want to access (in hexadecimal): \n";
  cin >> s;
  long long val = dec(s);

  int I = (location/wordSize)%cmb;
  int J = location%wordSize;

  if(cache[I][J] == val)
  {
    hits ++;
    cout << "The element is in the cache.\n" << endl;
  }
  else
  {
    cout << "Not in cache! Inserting\n";
    long long val = location - location%wordSize;
    for(int j=0;j<wordSize; j++)
    {
      cache[I][j] = val+j;
    }
  }
  return;
}

//--------------------------------------------------------------------------------------
void fourWay(int cmb, int wordSize)
{
  int hits = 0;
  long long cache[cmb/4][4][wordSize];
  int counter[cmb/4][4];
  //Initialize
  for(int i=0; i<cmb/4; i++)
  {
    for(int j=0; j<4; j++)
    counter[i][j] = -1;
  }

  for(int i=0; i<cmb/4; i++)
  {
    for(int j=0; j<4; j++)
    {
      for(int k=0; k<wordSize; k++)
      cache[i][j][k] = 0;
    }
  }
  /////////////////////////////////
  bool is_full[cmb/4] = {false};
  int temp = 0;
  cout << "Press \n[1] for LRU\n[2] for FIFO\n";
  cin >> temp;
  while(addressFile >> s)
  {
    location = dec(s);
    int I = (location/wordSize)%(cmb/4);
    int K = location%wordSize;
    int J;
    bool flag = false;
    int x=0;

    for(int j=0; j<4; j++)
    {
      if(cache[I][j][K] == location)
      {
        // cout << location << " " << s << endl;
        hits++;
        x = j;
        if(temp == 1)
        {
          counter[I][j] = 0;
          for(int j=0; j<4; j++)
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
        // cout << "is_full\n";
        for(int j=0; j<4; j++)
        {
          if(counter[I][j] == -1)
          {
            J = j;
            counter[I][J] ++;
            // cout << I << " " << J << " " << counter[I][J] << endl;
            if(j==3)
            is_full[I] = true;
            break;
          }
          else
          counter[I][j]++;
          // cout << I << " " << j << " " << counter[I][j] << endl;
        }
      }
      else
      {
        //LRU
        // cout << "LRU\n";
        int max = 0;

        for(int j=0; j<4; j++)
        if(counter[I][j]>max)
        {
          max = counter[I][j];
          J = j;
        }

        counter[I][J] = 0;

        for(int j=0; j<4; j++)
        if(J != j)
        counter[I][j]++;
        // for(int j=0; j<4; j++)
        //   cout << I << " " << j << " " << counter[I][j] << endl;
      }
      long long val = location - location%wordSize;
      for(int k=0; k<wordSize; k++)
      {
        cache[I][J][k] = val+k;
        // std::cout << I << " " << J << " " << k << " " << cache[I][J][k] << '\n';
      }

    }
  }
  cout << "No.of hits: " << hits << endl;
  cout << "Enter the element you want to access (in hexadecimal): \n";
  cin >> s;
  long long val = dec(s);
  int I = (location/wordSize)%(cmb/4);
  int K = location%wordSize;
  int J;
  bool flag = false;
  int x = 0;
  for(int j=0; j<4; j++)
  {
    if(cache[I][j][K] == val)
    {
      cout << "Element in cache!" << endl;
      hits++;
      x = j;
      if(temp == 1)
      {
        counter[I][j] = 0;
        for(int j=0; j<4; j++)
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
    std::cout << "Element not in cache. Inserting!" << '\n';
    if(!is_full[I])
    {
      // cout << "is_full\n";
      for(int j=0; j<4; j++)
      {
        if(counter[I][j] == -1)
        {
          J = j;
          counter[I][J] ++;
          // cout << I << " " << J << " " << counter[I][J] << endl;
          if(j==3)
          is_full[I] = true;
          break;
        }
        else
        counter[I][j]++;
        // cout << I << " " << j << " " << counter[I][j] << endl;
      }
    }
    else
    {
      //LRU
      // cout << "LRU\n";
      int max = 0;

      for(int j=0; j<4; j++)
      if(counter[I][j]>max)
      {
        max = counter[I][j];
        J = j;
      }

      counter[I][J] = 0;

      for(int j=0; j<4; j++)
      if(J != j)
      counter[I][j]++;
      // for(int j=0; j<4; j++)
      //   cout << I << " " << j << " " << counter[I][j] << endl;
    }
    long long val = location - location%wordSize;
    for(int k=0; k<wordSize; k++)
    {
      cache[I][J][k] = val+k;
      // std::cout << I << " " << J << " " << k << " " << cache[I][J][k] << '\n';
    }
  }
  return;
}
