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

	Configuration (CF)
		msg:3
		:1
		id:4
		time:16
		serialNumber:8 [16]
		
	Data (DA)
		msg:3
		sensor:5
		nextNode:4
		sequenceNumber:4
		data:8 [32]
		
	Acknowledge (AC)
		msg:3
		:1
		destination:4
		nextNode:4
		sequence:4
		
	Route Information (RI)
		msg:3
		:1
		source:4
		node0:4
		
	Tx Level (TL)
		msg:3
		value:5
		
	Energy Level (EL)
		msg:3
		value:5
		
	Queue Level (QL)
		msg:3
		value:5
		
	Busy Level (BL)
		msg:3
		value:5
	
	
Data framing
	SOF = 255
	
Message example
	uint8 uint8 uint8
	SOF <byte> <byte> <byte>




Super cycle -> enable slot sharing



	
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