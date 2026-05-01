#ifndef __ENGINE_TEST_UTILS_H__
#define __ENGINE_TEST_UTILS_H__

#include "esp_log.h"
#include "unity.h"

#define ARR_SIZE(x) sizeof(x) / sizeof(x[0])

#define TEST_ASSERT_LOG(condition, format, ...)               \
	do {                                                      \
		if (!condition) {                                     \
			ESP_LOGE(TAG, format __VA_OPT__(, ) __VA_ARGS__); \
		}                                                     \
		TEST_ASSERT(condition);                               \
	} while (0)

#endif // __ENGINE_TEST_UTILS_H__
