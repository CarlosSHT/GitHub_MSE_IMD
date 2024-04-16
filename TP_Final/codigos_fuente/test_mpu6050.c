#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

#define MPU_WAKEUP 0
#define MPU_SETDATARATE 1
#define MPU_CONF_ACCEL 2
#define MPU_READ_ACCEL 4

#define DELAY_FUNCS	2000000
#define DELAY_READS	100000

int main(void)
{
	int i=0;
	uint8_t val_data[6];

    printf("Inicio aplicacion Test MPU6050\n");
	
	int my_dev = open("/dev/mse_mpu6050_00", 0);

	if (my_dev < 0)
	{
		perror("Fail to open device file: /dev/mse_mpu6050_00");
	}
	else
	{
		ioctl(my_dev, MPU_WAKEUP, 0); 
		usleep(DELAY_FUNCS);

		ioctl(my_dev, MPU_SETDATARATE, 0x07); 
		usleep(DELAY_FUNCS);

		ioctl(my_dev, MPU_CONF_ACCEL, 0x00); 
		usleep(DELAY_FUNCS);

		for (i = 1; i <= 2000; ++i) {
			ioctl(my_dev, MPU_READ_ACCEL, &val_data); 
			printf("Recibido ax 0x%02X%02X\n", val_data[0], val_data[1]);
			printf("Recibido ay 0x%02X%02X\n", val_data[2], val_data[3]);
			printf("Recibido az 0x%02X%02X\n\n", val_data[4], val_data[5]);
			usleep(DELAY_READS);
		}

		close(my_dev);

	}
	close(my_dev);

	return 0;
}
