using System;
using dk.iha;

namespace RfSuitLoggerInterfaces
{
  public class EntrySerializer : PrefixedSerializer<Entry>
  {
    private static readonly Lazy<EntrySerializer> _instance = new Lazy<EntrySerializer>();
    private EntrySerializer() : base("rflog") {}
    public static EntrySerializer Instance { get { return _instance.Value; } }
  }
}
