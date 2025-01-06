#ifndef CAT
#define CAT

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  int E;
  int T;
} Flags;

void ReadFlags(int argc, char *argv[], Flags *flags);
// void NoArgs(int fd);
void Args(char *argv[], Flags flags, int argc);

#endif
