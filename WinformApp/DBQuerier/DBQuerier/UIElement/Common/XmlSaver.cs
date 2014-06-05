using System;
using System.IO;
using System.Text;
using System.Windows.Forms;

using DBQuerier.UIElement;
using DBQuerier.Common;
using DBQuerier.DataObj;

namespace DBQuerier.UIElement.Common
{
    class XmlSaver : IMenuItemInvoker
    {
        private MenuItemXmlSaverDef m_XmlSaverDef;

        public XmlSaver(MenuItemXmlSaverDef xmlSaverDef)
        {
            m_XmlSaverDef = xmlSaverDef;
        }

        #region IMenuItemInvoker Members

        public Boolean CanInvoke(Object[] invokeArgs)
        {
            Boolean bCanInvoke = false;

            if (invokeArgs.Length > 0)
            {
                String xmlStr = invokeArgs[0] as String;

                if (xmlStr != null)
                    bCanInvoke = XmlSaveOpener.IsXmlString(xmlStr);
            }

            return bCanInvoke;
        }

        public void Invoke(Object[] invokeArgs)
        {
            if (invokeArgs.Length > 0)
            {
                String xmlStr = invokeArgs[0] as String;

                if (xmlStr != null)
                {
                    StringBuilder xmlFileNameStrB = new StringBuilder();
                    String xmlFileName = String.Empty;
                    Boolean bXmlFileName = false;
                    Boolean bSaveDirectory = false;
                    Boolean bDupFileName = true;
                    String savedXmlFileName = String.Empty; // The file name of saved xml file.

                    if (!String.IsNullOrEmpty(m_XmlSaverDef.SaveDirectory))
                        bSaveDirectory = Directory.Exists(m_XmlSaverDef.SaveDirectory);

                    for (Int32 i = 1; i < invokeArgs.Length; ++i)
                        xmlFileNameStrB.Append(invokeArgs[i].ToString()).Append('_');
                    if (xmlFileNameStrB.Length > 0)
                    {
                        xmlFileNameStrB.Remove(xmlFileNameStrB.Length - 1, 1);
                        xmlFileNameStrB.Append(".xml");
                        xmlFileName = xmlFileNameStrB.ToString();

                        if (xmlFileName.IndexOfAny(XmlSaveOpener.GetNonFileNameChars()) < 0)
                            bXmlFileName = true;

                        if (bSaveDirectory && bXmlFileName)
                            bDupFileName = File.Exists(Path.Combine(m_XmlSaverDef.SaveDirectory, xmlFileName));
                    }

                    if (!bSaveDirectory || !bXmlFileName || bDupFileName)
                    {
                        SaveFileDialog saveFileDlg = new SaveFileDialog();

                        saveFileDlg.DefaultExt = "xml";
                        saveFileDlg.Filter = "Xml files (*.xml)|*.xml|All files (*.*)|*.*";
                        if (bSaveDirectory)
                            saveFileDlg.InitialDirectory = m_XmlSaverDef.SaveDirectory;
                        if (bXmlFileName)
                            saveFileDlg.FileName = xmlFileName;

                        if (saveFileDlg.ShowDialog() == DialogResult.OK)
                            savedXmlFileName = XmlSaveOpener.SaveXmlToFile(xmlStr, saveFileDlg.FileName);
                    }
                    else
                    {
                        savedXmlFileName = XmlSaveOpener.SaveXmlToFile(xmlStr, Path.Combine(m_XmlSaverDef.SaveDirectory, xmlFileName));
                    }

                    if (!String.IsNullOrEmpty(savedXmlFileName))
                    {
                        DialogResult dlgRet = DialogResult.None;

                        dlgRet = MessageBox.Show("Do you want to open located folder?", "Information", MessageBoxButtons.YesNo);
                        if (dlgRet == DialogResult.Yes)
                            this.OpenDestDirAndSelectFile(savedXmlFileName);
                    }
                }
            }
        }

        #endregion

        private void OpenDestDirAndSelectFile(String fileName)
        {
            // Reference 
            //    http://bbs.csdn.net/topics/360258100
            //    http://zhidao.baidu.com/link?url=mHE40oHLXcd2WDE7P95X8Pct74YJluHm8IwBb8OIdGebU-ZTOZqS2VCRQ8sgKIgNxB4JSMG8mZaPb9yUMs6H_q

            String parameterStr = String.Format("/select,{0}", fileName);

            NativeApi.ShellExecute(IntPtr.Zero, "open", "explorer.exe", parameterStr, null, ShowCommands.SW_SHOWNORMAL);
        }
    }
}
