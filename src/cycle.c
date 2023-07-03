#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "ws2811.h"
#include "config.h"
#include "cycle.h"

#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))

const double phase_mid = 0.81;
const double max_brightness = 0.5;

void run_daynight_cycle() {
	const ws2811_t* leds = config->ledstring;
	const int num_leds = config->width * config->height;
	const unsigned night = 0x00000000;
	const unsigned day = 0x10101010;
	const double sunrise = 5.5 * 60.0 * 60.0;
	const double sunset = 21.0 * 60.0 * 60.0;
	const double noon = 13.0 * 60.0 * 60.0;
	const double change1 = ((noon - sunrise) / 1.75) + sunrise;
	const double change2 = ((sunset - noon) / 2.0) + noon;
	const double phase1_cf = phase_mid / (change1 - sunrise);
	const double phase2_cf = (1.0 - phase_mid) / (noon - change1);
	const double phase3_cf = (phase_mid - 1.0) / (change2 - noon);
	const double phase4_cf = -phase_mid / (sunset - change2); 
	struct timespec current_time;
	struct tm* info;
	double daytime = 0.0;
	double raw_result = 0.0, raw_red = 0.0, raw_green = 0.0, raw_blue = 0.0;
	unsigned mapped_color = 0;

	while (config->running) {
		clock_gettime(CLOCK_REALTIME, &current_time);
		info = localtime(&current_time.tv_sec);

		daytime = ((((info->tm_hour * 60.0 * 60.0) + (info->tm_min * 60.0) + info->tm_sec) * 1000.0) + (current_time.tv_nsec / 1000000)) / 1000.0;

		if (daytime >= sunrise && daytime <= change1) {
			printf("P1 ");
			raw_result = (phase1_cf * (daytime - sunrise))/* / 1000.0 */;
		} else if (daytime >= change1 && daytime <= noon) {
			printf("P2 ");
			raw_result = ((phase2_cf * (daytime - change1))/* / 1000.0 */) + phase_mid;
		} else if (daytime >= noon && daytime <= change2) {
			printf("P3 ");
			raw_result = ((phase3_cf * (daytime - noon))/* / 1000.0*/) + 1.0;
		} else if (daytime >= change2 && daytime <= sunset) {
			printf("P4 ");
			raw_result = ((phase4_cf * (daytime - change2))/* / 1000.0*/) + phase_mid;
		} else {
			printf("PN ");
			raw_result = 0.0;
		}
		
		raw_red = max((raw_result > 0.5239876754 ? ((-1.5 * pow(raw_result - 1.0, 2.0)) + 1.0) : ((-2.0 * pow((3.2 * raw_result) - 1.0, 4.0)) + (2.0 * pow((3.1 * raw_result) - 1.0, 2.0)) + 0.3)) * max_brightness, 0.0);
		raw_green = max(((-2.0 * pow(raw_result - 1.0, 2.0)) + 1.0) * max_brightness, 0.09);
		raw_blue = max((raw_result > 0.5386578985 ? ((-1.0 * pow(raw_result - 1.0, 2.0)) + 1.0) : ((-2.0 * pow((3.2 * raw_result) - 1.2, 4.0)) + (2.0 * pow((2.9 * raw_result) - 0.95, 3.0)) + 0.46)) * max_brightness, 0.125);
		mapped_color = (((int)(min(raw_result, 1.0) * max_brightness * 255)) << 24)
				| ((((int)(raw_red * 255)) & 0xFF) << 16)
				| ((((int)(raw_green * 255)) & 0xFF) << 8)
				| (((int)(raw_blue * 255)) & 0xFF);

		printf("%lf %lf %lf %lf %lf %lf %d\n", daytime, raw_result, min(raw_result, 1.0) * max_brightness, raw_red, raw_green, raw_blue, num_leds);

		for (int i = 0; i < num_leds; ++i) {
			leds->channel[0].leds[i] = mapped_color;
		}

		usleep(1000000 / 15);
	}
}
