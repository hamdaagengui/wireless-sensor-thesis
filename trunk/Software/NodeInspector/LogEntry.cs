using System;

namespace NodeInspector
{
	class LogEntry
	{
		public const int TYPE_MESSAGE = 0;
		public const int TYPE_WARNING = 1;
		public const int TYPE_ERROR = 2;

		public const int LEVEL_APPLICATION = 0;
		public const int LEVEL_FRAMEWORK = 1;

		public int Source;
		public int Type;
		public int Level;
		public UInt32 Number;
		public DateTime Time;
		public string Text;
	}
}
