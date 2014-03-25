using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

using LameWrapper;

namespace TestLameWrapper
{
    class Program
    {
        static void Main(string[] args)
        {
            FileStream wavRStream = null;
            FileStream mp3WStream = null;
            LameEncodeStream lameStream = null;
            String testDir = @"D:\Joseph_Data\ProjectData\MyProgram\Cpp\LameWrapper\TestFiles";

            mp3WStream = new FileStream(Path.Combine(testDir, "Test.mp3"), FileMode.Create, FileAccess.Write);
            lameStream = new LameEncodeStream(mp3WStream);

            wavRStream = new FileStream(Path.Combine(testDir, "Test.wav"), FileMode.Open, FileAccess.Read);

            Byte[] buffer = new Byte[4096*4];

            lameStream.InitialLame();
            for (Int32 readByte = wavRStream.Read(buffer, 0, buffer.Length);
                readByte > 0;
                readByte = wavRStream.Read(buffer, 0, buffer.Length))
            {
                if (readByte % 2 == 0)
                    lameStream.Write(buffer, 0, readByte);
                else
                    lameStream.Write(buffer, 0, readByte-1);
            }

            wavRStream.Close();
            lameStream.Close();
            mp3WStream.Close();
        }
    }
}
