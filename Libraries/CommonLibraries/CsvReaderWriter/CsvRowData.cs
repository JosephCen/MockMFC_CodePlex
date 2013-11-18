using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CsvReaderWriter
{
    public class CsvRowData
    {
        #region Fields

        private CsvRowReader m_ParentReader;
        private String[] m_RowArr;

        #endregion

        #region Constructor

        internal CsvRowData(CsvRowReader parentReader, String[] rowArr)
        {
            m_ParentReader = parentReader;
            m_RowArr = rowArr;
        }

        #endregion

        #region Methods

        #endregion

        #region Properties

        public String this[Int32 index]
        {
            get { return m_RowArr[index]; }
            set { m_RowArr[index] = value; }
        }

        public String this[String headerStr, Int32 occur]
        {
            get { return m_RowArr[m_ParentReader.GetHeaderIndex(headerStr, occur)]; }
            set { m_RowArr[m_ParentReader.GetHeaderIndex(headerStr, occur)] = value;}
        }

        public String this[String headerStr]
        {
            get { return m_RowArr[m_ParentReader.GetHeaderIndex(headerStr, 0)]; }
            set { m_RowArr[m_ParentReader.GetHeaderIndex(headerStr, 0)] = value; }
        }

        public String[] Headers
        {
            get { return m_ParentReader.Headers; }
        }

        #endregion
    }
}
