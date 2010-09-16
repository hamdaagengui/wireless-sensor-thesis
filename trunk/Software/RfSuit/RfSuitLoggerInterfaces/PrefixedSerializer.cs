using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Compression;
using ProtoBuf;

namespace RfSuitLoggerInterfaces
{
  public class PrefixedSerializer<T>
  {
    private readonly string _ext;
    /// <summary>
    /// Setup a prefix serializer for a given type, with a given extension.
    /// The serializer will accept two types of files, one that is plain (filename + "." + extension) and one that is compressed using deflate (filename + "." + extension + "z").
    /// </summary>
    /// <param name="extension">without leading dot eg. "mp3"</param>
    public PrefixedSerializer(string extension)
    {
      _ext = "." + extension;
    }

    public PrefixedReaderEnumerable<T> OpenRead(string path)
    {
      var extension = Path.GetExtension(path).ToLower();
      if (extension == _ext + "z")
      {
        return new PrefixedReaderEnumerable<T>(new DeflateStream(File.OpenRead(path), CompressionMode.Decompress));
      }
      if (extension == _ext)
      {
        return new PrefixedReaderEnumerable<T>(File.OpenRead(path));
      }
      throw new ArgumentException("Missing file extension!");
    }

    public PrefixedWriter<T> OpenWrite(string path)
    {
      if(Path.HasExtension(path))
      {
        if (Path.GetExtension(path) == _ext + "z")
          return OpenWrite(path, true);
      }
      return OpenWrite(path, false);
    }

    public PrefixedWriter<T> OpenWrite(string path, bool compressed)
    {
      var realExt = (compressed ? _ext + "z" : _ext);
      if (Path.HasExtension(path))
      {
        var ext = Path.GetExtension(path).ToLower();
        if (ext != realExt)
          path += realExt;
      }
      else
      {
        path += realExt;
      }

      Stream s = File.OpenWrite(path);
      return new PrefixedWriter<T>(compressed ? new DeflateStream(s, CompressionMode.Compress) : s);
    }
  }

  public class PrefixedWriter<T> : IDisposable
  {
    private readonly Stream _stream;

    public PrefixedWriter(Stream stream)
    {
      _stream = stream;
    }

    public void Write(T item)
    {
      Serializer.SerializeWithLengthPrefix(_stream, item, PrefixStyle.Base128);
    }

    public void Write(IEnumerable<T> items)
    {
      foreach (var item in items)
        Write(item);
    }

    public void Close()
    {
      _stream.Close();
    }

    #region IDisposable Members

    public void Dispose()
    {
      _stream.Close();
    }

    #endregion
  }

  public class PrefixedReaderEnumerable<T> : IEnumerable<T>, IEnumerator<T>
  {
    private T _current;
    private readonly Stream _stream;

    public PrefixedReaderEnumerable(Stream stream)
    {
      _stream = stream;
    }

    public void Close()
    {
      _stream.Close();
    }

    #region IEnumerator<Entry> Members

    public T Current
    {
      get { return _current; }
    }

    #endregion

    #region IDisposable Members

    public void Dispose()
    {
      _stream.Close();
    }

    #endregion

    #region IEnumerator Members

    object System.Collections.IEnumerator.Current
    {
      get { return _current; }
    }

    public bool MoveNext()
    {
      if (_stream.Position >= _stream.Length)
        return false;
      _current = Serializer.DeserializeWithLengthPrefix<T>(_stream, PrefixStyle.Base128);
      return true;
    }

    public void Reset()
    {
      _stream.Seek(0, SeekOrigin.Begin);
    }

    #endregion

    #region IEnumerable<T> Members

    public IEnumerator<T> GetEnumerator()
    {
      return this;
    }

    #endregion

    #region IEnumerable Members

    System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
    {
      return this;
    }

    #endregion
  }
}
