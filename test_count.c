/*
 *  Simple example of a CUnit unit test.
 *
 *  This program (crudely) demonstrates a very simple "black box"
 *  test of the standard library functions fprintf() and fread().
 *  It uses suite initialization and cleanup functions to open
 *  and close a common temporary file used by the test functions.
 *  The test functions then write to and read from the temporary
 *  file in the course of testing the library functions.
 *
 *  The 2 test functions are added to a single CUnit suite, and
 *  then run using the CUnit Basic interface.  The output of the
 *  program (on CUnit version 2.0-2) is:
 *
 *           CUnit : A Unit testing framework for C.
 *           http://cunit.sourceforge.net/
 *
 *       Suite: Suite_1
 *         Test: test of fprintf() ... passed
 *         Test: test of fread() ... passed
 *
 *       --Run Summary: Type      Total     Ran  Passed  Failed
 *                      suites        1       1     n/a       0
 *                      tests         2       2       2       0
 *                      asserts       5       5       5       0
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "CUnit/Basic.h"

#include "stem_lib.h"


node * head = NULL;

int init_suite1(void)
{
    printf("init suite1\n");
    return 0;
}

/* The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int clean_suite1(void)
{
    printf("\n\nclean_suite1\n");
    if(NULL != head)
    {
        delete_nodes(head);
        head = NULL;
    }
    delete_all_storage();
    return 0;
}

void test_null_case(void)
{
    CU_ASSERT_EQUAL(find_node(NULL,"A"),0);
}

void test_null_stem_stem(void)
{
    head = new_node("",0);
    CU_ASSERT_EQUAL(find_node(head,NULL),0);
}

void test_initial_case(void)
{
    delete_nodes(head);
    head = new_node("A",1);

    CU_ASSERT_EQUAL(find_node(head,"A"),1);
}

void test_add_existing(void)
{
    delete_nodes(head);
    head = new_node("A",1);

    head = count(head,"A",1);

    CU_ASSERT_EQUAL(find_node(head,"A"),2);
}



void test_one_empty(void)
{
    delete_nodes(head);
    head = NULL;

    head = count(head,"A",1);

    CU_ASSERT_EQUAL(find_node(head,"A"),1);
}

void test_double_entry_empty(void)
{
    delete_nodes(head);
    head = NULL;

    head = count(head,"AA",1);

    CU_ASSERT_EQUAL(find_node(head,"AA"),1);
}

void test_double_entry_then_split(void)
{
    delete_nodes(head);
    head = new_node("AA",2);

    head = count(head,"A",1);

    CU_ASSERT_EQUAL(find_node(head,"AA"),2);
    CU_ASSERT_EQUAL(find_node(head,"A"),1);
}

void test_double_entry_one_there(void)
{
    delete_nodes(head);
    head = new_node("A",2);

    head = count(head,"AA",1);

    CU_ASSERT_EQUAL(find_node(head,"AA"),1);
    CU_ASSERT_EQUAL(find_node(head,"A"),2);
}

void test_set_data(void)
{
    delete_nodes(head);
    head = new_node("A",2);

    head = count(head,"AA",1);
    head = count(head,"AVA",1);
    head = count(head,"ADA",1);
    head = count(head,"ADAM",1);
    head = count(head,"AVB",1);
    head = count(head,"AVC",1);
    head = count(head,"AVA",1);

    CU_ASSERT_EQUAL(find_node(head,"AA"),1);
    CU_ASSERT_EQUAL(find_node(head,"A"),2);
    CU_ASSERT_EQUAL(find_node(head,"AVA"),2);
    CU_ASSERT_EQUAL(find_node(head,"ADAM"),1);
    CU_ASSERT_EQUAL(find_node(head,"AVB"),1);
    CU_ASSERT_EQUAL(find_node(head,"AVC"),1);
    CU_ASSERT_EQUAL(find_node(head,"ADA"),1);

#if 0
    printf("\n\n");
    prettyprintEntries(head,"");
    printf("\n");
#endif

}

void test_small_set_data(void)
{
    delete_nodes(head);
    head = new_node("A",2);

    head = count(head,"AA",1);
    head = count(head,"AVA",1);
    head = count(head,"AVB",1);
    head = count(head,"AVC",1);
    head = count(head,"AVA",1);

    CU_ASSERT_EQUAL(find_node(head,"AVA"),2);
    CU_ASSERT_EQUAL(find_node(head,"AVB"),1);
    CU_ASSERT_EQUAL(find_node(head,"AVC"),1);
}

void test_add_long_long_keyword(void)
{
    delete_nodes(head);
    head = new_node("ABCDEFGHIJKLMNOP",2);

    CU_ASSERT_EQUAL(find_node(head,"ABCDEFGHIJKLMNOP"),2);

    printf("\n\n");
    prettyprintTree(head,"");
    printf("\n");
}

void test_long_set_data(void)
{
    delete_nodes(head);
    head = new_node("A",2);

    head = count(head,"AAAAAAA",1);
    head = count(head,"AVA",1);
    head = count(head,"AVB",1);
    head = count(head,"BETA",1);
    head = count(head,"ZETA",1);
    head = count(head,"AVC",1);
    head = count(head,"AVA",1);
    head = count(head,"AAAAAAA",1);
    head = count(head,"ABCDEFGHIJ",1);

    CU_ASSERT_EQUAL(find_node(head,"AAAAAAA"),2);
    CU_ASSERT_EQUAL(find_node(head,"BETA"),1);
    CU_ASSERT_EQUAL(find_node(head,"ZETA"),1);
    CU_ASSERT_EQUAL(find_node(head,"ABCDEFGHIJ"),1);

    printf("\n\n");
    prettyprintTree(head,"");
    printf("\n");
}

void test_single_entries_work()
{
    delete_nodes(head);
    head = NULL;

    head = count(head,"A",1);
    head = count(head,"1",1);

    CU_ASSERT_EQUAL(find_node(head,"1"),1);
    CU_ASSERT_EQUAL(find_node(head,"A"),1);
    CU_ASSERT_EQUAL(getNodesUsed(),2);
}

void test_dupes_numbers()
{
    delete_nodes(head);
    head = new_node("A",1);

    head = count(head,"1",1);
    head = count(head,"1",1);

    CU_ASSERT_EQUAL(find_node(head,"1"),2);
    CU_ASSERT_EQUAL(find_node(head,"A"),1);

}

void test_add_b_then_a()
{
    delete_nodes(head);
    head = new_node("B",1);

    head = count(head,"A",1);

    CU_ASSERT_EQUAL(find_node(head,"A"),1);
    CU_ASSERT_EQUAL(find_node(head,"B"),1);
}

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main()
{
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
   if (
           (NULL == CU_add_test(pSuite, "test of NULL find", test_null_case)) ||
           (NULL == CU_add_test(pSuite, "test of NULL stem find", test_null_stem_stem)) ||
           (NULL == CU_add_test(pSuite, "test of initial case", test_initial_case)) ||
           (NULL == CU_add_test(pSuite, "test of add existing case", test_add_existing)) ||
           (NULL == CU_add_test(pSuite, "test of add one on empty case", test_one_empty)) ||
           (NULL == CU_add_test(pSuite, "test of add AA on empty case", test_double_entry_empty)) ||
           (NULL == CU_add_test(pSuite, "test of add AA on A case", test_double_entry_one_there)) ||
           (NULL == CU_add_test(pSuite, "test of add really long case", test_add_long_long_keyword)) ||
           (NULL == CU_add_test(pSuite, "test of add a set of records", test_set_data)) ||
           (NULL == CU_add_test(pSuite, "test of add a long set of records", test_long_set_data)) ||
           (NULL == CU_add_test(pSuite, "test of add a set of single numbers", test_single_entries_work)) ||
           (NULL == CU_add_test(pSuite, "test of add a b then a", test_add_b_then_a)) ||
           (NULL == CU_add_test(pSuite, "test of add a set of dup numbers", test_dupes_numbers)) ||
           (NULL == CU_add_test(pSuite, "test of add a smaller set of records", test_small_set_data))
      )
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}

