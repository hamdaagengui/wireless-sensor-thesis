// This file is auto generated by the Message Builder.
// Don't bother making changes to this file as they will be lost when recompiling.

// Message Builder by Claus Andersen.

using System;

using System.Reflection;

namespace RfSuit
{
	class MessageDispatcher
	{
		interface IMessageHandler
		{
			void Handle(Byte[] frame);
		}

		public delegate void MessageHandlerDelegate<T>(T message);
		
		class MessageHandler<T> : IMessageHandler where T : class
		{
			MessageHandlerDelegate<T> handler;
			ConstructorInfo constructor;

			public MessageHandler(MessageHandlerDelegate<T> handler)
			{
				this.handler = handler;
				constructor = typeof(T).GetConstructor(new Type[] { typeof(Byte[]) });
			}

			public void Handle(Byte[] frame)
			{
				var msg = constructor.Invoke(new object[] { frame }) as T;
				handler(msg);
			}
		}

		IMessageHandler[] handlers = new IMessageHandler[256];
	
		public delegate bool MessagePredicateDelegate(Byte source);
		public event MessagePredicateDelegate MessagePredicate;

		public void HandleFrame(byte[] frame)
		{
			Byte source = frame[0];
			Byte messageId = frame[1];

			if (MessagePredicate == null || MessagePredicate(source))
			{
				var handler = handlers[messageId];
				if (handler != null)
				{
					handler.Handle(frame);
				}
			}
		}

		public void AddHandler<T>(MessageHandlerDelegate<T> handler) where T : class
		{
			dynamic id = typeof(T).GetField("MessageId", BindingFlags.NonPublic | BindingFlags.Static).GetValue(null);

			handlers[id] = new MessageHandler<T>(handler);
		}
	}

	class StartScanMessage
	{
		public Byte Source;
		static readonly Byte MessageId = 0;
		public Byte Interval;
		public Byte Channel;
		public Byte TxPower;

		public StartScanMessage()
		{ }

		public StartScanMessage(Byte[] frame)
		{
			Source = frame[0];
			Interval = frame[2];
			Channel = frame[3];
			TxPower = frame[4];
		}

		static public Byte[] Create(Byte source, Byte interval, Byte channel, Byte txPower)
		{
			Byte[] buffer = new Byte[5];
			buffer[0] = source;
			buffer[1] = MessageId;
			buffer[2] = interval;
			buffer[3] = channel;
			buffer[4] = txPower;
			return buffer;
		}

		public Byte[] Create()
		{
			return Create(Source, Interval, Channel, TxPower);
		}
	}

	class ReportResultsMessage
	{
		public Byte Source;
		static readonly Byte MessageId = 1;
		public Byte[] Rssis = new Byte[16];

		public ReportResultsMessage()
		{ }

		public ReportResultsMessage(Byte[] frame)
		{
			Source = frame[0];
			Array.Copy(frame, 2, Rssis, 0, 16);
		}

		static public Byte[] Create(Byte source, Byte[] rssis)
		{
			Byte[] buffer = new Byte[18];
			buffer[0] = source;
			buffer[1] = MessageId;
			Array.Copy(rssis, 0, buffer, 2, 16);
			return buffer;
		}

		public Byte[] Create()
		{
			return Create(Source, Rssis);
		}
	}

}
