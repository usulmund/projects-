#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define N 25
#define M 80
void output_life(int **matrix, int n, int m);
void creat_local_area(int **matrix, int (*local_area)[3], int mid_I, int mid_J);
int check_local_area(int (*local_area)[3]);
int check_Up(int **matrix, int n, int mid_J);
int check_Down(int **matrix, int n, int mid_J);
int check_Right(int **matrix, int m, int mid_I);
int check_Left(int **matrix, int m, int mid_I);
int check_UL(int **matrix, int n, int m);
int check_UR(int **matrix, int n, int m);
int check_DL(int **matrix, int n, int m);
int check_DR(int **matrix, int n, int m);
void insert(int **matrix, int **tmp, int n, int m);
int compare(int **matrix, int **past_matrix, int n, int m);
int countLife(int **matrix, int n, int m);
int setfield(int **matrix, int n, int m);

int main(void) {
  int **matrix = malloc(N * sizeof(int *));
  for (int i = 0; i < N; i++) {
    matrix[i] = malloc(M * sizeof(int));
  }
  int **tmp = malloc(N * sizeof(int *));
  for (int i = 0; i < N; i++) {
    tmp[i] = malloc(M * sizeof(int));
  }
  int **past_matrix = malloc(N * sizeof(int *));
  for (int i = 0; i < N; i++) {
    past_matrix[i] = malloc(M * sizeof(int));
  }
  int correct = setfield(matrix, N, M);
  if (correct == 1) {
    int speed = 200000;
    int local_area[3][3];
    output_life(matrix, N, M);
    printf("Day: 0\nPopulation: %d\n", countLife(matrix, N, M));
    fcntl(STDOUT_FILENO, F_SETFL,
          fcntl(STDOUT_FILENO, F_GETFL, 0) | O_NONBLOCK);
    int k = 0;
    int is_ok = 1;
    while (is_ok == 1) {
      char ch = getc(stdin);
      if (ch == 'w' && speed >= 100000) {
        speed -= 50000;
      }
      if (ch == 's' && speed <= 250000) {
        speed += 50000;
      }
      printf("\33[0d\33[2J");
      usleep(speed);
      for (int i = 1; i < N - 1; i++) {
        for (int j = 1; j < M - 1; j++) {
          creat_local_area(matrix, local_area, i, j);
          int main_el = check_local_area(local_area);
          tmp[i][j] = main_el;
        }
      }
      for (int j = 1; j < M - 1; j++) {
        int main_el = check_Up(matrix, N, j);
        tmp[0][j] = main_el;
      }
      for (int j = 1; j < M - 1; j++) {
        int main_el = check_Down(matrix, N, j);
        tmp[N - 1][j] = main_el;
      }
      for (int i = 1; i < N - 1; i++) {
        int main_el = check_Left(matrix, M, i);
        tmp[i][0] = main_el;
      }
      for (int i = 1; i < N - 1; i++) {
        int main_el = check_Right(matrix, M, i);
        tmp[i][M - 1] = main_el;
      }
      tmp[0][0] = check_UL(matrix, N, M);
      tmp[0][M - 1] = check_UR(matrix, N, M);
      tmp[N - 1][0] = check_DL(matrix, N, M);
      tmp[N - 1][M - 1] = check_DR(matrix, N, M);
      insert(past_matrix, matrix, N, M);
      insert(matrix, tmp, N, M);
      int same = compare(matrix, past_matrix, N, M);
      output_life(matrix, N, M);
      k++;
      printf("Day: %d\nPopulation: %d\n", k, countLife(matrix, N, M));
      if (same == 1 || ch == 'q') {
        is_ok = 0;
      }
    }
  } else {
    printf("Try again ;)\n");
  }
  for (int i = 0; i < N; i++) {
    free(matrix[i]);
    free(tmp[i]);
    free(past_matrix[i]);
  }
  free(matrix);
  free(tmp);
  free(past_matrix);
  return 0;
}

