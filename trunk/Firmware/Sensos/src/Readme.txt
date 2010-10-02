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



Messages

	Configuration (CFG)
		msg:4
		id:4
		timeSlot:16
		serialNumber:8 [16]
		
	Sensor Data (SED)
		msg:4
		nextNode:4
		sequenceNumber:4
		length:4
		sensor:8
		data:8 [16]

	Sensor Data Acknowledge (SDA)
		msg:4
		nextNode:4
		destination:4
		sequenceNumber:4

	Route Information (RIN)
		msg:4
		node:4
		rssi0:5
		...
		rssi15:5
		
	Tx Level (TXL)
		msg:4
		node:4
		value:8
		
	Energy Level (ENL)
		msg:4
		node:4
		value:8
		
	Queue Level (QUL)
		msg:4
		node:4
		value:8
		
	Busy Level (BUL)
		msg:4
		node:4
		value:8
	
	Status Report (STA)
		msg:4
		node:4
		txLevel:8
		energyLevel:8
		queueLevel:8
		busyLevel:8
	
Data framing
	SOF = 255
	
Message example
	uint8 uint8 uint8
	SOF <byte> <byte> <byte>

	
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