using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace CsvReaderWriter
{
    public class CsvWriter : IDisposable
    {
        #region Fields

        private readonly String m_CsvFileName;
        private Encoding m_CsvEncoding;
        private StreamWriter m_CsvWriter;
        private readonly Char m_CsvSplitCh;
        private readonly Char m_CsvLimitCh;
        private readonly String m_CsvLimitStr;
        private readonly String m_CsvDupLimitStr;
        private readonly String m_NewLineReplaceStr;

        #endregion

        #region Constructor

        public CsvWriter(String csvFileName)
            : this(csvFileName, Encoding.Default, ',', '"', " ")
        { }

        public CsvWriter(String csvFileName, Encoding csvEncoding)
            : this(csvFileName, csvEncoding, ',', '"', " ")
        { }

        public CsvWriter(String csvFileName, Encoding csvEncoding, Char csvSplitCh, Char csvLimitCh, String newLineReplaceStr)
        {
            m_CsvFileName = csvFileName;
            m_CsvEncoding = csvEncoding;
            m_CsvSplitCh = csvSplitCh;
            m_CsvLimitCh = csvLimitCh;
            m_CsvLimitStr = csvLimitCh.ToString();
            m_CsvDupLimitStr = String.Concat(m_CsvLimitStr, m_CsvLimitStr);
            m_NewLineReplaceStr = newLineReplaceStr;
            m_CsvWriter = null;
        }

        #endregion

        #region Methods

        public void CreateFile(Boolean bOverwritten)
        {
            if (null == m_CsvWriter)
            {
                m_CsvWriter = new StreamWriter(File.Open(m_CsvFileName, (bOverwritten ? FileMode.Create : FileMode.CreateNew), FileAccess.Write),
                                               m_CsvEncoding);
            }
            else
            {
                throw new InvalidOperationException("File has been created");
            }
        }

        public void WriteLine(String[] strArr)
        {
            if (null == m_CsvWriter)
                CreateFile(false);

            for (Int32 i = 0; i < strArr.Length; ++i)
            {
                String curCell = strArr[i];

                if (curCell.IndexOf(m_CsvSplitCh) >= 0 || curCell.IndexOf(m_CsvLimitCh) >= 0 || curCell.IndexOf('\n') >= 0)
                {
                    curCell = curCell.Replace(m_CsvLimitStr, m_CsvDupLimitStr);
                    curCell = curCell.Replace(Environment.NewLine, m_NewLineReplaceStr);
                    curCell = curCell.Replace("\n", m_NewLineReplaceStr);
                    curCell = String.Concat(m_CsvLimitStr, curCell, m_CsvLimitStr);
                }

                m_CsvWriter.Write(curCell);
                if (i < (strArr.Length - 1))
                    m_CsvWriter.Write(m_CsvSplitCh);
            }
            m_CsvWriter.WriteLine();
        }

        public void Close()
        {
            m_CsvWriter.Close();
        }

        public void DelCurrentCsv()
        {
            m_CsvWriter.Close();
            if (File.Exists(m_CsvFileName))
                File.Delete(m_CsvFileName);
        }

        public void Dispose()
        {
            if (null != m_CsvWriter)
            {
                m_CsvWriter.Close();
                m_CsvWriter = null;
            }
        }

        #endregion
    }
}