int compare(int **matrix, int **past_matrix, int n, int m) {
  int check = 0;
  int same = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (matrix[i][j] == past_matrix[i][j]) {
        check++;
      }
    }
  }
  if (check == n * m) {
    same = 1;
  }
  return same;
}
int check_UL(int **matrix, int n, int m) {
  int main_el;
  int sum = matrix[0][1] + matrix[1][0] + matrix[1][1];
  sum += matrix[n - 1][0] + matrix[n - 1][1];
  sum += matrix[0][m - 1] + matrix[1][m - 1];
  sum += matrix[n - 1][m - 1];
  if (matrix[0][0] == 0) {
    if (sum == 3) {
      main_el = 1;
    } else {
      main_el = 0;
    }
  } else {
    if (sum == 2 || sum == 3) {
      main_el = 1;
    } else {
      main_el = 0;
    }
  }
  return main_el;
}
int check_UR(int **matrix, int n, int m) {
  int main_el;
  int sum = matrix[0][m - 2] + matrix[1][m - 1] + matrix[1][m - 2];
  sum += matrix[n - 1][m - 2] + matrix[n - 1][m - 1];
  sum += matrix[0][0] + matrix[1][0];
  sum += matrix[n - 1][0];
  if (matrix[0][m - 1] == 0) {
    if (sum == 3) {
      main_el = 1;
    } else {
      main_el = 0;
    }
  } else {
    if (sum == 2 || sum == 3) {
      main_el = 1;
    } else {
      main_el = 0;
    }
  }
  return main_el;
}
int check_DL(int **matrix, int n, int m) {
  int main_el;
  int sum = matrix[n - 1][1] + matrix[n - 2][1] + matrix[n - 1][0];
  sum += matrix[n - 1][m - 1] + matrix[n - 2][m - 1];
  sum += matrix[0][0] + matrix[0][1];
  sum += matrix[0][m - 1];
  if (matrix[n - 1][0] == 0) {
    if (sum == 3) {
      main_el = 1;
    } else {
      main_el = 0;
    }
  } else {
    if (sum == 2 || sum == 3) {
      main_el = 1;
    } else {
      main_el = 0;
    }
  }
  return main_el;
}
int check_DR(int **matrix, int n, int m) {
  int main_el;
  int sum = matrix[n - 1][m - 2] + matrix[n - 2][m - 1] + matrix[n - 2][m - 2];
  sum += matrix[n - 1][0] + matrix[n - 2][0];
  sum += matrix[0][m - 1] + matrix[0][m - 2];
  sum += matrix[0][0];
  if (matrix[n - 1][m - 1] == 0) {
    if (sum == 3) {
      main_el = 1;
    } else {
      main_el = 0;
    }
  } else {
    if (sum == 2 || sum == 3) {
      main_el = 1;
    } else {
      main_el = 0;
    }
  }
  return main_el;
}
int check_Left(int **matrix, int m, int mid_I) {
  int main_el;
  int sum = matrix[mid_I - 1][0] + matrix[mid_I + 1][0];
  sum += matrix[mid_I - 1][m - 1] + matrix[mid_I][m - 1] +
         matrix[mid_I + 1][m - 1];
  sum += matrix[mid_I - 1][m - 2] + matrix[mid_I][m - 2] +
         matrix[mid_I + 1][m - 2];
  if (matrix[mid_I][0] == 0) {
    if (sum == 3) {
      main_el = 1;
    } else {
      main_el = 0;
    }
  } else {
    if (sum == 2 || sum == 3) {
      main_el = 1;
    } else {
      main_el = 0;
    }
  }
  return main_el;
}
int check_Right(int **matrix, int m, int mid_I) {
  int main_el;
  int sum = matrix[mid_I - 1][m - 1] + matrix[mid_I + 1][m - 1];
  sum += matrix[mid_I - 1][0] + matrix[mid_I][0] + matrix[mid_I + 1][0];
  sum += matrix[mid_I - 1][m - 2] + matrix[mid_I][m - 2] +
         matrix[mid_I + 1][m - 2];
  if (matrix[mid_I][m - 1] == 0) {
    if (sum == 3) {
      main_el = 1;
    } else {
      main_el = 0;
    }
  } else {
    if (sum == 2 || sum == 3) {
      main_el = 1;
    } else {
      main_el = 0;
    }
  }
  return main_el;
}
int check_Down(int **matrix, int n, int mid_J) {
  int main_el;
  int sum = matrix[n - 1][mid_J - 1] + matrix[n - 1][mid_J + 1];
  sum += matrix[0][mid_J - 1] + matrix[0][mid_J] + matrix[0][mid_J + 1];
  sum += matrix[n - 2][mid_J - 1] + matrix[n - 2][mid_J] +
         matrix[n - 2][mid_J + 1];
  if (matrix[n - 1][mid_J] == 0) {
    if (sum == 3) {
      main_el = 1;
    } else {
      main_el = 0;
    }
  } else {
    if (sum == 2 || sum == 3) {
      main_el = 1;
    } else {
      main_el = 0;
    }
  }
  return main_el;
}
int check_Up(int **matrix, int n, int mid_J) {
  int main_el;
  int sum = matrix[0][mid_J - 1] + matrix[0][mid_J + 1];
  sum += matrix[1][mid_J - 1] + matrix[1][mid_J] + matrix[1][mid_J + 1];
  sum += matrix[n - 1][mid_J - 1] + matrix[n - 1][mid_J] +
         matrix[n - 1][mid_J + 1];
  if (matrix[0][mid_J] == 0) {
    if (sum == 3) {
      main_el = 1;
    } else {
      main_el = 0;
    }
  } else {
    if (sum == 2 || sum == 3) {
      main_el = 1;
    } else {
      main_el = 0;
    }
  }
  return main_el;
}
void output_life(int **matrix, int n, int m) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (matrix[i][j] == 1) {
        printf("#");
      } else {
        printf(" ");
      }
    }
    printf("|\n");
  }
  for (int i = 0; i < m; i++) {
    printf("-");
  }
  printf("\n\n");
}
int setfield(int **matrix, int n, int m) {
  FILE *fin;
  int flag = 0;
  int mode = 0;
  printf("Choose field: 1 - classic; 2 - gun; 3 - sssr; 4 - virus; 5 - "
         "starwars; 6 - laser butterfly; 7 - custom.\n");
  scanf("%d", &mode);
  if (mode == 1) {
    fin = fopen("data/classic.txt", "r");
    flag = 1;
  } else if (mode == 2) {
    fin = fopen("data/gun.txt", "r");
    flag = 1;
  } else if (mode == 3) {
    fin = fopen("data/sssr.txt", "r");
    flag = 1;
  } else if (mode == 4) {
    fin = fopen("data/virus.txt", "r");
    flag = 1;
  } else if (mode == 5) {
    fin = fopen("data/starwars.txt", "r");
    flag = 1;
  } else if (mode == 6) {
    fin = fopen("data/butterfly.txt", "r");
    flag = 1;
  } else if (mode == 7) {
    char str[100];
    scanf("%99s", str);
    fin = fopen(str, "r");
    if (fin != NULL) {
      flag = 1;
    }
  } else {
    flag = 0;
  }
  if (flag == 1) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        fscanf(fin, "%d", &matrix[i][j]);
      }
    }
  }
  fclose(fin);
  return flag;
}
void creat_local_area(int **matrix, int (*local_area)[3], int mid_I,
                      int mid_J) {
  for (int i = mid_I - 1; i < mid_I + 2; i++) {
    for (int j = mid_J - 1; j < mid_J + 2; j++) {
      local_area[i - mid_I + 1][j - mid_J + 1] = matrix[i][j];
    }
  }
}
int check_local_area(int (*local_area)[3]) {
  int main_el;
  int sum = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      sum += local_area[i][j];
    }
  }
  sum -= local_area[1][1];
  if (local_area[1][1] == 1) {
    if (sum == 2 || sum == 3) {
      main_el = 1;
    } else {
      main_el = 0;
    }
  } else {
    if (sum == 3) {
      main_el = 1;
    } else {
      main_el = 0;
    }
  }
  return main_el;
}
void insert(int **matrix, int **tmp, int n, int m) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      matrix[i][j] = tmp[i][j];
    }
  }
}
int countLife(int **matrix, int n, int m) {
  int sum = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (matrix[i][j] == 1)
        sum += 1;
    }
  }
  return sum;
}
