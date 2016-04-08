#include <check.h>
#include "../include/calcul.h"

START_TEST(test_degrees_to_radian)
{
	ck_assert(degrees_to_radian(0.0) == 0.0);
	ck_assert(degrees_to_radian(360.) == 2*M_PI);
	ck_assert(degrees_to_radian(180.) == M_PI);
	ck_assert(degrees_to_radian(-180.) == -M_PI);
	ck_assert(degrees_to_radian(90.) == M_PI/2);
	ck_assert(degrees_to_radian(-90.) == -(M_PI/2));
}
END_TEST

START_TEST(test_lat_to_pixels)
{
	ck_assert(lat_to_pixels(39.7492900,39.7492900,39.7525610) == 1.);
	ck_assert(lat_to_pixels(39.7525610,39.7492900,39.7525610) == 0.);
}
END_TEST

Suite *calcul_suite(void)
{
	TCase *tc_core;
	Suite *s;
	
	tc_core = tcase_create("Core");
	tcase_add_test(tc_core,test_degrees_to_radian);
	tcase_add_test(tc_core,test_lat_to_pixels);
	//Add other tests

	s = suite_create("Calcul");
	suite_add_tcase(s,tc_core);
	//add other tcase

	return s;
}

int main()
{
	int failures;
	Suite *s;
	SRunner *sr;

	s = calcul_suite();
	sr = srunner_create(s);

	srunner_run_all(sr,CK_VERBOSE);
	failures = srunner_ntests_failed(sr);
	srunner_free(sr);

	return (failures == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
	
}

