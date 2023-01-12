//  I WANT TO PLAY WITH YOU
//          YOUR FRIEND, AI
#include <stdio.h>

int X = 40;
int Y = 12;
//  1 = 1,5
//  2 = 4,5
//  3 = 7,5
//  4 = 10,5

void field(int j);
void racketL(int i, int j, int rack_posL);
void racketR(int i, int j, int rack_posR, int rack_posL);
void boll(int i, int j);
int fly(int rack_posL, int rack_posR, int dir);
void middle(int i, int j, int rack_pos);
int check_fail();

int main(void) {
  int rack_posL = 9;  // нижнняя точка ракетки
  int rack_posR = 13;
  char com;
  int scoreL = 0;
  int scoreR = 0;
  int dir = 1;

  printf("\n%d : %d\n\n", scoreL, scoreR);
  for (int j = 27; j > 0; j--) {
    for (int i = 0; i < 82; i++) {
      field(j);
      middle(i, j, rack_posL);
      racketL(i, j, rack_posL);
      racketR(i, j, rack_posR, rack_posL);
      boll(i, j);
    }
    printf("\n");
  }

  int ascZ = 122;
  int ascA = 97;
  int ascM = 109;
  int ascK = 107;
  int ascLINE = 10;
  while (scanf("%c", &com)) {
    if ((com == (char)ascZ || com == (char)90) && rack_posL > 3) {
      rack_posL -= 2;
    } else if ((com == (char)ascA || com == (char)65) && rack_posL < 23) {
      rack_posL += 2;
    } else if ((com == (char)ascM || com == (char)77) && rack_posR > 3) {
      rack_posR -= 2;
    } else if ((com == (char)ascK || com == (char)75) && rack_posR < 23) {
      rack_posR += 2;
    } else if (com == (char)113) {  //  q
      break;
    } else if (com != (char)ascZ && com != (char)ascA && com != (char)ascM &&
               com != (char)ascK && com != (char)ascLINE && com != (char)32) {
      printf("Incorrect enter\n");
    }

    if (com != (char)ascLINE) {
      printf("\33[0d\33[2J");
      printf("\n%40.1d : %-40.1d\n\n", scoreL, scoreR);

      for (int j = 27; j > 0; j--) {
        for (int i = 0; i < 82; i++) {
          field(j);
          middle(i, j, rack_posL);
          racketL(i, j, rack_posL);
          racketR(i, j, rack_posR, rack_posL);
          boll(i, j);
        }
        printf("\n");
      }
    }
    dir = fly(rack_posL, rack_posR, dir);
    int fail = check_fail(X);

    if (fail == 1) {  // пропустил левый
      X = 20;
      scoreR++;
    } else if (fail == 2) {  // пропустил правый
      X = 30;
      scoreL++;
    }

    if (scoreL == 21) {
      printf("\n%40.1d : %-40.1d\n\n", scoreL, scoreR);
      printf("\n\t\t\t\t    Left is win!\n");
      break;
    } else if (scoreR == 21) {
      printf("\n%40.1d : %-40.1d\n\n", scoreL, scoreR);
      printf("\n\t\t\t\t   Right is win!\n");
      break;
    }
  }
  return 0;
}

void field(int j) {
  if (j == 27 || j == 1) {
    int asc_broad = 36;
    printf("%c", (char)asc_broad);
  } else {
    int asc_space = 32;
    printf("%c", (char)asc_space);
  }
}

void racketL(int i, int j, int rack_posL) {
  if ((i == 10 && j == rack_posL) || (i == 10 && j == rack_posL + 1) ||
      (i == 10 && j == rack_posL + 2)) {
    int asc_stick = 124;
    printf("%c", (char)asc_stick);
  }
}

