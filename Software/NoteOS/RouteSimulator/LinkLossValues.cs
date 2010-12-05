using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

namespace RouteSimulator
{
	class LinkLossValues : INotifyPropertyChanged
	{
		public event PropertyChangedEventHandler PropertyChanged;

		int node;
		public int Node
		{
			get { return node; }
			set
			{
				node = value;
				this.NotifyPropertyChanged("Node");
			}
		}

		public int[] Loss = new int[15];

		public int Loss0
		{
			get { return Loss[0]; }
			set
			{
				Loss[0] = value;
				this.NotifyPropertyChanged("Loss0");
			}
		}

		public int Loss1
		{
			get { return Loss[1]; }
			set
			{
				Loss[1] = value;
				this.NotifyPropertyChanged("Loss1");
			}
		}

		public int Loss2
		{
			get { return Loss[2]; }
			set
			{
				Loss[2] = value;
				this.NotifyPropertyChanged("Loss2");
			}
		}

		public int Loss3
		{
			get { return Loss[3]; }
			set
			{
				Loss[3] = value;
				this.NotifyPropertyChanged("Loss3");
			}
		}

		public int Loss4
		{
			get { return Loss[4]; }
			set
			{
				Loss[4] = value;
				this.NotifyPropertyChanged("Loss4");
			}
		}

		public int Loss5
		{
			get { return Loss[5]; }
			set
			{
				Loss[5] = value;
				this.NotifyPropertyChanged("Loss5");
			}
		}

		public int Loss6
		{
			get { return Loss[6]; }
			set
			{
				Loss[6] = value;
				this.NotifyPropertyChanged("Loss6");
			}
		}

		public int Loss7
		{
			get { return Loss[7]; }
			set
			{
				Loss[7] = value;
				this.NotifyPropertyChanged("Loss7");
			}
		}

		public int Loss8
		{
			get { return Loss[8]; }
			set
			{
				Loss[8] = value;
				this.NotifyPropertyChanged("Loss8");
			}
		}

		public int Loss9
		{
			get { return Loss[9]; }
			set
			{
				Loss[9] = value;
				this.NotifyPropertyChanged("Loss9");
			}
		}

		public int Loss10
		{
			get { return Loss[10]; }
			set
			{
				Loss[10] = value;
				this.NotifyPropertyChanged("Loss10");
			}
		}

		public int Loss11
		{
			get { return Loss[11]; }
			set
			{
				Loss[11] = value;
				this.NotifyPropertyChanged("Loss11");
			}
		}

		public int Loss12
		{
			get { return Loss[12]; }
			set
			{
				Loss[12] = value;
				this.NotifyPropertyChanged("Loss12");
			}
		}

		public int Loss13
		{
			get { return Loss[13]; }
			set
			{
				Loss[13] = value;
				this.NotifyPropertyChanged("Loss13");
			}
		}

		public int Loss14
		{
			get { return Loss[14]; }
			set
			{
				Loss[14] = value;
				this.NotifyPropertyChanged("Loss14");
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
