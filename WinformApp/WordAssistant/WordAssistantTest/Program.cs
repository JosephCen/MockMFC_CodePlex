using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using System.Net;
using System.IO;

using WordAssistantLib;

namespace WordAssistantTest
{
    class Program
    {
        static void Test()
        {
            String wordStr = String.Empty;
            HttpWebRequest request = null;
            HttpWebResponse response = null;
            StreamReader reader = null;

            wordStr = "apparel";

            request = WebRequest.Create(String.Format("http://www.iciba.com/{0}", wordStr)) as HttpWebRequest;
            if (null == request)
                return;

            request.UserAgent = "Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0)";

            response = request.GetResponse() as HttpWebResponse;
            Console.WriteLine("StatusCode: {0}", response.StatusCode.ToString());
            Console.WriteLine("ContentType: {0}", response.ContentType);
            reader = new StreamReader(response.GetResponseStream(), Encoding.UTF8);

            Regex soundResPat = new Regex("http://res\\.iciba\\.com[a-z0-9/]*\\.mp3"); //http://res.iciba.com/resource/amp3/1/0/2f/cb/2fcbff5f9416f04eb81b1b8faa01eeee.mp3'
            Match mat = soundResPat.Match(reader.ReadToEnd());

            if (mat.Success)
            {
                String resUri = mat.Groups[0].ToString();
                Stream responseStream = null;
                FileStream mp3FStream = null;
                Int32 byteVal = 0;

                request = WebRequest.Create(resUri) as HttpWebRequest;
                request.UserAgent = "Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0)";
                response = request.GetResponse() as HttpWebResponse;
                Console.WriteLine("ContentType: {0}", response.ContentType);
                mp3FStream = new FileStream(String.Concat(wordStr, ".mp3"), FileMode.Create);
                responseStream = response.GetResponseStream();

                while ((byteVal = responseStream.ReadByte()) >= 0)
                    mp3FStream.WriteByte(Convert.ToByte(byteVal));

                mp3FStream.Close();
                mp3FStream.Dispose();
            }
        }

        static void TestICibaClient()
        {
            HttpClientICiba iCibaClient = new HttpClientICiba();
            String[] wordArr = new String[] { "aaabbbcccddd", "capacity", "buffer", "client" };

            foreach (String eachWord in wordArr)
                Console.WriteLine(iCibaClient.DownloadAudio(eachWord));
        }

        static void DownloadWordList(String listFileName, String dirName)
        {
            if (!File.Exists(listFileName))
                throw new Exception("Cannot find given wordlist");

            if (!Path.IsPathRooted(dirName))
                dirName = Path.GetFullPath(dirName);

            if (Directory.Exists(dirName))
                Directory.Delete(dirName, true);
            Directory.CreateDirectory(dirName);

            using (StreamReader listReader = new StreamReader(listFileName))
            {
                Int32 wordIdx = 0;
                String wordStr = String.Empty;
                String fileName = String.Empty;
                HttpClientICiba iCibaClient = new HttpClientICiba();

                do
                {
                    wordStr = listReader.ReadLine();
                    if (null == wordStr)
                        wordStr = String.Empty;
                    wordStr = wordStr.Trim();
                    if (wordStr.Length > 0 && wordStr.IndexOf(' ') < 0)
                    {
                        ++wordIdx;
                        fileName = String.Format("{0}_{1}.mp3", wordIdx.ToString("0000"), wordStr);
                        fileName = Path.Combine(dirName, fileName);
                        Console.WriteLine(iCibaClient.DownloadAudio(wordStr, fileName));
                    }
                } while (wordStr.Length > 0);

                listReader.Close();
            }
        }

        static void JoinAudioFiles(String audioFolder)
        {
            List<String> fileList = new List<String>();
            
            AudioJoinerLameExe audioJoiner = new AudioJoinerLameExe();

            foreach (String eachFile in Directory.GetFiles(audioFolder, "*.mp3"))
                fileList.Add(eachFile);

            audioJoiner.JoinAudio(fileList.ToArray());
        }

        static void Main(string[] args)
        {
            String wordList = @"C:\Joseph_Data\ProjectData\MyProgram\WordAssistant\WordAssistantTest\bin\Debug\TmpFolder\MyWordList.txt";
            String audioDir = @"C:\Joseph_Data\ProjectData\MyProgram\WordAssistant\WordAssistantTest\bin\Debug\TmpFolder\TempAudio";

            //DownloadWordList(wordList, audioDir);
            JoinAudioFiles(audioDir);

            Console.ReadLine();
        }
    }
}
