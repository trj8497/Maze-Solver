/*!
 * File name: mopsolver.c
 *
 * Author's name: Tejaswini Jagtap
 * Date: 11/3/2018
 *
 * A program to check whether or not a maze is solvable, assuming fixed 
 * entrance and exit coordinates and calculate the fewest number of steps 
 * necessary to solve a maze, if a solution exists.
 */

#define _GNU_SOURCE

#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>
#include<unistd.h>

#include "QueueADT.h" //Queue

int row;
int col;
int max_capacity = 8;
int row_capacity;
int size = 0;
int size_0 = 0;
int c2;


/*!
 *Structure for co ordinates of the elements in the maze_array that is a 2D
 *array
 *x: x co ordinate 
 *y: y co ordinate 
 */
typedef struct Co_ord1
{
  int x;
  int y;
}Co_ord;

/*!
 * Structure for the each element that will hold its parent, co ordinates, 
 * neighbors, number of neighbors and whether or not the element is visited
 */
typedef struct Element
{
  Co_ord co;
  Co_ord last;
  Co_ord neigh[4];
  int size_neigh;
  int visited;
}element;

/*!
 *This function reads the file character by character and stores it in the 
 *maze_array
 *
 *\param fp: pointer of the file from which the data is to be read
 *\param maze_array: 2D array that stores the read data from the file 
 *\return: maze_array 
 */
char** readFile(FILE *fp, char **maze_array)
{ 
  int p = 0;
  int fog;
  row_capacity = max_capacity;
  int col_capacity = max_capacity;
  maze_array = calloc(row_capacity, sizeof(char *));
  for(int i = 0; i < max_capacity; i++) {
      maze_array[i] = calloc(col_capacity, sizeof(char));
  }
  char c;
  while((c = fgetc(fp)) != EOF)
  {
    if (p == 1)
    {
      fog = col;
    }
    if (c == '\n')
    {
      row++;
      if (row >= row_capacity)
      {
        row_capacity = row_capacity * 5;
        maze_array = realloc(maze_array, row_capacity * sizeof(char*));
        for( int i = row; i < row_capacity; i++) 
        {
            maze_array[i] = calloc(col_capacity, sizeof(char));
        } 
      }
      col = 0;
    }
    if (c == '1' || c == '0')
    {
      col++;
      p = 1;
      if (col >= col_capacity)
      {
        col_capacity = col_capacity * 5;
        for(int i = 0; i < row_capacity; i++) 
        {
          maze_array[i] = realloc(maze_array[i], col_capacity * sizeof(char));
        }
      }
      maze_array[row][col-1] = (char)c;
      size++;
    }
  }
  
  col = fog;
  return maze_array;
}

/*!
 * This function prints the maze_array
 *
 * \param maze_array: 2D array that stores the data that is read from the file
 */
void pretty_print(char **maze_array, FILE *out)
{
  fprintf(out, "|");
  for (int i = 0; i < (2*col+1); i++)
  {
    fprintf(out, "-");
  }
  fprintf(out, "|\n");
  for (int i = 0; i < row; i++)
  {
    if (i != 0)
    {
      fprintf(out, "| ");
    }
    else 
    {
      fprintf(out, "  ");
    }
    
    for (int j = 0; j < col; j++)
    {
        if (maze_array[i][j] == '0')
        {
          fprintf(out, ". ");
        }
        else 
        {
          fprintf(out, "# ");
        }
    }
    if (i != row-1)
    {
      fprintf(out, "|");
    }
    else
    {
      fprintf(out, " ");
    }
    fprintf(out, "\n");
  }
  fprintf(out, "|");
  for (int i = 0; i < (2*col+1); i++)
  {
    fprintf(out, "-");
  }
  fprintf(out, "|\n");
}

/*!
 * This function frees the maze_array's allocated space
 * 
 * \param maze_array: 2D array that stores the data read from the file
 */
