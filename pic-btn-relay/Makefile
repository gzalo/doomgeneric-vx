all: demo.hex

demo.hex: demo.c fuses.h
	xc8 --chip=12F683 --outdir=output demo.c