using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;

namespace WordAssistantLib
{
    public class HttpClientICiba
    {
        private SimpleHttpClient m_HttpClient;
        private const String mC_ICibaUrl = "http://www.iciba.com/";
        private const String mC_ICibaUrlOpen = "http://open.iciba.com/huaci/dict.php?word=";
        private Regex m_audioResPat;
        private Regex m_audioResPatOpen;

        public HttpClientICiba()
        {
            m_HttpClient = new SimpleHttpClient();
            m_audioResPat = new Regex("<a[^<>]+?onclick=\"asplay\\('([^()']+?)'\\)", RegexOptions.Compiled);
            m_audioResPatOpen = new Regex("<a[^<>]+?onclick=\\\\\"asplay_hanci\\(\\\\'([^()']+?)\\\\'\\)", RegexOptions.Compiled);
        }

        private String FindAudioUrl(String pageSrc)
        {
            String clsUnfound = "class=\"question unfound_tips\"";
            String clsProns = "class=\"prons\"";
            String audioUrl = String.Empty;

            if (pageSrc.IndexOf(clsUnfound, StringComparison.Ordinal) < 0)
            {
                Int32 startAt = pageSrc.IndexOf(clsProns, StringComparison.Ordinal);

                if (startAt >= 0)
                {
                    String subHtml = pageSrc.Substring(startAt + clsProns.Length, 2000);
                    Match regexMat = null;

                    regexMat = m_audioResPat.Match(subHtml);
                    if (regexMat.Success)
                        audioUrl = regexMat.Groups[1].ToString();
                }
            }

            return audioUrl;
        }

        private String FindAudioUrlOpen(String pageSrc)
        {
            String clsProns = "class=\\\"icIBahyI-prons\\\"";
            String audioUrl = String.Empty;
            Int32 startAt = 0;

            startAt = pageSrc.IndexOf(clsProns, StringComparison.Ordinal);
            if (startAt >= 0)
            {
                String subHtml = pageSrc.Substring(startAt);
                Match regexMat = null;

                regexMat = m_audioResPatOpen.Match(subHtml);
                while (regexMat.Success)
                {
                    audioUrl = regexMat.Groups[1].ToString();
                    if (audioUrl.IndexOf("res-tts", StringComparison.OrdinalIgnoreCase) >= 0)
                        regexMat = regexMat.NextMatch();
                    else
                        break;
                }
            }

            return audioUrl;
        }

        public String DownloadAudio(String wordStr, String audioFileName = null)
        {
            String wordSrc = String.Empty;
            String audioUrl = String.Empty;
            String fileName = String.Empty;

            if (String.IsNullOrEmpty(audioFileName))
                audioFileName = wordStr;

            //wordSrc = m_HttpClient.GetHttpString(String.Format("http://www.iciba.com/{0}", wordStr));
            //audioUrl = FindAudioUrl(wordSrc);
            wordSrc = m_HttpClient.GetHttpString(String.Concat(mC_ICibaUrlOpen, wordStr));
            audioUrl = FindAudioUrlOpen(wordSrc);
            if (!String.IsNullOrEmpty(audioUrl))
                fileName = m_HttpClient.GetAudioFile(audioUrl, audioFileName);

            return fileName;
        }
    }
}
