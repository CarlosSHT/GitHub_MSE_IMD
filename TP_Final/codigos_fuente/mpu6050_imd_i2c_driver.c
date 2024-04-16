// SPDX-License-Identifier: GPL-2.0-or-later

#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include <linux/of.h>
#include <linux/types.h>
#include <linux/string.h>

#define NAME_LEN_DEV	14
#define MPU_ID_VAL			0x68
#define I2C_M_WRITE			0

#define LENGTH_READ_REG		2
#define LENGTH_WRITE_REG	2

#define REG_WHO_AM_I		0x75
#define REG_PWR_MGMT_1		0x6B
#define REG_SMPLRT_DIV		0x19
#define REG_ACCEL_CONFIG	0x1C
#define REG_GYRO_CONFIG		0x1B
#define REG_ACCEL_XOUT_H	0x3B
#define REG_GYRO_XOUT_H		0x43

#define WAKEUP_VAL_DAT		0x00

/*
 * Definicion de los ID correspondientes al Device Tree. Estos deben ser informados al
 * kernel mediante la macro MODULE_DEVICE_TABLE
 *
 * NOTA: Esta seccion requiere que CONFIG_OF=y en el kernel
 */

static const struct of_device_id mse_dt_ids[] = {
	{ .compatible = "mse,IMD_TPF", },
	{ /* sentinel */ }
};

MODULE_DEVICE_TABLE(of, mse_dt_ids);

/* Private device structure */
struct mse_dev {
	struct i2c_client *client;
	struct miscdevice mse_miscdevice;
	char name[NAME_LEN_DEV];
	bool isready;
};

/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
enum {
	mpu_wakeup = 0,
	mpu_setsampling,
	mpu_accelconf,
	mpu_gyroscopconf,
	mpu_readaccel,
	mpu_readgyroscop,
};

/* Functions for reading and writing registers of the MPU6050 */
static int mpu6050_read_register(struct i2c_adapter *adap, uint8_t addr, uint8_t reg, uint8_t *val)
{
	struct i2c_msg msgs[LENGTH_READ_REG];
	uint8_t i2c_tx_buff;
	uint8_t i2c_rx_buff;
	int ret_val;

	/* Write I2C send register to read */
	i2c_tx_buff = reg;
	msgs[0].addr = addr;
	msgs[0].flags = I2C_M_WRITE;	// to read
	msgs[0].len = sizeof(i2c_tx_buff)/sizeof(uint8_t);
	msgs[0].buf = &i2c_tx_buff;

	/* Read I2C read register value */
	msgs[1].addr = addr;
	msgs[1].flags = I2C_M_RD; // Para lectura
	msgs[1].len = sizeof(i2c_rx_buff)/sizeof(uint8_t);
	msgs[1].buf = &i2c_rx_buff;

	ret_val = i2c_transfer(adap, msgs, 2);
	*val = i2c_rx_buff;
	if (ret_val < 0) {
		*val = 0;
		pr_err("%s", "Error : Can't use I2C bus\n");
		return ret_val;
	}

	pr_info("MPU6050 reg:0x%02Xh is val:0x%02Xh", reg, *val);
	return ret_val;
}


static int mpu6050_write_register(const struct i2c_client *client, uint8_t reg, const char *buf)
{
	uint8_t tx_buffer[LENGTH_WRITE_REG];

	tx_buffer[0] = reg;
	tx_buffer[1] = *buf;

	pr_info("MPU6050 writing in reg:0x%02Xh value:0x%02Xh\n", tx_buffer[0], tx_buffer[1]);

	return i2c_master_send(client, tx_buffer, LENGTH_WRITE_REG);
}


static int mpu6050_read_register_block(const struct i2c_client *client, uint8_t reg, uint8_t *buff_rx, int count)
{
	int ret_val;
	uint8_t tx_buffer;

	tx_buffer = reg;

	ret_val = i2c_master_send(client, &tx_buffer, 1);
	if (ret_val < 0)
		return ret_val;

	ret_val = i2c_master_recv(client, buff_rx, count);
	if (ret_val < 0)
		return ret_val;

	return 0;
}

/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/

