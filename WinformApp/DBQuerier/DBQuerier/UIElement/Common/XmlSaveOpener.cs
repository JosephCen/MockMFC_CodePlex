using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using System.Xml;

using DBQuerier.UIElement;


namespace DBQuerier.UIElement.Common
{
    static class XmlSaveOpener
    {
        private static String mS_WebExployerApp;
        private static Char[] mS_NonFileNameChs;

        public static String GetDefaultIExplorer()
        {
            if (String.IsNullOrEmpty(mS_WebExployerApp))
            {
                // Default web exployer app *.exe path
                String defaultWebExployerApp = @"http\shell\open\command";
                // Default web exployer name
                // @"http\shell\open\ddeexec\Application"
                Microsoft.Win32.RegistryKey webExployerAppRegKey = null;
                String regKeyVal = String.Empty;

                webExployerAppRegKey = Microsoft.Win32.Registry.ClassesRoot.OpenSubKey(defaultWebExployerApp);
                regKeyVal = webExployerAppRegKey.GetValue("") as String;
                if (!String.IsNullOrEmpty(regKeyVal))
                {
                    // Fetch *.exe path from string ("C:\Program Files (x86)\Internet Explorer\iexplore.exe" -nohome).
                    System.Text.RegularExpressions.Regex appExePat = null;
                    System.Text.RegularExpressions.Match m = null;

                    appExePat = new System.Text.RegularExpressions.Regex("([A-Z]:(\\\\[^/:*?\"<>|\\\\]+)+)"); // ([A-Z]:(\\\\[\\w. ()]+)+)
                    m = appExePat.Match(regKeyVal);
                    if (m.Success && m.Groups.Count > 1)
                        mS_WebExployerApp = m.Groups[1].Value;
                }
            }

            return mS_WebExployerApp;
        }

        public static Boolean IsXmlString(String xmlStr)
        {
            return Regex.IsMatch(xmlStr, "<[^<>]+>", RegexOptions.Multiline);
        }

        public static String SaveXmlToFile(String xmlStr, String fileName)
        {
            String xmlFileName = String.Empty;

            if (!String.IsNullOrEmpty(xmlStr))
            {
                XmlDocument xmlDoc = new XmlDocument();

                xmlDoc.LoadXml(xmlStr);
                if (String.IsNullOrEmpty(fileName))
                    xmlFileName = String.Concat(Path.GetTempFileName(), ".xml");
                else
                    xmlFileName = fileName;
                xmlDoc.Save(xmlFileName);
            }
            else
            {
                throw new InvalidDataException("XmlString property must be set!");
            }

            return xmlFileName;
        }

        public static void OpenXmlByExployer(String xmlStr, String viewerApp)
        {
            ProcessStartInfo webExployerProcInfo = null;
            Process webExployerProc = null;
            String tempXmlFilePath = String.Empty;

            tempXmlFilePath = SaveXmlToFile(xmlStr, null);
            webExployerProcInfo = new ProcessStartInfo();
            if (String.IsNullOrEmpty(viewerApp))
                webExployerProcInfo.FileName = GetDefaultIExplorer();
            else
                webExployerProcInfo.FileName = viewerApp;
            webExployerProcInfo.Arguments = tempXmlFilePath;

            webExployerProc = Process.Start(webExployerProcInfo);
            System.Threading.Thread.Sleep(1000);
            File.Delete(tempXmlFilePath);
        }

        public static Char[] GetNonFileNameChars()
        {
            if (mS_NonFileNameChs == null)
                mS_NonFileNameChs = new Char[] {'/', ':', '*', '?', '"', '<', '>', '|', '\''};

            return mS_NonFileNameChs;
        }
    }
}
