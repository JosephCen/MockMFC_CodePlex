using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;

namespace WordAssistantLib
{
    class SimpleHttpClient
    {
        private String m_UserAgent;

        private const String mC_CTypeTextHtml = "text/html";
        private const String mC_CTypeAudioMpeg = "audio/mpeg";

        public SimpleHttpClient()
        {
            m_UserAgent = "Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0)";
        }

        private HttpWebResponse GetHttpWebResponse(String requestUrlStr)
        {
            HttpWebRequest request = null;
            HttpWebResponse response = null;

            request = WebRequest.Create(requestUrlStr) as HttpWebRequest;
            if (null == request)
                throw new Exception("Fail to create HttpWebRequest.");

            request.UserAgent = m_UserAgent;

            response = request.GetResponse() as HttpWebResponse;
            if (null == response)
                throw new Exception("Fail to get HttpWebResponse from given Url.");
            if (HttpStatusCode.OK != response.StatusCode)
                throw new Exception(String.Format("Invalid return status code: {0}", 
                                                  response.StatusCode.ToString()));

            return response;
        }

        private String MapContentType(HttpWebResponse response)
        {
            String contentType = String.Empty;
            String retVal = String.Empty;

            contentType = response.ContentType.ToLower();

            if (contentType.IndexOf(mC_CTypeTextHtml) >= 0)
                retVal = mC_CTypeTextHtml;
            else if (contentType.IndexOf(mC_CTypeAudioMpeg) >= 0)
                retVal = mC_CTypeAudioMpeg;
            else
                throw new Exception("Unknown response content type");

            return retVal;
        }

        private Encoding MapTextEncoding(HttpWebResponse response)
        {
            String contentType = String.Empty;
            Encoding retVal = null;

            contentType = response.ContentType.ToLower();

            if (contentType.IndexOf("utf-8") >= 0)
                retVal = Encoding.UTF8;
            else
                throw new Exception("Unknown response characterset");

            return retVal;
        }

        public String GetHttpString(String requestUrlStr)
        {
            HttpWebResponse response = null;
            StreamReader contentReader = null;

            response = GetHttpWebResponse(requestUrlStr);
            if (mC_CTypeTextHtml != MapContentType(response))
                throw new Exception("Wrong ContentType, expect \"text/html\"");

            contentReader = new StreamReader(response.GetResponseStream(), MapTextEncoding(response));

            return contentReader.ReadToEnd();
        }

        private void WriteFileFromStream(Stream stream, String fileName)
        {
            using (FileStream fStream = new FileStream(fileName, FileMode.Create))
            {
                Int32 bufferLen = 0;
                Int32 bufferCap = 1024;
                Byte[] buffer = new Byte[bufferCap];

                do
                {
                    bufferLen = stream.Read(buffer, 0, bufferCap);
                    fStream.Write(buffer, 0, bufferLen);
                } while (bufferLen > 0);
                fStream.Flush();
            }
        }

        public String GetAudioFile(String requestUrlStr, String fileName)
        {
            HttpWebResponse response = null;
            String mp3FileExt = ".mp3";

            response = GetHttpWebResponse(requestUrlStr);
            if (mC_CTypeAudioMpeg != MapContentType(response))
                throw new Exception("Wrong ContentType, expect \"audio/mpeg\"");

            if (!fileName.EndsWith(mp3FileExt, StringComparison.OrdinalIgnoreCase))
                fileName = String.Concat(fileName, mp3FileExt);
            if (!Path.IsPathRooted(fileName))
                fileName = Path.GetFullPath(fileName);

            using (Stream stream = response.GetResponseStream())
            {
                StreamHelper.WriteStreamToFile(stream, fileName);
                stream.Close();
            }

            return fileName;
        }

        public String UserAgent
        {
            get { return m_UserAgent; }
            set { m_UserAgent = value; }
        }
    }
}
