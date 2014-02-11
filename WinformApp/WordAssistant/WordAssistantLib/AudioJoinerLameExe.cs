using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.IO;
using System.Text;

namespace WordAssistantLib
{
    public class AudioJoinerLameExe
    {
        private const String mC_LameFilePath = @"C:\Joseph_Data\ProjectData\MyProgram\WordAssistant\WordAssistantTest\lame3.99.3\lame.exe";
        private String m_TempDir;
        private String m_LameFilePath;

        public AudioJoinerLameExe()
        {
            m_TempDir = Path.Combine(Environment.CurrentDirectory, "AudioJoinerLameExe_Temp");
            m_LameFilePath = mC_LameFilePath;
        }

        private String CreateTempDir()
        {
            String tempDir = String.Empty;

            tempDir = m_TempDir;
            if (!Path.IsPathRooted(tempDir))
                tempDir = Path.GetFullPath(tempDir);
            //tempDir = Path.Combine(tempDir, String.Concat(AppDomain.CurrentDomain.FriendlyName, "_Tmp"));

            if (Directory.Exists(tempDir))
                Directory.Delete(tempDir, true);
            Directory.CreateDirectory(tempDir);

            while (!Directory.Exists(tempDir))
                System.Threading.Thread.Sleep(500);

            return tempDir;
        }

        private void RunLameExe(String workingDir, String exeArgs)
        {
            Process lameExe = null;
            ProcessStartInfo procSI = null;

            procSI = new ProcessStartInfo(m_LameFilePath);
            procSI.CreateNoWindow = true;
            procSI.UseShellExecute = false;
            procSI.WorkingDirectory = workingDir;
            procSI.Arguments = exeArgs;

            lameExe = Process.Start(procSI);
            lameExe.WaitForExit(10000);
            if (!lameExe.HasExited)
                throw new Exception("lame crashed");
            if (lameExe.ExitCode != 0)
                throw new Exception("lame error happened");
        }

        private String DecodeSingleFile(String mp3File, String tempDir, Boolean withWavHeader)
        {
            String oriMp3File = "OrigFile.mp3";
            String mp3FileConstFmt = "TempSingleWord.mp3"; // resample to 22.05 kHz, single channel
            String wavFileName = "TempSingleWord.wav";

            //// Copy the original mp3 file to tempDir
            //oriMp3File = Path.Combine(tempDir, oriMp3File);
            //File.Copy(mp3File, oriMp3File);

            RunLameExe(tempDir, String.Concat("--resample 22.05 -m m \"", mp3File, "\" \"", mp3FileConstFmt, "\""));
            if (!File.Exists(Path.Combine(tempDir, mp3FileConstFmt)))
                throw new Exception("Fail to resample mp3");
            else
                File.Delete(oriMp3File);

            if (withWavHeader)
                RunLameExe(tempDir, String.Concat("--decode \"", mp3FileConstFmt, "\" \"", wavFileName, "\""));
            else
                RunLameExe(tempDir, String.Concat("--decode -t \"", mp3FileConstFmt, "\" \"", wavFileName, "\""));
            if (!File.Exists(Path.Combine(tempDir, wavFileName)))
                throw new Exception("Cannot found output wav");
            else
                File.Delete(Path.Combine(tempDir, mp3FileConstFmt));

            return Path.Combine(tempDir, wavFileName);
        }

        private String EncodeWavFile(String wavFile, String tempDir, String mp3File)
        {
            RunLameExe(tempDir, String.Concat("--abr 32 \"", wavFile, "\" \"", mp3File, "\""));
            if (!File.Exists(mp3File))
                throw new Exception("Fail to encode mp3");

            return mp3File;
        }

        public String JoinAudio(IEnumerable<String> fileArr, String mp3File = null)
        {
            String wavFileName = "TempWordList.wav";
            String mp3FileExt = ".mp3";
            Int32 wavFileLen = 0;
            String tempDir = CreateTempDir();
            Boolean isFirstFile = true;
            Int32 blankSampleCnt = (22 * 1024) / 2;

            wavFileName = Path.Combine(tempDir, wavFileName);

            using (FileStream joinedFStream = new FileStream(wavFileName, FileMode.Create))
            {
                String singleWordWav = String.Empty;
                
                foreach (String eachFile in fileArr)
                {
                    singleWordWav = DecodeSingleFile(eachFile, tempDir, isFirstFile);
                    isFirstFile = false;
                    wavFileLen += StreamHelper.WriteFileToStream(singleWordWav, joinedFStream);
                    File.Delete(singleWordWav);

                    // insert several second blank
                    wavFileLen += StreamHelper.WriteValueToStream((Int16)0, blankSampleCnt, joinedFStream);
                }

                joinedFStream.Position = 4;
                StreamHelper.WriteValueToStream((wavFileLen - 8), 1, joinedFStream);
                joinedFStream.Position = 40;
                StreamHelper.WriteValueToStream((wavFileLen - 44), 1, joinedFStream);

                joinedFStream.Flush();
                joinedFStream.Close();
            }

            if (String.IsNullOrEmpty(mp3File))
                mp3File = wavFileName;
            if (!mp3File.EndsWith(mp3FileExt, StringComparison.OrdinalIgnoreCase))
                mp3File = String.Concat(mp3File, mp3FileExt);
            if (!Path.IsPathRooted(mp3File))
                mp3File = Path.GetFullPath(mp3File);

            // Encode mp3 file by lame.exe
            EncodeWavFile(wavFileName, tempDir, mp3File);

            return wavFileName;
        }

        public String LameFilePath
        {
            get { return m_LameFilePath; }
            set { m_LameFilePath = value; }
        }

        public String TempDir
        {
            get { return m_TempDir; }
            set { m_TempDir = value; }
        }
    }
}