void freeArray(char **maze_array)
{
  for (int i = 0; i < row_capacity; i++)
  {
    if (maze_array[i] != NULL)
    {
      free(maze_array[i]);
    }
  }
  free(maze_array);
}

/*!
 * This function prints the 2D array that has the shortest path shown 
 *
 * \param array: a 2D array that has shortest path shown
 */
void pretty_print_sol(char array[row][col], FILE *out)
{
  fprintf(out, "|");
  for (int i = 0; i < (2*col+1); i++)
  {
    fprintf(out, "-");
  }
  fprintf(out, "|\n");
  for (int i = 0; i < row; i++)
  {
    if (i != 0)
    {
      fprintf(out, "| ");
    }
    else 
    {
      fprintf(out, "  ");
    }
    for (int j = 0; j < col; j++)
    {
        if (array[i][j] == '0')
        {
          fprintf(out, ". ");
        }
        else if (array[i][j] == '1')
        {
          fprintf(out, "# ");
        }
        else
        {
          fprintf(out, "+ ");
        }
    }
    if (i != row-1)
    {
      fprintf(out, "|");
    }
    else
    {
      fprintf(out, " ");
    }
    fprintf(out, "\n");
  }
  fprintf(out, "|");
  for (int i = 0; i < (2*col+1); i++)
  {
    fprintf(out, "-");
  }
  fprintf(out, "|\n");
}

/*!
 * This element find the neighbors of the element that has value '0' in them.
 *
 * \param neigh[4]: a array that will store the neighbors of the element(max 
 * number of neighbors will be 4)
 * \param maze_array: a 2D array that stores the data read from the file 
 * \param co: co ordinate of the element of whiich nighbors are to be found 
 */
int find_neigh(Co_ord neigh[4], char **maze_array, Co_ord co)
{
  Co_ord o;  
  int c = 0;
  int i = co.x;
  int j = co.y;
  if (i-1 > -1 && (maze_array[i-1][j] == '0'))
  {
    o.x = i-1;
    o.y = j;
    neigh[c] = o;
    c++;
  }
  if (j-1 > -1 && (maze_array[i][j-1] == '0'))
  {
    o.x = i;
    o.y = j-1;
    neigh[c] = o;
    c++;
  }
  if (i+1 < row && (maze_array[i+1][j] == '0'))
  {
    o.x = i+1;
    o.y = j;
    neigh[c] = o;
    c++;
  }
  if (j+1 < col && (maze_array[i][j+1] == '0'))
  {
    o.x = i;
    o.y = j+1;
    neigh[c] = o;
    c++;
  }
  return c;
} 

/*!
 * This function tracks the shortest path from the last element and updates the
 * array with a '+' character 
 *
 * \parsm last: the last element from which the backtracking starts 
 * \param array: the array that keeps track of shortest track
 * \param list_0: array that stores all the element that contain '0' as their 
 * vlaue that are visited
 */
int back_tracking(element last, char array[row][col],element list_0[size_0])
{
  int sol = 0;
  int i, j, a, b;
  for (int q= 0; q < size_0; q++)
  {
    if (list_0[q].co.x == last.co.x && list_0[q].co.y == last.co.y)
    {
      a = list_0[q].co.x;
      b = list_0[q].co.y;
      i = list_0[q].last.x;
      j = list_0[q].last.y;
    }
  }
  while (i != -1 && j != -1)
  {
    array[a][b] = '+';
    sol++;
    a = i;
    b = j;
    for (int q= 0; q <size_0; q++)
    {
      if (list_0[q].co.x == a && list_0[q].co.y == b)
      {
        i = list_0[q].last.x;
        j = list_0[q].last.y;
      }
    }
  }
  array[0][0] = '+';
  sol++;
  return sol;
}

/*!
 * This function checks whether the solution to the maze exists or not and 
 * creates a rray that stores all the elements that contains '0' as their 
 * value that are visited
 *
 * \param maze_array: 2D array that stores the data read forom the file
 * \param array: 2D array that keeps track of shortest path 
 */
