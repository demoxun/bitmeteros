#include <stdlib.h>
#include <stdio.h>
#include "common.h"
#include <test.h> 
#include <stdarg.h> 
#include <stddef.h> 
#include <setjmp.h> 
#include <cmockery.h> 
#include "client.h"
#include "bmws.h"

/*
Contains unit tests for the nameValuePair module.
*/

void testGetValueForName(void** state) {
 // Checks that the getValueForName function works correctly
	assert_true(getValueForName("p", NULL, NULL) == (char*)NULL);

 // A single pair
	struct NameValuePair* param1 = malloc(sizeof(struct NameValuePair));
	param1->name  = "a";
	param1->value = "b";
	param1->next = NULL;
	assert_true(getValueForName("p", param1, NULL) == NULL);
	assert_string_equal("x", getValueForName("p", param1, "x"));
	assert_string_equal("b", getValueForName("a", param1, NULL));
	assert_string_equal("b", getValueForName("a", param1, "x"));

 // Multiple pairs
	struct NameValuePair* param2 = malloc(sizeof(struct NameValuePair));
	param1->next = param2;
	param2->name  = "c";
	param2->value = "d";
	param2->next = NULL;
	assert_true(getValueForName("p", param1, NULL) == NULL);
	assert_string_equal("x", getValueForName("p", param1, "x"));
	assert_string_equal("b", getValueForName("a", param1, NULL));
	assert_string_equal("b", getValueForName("a", param1, "x"));
	assert_string_equal("d", getValueForName("c", param1, NULL));
	assert_string_equal("d", getValueForName("c", param1, "x"));

	free(param1);
	free(param2);
}

void testGetValueNumForName(void** state) {
 // Checks that the getValueNumForName function works correctly
	assert_int_equal(-1, getValueNumForName("p", NULL, -1));

 // A single pair
	struct NameValuePair* param1 = malloc(sizeof(struct NameValuePair));
	param1->name  = "a";
	param1->value = "7";
	param1->next = NULL;
	assert_int_equal(-1, getValueNumForName("p", param1, -1));
	assert_int_equal( 7, getValueNumForName("a", param1, -1));

 // Multiple pairs
	struct NameValuePair* param2 = malloc(sizeof(struct NameValuePair));
	param1->next = param2;
	param2->name  = "c";
	param2->value = "0";
	param2->next = NULL;
	assert_int_equal(-1, getValueNumForName("p", param1, -1));
	assert_int_equal( 7, getValueNumForName("a", param1, -1));
	assert_int_equal( 0, getValueNumForName("c", param1, -1));

	struct NameValuePair* param3 = malloc(sizeof(struct NameValuePair));
	param2->next = param3;
	param3->name  = "x";
	param3->value = "notanumber";
	param3->next = NULL;
	assert_int_equal(-1, getValueNumForName("p", param1, -1));
	assert_int_equal( 7, getValueNumForName("a", param1, -1));
	assert_int_equal( 0, getValueNumForName("c", param1, -1));
	assert_int_equal(-1, getValueNumForName("x", param1, -1));

	free(param1);
	free(param2);
	free(param3);
}