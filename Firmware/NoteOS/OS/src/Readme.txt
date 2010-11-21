Debug
	Define 'DEBUG' to enable debugging in the framework code.
	Level 0: Detect but ignore errors.
	Level 1: Detect errors and send debug messages.
	Level 2: Detect errors and send debug messages. Freeze program.
	Level 3: Detect errors and freeze program.

Log
	Define 'LOG' to enable logging in the framework code.
	Level 1: 

Statistics
	Define 'STATISTICS' to enable statistics variables and code in the framework code.
	Level 1: 

Processor peripherals
	Communicating/Asynchronous communication peripheral
		UART (uint8_t[])
	Transactional/Synchronous communication peripheral
		SPI (void)
		IIC (void)
	Sampling/Constant rate/Continues/Streaming peripheral
		ADC (uint16_t)
		Timer (void)
	Notifying peripheral
		Input capture (uint16_t)
		GPIO change (uint64_t)
		Analog comparator (uint8_t)
	Output peripheral
		GPIO
		PWM
	Input peripheral
		GPIO
		ADC (cached values)
		Analog comparator



Connecting

	1. Register QR on GW
	2. 

Routing

	On FrameReceive:
		On EnergyLevelCommand => routeInfo[frame.source].energyLevel = value
		On QueueLevelCommand => routeInfo[frame.source].queueLevel = value
		On BusyLevelCommand => routeInfo[frame.source].busyLevel = value
		On StaticInfoCommand => routeInfo[frame.source].? = values

	1. Find the desired route in the routing table
	2. Determine the tx power needed to reach the first node in the route
	3. Send frame