void racketR(int i, int j, int rack_posR, int rack_posL) {
  // если нет ракетки напротив
  int asc_stick = 124;

  if (rack_posR > rack_posL + 2 ||
      rack_posR < rack_posL - 2) {  // нет ракетки напротив
    if (Y != rack_posR && Y != rack_posR + 1 &&
        Y != rack_posR + 2) {  // мяча нет
      if ((i == 70 && j == rack_posR) || (i == 70 && j == rack_posR + 1) ||
          (i == 70 && j == rack_posR + 2)) {
        printf("%c", (char)asc_stick);
      }
    } else if (Y == rack_posR && Y != rack_posR + 1 &&
               Y != rack_posR + 2) {  // мяч напротив первой
      if ((i == 69 && j == rack_posR) || (i == 70 && j == rack_posR + 1) ||
          (i == 70 && j == rack_posR + 2)) {
        printf("%c", (char)asc_stick);
      }
    } else if (Y != rack_posR && Y == rack_posR + 1 &&
               Y != rack_posR + 2) {  // мяч напротив второй
      if ((i == 70 && j == rack_posR) || (i == 69 && j == rack_posR + 1) ||
          (i == 70 && j == rack_posR + 2)) {
        printf("%c", (char)asc_stick);
      }
    } else if (Y != rack_posR && Y != rack_posR + 1 &&
               Y == rack_posR + 2) {  // мяч напротив третьей
      if ((i == 70 && j == rack_posR) || (i == 70 && j == rack_posR + 1) ||
          (i == 69 && j == rack_posR + 2)) {
        printf("%c", (char)asc_stick);
      }
    }
  } else if (rack_posR == rack_posL + 2) {  // 1
    if (Y != rack_posR && Y != rack_posR + 1 &&
        Y != rack_posR + 2) {  // мяча нет
      if ((i == 69 && j == rack_posR) || (i == 70 && j == rack_posR + 1) ||
          (i == 70 && j == rack_posR + 2)) {
        printf("%c", (char)asc_stick);
      }
    } else if (Y == rack_posR && Y != rack_posR + 1 &&
               Y != rack_posR + 2) {  // мяч напротив первой
      if ((i == 68 && j == rack_posR) || (i == 70 && j == rack_posR + 1) ||
          (i == 70 && j == rack_posR + 2)) {
        printf("%c", (char)asc_stick);
      }
    } else if (Y != rack_posR && Y == rack_posR + 1 &&
               Y != rack_posR + 2) {  // мяч напротив второй
      if ((i == 69 && j == rack_posR) || (i == 69 && j == rack_posR + 1) ||
          (i == 70 && j == rack_posR + 2)) {
        printf("%c", (char)asc_stick);
      }
    } else if (Y != rack_posR && Y != rack_posR + 1 &&
               Y == rack_posR + 2) {  // мяч напротив третьей
      if ((i == 69 && j == rack_posR) || (i == 70 && j == rack_posR + 1) ||
          (i == 69 && j == rack_posR + 2)) {
        printf("%c", (char)asc_stick);
      }
    }
  } else if (rack_posR == rack_posL) {  // 2
    if (Y != rack_posR && Y != rack_posR + 1 &&
        Y != rack_posR + 2) {  // мяча нет
      if ((i == 69 && j == rack_posR) || (i == 69 && j == rack_posR + 1) ||
          (i == 69 && j == rack_posR + 2)) {
        printf("%c", (char)asc_stick);
      }
    } else if (Y == rack_posR && Y != rack_posR + 1 &&
               Y != rack_posR + 2) {  // мяч напротив первой
      if ((i == 68 && j == rack_posR) || (i == 69 && j == rack_posR + 1) ||
          (i == 69 && j == rack_posR + 2)) {
        printf("%c", (char)asc_stick);
      }
    } else if (Y != rack_posR && Y == rack_posR + 1 &&
               Y != rack_posR + 2) {  // мяч напротив второй
      if ((i == 69 && j == rack_posR) || (i == 68 && j == rack_posR + 1) ||
          (i == 69 && j == rack_posR + 2)) {
        printf("%c", (char)asc_stick);
      }
    } else if (Y != rack_posR && Y != rack_posR + 1 &&
               Y == rack_posR + 2) {  // мяч напротив третьей
      if ((i == 69 && j == rack_posR) || (i == 69 && j == rack_posR + 1) ||
          (i == 68 && j == rack_posR + 2)) {
        printf("%c", (char)asc_stick);
      }
    }
  } else if (rack_posR == rack_posL - 1) {  // 3
    if (Y != rack_posR && Y != rack_posR + 1 &&
        Y != rack_posR + 2) {  // мяча нет
      if ((i == 70 && j == rack_posR) || (i == 69 && j == rack_posR + 1) ||
          (i == 69 && j == rack_posR + 2)) {
        printf("%c", (char)asc_stick);
      }
    } else if (Y == rack_posR && Y != rack_posR + 1 &&
               Y != rack_posR + 2) {  // мяч напротив первой
      if ((i == 69 && j == rack_posR) || (i == 69 && j == rack_posR + 1) ||
          (i == 69 && j == rack_posR + 2)) {
        printf("%c", (char)asc_stick);
      }
    } else if (Y != rack_posR && Y == rack_posR + 1 &&
               Y != rack_posR + 2) {  // мяч напротив второй
      if ((i == 70 && j == rack_posR) || (i == 68 && j == rack_posR + 1) ||
          (i == 69 && j == rack_posR + 2)) {
        printf("%c", (char)asc_stick);
      }
    } else if (Y != rack_posR && Y != rack_posR + 1 &&
               Y == rack_posR + 2) {  // мяч напротив третьей
      if ((i == 70 && j == rack_posR) || (i == 69 && j == rack_posR + 1) ||
          (i == 68 && j == rack_posR + 2)) {
        printf("%c", (char)asc_stick);
      }
    }
  } else if (rack_posR == rack_posL - 2) {  // 4
    if (Y != rack_posR && Y != rack_posR + 1 &&
        Y != rack_posR + 2) {  // мяча нет
      if ((i == 70 && j == rack_posR) || (i == 70 && j == rack_posR + 1) ||
          (i == 69 && j == rack_posR + 2)) {
        printf("%c", (char)asc_stick);
      }
    } else if (Y == rack_posR && Y != rack_posR + 1 &&
               Y != rack_posR + 2) {  // мяч напротив первой
      if ((i == 69 && j == rack_posR) || (i == 70 && j == rack_posR + 1) ||
          (i == 69 && j == rack_posR + 2)) {
        printf("%c", (char)asc_stick);
      }
    } else if (Y != rack_posR && Y == rack_posR + 1 &&
               Y != rack_posR + 2) {  // мяч напротив второй
      if ((i == 70 && j == rack_posR) || (i == 69 && j == rack_posR + 1) ||
          (i == 69 && j == rack_posR + 2)) {
        printf("%c", (char)asc_stick);
      }
    } else if (Y != rack_posR && Y != rack_posR + 1 &&
               Y == rack_posR + 2) {  // мяч напротив третьей
      if ((i == 70 && j == rack_posR) || (i == 70 && j == rack_posR + 1) ||
          (i == 68 && j == rack_posR + 2)) {
        printf("%c", (char)asc_stick);
      }
    }
  } else if (rack_posR == rack_posL + 1) {  // 5
    if (Y != rack_posR && Y != rack_posR + 1 &&
        Y != rack_posR + 2) {  // мяча нет
      if ((i == 69 && j == rack_posR) || (i == 69 && j == rack_posR + 1) ||
          (i == 70 && j == rack_posR + 2)) {
        printf("%c", (char)asc_stick);
      }
    } else if (Y == rack_posR && Y != rack_posR + 1 &&
               Y != rack_posR + 2) {  // мяч напротив первой
      if ((i == 68 && j == rack_posR) || (i == 69 && j == rack_posR + 1) ||
          (i == 70 && j == rack_posR + 2)) {
        printf("%c", (char)asc_stick);
      }
    } else if (Y != rack_posR && Y == rack_posR + 1 &&
               Y != rack_posR + 2) {  // мяч напротив второй
      if ((i == 69 && j == rack_posR) || (i == 68 && j == rack_posR + 1) ||
          (i == 70 && j == rack_posR + 2)) {
        printf("%c", (char)asc_stick);
      }
    } else if (Y != rack_posR && Y != rack_posR + 1 &&
               Y == rack_posR + 2) {  // мяч напротив третьей
      if ((i == 69 && j == rack_posR) || (i == 69 && j == rack_posR + 1) ||
          (i == 69 && j == rack_posR + 2)) {
        printf("%c", (char)asc_stick);
      }
    }
  }
}

