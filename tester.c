#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>
#include "testee.h"

// redefine assert to set a boolean flag
#ifdef assert
#undef assert
#endif
#define assert(x) (rslt = rslt && (x))

// the function to test
int my_div(int x, int y);

// main result return code used by redefined assert
static int rslt;

// variables controling stub functions
static int expected_code;
static int should_exit;
static jmp_buf jump_env;

// test suite main variables
static int done;
static int num_tests;
static int tests_passed;

//  utility function
void TestStart(char *name)
{
    num_tests++;
    rslt = 1;
    printf("-- Testing %s ... ",name);
}

//  utility function
void TestEnd()
{
    if (rslt) tests_passed++;
    printf("%s\n", rslt ? "success" : "fail");
}

// stub function
void exit(int code)
{
    if (!done)
    {
        assert(should_exit==1);
        assert(expected_code==code);
        longjmp(jump_env, 1);
    }
    else
    {
        _exit(code);
    }
}

// test case
void test_normal()
{
    int jmp_rval;
    int r;

    TestStart("test_normal");
    should_exit = 0;
    if (!(jmp_rval=setjmp(jump_env)))
    {
        r = my_div(12,3);
    }

    assert(jmp_rval==0);
    assert(r==4);
    TestEnd();
}

// test case
void test_div0()
{
    int jmp_rval;
    int r;

    TestStart("test_div0");
    should_exit = 1;
    expected_code = 2;
    if (!(jmp_rval=setjmp(jump_env)))
    {
        r = my_div(2,0);
    }

    assert(jmp_rval==1);
    TestEnd();
}

int main()
{
    num_tests = 0;
    tests_passed = 0;
    done = 0;
    test_normal();
    test_div0();
    printf("Total tests passed: %d\n", tests_passed);
    done = 1;
    return !(tests_passed == num_tests);
}
