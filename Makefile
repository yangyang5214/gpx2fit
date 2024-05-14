CFLAGS = -I. fit/fit.c fit/fit_example.c fit/fit_crc.c fit/fit_convert.c


build:
	@mkdir -p  bin
	@gcc $(CFLAGS) main.c -o bin/gpx2fit -lxml2