int breadth_first_search(char ** maze_array, char array[row][col])
{
  int c = 0;
  int size_neigh = 0;
  for (int i = 0; i < row; i++)
  {
    for (int j = 0; j < col; j++)
    {
      if (maze_array[i][j] == '0')
      {
        size_0++;
      }
    }
  }
  element list_0[size_0];
  for (int i = 0; i < row; i++)
  {
    for (int j = 0; j < col; j++)
    {
      if (maze_array[i][j] != '1')
      {
        list_0[c].co.x = i;
        list_0[c].co.y = j;
        size_neigh = find_neigh(list_0[c].neigh, maze_array, list_0[c].co);
        list_0[c].size_neigh = size_neigh;
        list_0[c].last.x = -1;
        list_0[c].last.y = -1;
        list_0[c].visited = 0;
        c++;
      }
    }
  }
  Queue aQueue = NULL;
  aQueue = createQueue(10);
  list_0[0].visited = 1;
  enQueue(aQueue, (void*)&list_0[0]);
  while (!isEmpty(aQueue))
  {
    element el = *(element*)deQueue(aQueue);
    for (int i = 0; i < el.size_neigh; i++)
    {
      int co_x = el.neigh[i].x;
      int co_y = el.neigh[i].y;
      for (int j = 0; j < size_0; j++)
      {
        if (list_0[j].co.x == co_x && list_0[j].co.y == co_y)
        {
          if (list_0[j].visited == 0)
          {
            list_0[j].visited = 1;
            list_0[j].last.x = el.co.x;
            list_0[j].last.y = el.co.y;
            enQueue(aQueue, (void*)&list_0[j]);
            break;
          }
        }
      }
    }
  }
  int s = 0;
  
  if ((list_0[size_0 - 1].visited == 1) && maze_array[0][0] == '0')
  {
    s = back_tracking(list_0[size_0-1], array, list_0);
  }
  freeQueue(aQueue);
   return s;
}

/*!
 *This function displays both the array that is maze_array and array and also 
 *prints out the number of solution steps 
 *
 * \param maze_array: 2D array that stores the data read form the file
 * \param array: 2D array that keeps the track of the shortetst path
 * \out: file in which all the details are to be printed
 */
void display(char ** maze_array, char array[row][col], FILE *out)
{
  fprintf(out, "|");
  for (int i = 0; i < (2*col+1); i++)
  {
    fprintf(out, "-");
  }
  fprintf(out, "|\n");
  for (int i = 0; i < row; i++)
  {
    if (i != 0)
    {
      fprintf(out, "| ");
    }
    else 
    {
      fprintf(out, "  ");
    }
    for (int j = 0; j < col; j++)
    {
        if (maze_array[i][j] == '0')
        {
          fprintf(out, ". ");
        }
        else 
        {
          fprintf(out, "# ");
        }
    }
    if (i != row-1)
    {
      fprintf(out, "|");
    }
    else
    {
      fprintf(out, " ");
    }
    fprintf(out, "\n");
  }
  fprintf(out, "|");
  for (int i = 0; i < (2*col+1); i++)
  {
    fprintf(out, "-");
  }
  fprintf(out, "|\n");
  c2 = breadth_first_search(maze_array, array);
  if (c2 == 0)
  {
    fprintf(out, "No solution.\n");
  }
  else 
  {
    fprintf(out, "Solution in %d steps.\n",c2);
  }
  fprintf(out, "|");
  for (int i = 0; i < (2*col+1); i++)
  {
    fprintf(out, "-");
  }
  fprintf(out, "|\n");
  for (int i = 0; i < row; i++)
  {
    if (i != 0)
    {
      fprintf(out, "| ");
    }
    else 
    {
      fprintf(out, "  ");
    }
    for (int j = 0; j < col; j++)
    {
        if (array[i][j] == '0')
        {
          fprintf(out, ". ");
        }
        else if (array[i][j] == '1')
        {
          fprintf(out, "# ");
        }
        else
        {
          fprintf(out, "+ ");
        }
    }
    if (i != row-1)
    {
      fprintf(out, "|");
    }
    else
    {
      fprintf(out, " ");
    }
    fprintf(out, "\n");
  }
  fprintf(out, "|");
  for (int i = 0; i < (2*col+1); i++)
  {
    fprintf(out, "-");
  }
  fprintf(out, "|");
  
}

