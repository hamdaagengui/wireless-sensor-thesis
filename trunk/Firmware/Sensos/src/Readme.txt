Debug
	Define 'DEBUG' to enable debugging in the framework code.
	Level 1: 

Log
	Define 'LOG' to enable logging in the framework code.
	Level 1: 

Statistics
	Define 'STATISTICS' to enable statistics variables and code in the framework code.
	Level 1: 

Processor peripherals
	Communicating peripheral / Asynchronous communication peripheral
		UART
	Transactional peripheral / Synchronous communication peripheral
		SPI
		IIC
	Sampling peripheral
		ADC
		Timer
	Notifying peripheral
		Input capture
		GPIO change
		Analog comparator
	Outputing
		GPIO
		PWM





IIC based sensor example - AccelSensor.c

	iicConfiguration configuration = {IIC_STANDARD_SPEED, Sensor_Completion};
	uint8_t adrBuf[2] = {2 >> 8, 2};
	uint8_t dataBuf[3 * 2];
	int16_t dataBuf[3];
	
	#define ACCEL_IIC_ADR  0x94
	
	int16_t x, y, z;

	Sensor_Update()
	{
		Iic_BeginTransaction(configuration);
		Iic_StartWrite(ACCEL_IIC_ADR, adrBuf, 2);
		Iic_Read(ACCEL_IIC_ADR, dataBuf, 6);
	 	Iic_EndTransaction();
	}
	
	Sensor_Completion()
	{
		x = *((int16_t*)&dataBuf[0]);
		y = *((int16_t*)&dataBuf[2]);
		z = *((int16_t*)&dataBuf[4]);
		PostSensorUpdate(SENSOR_ID, x, y, z);
		
	}




SPI based sensor example - LightSensor.c

	const spiConfiguration configuration = {GPIOA2, SPI_BITRATE_1000000, Sensor_Completion};
	const uint8_t outbuf[4] = {CMD_READ_REG, 1};
	uint8_t inbuf[4];
	
	uint16_t value;

	Sensor_Update()
	{
		Spi_Transfer(&configuration, outbuf, inbuf, 4);
	}
	
	Sensor_Completion()
	{
		value = *((uint16_t*)&inbuf[2]);
		PostSensorUpdate(SENSOR_ID, value);
	}