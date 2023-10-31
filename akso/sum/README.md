Assembly implementation of function
void sum(int64_t *x, size_t n)

Function performs a sum operation defined by the following pseudocode:
y = 0;
for (i = 0; i < n; ++i)
  y += x[i] * (2 ** floor(64 * i * i / n));
x[0, ..., n-1] = y;

Program doesn't allocate any additional memory.

(I promisse this was hella hard).