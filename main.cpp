#include "tests/tests.h"

int main(int, char**) {
  test_omitargs();
  test_pickargs();
  test_lambda_enabler();
  test_observablify();
}