/* Functions used for file operations */
static int mse_mpu6050_open(struct inode *node, struct file *file)
{
	struct mse_dev *mse_mpu6050;
	uint8_t id_val;
	int ret_val;

	mse_mpu6050 = container_of(file->private_data, struct mse_dev, mse_miscdevice);

	/*
	 * Aqui ira las llamadas a i2c_transfer() que correspondan pasando
	 * como dispositivo mse_mpu6050->client
	 */
	pr_info("my device address is : 0x%02Xh\n", mse_mpu6050->client->addr);
	id_val = 0;
	ret_val = mpu6050_read_register(mse_mpu6050->client->adapter, mse_mpu6050->client->addr, REG_WHO_AM_I, &id_val);

	if (ret_val < 0)
		return ret_val;

	if (id_val != MPU_ID_VAL) {
		mse_mpu6050->isready = false;
		pr_warn("MPU6050 device is disconnected\n");
	}

	pr_warn("MPU6050 device is connected\n");
	mse_mpu6050->isready = true;

	pr_info("%s", "mse_mpu6050_open() called.");
	return 0;
}

static ssize_t mse_mpu6050_read(struct file *file, char __user *userbuf, size_t count, loff_t *ppos)
{
	struct mse_dev *mse_mpu6050;

	mse_mpu6050 = container_of(file->private_data, struct mse_dev, mse_miscdevice);

	/*
	 * Aqui ira las llamadas a i2c_transfer() que correspondan pasando
	 * como dispositivo mse_mpu6050->client
	 */

	pr_info("%s", "mse_mpu6050_read() fue invocada.");
	return 0;
}

static ssize_t mse_mpu6050_write(struct file *file, const char __user *buffer, size_t len, loff_t *offset)
{
	struct mse_dev *mse_mpu6050;

	mse_mpu6050 = container_of(file->private_data, struct mse_dev, mse_miscdevice);

	/*
	 * Aqui ira las llamadas a i2c_transfer() que correspondan pasando
	 * como dispositivo mse_mpu6050->client
	 */

	pr_info("%s", "mse_mpu6050_write() fue invocada.");
	return 0;
}

static long mse_mpu6050_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct mse_dev *mse_mpu6050;
	int ret_val;
	uint8_t recv_Data[6];

	mse_mpu6050 = container_of(file->private_data, struct mse_dev, mse_miscdevice);

	if (mse_mpu6050->isready == false)
	{
		return -1;
	}

	uint8_t val_dat;

	/*
	 * Aqui ira las llamadas a i2c_transfer() que correspondan pasando
	 * como dispositivo mse_mpu6050->client
	 */
	switch (cmd) {
	case mpu_wakeup:
		val_dat = WAKEUP_VAL_DAT;
		ret_val = mpu6050_write_register(mse_mpu6050->client, REG_PWR_MGMT_1, &val_dat);
		if (ret_val < 0)
			pr_err("%s", "Error : Can't write wakeup mpu6050");
		break;

	case mpu_setsampling:
		val_dat = arg;	// data rate
		ret_val = mpu6050_write_register(mse_mpu6050->client, REG_SMPLRT_DIV, &val_dat);
		if (ret_val < 0)
			pr_err("%s", "Error : Can't write data rate mpu6050");
		break;

	case mpu_accelconf:
		val_dat = arg;	// accel config ±2g, ±4g, ±8g, ±16g
		ret_val = mpu6050_write_register(mse_mpu6050->client, REG_ACCEL_CONFIG, &val_dat);
		if (ret_val < 0)
			pr_err("%s", "Error : Can't write accel config mpu6050");
		break;

	case mpu_gyroscopconf:
		val_dat = arg;	// gyroscope config ±250 °/s, ±500 °/s, 1000 °/s, 2000 °/s
		ret_val = mpu6050_write_register(mse_mpu6050->client, REG_GYRO_CONFIG, &val_dat);
		if (ret_val < 0)
			pr_err("%s", "Error : Can't write accel config mpu6050");
		break;

	case mpu_readaccel:
		ret_val = mpu6050_read_register_block(mse_mpu6050->client, REG_ACCEL_XOUT_H, recv_Data, sizeof(recv_Data));
		if (ret_val < 0)
			pr_err("%s", "Error : Can't read accel values from MPU6050\n");
		
		copy_to_user((void __user *)arg, recv_Data, sizeof(recv_Data));

		pr_info("Accel Ax hexadecimal 0x%02X%02X\n", recv_Data[0], recv_Data[1]);
		pr_info("Accel Ay hexadecimal 0x%02X%02X\n", recv_Data[2], recv_Data[3]);
		pr_info("Accel Az hexadecimal 0x%02X%02X\n\n", recv_Data[4], recv_Data[5]);

		break;

	case mpu_readgyroscop:
		ret_val = mpu6050_read_register_block(mse_mpu6050->client, REG_GYRO_XOUT_H, recv_Data, sizeof(recv_Data));
		if (ret_val < 0)
			pr_err("%s", "Error : Can't read gryroscope values from MPU6050\n");

		copy_to_user((void __user *)arg, recv_Data, sizeof(recv_Data));

		pr_info("Gyroscope Gx hexadecimal 0x%02X%02X\n", recv_Data[0], recv_Data[1]);
		pr_info("Gyroscope Gy hexadecimal 0x%02X%02X\n", recv_Data[2], recv_Data[3]);
		pr_info("Gyroscope Gz hexadecimal 0x%02X%02X\n\n", recv_Data[4], recv_Data[5]);

		break;

	default:
		break;
	}
	
	return 0;
}

