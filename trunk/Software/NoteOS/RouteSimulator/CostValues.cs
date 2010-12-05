using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

namespace RouteSimulator
{
	class CostValues : INotifyPropertyChanged
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

		public int[] Costs = new int[15];

		public int Cost0
		{
			get { return Costs[0]; }
			set
			{
				Costs[0] = value;
				this.NotifyPropertyChanged("Cost0");
			}
		}

		public int Cost1
		{
			get { return Costs[1]; }
			set
			{
				Costs[1] = value;
				this.NotifyPropertyChanged("Cost1");
			}
		}

		public int Cost2
		{
			get { return Costs[2]; }
			set
			{
				Costs[2] = value;
				this.NotifyPropertyChanged("Cost2");
			}
		}

		public int Cost3
		{
			get { return Costs[3]; }
			set
			{
				Costs[3] = value;
				this.NotifyPropertyChanged("Cost3");
			}
		}

		public int Cost4
		{
			get { return Costs[4]; }
			set
			{
				Costs[4] = value;
				this.NotifyPropertyChanged("Cost4");
			}
		}

		public int Cost5
		{
			get { return Costs[5]; }
			set
			{
				Costs[5] = value;
				this.NotifyPropertyChanged("Cost5");
			}
		}

		public int Cost6
		{
			get { return Costs[6]; }
			set
			{
				Costs[6] = value;
				this.NotifyPropertyChanged("Cost6");
			}
		}

		public int Cost7
		{
			get { return Costs[7]; }
			set
			{
				Costs[7] = value;
				this.NotifyPropertyChanged("Cost7");
			}
		}

		public int Cost8
		{
			get { return Costs[8]; }
			set
			{
				Costs[8] = value;
				this.NotifyPropertyChanged("Cost8");
			}
		}

		public int Cost9
		{
			get { return Costs[9]; }
			set
			{
				Costs[9] = value;
				this.NotifyPropertyChanged("Cost9");
			}
		}

		public int Cost10
		{
			get { return Costs[10]; }
			set
			{
				Costs[10] = value;
				this.NotifyPropertyChanged("Cost10");
			}
		}

		public int Cost11
		{
			get { return Costs[11]; }
			set
			{
				Costs[11] = value;
				this.NotifyPropertyChanged("Cost11");
			}
		}

		public int Cost12
		{
			get { return Costs[12]; }
			set
			{
				Costs[12] = value;
				this.NotifyPropertyChanged("Cost12");
			}
		}

		public int Cost13
		{
			get { return Costs[13]; }
			set
			{
				Costs[13] = value;
				this.NotifyPropertyChanged("Cost13");
			}
		}

		public int Cost14
		{
			get { return Costs[14]; }
			set
			{
				Costs[14] = value;
				this.NotifyPropertyChanged("Cost14");
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
