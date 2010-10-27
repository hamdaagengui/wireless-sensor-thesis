using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

namespace NodeInspector
{
	class DynamicVariable : INotifyPropertyChanged
	{
		public event PropertyChangedEventHandler PropertyChanged;

		string name;
		public string Name
		{
			get { return name; }
			set
			{
				name = value;
				this.NotifyPropertyChanged("Name");
			}
		}

		int val;
		public int Value
		{
			get { return val; }
			set
			{
				val = value;
				this.NotifyPropertyChanged("Value");
			}
		}

		void NotifyPropertyChanged(string name)
		{
			if (PropertyChanged != null)
			{
				PropertyChanged(this, new PropertyChangedEventArgs(name));
			}
		}
	}
}
