using System;
using System.Collections.Generic;
using System.Text;

namespace CsvReaderWriter
{
    public class CsvRowReader : IEnumerable<CsvRowData>, IDisposable
    {
        #region Fields

        private CsvReader m_Reader;
        private Boolean m_WithHeader;
        private String[] m_HeaderArr;
        private IDictionary<String, Int32[]> m_HeaderIdxDic;

        #endregion

        #region Constructor

        public CsvRowReader(CsvReader csvReader, Boolean withHeader)
        {
            if (null == csvReader)
                throw new ArgumentNullException("csvReader");

            m_Reader = csvReader;
            m_WithHeader = withHeader;
            m_HeaderArr = null;
            m_HeaderIdxDic = null;
        }

        #endregion

        #region Methods

        private void ReadHeaderArr_Impl()
        {
            if (null == m_HeaderArr)
            {
                m_HeaderArr = m_Reader.ReadOneLine();
                if (null == m_HeaderArr || m_HeaderArr.Length == 0)
                    throw new InvalidOperationException("Fail to read header row!");

                m_HeaderIdxDic = new SortedList<String, Int32[]>(StringComparer.Ordinal);
                for (Int32 i = 0; i < m_HeaderArr.Length; ++i)
                {
                    Int32[] indexArr = null;

                    if (!m_HeaderIdxDic.TryGetValue(m_HeaderArr[i], out indexArr))
                    {
                        m_HeaderIdxDic.Add(m_HeaderArr[i], new Int32[] { i });
                    }
                    else
                    {
                        Array.Resize(ref indexArr, indexArr.Length + 1);
                        indexArr[indexArr.Length - 1] = i;
                    }
                }
            }
        }

        private void ReadHeaderArr()
        {
            if (m_WithHeader)
            {
                if (null == m_HeaderArr)
                    ReadHeaderArr_Impl();
            }
            else
            {
                throw new InvalidOperationException("This method should not be called for a csv without header.");
            }
        }

        public Int32 GetHeaderIndex(String headerStr, Int32 occur)
        {
            Int32[] indexArr = null;
            Int32 retIndex = -1;

            ReadHeaderArr();

            if (m_HeaderIdxDic.TryGetValue(headerStr, out indexArr))
            {
                if (occur < indexArr.Length)
                    retIndex = indexArr[occur];
            }

            if (retIndex < 0)
                throw new InvalidOperationException("Fail to find given header value!");

            return retIndex;
        }

        public Int32 GetHeaderIndex(String headerStr)
        {
            return GetHeaderIndex(headerStr, 0);
        }

        public CsvRowData ReadNextRow()
        {
            String[] rowArr = null;
            CsvRowData csvRowData = null;

            ReadHeaderArr();

            rowArr = m_Reader.ReadOneLine();
            if (null != rowArr && rowArr.Length > 0)
                csvRowData = new CsvRowData(this, rowArr);

            return csvRowData;
        }

        public IEnumerator<CsvRowData> GetEnumerator()
        {
            return (new Enumerator(this));
        }

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return (new Enumerator(this));
        }

        public void Dispose()
        {
            if (null != m_Reader)
            {
                m_Reader.Dispose();
                m_Reader = null;
            }
        }

        #endregion

        #region Properties

        public String[] Headers
        {
            get
            {
                ReadHeaderArr();

                return m_HeaderArr;
            }
        }

        public Boolean WithHeader
        {
            get { return m_WithHeader; }
        }

        #endregion

        internal struct Enumerator : IEnumerator<CsvRowData>
        {
            #region Fields

            private CsvRowReader m_RowReader;
            private CsvRowData m_CurrentData;

            #endregion

            #region Constructor

            internal Enumerator(CsvRowReader rowReader)
            {
                m_RowReader = rowReader;
                m_CurrentData = null;
            }

            #endregion

            #region Methods

            public CsvRowData Current
            {
                get { return m_CurrentData; }
            }

            public void Dispose()
            { }

            object System.Collections.IEnumerator.Current
            {
                get { return m_CurrentData; }
            }

            public Boolean MoveNext()
            {
                m_CurrentData = m_RowReader.ReadNextRow();

                return (m_CurrentData != null);
            }

            public void Reset()
            {
                throw new NotImplementedException();
            }

            #endregion
        }
    }
}
