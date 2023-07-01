
all:
	gcc -I./rpi_ws281x rpi_ws281x/mailbox.c rpi_ws281x/ws2811.c rpi_ws281x/pcm.c rpi_ws281x/dma.c rpi_ws281x/rpihw.c rpi_ws281x/pwm.c src/*.c -o geckoled -g -pthread -lm

clean:
	rm -f geckoled
