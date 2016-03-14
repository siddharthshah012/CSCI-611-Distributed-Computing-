#include "goldchase.h"
#include "Map.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<fcntl.h>


using namespace std;




struct GameBoard
{
  int rows; //4 bytes
  int cols; //4 bytes
  unsigned char players;
  unsigned char map[0];
};

//struct GameBoard gBoard

int main()
{
  string goldNumber;
  ifstream mystream;
  string mapColSize;
  string stringLength;
  srand(time(NULL));
  //char ch;
  //char storeStar[100];
  //std::vector<char> v[100];
  //int goldNumber;
  mystream.open("mymap.txt");
  int rowNumber=0;
  int rowFinal = 0;
  int colNumber ;

  sem_t *sem_pointer;

//semaphore, O_CREAT|O_EXCL (because this will fail if sem
//exists, which means your are not the first player)


//first player
//sem_wait




  while (!mystream.eof())
  {

    //int i=0;
    getline(mystream,goldNumber);

    //cout<< "Gold number : " <<goldNumber <<endl;



    while (getline(mystream, mapColSize))
    {
      rowNumber++;
      //cout<<"mapColSize : "<< mapColSize <<endl;

      colNumber = mapColSize.size();
    }



  }
  //cout <<"Column Number : " << colNumber<<endl;
  //cout <<"Row Number : " << rowNumber<< endl;


  mystream.close();

  int int_goldNumber = atoi (goldNumber.c_str());
  int mulRowCol = colNumber * rowNumber;

  sem_pointer = sem_open("/Semaphore_open",O_RDWR|O_CREAT, S_IRUSR|S_IWUSR,1);
  sem_wait(sem_pointer);
  int fd=shm_open("/toddsGoldMem", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
  ftruncate(fd,mulRowCol + sizeof(GameBoard));

  GameBoard* goldmap= (GameBoard*)mmap(NULL, mulRowCol+ sizeof(GameBoard), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  goldmap->rows=rowNumber;
  goldmap->cols=colNumber;

  ifstream myfile;


  myfile.open("mymap.txt");

  char ch;
  char arrayMap[mulRowCol];
  string ignoreNumber;
  getline(myfile, ignoreNumber);

  while (!myfile.get(ch).eof())
  {
    //get(ch);
    //for (int i =0 ; i < mulRowCol ; i++)
    int i =0;
    arrayMap[i] = ch;
    i++;
  }

  myfile.close();

  /*char byte = 0;
  if (ch == '*')
  {
    byte|=G_WALL;
  }*/

  int j = 0;

  while (/*arrayMap[j] != '\0'*/ j <= sizeof(arrayMap))
  {
    if (arrayMap[j] == ' ')
    {
      //GameBoard->Map[j] = 0;
      goldmap->map[j] = 0;
    }
    else if (arrayMap[j] == '*')
    {
      goldmap->map[j] = G_WALL;
    }
    else if (arrayMap[j] == 'G')
    {
      goldmap->map[j] = G_GOLD;
    }
    else if (arrayMap[j] == 'F')
    {
      goldmap->map[j] = G_FOOL;
    }
    j++;
  }
  //goldmap->map[37];
  /////////////////////////
  Map goldMine(goldmap->map, goldmap->rows,goldmap->cols);
  /////////////

  /*int randomPosition = 0;
  int a;
  int b;

  //sem_wait(sem_ptr);

  for (a = 0; a < int_goldNumber-1 ; a++ )
  {
    randomPosition = rand()% sizeof(arrayMap);
    while (goldmap->map[randomPosition] != 0)
    {
      randomPosition = rand()%sizeof(arrayMap);
    }
    goldmap->map[randomPosition] = G_FOOL;
  }
  randomPosition = rand()% sizeof(arrayMap);

  while (goldmap->map[randomPosition] != 0)
  {
    randomPosition= rand()% sizeof(arrayMap);
  }

  if (int_goldNumber >0)
  {
    goldmap->map[randomPosition] = G_GOLD;
  }*/





  return 0;
}
