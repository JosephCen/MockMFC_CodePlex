using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace CsvReaderWriter
{
    public class CsvReader : IDisposable
    {
        #region Fields

        private readonly String m_CsvFileName;
        private Encoding m_CsvEncoding;
        private StreamReader m_CsvReader;
        private readonly Char m_CsvSplitCh;
        private readonly Char m_CsvLimitCh;
        private readonly String m_NewLineReplaceStr;

        #endregion

        #region Constructor

        public CsvReader(String csvFileName)
            : this(csvFileName, Encoding.Default, ',', '"', " ")
        { }

        public CsvReader(String csvFileName, Encoding csvEncoding)
            : this(csvFileName, csvEncoding, ',', '"', " ")
        { }

        public CsvReader(String csvFileName, Encoding csvEncoding, Char csvSplitCh, Char csvLimitCh, String newLineReplaceStr)
        {
            m_CsvFileName = csvFileName;
            m_CsvEncoding = csvEncoding;
            m_CsvSplitCh = csvSplitCh;
            m_CsvLimitCh = csvLimitCh;
            m_NewLineReplaceStr = newLineReplaceStr;
            m_CsvReader = null;
        }

        #endregion

        #region Methods

        public void OpenFile()
        {
            if (null == m_CsvReader)
            {
                m_CsvReader = new StreamReader(File.Open(m_CsvFileName, FileMode.Open, FileAccess.Read),
                                               m_CsvEncoding);
            }
            else
            {
                throw new InvalidOperationException("File has been opened.");
            }
        }

        //public String[] ReadOneLine()
        //{
        //    if (null == m_CsvReader)
        //        OpenFile();

        //    String lineStr = m_CsvReader.ReadLine();
        //    Int32 firstIdx = 0;

        //    if (!String.IsNullOrEmpty(lineStr))
        //    {
        //        List<String> strList = new List<String>();

        //        while (firstIdx < lineStr.Length)
        //        {
        //            if ('"' == lineStr[firstIdx])
        //            {
        //                StringBuilder strUnitStrB = new StringBuilder();
        //                Char ch1 = lineStr[++firstIdx];
        //                Char ch2 = (++firstIdx < lineStr.Length ? lineStr[firstIdx] : '\0');

        //                while ('"' != ch1 || '"' == ch2)
        //                {
        //                    strUnitStrB.Append(ch1);
        //                    if ('"' == ch1)
        //                    {
        //                        ch1 = lineStr[++firstIdx];
        //                        ch2 = (++firstIdx < lineStr.Length ? lineStr[firstIdx] : '\0');
        //                    }
        //                    else
        //                    {
        //                        ch1 = ch2;
        //                        ch2 = (++firstIdx < lineStr.Length ? lineStr[firstIdx] : '\0');
        //                    }
        //                }
        //                strList.Add(strUnitStrB.ToString());
        //            }
        //            else
        //            {
        //                Int32 nextIdx = lineStr.IndexOf(',', firstIdx);

        //                nextIdx = (-1 != nextIdx ? nextIdx : lineStr.Length);
        //                strList.Add(lineStr.Substring(firstIdx, nextIdx - firstIdx));
        //                firstIdx = nextIdx;
        //            }
        //            ++firstIdx;
        //        }

        //        return (strList.Count > 0 ? strList.ToArray() : null);
        //    }

        //    return null;
        //}

        public String[] ReadOneLine()
        {
            if (null == m_CsvReader)
                OpenFile();

            String lineStr = m_CsvReader.ReadLine();

            if (null != lineStr)
            {
                Int32 idx = 0;
                List<String> strList = new List<String>();

                while (idx < lineStr.Length)
                {
                    if (m_CsvLimitCh == lineStr[idx])
                    {
                        Char curCh = '\0';
                        Int32 subIdx = idx + 1;
                        StringBuilder subStrB = new StringBuilder();

                        while (true)
                        {
                            if (subIdx == lineStr.Length)
                            {
                                String lineLeftStr = m_CsvReader.ReadLine();

                                if (null == lineLeftStr)
                                    throw new InvalidDataException("Invalid CSV format.");

                                lineStr = String.Concat(lineStr, m_NewLineReplaceStr, lineLeftStr);
                            }

                            curCh = lineStr[subIdx];

                            if (m_CsvLimitCh == curCh)
                            {
                                ++subIdx;
                                if (subIdx < lineStr.Length && m_CsvLimitCh == lineStr[subIdx]) // read [""]
                                    curCh = lineStr[subIdx];
                                else
                                    break;
                            }
                            subStrB.Append(curCh);
                            ++subIdx;
                        }
                        strList.Add(subStrB.ToString());
                        idx = subIdx;
                    }
                    else
                    {
                        Int32 nextSplitCh = lineStr.IndexOf(m_CsvSplitCh, idx);
                        String subStr = String.Empty;

                        nextSplitCh = (-1 == nextSplitCh ? lineStr.Length : nextSplitCh);
                        subStr = lineStr.Substring(idx, nextSplitCh - idx);
                        if (subStr.IndexOf(m_CsvLimitCh) >= 0)
                            throw new InvalidDataException("Invalid CSV format.");
                        strList.Add(subStr);
                        idx = nextSplitCh;
                    }

                    if (idx < lineStr.Length && m_CsvSplitCh != lineStr[idx])
                        throw new InvalidDataException("Invalid CSV format.");
                    else
                        ++idx;
                }
                // Add last empty str. i.e. [abc,"",]
                if (idx > 0 && idx == lineStr.Length && m_CsvSplitCh == lineStr[idx - 1])
                    strList.Add(String.Empty);

                return strList.ToArray();
            }
            else
            {
                // null == lineStr
                return null;
            }
        }

        public void Dispose()
        {
            if (null != m_CsvReader)
            {
                m_CsvReader.Close();
                m_CsvReader = null;
            }
        }

        #endregion

        #region Properties

        public Boolean EndOfReader
        {
            get { return m_CsvReader.EndOfStream; }
        }

        #endregion
    }
}