/* Structure of file_operations */
static const struct file_operations mse_mpu6050_fops = {
	.owner = THIS_MODULE,
	.open = mse_mpu6050_open,
	.read = mse_mpu6050_read,
	.write = mse_mpu6050_write,
	.unlocked_ioctl = mse_mpu6050_ioctl,
};

/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
static int mse_mpu6050_probe(struct i2c_client *client)
{
	struct mse_dev *mse_mpu6050;
	static int counter;
	int ret_val;

	/* Allocate new private structure */
	mse_mpu6050 = devm_kzalloc(&client->dev, sizeof(struct mse_dev), GFP_KERNEL);

	/* Store pointer to the device-structure in bus device context */
	i2c_set_clientdata(client, mse_mpu6050);

	/* Store pointer to I2C client device in the private structure */
	mse_mpu6050->client = client;

	/* Initialize the misc device, mse is incremented after each probe call */
	sprintf(mse_mpu6050->name, "mse_mpu6050_%02d", counter++);

	/* Initialize false device is ready */
	mse_mpu6050->isready = false;

	mse_mpu6050->mse_miscdevice.name = mse_mpu6050->name;
	mse_mpu6050->mse_miscdevice.minor = MISC_DYNAMIC_MINOR;
	mse_mpu6050->mse_miscdevice.fops = &mse_mpu6050_fops;

	/* Register misc device */
	ret_val = misc_register(&mse_mpu6050->mse_miscdevice);

	if (ret_val != 0) {
		pr_err("Device registration failed %s\n", mse_mpu6050->mse_miscdevice.name);
		return ret_val;
	}

	pr_info("Device %s: minor assigned: %i\n", mse_mpu6050->mse_miscdevice.name, mse_mpu6050->mse_miscdevice.minor);

	return 0;
}

static void mse_mpu6050_remove(struct i2c_client *client)
{
	struct mse_dev *mse_mpu6050;

	/* Get device structure from bus device context */
	mse_mpu6050 = i2c_get_clientdata(client);

	/* Deregister misc device */
	misc_deregister(&mse_mpu6050->mse_miscdevice);

	pr_info("Device %s: successfully removed\n", mse_mpu6050->mse_miscdevice.name);
}

/*--------------------------------------------------------------------------------*/

static struct i2c_driver mse_driver_tpf = {
	.probe = mse_mpu6050_probe,
	.remove = mse_mpu6050_remove,
	.driver = {
		.name = "mse_mpu6050_driver_tpf",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(mse_dt_ids),
	},
};

/*----------------------------------------------------------------------*/

module_i2c_driver(mse_driver_tpf);


MODULE_AUTHOR("Carlos Herrera Trujillo <carlos.herrera.trujillo@gmail.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Modulo driver MPU6050 para el TP Final del curso IMD");
MODULE_INFO(mse_imd, "Driver Ver 1 mpu6050");

