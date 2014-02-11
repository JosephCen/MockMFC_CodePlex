using System;
using System.Collections.Generic;
using System.IO;

namespace WordAssistantLib
{
    static class StreamHelper
    {
        public static Int32 WriteStreamToStream(Stream srcStream, Stream destStream)
        {
            Int32 bufferLen = 0;
            Int32 bufferCap = 1024;
            Byte[] buffer = new Byte[bufferCap];
            Int32 writenByteCnt = 0;

            do
            {
                bufferLen = srcStream.Read(buffer, 0, buffer.Length);
                writenByteCnt += bufferLen;
                destStream.Write(buffer, 0, bufferLen);
            } while (bufferLen > 0);
            destStream.Flush();

            return writenByteCnt;
        }

        public static Int32 WriteValueToStream(Int16 value, Int32 count, Stream destStream)
        {
            Byte[] dataBuf = null;

            dataBuf = BitConverter.GetBytes(value);
            for (Int32 i = 0; i < count; ++i)
                destStream.Write(dataBuf, 0, dataBuf.Length);

            return (dataBuf.Length * count);
        }

        public static Int32 WriteValueToStream(Int32 value, Int32 count, Stream destStream)
        {
            Byte[] dataBuf = null;

            dataBuf = BitConverter.GetBytes(value);
            for (Int32 i = 0; i < count; ++i)
                destStream.Write(dataBuf, 0, dataBuf.Length);

            return (dataBuf.Length * count);
        }

        public static Int32 WriteFileToStream(String fileName, Stream destStream)
        {
            Int32 writenByteCnt = 0;

            using (FileStream srcFStream = new FileStream(fileName, FileMode.Open))
            {
                writenByteCnt = WriteStreamToStream(srcFStream, destStream);
                srcFStream.Close();
            }

            return writenByteCnt;
        }

        public static Int32 WriteStreamToFile(Stream stream, String fileName)
        {
            Int32 writenByteCnt = 0;

            using (FileStream fStream = new FileStream(fileName, FileMode.Create))
            {
                writenByteCnt = WriteStreamToStream(stream, fStream);
                fStream.Close();
            }

            return writenByteCnt;
        }
    }
}
