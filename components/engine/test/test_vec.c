#include "engine_test_utils.h"
#include "unity.h"
#include "unity_test_runner.h"
#include "vec.h"

static const char *TAG = "test_vec";

#define VEC(x, y) \
	(struct vec)  \
	{             \
		x, y      \
	}

TEST_CASE("Test vec_eq (equal)", "[vec]")
{
	struct vec vecs[] = { { -5, 34 }, { 3459, -342 }, { 0, 0 } };
	for (size_t i = 0; i < ARR_SIZE(vecs); i++) {
		TEST_ASSERT_LOG(vec_eq(vecs[i], vecs[i]), "%d;%d == %d;%d", vecs[i].x,
						vecs[i].y, vecs[i].x, vecs[i].y);
	}
}

TEST_CASE("Test vec_eq (not equal)", "[vec]")
{
	struct vec vecs[] = {
		{ -5, 34 }, { 3459, -342 }, { 0, 0 }, { -128, 128 }, { 128, -128 }
	};
	for (size_t i = 0; i < ARR_SIZE(vecs) - 1; i++) {
		for (size_t j = i + 1; j < ARR_SIZE(vecs); j++) {
			TEST_ASSERT_LOG(!vec_eq(vecs[i], vecs[j]), "%d;%d != %d;%d",
							vecs[i].x, vecs[i].y, vecs[j].x, vecs[j].y);
		}
	}
}

TEST_CASE("Test vec_add", "[vec]")
{
	struct vec_check {
		struct vec v1;
		struct vec v2;
		struct vec res;
	};
	struct vec_check check_table[] = {
		{ { 0, 0 }, { 0, 0 }, { 0, 0 } },
		{ { -35, 934 }, { 713, -8564 }, { 678, -7630 } },
		{ { -132, 0 }, { 0, 132 }, { -132, 132 } },
	};
	for (size_t i = 0; i < ARR_SIZE(check_table) - 1; i++) {
		TEST_ASSERT_LOG(vec_eq(vec_add(check_table[i].v1, check_table[i].v2),
							   check_table[i].res),
						"%d;%d + %d;%d == %d;%d", check_table[i].v1.x,
						check_table[i].v1.y, check_table[i].v2.x,
						check_table[i].v2.y, check_table[i].res.x,
						check_table[i].res.y);
	}
}

TEST_CASE("Test vec_sub", "[vec]")
{
	struct vec_check {
		struct vec v1;
		struct vec v2;
		struct vec res;
	};
	struct vec_check check_table[] = {
		{ { 0, 0 }, { 0, 0 }, { 0, 0 } },
		{ { 7, 8 }, { 6, 7 }, { 1, 1 } },
		{ { -35, 934 }, { -713, 8564 }, { 678, -7630 } },
		{ { -132, 0 }, { 0, -132 }, { -132, 132 } },
	};
	for (size_t i = 0; i < ARR_SIZE(check_table) - 1; i++) {
		TEST_ASSERT_LOG(vec_eq(vec_sub(check_table[i].v1, check_table[i].v2),
							   check_table[i].res),
						"%d;%d - %d;%d == %d;%d", check_table[i].v1.x,
						check_table[i].v1.y, check_table[i].v2.x,
						check_table[i].v2.y, check_table[i].res.x,
						check_table[i].res.y);
	}
}

TEST_CASE("Test vec_mul", "[vec]")
{
	struct vec_check {
		struct vec v1;
		int scalar;
		struct vec res;
	};
	struct vec_check check_table[] = {
		{ { 0, 0 }, 234, { 0, 0 } },
		{ { 475, -13485 }, 0, { 0, 0 } },
		{ { 1283, -4823 }, 1, { 1283, -4823 } },
		{ { 1283, -4823 }, -1, { -1283, 4823 } },
		{ { -1, 2 }, 32, { -32, 64 } },
		{ { -35, 934 }, -8564, { 299740, -7998776 } },
	};
	for (size_t i = 0; i < ARR_SIZE(check_table) - 1; i++) {
		TEST_ASSERT_LOG(
			vec_eq(vec_mul(check_table[i].v1, check_table[i].scalar),
				   check_table[i].res),
			"%d;%d * %d == %d;%d", check_table[i].v1.x, check_table[i].v1.y,
			check_table[i].scalar, check_table[i].res.x, check_table[i].res.y);
	}
}
