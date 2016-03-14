#include "goldchase.h"
#include "Map.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <errno.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdlib>


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
  GameBoard* goldmap;
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

  sem_t* sem_pointer;

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
  //cout<< mulRowCol <<endl;


  sem_pointer = sem_open("/Semaphore_open",O_CREAT | O_EXCL, S_IRUSR|S_IWUSR,1);
  //shm_unlink("/toddsGoldMem");
  //sem_close(sem_pointer);
  //sem_unlink("/Semaphore_open");

  if (sem_pointer == SEM_FAILED)
  {
    if (errno != EEXIST)
    {
        perror ("Opening Semaphore");


        exit(1);
    }
  }
  if(sem_pointer != SEM_FAILED)
  {
  //sem_wait(sem_pointer);
  int fd=shm_open("/toddsGoldMem", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
  ftruncate(fd,mulRowCol + sizeof(GameBoard));

  goldmap= (GameBoard*)mmap(NULL, mulRowCol+ sizeof(GameBoard), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  goldmap->rows=rowNumber;
  goldmap->cols=colNumber;

  ifstream myfile;


  myfile.open("mymap.txt");

  char ch;
  char arrayMap[mulRowCol];
  string ignoreNumber;
  getline(myfile, ignoreNumber);
  int i =0;

  while (!myfile.get(ch).eof())
  {

    //get(ch);
    //for (int i =0 ; i < mulRowCol ; i++)

    //cout<< "character" << ch << endl;
	if(ch==' ' || ch=='*'){
    arrayMap[i] = ch;
    //cout << i;
    //cout<<
    i++;}
  }

  myfile.close();

  /*char byte = 0;
  if (ch == '*')
  {
    byte|=G_WALL;
  }*/

  int j = 0;

  while (j < mulRowCol /*j <= sizeof(arrayMap)*/)
  {
    if (arrayMap[j] == ' ')
    {
      //GameBoard->Map[j] = 0;
      goldmap->map[j] = 0;
	//j++;

    }
    else if (arrayMap[j] == '*')
    {
      goldmap->map[j] = G_WALL;
 	//j++;

    }
    j++;

  }


  Map goldMine((const char*) goldmap->map, goldmap->rows,goldmap->cols);

  goldMine.drawMap();
  int randomPosition = 0;
  int a;
  int b;

  //sem_wait(sem_ptr);

  for (a = 0; a < int_goldNumber-1 ; a++ )
  {
    randomPosition = rand()% mulRowCol;
    while (goldmap->map[randomPosition] != 0)
    {
      randomPosition = rand()%mulRowCol;
    }
    goldmap->map[randomPosition] = G_FOOL;
  }
  randomPosition = rand()% mulRowCol;

  while (goldmap->map[randomPosition] != 0)
  {
    randomPosition= rand()% mulRowCol;
  }

  if (int_goldNumber >0)
  {
    goldmap->map[randomPosition] = G_GOLD;
  }
  goldMine.drawMap();


  while (goldmap->map[randomPosition] != 0)
  {
    randomPosition = rand()%mulRowCol;
  }
  goldmap->map[randomPosition] = G_PLR0;
  goldMine.drawMap();

  char key = goldMine.getKey();

  int currentPosition = randomPosition;
  //int currentcolumn = currentPosition % goldmap->cols;
  //int currentRow = currentPosition / goldmap->cols;

  goldmap->players |=G_PLR0;
sem_post(sem_pointer);
  //cerr<<currentPlayer;

int flag = 0;
  while (key != 'Q')
  {
    sem_wait(sem_pointer);

    if (key == 'h')
    {
    if ((currentPosition % goldmap->cols) != 0)
    {
        if (goldmap->map[currentPosition-1] == G_WALL)
        {
            goldmap->map[currentPosition] |= G_PLR0;
        }

        else if (goldmap->map[currentPosition-1] == G_GOLD)
        {

            goldmap->map[currentPosition-1] |= G_PLR0;
            goldmap->map[currentPosition] &=~ G_PLR0;
            flag = 1;
            goldmap->map[currentPosition] &=~ G_GOLD;
            currentPosition= currentPosition-1;
            goldMine.drawMap();
            goldMine.postNotice ("Congratulations You Have found the real gold. Move to the edge of the map to exit!");
        }
        else if (goldmap->map[currentPosition-1]== G_FOOL)
        {
            goldmap->map[currentPosition-1] |= G_PLR0;
            goldmap->map[currentPosition] &=~ G_PLR0;
            currentPosition= currentPosition-1;
            goldMine.drawMap();
            goldMine.postNotice ("You have found Fool's Gold");
        }

        else
        {
            goldmap->map[currentPosition-1] |= G_PLR0;
            goldmap->map[currentPosition] &=~ G_PLR0;
            currentPosition= currentPosition-1;
        }
        goldMine.drawMap();

    }
    else if (flag == 1)
    {
        goldmap->map[currentPosition] &=~ G_PLR0;
        goldMine.drawMap();
        goldMine.postNotice ("Congrats! You have won!!!");
        goldmap->players &=~ G_PLR0;
        break;

    }
    }
    else if (key == 'j')
    {
        //currentRow-1;

        if ((currentPosition + goldmap->cols )<mulRowCol)
        {
            if (goldmap->map[currentPosition+ goldmap->cols] == G_WALL)
            {
                goldmap->map[currentPosition] |= G_PLR0;
            }

            else if (goldmap->map[currentPosition + goldmap->cols] == G_GOLD)
            {
                goldmap->map[currentPosition+goldmap->cols] |= G_PLR0;
                goldmap->map[currentPosition] &=~ G_PLR0;
                currentPosition= currentPosition+goldmap->cols;
                flag = 1;
                 goldmap->map[currentPosition] &=~ G_GOLD;
                goldMine.drawMap();
                goldMine.postNotice ("Congratulations You Have found the real gold. Move to the edge of the map to exit!");
            }

            else if (goldmap->map[currentPosition + goldmap->cols] == G_FOOL)
            {
                goldmap->map[currentPosition+goldmap->cols] |= G_PLR0;
                goldmap->map[currentPosition] &=~ G_PLR0;
                currentPosition= currentPosition+goldmap->cols;
                goldMine.drawMap();
                goldMine.postNotice ("You found Fool's Gold");
            }

            else
            {
                goldmap->map[currentPosition+goldmap->cols] |= G_PLR0;
                goldmap->map[currentPosition] &=~ G_PLR0;
            //goldmap->
                currentPosition= currentPosition+goldmap->cols;

            }
        }
        else if (flag == 1)
    {
        goldmap->map[currentPosition] &=~ G_PLR0;
        goldMine.drawMap();
        goldMine.postNotice ("Congrats! You have won!!!");
        goldmap->players &=~ G_PLR0;
        break;

    }



        goldMine.drawMap();

    }
    else if (key == 'k')
    {
        //currentRow+1;
        if ((currentPosition-goldmap->cols)> 0)
        {

            if (goldmap->map[currentPosition-goldmap->cols] == G_WALL)
            {
                goldmap->map[currentPosition] |= G_PLR0;
            }

            else if (goldmap->map[currentPosition-goldmap->cols] == G_GOLD)
            {
                goldmap->map[currentPosition-goldmap->cols] |= G_PLR0;
                goldmap->map[currentPosition] &=~ G_PLR0;
                currentPosition= currentPosition-goldmap->cols;
                flag = 1;
                goldmap->map[currentPosition] &=~ G_GOLD;
                goldMine.drawMap();
                goldMine.postNotice ("Congratulations You Have found the real gold. Move to the edge of the map to exit!");
            }
            else if (goldmap->map[currentPosition-goldmap->cols] == G_FOOL)
            {
                goldmap->map[currentPosition-goldmap->cols] |= G_PLR0;
                goldmap->map[currentPosition] &=~ G_PLR0;
                currentPosition= currentPosition-goldmap->cols;
                goldMine.drawMap();
                goldMine.postNotice ("You have found Fool's Gold");
            }

            else
            {
                goldmap->map[currentPosition-goldmap->cols] |= G_PLR0;
                goldmap->map[currentPosition] &=~ G_PLR0;
                currentPosition= currentPosition-goldmap->cols;
            }
        }

else if (flag == 1)
    {
        goldmap->map[currentPosition] &=~ G_PLR0;
        goldMine.drawMap();
        goldMine.postNotice ("Congrats! You have won!!!");
        goldmap->players &=~ G_PLR0;
        break;

    }


        goldMine.drawMap();
    }
    else if (key == 'l')
    {
        //currentcolumn+1;
        if (currentPosition%(goldmap->cols) != (goldmap->cols-1))
        {
        if (goldmap->map[currentPosition+1] == G_WALL)
        {
            goldmap->map[currentPosition] |= G_PLR0;
        }
        else if (goldmap->map[currentPosition+1] == G_GOLD)
        {
            goldmap->map[currentPosition+1] |= G_PLR0;
            goldmap->map[currentPosition] &=~ G_PLR0;
            currentPosition= currentPosition+1;
            flag = 1;
            goldmap->map[currentPosition] &=~ G_GOLD;
            goldMine.drawMap();
            goldMine.postNotice ("Congratulations You Have found the real gold. Move to the edge of the map to exit!");
        }
        else if (goldmap->map[currentPosition+1] == G_FOOL)
        {
            goldmap->map[currentPosition+1] |= G_PLR0;
            goldmap->map[currentPosition] &=~ G_PLR0;
            currentPosition= currentPosition+1;
            goldMine.drawMap();
            goldMine.postNotice ("You have found Fool's Gold");
        }

        else
        {
            goldmap->map[currentPosition+1] |= G_PLR0;
            goldmap->map[currentPosition] &=~ G_PLR0;
            currentPosition= currentPosition+1;
        }
        }
        else if (flag == 1)
        {
        goldmap->map[currentPosition] &=~ G_PLR0;
        goldMine.drawMap();
        goldMine.postNotice ("Congrats! You have won!!!");
        goldmap->players &=~ G_PLR0;
        break;

        }

        goldMine.drawMap();
   }
   key = goldMine.getKey();

    sem_post(sem_pointer);

  }
    goldmap->map[currentPosition] &=~ G_PLR0;
    goldmap->players &=~ G_PLR0;
}

else
{
    sem_pointer = sem_open("/Semaphore_open",O_RDWR, S_IRUSR|S_IWUSR,1);
    int fd=shm_open("/toddsGoldMem", O_RDWR, S_IRUSR|S_IWUSR);
    //ftruncate(fd,mulRowCol + sizeof(GameBoard));
    int row;
    int col;
    read(fd,&row,sizeof(int));
    read(fd,&col,sizeof(int));


    int mulRowCol1 = row*col;
    goldmap= (GameBoard*)mmap(NULL, mulRowCol1+ sizeof(GameBoard), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);


    Map goldMine((const char*) goldmap->map, row,col);
    unsigned char currentPlayer = 0;


    // To assign players

    if (!(goldmap->players & G_PLR0))
    {
        currentPlayer = G_PLR0;
    }
    else if (!(goldmap->players & G_PLR1))
    {
        currentPlayer = G_PLR1;
    }
    else if (!(goldmap->players & G_PLR2))
    {
        currentPlayer = G_PLR2;
    }
    else if (!(goldmap->players & G_PLR3))
    {
        currentPlayer = G_PLR3;
    }
    else if (!(goldmap->players & G_PLR4))
    {
        currentPlayer = G_PLR4;
    }
  //cerr<<currentPlayer;
   // goldmap->players |= currentPlayer;

    int randomPosition = rand() % mulRowCol;

    while (goldmap->map[randomPosition] != 0)
    {
        randomPosition = rand()%mulRowCol;
    }
    goldmap->map[randomPosition] |= currentPlayer;
    goldMine.drawMap();


    int currentPosition = randomPosition;
    int key;
  //int currentcolumn = currentPosition % goldmap->cols;
  //int currentRow = currentPosition / goldmap->cols;

  goldmap->players |= currentPlayer;


int flag = 0;


  while (key != 'Q')
  {
    sem_wait(sem_pointer);

    if (key == 'h')
    {
    if (currentPosition % goldmap->cols != 0)
    {
        if (goldmap->map[currentPosition-1] == G_WALL)
        {
            goldmap->map[currentPosition] |= currentPlayer;
        }

        else if (goldmap->map[currentPosition-1] == G_GOLD)
        {

            goldmap->map[currentPosition-1] |= currentPlayer;
            goldmap->map[currentPosition] &=~ currentPlayer;
            currentPosition= currentPosition-1;
            flag = 1;
            goldmap->map[currentPosition] &=~ G_GOLD;
            goldMine.drawMap();
            goldMine.postNotice ("Congratulations You Have found the real gold. Move to the edge of the map to exit!");
        }
        else if (goldmap->map[currentPosition-1]== G_FOOL)
        {
            goldmap->map[currentPosition-1] |= currentPlayer;
            goldmap->map[currentPosition] &=~ currentPlayer;
            currentPosition= currentPosition-1;
            goldMine.drawMap();
            goldMine.postNotice ("You have won Fool's Gold");
        }

        else
        {
            goldmap->map[currentPosition-1] |= currentPlayer;
            goldmap->map[currentPosition] &=~ currentPlayer;
            currentPosition= currentPosition-1;
        }
        }
         else if (flag == 1)
        {
        goldmap->map[currentPosition] &=~ currentPlayer;
        goldMine.drawMap();
        goldMine.postNotice ("Congrats! You have won!!!");
        goldmap->players &=~ currentPlayer;
        break;

        }
        goldMine.drawMap();


    }
    else if (key == 'j')
    {
        //currentRow-1;

        if ((currentPosition + goldmap->cols )<mulRowCol)
        {
            if (goldmap->map[currentPosition+ goldmap->cols] == G_WALL)
            {
                goldmap->map[currentPosition] |= currentPlayer;
            }

            else if (goldmap->map[currentPosition + goldmap->cols] == G_GOLD)
            {
                goldmap->map[currentPosition+goldmap->cols] |= currentPlayer;
                goldmap->map[currentPosition] &=~ currentPlayer;
            //goldmap->
                currentPosition= currentPosition+goldmap->cols;
                flag = 1;
            goldmap->map[currentPosition] &=~ G_GOLD;
                goldMine.drawMap();
                goldMine.postNotice ("Congratulations You Have found the real gold. Move to the edge of the map to exit!");
            }

            else if (goldmap->map[currentPosition + goldmap->cols] == G_FOOL)
            {
                goldmap->map[currentPosition+goldmap->cols] |= currentPlayer;
                goldmap->map[currentPosition] &=~ currentPlayer;
                currentPosition= currentPosition+goldmap->cols;
                goldMine.drawMap();
                goldMine.postNotice ("You found Fool's Gold");
            }

            else
            {
                goldmap->map[currentPosition+goldmap->cols] |= currentPlayer;
                goldmap->map[currentPosition] &=~ currentPlayer;
            //goldmap->
                currentPosition= currentPosition+goldmap->cols;

            }
        }
         else if (flag == 1)
        {
        goldmap->map[currentPosition] &=~ currentPlayer;
        goldMine.drawMap();
        goldMine.postNotice ("Congrats! You have won!!!");
        goldmap->players &=~ currentPlayer;
        break;

        }



        goldMine.drawMap();

    }
    else if (key == 'k')
    {
        //currentRow+1;
        if ((currentPosition-goldmap->cols)> 0)
        {

            if (goldmap->map[currentPosition-goldmap->cols] == G_WALL)
            {
                goldmap->map[currentPosition] |= currentPlayer;
            }

            else if (goldmap->map[currentPosition-goldmap->cols] == G_GOLD)
            {
                goldmap->map[currentPosition-goldmap->cols] |= currentPlayer;
                goldmap->map[currentPosition] &=~ currentPlayer;
                currentPosition= currentPosition-goldmap->cols;
                flag = 1;
            goldmap->map[currentPosition] &=~ G_GOLD;
                goldMine.drawMap();
                goldMine.postNotice ("Congratulations You Have found the real gold. Move to the edge of the map to exit!");
            }
            else if (goldmap->map[currentPosition-goldmap->cols] == G_FOOL)
            {
                goldmap->map[currentPosition-goldmap->cols] |= currentPlayer;
                goldmap->map[currentPosition] &=~ currentPlayer;
                currentPosition= currentPosition-goldmap->cols;
                goldMine.drawMap();
                goldMine.postNotice ("You have found Fool's Gold");
            }

            else
            {
                goldmap->map[currentPosition-goldmap->cols] |= currentPlayer;
                goldmap->map[currentPosition] &=~ currentPlayer;
                currentPosition= currentPosition-goldmap->cols;
            }
        }
 else if (flag == 1)
        {
        goldmap->map[currentPosition] &=~ currentPlayer;
        goldMine.drawMap();
        goldMine.postNotice ("Congrats! You have won!!!");
        goldmap->players &=~ currentPlayer;
        break;

        }



        goldMine.drawMap();
    }
    else if (key == 'l')
    {
        //currentcolumn+1;
        if (currentPosition%(goldmap->cols) != (goldmap->cols - 1))
        {
        if (goldmap->map[currentPosition+1] == G_WALL)
        {
            goldmap->map[currentPosition] |= currentPlayer;
        }
        else if (goldmap->map[currentPosition+1] == G_GOLD)
        {
            goldmap->map[currentPosition+1] |= currentPlayer;
            goldmap->map[currentPosition] &=~ currentPlayer;
            currentPosition= currentPosition+1;
            flag = 1;
            goldmap->map[currentPosition] &=~ G_GOLD;
            goldMine.drawMap();
            goldMine.postNotice ("Congratulations You Have found the real gold. Move to the edge of the map to exit!");
        }
        else if (goldmap->map[currentPosition+1] == G_FOOL)
        {
            goldmap->map[currentPosition+1] |= currentPlayer;
            goldmap->map[currentPosition] &=~ currentPlayer;
            currentPosition= currentPosition+1;
            goldMine.drawMap();
            goldMine.postNotice ("You have found Fool's Gold");
        }

        else
        {
            goldmap->map[currentPosition+1] |= currentPlayer;
            goldmap->map[currentPosition] &=~ currentPlayer;
            currentPosition= currentPosition+1;
        }
}
 else if (flag == 1)
        {
        goldmap->map[currentPosition] &=~ currentPlayer;
        goldMine.drawMap();
        goldMine.postNotice ("Congrats! You have won!!!");
        goldmap->players &=~ currentPlayer;
        break;

        }
        goldMine.drawMap();
   }


    sem_post(sem_pointer);
    key = goldMine.getKey();


  }
  goldmap->map[currentPosition] &=~ currentPlayer;
  goldmap->players &=~ currentPlayer;
}

if (goldmap->players == 0)
{
  shm_unlink("/toddsGoldMem");
  sem_close(sem_pointer);
  sem_unlink("/Semaphore_open");
}
  return 0;
}