void boll(int i, int j) {
  if (i == X && j == Y) {
    int asc_astr = 42;
    printf("%c", (char)asc_astr);
  }
}

int fly(int rack_posL, int rack_posR, int dir) {
  if (Y == 26 && dir != 2 && dir != 3) {
    switch (dir) {
    case 1:
      dir = 2;
      X++;
      break;
    case 4:
      dir = 3;
      X--;
      break;
    }
    return dir;
  }
  if (Y == 2 && dir != 1 && dir != 4) {
    switch (dir) {
    case 2:
      dir = 1;
      X++;
      break;
    case 3:
      dir = 4;
      X--;
      break;
    }
    return dir;
  }
  if (X == 10 && Y >= rack_posL && Y <= rack_posL + 2 && dir != 2 && dir != 1) {
    // удар об левую
    switch (dir) {
    case 3:
      dir = 2;
      break;
    case 4:
      dir = 1;
      break;
    }
    return dir;
  }
  if (X == 70 && Y >= rack_posR && Y <= rack_posR + 2 && dir != 4 && dir != 3) {
    // удар об правую
    switch (dir) {
    case 1:
      dir = 4;
      break;
    case 2:
      dir = 3;
      break;
    }
    return dir;
  }
  switch (dir) {
  case 1:
    X++;
    Y++;
    break;
  case 2:
    X++;
    Y--;
    break;
  case 3:
    X--;
    Y--;
    break;
  case 4:
    X--;
    Y++;
    break;
  }
  return dir;
}

void middle(int i, int j, int rack_pos) {
  int asc_stick = 124;
  if (j != rack_pos && j != rack_pos + 1 && j != rack_pos + 2) {  // нет ракетки
    if (i == 40 && j != Y) {
      printf("%c", (char)asc_stick);  // если нет ракетки и нет мяча
    } else if (i == 39 && j == Y &&
               X < 40) {  // если нет ракетки и есть мяч cлева
      printf("%c", (char)asc_stick);
    } else if (i == 40 && j == Y &&
               X > 40) {  // если нет ракетки и есть мяч справа
      printf("%c", (char)asc_stick);
    }
  } else {  //  если ракетка
    if (i == 39 && j != Y) {
      printf("%c", (char)asc_stick);  // если нет мяча и ракетки
    } else if (i == 38 && j == Y && X < 40) {
      printf("%c", (char)asc_stick);  // если есть мяч cлева и не ракетик
    } else if (i == 39 && j == Y && X > 40) {
      printf("%c", (char)asc_stick);  // если есть мяч справа и не ракетик
    }
  }
}

int check_fail() {
  if (X == 0) {
    return 1;
  }
  if (X == 81) {
    return 2;
  }
  return 0;
}