/*!
 *main function reads command line argument and calls the function as per the 
 *flags that are read
 *
 * \param argc: number of command line arguments
 * \param argv[]: arguments
 */ 
int main (int argc, char* argv[])
{ 
  int h = 0;
  int d = 0;
  int s = 0;
  int p = 0;
  int i = 0;
  int o = 0;
  FILE *fp;
  char **maze_array = NULL; 
  FILE *out = stdout;
  int opt;
  while((opt = getopt(argc, argv, "hdspi:o:")) != -1)
  {
    switch(opt)
    {
      case 'h':
        h = 1;
        break;
      case 'd':
        d = 1;
        break;
      case 's':
        s = 1;
        break;
      case 'p':
        p = 1;
        break;
      case 'i':
        ;
        fp = fopen(optarg, "r");
        maze_array = readFile(fp, maze_array);      
        i = 1;
        break;
      case 'o':
        out = fopen(optarg, "w");
        o = 1;
        break;
      default:
        printf("Unorganized option '%c'.\n", opt);
    }
  }
  
  if (h == 1)
  {
    printf("USAGE:\n");
    printf("mopsolver [-hdsp] [-i INFILE] [-o OUTFILE]\n\n");
    printf("Options:\n");
    printf("\t-h\tPrint this helpful message to stdout and exit.\n");
    printf("\t-d\tPretty print (display) the maze after reading.\n");
    printf("\t-s\tPrint shortest solution steps.\n");
    printf("\t-p\tPrint an optimal path.\n");
    printf("\t-i INFILE\tRead maze from INFILE.\n");
    printf("\t-o OUTFILE\tWrite all output to OUTFILE.\n");
    exit(0);
  }
  else
  {
    if (i != 1)
    {
       maze_array = readFile(stdin, maze_array); 
    }
    char array[row][col];
    for (int i = 0; i < row; i++)
    {
      for(int j = 0; j < col; j++)
      {
        array[i][j] = maze_array[i][j];
      }
    }
    if (d == 1 && o != 1)
    {  
      pretty_print(maze_array, out);
    }
    if (s == 1 && o != 1 && p != 1)
    {
      int c = breadth_first_search(maze_array, array);
      if (c == 0)
      {
        fprintf(out, "No solution.\n");
      }
      else
      {
        fprintf(out, "Solution in %d steps.\n", c);
      }
    }
    else if (s == 1 && o == 1 && p != 1)
    {
      display(maze_array, array, out);
    }
    else if (s == 1 && o ==1 && p ==1)
    {
      display(maze_array, array, out);
    }
    else if (s != 1 && o == 1 && p == 1)
    {
      display(maze_array, array, out);
    }
    else if (s != 1 && o != 1 && p == 1)
    {
      breadth_first_search(maze_array, array);
      pretty_print_sol(array, out);
    }
    else if (s != 1 && o == 1 && p != 1)
    {
      display(maze_array, array, out);
    }
    else if (s == 1 && o != 1 && p == 1)
    {
      int c = breadth_first_search(maze_array, array);
      if (c == 0 )
      {
        fprintf(out, "No solution.\n");
      }
      else
      {
        fprintf(out, "Solution in %d steps.\n", c);
      }
      pretty_print_sol(array, out);
    }
  }
  freeArray(maze_array);
  fclose(fp);
  fclose(out);
  return 0;
}
