/*
 * Stacks and Queues
 */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

// 3.1 Use a single array to implement three stacks
// Solution -- one stack starts on left end and grows right,
// one stack starts on right end and grows left, one stack
// starts in the middle and grows in arbitrary direction.
// You maintain a set of state for each (stack pointer, size)
class three_in_one
{
  static enum class arr { L = 0, R = 1, M = 2, ARR_MAX = 3 };

public:
  three_in_one(void)
  {
    memset(arr, 0, sizeof(arr));
    sp[L] = st[L] = 0;            // will grow right
    sp[M] = st[M] = arr_size / 3; // will grow right
    sp[R] = st[R] = arr_size-1;   // will grow left

    // Current sizes (0)
    sz[L] = 0;
    sz[M] = 0;
    sz[R] = 0;
  }

  void push(arr which, int what)
  {
    bool still_space = true;
    switch(which) {
    case arr::L:
      if (sp[which] == st[arr::M]) {
	// collision
	if (get_num_free_slots() > 0) {
	  memmove(arr+st[arr::M]+1, arr+st[arr::M], sz[arr::M]);
	  sp[arr::M]++;
	}
      }
      break;

    case arr::M:
    case arr::R:
      if (sp[arr::M] > sp[arr::R]) {
	// collision
	if (get_num_free_slots() > 0) {
	  memmove(arr+st[M]-1, arr+st[M], sz[M]);
	  sp[arr::M]--;
	}
      }
      break;
    }

    arr[sp[which]] = what;
    sz[which]++;
    sp[which] += dir(which);
  }

  int pop(arr which)
  {
    if (is_empty(which)) {
      cerr << "Empty" << endl;
      return -1;
    }

    auto ret = arr[sp[which]];
    sz[which]--;
    sp[which] -= dir(which);
  }

  int peek(arr which)
  {
    if (is_empty(which)) return -1;
    return arr[sp[which]];
  }
  bool is_empty(arr which) { return (sz[which] == 0); }

private:
  static const unsigned ARR_SIZE = 1024;

  unsigned get_num_free_slots(void)
  {
    return ARR_SIZE - get_num_used_slots();
  }

  unsigned get_num_used_slots(void)
  {
    return sz[arr:L] + sz[arr:R] + sz[arr:M];
  }

  int dir(arr which)
  {
    switch(which) {
    case arr::L: return 1;
    case arr::M: return 1;
    case arr::R: return -1;
    }
    return 0;
  }

  int arr[ARR_SIZE];
  unsigned st[arr::ARR_MAX];
  unsigned sp[arr::ARR_MAX];
  size_t sz[arr::ARR_MAX];
};

int main(int argc, char **argv)
{
  return 0;
}
